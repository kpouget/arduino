#define trigPin 13
#define echoPin 12

void dist_setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long dist_read() {
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration/2) / 29.1;

  return distance;
}
