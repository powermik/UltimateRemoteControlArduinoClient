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

Servo Servo09;
Servo Servo10;
Servo Servo11;
Servo Servo12;
//SoftwareSerial Serial1(3, 2); // RX, TX


#define PWMA_PIN
#define AIN2_PIN
#define AIN1_PIN
#define SERVO09_PIN 9
#define SERVO10_PIN 10
#define SERVO11_PIN 11
#define SERVO12_PIN 12
#define LED_PIN 13
#define BLUETOOTHPOWER_PIN 12
#define DEBUG_ENABLED
#define MAXBUFFER 100
#define BLUETOOTHNAME "cler"


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
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,HIGH);
  delay(500);
  digitalWrite(BLUETOOTHPOWER_PIN,HIGH);
  setupBlueToothConnection();
  digitalWrite(LED_PIN,LOW);
  pinMode(LED_PIN,OUTPUT);
  
  //Start
  Serial1.print('!');
  
  Servo09.attach(SERVO09_PIN);  
  Servo10.attach(SERVO10_PIN);  
  Servo11.attach(SERVO11_PIN);  
  Servo12.attach(SERVO12_PIN);  
    
} 
 
void loop() 
{ 
  delay(100);
    Serial1.print('.');
    Serial.print('.');
    while(Serial1.available() && recvChar!='x'){
    Serial1.print('d');
    Serial.print('d');
      if (i>(MAXBUFFER-1)) {
           i=MAXBUFFER-1;
      }    
      recvChar = Serial1.read();
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
     Serial1.print('!');
     
     //set received parameters on hardware
     
     i=0;
     recvChar='0';
     if (A>101) {
       digitalWrite(LED_PIN,HIGH);
     } else {
       digitalWrite(LED_PIN,LOW);
     }
     
     Servo09.write(map(A,0,255,-90,90)); 
     Servo10.write(map(B,0,255,-90,90)); 
     Servo11.write(map(C,0,255,-90,90)); 
     Servo12.write(map(D,0,255,-90,90)); 
      
     
    }
} 
 
void setupBlueToothConnection()
{
  delay(1500);
  showString(PSTR("AT+PIN0000")); //set the Serial1 work in slave mode
  delay(1500);
  showString(PSTR("AT+NAME"));
  showString(PSTR(BLUETOOTHNAME));
 // showString(PSTR("")); //set the Serial1 name as BLUETOOTHNAME 
  delay(2000); // This delay is required.
  Serial.flush();
}



//neat little trick to lower ram usage
void showString (PGM_P s) {
        char c;
        while ((c = pgm_read_byte(s++)) != 0)
            Serial1.print(c);
    }


