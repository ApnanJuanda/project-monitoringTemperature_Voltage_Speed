#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);
const int numReadings = 25;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int xmax=128;                                  
int ymax=62;                                  
int xcenter=xmax/2;                            
int ycenter=ymax/2+10;                        
int arc=ymax/2;                            
int angle=0;                                  
char* label[] = {"VOLTAGE","COOLANT","INTAKE", "VOLT"};  
int labelXpos[] = {45, 47, 49, 53};                    
#define potmeterPin A2 //masukkan pin input ke arduino dari rangkaian voltage divider                          
int p, w, m,a=10;
u8g_uint_t xx=0;

const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 11;
const int ledPin = 13;

long duration;
int distance;
int safetyDistance;


void gauge(uint8_t angle) 
{
  // draw border of the gauge
  if (w<0)
          {
            w=0;
          }


  u8g.drawCircle(xcenter,ycenter,arc+6, U8G_DRAW_UPPER_RIGHT);
  u8g.drawCircle(xcenter,ycenter,arc+4, U8G_DRAW_UPPER_RIGHT);
  u8g.drawCircle(xcenter,ycenter,arc+6, U8G_DRAW_UPPER_LEFT);
  u8g.drawCircle(xcenter,ycenter,arc+4, U8G_DRAW_UPPER_LEFT);
  
  // draw the needle //pembuatan lengkung lingkarannya
  float x1=sin(2*angle*2*3.14/360);        //pembuatan lingkaran  
  float y1=cos(2*angle*2*3.14/360);        // pembuatan lingkaran
  u8g.drawLine(xcenter, ycenter, xcenter+arc*x1, ycenter-arc*y1);
  u8g.drawDisc(xcenter, ycenter, 5, U8G_DRAW_UPPER_LEFT);
  u8g.drawDisc(xcenter, ycenter, 5, U8G_DRAW_UPPER_RIGHT);
  u8g.setFont(u8g_font_chikita);
  
  // show scale labels pembuatan angka pada lingkarannya
  u8g.drawStr( 20, 42, "0");                  
  u8g.drawStr( 25, 18, "25");
  u8g.drawStr( 60, 14, "50");
  u8g.drawStr( 95, 18, "75");
  u8g.drawStr( 105, 42, "100");
  
  // show gauge label pembuatan bacaan VOLTAGE yang ditengah
  u8g.setPrintPos(labelXpos[0],32);            
  u8g.print(label[0]);
  
  // show digital value and align its position penunjukkan angka digital voltage
  u8g.setFont(u8g_font_profont22);            
  u8g.setPrintPos(54,60);
  if (w<10){                              
    u8g.print("0");
  }
  if (w>99) 
    {                                  
     u8g.setPrintPos(47,60);
    }
  u8g.print(w+1
  
  
  );
}



void setup(void) 
{
  for (int thisReading = 0; thisReading < numReadings; thisReading++) 
    {
       readings[thisReading] = 0;
    }
    u8g.setFont(u8g_font_chikita);
    u8g.setColorIndex(1);   
                       
  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) 
    {
      u8g.setColorIndex(255);                    
    }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) 
    {
      u8g.setColorIndex(3);                      
    }
  else if ( u8g.getMode() == U8G_MODE_BW ) 
    {
      u8g.setColorIndex(1);                      
    }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) 
    {
      u8g.setHiColorByRGB(255,255,255);
    }
}


void loop(void) {
    // subtract the last reading:
  total = total - readings[readIndex];
  
  // read from the sensor:
  readings[readIndex] = analogRead(potmeterPin); //pembacaan data analog dari rangkaian voltage divider
  
  // add the reading to the total:
  total = total + readings[readIndex];
  
  // advance to the next position in the array:
  readIndex = readIndex + 1;
 
  // if we're at the end of the array...
  if (readIndex >= numReadings) 
  {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
 
  // calculate the average:
  average = total / numReadings;
 
  p = analogRead(potmeterPin); //ini kuncinya untuk menyesuaikan angka digital             
  w = map(p,0,1023,0,100);  //Change the first 2 numbers to calibrate voltage to 0-100%    ini kuncinya untuk menyesuaikan angka digital   sekaligus range dari implementasi angka voltage yang sebenarnya nanti dibuat 0-1023 o menandakan  voltage 0 dan 0 volt dan 1023 menandakan 5 vol dan voltage 100 volt selebih dari itu rentangnya saja     
  m = map(p,0,1023 ,0,90);   //Change the first 2 numbers to calibrate voltage to 0-100%    ini kuncinya untuk menyesuaikan angka digital
                
  // show needle and dial
  xx = m;                                    
  if (xx<45){                                
    xx=xx+135;
  }
  else if (xx>135){
    xx=135; //below 0 should keep pointing at 180 degrees
  }
  else if (xx>90){
  
    xx=45; //above 100 should keep pointing at 0 degrees
  }
  else {
    xx=xx-45;
  }
  
  // picture loop
  {
    u8g.firstPage();
    do {            
      gauge(xx);          //lihat void gauge xx disini maksudnya variabel pengganti dari nilai analog input rangkaian voltage divider
    }
    while( u8g.nextPage() );
  }
}
