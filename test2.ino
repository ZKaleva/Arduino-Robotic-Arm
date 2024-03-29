#include <SoftwareSerial.h>

#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);
int pos = 90;
int posMin = 70;
int posMax = 110;

// First pin positions
int currentFirstPos = 100;
int initialFirstPos = 100;
int firstPosMin = 65;
int firstPosMax = 110;
int lastFirstPos = 100;
// Second pin positions
int currentSecondPos = 90;
int initialSecondPos = 90;
int secondPosMin = 60;
int secondPosMax = 140;
// fourth pin positions
int currentFourthPos = 90;
int initialFourthPos = 90;
int fourthPosMin = 60;
int fourthPosMax = 140;
bool finishLoop = false;
//fifth
int currentZeroPos = 90;
int currentThirdPos = 90;
int pin[] = {9, 6, 5, 3, 11, 10};

const int PIN_COUNT = sizeof(pin) / sizeof(pin[0]);
Servo s[PIN_COUNT];
int control = 1;
#define RED 255, 0, 0
int tmpkey = 0;
bool is_first = true;
const int buttonPin = 4;
int flip = 0;
bool shouldResetLoop = false;
bool shouldStopLoop = false;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  // OLED test.
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(SSD1306_WHITE);//Sets the font display color
  display.clearDisplay();//cls
  display.setTextSize(2);
  display.setCursor(30,30);
  display.println(F("Hello!"));
  display.display();
  delay(3000);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
  exit(0);
  }

  if (shouldResetLoop) {
  shouldResetLoop = false;
  }

  attachAll();
  moveServoToPosition(1, 45, currentFirstPos, "Shoulder");
  moveServoToPosition(2, 170, currentSecondPos, "Forearm");
  moveServoToPosition(4, 160, currentFourthPos, "Gripper");
  moveServoToPosition(0, 160, currentZeroPos, "Board");
  moveServoToPosition(4, 90, currentFourthPos, "Gripper");
  moveServoToPosition(2, 100, currentSecondPos, "Forearm");
  moveServoToPosition(0, 90, currentZeroPos, "Board");
  moveServoToPosition(0, 180, currentZeroPos, "Board");
  moveServoToPosition(2, 170, currentSecondPos, "Forearm");
  moveServoToPosition(4, 160, currentFourthPos, "Gripper");
  moveServoToPosition(3, 45, currentThirdPos, "Wrist Pitch");
  moveServoToPosition(3, 100, currentThirdPos, "Wrist Pitch");
  moveServoToPosition(0, 90, currentZeroPos, "Board");
  moveServoToPosition(4, 90, currentFourthPos, "Gripper");
  moveServoToPosition(2, 65, currentSecondPos, "Forearm");
  moveServoToPosition(1, 110, lastFirstPos, "Gripper");

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Goodbye..");
  display.display();
  detachAll();
  exit(0);
}

void updateOLED() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Servo: ");
  display.print(tmpkey + 1);
  display.print(" Angle: ");
  display.print(s[tmpkey].read());
  display.display();
}

void attachOne(int pin) {
  s[tmpkey].attach(pin);
}
void detachOne() {
  s[tmpkey].detach();
}
void attachAll() {
  for (int i = 0; i < PIN_COUNT; ++i) {
    s[i].attach(pin[i]);
  }
}
void detachAll() {
 for (int i = 0; i < PIN_COUNT; ++i) {
  s[i].detach();
 }
 finishLoop = true;
}
void moveSingleServo(int servo, int angle) {
 s[servo].write(angle);
}
void moveServo(int angle) {
 s[tmpkey].write(angle);
}

void moveServoToPosition(int servo_id, int targetPosition, int &currentPos, String partName) {
  display.clearDisplay();
  display.setTextSize(1.7);
  display.setCursor(0, 0);

  while (currentPos != targetPosition) {

    if (targetPosition < currentPos) {
      if (servo_id == 2) {
        display.print(" Moving " + partName + " up...");
        Serial.println("Moving up...");
      // } else if (servo_id == 0) {
      //   display.print(" Turning " + partName + "right...");
      //   Serial.println("Turning right...");
      } else {
      display.print(" Moving " + partName + " down...");
      Serial.println("Moving down...");
      }
      currentPos -= 5;
    } else {
      if (servo_id == 2) {
        display.print(" Moving " + partName + " down...");
        Serial.println("Moving down...");
      // } else if (servo_id == 0) {
      //   display.print(" Turning " + partName + "left...");
      //   Serial.println("Turning left...");
        } else {
        display.print(" Moving " + partName + " up...");
      
       Serial.println("Moving up...");
      }
      currentPos += 5;
    }

   display.setCursor(0, 10);  // Adjust the position as needed
   display.print("Current Position: " + String(currentPos));
   display.setCursor(0,20); //Adjust the position as needed
   display.print("Current Part: " + partName);
   display.display();
   delay(10); 

   // display.display();
   // Serial.print("Current position: ");
   // Serial.println(currentPos);
   display.clearDisplay();

    moveSingleServo(servo_id, currentPos);
    delay(200);
  }
  delay(1000);
}
