import processing.serial.*;
import controlP5.*;

ControlP5 cp5;

float data[] = {100.32, 403.2, 90};
String commandString;
String transmissionString = "NO";


Serial myPort = new Serial(this, Serial.list()[2], 115200);

void setup(){
  size(860, 480);
  background(0);
  
  cp5 = new ControlP5(this);
  
  cp5.addButton("red")
     .setValue(0)
     .setPosition(100,100)
     .setSize(200,19)
     ;
  
  cp5.addButton("blue")
     .setValue(100)
     .setPosition(100,120)
     .setSize(200,19)
     ;
     
  cp5.addButton("green")
     .setPosition(100,140)
     .setSize(200,19)
     .setValue(0)
     ;
}

void draw(){
  myPort.write(transmissionString);
  print(transmissionString);
  transmissionString = "NO\n";
}

String convertData2Str(float[] data){
  String dataString = "";
  
  for(int i = 0; i < data.length; i++){
    if(i != 0) dataString += ",";
    
    dataString += str(data[i]);
  }
  
  return dataString;
}

String createTransString(String command, float[] parameters){
  String transmissionStr = command;
  
  for(int i = 0; i < parameters.length; i++){
    transmissionStr += ",";
    transmissionStr += str(parameters[i]);
  }
    
  transmissionStr += "\n";
  
  return transmissionStr;
}

public void red() {
  commandString = "RE";
  data[0] = 200;
  data[1] = 808.32;
  data[2] = 9;
  transmissionString = createTransString(commandString, data);
}

public void blue() {
  commandString = "BL";
  data[0] = 95.5;
  data[1] = 1000;
  data[2] = 29.6;
  transmissionString = createTransString(commandString, data);
}

public void green() {
  commandString = "GR";
  data[0] = 106.7;
  data[1] = 8.12;
  data[2] = 2000;
  transmissionString = createTransString(commandString, data);
}