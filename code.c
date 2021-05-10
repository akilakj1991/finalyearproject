#include <DS1307new.h>
#include <TinyGPS.h>
#include <Wire.h>

#define pinPowerSIM900 9
#defien ID 102

TinyGPS gps;

unsigned char buffer[64];
int count = 0;

uint16_t startAddr = 0x0000;
uint16_t lastAddr;
uint16_t TimeIsSet = 0xaa55;

int currentDay = 0;
int currentMonth = 0;
int currentYear = 0;
int currentHour = 0;
int currentMinute = 0;

float flat, flon;
float previousFLat = 0.0;
float previousFLon = 0.0;

void setup() {
  pinMode(pinPowerSIM900, OUTPUT); digitalWrite(pinPowerSIM900, LOW);

  powerUpOrDown();

  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  // put your setup code here, to run once:
  RTC.getRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
  if (TimeIsSet != 0xaa55){
    RTC.stopClock();
    RTC.fillByYMD(2016,9,5);
    RTC.fillByHMS(10,21,30);

    RTC.setTime();
    TimeIsSet = 0xaa55;
    RTC.setRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
    RTC.startClock();
  }else{
    RTC.getTime();
  }
  RTC.ctrl = 0x00;  // 0x00 = diable sqw pin, 0x10 = 1Hz
                    // 0x11 = 4096Hz, 0x12=819Hz
  RTC.setCTRL();
  delay(5000);
}
void loop() {
  RTC.getTime();
  currentDay = RTC.day;
  currentMonth = RTC.month;
  currentYear = RTC.year;
  currentHour = RTC.hour;
  currentMinute = RTC.minute;

  bool newData = false;

  for(unsigned long start = millis();millis() - start <1000;){
    while (Serial1.available()){
      char c = Serial1.read();
      if (gps.encode(c))
        newData = true;
    }
  }
  if (newData){
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    if((flat != previousFLat)||(flon != previousFLon)){
      previousFLat = flat;
      previousFLon = flon;
      sendSQL();  
    }
  }
}
void powerUpOrDown(){
  digitalWrite(pinPowerSIM900, HIGH);
  delay(2000);
  digitalWrite(pinPowerSIM900, LOW);
  delay(3000);
}
void sendSQL(){
	Serial2.println("AT+CSQ"); // Signal quality check
	delay(100);
	Serial2.println("AT+CGATT?"); //Attach or Detach from GPRS Support
	delay(100);
	Serial2.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");//setting the SAPBR, the connection type is using gprs
	delay(1000);
	Serial2.println("AT+SAPBR=3,1,\"APN\",\"internet\"");//setting the APN, Access point name string
	delay(4000);
	Serial2.println("AT+SAPBR=1,1");//setting the SAPBR
	delay(2000);
	Serial2.println("AT+SAPBR=2,1");//setting the SAPBR
	delay(2000);
	Serial2.println("AT+HTTPINIT"); //init the HTTP request
	delay(2000); 
	Serial2.println("AT+HTTPARA=\"CID\",1");
	delay(3000);
	Serial2.print("AT+HTTPPARA=\"URL\",\"122.57.2.189:8888/update_device_loc.php?");
	Serial2.print("device_id=");
	Serial2.print(ID);
	Serial2.print("&lat=");
	Serial2.print(previousFLat, 8);
	Serial2.print("&lng=");
	Serial2.print(previousFLon, 8);
	Serial2.print("&time_date");
	Serial2.print(currentYear);
	Serial2.print("-");
	if (currentMonth < 10) { Serial2.print("0");Serial2.print(currentMonth);}
	else { Serial2.print(currentMonth);}
	Serial2.print("-");
	if (currentDay < 10) { Serial2.print("0");Serial2.print(currentDay);}
	else { Serial2.print(currentDay);}
	Serial2.print(" ");
	if (currentHour < 10) { Serial2.print("0");Serial2.print(currentHour);}
	else { Serial2.print(currentHour);}
	Serial2.print(":");
	if (currentMinute < 10) { Serial2.print("0");Serial2.print(currentMinute);}
	else { Serial2.print(currentMinute);}
	Serial2.print("\"");
	delay(6000);
	Serial2.println("AT+HTTPACTION=0");//submit the request 
	delay(15000);//the delay is very important, the delay time is base on the return from the website, if the return datas are very large, the time required longer.
	Serial2.println("AT+HTTPTERM");
	delay(500);
	Serial2.println("AT+SAPBR=0,1");
	Serial2.println("AT+HTTPREAD");// read the data from the website you access
	delay(3000);
	Serial2.println("");
	delay(3000);
}