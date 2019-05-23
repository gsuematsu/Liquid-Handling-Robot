
const int pinStep = 9;
const int pinDir = 8;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(pinStep, OUTPUT);
  pinMode(pinDir, OUTPUT);
  digitalWrite(pinDir, LOW);

}

// the loop function runs over and over again forever
void loop() {

  digitalWrite(pinStep, HIGH);
  delayMicroseconds(330);
  digitalWrite(pinStep, LOW);
  // delayMicroseconds(250);

}


