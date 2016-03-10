#include <Servo.h> 
#include <EEPROM.h>

struct TRUNOUT {
  int servoPin;
  int buttonsPin;
  int relePin;
  byte posGREEN;
  byte posRED;
};

int T = 1;
TRUNOUT motors[] = {{3,2,4,90,90}};
Servo s[1];

int SETUPPIN = 2;

int GREEN = 2;
int RED = 1;

// -------
void setup() {
  Serial.begin(9600);
  for (int i=0 ; i<T ; i++) {
    motors[i].posGREEN = EEPROM.read(i*4);
    motors[i].posRED = EEPROM.read(i*4+1);
    
    if (motors[i].posGREEN == 0) {
      motors[i].posGREEN = 90;
    }
    if (motors[i].posRED == 0) {
      motors[i].posRED = 90;
    }
    
    s[i].attach(motors[i].servoPin);
    moveServo(i, motors[i].posGREEN);
    pinMode(motors[i].relePin, OUTPUT);
  }
  
  pinMode(SETUPPIN, INPUT);
}

// -------
void loop() { 
  if (digitalRead(SETUPPIN)==HIGH) {
    digitalWrite(LED_BUILTIN, ((millis()/1000)%2));
    prog();
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    normal();
  }
  delay(100);
}

// -------
void prog() {
  int buttonsValues[T];
  readButtons(buttonsValues);
  int button = readButton(0);
  int change = 0 ;
  if (button == GREEN) {
    change = 1;
  } else if (button == RED) {
    change = -1;
  }
  
  for (int i=0 ; i<T ; i++) {
    int pos = buttonsValues[i];
    if (pos==GREEN) {
      motors[i].posGREEN = normalize(motors[i].posGREEN + change);
      moveServo(motors[i].servoPin, motors[i].posGREEN);
      EEPROM.write(i*4, motors[i].posGREEN);
    } else if (pos==RED) {
      motors[i].posRED = normalize(motors[i].posRED + change);
      moveServo(motors[i].servoPin, motors[i].posRED);
      EEPROM.write(i*4+1, motors[i].posRED);
    }
    
    Serial.print("m.posGREEN = " );
    Serial.print(motors[i].posGREEN);
    Serial.print("\t m.posRED = " );
    Serial.println(motors[i].posRED);
  }
}

// -------
void normal() {
  int buttonsValues[T];
  readButtons(buttonsValues);

  for (int i=0 ; i<T ; i++) {
    TRUNOUT m = motors[i];
    int pos = buttonsValues[i];
    if (pos==GREEN) {
      moveServo(m.servoPin, m.posGREEN);
      digitalWrite(m.relePin, LOW);
    } else if (pos==RED) {
      moveServo(m.servoPin, m.posRED);
      digitalWrite(m.relePin, HIGH);
    }
  }
}

// -------
int readButton(int pin) {
  int sensorValue = analogRead(pin);
  int outputValue = 0;
  if(sensorValue>750) {
    outputValue = 2;
  } else if(sensorValue>250) {
    outputValue = 1;
  }

/*  Serial.print("pin = " );
  Serial.print(pin);
  Serial.print("\t sensor = " );
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);
  */
  
  return outputValue;
}

// -------
void readButtons(int res[]) {
  for (int i=0 ; i<T ; i++) {
    TRUNOUT m = motors[i];
    res[i] = readButton(m.buttonsPin);
  }
}

// -------
void moveServo(int i, int pos) {
  if(s[i].attached()) {
    s[i].write(pos);
  } else {
    Serial.println("KO");
  }
}

// -------
int normalize(int v) {
  return limits(v, 0, 180);
}

// -------
int limits(int v, int l, int h) {
  if (v < l) {
    return l; 
  } else if (v > h) {
    return h;
  } else {
    return v;
  }
}
