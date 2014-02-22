#include <stdio.h>
#include <math.h>
#include "myLib.h"
#include "text.h"
#include "main.h"

int main()
{
    REG_DISPCTL = BG2_ENABLE | MODE3;

    base = 10;

    cursor = 0;

    numbers[0] = 0;
    numbers[1] = 0;
    numbers[2] = 0;
    numbers[3] = 0;
    numbers[4] = 0;

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

    operator_index = 0;

    updateScreen();

    while(1) {

        old_buttons = buttons;
        buttons = BUTTONS;

        checkForInput();
    }
}

void updateScreen() {

    waitForVblank();
    clearScreen();
    displayLabels();
    displayNumbers();
    displayOperator();
    displayCursor();
}

void checkForInput() {

    if(BUTTON_PRESSED(BUTTON_R)) {
        switchBase();
        updateScreen();
    }

    if(BUTTON_PRESSED(BUTTON_L)) {
        swapTopOfStack();
        updateScreen();
    }

    if(BUTTON_PRESSED(BUTTON_START)) {
        numbers[4] = numbers[3];
        numbers[3] = numbers[2];
        numbers[2] = numbers[1];
        numbers[1] = numbers[0];
        updateScreen();
    }

    if(BUTTON_PRESSED(BUTTON_SELECT)) {
        numbers[0] = numbers[1];
        numbers[1] = numbers[2];
        numbers[2] = numbers[3];
        numbers[3] = numbers[4];
        updateScreen();
    }

    if(BUTTON_PRESSED(BUTTON_A)) {
        switchOperator();
        updateScreen();
    }

    if(BUTTON_PRESSED(BUTTON_B)) {
        performOperation();
        updateScreen();
    }

    if(BUTTON_PRESSED(BUTTON_LEFT)) {
        moveCursorLeft();
        updateScreen();
    }

    if(BUTTON_PRESSED(BUTTON_RIGHT)) {
        moveCursorRight();
        updateScreen();
    }

    if(BUTTON_PRESSED(BUTTON_UP)) {
        incrementDigit();
        updateScreen();
    }

    if(BUTTON_PRESSED(BUTTON_DOWN)) {
        decrementDigit();
        updateScreen();
    }
}

void performOperation() {

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

void switchOperator() {
    operator_index++;
    operator_index %= 13;
}

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

    boundCursor();
}

void displayOperator() {
    char str[3];
    
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

    str[2] = 0;

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

void displayNumbers() {

    char str[33];
    int i;

    if(base == 10) {
        for(i = 0; i < NUM_NUMS; i++) {
            sprintf(str, "%010u", numbers[i]);
            drawString(140 - 10*i, 40 + 22*6, str, BLUE);
        }
    }
    else if(base == 16) {
        for(i = 0; i < NUM_NUMS; i++) {
            sprintf(str, "%08X", numbers[i]);
            drawString(140 - 10*i, 40 + 24*6, str, BLUE);
        }
    }
    else if(base == 2) {
        for(i = 0; i < NUM_NUMS; i++) {
            int2bin(str, numbers[i]);
            drawString(140 - 10*i, 40, str, BLUE);
        }
    }
    else if(base == 8) {
        for(i = 0; i < NUM_NUMS; i++) {
            sprintf(str, "%011o", numbers[i]);
            drawString(140 - 10*i, 40 + 21*6, str, BLUE);
        }
    }
}

void incrementDigit() {
    if(!digitIsMax()) {
        numbers[0] += pow(base, cursor);
    }
}

void decrementDigit() {
    if(!digitIsZero()) {
        numbers[0] -= pow(base, cursor);
    }
}

unsigned int digitIsMax() {

    int i = 0;
    unsigned int ret = numbers[0];
    for(i = 0; i < cursor; i++) {
        ret /= base;
    }
    return (ret%base == (base - 1));
}

unsigned int digitIsZero() {

    int i = 0;
    unsigned int ret = numbers[0];
    for(i = 0; i < cursor; i++) {
        ret /= base;
    }
    return !(ret%base);
}

void moveCursorLeft() {
    cursor++;
    boundCursor();
}

void moveCursorRight() {
    cursor--;
    boundCursor();
}

void boundCursor() {

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

void displayCursor() {
    drawRect(150, 40 + (31-cursor)*6, 2, 6, RED);
}

void swapTopOfStack() {
    unsigned int temp = numbers[0];
    numbers[0] = numbers[1];
    numbers[1] = temp;
}

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

void displayLabels() {

    char str[20];

    sprintf(str, "GBA Stack Calculator");
    drawString(20, 120-3*strlen(str), str, GREEN);

    sprintf(str, "Current Base: %d", base);
    drawString(50, 20, str, GREEN);
}

void clearScreen() {
    drawRect(0,0,160,240,BLACK);
}
