#include <Adafruit_NeoPixel.h>

char receivedData[256];

#define mySerial Serial1

#define PIN 6

int sensorPin = A0;

uint32_t globalLEDcolor;



Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PIN, NEO_GRB + NEO_KHZ800);



void setup()

{

  // Open serial communications and wait for port to open:

  mySerial.begin(9600);

  strip.begin();

  globalLEDcolor = strip.Color(255, 9, 87);

  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(170);

}



void loop() // run over and over

{

//test Print lines here

  Serial.println("ajayveer");

  memset(receivedData, 0, sizeof(receivedData));

  while (!mySerial.available());   // stay here so long as COM port is empty

  int n  = mySerial.readBytes(receivedData, sizeof(receivedData));

  receivedData[n] = '\0';

  Serial.println(receivedData);

  char * bufferPointer = receivedData;

  

  switch(receivedData[0]) {

	case 'A':

		bufferPointer++;

		lightIndividualLEDS(bufferPointer); 

		break;

	case 'B':

		lightLEDWithGlobalColor(10);

		break;

	case 'C':

		colorWipe(strip.Color(0, 0, 0), 0);

		break;

	case 'D':
		bufferPointer++;
		updateGlobalColor(bufferPointer);
		break;

      case 'P': 
                rainbow(20);
                break;
      
      
      case 'Q': 
                rainbowCycle(20);
                break;
      
      
      case'R':
               theaterChaseRainbow(50); 
               break;

  }

}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
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

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


void lightIndividualLEDS(char * ptr) {

	char * led = strtok(ptr, ",");

	while(led != 0) {

		int ledAddress = atoi(led);

		strip.setPixelColor(ledAddress, globalLEDcolor);

		strip.show();

		led = strtok(0, ",");

	}

}



void updateGlobalColor(char * ptr) {

	char * color = strtok(ptr, ",");

	uint8_t red = atoi(color);

	

	color = strtok(0, ",");

	uint8_t green = atoi(color);

	

	color = strtok(0, ",");

	uint8_t blue = atoi(color);

	

	globalLEDcolor = strip.Color(red, green, blue);

}



void lightLEDWithGlobalColor(uint8_t wait) {

  for(uint16_t i=0; i<strip.numPixels(); i++) {

      strip.setPixelColor(i, globalLEDcolor);

      strip.show();

      delay(wait);

  }

}

// Fill the dots one after the other with a color

void colorWipe(uint32_t c, uint8_t wait) {

  for(uint16_t i=0; i<strip.numPixels(); i++) {

      strip.setPixelColor(i, c);

      strip.show();

      delay(wait);

  }

}
