// demo: CAN-BUS Shield, receive data with interrupt mode, and set mask and filter
//
// when in interrupt mode, the data coming can't be too fast, must >20ms, or else you can use check mode
// loovee, 2014-7-8

#include <SPI.h>
#include "mcp_can.h"

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

unsigned char flagRecv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];
int var=0;
int x=0;
int relay1 = A0;
int relay2 = A1;

void setup()
{
    //Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        //Serial.println("CAN BUS Shield init fail");
        //Serial.println(" Init CAN BUS Shield again");
    }
    //Serial.println("CAN BUS Shield init ok!");
    
    CAN.init_Filt(0,1,0x10046A01);
    CAN.init_Mask(0,1,0x1FFFFFFF);
    CAN.init_Mask(1,1,0x1FFFFFFF); 
    //Serial.println("Mask and filter set!"); 
    //attachInterrupt(0, MCP2515_ISR, FALLING); // start interrupt

    pinMode(relay1,OUTPUT);
    pinMode(relay2,OUTPUT);
}

void MCP2515_ISR()
{
    flagRecv = 1; 
}



void loop()
{
    MCP2515_ISR();
    if(flagRecv)                   // check if get data
    {
        flagRecv = 0;                // clear flag
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
        
        if (x==1) {
            var=(buf[4]<<8) | buf[3];
            x=0;
            if (var==1094) {
              //Serial.println("D");
              digitalWrite(relay1,LOW);
              digitalWrite(relay2,HIGH);
          }
          
          if (var==1934) {
             //Serial.println("U");
             digitalWrite(relay1,HIGH);
             digitalWrite(relay2,LOW);
          }
        }

        if(buf[7] >= 0b10000000) {
          x++;
        }
    }
}
