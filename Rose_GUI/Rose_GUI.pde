import controlP5.*;
import processing.serial.*;

int SHAPEKO_XMAXMM = 173;
int SHAPEKO_YMAXMM = 158;
int SHAPEKO_ZMAXMM = 65;
float MAX_WELL_VOLUME = 360;

String COMMAND_DO_NOTHING = "NO";
String COMMAND_GO_TO = "GT";
String COMMAND_PIPETTE_UP = "PU";
String COMMAND_PIPETTE_DOWN = "PD";
String COMMAND_SERIAL_DILUTION = "SD";

String transmissionString = "";
String commandString = "NO";
float data[] = {0, 0, 0, 0};

Serial myPort = new Serial(this, Serial.list()[2], 115200);

boolean isSendingCommand = false;

float dest[] = {0, 0, 0};
float dilutionFactor = 2.0;
float finalVolume = 100;
float pipetteVol = 10;

PImage rose;

ControlP5 cp5;

Button btnGoTo;
Slider2D sld2dPos;
Slider sldrZPos;
Textfield txtX;
Textfield txtY;
Textfield txtZ;
Textlabel lblDestinationError;

Button btnStartDilution;
Textfield txtDilutionFactor;
Textfield txtFinalVolume;
Textlabel lblDilutionFactorError;
Textlabel lblFinalVolumeError;

Button btnPipetteUp;
Button btnPipetteDown;
Textfield txtPipetteVol;
Textlabel lblPipetteError;

