#include <EEPROM.h>

#define  STICK_X 9
#define STICK_Y 8

const int buttonPin = 0;

int Xstick;
int Ystick;
int deadzone;
int upperBound;
int lowerBound;
bool isInCalibration;
bool isCalibrationButtonPressed;
unsigned long calibrationButtonLastPressedTimeStamp;
unsigned long calibrationButtonPressedDuration;
unsigned long calibrationWriteLastTimeStamp;

void setup() {
  Joystick.useManualSend(true);
  setDeadzone();
  setBounds();
  isInCalibration = false;
  isCalibrationButtonPressed = false;
  calibrationButtonLastPressedTimeStamp = 0;
  calibrationButtonPressedDuration = 0;
  calibrationWriteLastTimeStamp = 0;

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  Xstick = 1023 - analogRead(STICK_X);
  Ystick = analogRead(STICK_Y);
  isCalibrationButtonPressed = !digitalRead(buttonPin);
  
  if (!isInCalibration) {
    checkCalibrationTrigger();
    
    if ((Xstick > 512 && Xstick <= upperBound) || (Xstick < 512 && Xstick >= lowerBound)) {
      Xstick = 512;
    }
  
    if ((Ystick > 512 && Ystick <= upperBound) || (Ystick < 512 && Ystick >= lowerBound)) {
      Ystick = 512;
    }

    Joystick.X(Xstick);
    Joystick.Y(Ystick);
    Joystick.button(1, isCalibrationButtonPressed);
    Joystick.Z(512);
    Joystick.Zrotate(512);
    Joystick.sliderLeft(0);
    Joystick.sliderRight(0);
    Joystick.hat(-1);
    Joystick.send_now();
  }
  else {
    persistBounds();
  }
}

void setDeadzone() {
  deadzone = 0;
  deadzone = EEPROM.read(0) << 8 | EEPROM.read(1);
}

void setBounds() {
  deadzone = deadzone - 512;
  upperBound = 512 + (deadzone + 20);
  lowerBound = 512 - (deadzone + 20);
}

void checkCalibrationTrigger() {
  unsigned long now = millis();
  
  if (isCalibrationButtonPressed) {
    if (calibrationButtonLastPressedTimeStamp == 0) {
      calibrationButtonLastPressedTimeStamp = now;
    }
    
    calibrationButtonPressedDuration = calibrationButtonPressedDuration + (now - calibrationButtonLastPressedTimeStamp);
    calibrationButtonLastPressedTimeStamp = now;
  }
  else {
    calibrationButtonLastPressedTimeStamp = 0;
    calibrationButtonPressedDuration = 0;
  }

  if (calibrationButtonPressedDuration >= 8000) {
    isInCalibration = true;
    digitalWrite(13, HIGH);
  }
}

void persistBounds() {
  unsigned long now = millis();
  
  if (((calibrationWriteLastTimeStamp + now) - calibrationWriteLastTimeStamp) >= 1000) {
    Xstick = abs(Xstick);
    Ystick = abs(Ystick);
    int highValue = Xstick;

    if (Ystick > Xstick) {
      highValue = Ystick;
    }

    EEPROM.write(0, highByte(highValue));
    EEPROM.write(1, lowByte(highValue));

    calibrationWriteLastTimeStamp = now;
  }
}

