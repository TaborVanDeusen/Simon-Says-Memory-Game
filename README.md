# Simon-Says-Memory-Game

Game Description: Simon Says Memory Game

Objective:
The goal of the Simon Says Memory Game is to test and improve the player's memory skills by challenging them to recall and repeat an ever-growing sequence of colors and sounds. The game progressively becomes more difficult as the sequence lengthens, and the player must keep up without making mistakes to achieve the highest score possible. The player wins the game if they complete level 10.

Rules:

Game Start:
○ The game starts when the slide switch on the Circuit Playground Express (CPX)
is turned to the "on" position.
○ The initial state is "START", and the message "Game Started!" is printed to the
serial monitor.

Sequence Display:
○ The CPX will generate and display a sequence of colors using its LEDs,
accompanied by corresponding sounds for passed or failed levels.
○ Each level begins by displaying the sequence from the start, up to the current
level.
○ Colors and sounds are displayed at a pace of one color per second.

Player Input:
○ The player must repeat the displayed sequence by pressing the left and right
buttons.
○ Left Button (Pin 4): Registers the player's input for the color blue.
○ Right Button (Pin 5): Registers the player's input for the color red.
○ The game waits for the player to input the entire sequence for the current level
before proceeding.

Checking Input:
○ After the player has entered the complete sequence, the game checks if the input
matches the generated sequence.
○ If the input is correct, the game advances to the next level, and the sequence
length increases by one. The Arduino will play a happy sound to alert the reader they have passed the level.
○ If the input is incorrect, the game transitions to the "GAME_OVER" state, and the player's final score and level reached are displayed. The Arduino will play a sad sound to alert the reader they have failed the level.

Game End:
○ The game ends when the player makes a mistake during the replication of the
sequence or when the player successfully completes all levels (up to level 10).
○ When the slide switch is turned to the "off" position, the game immediately stops,
prints "Game Ended" once to the serial monitor, and resets.

Scoring System:
● The player's score is determined by the number of levels successfully completed.
● Each correct sequence entered advances the player to the next level.
● The maximum score is achieved by completing all 10 levels.

Levels:
● The game starts at level 1 with a sequence of 1 color.
● Each subsequent level adds one more color to the sequence, making it more challenging
to remember and repeat the sequence.
● The maximum level is 10.
● If level 10 is completed, the game will start over at level 1.
● Any time the game is turned off and back on it will immediately start at the first level,
regardless of what level the game was ended at.

User Inputs/Outputs Definition Sheet

Inputs:

Slide Switch:
○ Function: Controls the start and stop of the game.
○ Pin: 7
○ Behavior:
■ On Position (HIGH): Starts the game from level 1. If the game is in the START state, it initializes the game and transitions to the SHOW_PATTERN state.
■ Off Position (LOW): Stops the game. "Game Ended" is printed once to the serial monitor, and the game is reset to the START state.

Left Button:
○ Function: Registers the player's input for the color blue.
○ Pin: 4
○ Behavior:
■ Pressed: Records the player's input as blue (value 2). This input is used during the USER_INPUT state to compare against the generated sequence. Debounced to prevent multiple registrations of a single press.

Right Button:
○ Function: Registers the player's input for the color red.
○ Pin: 5
○ Behavior:
■ Pressed: Records the player's input as red (value 1). This input is used during the USER_INPUT state to compare against the generated sequence. Debounced to prevent multiple registrations of a single press.

Outputs:

LEDs:
○ Function: Displays the color sequence to be memorized.
○ LED Positions: 0 to 9 on the Circuit Playground Express.
○ Colors:
■ Red (CP_RED): Represents one part of the sequence (value 1).
■ Blue (CP_BLUE): Represents another part of the sequence (value 2).
○ Behavior:
■ During the SHOW_PATTERN state, the LEDs light up sequentially in the generated pattern. Each LED lights up for 500 ms with a 500 ms interval between each color.

Speakers:
○ Function: Plays corresponding tones for when the player passes a level and when
the game ends.
○ Tones:
■ Level Passed: 1000 Hz (100 ms) 1200 Hz (100 ms) 1400 Hz (100 ms)
■ Game Ended: 300 Hz (1000 ms)
○ Behavior:
■ When a level is passed, play the happy tone to alert the player that they passed the level.
■ When a level is failed and the game ends, play the sad tone to alert the player that the game has ended.

Serial Monitor:
○ Function: Provides game status updates and messages.
○ Messages:
■ "Game Started!": Printed when the game is initialized and the slide switch is turned on.
■ "Game Ended": Printed once when the slide switch is turned off, indicating the game has stopped.
■ "Congratulations! You won the game!": Printed when the player successfully completes all 10 levels.
■ "Game Over! Your score is: X, reached level: Y": Printed when the player makes a mistake, displaying their final score and level reached.
○ Behavior:
■ Provides feedback and status updates to the player throughout the game.
