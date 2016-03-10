#define XRANGE  50
int x, gss;

int sensor = 0;
int rojo = 5;
int amarillo = 6;
int verde = 7;
unsigned long paso;

void setup() {
  Serial.begin(9600);

  pinMode(rojo, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(verde, OUTPUT);

  digitalWrite(rojo, HIGH);
  digitalWrite(amarillo, HIGH);
  digitalWrite(verde, LOW);

  paso = millis() / 1000;
}


void loop() {
  int aValue = analogRead(sensor);
  x = map(aValue, 0, 1024, 0, XRANGE);
  gss = map(aValue, 102, 922, -640, 640);

  if (abs(gss) > 15) {
    paso = millis() / 1000;
  }
  
  unsigned long diff = millis() / 1000 - paso;
  if (diff > 10) {
    digitalWrite(verde, LOW);
    digitalWrite(amarillo, HIGH);
    digitalWrite(rojo, HIGH);
  } else if (diff > 5) {
    digitalWrite(verde, HIGH);
    digitalWrite(amarillo, LOW);
    digitalWrite(rojo, HIGH);
  } else {
    digitalWrite(verde, HIGH);
    digitalWrite(amarillo, HIGH);
    digitalWrite(rojo, LOW);
  }

//  Serial.print("|");
//  for (int i = 0; i < x; i++) {
//    if (i == XRANGE / 2 - 1)Serial.print("|");
//    else Serial.print("-");
//  }
//  Serial.print("O");
//  for (int i = x + 1; i < XRANGE; i++) {
//    if (i == XRANGE / 2 - 1)Serial.print("|");
//    else Serial.print("-");
//  }
//  Serial.print("|");
//  Serial.print(gss);
//  Serial.print("Gauss ");
//  Serial.println(diff);
}
