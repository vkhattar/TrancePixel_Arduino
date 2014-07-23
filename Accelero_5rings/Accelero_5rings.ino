#include <Wire.h>
#include <Adafruit_LSM303.h>
#include <Math.h>
#include <Adafruit_NeoPixel.h>
#define PIN 6
#define yaw_offset 15
int led = 7;

Adafruit_LSM303 lsm;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PIN, NEO_GRB + NEO_KHZ800);
float pitch_angle;
float roll_angle;
float yaw_angle;
//int pixel[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109.110,111,112,113,114,115,116,117,118,119};
int prev_accel_x = 0;
int prev_accel_y = 0;
int prev_accel_z = 0;
int rate_accel_x = 0;
int rate_accel_y = 0;
int rate_accel_z = 0;
int strip_speed = 0;


void setup() 
{
  strip.begin(); //beginning communication on d6 
  strip.show(); //initilize
  strip.setBrightness(127); //Setting the brightness to 127. Max value is 255. 
  Serial.begin(9600); //Setting the baud rate to 9600
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
    while (1);
  }
}


/*
**get_angle(): calculating the pitch, roll and yaw angles. 
**get_acceleration(): Finding the difference between two consecutive accelerations
**set_speed(): Set the speed for blinking of lights based on get_acceleration()
**Blackout(): Switches off all the LEDs
**strip.Color((R,G,B),Delay): red , green and blue values, delay is the blinking speed
**theaterChase(strip.Color()): TheatreChase look in the example
**colorWipe(strip.color(R,G,B)): Will light up the LEDs starting from 0..23
*/

void loop() 
{
  get_angle();
  get_acceleration();
  set_speed_brightness();
  Blackout();
  //speed_test(strip.Color(127, 127, 127));
  //theaterChase(strip.Color(127, 127, 127), strip_speed); // White
  //theaterChase(strip.Color(32,200,100), strip_speed); // Red
  //theaterChase(strip.Color(0,   0, 127), strip_speed); // Blue
  
  //colorWipe(strip.Color(127, 127, 127));
  
   //angle_test(0.5);
   //pitch_test(strip.Color(127, 127, 0));  
   //roll_test(strip.Color(0, 127, 127)); 
   yaw_test(strip.Color(127, 127, 127));
}

void get_acceleration(){
  lsm.read();
  
  int new_accel_x = lsm.accelData.x;  
  int accel_x[] = {new_accel_x,prev_accel_x};
  prev_accel_x = new_accel_x;
  rate_accel_x = abs(accel_x[0] - accel_x[1]);
  Serial.print("Accel_rate X: "); Serial.print(rate_accel_x); Serial.print(" ");
  
  int new_accel_y = lsm.accelData.y;  
  int accel_y[] = {new_accel_y,prev_accel_y};
  prev_accel_y = new_accel_y;
  rate_accel_y = abs(accel_y[0] - accel_y[1]);
  Serial.print("Accel_rate Y: "); Serial.print(rate_accel_y); Serial.print(" ");
  
  int new_accel_z = lsm.accelData.z;  
  int accel_z[] = {new_accel_z,prev_accel_z};
  prev_accel_z = new_accel_z;
  rate_accel_z = abs(accel_z[0] - accel_z[1]);
  Serial.print("Accel_rate Z: "); Serial.print(rate_accel_z); Serial.print(" ");
   
   
}

void set_speed_brightness(){
  if(rate_accel_x < 100 || rate_accel_y < 100 || rate_accel_z < 100){strip_speed = 2000; strip.setBrightness(25);}
  else if(rate_accel_x < 150 || rate_accel_y < 150 || rate_accel_z < 150){strip_speed = 1500;strip.setBrightness(75);}
  else if(rate_accel_x < 175 || rate_accel_y < 175 || rate_accel_z < 175){strip_speed = 1000;strip.setBrightness(100);}
  else if(rate_accel_x < 250 || rate_accel_y < 250 || rate_accel_z < 250){strip_speed = 500;strip.setBrightness(150);}
  else if(rate_accel_x < 750 || rate_accel_y < 750 || rate_accel_z < 750){strip_speed = 250;strip.setBrightness(200);}
  else if(rate_accel_x < 1000 || rate_accel_y < 1000 || rate_accel_z < 1000){strip_speed = 125;strip.setBrightness(225);}
  else if(rate_accel_x < 1500 || rate_accel_y < 1500 || rate_accel_z < 1500){strip_speed = 62;strip.setBrightness(250);}
  else{strip_speed = 10;strip.setBrightness(255);}
  
}

