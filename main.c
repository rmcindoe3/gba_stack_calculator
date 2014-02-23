#include <stdio.h>
#include <math.h>
#include <string.h>
#include "myLib.h"
#include "text.h"
#include "main.h"

int main()
{
    //Sets display mode to Mode 3 with BG2 enabled.
    REG_DISPCTL = BG2_ENABLE | MODE3;

    //Initialize base to base 10.
    base = 10;

    //Start in OPERATOR state.
    state = OPERATOR;

    //Start the digit cursor at 0.
    cursor = 0;

    //Initialize our numbers to zero in case they are not already at zero.
    numbers[0] = 0;
    numbers[1] = 0;
    numbers[2] = 0;
    numbers[3] = 0;
    numbers[4] = 0;

    //Sets up our array of operators.
    operators[0] = '+';
    operators[1] = '-';
    operators[2] = '*';
    operators[3] = '/';
    operators[4] = AND;
    operators[5] = OR;
    operators[6] = '!';
    operators[7] = '&';
    operators[8] = '|';
    operators[9] = '~';
    operators[10] = '^';
    operators[11] = '<';
    operators[12] = '>';

    //Initialize the current operator.
    operator_index = 0;

    //Displays everything to the screen.
    updateScreen();

    //Our main while loop.
    while(1) {

        //Update our button variables.
        old_buttons = buttons;
        buttons = BUTTONS;

        //If we're in the OPERATOR state, then check for operator input.
        if(state == OPERATOR) {
            checkOperatorButtons();
        }
        //If we're in the INPUT state, then check for input pad input.
        else if(state == INPUT) {
            checkInputButtons();
        }
        //If we're in the INSTRUCTIONS state, then check for instructions input.
        else if(state == INSTRUCTIONS) {
            checkInstructionsButtons();
        }
    }
}

/** updateScreen **********************************************
 * Clears and updates the screen with all the latest settings.
 *************************************************************/
void updateScreen() {

    //Clears the screen, then displays all the different screen components.
    waitForVblank();
    clearScreen();
    displayLabels();
    displayNumbers();
    displayOperator();
    displayCursor();
}

/** checkOperatorButtons **************************************
 * Checks for user input while in the OPERATOR state.
 *************************************************************/
void checkOperatorButtons() {

    //If the R button is pressed, switch bases and then update the screen.
    if(BUTTON_PRESSED(BUTTON_R)) {
        switchBase();
        updateScreen();
    }

    //If the L button is pressed, swap the top two numbers on the stack
    //  and then update the screen.
    if(BUTTON_PRESSED(BUTTON_L)) {
        swapTopOfStack();
        updateScreen();
    }

    //If the start button is pressed, change game state to INPUT,
    //  reinitialize the input_cursor to zero, and then display the input pad.
    if(BUTTON_PRESSED(BUTTON_START)) {
        state = INPUT;
        input_cursor = 0;
        displayInputPad();
    }

    //If the select button is pressed, then switch to INSTRUCTIONS state.
    if(BUTTON_PRESSED(BUTTON_SELECT)) {
        state = INSTRUCTIONS;
        displayInstructionsScreen();
    }

    //If the A button is pressed, switch the current operator
    //  and then update the screen.
    if(BUTTON_PRESSED(BUTTON_A)) {
        switchOperator();
        updateScreen();
    }

    //If the B button is pressed, perform the currently selected operation
    //  and then update the screen.
    if(BUTTON_PRESSED(BUTTON_B)) {
        performOperation();
        updateScreen();
    }

    //If the left button is pressed, move the cursor left
    //  and then update the screen.
    if(BUTTON_PRESSED(BUTTON_LEFT)) {
        moveCursorLeft();
        updateScreen();
    }

    //If the right button is pressed, move the cursor right 
    //  and then update the screen.
    if(BUTTON_PRESSED(BUTTON_RIGHT)) {
        moveCursorRight();
        updateScreen();
    }

    //If the up button is pressed, move the stack upwards
    //  and then update the screen.
    if(BUTTON_PRESSED(BUTTON_UP)) {
        numbers[4] = numbers[3];
        numbers[3] = numbers[2];
        numbers[2] = numbers[1];
        numbers[1] = numbers[0];
        updateScreen();
    }

    //If the down button is pressed, move the stack downwards
    //  and then update the screen.
    if(BUTTON_PRESSED(BUTTON_DOWN)) {
        numbers[0] = numbers[1];
        numbers[1] = numbers[2];
        numbers[2] = numbers[3];
        numbers[3] = numbers[4];
        updateScreen();
    }
}

