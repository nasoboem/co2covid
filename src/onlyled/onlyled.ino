#include <Arduino.h>

#define RXD2 16
#define TXD2 17



long lastMsg = 0;
char msg[50];
int value = 0;

byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};  // get gas command
byte cmdCal[9] = {0xFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78};  // calibrate command
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
	//Serial2.begin(9600,SERIAL_8N1,9,10);  // Setup a serial connection with the sensor
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.begin(115200);
	
  
  
	 warmingTimer = millis();  // initilize warmup timer
   while (millis() - warmingTimer < 120000)
	 {
         ledscroll();
	 }
  calibrate();
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

} // loop

void getReadings()
{
	while (Serial2.available())  // this clears out any garbage in the RX buffer
	{
		int garbage = Serial2.read();
	}

	Serial2.write(cmd, 9);  // Sent out read command to the sensor
	Serial2.flush();  // this pauses the sketch and waits for the TX buffer to send all its data to the sensor

	while (!Serial2.available())  // this pauses the sketch and waiting for the sensor responce
	{
		delay(0);
	}

	Serial2.readBytes(response, 9);  // once data is avilable, it reads it to a variable
	int responseHigh = (int)response[2];
	int responseLow = (int)response[3];
	CO2ppm = (256 * responseHigh) + responseLow;
    // CO2ppm = analogRead(34);
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

void ledscroll(){
   for (int index = 0; index < si; index++)
  {
    digitalWrite(ledPins[index], HIGH);
    delay(50);
  }
  for (int index = si - 1; index >= 0; index--)
  {
    digitalWrite(ledPins[index], LOW);
    delay(50);
  }
}