void setup(){
   size(860, 480);
   background(0);
   
   rose = loadImage("Logo.png");
   
   cp5 = new ControlP5(this);
   
   /////////////////////////////////////////////////////////
   //--------------- Go To Objects -----------------------//
   /////////////////////////////////////////////////////////
   btnGoTo = cp5.addButton("btnGoTo")
     .setLabel("go to")
     .setPosition(295,180)
     .setSize(80,40)
     .setFont(createFont("arial", 15))
     ; 
     
   sld2dPos = cp5.addSlider2D("xy destination")
     .setPosition(30,40)
     .setSize(SHAPEKO_XMAXMM,SHAPEKO_YMAXMM)
     .setMinMax(0,0,SHAPEKO_XMAXMM,SHAPEKO_YMAXMM)
     .setValue(0,0)
     ;
     
   sldrZPos = cp5.addSlider("z destination")
     .setPosition(230, 40)
     .setSize(20, 150)
     .setRange(SHAPEKO_ZMAXMM,0)
     ;
       
   txtX = cp5.addTextfield("x")
     .setPosition(310,35)
     .setSize(50,20)
     .setFont(createFont("arial", 15))
     .setFocus(true)
     .setColor(255)
     .setValue("0.00")
     ;
   txtY = cp5.addTextfield("y")
     .setPosition(310,85)
     .setSize(50,20)
     .setFont(createFont("arial", 15))
     .setFocus(true)
     .setColor(255)
     .setValue("0.00")
     ;
   txtZ = cp5.addTextfield("z")
     .setPosition(310, 135)
     .setSize(50,20)
     .setFont(createFont("arial", 15))
     .setFocus(true)
     .setColor(255)
     .setValue("0.00")
     ;
     
   cp5.addTextlabel("lblX")
      .setText("mm")
      .setPosition(360,35)
      .setFont(createFont("arial",15))
      ;
      
   cp5.addTextlabel("lblY")
      .setText("mm")
      .setPosition(360,85)
      .setFont(createFont("arial",15))
      ;
   cp5.addTextlabel("lblZ")
      .setText("mm")
      .setPosition(360,135)
      .setFont(createFont("arial",15))
      ;
     
   sld2dPos.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_RELEASE || theEvent.getAction() == ControlP5.ACTION_RELEASE_OUTSIDE) {
        String str[] = {nfc(sld2dPos.getArrayValue()[0], 2),  nfc(sld2dPos.getArrayValue()[1], 2)};
        dest[0] = float(str[0]);
        dest[1] = float(str[1]);
        
        cp5.get(Textfield.class,"x").setValue(str[0]);
        cp5.get(Textfield.class,"y").setValue(str[1]);
      }
    }
  }
  );
  
  sldrZPos.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_RELEASE || theEvent.getAction() == ControlP5.ACTION_RELEASE_OUTSIDE) {
        String str = nfc(sldrZPos.getValue(), 2);
        dest[2] = float(str);
        
        cp5.get(Textfield.class,"z").setValue(str);
      }
    }
  }
  );
  
  txtX.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_CLICK) {
         cp5.get(Textfield.class,"x").clear();
      }
    }
  }
  );
  
  txtY.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_CLICK) {
         cp5.get(Textfield.class,"y").clear();
      }
    }
  }
  );
  
  txtZ.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_CLICK) {
         cp5.get(Textfield.class,"z").clear();
      }
    }
  }
  );
  
  txtX.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_EXIT) {
        if(txtX.getText().length() == 0) txtX.setValue(str(dest[0]));

        dest[0] = float(txtX.getText());
        cp5.get(Slider2D.class,"xy destination").setValue(dest[0], dest[1]);
      }
    }
  }
  );
  
  txtY.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_EXIT) {
        if(txtY.getText().length() == 0) txtY.setValue(str(dest[1]));
        
        dest[1] = float(txtY.getText());
        cp5.get(Slider2D.class,"xy destination").setValue(dest[0], dest[1]);
      }
    }
  }
  );
  
  txtZ.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_EXIT) {
        if(txtZ.getText().length() == 0) txtZ.setValue(str(dest[2]));
        
        dest[2] = float(txtZ.getText());
        cp5.get(Slider.class,"z destination").setValue(dest[2]);
      }
    }
  }
  );
  
  lblDestinationError = cp5.addTextlabel("lblDestinationError")
      .setText("Destination has been set outside of Rose's range.\nPlease input valid coordinates.")
      .setPosition(10,5)
      .setFont(createFont("arial",15))
      .setColorValue(#ff0000)
      .hide()
      ;  
  
  
  /////////////////////////////////////////////////////////
  //-------- Serial Dilution Objects --------------------//
  /////////////////////////////////////////////////////////
  btnStartDilution = cp5.addButton("btnStartDilution")
     .setLabel("start dilution")
     .setPosition(70,400)
     .setSize(160,40)
     .setFont(createFont("arial", 18))
     ; 
  
  txtDilutionFactor = cp5.addTextfield("dilution factor")
     .setPosition(70,290)
     .setSize(50,20)
     .setFont(createFont("arial", 15))
     .setFocus(true)
     .setColor(255)
     .setValue("2")
     ;
     
  txtFinalVolume = cp5.addTextfield("final well volume")
     .setPosition(70,340)
     .setSize(50,20)
     .setFont(createFont("arial", 15))
     .setFocus(true)
     .setColor(255)
     .setValue("100")
     ;
     
  cp5.addTextlabel("lblFinalVolume")
      .setText("uL")
      .setPosition(120,340)
      .setFont(createFont("arial",15))
      ;
  
  txtDilutionFactor.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_CLICK) {
         cp5.get(Textfield.class,"dilution factor").clear();
      }
    }
  }
  );
  
  txtFinalVolume.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_CLICK) {
         cp5.get(Textfield.class,"final well volume").clear();
      }
    }
  }
  );
  
  txtDilutionFactor.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_EXIT) {
        if(txtDilutionFactor.getText().length() == 0) txtDilutionFactor.setValue(str(dilutionFactor));
        
        dilutionFactor = float(txtDilutionFactor.getText());
      }
    }
  }
  );
  
  txtFinalVolume.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_EXIT) {
        if(txtFinalVolume.getText().length() == 0) txtFinalVolume.setValue(str(finalVolume));
        
        finalVolume = float(txtFinalVolume.getText());
      }
    }
  }
  );
  
  lblDilutionFactorError = cp5.addTextlabel("lblDilutionFactorError")
      .setText("Dilution factor must be a positive number.\nPlease input valid dilution factor.")
      .setPosition(10,250)
      .setFont(createFont("arial",15))
      .setColorValue(#ff0000)
      .hide()
      ;  
      
  lblFinalVolumeError = cp5.addTextlabel("lblFinalVolumeError")
      .setText("Final volume is too large for well.\nPlease input valid volume.")
      .setPosition(10,250)
      .setFont(createFont("arial",15))
      .setColorValue(#ff0000)
      .hide()
      ; 
  
  
  /////////////////////////////////////////////////////////
  //---------------- Pipette Objects --------------------//
  /////////////////////////////////////////////////////////
   btnPipetteUp = cp5.addButton("btnPipetteUp")
     .setLabel("pipette up")
     .setPosition(310,290)
     .setSize(110,40)
     .setFont(createFont("arial", 12))
     ; 
     
   btnPipetteDown = cp5.addButton("btnPipetteDown")
     .setLabel("pipette down")
     .setPosition(310, 400)
     .setSize(110,40)
     .setFont(createFont("arial", 12))
     ; 
  
   txtPipetteVol = cp5.addTextfield("pipette vol")
     .setPosition(315, 350)
     .setSize(50,20)
     .setFont(createFont("arial", 15))
     .setFocus(true)
     .setColor(255)
     .setValue("10")
     ;
     
   cp5.addTextlabel("lblPipetteVolume")
      .setText("uL")
      .setPosition(365,350)
      .setFont(createFont("arial",15))
      ;
  
  lblPipetteError = cp5.addTextlabel("lblPipetteError")
      .setText("Volume must be\nnumber 0-200")
      .setPosition(310,250)
      .setFont(createFont("arial",15))
      .setColorValue(#ff0000)
      .hide()
      ; 
      
  txtPipetteVol.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_CLICK) {
         cp5.get(Textfield.class,"pipette vol").clear();
      }
    }
  }
  );
  
  txtPipetteVol.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction() == ControlP5.ACTION_EXIT) {
        if(txtPipetteVol.getText().length() == 0) txtPipetteVol.setValue(str(pipetteVol));
        
        pipetteVol = float(txtPipetteVol.getText());
      }
    }
  }
  );
}

