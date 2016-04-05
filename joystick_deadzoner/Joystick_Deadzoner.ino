#define  STICK_X 9
#define STICK_Y 8
#define DEADZONE 12

int Xstick;
int Ystick;
int upperBound;
int lowerBound;

void setup() {
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
  
  Keyboard.print("X: ");
  Keyboard.println(Xstick);
  Keyboard.print("Y: ");
  Keyboard.println(Ystick);
  
  delay(750);
}

