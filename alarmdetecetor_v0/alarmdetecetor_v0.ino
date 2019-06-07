#include <arduinoFFT.h>
arduinoFFT FFT = arduinoFFT();

#define SAMPLES 256              //Must be a power of 2
#define SAMPLING_FREQUENCY 10000 //Hz, must be 10000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT.
//#define amplitude 5

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = D4;      // select the pin for the LED

unsigned int sampling_period_us;
unsigned long microseconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;
unsigned int count_low = 0;
unsigned int count_high = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros() - oldTime;
    //Serial.println(newTime);
    oldTime = newTime;
    vReal[i] = analogRead(sensorPin); // A conversion takes about 1mS on an ESP8266
    vImag[i] = 0;
    while (micros() < (newTime + sampling_period_us)) {
      /* do nothing to wait */
    }
  }
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  //double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
  //Serial.print("Peak = ");
  //Serial.println( peak, 6);
  delay(15);//???delay to stablize the signal???
  int val;
  _Bool flag = 0;
  for (int i = 66; i < 76; i++) { //70-76 base on test result
    val = (int)vReal[i];
    if (val > 65) {
      flag = 1;
    }
    //Serial.print (i);
    //Serial.print (", ");
    //Serial.println(val);
    delay(50);
  }
  //Serial.println ((int)flag);
  //set
  if (flag == 1) {
    //if (count_high >= 2) {
      count_low = 0;
       //Serial.print (count_high);
      digitalWrite(ledPin, LOW);
    //} else {
      //count_high++;
    //}
  } else {                    //no alarm detected
    if (count_low >= 5) {
      //count_high = 0;
      digitalWrite(ledPin, HIGH);
    } else {
      count_low++;
    }
  }
   //Serial.print(count_low);
   //Serial.print(" @ ");
   //Serial.println(count_high);
}
