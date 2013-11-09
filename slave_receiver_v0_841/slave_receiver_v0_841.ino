#include <Wire.h>
#include <LiquidCrystal_I2C.h>


void trabalhePREFIX(char* PREFIX, unsigned int FASE);
void zerarPREFIXO (char *PREFIX,  unsigned int FASE);
unsigned int tamSTR(char *STR);
int grep(char *STR, char *CATAR);

// encapsulamento: 10+1+10+1(22)  PATH(80 a 112*) +1 MSG (64); = 247

const unsigned int MSG_LIMITE=247; 
const unsigned int TAM_PATH=10;
const unsigned int TOTREPS=10;
const unsigned int TCP_BUFFER_SIZE=92;// 119; 



LiquidCrystal_I2C lcd(56,20,4); // definicao do LCD com ID=56 no barramento I2C, e 20 col x 4 lin

boolean DEBUGAR=true;
//boolean DEBUGAR=false;

boolean FORMAR_PACOTE = false;
boolean PACOTE_FORMADO = false;
boolean PATH_COMPLETO = false;

char PATH[TOTREPS*TAM_PATH]; // 8 * 10 (ate 80 caracteres)
char packetBuffer[TCP_BUFFER_SIZE];

                // AA1BBB23*,
char ORIGEM[10];
char DESTINO[10];
char PAYLOAD[MSG_LIMITE];
//char PAYLOAD[64];

unsigned short int complemento, repeaters=0;
unsigned short int repeater; // contagem de caracteres para repeaters
unsigned short int contadorPAYLOAD;





void setup()
{
  pinMode(8,OUTPUT); pinMode(9, OUTPUT);
  pinMode(7,INPUT);
  pinMode(11,OUTPUT);
  lcd.init();
  //lcd.backlight(); // se existisse o pino 10 do PCD8574 conectado ao visor 

  Serial.begin(9600);           // start serial for output
  
  lcd.setCursor(0,0);
  lcd.print("APRS Gerenciador I2C");
  lcd.setCursor(0,1);
  lcd.print("Roney Monte Nov/2012");
  
  delay(1000);
  for (int i=0; i<3; i++) {digitalWrite(8,0);digitalWrite(9,1); digitalWrite(11, HIGH); delay(100); 
                           digitalWrite(8,1);digitalWrite(9,0); digitalWrite(11, LOW); delay(100); }
 delay(1000);
 Serial.println("");
 Serial.println("Iniciando Slave Receiver v0.81"); 

 Serial.print("Debug "); if(DEBUGAR)Serial.println("ON"); else Serial.println("OFF");

  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  
   Wire.onRequest(requestEvent); // register event
}

void requestEvent()
{ unsigned short int delay0;
  if (DEBUGAR)
  {
  delay0=100;
  Wire.write(delay0); // respond with message of 6 bytes
  }
  else
  {
  delay0=30;
  Wire.write(30);
  }
  
  if(DEBUGAR){Serial.print("INF _Novo delay enviado via I2C=");Serial.println(delay0);}
}
  





