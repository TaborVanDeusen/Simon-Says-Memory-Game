#include <Adafruit_CircuitPlayground.h> // Include the library for Circuit Playground
#include <AsyncDelay.h> // Include the library for handling delays asynchronously

AsyncDelay timer; // Declare an AsyncDelay object to manage non-blocking delays

// Define color codes for red and blue
#define CP_RED     0xFF0000
#define CP_BLUE    0x0000FF

// Define pin numbers for buttons and the slide switch
#define LEFT_BUTTON_PIN 4
#define RIGHT_BUTTON_PIN 5
#define SLIDE_SWITCH_PIN 7

// Declare volatile boolean flags to track button and switch states
volatile bool leftButtonPressed = false;
volatile bool rightButtonPressed = false;
volatile bool slideSwitchState = false;
bool gameEnded = false; // Flag to keep track of whether the game has ended or not

// Define the possible game states using an enum
enum GameState { START, SHOW_PATTERN, USER_INPUT, CHECK_INPUT, NEXT_LEVEL, GAME_OVER };
GameState gameState = START; // Initialize the game state to START

// Declare arrays to store the pattern and user input
int pattern[10];
int userPattern[10];
// Initialize variables for game progress
int level = 1;
int score = 0;
int currentStep = 0;
// Set a debounce delay for button presses
const unsigned long debounceDelay = 100; // Increase debounce delay to 100 ms
// Declare variables to track the last button press times
volatile unsigned long lastLeftButtonPressTime = 0;
volatile unsigned long lastRightButtonPressTime = 0;
volatile unsigned long lastSlideSwitchChangeTime = 0;

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate
  CircuitPlayground.begin(); // Initialize Circuit Playground

  // Attach interrupts for button presses
  attachInterrupt(digitalPinToInterrupt(LEFT_BUTTON_PIN), handleLeftButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_BUTTON_PIN), handleRightButton, FALLING);
}

void loop() {
  unsigned long currentMillis = millis(); // Get the current time in milliseconds

  // Read slide switch state with debounce
  if (currentMillis - lastSlideSwitchChangeTime >= debounceDelay) {
    bool newSlideSwitchState = CircuitPlayground.slideSwitch(); // Read the current state of the slide switch
    if (newSlideSwitchState != slideSwitchState) { // Check if the switch state has changed
      slideSwitchState = newSlideSwitchState; // Update the switch state
      if (!slideSwitchState && !gameEnded) { // If the switch is off and the game has not ended
        Serial.println("Game Ended");
        gameEnded = true; // Set the flag to true to indicate that the game has ended
      }
      lastSlideSwitchChangeTime = currentMillis; // Update the last switch change time
    }
  }

  if (!slideSwitchState) { // If the slide switch is off
    stopGame(); // Stop the game
    return; // Exit the loop
  }

  // Handle the game state machine
  switch (gameState) {
    case START:
      startGame(); // Start the game
      break;

    case SHOW_PATTERN:
      showPattern(); // Show the pattern to the user
      break;

    case USER_INPUT:
      // Check left button press with debounce
      if (leftButtonPressed && currentMillis - lastLeftButtonPressTime >= debounceDelay) {
        userPattern[currentStep] = 2; // Register a left button press (blue) in the user pattern
        currentStep++; // Move to the next step
        leftButtonPressed = false; // Reset the left button pressed flag
        lastLeftButtonPressTime = currentMillis; // Update the last left button press time
        // Provide visual feedback for button press
        CircuitPlayground.setPixelColor(currentStep - 1, CP_BLUE); // Light up the pixel blue
        delay(200); // Visual feedback delay
        CircuitPlayground.clearPixels(); // Clear the pixels
        if (currentStep >= level) { // If user has completed their input for this level
          gameState = CHECK_INPUT; // Move to the next state to check the input
        }
      }
      
      // Check right button press with debounce
      if (rightButtonPressed && currentMillis - lastRightButtonPressTime >= debounceDelay) {
        userPattern[currentStep] = 1; // Register a right button press (red) in the user pattern
        currentStep++; // Move to the next step
        rightButtonPressed = false; // Reset the right button pressed flag
        lastRightButtonPressTime = currentMillis; // Update the last right button press time
        // Provide visual feedback for button press
        CircuitPlayground.setPixelColor(currentStep - 1, CP_RED); // Light up the pixel red
        delay(200); // Visual feedback delay
        CircuitPlayground.clearPixels(); // Clear the pixels
        if (currentStep >= level) { // If user has completed their input for this level
          gameState = CHECK_INPUT; // Move to the next state to check the input
        }
      }
      break;

    case CHECK_INPUT:
      if (currentStep >= level) { // Check input against pattern only when user has entered their entire sequence
        if (checkInput()) { // If the user's input matches the pattern
          score += level; // Increase the score by the current level
          CircuitPlayground.playTone(1000, 100); // Play a tone (E note)
          delay(50); // Short pause
          CircuitPlayground.playTone(1200, 100); // Play a tone (G note)
          delay(50); // Short pause
          CircuitPlayground.playTone(1400, 100); // Play a tone (B note)
          delay(1000); // 1 second break
          if (level == 10) { // If the user has completed all levels
            gameState = GAME_OVER; // Move to the game over state
            Serial.println("Congratulations! You Beat The Game!");
          } else {
            gameState = NEXT_LEVEL; // Move to the next level
          }
        } else { // If the user's input does not match the pattern
          gameState = GAME_OVER; // Move to the game over state
        }
      }
      break;

    case NEXT_LEVEL:
      level++; // Increase the level
      startNextLevel(); // Start the next level
      break;

    case GAME_OVER:
      Serial.print("Game Over! Your Score Is: ");
      Serial.print(score); // Print the final score
      Serial.print(", Reached Level: ");
      Serial.println(level); // Print the level reached
      CircuitPlayground.playTone(300, 1000); // Play a sad tone
      gameState = START; // Reset the game state to START
      break;
  }
}

