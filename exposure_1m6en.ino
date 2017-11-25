// sketch by Nicu FLORICA (niq_ro) from http://nicuflorica.blogspot.ro & http://arduinotehniq.blogspot.com/
// and Adrian ROMAN (ARelectronics)
// version 1m5 - 28.04.2016 
// version 1m6 - 24.11.2015, for LCD1602 shield with buttons

#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h> 

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // configuration for 1602 shield
#define goarna 11  // ver 1.5 use 10
#define bec 13
#define bec2 12

int settemp, setzone;
int tempmin = 1;     // minimum 10 seconds
int tempmax = 60;    // maximum 600 seconds


int lcd_key     = 0;
int adc_key_in  = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

byte meniu = 0; // if MENIU = 0 is clasical 
                // 1 - change zone
                // 2 - change time
                // 3 - change language

void setup () {
 //   Serial.begin(9600); //start debug serial interface
 //   Wire.begin(); //start I2C interface

//EEPROM.write(300, 6);  // must use just first time, after that must comment
//EEPROM.write(400, 1);  // must use just firs date

settemp = EEPROM.read(300);
setzone = EEPROM.read(400);
if (settemp > tempmax) settemp = tempmax;
if (setzone > 3) setzone = 3;
if (setzone < 1) setzone = 1;
   
   pinMode(bec, OUTPUT);   
   pinMode(bec2, OUTPUT);   
   pinMode(goarna, OUTPUT);   
   digitalWrite(bec, LOW); // set first in 0 logical level
   digitalWrite(bec2, LOW); // set second in 0 logical level
   digitalWrite(goarna, LOW); //set in 0 logical level
   tone(goarna, 4000,500);
   
    lcd.begin(16,2); //Start LCD (defined as 16 x 2 characters)
    lcd.clear(); 
  
   lcd.setCursor (0,0);
   lcd.print ("Exposure system");
   lcd.setCursor (0,1);
   lcd.print ("for PCB 2 sides");
   delay (2000);
   lcd.clear ();
   lcd.setCursor (0,0);
  lcd.print (" Made by niq_ro");
  lcd.setCursor (0,1);
  lcd.print ("& ARelectronics");
  delay(2000);
  lcd.clear ();
}
           

void loop () {

lcd_key = read_LCD_buttons();  // read the buttons
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnSELECT:
     {
     meniu = meniu + 1;
     delay(1000);
     lcd_key = btnNONE;
     lcd.clear();
     break;
     }
     case btnRIGHT:
     {
     expunere(); 
     break;
     }
     case btnNONE:
     {
     break;
     }
   }
if (meniu > 3) meniu = 0;

if (meniu == 0)
{  
 digitalWrite(goarna, LOW);
 digitalWrite(bec, LOW);
 digitalWrite(bec2, LOW);
 
 //   lcd.setCursor (0,1);
    lcd.print ("Time: ");  
    lcd.print(settemp*10);
    lcd.print (" sec. ");
    
    lcd.setCursor (0,1);

 if (setzone == 1) lcd.print ("just bottom face");   
 if (setzone == 2) lcd.print ("both face       ");   
 if (setzone == 3) lcd.print ("just upper face ");

lcd_key = read_LCD_buttons();  // read the buttons
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
    case btnRIGHT:
     {
     lcd.clear();
     tone(goarna, 2000,50);
     expunere(); 
     break;
     }
   case btnSELECT:
     {
     meniu = 1;
     delay(1000);
     lcd_key = btnNONE;
     lcd.clear();
     break;
     }
   case btnNONE:
     {
     break;
     }
   }
} // end menu = 0;

if (meniu == 1)
{
  while (meniu == 1)
  {
  lcd.setCursor (0,0);
  lcd.print ("Set Zone: ");
  
lcd_key = read_LCD_buttons();  // read the buttons
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
    case btnUP:  // 1
     {
     setzone = setzone + 1;
     delay(250);
     if (setzone > 3)
      { 
      setzone = 1;
      }
     break; 
     }
    case btnDOWN:  // 2
     {
     setzone = setzone - 1;
     delay(250);
     if (setzone < 1)
      { 
      setzone = 3;
      }
     break;
     }
   case btnSELECT: // 4
     {
     meniu = 2;
     delay(500);
     lcd_key = btnNONE;
     lcd.clear();
     break;
     }
   case btnNONE:  // 5
     {
     break;
     }
   }
      
    delay(50);
    lcd.print (setzone);
    lcd.print("  "); 

lcd.setCursor (0,1); 
 if (setzone == 1) lcd.print ("just bottom face");   
 if (setzone == 2) lcd.print ("both face       ");   
 if (setzone == 3) lcd.print ("just upper face ");
 }
} // end menu = 1;

if (meniu == 2)
{
  while (meniu == 2)
  {
    lcd.setCursor (0,0);
   lcd.print ("Set Time:");
    
lcd_key = read_LCD_buttons();  // read the buttons
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnSELECT:
     {
     meniu = 3;
     delay(500);
     lcd_key = btnNONE;
     lcd.clear();
     break;
     }
   case btnUP:
     {
     settemp = settemp + 1;
     if (settemp > 60)
      { 
      settemp = 1;
      }
      break;
     }
    case btnDOWN:
     {
     settemp = settemp - 1;
     if (settemp < 1) 
      { 
      settemp = 60;
      }
      break;
     }
   case btnNONE:
     {
     break;
     }
   }

    if (settemp <10) lcd.print(" ");
    lcd.print (settemp*10);
    lcd.print(" sec "); 
    delay(150);
  }
}  // end menu = 2

if (meniu == 3)
{
  EEPROM.write(400, setzone);
  EEPROM.write(300, settemp);
  tone(goarna, 2000,50);
  lcd.clear();
  delay(500);
  meniu = 0;
}

}  // end loop



void expunere() {

lcd.clear();
tone(goarna, 4000,1000);

   lcd.setCursor (0,0);
   lcd.print ("  Exposure:   ");  
   lcd.setCursor (0,1);

if (setzone == 1) {
   digitalWrite(bec, HIGH);
   lcd.print ("just bottom face");   
 }
 if (setzone == 2) {
   digitalWrite(bec, HIGH);
   digitalWrite(bec2, HIGH);
   lcd.print ("both face       ");   
 }   
 if (setzone == 3) {
   digitalWrite(bec2, HIGH);
   lcd.print ("just upper face ");
 }


for (int contor = settemp*10; contor > 0 ; contor --) 
{ // 
// digitalWrite(bec, HIGH);
 
   lcd.setCursor (0,0);
   lcd.print ("EXPOSURE!      ");  
    
    lcd.setCursor (0,1);
    lcd.print ("Time: ");  
    if (contor < 100) lcd.print(" ");
    if (contor < 10) lcd.print(" ");
    lcd.print(contor);
    lcd.print (" sec.  ");  

    lcd.setCursor (16,1);
    if (contor < 6){
      lcd.print ("*");  
       digitalWrite(goarna, HIGH);
       tone(goarna, 3000,200);
    if (contor < 2) {
      tone(goarna, 4000,1000);
    }
    else
    lcd.print (" ");  
    }  
delay(1000);
}
lcd.clear();
}


// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 

 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 // For V1.0 comment the other threshold and use the one below:
/*
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
*/
 return btnNONE;  // when all others fail, return this...
}