/** checkInputButtons *********************************
 * Checks for user input while in the INPUT state.
 *****************************************************/
void checkInputButtons() {

    //If the up button is pressed, move the input pad cursor up
    //  and then redraw the input pad.
    if(BUTTON_PRESSED(BUTTON_UP)) {
        moveInputCursorUp();
        waitForVblank();
        displayInputPad();
    }

    //If the down button is pressed, move the input pad cursor down
    //  and then redraw the input pad.
    if(BUTTON_PRESSED(BUTTON_DOWN)) {
        moveInputCursorDown();
        waitForVblank();
        displayInputPad();
    }

    //If the left button is pressed, move the input pad cursor left
    //  and then redraw the input pad.
    if(BUTTON_PRESSED(BUTTON_LEFT)) {
        moveInputCursorLeft();
        waitForVblank();
        displayInputPad();
    }

    //If the right button is pressed, move the input pad cursor right
    //  and then redraw the input pad.
    if(BUTTON_PRESSED(BUTTON_RIGHT)) {
        moveInputCursorRight();
        waitForVblank();
        displayInputPad();
    }

    //If the start button is pressed, then change the state to OPERATOR
    //  set the selected digit, and then update the screen.
    if(BUTTON_PRESSED(BUTTON_START)) {
        state = OPERATOR;
        setDigit();
        updateScreen();
    }
}

/** checkInstructionsButtons *********************************
 * Checks for user input while in the INSTRUCTIONS state.
 *****************************************************/
void checkInstructionsButtons() {

    //If the start or select buttons are pressed, then switch back
    //  to the OPERATOR state.
    if(BUTTON_PRESSED(BUTTON_START) || BUTTON_PRESSED(BUTTON_SELECT)) {
        state = OPERATOR;
        updateScreen();
    }
}

/** setDigit ******************************************
 * Sets the currently selected digit to the number selected
 * via the input pad.
 *****************************************************/
void setDigit() {

    unsigned int temp = numbers[0];
    int i = 0;

    //This for loop grabs the current value of the selected digit.
    for(i = 0; i < cursor; i++) {
        temp /= base;
    }

    //This subtracts the selected digits value from the number.
    //  Essentially setting the digit to zero.
    numbers[0] -= (temp%base)*pow(base,cursor);

    //Once the digit is zero, we can then add the number selected
    //  from the input pad to our number to set it.
    numbers[0] += (input_cursor)*pow(base,cursor);
}

/** moveInputCursorUp *********************************
 * Moves the input pad cursor up one spot, if possible.
 *****************************************************/
void moveInputCursorUp() {

    //The following if statements determine if the input cursor
    //  can be moved upwards or not.  If it can be moved upwards,
    //  then it will change the cursor appropriately.

    if(base == 2) {
        if(input_cursor != 1) {
            input_cursor = 1;
        }
    }
    else if(base == 8) {
        if(input_cursor != 7 && input_cursor != 5 && input_cursor != 6) {
            if(input_cursor == 0) {
                input_cursor = 1;
            }
            else {
                input_cursor += 3;
            }
        }
    }
    else if(base == 10) {
        if(input_cursor != 9 && input_cursor != 8 && input_cursor != 7) {
            if(input_cursor == 0) {
                input_cursor = 1;
            }
            else {
                input_cursor += 3;
            }
        }
    }
    else if(base == 16) {
        if(input_cursor != 15 && input_cursor != 14 && input_cursor != 13) {
            if(input_cursor == 0) {
                input_cursor = 1;
            }
            else {
                input_cursor += 3;
            }
        }
    }
}

/** moveInputCursorDown *********************************
 * Moves the input pad cursor down one spot, if possible.
 *****************************************************/
void moveInputCursorDown() {

    //The following if statements determine if the input cursor
    //  can be moved downwards or not.  If it can be moved downwards,
    //  then it will change the cursor appropriately.

    if(base == 2) {
        if(input_cursor != 0) {
            input_cursor = 0;
        }
    }
    else {
        if(input_cursor != 0 && input_cursor != 2 && input_cursor != 3) {
            if(input_cursor == 1) {
                input_cursor = 0;
            }
            else {
                input_cursor -= 3;
            }
        }
    }
}

/** moveInputCursorLeft *********************************
 * Moves the input pad cursor to the left one spot, if possible.
 *****************************************************/
