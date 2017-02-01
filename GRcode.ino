const int buzzerPin = 13; //buzzerPin on pin 13
const int buttonPin = 12; //button on pin 10
const int buttonPin2 = 11;
const int buttonPin3 = 10;
const int LEDPin = A5;

int buttonState, buttonState2, buttonState3; //hold state HIGH or LOW of button
int timeChange = 30000;//600000

unsigned long currentTime; //store time from when controller has started
unsigned long previousTime = 0; // previous time to use instead of delay
unsigned long offsetTime; // start of time when button is pressed
unsigned long timeReset; //the result of currentTime - offsetTime

unsigned long buttonPressTime  = 0; //delay to only check on button every so often
bool buttonPressed = false, buttonPressed2 = false, buttonPressed3 = false; // has a buttonPress been registered
unsigned long timeCounter = 0; // where we're gonna store the amount of time to wait before buzzing

bool timeSet = false; //bool to determine whether time has been set or not
int stateChange = 1;

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT); // Set buzzerPin as an output
  pinMode(LEDPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); //Set buttonPin as input
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
}

void loop() {

  currentTime = millis();
  buttonState3 = digitalRead(buttonPin3);


    buttonState = digitalRead(buttonPin);


  if ((buttonState == HIGH) && (buttonPressed == false) && (stateChange == 1)) {
    timeCounter += timeChange; //10 sec
    buttonPressed = true;
  }
  else if (buttonState == LOW) {
    buttonPressed = false;
  }


    buttonState2 = digitalRead(buttonPin2);


  if ((buttonState2 == HIGH) && (buttonPressed2 == false) && (timeCounter - timeChange < ((2^32)-timeChange - 100)) && (stateChange == 1)) {
    timeCounter -= timeChange; //10 sec
    buttonPressed2 = true;
  }
  else if (buttonState2 == LOW) {
    buttonPressed2 = false;
  }

  if ((buttonState3 == HIGH) && (buttonPressed3 == false)) {
    buttonPressed3 = true;
    switch (stateChange) {
      case 1:
        if (timeCounter != 0) {
          offsetTime = millis();
          timeSet = true;
          analogWrite(LEDPin, 255);
          stateChange++;
        }
        break;
      case 2:
        offsetTime = 0;
        timeSet = false;
        timeCounter = 0;
        previousTime = 0;
        digitalWrite(buzzerPin, LOW);
        analogWrite(LEDPin, 0);
        stateChange--;
        break;
    }
  }
  else if (buttonState3 == LOW) {
    buttonPressed3 = false;
  }
  Serial.println(stateChange);
  Serial.println(timeCounter);
  //Serial.println(offsetTime);

  timeReset = (millis() - offsetTime);

  if (timeSet) {
    if (timeReset - previousTime >= timeCounter) {
      previousTime = timeReset;
      digitalWrite(buzzerPin, HIGH);
      analogWrite(LEDPin, 0);
    }
  }
}
