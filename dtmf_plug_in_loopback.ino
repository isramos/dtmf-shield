/*
  dtmf_loopback
  this is a demo code for use with DTMF Module available at http://dossant.com
  
  Product page: http://www.dossant.com/projects/products/dtmf-module/
   
  This code sends a random dtmf code on pins 11 and 12 every second, 
  and receive dtmf digits via pins 3, 4, 5 and 6 upon a rising edge interrupt on pin 2
  
  for loopback, add jumper to J4 pins AIN and OUT
 
 created 2-12-2013 by Igor Ramos 
  
 */
#include <Tone.h>

Tone freq1;
Tone freq2;

const int DTMF_freq1[] = { 1336, 1209, 1336, 1477, 1209, 1336, 1477, 1209, 1336, 1477 };
const int DTMF_freq2[] = {  941,  697,  697,  697,  770,  770,  770,  852,  852,  852 };

int dr = 12; //dr goes high when data ready
int d0 = 11;
int d1 = 10;
int d2 = 9;
int d3 = 8;
int a1 = 6;
int a2 = 7;

char data_ready=0, data[4] = {0,0,0,0};
char data_ready_status=0;
char dr_serviced=0;
char tx_in_progress=0;


void setup() {      

     Serial.begin(9600);
    
     Serial.println("Dossant DTMF Module: Loopback example");   
     
  // initialize the digital pin as an output.
    pinMode(d0, INPUT);    
    pinMode(d1, INPUT); 
    pinMode(d2, INPUT); 
    pinMode(d3, INPUT); 
    pinMode(dr, INPUT); 
    
    freq1.begin(a1);
    freq2.begin(a2);
    
}



// the loop routine runs over and over again forever:
void loop() {
static unsigned long int time, task, tx_time;
char tmp;

    time = millis();
    
    data_ready_status = digitalRead(dr);
          
    if (time - task > 1000) // send random char every 500ms
    {
       task = time;
       
       if (tx_in_progress)
         Serial.println("Rx Timeout");  
        
        char randNumber = random(9);
        Serial.print("\n\rTransmitting DTMF: "); 
        Serial.println(randNumber,DEC);

        playDTMF(randNumber, 500);
        tx_in_progress =1;
    }
    
    if (data_ready_status) 
    {
      if (!dr_serviced)
      {
        //read data pins
        data[0] = digitalRead(d0);
        data[1] = digitalRead(d1);
        data[2] = digitalRead(d2);
        data[3] = digitalRead(d3);
  
        tmp = dtmf_digit(); //assemble dtmf digit;
        Serial.print("DTMF digit received: ");
        Serial.println(tmp,DEC); 
        
        dr_serviced =1; //clear received flag
        tx_in_progress =0;    //clear  tx in progress flag
      }
 
    }
    else 
      dr_serviced =0;
          
  
}


  
// ----DTMF receive functions ----
char dtmf_digit ( void  )  //assemble the bits into a digit
{
  char dtmf_digit;

// dtmf digit is decoded per Page 5 Table 1 of chip datasheet
// http://www.zarlink.com/zarlink/mt8870d-datasheet-oct2006.pdf

  dtmf_digit = 8* data[3] +  4* data[2] + 2* data[1] + data[0] ;
  
  if (dtmf_digit==10)
    dtmf_digit =0;
    
  return dtmf_digit;
}

// ----DTMF Send functions ----
void playDTMF(uint8_t number, long duration)
{
  freq1.play(DTMF_freq1[number], duration);
  freq2.play(DTMF_freq2[number], duration);
}