void get_angle()
{
  lsm.read();
  float pitch = 0;
  float roll = 0;
  float yaw = 0;
  
  float pitch_ang, roll_ang,yaw_ang;
  
  pitch = atan((lsm.accelData.x)/sqrt((pow(lsm.accelData.y,2)+pow(lsm.accelData.z,2))));
  pitch_angle = pitch*(180/PI);
  
  roll = atan((lsm.accelData.y)/sqrt((pow(lsm.accelData.x,2)+pow(lsm.accelData.z,2))));
  roll_angle = roll*(180/PI);
  
  /*yaw = atan((-(lsm.magData.y*cos(roll))-(lsm.magData.z*sin(roll)))/((lsm.magData.x*cos(pitch))+(lsm.magData.y*sin(pitch)*sin(roll))+(lsm.magData.z*sin(pitch)*cos(roll))));
  yaw_angle = yaw*(180/PI);*/
  yaw_angle = (atan2(lsm.magData.y,lsm.magData.x) * 180) / PI;
  if (yaw_angle < 0)
  {
    yaw_angle = 360 + yaw_angle;
  }
    //Serial.print("\nyaw_angle "); Serial.println(yaw_angle);       Serial.print(" ");
    
}


void speed_test(uint32_t c){
  int i;
  for(i = 0; i < strip.numPixels();i++){
    strip.setPixelColor(i, c);
  }
  
 strip.show(); //it willl show the above pattern 
  
 delay(strip_speed);
 
 Blackout();
  
  delay(strip_speed);


}

void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(strip_speed);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void angle_test(uint8_t wait) {
  uint16_t i,j,k,l;
   for(j=0; j<256; j++) {
    
     if(roll_angle < 0){
        for(i=12; i<24; i++) {
          strip.setPixelColor(i, Wheel(i+j));
        }
      for(k = 36;k<48;k++){
         strip.setPixelColor(k, Wheel(k+j));
      }
      for(l = 60;l<72;l++){
         strip.setPixelColor(l, Wheel(l+j));
      }
      
    }
    if(roll_angle > 0){
      for(i=0; i<12; i++) {
        strip.setPixelColor(i, Wheel(i+j));
      }
      for(k = 24;k<36;k++){
         strip.setPixelColor(k, Wheel(k+j));
      }
      for(l = 48;l<60;l++){
         strip.setPixelColor(l, Wheel(l+j));
      }
    }
    strip.show();
    delay(wait);
  }
}

