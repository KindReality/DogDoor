#define lowerEchoPin 2
#define lowerTrigPin 3
#define upperEchoPin 20
#define upperTrigPin 21

#define audioPin 10


int state = 0;// 0 waiting, 1 low, 2 alarm
unsigned long last = 0;
unsigned long lastLower = 0;
unsigned long lastUpper = 0;
unsigned long lastAlarm = 0;
float triggerDistance = 30;
unsigned long tolerance = 100;

void setup() {
  //lower
  pinMode(lowerTrigPin, OUTPUT);
  pinMode(lowerEchoPin, INPUT);
  //upper
  pinMode(upperTrigPin, OUTPUT);
  pinMode(upperEchoPin, INPUT);
  //
  pinMode(audioPin, OUTPUT);
  Serial.begin(9600);
  lastLower = millis();
}

void loop() {
  //
  unsigned long duration;
  //pulse
  digitalWrite(lowerTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(lowerTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(lowerTrigPin, LOW);
  //listen
  duration = pulseIn(lowerEchoPin, HIGH);
  //calculate
  float lowerDistance = ((float)duration * 0.034 / (float)2) * 0.393701; // Speed of sound wave divided by 2 times .393701 to convert to inches
  //
  //pulse
  digitalWrite(upperTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(upperTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(upperTrigPin, LOW);
  //listen
  duration = pulseIn(upperEchoPin, HIGH);
  //calculate
  float upperDistance = ((float)duration * 0.034 / (float)2) * 0.393701; // Speed of sound wave divided by 2 times .393701 to convert to inches
  //
  //
  last = micros();
  //
  //
  if (lowerDistance < triggerDistance) {
    lastLower = last;
  }
  if (upperDistance < triggerDistance) {
    lastUpper = last;
  }
  //
  //
  if (state == 0) {
    Serial.println("state 0");
    lightOff();
    if (lastLower == last) {
      state = 1;
    }
  //
  //
  } else if (state == 1) {
    Serial.println("state 1");
    lightOn();
    if (((lastLower > lastUpper) ? lastLower - lastUpper : lastUpper - lastLower) < 2000000) {//disarm 
      state = 0;
    } else {
    }
    
    if (last - lastLower > 1000000) {//alarm
      lastAlarm = last;
      state = 2;
    }
  //
  //
  } else if (state == 2) {
    Serial.println("state 3");
    digitalWrite(audioPin, HIGH);
    if (last - lastAlarm > 500000) {
      digitalWrite(audioPin, LOW);
      state = 0;
    }
  }
  Serial.print(" Lower:");
  Serial.print(lowerDistance);
  Serial.println("in ");
  Serial.print(" Upper:");
  Serial.print(upperDistance);
  Serial.println("in ");
  Serial.println();
}

void lightOn(){
  digitalWrite(17, HIGH);
  digitalWrite(30, HIGH);
}

void lightOff(){
  digitalWrite(17, LOW);
  digitalWrite(30, LOW);
}