void loop()
{  
   // pino D7 ativa ou desativa o debugging
   if (!DEBUGAR && digitalRead(7)) {DEBUGAR=true; Serial.println("Debug ON");}
   if (DEBUGAR && !digitalRead(7)) {DEBUGAR=false;Serial.println("Debug OFF");}
  
  digitalWrite(8,0);
  delay(250);
  digitalWrite(8,1);
  delay(250);

  //      }PP2MD-9>QX148Q,TCPIP,PU1RYM-6*:`MEfoS+>/]";F}=
  
  
  if(FORMAR_PACOTE && PACOTE_FORMADO)
  { //boolean DEBUGAR=true;  // *** DEPOIS RETIRAR ESSA LINHA BOOLEANA ***
  
  
    if (PAYLOAD[0]=='}')
    { 
      if (DEBUGAR) Serial.println("*** ENCAPSULAMENTO ***");
      int tipo=0; // para receber ORIGEM
      int posicao=1; // contador do encapsulamento
    
      for (int i=1; PAYLOAD[i] != 0x00 ; i++)
      {
        
        
        switch(tipo)
        {
          case 0:  // receber ORIGEM
                
                if (PAYLOAD[i]!='>')
                {
                ORIGEM[i-posicao]=PAYLOAD[i];

                }
                else  {tipo=1; ORIGEM[i-posicao]=0x00; posicao=i+1; } // passa tipo 1 para receber DESTINO
          break;
          
          case 1:

                if (PAYLOAD[i]!=',')
                {
                DESTINO[i-posicao]=PAYLOAD[i];
                }
                else {tipo=2; DESTINO[i-posicao]=0x00; posicao=i+1; } // passa tipo 2 para receber PATH
          break;
          
          case 2:

                if (PAYLOAD[i]!= ':')
                {
                  PATH[i-posicao]=PAYLOAD[i];
                }
                else {tipo=3; PATH[i-posicao]=0x00; posicao=i+1; } // passa tipo 3 para receber MSG
          break;
          
          case 3:
                if (DEBUGAR) Serial.print("MSG");
                
                if (DEBUGAR) {Serial.print("3) i=");Serial.print(i);
                              Serial.print(", pos=");Serial.print(posicao);
                              Serial.print(", char=0x");Serial.print(PAYLOAD[i],HEX);
                          }
                
                if (PAYLOAD[i]!=0x00)
                {
                  PAYLOAD[i-posicao]=PAYLOAD[i];
                  PAYLOAD[i]=0x00; // limpa o byte que estava encapsulado;
                }
                
                else {tipo=4; PAYLOAD[i-posicao]=0x00; } // final
          break;
          
          case 4:
                          if (DEBUGAR) {Serial.print("4) i=");Serial.print(i);
                              Serial.print(", pos=");   Serial.print(posicao);
                              Serial.print(", char=0x");Serial.print(PAYLOAD[i],HEX);
                          }
          
          break;
          
          
        }
        
        //if (PAYLOAD[i]!='>')
        //ORIGEM[i-1]=PAYLOAD[i];
        
      }
      
    }    
    
    
    
    /*
    digitalWrite(11, HIGH); delay (100); digitalWrite(11, LOW);delay(100);
    digitalWrite(11, HIGH); delay (100); digitalWrite(11, LOW);
    */
    
    
    
    trabalhePREFIX("",1);
    if(DEBUGAR)
    {
    Serial.print("DST=");  Serial.print  ( String(DESTINO) );
    Serial.print(" DE=");  Serial.println( String(ORIGEM) );
    }
    
    trabalhePREFIX(DESTINO,2);
    trabalhePREFIX(ORIGEM,3);
    
    if(DEBUGAR)
    {
    Serial.print("PATH="); Serial.print( String(PATH) ); 
    Serial.print(" (tot "); Serial.print (repeaters); Serial.println (")");
    }
    
    trabalhePREFIX(PATH,5);
    
    if(DEBUGAR)
    {Serial.print("Mensagem APRS:"); Serial.println( PAYLOAD );}
    
    trabalhePREFIX(PAYLOAD,11);
    
    if(DEBUGAR)
    Serial.println("");
    
    FORMAR_PACOTE=false;
    PACOTE_FORMADO=false;
    
    
    /// IGATE
    Serial.println("");
    
    int ipJaRepetido = grep  ( PATH, "TCPIP" );
    if (ipJaRepetido < 0) {
      Serial.println ("nao passou na rede APRS-IS");
       trabalhePREFIX("",14); // coloca simbolo ^ no LCD
    }
    else Serial.println("ja passou origem APRS-IS");
    
    //if(DEBUGAR)
    //{
    Serial.println("");
    Serial.print("APRSIS posicao= "); Serial.print(ipJaRepetido,DEC);
    
    Serial.println("");
    
    Serial.write(ORIGEM);Serial.write(">");Serial.write(DESTINO);
    Serial.write(",");Serial.write(PATH);
    Serial.write(",");Serial.write("PU1RYM-5,I");
    Serial.write(":");Serial.write(PAYLOAD);
    
    //Serial.println("");
    //}
    
  }
  
}





