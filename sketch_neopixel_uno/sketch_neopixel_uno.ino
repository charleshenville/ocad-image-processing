
#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 24
#define TIME_PER_PERCENT 100

// #ifdef __AVR__
//   #include <avr/power.h>
// #endif

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

double rawList[] = {0.0, 0.0, 0.0, 0.0, 0.029, 0.44, 0.97, 0.87, 0.478, 0.014, 0.009, 0.0, 0.0};
// [0, 0, 0, 0, 0, 50Y, 100Y, 100P, 50P, 0, 0, 0, 0]

void setup()
{
    // put your setup code here, to run once:
    pixels.begin();
}

void loop()
{
    // put your main code here, to run repeatedly:
    
    int last = 0;
    int lastColour = 1;
    for(int i =0 ; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, 0, 0, 0);
      pixels.show();
    }

    for (int i = 0; i < 13; i++)
    {
        if (rawList[i] >= 0.3 && rawList[i] <= 0.5)
        {
            pixels.setBrightness(255);
            iterateThruPercent(i, 50, last, &lastColour);
            last = 50;
        }
        else if (rawList[i] > 0.5)
        {
            pixels.setBrightness(255);
            iterateThruPercent(i, 100, last, &lastColour);
            last = 100;
        }
        else
        {
            if(last == 0){
              delay(50*TIME_PER_PERCENT);
            }
            iterateThruPercent(i, 0, last, &lastColour);
            last = 0;
        }

    }
    delay(1000);
}

void iterateThruPercent(int numList, int maxPercent, int last, int * lastColour){


    int flag = 1;
    if(last > maxPercent){
      flag=-1;
    }

    if(maxPercent == 0){
        
        for(int per = last; ((per < maxPercent && flag == 1) || (per > maxPercent && flag ==-1)); per+=flag){
            
            double intensity = ((double)(per))/(100);
            int inte = (int)(intensity*255);
            
            pixels.setBrightness(inte);
            pixels.show();

            delay(TIME_PER_PERCENT);
        }

        return;
    }

    if(numList <=6){

        if(*lastColour !=1){fadeToColor(pixels.Color((int)(((double)(last)/100)*252), (int)(((double)(last)/100)*124), (int)(((double)(last)/100)*214)), 
                                        pixels.Color((int)(((double)(last)/100)*250), (int)(((double)(last)/100)*155), (int)(((double)(last)/100)*0)), 60);}
        (*lastColour) = 1;

        for(int per = last; ((per < maxPercent && flag == 1) || (per > maxPercent && flag ==-1)); per+=flag){
            
            double intensity = ((double)(per))/(100);
            int R=(int)(intensity*250);
            int G=(int)(intensity*155);
            int B=(int)(intensity*0);

            for(int i =0 ; i<NUMPIXELS; i++){

                pixels.setPixelColor(i, R, G, B);
                pixels.show();
            }

            delay(TIME_PER_PERCENT);
        }
        return;

    }
    else{

        if(*lastColour != 2){fadeToColor(pixels.Color((int)(((double)(last)/100)*250), (int)(((double)(last)/100)*155), (int)(((double)(last)/100)*0)), 
                                        pixels.Color((int)(((double)(last)/100)*252), (int)(((double)(last)/100)*124), (int)(((double)(last)/100)*214)), 60);}
        (*lastColour) = 2;

        for(int per = last; (((per < maxPercent) && (flag == 1)) || ((per > maxPercent) && (flag ==-1))); per+=flag){
            
            
            double intensity = ((double)(per))/(100);
            int R=(int)(intensity*252);
            int G=(int)(intensity*124);
            int B=(int)(intensity*214);

            for(int i =0 ; i<NUMPIXELS; i++){

                pixels.setPixelColor(i, R, G, B);
                pixels.show();

            }

            delay(TIME_PER_PERCENT);
        }
        return;
    }
}

void fadeToColor(uint32_t c1, uint32_t c2, int wait) {

  int r1 = (c1 >> 16) & 0xFF;
  int g1 = (c1 >> 8) & 0xFF;
  int b1 = c1 & 0xFF;

  int r2 = (c2 >> 16) & 0xFF;
  int g2 = (c2 >> 8) & 0xFF;
  int b2 = c2 & 0xFF;

  for (float t = 0.0; t < 1.0; t += 0.01) {
    int r = (int)(r1 + t * (r2 - r1));
    int g = (int)(g1 + t * (g2 - g1));
    int b = (int)(b1 + t * (b2 - b1));

    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, r, g , b);
    }

    pixels.show();
    delay(wait);
  }
}

