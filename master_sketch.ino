//master 

#include <Wire.h> 

//slave address
#define SLAVE_ADDR 9   // address on the R3
#define ANSWERSIZE 5
#define NUM_SAMPLES 2 
#define TX_PERIOD_MS 50


const int Yaxis = A0 ; 
const int Xaxis = A1 ;
int vry = 0;
int vrx = 0;

int8_t array_buffer [2];  // this will store my two values let y= [0] x = [1]
unsigned long lastTx = 0;

int readAverage(int pin) {
  long sum = 0 ;
  for (uint8_t i = 0 ; i < NUM_SAMPLES ; i++ ){
    sum += analogRead(pin);
    delayMicroseconds(200);
  }
  return sum / NUM_SAMPLES;
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Wire.begin(); //master by default 
Serial.println("Master initialized with averaging");



}

void loop() {
  // put your main code here, to run repeatedly:

delay(50);
Serial.println("reading pot");


vrx = readAverage(Xaxis);
vry = readAverage(Yaxis); 
Serial.print("current values: ");
Serial.print("Y-axis:");
Serial.print(vry);
Serial.print(" X-axis: ");
Serial.println(vrx);

///////////////////////////////
///pack the data into array////
//   map() return a long so we cast into int8_t which is
//   always 8bit 
array_buffer [0] = (int8_t) map ( vry , 0 , 1023 , -127 , 127 );
array_buffer [1] = (int8_t) map ( vrx , 0 , 1023 , -127 , 127 );

Serial.print("mapped values as: ");
Serial.print("Y-axis: ");
Serial.print(array_buffer[0]);
Serial.print("  X-axis: ");
Serial.println(array_buffer[1]);




if (millis() - lastTx >= TX_PERIOD_MS) {
    Serial.println("writing to slave");
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write((uint8_t*)array_buffer, sizeof(array_buffer));
    Wire.endTransmission();
    lastTx = millis();

    // Debug prints (optional)
    Serial.print("Y mapped: "); Serial.print(array_buffer[0]);
    Serial.print("  X mapped: "); Serial.println(array_buffer[1]);
  }

}
