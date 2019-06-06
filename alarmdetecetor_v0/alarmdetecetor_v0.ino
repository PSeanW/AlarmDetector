#include <arduinoFFT.h>
arduinoFFT FFT = arduinoFFT();

#define SAMPLES 256              //Must be a power of 2
#define SAMPLING_FREQUENCY 10000 //Hz, must be 10000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT.
#define amplitude 5

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = D4;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

unsigned int sampling_period_us;
unsigned long microseconds;
byte peak[] = {0, 0, 0, 0, 0, 0, 0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
  //FFT.Revision();
  //Serial.println( "0.1 0.2 0.5 1K  2K  4K  8K");

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
  int val = 0;
  _Bool flag = 0;
  for (int i = 70; i < 76; i++) {
    val = (int)vReal[i];
    if (val > 70) {
      flag = 1;
    } 
    //Serial.print (i);
    //Serial.print (", ");
    //Serial.println(val);
    delay(100);
  }
  if(flag == 1){
    digitalWrite(ledPin, LOW);
  }else{
    digitalWrite(ledPin, HIGH);
    }
    Serial.print ((int)flag);
}
