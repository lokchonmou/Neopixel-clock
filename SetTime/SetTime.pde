import processing.serial.*;


public static final short portIndex = 1;  
public static final String TIME_HEADER = "T"; 

Serial myPort;     

void setup() {  
  size(200, 200);
  println(Serial.list());
  println(" Connecting to -> " + Serial.list()[portIndex]);
  myPort = new Serial(this, Serial.list()[portIndex], 9600);
}

void draw()
{
  textSize(20);
  textAlign(CENTER);
  fill(0);
  text("Click to send\nTime Sync", 0, 75, 200, 175);
  if ( myPort.available() > 0) {  // If data is available,
    char val = char(myPort.read());         // read it and store it in val
      print(val); // echo everying but time request 
  }
}

void mousePressed() {  
  sendTimeMessage(TIME_HEADER);
}


void sendTimeMessage(String header) {  
  String timeStr = str(hour())+','+str(minute())+','+str(second());  
  myPort.write(header); 
  myPort.write(timeStr); 
  myPort.write('\n');  
  println(header, timeStr);
}