void moveInputCursorLeft() {

    //The following if statements determine if the input cursor
    //  can be moved to the left or not.  If it can be moved,
    //  then it will change the cursor appropriately.

    if(base != 2 && input_cursor != 0) {
        if(base != 8 || input_cursor != 7) {
            if(input_cursor != 3 && input_cursor != 6 && input_cursor != 9 && input_cursor != 12 && input_cursor != 15) {
                input_cursor++;
            }
        }
    }
}

/** moveInputCursorRight *********************************
 * Moves the input pad cursor to the right one spot, if possible.
 *****************************************************/
void moveInputCursorRight() {

    //The following if statements determine if the input cursor
    //  can be moved to the right or not.  If it can be moved,
    //  then it will change the cursor appropriately.

    if(base != 2 && input_cursor != 0) {
        if(input_cursor != 0 && input_cursor != 1 && input_cursor != 4 && input_cursor != 7 && input_cursor != 10 && input_cursor != 13) {
            input_cursor--;
        }
    }

}

/** performOperation *********************************
 * Peforms the currently selected operation on the stack.
 *****************************************************/
void performOperation() {

    //The following if statements determine which operation
    //  is selected and then perform that operation on the numbers.

    if(operator_index == 0) {
        numbers[0] = numbers[0] + numbers[1];
    }
    else if(operator_index == 1) {
        numbers[0] = numbers[0] - numbers[1];
    }
    else if(operator_index == 2) {
        numbers[0] = numbers[0] * numbers[1];
    }
    else if(operator_index == 3) {
        numbers[0] = numbers[0] / numbers[1];
    }
    else if(operator_index == 4) {
        numbers[0] = numbers[0] && numbers[1];
    }
    else if(operator_index == 5) {
        numbers[0] = numbers[0] || numbers[1];
    }
    else if(operator_index == 6) {
        numbers[0] = !numbers[0];
    }
    else if(operator_index == 7) {
        numbers[0] = numbers[0] & numbers[1];
    }
    else if(operator_index == 8) {
        numbers[0] = numbers[0] | numbers[1];
    }
    else if(operator_index == 9) {
        numbers[0] = ~numbers[0];
    }
    else if(operator_index == 10) {
        numbers[0] = numbers[0] ^ numbers[1];
    }
    else if(operator_index == 11) {
        numbers[0] = numbers[0] << 1;
    }
    else if(operator_index == 12) {
        numbers[0] = numbers[0] >> 1;
    }
}

/** switchOperator ***********************************
 * Switches the currently selected operator.
 *****************************************************/
void switchOperator() {
    operator_index++;
    operator_index %= 13;
}

/** switchBase ****************************************
 * Switches the currently selected base.
 *****************************************************/
void switchBase() {

    if(base == 10) {
        base = 16;
    }
    else if(base == 16) {
        base = 2;
    }
    else if(base == 2) {
        base = 8;
    }
    else if(base == 8) {
        base = 10;
    }

    //We bound the cursor here to make sure that if you switch
    //  from a lower base to a higher base the cursor position
    //  is not too large for the new base.
    boundCursor();
}

/** displayOperator ************************************
 * Displays the currently selected operator to the screen.
 *****************************************************/
void displayOperator() {

    char str[3];
    
    //These if statements fill out our string with the
    //  appropriate characters for the selected operation.
    if(operator_index == 4) {
        str[0] = '&';
        str[1] = '&';
    }
    else if(operator_index == 5) {
        str[0] = '|';
        str[1] = '|';
    }
    else if(operator_index == 11) {
        str[0] = '<';
        str[1] = '<';
    }
    else if(operator_index == 12) {
        str[0] = '>';
        str[1] = '>';
    }
    else {
        str[0] = ' ';
        str[1] = operators[operator_index];
    }

    //Adds a null terminator to the string.
    str[2] = 0;

    //Draws the string to the screen, with location
    //  based on which base we are currently in.
    if(base == 16) {
        drawString(140, 20 + 24*6, str, BLUE);
    }
    else if(base == 10) {
        drawString(140, 20 + 22*6, str, BLUE);
    }
    else if(base == 2) {
        drawString(140, 20, str, BLUE);
    }
    else if(base == 8) {
        drawString(140, 20 + 21*6, str, BLUE);
    }
}

/** displayNumbers ***********************************
 * Draws the numbers of our stack onto the screen.
 *****************************************************/
