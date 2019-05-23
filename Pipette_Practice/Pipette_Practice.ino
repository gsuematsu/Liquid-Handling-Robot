const int pinsPipette[] = {2, 4, 7, 8, 12};
  // Enter:   0  e
  // Pipette: 1  p
  // Up:      2  +
  // Down:    3  -
  // Menu:    4  m
  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(int i = 0; i < sizeof(pinsPipette)/sizeof(int); i++){
    pinMode(pinsPipette[i], OUTPUT);
    digitalWrite(pinsPipette[i], LOW);
  }

  pressButton('p');
  
}

void loop() {
  // looping code
}


/**
 * Press Button: 
 * Indicate using the char key which button you want to press. HIGH is 
 * written to the pin associated with that button. Mappings for the chars:
 * Menu     ->  'm'
 * Enter    ->  'e'
 * Pipette  ->  'p'
 * Up       ->  '+'
 * Down     ->  '-'
 */
void pressButton(char b) {
  
  switch(b) {
    // Menu 
    case 'm':
      digitalWriteOnOff(4);
      Serial.println("Menu");
      break;
    // Enter
    case 'e':
      digitalWriteOnOff(0);
      Serial.println("Enter");
      break;
    // Pipette
    case 'p':
      digitalWriteOnOff(1);
      Serial.println("Pipette");
      break;
    // Up
    case '+':
      digitalWriteOnOff(2);
      Serial.println("Up");
      break;
    // Down
    case '-':
      digitalWriteOnOff(3);
      Serial.println("Down");
      break;
    // Default
    default:
      Serial.println("unknown command");
  }
}

/**
 * Digital Write On/Off:
 * Writes HIGH to pin p, delays for 500 ms, then writes LOW
 */
void digitalWriteOnOff(int p) {
  digitalWrite(pinsPipette[p], HIGH);
  delay(500);
  digitalWrite(pinsPipette[p], LOW);
}

// ====================================================================================


void firstTestFunc() {
    // put your main code here, to run repeatedly:
  for(int i = 0; i < sizeof(pinsPipette)/sizeof(int); i++){
    pinMode(pinsPipette[i], OUTPUT);
    digitalWrite(pinsPipette[i], LOW);
  }
  delay(1000);

  for(int i = 0; i < sizeof(pinsPipette)/sizeof(int); i++){
    pinMode(pinsPipette[i], OUTPUT);
    digitalWrite(pinsPipette[i], HIGH);
  }
  delay(1000);
}