void pitch_test(uint32_t c){
  strip.setBrightness(255);
  uint16_t i,j;
    if(pitch_angle >- 90 && pitch_angle <-75) {
      for(int i=0;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
    }  
    else if (pitch_angle >- 75 && pitch_angle <-60) {
      for(int i=1;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
    }
    else if (pitch_angle >- 60 && pitch_angle <-45) {
      for(int i=2;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
    }
    else if (pitch_angle >- 45 && pitch_angle <-30) {
       for(int i=3;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
    }
    else if (pitch_angle >- 30 && pitch_angle <-15) {
       for(int i=4;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
    }
    else if (pitch_angle >- 15 && pitch_angle < 0) {
     for(int i=5;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
    }   
    else if (pitch_angle > 0 && pitch_angle < 15) {
       for(int i=6;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
    }
    else if (pitch_angle > 15 && pitch_angle < 30) {
       for(int i=7;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
    }
    else if (pitch_angle > 30 && pitch_angle < 45) {
     for(int i=8;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
    }
    else if (pitch_angle > 45 && pitch_angle < 60) {
       for(int i=9;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
    }
    else if (pitch_angle > 60 && pitch_angle < 75) {
      for(int i=10;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
    }
    else  {
       for(int i=11;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
    }
    strip.show();
 }
    

void roll_test(uint32_t c){
       
    strip.setBrightness(255);
    uint16_t i,j;
    if(roll_angle >- 90 && roll_angle <-75) {
        for(int i=0;i<120;i+=24){
            strip.setPixelColor(i, c);
        }
    }
    else if (roll_angle >- 75 && roll_angle <-60) {
        for(int i=1;i<120;i+=24){
            strip.setPixelColor(i, c);
        }
    }
    else if (roll_angle >- 60 && roll_angle <-45) {
        for(int i=2;i<120;i+=24){
            strip.setPixelColor(i, c);
        }
    }
    else if (roll_angle >- 45 && roll_angle <-30) {
        for(int i=3;i<120;i+=24){
            strip.setPixelColor(i, c);
        }
    }
    else if (roll_angle >- 30 && roll_angle <-15) {
        for(int i=4;i<120;i+=24){
            strip.setPixelColor(i, c);
        }
    }
    else if (roll_angle >- 15 && roll_angle < 0) {
        for(int i=5;i<120;i+=24){
            strip.setPixelColor(i, c);
        }
    }
    else if (roll_angle > 0 && roll_angle < 15) {
        for(int i=6;i<120;i+=24){
            strip.setPixelColor(i, c);
        }
    }
    else if (roll_angle > 15 && roll_angle < 30) {
        for(int i=7;i<120;i+=24){
            strip.setPixelColor(i, c);
        }
    }
    else if (roll_angle > 30 && roll_angle < 45) {
        for(int i=8;i<120;i+=24){
            strip.setPixelColor(i, c);
        }
    }
    else if (roll_angle > 45 && roll_angle < 60) {
        for(int i=9;i<120;i+=24){
            strip.setPixelColor(i, c);
        }
    }
    else if (roll_angle > 60 && roll_angle < 75) {
        for(int i=10;i<120;i+=24){
            strip.setPixelColor(i, c);
        }
    }
    else  {
        for(int i=11;i<120;i+=24){
            strip.setPixelColor(i, c);
        }
    }
    strip.show();
    }

void yaw_test(uint32_t c){
    strip.setBrightness(255);
    uint16_t i,j;
    if(yaw_angle > 0 && yaw_angle <15) {
       for(int i=0;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
  }  
    else if (yaw_angle > 15 && yaw_angle <30) {
      for(int i=1;i<120;i+=24){
        strip.setPixelColor(i, c);
      }  
    }
    else if (yaw_angle > 30 && yaw_angle < 45) {
      for(int i=2;i<120;i+=24){
        strip.setPixelColor(i, c);
      }   
    }
    else if (yaw_angle >  45 && yaw_angle < 60) {
      for(int i=3;i<120;i+=24){
        strip.setPixelColor(i, c);
      }  
    }
    else if (yaw_angle > 60 && yaw_angle < 75) { 
      for(int i=4;i<120;i+=24){
        strip.setPixelColor(i, c);
      }  
    }
    else if (yaw_angle > 75 && yaw_angle < 90) { 
      for(int i=5;i<120;i+=24){
        strip.setPixelColor(i, c);
      } 
    }   
    else if (yaw_angle > 90 && yaw_angle < 105) {
      for(int i=6;i<120;i+=24){
        strip.setPixelColor(i, c);
      }  
    }
    else if (yaw_angle > 105 && yaw_angle < 120) {
      for(int i=7;i<120;i+=24){
        strip.setPixelColor(i, c);
      } 
    }
    else if (yaw_angle > 120 && yaw_angle < 135) { 
      for(int i=8;i<120;i+=24){
        strip.setPixelColor(i, c);
      }  
    }
    else if (yaw_angle > 135 && yaw_angle < 150) {
      for(int i=9;i<120;i+=24){
        strip.setPixelColor(i, c);
      }  
    }
    else if (yaw_angle > 150 && yaw_angle < 165) { 
      for(int i=10;i<120;i+=24){
        strip.setPixelColor(i, c);
      } 
    }
    else if (yaw_angle > 165 && yaw_angle < 180) { 
      for(int i=11;i<120;i+=24){
        strip.setPixelColor(i, c);
      } 
    }
    else if (yaw_angle > 180 && yaw_angle < 195) {
      for(int i=12;i<120;i+=24){
        strip.setPixelColor(i, c);
      }  
    }
    else if (yaw_angle > 195 && yaw_angle < 210) { 
      for(int i=13;i<120;i+=24){
        strip.setPixelColor(i, c);
      } 
    }
    else if (yaw_angle > 210 && yaw_angle < 225) { 
      for(int i=14;i<120;i+=24){
        strip.setPixelColor(i, c);
      } 
    }
    else if (yaw_angle > 225 && yaw_angle < 240) { 
      for(int i=15;i<120;i+=24){
        strip.setPixelColor(i, c);
      }  
    }
    else if (yaw_angle > 240 && yaw_angle < 255) { 
      for(int i=16;i<120;i+=24){
        strip.setPixelColor(i, c);
      }
    }
    else if (yaw_angle > 255 && yaw_angle < 270) { 
      for(int i=17;i<120;i+=24){
        strip.setPixelColor(i, c);
      } 
    }
    else if (yaw_angle > 270 && yaw_angle < 285) { 
      for(int i=18;i<120;i+=24){
        strip.setPixelColor(i, c);
      } 
    }
    else if (yaw_angle > 285 && yaw_angle < 300) { 
      for(int i=19;i<120;i+=24){
        strip.setPixelColor(i, c);
      }  
    }
    else if (yaw_angle > 300 && yaw_angle < 315) { 
      for(int i=20;i<120;i+=24){
        strip.setPixelColor(i, c);
      }  
    }
    else if (yaw_angle > 315 && yaw_angle < 330) { 
      for(int i=21;i<120;i+=24){
        strip.setPixelColor(i, c);
      }  
    }
    else if (yaw_angle > 330 && yaw_angle < 345) {
      for(int i=22;i<120;i+=24){
        strip.setPixelColor(i, c);
      } 
    }
    else if (yaw_angle > 345 && yaw_angle < 360) {
      for(int i=23;i<120;i+=24){
        strip.setPixelColor(i, c);
      } 
    }
    strip.show();

    }

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


void Blackout(){
  int i;
  for(i = 0;i<strip.numPixels();i++){
    strip.setPixelColor(i,0);
  }
  strip.show();
}