void receiveEvent(int howMany)
{
  noInterrupts();
  short int contador=0;
  char MENSAGEM[32];
  char c, primeiro;
  
  MENSAGEM[0]=0x00; // limpa a string de MENSAGEM
  digitalWrite(8,0);digitalWrite(9,1);

  ///// FASE 1 = RECEPCAO da STRING ate 31 chars
  
  complemento = Wire.read();
  
  if (complemento==255)
  {
    if (DEBUGAR) Serial.println("INF TIMEOUT");
    
    boolean FORMAR_PACOTE = false;
    boolean PACOTE_FORMADO = false;
    boolean PATH_COMPLETO = false;
    interrupts();
    return;
  }
  

  //if (complemento==11)
  //{
  //}
  //
  //else
  
  while(0 < Wire.available()) // loop through all but the last
  {
    c = Wire.read();

    if (contador==0) primeiro=c;
    
    if (primeiro != '_' && (c!=0x00) ) // descarta a gravacao em strings se for __controle
     switch(complemento)
     {
      case 0:
          MENSAGEM[contador]=c;
          break;
          
      case 2:
          DESTINO[contador]=c;
        break;
    
      case 3:  
          ORIGEM[contador]=c;
        break;
    
      case 5: // o ultimo complemento recebido era da ORIGEM, nesse caso este seria do PATH, mas nao totalmente
          PATH[repeater]=c;
              //Serial.print(c,HEX);Serial.print("[");Serial.print(repeater);Serial.print("] ");
              repeater++;
        break;
        
      case 11: // recebimento de varias partes da MENSAGEM APRS
              if (contadorPAYLOAD<MSG_LIMITE) PAYLOAD[contadorPAYLOAD]=c;
        break;      
        
      case 12:
          PACOTE_FORMADO=true;
        break;
      }
      else if ( c >0x1F && c <0x7F && complemento!=11) MENSAGEM[contador]=c; // aqui so recebe SE for __controle
      
      
      //if(DEBUGAR)Serial.print(contador);
      if (c > ' ' && c < 'z') contador++; // so incrementa contador SE NAO FOR char de controle;
      if (complemento==11) contadorPAYLOAD++;
      
  }

  if (DEBUGAR) { digitalWrite(11, HIGH); delay(2); digitalWrite(11, LOW);}
  if (!DEBUGAR) { digitalWrite(11, HIGH); delay (1); digitalWrite(11, LOW);}
  
  
  switch(complemento)
  {
    case 0: MENSAGEM[contador]=0x00;  break;
    case 1: FORMAR_PACOTE=true;
            PACOTE_FORMADO=false;       
         break;
         
    case 2: DESTINO [contador]=0x00;  break;
    case 3: ORIGEM  [contador]=0x00;   
            repeater=0; 
            repeaters=0;
            zerarPREFIXO (PATH,TOTREPS*TAM_PATH); 
            PATH_COMPLETO=false; // preparando para receber PATH
        break;
    case 5:
        
        if (primeiro != '_')
        {
          repeaters++;      //apenas para registrar, aumenta a quantida de PATHs
          PATH[repeater]=',';
          //if(DEBUGAR){Serial.print("posicao PATH=");Serial.println(repeater);}
          repeater++;       //adiciona um caractere que sera a virgula
        }
        else MENSAGEM[contador]=0x00; // se primeiro for '_'
        break;
        
    case  7 :        // terminou PATHs
         PATH [--repeater]=0x00;  //finalizacao da string de PATHs, sobrepoe a ultima virgula do 'case 5'
         PATH_COMPLETO=true;
         //if(DEBUGAR){ Serial.print("Repeaters=");Serial.println(repeaters); }
       break;
       
     case 10: 
               contadorPAYLOAD=0;
               zerarPREFIXO(PAYLOAD,MSG_LIMITE);
       break;  
       
    case 11: PAYLOAD[contadorPAYLOAD]=0x00;
    
      break;
       
    default: MENSAGEM[contador]=0x00;
  }



  ///// FASE 2 = TRATAMENTO das INFORMACOES
  if (DEBUGAR)
  {
      if ((complemento==0) && MENSAGEM[0]=='p' && MENSAGEM[1]=='u' && MENSAGEM[2]=='l' && MENSAGEM[3]=='s'
          && MENSAGEM[4]=='e' && complemento==0) 
          {;// nao faca nada // pulse = keepAlive
          }
       
      else
      {
      //Serial.print("("); Serial.print(howMany, DEC);
      Serial.print("[");
      Serial.print( complemento );         // print the integer
      Serial.print("] ");  
      
      switch(complemento)
        {
          case 2: Serial.println(DESTINO); break;
          case 3: Serial.println(ORIGEM); break;
          
          case 5:
          
                if (primeiro!='_') {
                  Serial.print(PATH);
                  Serial.print("["); Serial.print( tamSTR(PATH) );
                  Serial.print("] (0x"); Serial.print( PATH[tamSTR(PATH)-3], HEX );
                  Serial.print(",0x");
                                        Serial.print( PATH[tamSTR(PATH)-2], HEX ); 
                  Serial.println (")");
                }
                 
                
                
                break; //nao quebrando, vai tambem cair no default: e imprimir MENSAGEM (nao _)
          case  7: Serial.print("Path definido:"); Serial.print(PATH); Serial.print("[");Serial.print(tamSTR(PATH));
                   Serial.println("]");
                break;
                
          case 11: Serial.println(PAYLOAD); break;
          default:  Serial.print("_ctl:"); Serial.println(MENSAGEM);
        }
        
      }
  }

  digitalWrite(8,1);digitalWrite(9,0);
  interrupts();
}




