/**
 * Copyright (c) 2012. All rights reserved.
 *
 * This file is part of rovercontrol.
 *
 * Rovercontrol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rovercontrol is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rovercontrol.  If not, see <http://www.gnu.org/licenses/>.
 *
 **************************************************************************************
 * rovercontrol
 * a simple sketch for a simple serial protocol for simple robot control ;)
 *
 * for the controlling side see: https://github.com/Nukehead/UltimateRemoteControl
 *
 *
 */


#include <Boards.h>
#include <Servo.h>
#include <avr/pgmspace.h>
#include <SoftwareSerial.h>

Servo lenk;
SoftwareSerial bluetooth(3, 2); // RX, TX

/* define only one bluetoothmodule */
#define SEEEDSTUDIOBLUETOOTHSLAVE 1
// #define CHINESEHC04
//#define SEEEDSTUDIONBLUETOOTHMASTER 1


#define PWMA_PIN
#define AIN2_PIN
#define AIN1_PIN
#define LENK_PIN 10
#define LED_PIN 13
#define BLUETOOTHPOWER_PIN 12
#define DEBUG_ENABLED
#define MAXBUFFER 100
#define BLUETOOTHNAME "crawlert"


char recvChar='0';
char tmpChar='0';
char cmdbuf[MAXBUFFER];
char tmpbuf[3];
int i=0;
int A=0,B=0,C=0,D=0,E=0;
 
void setup() 
{ 
  pinMode(BLUETOOTHPOWER_PIN,OUTPUT);
  digitalWrite(BLUETOOTHPOWER_PIN,LOW);

  #if defined SEEEDSTUDIOBLUETOOTHSLAVE || defined SEEEDSTUDIONBLUETOOTHMASTER
    bluetooth.begin(38400);
  #endif

  #if  defined CHINESEHC04 
    bluetooth.begin(9600);
  #endif 

  Serial.begin(9600);
  pinMode(LED_PIN,OUTPUT);

  #if defined DEBUG_ENABLED
	  digitalWrite(LED_PIN,HIGH);
	  delay(500);
  #endif
  digitalWrite(BLUETOOTHPOWER_PIN,HIGH);

  setupBlueToothConnection();

  #if defined DEBUG_ENABLED
  	digitalWrite(LED_PIN,LOW);
  #endif

  //Start
  bluetooth.print('!');
  

  //attach servos and motors
  lenk.attach(LENK_PIN);  
} 
 
void loop() 
{ 
  delay(100);
    
    while(bluetooth.available() && recvChar!='x'){
      if (i>(MAXBUFFER-1)) {
           i=MAXBUFFER-1;
      }    
      recvChar = bluetooth.read();
      cmdbuf[i++] = recvChar;
    }
   
    if ( recvChar=='x') {
      for (i=0;i<MAXBUFFER && cmdbuf[i]!='x';i++) {
      	#if defined DEBUG_ENABLED 
          Serial.print (cmdbuf[i]);
	#endif

       switch (cmdbuf[i]) {
            case '?':
              break;
            case 'A':
                tmpbuf[0] = cmdbuf[i+1]; tmpbuf[1] = cmdbuf[i+2]; tmpbuf[2] = cmdbuf[i+3];
                A=atoi(tmpbuf);
                i=i+3;
              break;
            case 'B':
                tmpbuf[0] = cmdbuf[i+1]; tmpbuf[1] = cmdbuf[i+2]; tmpbuf[2] = cmdbuf[i+3];
                B=atoi(tmpbuf);
              break;
            case 'C':
                tmpbuf[0] = cmdbuf[i+1]; tmpbuf[1] = cmdbuf[i+2]; tmpbuf[2] = cmdbuf[i+3];
                C=atoi(tmpbuf);
      		#if defined DEBUG_ENABLED 
                Serial.print("C:");
                Serial.println(C);
		#endif
              break;
            case 'D':
                tmpbuf[0] = cmdbuf[i+1]; tmpbuf[1] = cmdbuf[i+2]; tmpbuf[2] = cmdbuf[i+3];
                D=atoi(tmpbuf);
      		#if defined DEBUG_ENABLED 
                Serial.print("D:");
                Serial.println(D);
		#endif
              break;
            case 'E':
                tmpbuf[0] = cmdbuf[i+1]; tmpbuf[1] = cmdbuf[i+2]; tmpbuf[2] = cmdbuf[i+3];
                E=atoi(tmpbuf);
      		#if defined DEBUG_ENABLED 
                Serial.print("E:");
                Serial.println(E);
		#endif
              break;
            case 0:
                recvChar=0;
                i=0;
              break;
       }
      }
     //ready for next 
     bluetooth.print('!');
     
     //set received parameters on r/c model hardware
     
     i=0;
     recvChar='0';
     if (A>101) {
       digitalWrite(LED_PIN,HIGH);
     } else {
       digitalWrite(LED_PIN,LOW);
     }
     
     lenk.write(map(C,0,255,-90,90)); 
     
    }
} 
 
void setupBlueToothConnection()
{
  #if defined SEEEDSTUDIOBLUETOOTHSLAVE
    showString(PSTR("\r\n+STWMOD=0\r\n")); //set the bluetooth work in slave mode
    showString(PSTR("\r\n+STNA="));
    showString(PSTR(BLUETOOTHNAME));
    showString(PSTR("\r\n")); //set the bluetooth name
    showString(PSTR("\r\n+STPIN=0000\r\n"));
    showString(PSTR("\r\n+STOAUT=1\r\n")); // Permit Paired device to connect me
    showString(PSTR("\r\n+STAUTO=1\r\n"));
    delay(2000); // This delay is required.
    showString(PSTR("\r\n+INQ=1\r\n")); //make the slave bluetooth inquirable
    delay(2000);
    bluetooth.flush();
	#if defined DEBUG_ENABLED
		Serial.print ("bluetooth SEEEDSTUDIOBLUETOOTHSLAVE initialized\r\n");
        #endif
  #endif

  #if defined SEEEDSTUDIOBLUETOOTHMASTER
    showString(PSTR("\r\n+STWMOD=1\r\n")); //set the bluetooth work in slave mode
    showString(PSTR("\r\n+STNA="));
    showString(PSTR(BLUETOOTHNAME));
    showString(PSTR("\r\n")); //set the bluetooth name
    showString(PSTR("\r\n+STPIN=0000\r\n"));
    showString(PSTR("\r\n+STAUTO=1\r\n"));
    delay(2000); // This delay is required.
    showString(PSTR("\r\n+INQ=1\r\n")); //make the slave bluetooth inquirable
    delay(2000);
    bluetooth.flush();
	#if defined DEBUG_ENABLED
		Serial.print ("bluetooth SEEEDSTUDIOBLUETOOTHMASTER initialized\r\n");
	#endif	
  #endif

  #if defined CHINESEHC04
   delay(1500);
   showString(PSTR("AT+PIN0000")); //set the bluetooth work in slave mode
   delay(1500);
   showString(PSTR("AT+NAME"));
   showString(PSTR(BLUETOOTHNAME));
   // showString(PSTR("")); //set the bluetooth name as BLUETOOTHNAME 
	bluetooth.flush();
   delay(2000); // This delay is required.
	
	#if defined DEBUG_ENABLED
		Serial.print ("bluetooth SEEEDSTUDIOBLUETOOTHMASTER initialized\r\n");
	#endif
  #endif
  
  Serial.flush();
  
}


//neat little trick to lower ram usage
void showString (PGM_P s) {
        char c;
        while ((c = pgm_read_byte(s++)) != 0)
            bluetooth.print(c);
    }
