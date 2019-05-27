
#include <SoftwareSerial.h> //include the software serial library


SoftwareSerial esp8266(3, 2); //set the software serial pins RX pin = 3, TX pin = 2 

//definition of variables
#define DEBUG true //show messages between ESP8266 and Arduino in serial port, when set to true
#define SSID "Colleges & Conversion" //replace x with your wifi network name
#define PASS "qweasdzxc123" //replace x with your wifi network password
int state = 5; // set intial robot state to stop

//*******************
//Function to send AT commands to esp8266
//*******************

String sendAT(String command, const int timeout, boolean debug)
{
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      char c = esp8266.read();
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}



//*****
//function to initialize arduino and esp communication
//*****
void setup()
{

  Serial.begin(9600);// begin the serial communication with baud of 9600
  esp8266.begin(9600);// begin the software serial communication with baud rate 9600
  
  sendAT("AT\r\n", 2000, DEBUG); // call sendAT function to send reset AT command
  sendAT("AT+CWMODE=1\r\n", 1000, DEBUG); //call sendAT function to set ESP8266 to station mode
  sendAT("AT+CWJAP=\""SSID"\",\""PASS"\"\r\n", 2000, DEBUG); //AT command to connect wit the wifi network

  while(!esp8266.find("OK")) { //wait for connection
  } 
  sendAT("AT+CIFSR\r\n", 1000, DEBUG); //AT command to print IP address on serial monitor
  sendAT("AT+CIPMUX=1\r\n", 1000, DEBUG); //AT command to set ESP8266 to multiple connections
  sendAT("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); // AT command to start web server on port 80
  delay(4000);
  Serial.println("initialization done");
  digitalWrite(13,HIGH); //switch "on" LED on arduino
  //Define motor speed
 
  pinMode(8,OUTPUT); // set digital pin 8,9,10,11 as output 
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  
}

/*
 * function to read data from esp8266
 */
void loop()
{
  
  if (esp8266.available())  //verify incoming data from esp8266
  {
    if (esp8266.find("+IPD,")) //if there is a message
    {
      String msg;
      esp8266.find("?"); //look for the message
      msg = esp8266.readStringUntil(' '); //read whole message
      String command = msg.substring(0, 3); //first 3 characters = command
      String valueStr = msg.substring(4);   //next 3 characters = value
      int value = valueStr.toInt(); // convert string value to integer
      if (DEBUG) {
        
        Serial.println(msg);
        Serial.println(command); //print command on serial monitor
        Serial.println(value); //print value on serial monitor
      }
           
      //move forward
      if(command == "cm1") {
          state = 1;
          Serial.println("forward");
      }

      //move backward
      if(command == "cm2") {
          state = 2;
          Serial.println("back");
      }

      //turn right
      if(command == "cm3") {
          state = 3;
          Serial.println("right");
       }
       
       //turn left
       if(command == "cm4") {
          state = 4;
          Serial.println("left");
       }

       //do nothing       
       if(command == "cm5") {
          state = 5;
          Serial.println("stop");
       }

       //test connection       
       if(command == "cm6") {
          state = 6;
          Serial.println("connected to client");
           Serial.println("now use arrow keys to control your robot");
       }

    }
  }
  
  //move forward
  if (state == 1) {
    digitalWrite(9,HIGH);
    digitalWrite(11,HIGH);    
  }
  //move backward
  if (state == 2) {
    digitalWrite(8,HIGH);
    digitalWrite(10,HIGH);    
  }
  //move right
  if (state == 3) {
     digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
      
  }
  //move left
  if (state == 4) {
    
    digitalWrite(8,HIGH);
    digitalWrite(11,HIGH); 
       
  }
  //stop
  if (state == 5) {
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
  }

  if (state == 6) {
    
  }
}


