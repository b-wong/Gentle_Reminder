const int buzzerPin = 13; //vibrating motor on pin 13
const int buttonPin = 12; //increase button on pin 12
const int buttonPin2 = 11;//decrease button on pin 11
const int buttonPin3 = 10;//confirm on pin 10
const int LEDPin = A5;//LED on pin A5

int buttonState, buttonState2, buttonState3; //hold state HIGH or LOW of button
int timeChange = 30000;//30 second timer

unsigned long currentTime; //store time from when controller has started
unsigned long previousTime = 0; // previous time to use instead of delay
unsigned long offsetTime; // start of time when button is pressed
unsigned long timeReset; //the result of currentTime - offsetTime

unsigned long buttonPressTime  = 0; //delay to only check on button every so often
bool buttonPressed = false, buttonPressed2 = false, buttonPressed3 = false; // has a buttonPress been registered
unsigned long timeCounter = 0; // where we're gonna store the amount of time to wait before buzzing

bool timeSet = false; //bool to determine whether time has been set or not
int stateChange = 1; //determines if the timer has started

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT); // Set buzzerPin as an output
  pinMode(LEDPin, OUTPUT);// Set LEDPin as an output
  pinMode(buttonPin, INPUT_PULLUP); //Set buttonPin as input
  pinMode(buttonPin2, INPUT_PULLUP);//Set buttonPin as input
  pinMode(buttonPin3, INPUT_PULLUP);//Set buttonPin as input
}

void loop() {

  currentTime = millis();//Save current program time
  buttonState3 = digitalRead(buttonPin3); //Read the current state of the buttons
  buttonState2 = digitalRead(buttonPin2);
  buttonState = digitalRead(buttonPin);

  if ((buttonState == HIGH) && (buttonPressed == false) && (stateChange == 1)) {
    timeCounter += timeChange; //If pressed increase timer
    buttonPressed = true;
  }
  else if (buttonState == LOW) {//Check if button is pressed
    buttonPressed = false;
  }

  if ((buttonState2 == HIGH) && (buttonPressed2 == false) && (timeCounter - timeChange < ((2 ^ 32) - timeChange - 100)) && (stateChange == 1)) {
    timeCounter -= timeChange; //If pressed decrease time
    buttonPressed2 = true;
  }
  else if (buttonState2 == LOW) {//Check if button is pressed
    buttonPressed2 = false;
  }

  if ((buttonState3 == HIGH) && (buttonPressed3 == false)) {
    buttonPressed3 = true;
    switch (stateChange) {//Determine state
      case 1://If not started, start timer
        if (timeCounter != 0) {
          offsetTime = millis();
          timeSet = true;
          analogWrite(LEDPin, 255);//Turn on LED
          stateChange++;
        }
        break;
      case 2://If started, end timer
        offsetTime = 0;//Reset all variables
        timeSet = false;
        timeCounter = 0;
        previousTime = 0;
        digitalWrite(buzzerPin, LOW);//Turn off motor
        analogWrite(LEDPin, 0);//Turn off LED
        stateChange--;
        break;
    }
  }
  else if (buttonState3 == LOW) {//Check if button is pressed
    buttonPressed3 = false;
  }

  timeReset = (millis() - offsetTime);

  if (timeSet) {// If the counter reaches the correct time
    if (timeReset - previousTime >= timeCounter) {
      previousTime = timeReset;
      digitalWrite(buzzerPin, HIGH);//Turn on motor
      analogWrite(LEDPin, 0);//Turn off LED
    }
  }
}