void displayNumbers() {

    char str[33];
    int i;

    //For each number in our stack...
    for(i = 0; i < NUM_NUMS; i++) {

        //Draw the string to the screen with location based
        //  on which base we are currently in.
        if(base == 10) {
            sprintf(str, "%010u", numbers[i]);
            drawString(140 - 10*i, 40 + 22*6, str, BLUE);
        }
        else if(base == 16) {
            sprintf(str, "%08X", numbers[i]);
            drawString(140 - 10*i, 40 + 24*6, str, BLUE);
        }
        else if(base == 2) {
            int2bin(str, numbers[i]);
            drawString(140 - 10*i, 40, str, BLUE);
        }
        else if(base == 8) {
            sprintf(str, "%011o", numbers[i]);
            drawString(140 - 10*i, 40 + 21*6, str, BLUE);
        }
    }
}

/** moveCursorLeft *********************************
 * Moves the cursor left one digit.
 *****************************************************/
void moveCursorLeft() {
    cursor++;
    boundCursor();
}

/** moveCursorRight *********************************
 * Moves the cursor right one digit.
 *****************************************************/
void moveCursorRight() {
    cursor--;
    boundCursor();
}

/** boundCursor **************************************
 * Makes sure the cursor is within the proper range 
 * of our currently selected base.
 *****************************************************/
void boundCursor() {

    //These if statements check to see if the cursor is
    //  outside the maximum digit place for the current base.
    //  If the cursor is out of the bounds, move it to the
    //  maximum value.

    if(base == 2) {
        if(cursor >= 32) {
            cursor = 31;
        }
    }
    else if(base == 10) {
        if(cursor >= 10) {
            cursor = 9;
        }
    }
    else if(base == 16) {
        if(cursor >= 8) {
            cursor = 7;
        }
    }
    else if(base == 8) {
        if(cursor >= 11) {
            cursor = 10;
        }
    }
}

/** displayCursor *************************************
 * Draws the cursor to the screen at the currently
 * selected digit.
 *****************************************************/
void displayCursor() {
    drawRect(150, 40 + (31-cursor)*6, 2, 6, RED);
}

/** swapTopOfStack ************************************
 * Swaps the top two numbers of the stack.
 *****************************************************/
void swapTopOfStack() {
    unsigned int temp = numbers[0];
    numbers[0] = numbers[1];
    numbers[1] = temp;
}

/** int2bin *******************************************
 * Prints a number in binary to a given string.
 * char* p - The string we want to print to.
 * int n - The number we want printed.
 *****************************************************/
void int2bin(char* p, int n) {

    int i;
    for(i = 0; i < 32; i++) {

        if(n%2) {
            p[31-i] = '1';
        }
        else {
            p[31-i] = '0';
        }
        n = (n >> 1);
    }
    p[32] = 0;
}

/** displayInputPad *********************************
 * Draws the input pad to the screen.
 *****************************************************/
void displayInputPad() {

    char str[4];

    //For each of the following if statements, we will first
    //  draw two rectangles (the dark and light gray rectangles
    //  that are the background of the input pad), then we will
    //  highlight the currently selected input pad number, 
    //  and finally we will draw the available input pad numbers
    //  on top of that. This needs to be done on a per base
    //  basis because each base has different available numbers.

    if(base == 10) {
        drawRect(108, 26 + (31-cursor)*6, 42, 22, DKGRAY);
        drawRect(110, 28 + (31-cursor)*6, 38, 18, LTGRAY);
        drawHighlight();

        sprintf(str, "987");
        drawString(110, 28 + (31-cursor)*6, str, BLACK);
        sprintf(str, "654");
        drawString(120, 28 + (31-cursor)*6, str, BLACK);
        sprintf(str, "321");
        drawString(130, 28 + (31-cursor)*6, str, BLACK);
        sprintf(str, "  0");
        drawString(140, 28 + (31-cursor)*6, str, BLACK);
    }
    else if(base == 8) {
        drawRect(108, 26 + (31-cursor)*6, 42, 22, DKGRAY);
        drawRect(110, 28 + (31-cursor)*6, 38, 18, LTGRAY);
        drawHighlight();

        sprintf(str, "  7");
        drawString(110, 28 + (31-cursor)*6, str, BLACK);
        sprintf(str, "654");
        drawString(120, 28 + (31-cursor)*6, str, BLACK);
        sprintf(str, "321");
        drawString(130, 28 + (31-cursor)*6, str, BLACK);
        sprintf(str, "  0");
        drawString(140, 28 + (31-cursor)*6, str, BLACK);
    }
    else if(base == 2) {
        drawRect(128, 26 + 12 + (31-cursor)*6, 22, 10, DKGRAY);
        drawRect(130, 28 + 12 + (31-cursor)*6, 18, 6, LTGRAY);
        drawHighlight();

        sprintf(str, "  1");
        drawString(130, 28 + (31-cursor)*6, str, BLACK);
        sprintf(str, "  0");
        drawString(140, 28 + (31-cursor)*6, str, BLACK);
    }
    else if(base == 16) {
        drawRect(88, 26 + (31-cursor)*6, 62, 22, DKGRAY);
        drawRect(90, 28 + (31-cursor)*6, 58, 18, LTGRAY);
        drawHighlight();

        sprintf(str, "FED");
        drawString(90, 28 + (31-cursor)*6, str, BLACK);
        sprintf(str, "CBA");
        drawString(100, 28 + (31-cursor)*6, str, BLACK);
        sprintf(str, "987");
        drawString(110, 28 + (31-cursor)*6, str, BLACK);
        sprintf(str, "654");
        drawString(120, 28 + (31-cursor)*6, str, BLACK);
        sprintf(str, "321");
        drawString(130, 28 + (31-cursor)*6, str, BLACK);
        sprintf(str, "  0");
        drawString(140, 28 + (31-cursor)*6, str, BLACK);
    }
}

