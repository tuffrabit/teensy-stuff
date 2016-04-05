#define  STICK_X 9
#define STICK_Y 8
#define DEADZONE 12

int Xstick;
int Ystick;
int upperBound;
int lowerBound;

void setup() {
  pinMode(0, INPUT_PULLUP);
  
  upperBound = 512 + DEADZONE;
  lowerBound = 512 - DEADZONE;
}

void loop() {
  Xstick = analogRead(STICK_X);
  Ystick = analogRead(STICK_Y);

  if ((Xstick > 512 && Xstick <= upperBound) || (Xstick < 512 && Xstick >= lowerBound)) {
    Xstick = 512;
  }

  if ((Ystick > 512 && Ystick <= upperBound) || (Ystick < 512 && Ystick >= lowerBound)) {
    Ystick = 512;
  }

  Joystick.X(Xstick);
  Joystick.Y(Ystick);
  Joystick.button(1, !digitalRead(0));
}
