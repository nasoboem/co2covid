#include <Arduino.h>

#define RXD2 16 // connect to TXD on MH-19
#define TXD2 17 // connect ro RXD on MH-19



long lastMsg = 0;
char msg[50];
int value = 0;
unsigned long tim = 0;
bool pressed = false;

byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};  // get gas command
byte cmdCal[9] = {0xFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78};  // calibrate command
byte autoCal[9] = {0xFF, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xE6};  // enable autocalibration
char response[9];  // holds the recieved data

int CO2ppm = 0;

unsigned long warmingTimer = 0;
unsigned long previousMillis = 0;

int ledPins[] = {12, 14, 27, 26, 25, 33};
int si = sizeof(ledPins);

void setup()
{
  for (int index = 0; index < si; index++)
    {
        pinMode(ledPins[index], OUTPUT);
        digitalWrite(ledPins[index], HIGH);
    }
	
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Setup a serial connection with the sensor
  Serial.begin(115200);
	
  
  //Serial2.write(autoCal,9); // activate to enable autocalibration
  
	 warmingTimer = millis();  // initilize warmup timer
   while (millis() - warmingTimer < 0000)
	 {
         ledscroll();
	 }
	//calibrate(); // activate to calibrate at startup
  pinMode(0, INPUT);
  

  
}

void loop()
{
  //reconnect();
  unsigned long currentMillis = millis();
	if ((unsigned long)(currentMillis - previousMillis) > 15000) {  // runs every 15 sec
	{
		getReadings();
		previousMillis = currentMillis;
	}
  }

  // check boot button to calibrate
  if (digitalRead(0) == 0 && tim == 0)
  {
    tim = millis();
    pressed = true;
  }
  else
  {
    pressed = false;
  }
  
  if (currentMillis - tim > 5000 && pressed == true)
  {
    //calibrate();
    blink(20);
  }
  Serial.println(digitalRead(0));

} // loop

void getReadings()
{
	while (Serial2.available())  //clears recieve buffer
	{
		int garbage = Serial2.read();
	}

	Serial2.write(cmd, 9);  // read sensor
	Serial2.flush(); 

	while (!Serial2.available())  // wait for sensor response
	{
		delay(0);
	}

	Serial2.readBytes(response, 9);  // read responce once available
	int responseHigh = (int)response[2];
	int responseLow = (int)response[3];
	CO2ppm = (256 * responseHigh) + responseLow;
  // CO2ppm = analogRead(34); // dummy input to test without sensor
  String CO2String = String(CO2ppm);
  Serial.print("CO2: ");
	Serial.println(CO2String);
  char message_buff[8];
  CO2String.toCharArray(message_buff, CO2String.length()+1); 
    
  led(CO2ppm);
}

void calibrate()
{
	Serial2.write(cmdCal, 9);
	delay(3000);
}

// translate CO2 level to LEDs
void led(int CO2) {

    int lednum = 0;

    if(CO2 > 1100){lednum = 6;}
    else if(CO2 > 1000){lednum = 5;}
    else if(CO2 > 900){lednum = 4;}
    else if(CO2 > 800){lednum = 3;}
    else if(CO2 > 600){lednum = 2;}
    else{lednum = 1;}
    Serial.println(lednum);

    for (int index = 0; index < lednum; index++)
    {
      digitalWrite(ledPins[index], HIGH);
    }

    for (int index = lednum; index < si; index++)
    {
      digitalWrite(ledPins[index], LOW);
    }
  }

// animation for warmup
void ledscroll(){
   for (int index = 0; index <= si; index++)
  {
    if (index < si){
      digitalWrite(ledPins[index], HIGH);
    }
    if (index > 0) {
      digitalWrite(ledPins[index-1], LOW);
    }
    delay(100);
  }
}

void blink(int a)
{
  for (int i = 0; i < a*60; i++)
  {
    for (int index = 0; index < si; index++)
    {
      digitalWrite(ledPins[index], HIGH);
    }
    delay(500);
    for (int index = 0; index < si; index++)
    {
      digitalWrite(ledPins[index], LOW);
    }
    delay(500);
  }
  
}
