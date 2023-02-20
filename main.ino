#include <LiquidCrystal_I2C.h>  // Include the lcd library
#include <SoftwareSerial.h> // Library for using serial communication
LiquidCrystal_I2C lcd(0x27,20,4); 

SoftwareSerial gsm(8, 9);    // Pins 8 (RX), 9 (TX) are used as used as software serial pins

String incomingData = "";     // for storing incoming serial data
unsigned short lightOut = 13; // Initialized a pin for relay module
unsigned short fanOut = 12;
unsigned short tvOut = 8;
int input = A0;
int buzzer = 10;
int inputstate;
float varr = 0;
float volt = 0;
int parr;
void setup() {

  Serial.begin(9600); // baudrate for serial monitor
  gsm.begin(9600);    // baudrate for GSM shield
  lcd.init();                      // initialize the lcd 

  pinMode(lightOut, OUTPUT);
  pinMode(fanOut, OUTPUT);
  pinMode(tvOut, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(buzzer, OUTPUT);


  
 digitalWrite(buzzer, LOW);
  digitalWrite(lightOut, LOW);
  digitalWrite(fanOut, LOW);
  digitalWrite(tvOut, LOW);
 
  
  lcd.print("GSM Control Home");
  lcd.setCursor(0, 1);
  lcd.print("   Automation   ");
  delay(2000);
  showInitializingMsg();
  lcd.clear();
  lcd.print("System Ready");
  delay(1000);
  gsm.println("AT+CMGF=1");
  delay(200);
  
  sendMessage("System Ready");
  
  gsm.println("AT+CNMI=2,2,0,0,0");
  delay(200);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Welcome     ");
  
}

void loop() {

varr = analogRead(A0);

 volt = (varr * 350 )/1023;

 lcd.setCursor(0, 0);
 lcd.print("    Welcome     ");
 lcd.setCursor(0, 1);
 lcd.print("Volt = ");
 lcd.print(volt);
  lcd.print("V                 ");
delay(500);


  if (volt >= 280 ){
 digitalWrite(buzzer,HIGH);
  lcd.setCursor(0, 1);
 lcd.print("High Voltage Detect");  
 delay(2000);
 sendMessage("High Voltage Detect");
  }

if (volt <= 180 ){
 digitalWrite(buzzer,HIGH);
  lcd.setCursor(0, 1);
 lcd.print("Low Voltage Detect");  
 delay(2000);
 sendMessage("Low Voltage Detect");
  }


}

/*
   Function for loading animation
*/
void showInitializingMsg() {
  String msg = "Initializing";
  // Here i'm using 16x2 display. So, characters in every line is 16
  unsigned int restOfCellInLine = (16 - msg.length());
  lcd.clear();
  lcd.print(msg);

  for (int i = 0; i < restOfCellInLine; i++) {
    for (int j = 0; j < restOfCellInLine; j++) {
      lcd.print(".");
      delay(1000);
    }
    lcd.clear();
    lcd.print(msg);
  }
}

void sendMessage(String msg) {
  lcd.setCursor(0, 1);
 lcd.print("SMS Sending ..........."); 
  gsm.println("AT+CMGF=1");    // Set the GSM Module in Text Mode
  delay(1000);
  gsm.println("AT+CMGS=\"+8801920639338\""); // Replace it with your mobile number
  delay(1000);
  gsm.println(msg);       // The SMS text you want to send
  delay(1000);
  gsm.println((char)26);  // ASCII code of CTRL+Z
  delay(1000);
  gsm.println();
  delay(1000);
}






/*
   Function for receiving sms

void receiveSMS() {
  while (gsm.available()) {
    incomingData = gsm.readStringUntil('\0'); // Get the data from the serial port.
    incomingData.toLowerCase();
    Serial.print(incomingData);
  }
}
*/
/*
   Function for sending sms
*/
