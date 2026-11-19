// Include the library
#include <LedControl.h>
#include <LiquidCrystal.h>

unsigned long lmt = 0;

// Make the pins into variables
// MAX7219 pins
const int din = 12;
const int cs = 11;
const int clk = 52;

// LCD Screen pins
const int rs = 50, e = 51, d4 = 4, d5 = 5, d6 = 6, d7 = 7;

// Make the joystick pins into variables
const int joyx = A0;
const int joyy = A1;
const int sw = 13;

// Set start to false and reset timer to make sure game doesn't start immediately
bool start = false;
int time = 0;
int time2 = 0;

// Initialize the MAX7219 module
LedControl lc(din, clk, cs, 1);

// Intialize the LCD Screen
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

// Make variables that records bar position
int ledposy = 6;
int yright = 7;

// Make variables that record ball position
float ballposX = 0;
float ballposY = 0;

// Make speed variables that are always added to ballpos
float speedX = 1;
float speedY = 1;

// Reset points
int points = 0;

// Allows for a different message everytime you lose after first starting the game
bool start2 = false;

void setup() {
  // Set up the LCD screen
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  lcd.print("Wanna play ping");
  lcd.setCursor(0, 1);
  lcd.print("pong? [Start]");

  // Enable and set up MAX7219 Module
  lc.shutdown(0, false);
  lc.setIntensity(0, 3);
  lc.clearDisplay(0);
  
  // Set the starting point
  lc.setLed(0, ballposY, ballposX, true);
  lc.setLed(0, 7, ledposy, true);
  lc.setLed(0, 7, yright, true);

  // Set start mode button pin to input
  pinMode(sw, INPUT_PULLUP);
}

void loop() {
  // Read the joystick analog values to determine joystick movements
  int x = analogRead(joyx);
  int y = analogRead(joyy);
  
  // Check if start was clicked and initalize LCD screen for point recording
  if (digitalRead(sw) == LOW && start2 == false) {
    start = !start;
    start2 = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Points: ");
    lcd.print(points);
    delay(300);
  }
  // Increment joystick timer
  time2++;


  // When game is started (joystick)
  if (start == true) {

    // Move bar based on joystick movements
    if (y > 550 && time2 >= 580) { 
      time2 = 0;
      lc.setLed(0, 7, yright, false);   
      lc.setLed(0, 7, ledposy, false);
      ledposy--;
      yright--;
      lc.setLed(0, 7, ledposy, true);
      lc.setLed(0, 7, yright, true);
    } else if (y < 470 && time2 >= 580) {
      time2 = 0;
      lc.setLed(0, 7, yright, false);    
      lc.setLed(0, 7, ledposy, false);
      ledposy++;
      yright++;
      lc.setLed(0, 7, ledposy, true);
      lc.setLed(0, 7, yright, true);
    }
  }
  // Increment ball timer
  time++;

  // Prevent bar from going out of bounds
  if (ledposy == -1) {
    ledposy = 0;
    yright = 1;
    lc.setLed(0, 7, ledposy, true);
    lc.setLed(0, 7, yright, true);
  } else if (yright == 8) {
    ledposy = 6;
    yright = 7;
    lc.setLed(0, 7, ledposy, true);
    lc.setLed(0, 7, yright, true);
  }

  // When game started (ball)
  if (start == true) {
    
    // Move ball and check timer
    if (time >= 1000) {
      // Reset timer
      time = 0;

      // Move the ball to a different postion depending on speedX and Y values
      lc.setLed(0, ballposY, ballposX, false);
      ballposY = ballposY + speedY;
      ballposX = ballposX + speedX;
      lc.setLed(0, ballposY, ballposX, true);
      lc.setLed(0, 7, ledposy, true);
      lc.setLed(0, 7, yright, true);
      
      // Check whether left wall is in contact with the ball
      if (round(ballposX) == 0 && round(ballposY) < 9) {
        // Set speed to a different value for randomness
        if (speedX > 0) {
          speedX = 0.7;
        } else if (speedX < 0) {
          speedX = -0.7;
        }
        // Invert speed to make ball bounce back
        speedX = -speedX;
        delay(50);
      }

      // Check whether right wall is in contact with ball
      if (round(ballposX) == 7 && round(ballposY) < 9) {
        // Set speed to a different value for randomness
        if (speedX > 0) {
          speedX = 0.7;
        } else if (speedX < 0) {
          speedX = -0.7;
        }       

        // Invert speed to make ball bounce back
        speedX = -speedX;
        delay(50);
      }

      // Check whether top wall is in contact with ball
      if (round(ballposY) <= 0 && round(ballposX) < 8) {

        // Invert speed for bounce back
        speedY = -speedY;
        delay(50);
      }


      // Check whether the bar hit the ball or not
      if (round(ballposY) >= 7 && round(ballposX) == yright) {

        // Invert speed and add 1 point
        speedY = -speedY;
        points++;
        delay(200);
        lcd.setCursor(9, 0);
        lcd.print(points);
      } else if (round(ballposY) >= 7 && round(ballposX) == ledposy) {
        speedY = -speedY;
        points++;
        delay(200);
        lcd.setCursor(9, 0);
        lcd.print(points);

        // Check whether the bar did not hit the ball
      } else if (round(ballposY) == 10) {

        // Stop the game
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Game over");
        start = false;
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Try Again");
        lcd.setCursor(0, 1);
        lcd.print("[Yes]");
      }
    }
  }

  // Read SW press when game has already been started once
  if (start2 == true && start == false) {
    if (digitalRead(sw) == LOW) {
      start = !start;
      lc.shutdown(0, false);
      lc.setIntensity(0, 3);
      lc.clearDisplay(0);

      // Reset speed, ballpos, and points and intialize game

      points = 0;

      ballposX = 0;
      ballposY = 0;
      speedX = 1;
      speedY = 1;

      ledposy = 6;
      yright = 7;

      lc.setLed(0, ballposY, ballposX, true);
      lc.setLed(0, 7, ledposy, true);
      lc.setLed(0, 7, yright, true);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Points: ");
      lcd.print(points);
      delay(300);
    }
  }

}





















































































































// Code by Hamdan Naseer (4RyuXD on Github)