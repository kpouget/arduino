int led = 13 ; // LED on arduino
int digitalPin = 3; // linear Hall magnetic sensor digital interface
int analogPin = A0; // linear Hall magnetic sensor analog interface
int digitalVal ; // digital readings
int analogVal; // analog readings
void setup ()
{
  pinMode (led, OUTPUT);
  pinMode (digitalPin, INPUT);
  //pinMode(analogPin, INPUT);
  Serial.begin(9600);
}
void loop ()
{
  // Read the digital interface
  analogVal = analogRead(analogPin);

  digitalWrite (led, analogVal < 500 ? HIGH : LOW);

  // Read the analog interface
  analogVal = analogRead(analogPin);
  Serial.println(analogVal); // print analog value
  delay(100);
}