void handleLeftButton() {
  unsigned long currentMillis = millis(); // Get the current time in milliseconds
  if (currentMillis - lastLeftButtonPressTime > debounceDelay) { // Check if the debounce delay has passed
    leftButtonPressed = true; // Set the left button pressed flag
    lastLeftButtonPressTime = currentMillis; // Update the last left button press time
  }
}

void handleRightButton() {
  unsigned long currentMillis = millis(); // Get the current time in milliseconds
  if (currentMillis - lastRightButtonPressTime > debounceDelay) { // Check if the debounce delay has passed
    rightButtonPressed = true; // Set the right button pressed flag
    lastRightButtonPressTime = currentMillis; // Update the last right button press time
  }
}

void startGame() {
  Serial.println("Game Started!"); // Print a message to indicate the game has started
  score = 0; // Reset the score
  level = 1; // Reset the level to 1
  generatePattern(); // Generate a new pattern for the game
  gameState = SHOW_PATTERN; // Set the game state to show the pattern
}

void showPattern() {
  CircuitPlayground.clearPixels(); // Clear any lit pixels
  for (int i = 0; i < level; i++) { // Loop through the pattern up to the current level
    if (pattern[i] == 1) { // If the pattern element is 1
      CircuitPlayground.setPixelColor(i, CP_RED); // Light up the pixel red
    } else { // If the pattern element is 2
      CircuitPlayground.setPixelColor(i, CP_BLUE); // Light up the pixel blue
    }
    delay(500); // Delay for half a second
    CircuitPlayground.clearPixels(); // Clear the pixels
    delay(500); // Delay for another half second
  }
  currentStep = 0; // Reset the current step to 0
  gameState = USER_INPUT; // Set the game state to user input
}

void generatePattern() {
  for (int i = 0; i < 10; i++) { // Loop to generate a pattern of 10 elements
    pattern[i] = random(1, 3); // Generate a random pattern of 1s and 2s
  }
}

bool checkInput() {
  for (int i = 0; i < level; i++) { // Loop through the user input and pattern up to the current level
    if (pattern[i] != userPattern[i]) { // If any element does not match
      return false; // Return false indicating the input does not match
    }
  }
  return true; // Return true indicating the input matches
}

void startNextLevel() {
  generatePattern(); // Generate a new pattern for the next level
  gameState = SHOW_PATTERN; // Set the game state to show the pattern
}

void stopGame() {
  // Reset game state and variables when the game is stopped
  gameState = START; // Reset the game state to START
  currentStep = 0; // Reset the current step
  score = 0; // Reset the score
  CircuitPlayground.clearPixels(); // Clear any lit pixels
  // Reset button states
  leftButtonPressed = false; // Reset the left button pressed flag
  rightButtonPressed = false; // Reset the right button pressed flag
  gameEnded = false; // Reset the gameEnded flag
}