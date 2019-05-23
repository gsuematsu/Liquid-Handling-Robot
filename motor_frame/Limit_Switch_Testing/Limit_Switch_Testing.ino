const int pinLimitSwitchXMin = A0;
const int pinLimitSwitchYMin = A1;
const int pinLimitSwitchZMin = A2;
const int pinLimitSwitchXMax = A3;
const int pinLimitSwitchYMax = A4;
const int pinLimitSwitchZMax = A5;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinLimitSwitchXMin, INPUT_PULLUP);
  pinMode(pinLimitSwitchYMin, INPUT_PULLUP);
  pinMode(pinLimitSwitchZMin, INPUT_PULLUP);
  pinMode(pinLimitSwitchXMax, INPUT_PULLUP);
  pinMode(pinLimitSwitchYMax, INPUT_PULLUP);
  pinMode(pinLimitSwitchZMax, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(analogRead(pinLimitSwitchXMin)); Serial.print(' ');
  Serial.print(analogRead(pinLimitSwitchYMin)); Serial.print(' ');
  Serial.print(analogRead(pinLimitSwitchZMin)); Serial.print(' ');
  Serial.print(analogRead(pinLimitSwitchXMax)); Serial.print(' ');
  Serial.print(analogRead(pinLimitSwitchYMax)); Serial.print(' ');
  Serial.println(analogRead(pinLimitSwitchZMax));
}
