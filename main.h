#ifndef MAIN_H
#define MAIN_H

#define NUM_NUMS 5
#define AND 0x01;
#define OR 0x02;

unsigned int numbers[NUM_NUMS];
char base;
char cursor;

unsigned int buttons;
unsigned int old_buttons;

char operators[13];
unsigned char operator_index;

void int2bin(char* p, int n);

void displayNumbers(void);
void displayOperator(void);
void displayCursor(void);
void displayLabels(void);

void checkForInput(void);

void swapTopOfStack(void);

void clearScreen(void);

void switchBase(void);
void switchOperator(void);

void updateScreen(void);

void moveCursorLeft(void);
void moveCursorRight(void);
void boundCursor(void);

void incrementDigit(void);
void decrementDigit(void);

unsigned int digitIsZero(void);
unsigned int digitIsMax(void);

void performOperation(void);

#endif
