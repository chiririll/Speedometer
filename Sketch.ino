#include <PCD8544.h>

/* Pins configuration*/
  // Analog
  #define R_SENSOR 0
  #define PULSE 1
  #define BAT 2

  // Digital
  #define VIBRO 11
  #define SOUND 10
  #define PULSE_VCC 2
  #define BL 9

  //Buttons
  #define BTN_DOWN 8
  #define BTN_OK 13
  #define BTN_RIGHT 12
/* ----------------- */

/* Units */
#define SPEED_UNIT " km/h"
#define DISTANCE_UNIT " km"
#define PULSE_UNIT " @"
#define TEMP_UNIT "*C"
#define PRESSURE_UNIT " mm"
/* ----- */

static PCD8544 lcd;

/* GUI */
//const String screens[] = {"Скорость", "Расстояние", "Пульс", "Погода", "Дата", "Настройки"};
const String screens[] = {"Speed", "Distance", "Pulse", "Weather", "Date", "Settings"};
const unsigned char screensCnt = 6;

const String settings[] = {"Diameter", "Max speed", "Display", "Date & time", "Auto-save", "Calibration"};
const unsigned char settingsCnt = 6;

unsigned char currentScreen = 0;
unsigned char currentLine = 0;
bool subScreen = false;

// SubScreens:

/* --- */

/* Values */
unsigned char currentSpeed = 0;
unsigned char maxSpeed = 0;

unsigned short currentDistance = 0;
/* ------ */

void printHeader (String header) {
  lcd.setCursor(0, 0);
  
  lcd.drawLine();
  lcd.setInverseOutput(true);
  lcd.print(" ");
  lcd.print(header);
  
  lcd.setCursor(55, 0);
  lcd.print("00:00");
  
  lcd.setInverseOutput(false);

  lcd.setCursor(0, 1);
  lcd.clearLine();
}

void drawScreen () {
  lcd.clear();
  printHeader(screens[currentScreen]);
  updScreen();
}

void updScreen() {
  switch (currentScreen) {
    case 0:
      speedScreen();
      break;
    case 1:
      distanceScreen();
      break;
    case 2:
      pulseScreen();
      break;
    case 3:
      weatherScreen();
      break;
    case 4:
      dateScreen();
      break;
    case 5:
      settingsScreen();
      break;
  }
}

void speedScreen() {
  lcd.setCursor(0, 2);
  lcd.print("Now: ");
  lcd.print(currentSpeed);
  lcd.print(SPEED_UNIT);
  lcd.setCursor(0, 4);
  lcd.print("Max: ");
  lcd.print(maxSpeed);
  lcd.print(SPEED_UNIT);
}

void distanceScreen() {
  lcd.setCursor(0, 2);
  lcd.print("Now: ");
  lcd.print(currentDistance);
  lcd.print(DISTANCE_UNIT);
  lcd.setCursor(0, 4);
  lcd.print("Total: ");
  lcd.print("1000");
  lcd.print(DISTANCE_UNIT);
}

void pulseScreen () {
  digitalWrite(PULSE_VCC, HIGH);
  lcd.setCursor(10, 3);
  lcd.print(getPulse());
  lcd.print(PULSE_UNIT);
  digitalWrite(PULSE_VCC, LOW);
}

void weatherScreen () {
  
}

void dateScreen () {
  
}

void settingsScreen () {
  for (int i = 1; i < 6; i++) {
    lcd.setCursor(0, i);
    if (i-1 == currentLine)
      lcd.setInverseOutput(true);
    else
      lcd.setInverseOutput(false);
    lcd.clearLine();
    lcd.print(settings[i-1]);
  }
  /*if (currentLine == 0 || currentLine == settingsCnt-1) {
    lcd.setCursor(0, currentLine+2);
    lcd.setInverse(true);
    lcd.clearLine();
    lcd.print(settings[currentLine]);
  }
  lcd.setInverse(false);*/
}

void runSubScreen() {
  subScreen = true;
  lcd.clear();
  printHeader(settings[currentScreen]);
}

unsigned char getPulse () {
  delay (1000);
  return 0;
}

void nextScreen () {
  currentScreen++;
  if (currentScreen >= screensCnt)
    currentScreen = 0;
  drawScreen();
  currentLine = 0;
}

void nextLine () {
  switch (currentScreen) {
    case 5:
      currentLine++;
      if (currentLine >= settingsCnt)
        currentLine = 0;
  }
  updScreen();
}

void action() {
  switch (currentScreen) {
    case 2:
      pulseScreen();
      break;
    case 5:
      if (!subScreen)
        runSubScreen();
      else
        drawScreen();
      break;
  }
}

#define BTN_TIMOUT_DURATION 100
int btnTimeout = 0;
void checkBtns () {
  if (millis() < btnTimeout)
    return;
    
  if (digitalRead(BTN_RIGHT) == LOW) {
    nextScreen();
    btnTimeout = millis() + BTN_TIMOUT_DURATION;
  } else if (digitalRead(BTN_DOWN) == LOW) {
    nextLine();
    btnTimeout = millis() + BTN_TIMOUT_DURATION;
  } else if (digitalRead(BTN_OK) == LOW) {
    action();
    btnTimeout = millis() + BTN_TIMOUT_DURATION;
  }
}

int lastR = 0;
void speedometer () {
  
}

void setup() {
  lcd.begin (84, 48);
  Serial.begin(9600);
  
  pinMode (PULSE_VCC, OUTPUT);
  pinMode (BL, OUTPUT);
  pinMode (SOUND, OUTPUT);
  pinMode (VIBRO, OUTPUT);
  
  pinMode (BTN_DOWN, INPUT_PULLUP);
  pinMode (BTN_OK, INPUT_PULLUP);
  pinMode (BTN_RIGHT, INPUT_PULLUP);
  
  digitalWrite(PULSE_VCC, LOW);
  digitalWrite(BL, HIGH);
  digitalWrite(SOUND, LOW);
  
  drawScreen();
}

void loop() {
  checkBtns();
}
