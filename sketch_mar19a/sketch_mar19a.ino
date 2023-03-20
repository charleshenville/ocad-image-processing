#include <Adafruit_CircuitPlayground.h>

double rawList[] = {0.0, 0.0, 0.0, 0.0, 0.029, 0.44, 0.97, 0.87, 0.478, 0.014, 0.009, 0.0, 0.0};

void setup() {
  // put your setup code here, to run once:
  CircuitPlayground.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  for(int i = 0; i<13; i++){

    delay(5000);
    if(i<=6){
      if(rawList[i]>=0.3 && rawList[i]<=0.5){
        for(int j = 0; j<10; j++){
        CircuitPlayground.setPixelColor(j, 125, 78, 0);
        }
      }
      else if(rawList[i]>0.5){
        for(int j = 0; j<10; j++){
        CircuitPlayground.setPixelColor(j, 250, 155, 0);
        }
      }
      else{
        for(int j = 0; j<10; j++){
        CircuitPlayground.setPixelColor(j, 0, 0, 0);
        }
      }
    }
    else{
      if(rawList[i]>=0.3 && rawList[i]<=0.5){
        for(int j = 0; j<10; j++){
        CircuitPlayground.setPixelColor(j, 126, 62, 107);
        }
      }
      else if(rawList[i]>0.5){
        for(int j = 0; j<10; j++){
        CircuitPlayground.setPixelColor(j, 252, 124, 214);
        }
      }
      else{
        for(int j = 0; j<10; j++){
        CircuitPlayground.setPixelColor(j, 0, 0, 0);
        }
      }
    }

    
  }

  delay(1000); // wait for 1 second
}
