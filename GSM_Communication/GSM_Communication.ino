#include <SoftwareSerial.h>

#define LED         13

void updateSerial(void);
void sendSMS(void);

/* Software serial object to communicate with SIM900 */
SoftwareSerial gsmSerial(7, 8);  //SIM900 (Tx->7 & Rx->8)

const String STM_message = "GSM";
String received_STM_message = "";
String phoneNumber = "";

void setup(void)
{
  /* Begin serial communication with Arduino (Serial Monitor) */
  Serial.begin(9600);

  /* Begin serial communication with SIM900 */
  gsmSerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  gsmSerial.println("AT+CMGF=1");  //Set SIM900 to SMS mode
  updateSerial();

  gsmSerial.println("AT+CNMI=2,2,0,0,0");  //Set SIM900 to send SMS data to serial port 
  updateSerial();
}

void loop(void)
{
  while (Serial.available() > 0)
  {
      received_STM_message = Serial.readString();

      if (received_STM_message == STM_message)
      {
        sendSMS();
        received_STM_message = "";
      }
      else 
      {
        phoneNumber = received_STM_message;
        Serial.println(phoneNumber);
        received_STM_message = "";
      }
  }
}


void updateSerial(void)
{
  delay(500);
  
  while (Serial.available()) 
  {
    gsmSerial.write(Serial.read()); //Forward what Serial received to Software Serial Port
  }
  
  while(gsmSerial.available()) 
  {
    Serial.write(gsmSerial.read()); //Forward what Software Serial received to Serial Port
  }
}

void sendSMS(void)
{
  gsmSerial.println("AT");
  updateSerial();

  gsmSerial.println("AT+CMGF=1"); //Configuring TEXT mode
  updateSerial();
  
  gsmSerial.print("AT+CMGS=\"+385");
  gsmSerial.print(phoneNumber);
  gsmSerial.println("\"");
  updateSerial();
  
  gsmSerial.print("ALARM ACTIVATED - ALERT!!!!"); //Text content
  updateSerial();
  
  gsmSerial.write(26);  //Serial1.write(0x1A);
}
