#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int r, int c, unsigned short color)
{
    videoBuffer[OFFSET(r, c)] = color;
}

void drawRect(int row, int col, int height, int width, volatile unsigned short color)
{
    int r;
    for(r=0; r<height; r++)
    {
        
        DMA[3].src = &color; 
        DMA[3].dst = &videoBuffer[OFFSET(row+r, col)];
        DMA[3].cnt = width |  DMA_SOURCE_FIXED | DMA_ON;       
    }
}

void waitForVblank()
{
    while(SCANLINECOUNTER > 160);
    while(SCANLINECOUNTER < 160);
}


