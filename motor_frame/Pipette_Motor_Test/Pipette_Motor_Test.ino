
const int pinStep = 13;
const int pinDir = 12;
int counter = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(pinStep, OUTPUT);
  pinMode(pinDir, OUTPUT);
  digitalWrite(pinDir, LOW);

  digitalWrite(pinDir, HIGH);
  for (int i = 0; i < 5000; i++) {
    digitalWrite(pinStep, HIGH);
    delayMicroseconds(2000);
    digitalWrite(pinStep, LOW);
    delayMicroseconds(2000);
  }
  Serial.begin(9600);



}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(pinDir, LOW);
  for (int i = 0; i < 1000; i++) {
    digitalWrite(pinStep, HIGH);
    delayMicroseconds(2000);
    digitalWrite(pinStep, LOW);
    delayMicroseconds(2000);
  }
  delay(5000);

  digitalWrite(pinDir, HIGH);
  for (int i = 0; i < 1000; i++) {
    digitalWrite(pinStep, HIGH);
    delayMicroseconds(2000);
    digitalWrite(pinStep, LOW);
    delayMicroseconds(2000);
    counter++;
    Serial.println(counter);
    delay(20);
  }
  delay(5000);
}

void dispense(int duration) {
  digitalWrite(pinDir, LOW);
  int stopWatch = millis();

  while (millis() - stopWatch < duration) {
    digitalWrite(pinStep, 1);   // turn the LED on (HIGH is the voltage level)
    delay(1);                       // wait for a second
    digitalWrite(pinStep, 0);    // turn the LED off by making the voltage LOW
    delay(1);
  }
}

void pullUp(int duration) {
  digitalWrite(pinDir, HIGH);
  int stopWatch = millis();

  while (millis() - stopWatch < duration) {
    digitalWrite(pinStep, 1);   // turn the LED on (HIGH is the voltage level)
    delay(1);                       // wait for a second
    digitalWrite(pinStep, 0);    // turn the LED off by making the voltage LOW
    delay(1);
  }
}

