#include <EEPROM.h>

#define  STICK_X 9
#define STICK_Y 8

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
  pinMode(0, INPUT_PULLUP);
}

void loop() {
  Xstick = analogRead(STICK_X);
  Ystick = analogRead(STICK_Y);
  isCalibrationButtonPressed = !digitalRead(0);
  
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
  upperBound = 512 + (deadzone + 10);
  lowerBound = 512 - (deadzone + 10);
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

  if (calibrationButtonPressedDuration >= 5000) {
    isInCalibration = true;
    digitalWrite(13, HIGH);
  }
}

void persistBounds() {
  unsigned long now = millis();
  
  if (((calibrationWriteLastTimeStamp + now) - calibrationWriteLastTimeStamp) >= 1000) {
    int highValue = Xstick;
    
    if (Ystick > Xstick) {
      highValue = Ystick;
    }

    EEPROM.write(0, highByte(highValue));
    EEPROM.write(1, lowByte(highValue));

    calibrationWriteLastTimeStamp = now;
  }
}
