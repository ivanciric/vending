#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <AFMotor.h>
hd44780_I2Cexp lcd;

const int LCD_COLS = 16;
const int LCD_ROWS = 2;
const int AOUTpin=4; 
const int DOUTpin=5; 
const int ledPin=13;
const int RED = 14;
const int WHITE = 15;
const int GREEN = 16;
int statusLedPin;

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b10001,
  0b01110,
  0b00000,
  0b00000
};

String incomingCommand = "";

AF_Stepper motor(200, 2);

void setup() {             
  Serial.setTimeout(50);
  Serial.begin(9600);   
  Serial.print("Ready.");
  
  pinMode(DOUTpin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(WHITE, OUTPUT);
  
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(WHITE, LOW);

  motor.setSpeed(30);
  
  int status;
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.createChar(0, smiley);
  lcd.lineWrap();
  
  if(status) 
  {
    status = -status; 
    hd44780::fatalError(status);
  }
}

void printStatus(String line1, String text) {
    lcd.clear();
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(text);
}

void printString(String text) {
    lcd.clear();
    lcd.print(text.substring(0, 31));
}

// 1 - red
// 2 - green
// 3 - white
void lightLed(char type) {
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW); 
  digitalWrite(WHITE, LOW);
  statusLedPin = (int) type + 13;
  digitalWrite(statusLedPin, HIGH);
}

void loop() {

  if (Serial.available() > 0) {
    incomingCommand = Serial.readString(); 
    Serial.println(incomingCommand);
    
    if (incomingCommand == "vend") {
      vend();
    } else if (incomingCommand == "clear") { 
      lcd.clear();
      digitalWrite(RED, LOW);
      digitalWrite(GREEN, LOW); 
      digitalWrite(WHITE, LOW);
    } else if (incomingCommand[0] == '1') {
      if (incomingCommand.substring(1).length() > 0) {
        printString(incomingCommand.substring(1));
      }
      lightLed(1);
    } else if (incomingCommand[0] == '2') {
      if (incomingCommand.substring(1).length() > 0) {
        printString(incomingCommand.substring(1));
      }
      lightLed(2);
    } else if (incomingCommand[0] == '3') {
      if (incomingCommand.substring(1).length() > 0) {
        printString(incomingCommand.substring(1));        
      }
      lightLed(3);
    } else {
      printString(incomingCommand);
    }
  }
}

void vend() {
  Serial.println("Vending...");
   motor.step(250, FORWARD, DOUBLE);
}
