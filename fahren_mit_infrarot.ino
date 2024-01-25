#include <SharpIR.h>

#define trigPin   10
#define echoPin   13
#define StartPin  3
#define StopPin   2
#define IRPin_Right     A0
#define IR_Right_Model  GP2Y0A21YK0F
#define IRPin_Left      A1
#define IR_Left_Model   GP2Y0A21YK0F

bool Program_state = false;
int IN1 = 4;
int IN2 = 5;
int IN3 = 6;
int IN4 = 7;
const int enA = 9;
const int enB = 8;

SharpIR leftsensor(SharpIR::IR_Left_Model, IRPin_Left);
SharpIR rightsensor(SharpIR::IR_Right_Model, IRPin_Right);

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(StopPin, INPUT_PULLUP);
  pinMode(StartPin, INPUT_PULLUP);
}

void loop() {
  float duration, distance;
  int leftdistance = leftsensor.getDistance();
  int rightdistance = rightsensor.getDistance();

  // Stoppbedingungen
  if (digitalRead(StopPin) == LOW) {
    Program_state = true;
    Serial.println("Stop");
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  }

  if (digitalRead(StartPin) == LOW) {
    Program_state = false;
    digitalWrite(enA, 0);
    digitalWrite(enB, 0);
    Serial.println("Startpin");
  }

  if (Program_state == false) {
    Serial.println("Continue");
  } else {
    Serial.println("Stop");
    return;
  }

  // Regelung basierend auf den IR-Sensorwerten
  int diff = rightdistance - leftdistance;

  Serial.print("Left Distance: ");
  Serial.print(leftdistance);
  Serial.print(" | Right Distance: ");
  Serial.print(rightdistance);
  Serial.print(" | Difference: ");
  Serial.println(diff);


  if (diff > 10) {
    // Rechtskurve
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(enA, 150);
    analogWrite(enB, 100);
  } else if (diff < -10) {
    // Linkskurve
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(enA, 50);
    analogWrite(enB, 150);
  } else {
    // Geradeaus fahren
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(enA, 150);
    analogWrite(enB, 150);
  }


  // Ultraschallmessung und Hinderniserkennung
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) * 0.0344;

  if (distance >= 400 || distance <= 2) {
    Serial.println("Distance = Out of range");
  } else {
    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  // Wenn ein Hindernis nahe ist, anhalten
  if (distance < 20) {
    Serial.println("Hindernis erkannt - Anhalten!");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);

}
}
