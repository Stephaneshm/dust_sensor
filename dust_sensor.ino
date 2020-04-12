//version 0.1a
int dustPin = 0; // dust sensor - Arduino A0 pin
int ledPin = 2;
 
float voltsMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
 
void setup()
{
  Serial.begin(57600);
  pinMode(ledPin,OUTPUT);
}
 
 
void loop()
{
  digitalWrite(ledPin,LOW); // power on the LED
  delayMicroseconds(280);
 
  voltsMeasured = analogRead(dustPin); // read the dust value
 
  delayMicroseconds(40);
  digitalWrite(ledPin,HIGH); // turn the LED off
  delayMicroseconds(9680);
 
  //measure your 5v and change below
  calcVoltage = voltsMeasured * (5.0 / 1024.0);
  dustDensity = 0.17 * calcVoltage - 0.1;
  Serial.println("GP2Y1010AU0F readings");
  Serial.print("Raw Signal Value = ");
  Serial.println(voltsMeasured);
  Serial.print("Voltage = ");
  Serial.println(calcVoltage);
  Serial.print("Dust Density = ");
  Serial.println(dustDensity); // mg/m3
  Serial.println("");
  delay(1000);
}
