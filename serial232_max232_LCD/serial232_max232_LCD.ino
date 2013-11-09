#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
byte c;

void setup() {

  //malloc(1024*sizeof(char));
  
  Serial.begin(1200);

  pinMode(9, OUTPUT); 
  pinMode(8, OUTPUT);

  lcd.begin(20, 4);
  lcd.autoscroll();
  lcd.clear();
  lcd.print("--------------------");
  lcd.print("Roney Monte Nov/2012");
  lcd.print("Serial para 1200 bps");  
  delay (3000);

  lcd.print("OK");
  
}

void loop ()
{ 
  digitalWrite(8,1);
  while ( Serial.available() )
  { 
    digitalWrite (8,0);
    digitalWrite (9,1);
    c = Serial.read();
    if ( (c >= '0') && (c <='z') ) lcd.write(c);
    digitalWrite (9,0);
  }
  
}