/** drawHighlight *************************************
 * Draws a red box underneath the currently selected 
 * input pad digit.
 *****************************************************/
void drawHighlight() {
    
    int row = 0;
    int col = 0;

    //The following if statements determine the row and column
    //  that the red box needs to be under for any input cursor value.

    if(input_cursor == 0) {
        row = 140;
        col = 28 + 12 + (31-cursor)*6;
    }
    else if(input_cursor >= 1 && input_cursor <= 3) {
        row = 130;
        col = 22 + (((input_cursor-1)*-1)+3)*6 + (31-cursor)*6;
    }
    else if(input_cursor >= 4 && input_cursor <= 6) {
        row = 120;
        col = 22 + (((input_cursor-4)*-1)+3)*6 + (31-cursor)*6;
    }
    else if(input_cursor >= 7 && input_cursor <= 9) {
        row = 110;
        col = 22 + (((input_cursor-7)*-1)+3)*6 + (31-cursor)*6;
    }
    else if(input_cursor >= 10 && input_cursor <= 12) {
        row = 100;
        col = 22 + (((input_cursor-10)*-1)+3)*6 + (31-cursor)*6;
    }
    else if(input_cursor >= 13 && input_cursor <= 15) {
        row = 90;
        col = 22 + (((input_cursor-13)*-1)+3)*6 + (31-cursor)*6;
    }

    //Draw our red highlighting box at the number's position.
    drawRect(row, col, 8, 6, RED);
}

/** displayLabels *************************************
 * Displays the labels at the top of the screen.
 *****************************************************/
void displayLabels() {

    char str[30];

    sprintf(str, "GBA Stack Calculator");
    drawString(20, 120-3*strlen(str), str, RED);

    sprintf(str, "Press Select for Instructions");
    drawString(35, 120-3*strlen(str), str, GREEN);

    sprintf(str, "Current Base: %d", base);
    drawString(50, 20, str, GREEN);
}

/** displayInstructionsScreen *************************
 * Displays the instructions screen on the display
 *****************************************************/
void displayInstructionsScreen() {

    char str[40];

    clearScreen();

    sprintf(str, "Instructions");
    drawString(10, 120-3*strlen(str), str, RED);

    sprintf(str, "Up...............Moves Stack Up");
    drawString(30, 120-3*strlen(str), str, BLUE);

    sprintf(str, "Down...........Moves Stack Down");
    drawString(40, 120-3*strlen(str), str, BLUE);

    sprintf(str, "Right........Moves Cursor Right");
    drawString(50, 120-3*strlen(str), str, BLUE);

    sprintf(str, "Left..........Moves Cursor Left");
    drawString(60, 120-3*strlen(str), str, BLUE);

    sprintf(str, "Start.....Opens Digit Input Pad");
    drawString(70, 120-3*strlen(str), str, BLUE);

    sprintf(str, "R..................Changes Base");
    drawString(80, 120-3*strlen(str), str, BLUE);

    sprintf(str, "L............Swaps Top of Stack");
    drawString(90, 120-3*strlen(str), str, BLUE);

    sprintf(str, "A.............Changes Operation");
    drawString(100, 120-3*strlen(str), str, BLUE);

    sprintf(str, "B............Performs Operation");
    drawString(110, 120-3*strlen(str), str, BLUE);

    sprintf(str, "Press Start to Return");
    drawString(140, 120-3*strlen(str), str, GREEN);

}

/** clearScreen ***************************************
 * Clears the screen.
 *****************************************************/
void clearScreen() {
    drawRect(0,0,160,240,BLACK);
}