void trabalhePREFIX(char* PREFIX, unsigned int FASE)
{
  
  unsigned int contLCD=0;
  int c;
  
  //if (DEBUGAR)
  //{
    //Serial.print("Recebeu fase="); Serial.print(FASE); Serial.print(" com PREFIX="); Serial.println(PREFIX);
  //}
  
  
  switch (FASE)
  {
      case 1:
            lcd.clear();
            break;
            
      case 2: // DESTINO
      
            lcd.setCursor(9,0);
            lcd.print(";");

            //lcd.setCursor(10,0);
            lcd.print( PREFIX );
            //lcd.print("Destino");
      
      break;
      
      case 3: // ORIGEM
            lcd.setCursor(0,0);
            lcd.print( PREFIX );
            //ORIGEM=String(ORIGEM);
            //lcd.print(ORIGEM);
      
      break;

      case 5:
            lcd.setCursor(0,1);

            
            
            for (int i=0; contLCD<20; i++)
            { 
                if ( PREFIX[i] > 0x1F && PREFIX[i]< 0x5B )
                {
                lcd.print( PREFIX[i] );
                contLCD++;
                }
                
                if (PREFIX[i] == 0x00) break;
                
                // se a palavra for WIDE, abrevie somente com o primeiro W e pule o ..IDE.
                if (PREFIX[i]=='W' && PREFIX[i+1]=='I' && PREFIX[i+2]=='D' && PREFIX[i+3]=='E')
                { i += 3; }
                
                if (PREFIX[i]=='T' && PREFIX[i+1]=='C' && PREFIX[i+2]=='P' && PREFIX[i+3]=='I' && PREFIX[i+4]=='P')
                { i += 4; }
            }
            
      
      break;
      
      case 11:
            if (DEBUGAR) Serial.println("---LCD---");
            
            lcd.setCursor(0,2);
            
            c=0;
            
            for (int i=0; c<40 ; i++)
            {
              if ( PREFIX[i] != 0x00)
              {
                if ( (PREFIX[i]>0x1F) && (PREFIX[i]<0x7F))
                { 
                  if (c==20) { lcd.setCursor(0,3) ; }
                
                  
                  
                  //if (DEBUGAR)
                  //{
                    //Serial.print( PREFIX[i] );Serial.print("[");Serial.print(i);
                    //Serial.print("/");Serial.print(c);Serial.print("] ");
                  //}
                  
                  // campo "=" signigica que TEM MENSAGEM APRS de POSICIONAMENTO
                  if ( PREFIX[i] == '=' && i==0) // POSICIONAMENTO   // nao PHG
                  {
                    while ( PREFIX[i] != '&' ) i++; i++; 
                  }
                  
                  if ( PREFIX[i] == '!' && i==0) // POSICIONAMENTO + PHG
                  {
                    while ( PREFIX[i] != '/'  ) i++; i++;
                   // agora que chegou no P, conte PHGphgd  
                  }
                  
                  
                  lcd.print( PREFIX[i] );
                  
                  c++;
                }
                
              }
              else break;
            }
            
      
      break;
     
      
      case 14:  
                lcd.setCursor(9,0);
                lcd.print('^');
             break;
            
  }
  
}

void zerarPREFIXO (char *PREFIX, unsigned int TAM)
{
  for (int i=0; i<TAM; i++) PREFIX[i]=0x00;
}

unsigned int tamSTR(char *STR)
{
    char c;
    unsigned int tamSTR=0;
    
      for(tamSTR=0; tamSTR<255; tamSTR++) // o valor de 255 eh uma suposicao alta, poderia ser maior ate
      { 
        c=char( STR[tamSTR] );
        if (c == 0x00) break;
      }
    return tamSTR;
}

int grep(char *STR, char *CATAR)
{
  String STRING = STR;
  String aProcurar = CATAR;
  int i;
  i = STRING.indexOf( aProcurar );
  /*
  if (DEBUGAR)
  {
    Serial.println("");
    Serial.print("String="); Serial.println(STRING); 
    Serial.print("a procurar="); Serial.print(aProcurar); Serial.print(", achado=");
    
    Serial.println(i);
  }*/
  
  
  return i ;
}