void draw(){
  background(0);
  image(rose, 9*width/16, 0, 3*width/8, height);
  // Draw grid
  drawSections();
   
  transmissionString = createTransString(commandString, data); 
  
  myPort.write(transmissionString);
  print(transmissionString);
  commandString = "NO";
  for(int i = 0; i < data.length; i++)
    data[i] = 0;
}

void drawSections(){
  stroke(255);
  line(0, height/2, width/2, height/2);
  line(width/2, 0, width/2, height);
  line(300, height/2, 300, height);
}

public void btnGoTo() {
  if (dest[0] > SHAPEKO_XMAXMM || dest[1] > SHAPEKO_YMAXMM || dest[2] > SHAPEKO_ZMAXMM)
    cp5.get(Textlabel.class,"lblDestinationError").show();
  else {
    cp5.get(Textlabel.class,"lblDestinationError").hide();
    
    commandString = COMMAND_GO_TO;
    for(int i = 0; i < dest.length; i++)
      data[i] = dest[i];
    
    print("Going to "); print(dest[0]); print(", ");
    print(dest[1]); print(", "); println(dest[2]);

  } 
}

public void btnStartDilution() {
  if (dilutionFactor < 1){
    cp5.get(Textlabel.class,"lblDilutionFactorError").show();
    cp5.get(Textlabel.class,"lblFinalVolumeError").hide();
  } else {
    cp5.get(Textlabel.class,"lblDilutionFactorError").hide();
    
    float[] dilutantVol = new float[12];
    dilutantVol[11] = finalVolume - finalVolume / dilutionFactor;
    
    for (int i = dilutantVol.length - 2; i >= 0; i--)
    dilutantVol[i] = (finalVolume + dilutantVol[i + 1]) * (dilutionFactor - 1) / dilutionFactor;


    if (dilutantVol[0]*dilutionFactor > MAX_WELL_VOLUME) 
      cp5.get(Textlabel.class,"lblFinalVolumeError").show();
    else{
      cp5.get(Textlabel.class,"lblFinalVolumeError").hide();
      
      commandString = COMMAND_SERIAL_DILUTION;
      data[0] = finalVolume;
      data[1] = dilutionFactor;
      
      println("Dilution has begun.");
      print("DF = "); print(dilutionFactor); print(", Vol = "); println(finalVolume);

    }   
  }
}

public void btnPipetteUp() {
  if (pipetteVol > 200 || pipetteVol < 0)
    cp5.get(Textlabel.class,"lblPipetteError").show();
  else {
    cp5.get(Textlabel.class,"lblPipetteError").hide();
    
    commandString = COMMAND_PIPETTE_UP;
    data[0] = pipetteVol;
    
    print("Pipetting up "); print(pipetteVol); println(" uL.");

  }

}

public void btnPipetteDown() {
  if (pipetteVol > 200 || pipetteVol < 0)
    cp5.get(Textlabel.class,"lblPipetteError").show();
  else {
    cp5.get(Textlabel.class,"lblPipetteError").hide();
    
    commandString = COMMAND_PIPETTE_DOWN;
    data[0] = pipetteVol;
    
    print("Pipetting down "); print(pipetteVol); println(" uL.");

  }
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