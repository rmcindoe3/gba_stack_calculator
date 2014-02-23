#ifndef MAIN_H
#define MAIN_H

/************************ MACROS **************************/

#define NUM_NUMS 5
#define AND 0x01
#define OR 0x02

#define OPERATOR 0x03
#define INPUT 0x04
#define INSTRUCTIONS 0x05

/********************** VARIABLES *************************/

//These are the numbers of our stack.
unsigned int numbers[NUM_NUMS];

//This is the currently selected base.
char base;

//This is the cursor's position.
char cursor;

//This is the input pad cursor's position.
char input_cursor;

//This is the state of the game.
char state;

//These keep track of which buttons are pressed.
unsigned int buttons;
unsigned int old_buttons;

//This is an array of possible operators.
char operators[13];

//This keeps track of which operator is currently selected.
unsigned char operator_index;

/********************** FUNCTIONS *************************/
/**** Full descriptions are located in the main.c file ****/

void displayNumbers(void);
void displayOperator(void);
void displayCursor(void);
void displayLabels(void);
void displayInputPad(void);
void drawHighlight(void);
void displayInstructionsScreen(void);

void setDigit(void);

void int2bin(char* p, int n);

void moveInputCursorUp(void);
void moveInputCursorDown(void);
void moveInputCursorLeft(void);
void moveInputCursorRight(void);

void checkOperatorButtons(void);
void checkInputButtons(void);
void checkInstructionsButtons(void);

void swapTopOfStack(void);

void clearScreen(void);

void switchBase(void);
void switchOperator(void);

void updateScreen(void);

void moveCursorLeft(void);
void moveCursorRight(void);
void boundCursor(void);

void performOperation(void);

#endif
