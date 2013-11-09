#include <Wire.h>
const unsigned int RUN = 20; // TEMPO
const unsigned int PAUSE = 0; // PAUSA entre Series
void setup()
{
Wire.begin();
//Serial.begin(9600);

}

void loop()
{
  // { y = 0; } //P0 = 0000
  // { y = 2; } //P1 = 0011
  // { y = 4; } //P2 = 0111
  // { y = 8; } //P3 = 1111
  
  
  
  

 // 1, 3, 7, 15 ( 0001, 0011, 0111, 1111)
 // 1, 2, 4, 8  ( 0001, 0010, 0100, 1000)
 // SHIFT LEFT 1 IBIT
 Serial.println("SHIFT LEFT 1 IBIT = Andar para esqueda 1 bit, ou seja, multiplicar por 2");
 for ( int y=B0000; y <= B1000; y <<= 1 )
  {
  Wire.beginTransmission(56);     //Begin the transmission to PCF8574 
  Wire.write(y);                                //Send the data to PCF8574
  Wire.endTransmission();            //End the Transmission

  Serial.print("Numero #"); Serial.print(y); Serial.print(" = B"); Serial.println( y,BIN);
  delay(RUN);
  if (y==B0000) y |= B0001;
  }
  //delay (PAUSE);
  
 // 1, 3, 7, 15 ( 0001, 0011, 0111, 1111)
 // 1, 2, 4, 8  ( 0001, 0010, 0100, 1000)
 // SHIFT RIGHT 1 BIT
 Serial.println("SHIFT RIGHT 1 BIT = Andar para direita 1 bit, ou seja, dividir por 2");
 for ( int y=B1000; y == B0000; y >>= 1 )
  {
  Wire.beginTransmission(56);     //Begin the transmission to PCF8574 
  Wire.write(y);                                //Send the data to PCF8574
  Wire.endTransmission();            //End the Transmission

  Serial.print("Numero #"); Serial.print(y); Serial.print(" = B"); Serial.println( y,BIN);
  delay(RUN);
  }
  delay (PAUSE);
  
  
  
  
  
  
  
  
 // 1, 3, 7, 15 ( 0001, 0011, 0111, 1111)
 // 1, 2, 4, 8  ( 0001, 0010, 0100, 1000)
 // SHIFT LEFT 1 BIT , MAKE OR 0001
 Serial.println("SHIFT LEFT 1 BIT , MAKE \"OR\" 0001 MSB = Andar para esquerda 1 bit e preencher 1 bit na direita");
 Serial.println("ou seja, multiplicar por 2 e adicionar 1 ( 1,   2+1=3   ,   6+1=7   ,   14+1=15 )");
  for ( int y=B0000; y <= B1111; y <<= 1 )
  {
  y |= B0001;     
  Wire.beginTransmission(56);     //Begin the transmission to PCF8574 
  Wire.write(y);                                //Send the data to PCF8574
  Wire.endTransmission();            //End the Transmission

  Serial.print("Numero #"); Serial.print(y); Serial.print(" = B"); Serial.println( y,BIN);
  delay(RUN);
  
  if (y==B0000) y |= B0001;
  }
  //delay (PAUSE);
  
 // 1, 3, 7, 15 ( 0001, 0011, 0111, 1111)
 // 1, 2, 4, 8  ( 0001, 0010, 0100, 1000)
 // SHIFT RIGHT 1 BIT and ZERO LSB
  Serial.println("SHIFT RIGHT 1 BIT and ZERO LSB = Andar para direita, e zerar LSB");
  Serial.println("ou seja, DIVIDIR por 2 e subtrair 1 ( 15,   15-1/2=7   ,   7-1/2=3   ,   3-1/2=1 ");
  for ( int y=B1111; y > B0000; y >>= 1 )
  {
  Wire.beginTransmission(56);     //Begin the transmission to PCF8574 
  Wire.write(y);                                //Send the data to PCF8574
  Wire.endTransmission();            //End the Transmission

  Serial.print("Numero #"); Serial.print(y); Serial.print(" = B"); Serial.println( y,BIN);
  delay(RUN);
  }
  delay (PAUSE);
  
  
  
  
  
  
  
 // 1, 3, 7, 15 ( 0001, 0011, 0111, 1111)
 // 1, 2, 4, 8  ( 0001, 0010, 0100, 1000)
 // 1111 XOR Y
 Serial.println("1111 XOR y SHIFT RIGHT");
  for ( int y=B10000; y >= B0001; y >>= 1 )
  {
    
  Wire.beginTransmission(56);     //Begin the transmission to PCF8574 
  Wire.write( B1111 ^ y );          //Send the data to PCF8574
  Wire.endTransmission();            //End the Transmission

  Serial.print("Numero #"); Serial.print(y); Serial.print(" = B"); Serial.print (y,BIN);
  Serial.print(", XOR=B"); Serial.print( B1111 ^ y,BIN); Serial.print(" decimal=");Serial.println(B1111^y,DEC);
  delay(RUN);
  }
  //delay (PAUSE);  
  
 // 1, 3, 7, 15 ( 0001, 0011, 0111, 1111)
 // 1, 2, 4, 8  ( 0001, 0010, 0100, 1000)
 // 1111 XOR Y
 Serial.println("1111 XOR y SHIFT LEFT");
  for ( int y=B0001; y <= B10000; y <<= 1 )
  {
    
  Wire.beginTransmission(56);     //Begin the transmission to PCF8574 
  Wire.write( B1111 ^ y );          //Send the data to PCF8574
  Wire.endTransmission();            //End the Transmission

  Serial.print("Numero #"); Serial.print(y); Serial.print(" = B"); Serial.print (y, BIN); 
  Serial.print(", XOR=B"); Serial.print( B1111 ^ y,BIN);Serial.print(" decimal=");Serial.println(B1111^y,DEC);
  delay(RUN); 
  }
  delay (PAUSE);  
  
  
  
}
