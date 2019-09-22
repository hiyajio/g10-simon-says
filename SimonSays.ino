#include <Wire.h>
#include <LiquidCrystal.h>
#include "pitches.h"

//Declaration
LiquidCrystal lcd(34, 32, 30, 28, 26, 24);

int buzzer = 7;
int yesButton = 52;
int noButton = 50;
int randNumber;

int tries = 0;
int seq = 3;
int lvl = 1;
int sublvl = 1;

int tStartReady;
int tAfterReady;


int spd = 250;
int lights[3] = {49, 51, 53};
int buttons[3] = {39, 41, 43};
int correctAnswers[20];
int guesses[20];

boolean alive = true;
boolean gameInProgress = false;


void setup() {
    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("- Color Coding -");
    lcd.setCursor(3, 1);
    lcd.print("TERO 2018");
    delay(5000);
    
  for(int i = 0; i <= 2; i++){
    pinMode(lights[i], OUTPUT);
    pinMode(buttons[i], INPUT);
  }  
  
  pinMode(buzzer, OUTPUT);
  pinMode(yesButton, INPUT);
  pinMode(noButton, INPUT);
  lcd.clear();
}


void disMainMenu(){
    lcd.setCursor(0,0);
    lcd.print("- Color Coding -");
    lcd.setCursor(2,1);
    lcd.print("-> NEW GAME?");
    
    if (digitalRead(yesButton)) {
      gameInProgress = true;
    }
}


void buttonSound(int command) {
  switch (command) {
    case 0: tone(buzzer, NOTE_C5, 100);
    break;
    case 1: tone(buzzer, NOTE_D5, 100);
    break;
    case 2: tone(buzzer, NOTE_G5, 100);
    break;
  }
}


void makeSound(String command) {
  if (command == "ready") {
    tone(buzzer, NOTE_C6, 700);
    delay(400);
    noTone(buzzer);
  } else if (command == "gameOver") {
    tone(buzzer, NOTE_A3, 200);
    delay(200);
    tone(buzzer, NOTE_E3, 400);
    delay(100);
  }else if (command == "levelUp") {
    tone(buzzer, NOTE_C5, 150);
    delay(50);
    tone(buzzer, NOTE_E5, 150);
    delay(50);
    tone(buzzer, NOTE_G5, 450);
    delay(100);
    tone(buzzer, NOTE_D5, 150);
    delay(50);
    tone(buzzer, NOTE_G5, 150);
    delay(50);
    tone(buzzer, NOTE_C6, 450);
    delay(100);
    tone(buzzer, NOTE_G5, 150);
    delay(50);
    tone(buzzer, NOTE_C6, 150);
    delay(50);
    tone(buzzer, NOTE_G6, 450);
    delay(100);
    noTone(buzzer);
  } else if (command == "sublevelUp") {
    tone(buzzer, NOTE_C5, 150);
    delay(50);
    tone(buzzer, NOTE_E5, 150);
    delay(50);
    tone(buzzer, NOTE_G5, 450);
    delay(50);
    noTone(buzzer);
  }
}


void lightUp(){
      for(int i = 0; i <= 2; i++){
        digitalWrite(lights[i], HIGH);
      }
      delay(500);
      for(int i = 0; i <= 2; i++){
        digitalWrite(lights[i], LOW);
      }
}


void initialize() {
  char buffer[7];
  char levelDec[15] = "- Level: ";
  strcat(levelDec, itoa(lvl, buffer, 10));
  strcat(levelDec, ".");
  strcat(levelDec, itoa(sublvl, buffer, 10));
  strcat(levelDec, " -");
  
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(levelDec);
  lcd.setCursor(1,1);
  lcd.print("Get ready...");
  
  tStartReady = millis()/1000;
  tAfterReady = millis()/1000;
  
  while ((tAfterReady - tStartReady) < 3){
    tAfterReady = millis()/1000;
    lcd.setCursor(14,1);
    lcd.print(3 - (tAfterReady - tStartReady));
  }
  
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print("Don't blink!");
  
  for(int i = 0; i < seq; i++) {
    randomSeed(analogRead(0));
    randNumber = random(3);
    
    switch(sublvl) {
      case 1 : spd = 380;
      break;
      case 2 : spd = 250;
      break;
      case 3 : spd = 160;
      break;
    }
    
    buttonSound(randNumber);
    digitalWrite(lights[randNumber], HIGH);
    delay(spd);
    noTone(buzzer);
    digitalWrite(lights[randNumber], LOW);
    delay(spd);
      
    correctAnswers[i] = randNumber;
  }
  
  delay(100);
  
  for(int i = 0; i <= 2; i++){
      digitalWrite(lights[i], HIGH);
  }
  tone(buzzer, NOTE_C6, 700);
  delay(500);
  for(int i = 0; i <= 2; i++){
      digitalWrite(lights[i], LOW);
  }
  noTone(buzzer);
  
  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.print("GO!");
}


void guess(int pressed) {
  digitalWrite(lights[pressed], HIGH);
  buttonSound(pressed);
  delay(200);
  digitalWrite(lights[pressed], LOW);
  noTone(buzzer);
  guesses[tries] = pressed;
  delay(50);
  
  if (guesses[tries] == correctAnswers[tries]) {
    tries++;
  } else {
    alive = false;
  }
}


void reset(){
  alive = true;
  tries = 0;
}


void loop() {
  // put your main code here, to run repeatedly: 
  disMainMenu();
  if (gameInProgress) {
    
    initialize();
    while ((tries < seq) && (alive) && (gameInProgress)) {
      
      if (digitalRead(buttons[0])) {
         guess(0);
      } else if (digitalRead(buttons[1])) {
         guess(1);
      } else if (digitalRead(buttons[2])) {
         guess(2);
      }
       else if (digitalRead(noButton)) {
         gameInProgress = false;
      } 
    }
    
    if ((alive) && (gameInProgress)) {
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("GOOD JOB!");
      lcd.setCursor(5,1);
      lcd.print("LEVEL UP!");
      
      sublvl++;
      
      if (((sublvl - 1) % 3 == 0) && (sublvl != 1)) {
        seq++;
        lvl++;
        sublvl = 1;  
       
       makeSound("levelUp"); 
      } else {
        makeSound("sublevelUp");
      }
      
      for(int i = 0; i <= 2; i++){
        lightUp();   
        delay(200);
      }
    } else if ((!alive) && (gameInProgress)){
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("GAME OVER");
        makeSound("gameOver");
        delay(1200);
        lcd.setCursor(2,1);
        lcd.print("Try again...");
        makeSound("gameOver");
        delay(1000);
        lvl = 1;
        sublvl = 1;
        seq = 3;
    }
    reset();
    delay(1000);
  }
}
