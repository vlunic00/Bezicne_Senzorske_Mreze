/*
  APDS9960 - Gesture Sensor
  This example reads gesture data from the on-board APDS9960 sensor of the
  Nano 33 BLE Sense and prints any detected gestures to the Serial Monitor.
  Gesture directions are as follows:
  - UP:    from USB connector towards antenna
  - DOWN:  from antenna towards USB connector
  - LEFT:  from analog pins side towards digital pins side
  - RIGHT: from digital pins side towards analog pins side
  The circuit:
  - Arduino Nano 33 BLE Sense
  This example code is in the public domain.
*/

#include <Arduino_APDS9960.h>

int result = 0;
int ourScore = 0;
int oppScore = 0;

void calculateScore(int result)
{
  if (result == 1)
    ourScore++;
  else if (result == 0)
    oppScore++;
}

void printScore()
{
  Serial.println("You:");
  Serial.println(ourScore);
  Serial.println("Opponent");
  Serial.println(oppScore);
}

int opponentChoice()
{
  int oppChoice = random(3);

  return oppChoice;
}

int calculateResult(int ourChoice, int oppChoice)
{
  switch (ourChoice)
  {
  case 0:
    if (oppChoice == 0)
    {
      Serial.println("Rock - Rock: It's a draw!");
      return 2;
    }

    else if (oppChoice == 1)
    {
      Serial.println("Rock - Scissors: You win!");
      return 1;
    }
    else
    {
      Serial.println("Rock - Paper: You lose!");
      return 0;
    }
    break;

  case 1:
    if (oppChoice == 0)
    {
      Serial.println("Scissors - Rock: You lose!");
      return 0;
    }

    else if (oppChoice == 1)
    {
      Serial.println("Scissors - Scissors: It's a draw!");
      return 2;
    }

    else
    {
      Serial.println("Scissors - Paper: You win!");
      return 1;
    }

    break;

  case 2:
    if (oppChoice == 0)
    {
      Serial.println("Paper - Rock: You win");
      return 1;
    }

    else if (oppChoice == 1)
    {
      Serial.println("Paper - Scissors: You lose!");
      return 0;
    }

    else
    {
      Serial.println("Paper - Paper: It's a draw!");
      return 2;
    }
    break;

  default:
    return 2;
  }
}

void setup()
{
  Serial.begin(9600);
  // in-built LED
  pinMode(LED_BUILTIN, OUTPUT);
  // Red
  pinMode(LEDR, OUTPUT);
  // Green
  pinMode(LEDG, OUTPUT);
  // Blue
  pinMode(LEDB, OUTPUT);

  randomSeed(analogRead(0));

  while (!Serial)
    ;
  if (!APDS.begin())
  {
    Serial.println("Error initializing APDS9960 sensor!");
  }
  // for setGestureSensitivity(..) a value between 1 and 100 is required.
  // Higher values makes the gesture recognition more sensible but less accurate
  // (a wrong gesture may be detected). Lower values makes the gesture recognition
  // more accurate but less sensible (some gestures may be missed).
  // Default is 80
  // APDS.setGestureSensitivity(80);
  Serial.println("Welcome to Rock, Paper, Scissors - powered by Arduino Nano 33 BLE");
  Serial.println("For Rock swipe UP, for Scissors swipe RIGHT and for Paper swipe LEFT");
  Serial.println("To reset the score swipe DOWN");
  Serial.println("Good luck, you're gonna need it!");
  Serial.println("Detecting gestures ...");
  // Turining OFF the RGB LEDs
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}
void loop()
{
  if (APDS.gestureAvailable())
  {

    // a gesture was detected, read and print to serial monitor
    int gesture = APDS.readGesture();
    switch (gesture)
    {
    case GESTURE_UP:
      result = calculateResult(0, opponentChoice());
      calculateScore(result);
      printScore();
      digitalWrite(LEDR, LOW);
      delay(1000);
      digitalWrite(LEDR, HIGH);
      break;
    case GESTURE_DOWN:
      Serial.println("Reseting score");
      ourScore = 0;
      oppScore = 0;
      printScore();
      digitalWrite(LEDG, LOW);
      delay(1000);
      digitalWrite(LEDG, HIGH);
      break;
    case GESTURE_LEFT:
      result = calculateResult(2, opponentChoice());
      calculateScore(result);
      printScore();
      digitalWrite(LEDB, LOW);
      delay(1000);
      digitalWrite(LEDB, HIGH);
      break;
    case GESTURE_RIGHT:
      result = calculateResult(1, opponentChoice());
      calculateScore(result);
      printScore();
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      break;
    default:
      break;
    }
  }
}