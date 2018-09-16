

#include <SPI.h>
#include <SparkFunDS3234RTC.h>

int dataPin = 6;    //SER, pin 14
int enablePin = 5; //OE', pin 13
int latchPin = 4;  //RCLK, pin 12
int clockPin = 3;  //SRCLK, pin 11


//ard pin 10 - 1
//ard pin 12 - 19
//ard pin 13 - 18
//ard pin 11 - 17
int chipSelect = 10;

int avg[10];
int pos = 0;

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(enablePin, OUTPUT);
  analogWrite(enablePin, 250);

  writeTime(0xFFFFFF);
  for (int i = 0; i < 10; i++)
    avg[i] = analogRead(0);

  for (int i = 255; i >= 100; i--) {
    analogWrite(enablePin, i);
    delay(15);
  }

  rtc.begin(chipSelect);

  //sec, min, hour, day, date, month, year
  rtc.setTime(0, 00, 12, 1, 5, 8, 18);

  rtc.set12Hour();
  rtc.update();
  
  Serial.begin(9600);
}

long houri = 0x0000;
long side = 0x0000;
long mini = 0x0000;
long minuted = 0x0000;
long oclock = 0x0000;

long testout = 0x0001;

long total = 0;

void loop() {
  static int newMin = -1;
  static int lastMin = -1;

  avg[pos] = readBrightness();
  pos++;
  if (pos >= 10)
    pos = 0;

  int avgValue = 0;
  for (int i = 0; i < 10; i++)
    avgValue += avg[i];

  int brightness = 200 + ((avgValue * 55.0) / 10240.0);
  
  analogWrite(enablePin, brightness);
  Serial.print(brightness);
  Serial.print("  :  ");
  Serial.print(avgValue / 10.0);
  Serial.print("\n");

  rtc.update();

  if (rtc.minute() != lastMin) {
    lastMin = rtc.minute();
    updateThings();
    writeTime(total);

    if ((int)((rtc.minute() + 2) / 5) != newMin) {
      for (int i = 100; i <= 235; i++) {
        analogWrite(enablePin, i);
        delay(20);
      }
      newMin = (int)((rtc.minute() + 2) / 5);
    }
  }

  delay(1000);
}

void updateThings() {
  
  int hourn = rtc.hour();
  if (rtc.minute() >= 33)
    hourn++;

  if (hourn == 13)
    hourn = 1;
  
  switch (hourn) {
    case 12: houri =  0x0001; break;
    case 11: houri = 0x0010; break;
    case 10: houri = 0x0008; break;
    case 9: houri =  0x0004; break;
    case 8: houri =  0x0080; break;
    case 7: houri =  0x0040; break;
    case 6: houri =  0x0020; break;
    case 5: houri =  0x0400; break;
    case 4: houri =  0x0200; break;
    case 3: houri =  0x0100; break;
    case 2: houri =  0x4000; break;
    case 1: houri =  0x2000; break;
  }

  if (rtc.minute() < 33)
    side = 0x0800;
  else
    side = 0x1000;

   minuted = 0x010000;
  
  switch (((int)(rtc.minute() + 2)) / 5) {
    case 0: mini = 0x0000; minuted = 0x0000; side = 0x0000; break;
    case 12: mini = 0x0000; minuted = 0x0000; side = 0x0000; break;
    
    case 1: mini = 0x8000; break;
    case 11: mini = 0x8000; break;
    
    case 2: mini = 0x100000; break;
    case 10: mini = 0x100000; break;
    
    case 3: mini = 0x020000; minuted = 0x0000; break;
    case 9: mini = 0x020000; minuted = 0x0000; break;
    
    case 4: mini = 0x040000; break;
    case 8: mini = 0x040000; break;
    
    case 5: mini = 0x048000; break;
    case 7: mini = 0x048000; break;
    
    case 6: mini = 0x200000; minuted = 0x0000; break;
  }

  if (((int)(rtc.minute() + 2)) / 5 == 0 || ((int)(rtc.minute() + 2)) / 5 == 12) {
    oclock = 0x0002;
  } else {
    oclock = 0x0000;
  }

  total = 0x080000 + mini + minuted + side + houri + oclock;
  Serial.print("Time: ");
  Serial.print(rtc.hour());
  Serial.print(" : ");
  Serial.println(rtc.minute());
}

void writeTime(long output) {

    Serial.print("Output: ");
    Serial.println(output, HEX);
  
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, lowByte(output));

    delay(3);
    
    shiftOut(dataPin, clockPin, LSBFIRST, lowByte(output >> 8));

    delay(3);
    
    shiftOut(dataPin, clockPin, LSBFIRST, lowByte(output >> 16));

    delay(3);
    
    digitalWrite(latchPin, HIGH);
}

int readBrightness() { return analogRead(0); }

