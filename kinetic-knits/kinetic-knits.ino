#include <Servo.h>

const int numPairs = 9;

// Assign digital pins for each pair (you can change them)
const int servoPins[numPairs] = {2, 3, 4, 5, 6, 7, 8, 9, 10};

// Create servo objects for both servos in each pair
Servo servoA[numPairs];
Servo servoB[numPairs];

// Each pair has its own random behavior variables
int currentAngle[numPairs];
int targetAngle[numPairs];
int minAngle[numPairs];
int maxAngle[numPairs];
int moveDirection[numPairs];
unsigned long nextMoveTime[numPairs];
unsigned long moveDuration[numPairs];
unsigned long moveStartTime[numPairs];
bool isMoving[numPairs];

void setup() {
  randomSeed(analogRead(A0)); // random seed from noise

  for (int i = 0; i < numPairs; i++) {
    servoA[i].attach(servoPins[i]);
    servoB[i].attach(servoPins[i]); // both on same signal wire (shared)
    currentAngle[i] = random(15, 8);   // start at random angle
    servoA[i].write(currentAngle[i]);
    servoB[i].write(currentAngle[i]);

    minAngle[i] = random(0, 30);        // each pair has its own min range
    maxAngle[i] = random(60, 90);       // and its own max range
    moveDirection[i] = 1;

    nextMoveTime[i] = millis() + random(1000, 7000);  // random initial delay
    isMoving[i] = false;
  }
}

void loop() {
  unsigned long now = millis();

  for (int i = 0; i < numPairs; i++) {

    if (!isMoving[i] && now >= nextMoveTime[i]) {
      // Start a new move
      targetAngle[i] = (moveDirection[i] == 1) ? maxAngle[i] : minAngle[i];
      moveDirection[i] *= -1; // reverse next time

      //moveDuration[i] = random(1000, 3000); // move duration between 1–3s
      moveDuration[i] = random(1000, 5000); // move duration between 1–3s
      moveStartTime[i] = now;
      isMoving[i] = true;
    }

    if (isMoving[i]) {
      float progress = (float)(now - moveStartTime[i]) / moveDuration[i];
      if (progress >= 1.0) {
        progress = 1.0;
        isMoving[i] = false;
        nextMoveTime[i] = now + random(1000, 17000); // pause 1–4s
      }

      int newAngle = currentAngle[i] + (targetAngle[i] - currentAngle[i]) * progress;
      servoA[i].write(newAngle);
      servoB[i].write(newAngle);

      if (!isMoving[i]) {
        currentAngle[i] = targetAngle[i];
      }
    }
  }
}
