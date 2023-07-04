
// Declaration and initialization of the input pins
int Analog_Input = A0; // Analog output of the sensor
int Digital_Input = 3; // Digital output of the sensor
int prev = 0;
void setup ()
{
  pinMode (Analog_Input, INPUT);
  pinMode (Digital_Input, INPUT);

  Serial.begin (9600); // Serial output with 9600 bps
}

// The program reads the current values of the input pins
// and outputs them to the serial output
void loop ()
{
  int Analog;
  int Digital;

  //Actual values are read, converted to the voltage value....
  Analog = analogRead (Analog_Input) ;
  Digital = digitalRead (Digital_Input);

  if (Analog != prev) {
    //... and output at this position
    Serial.print (" D0: "); Serial.print (Digital);
    Serial.print (" A0: "); Serial.print (Analog);
    Serial.println (""); ;
    prev = Analog;
  }
  //Serial.println ("----------------------------------------------------------------");
  delay (100);
}
