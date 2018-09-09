#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color)
{
    videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color)
{
    int r, c;
    for(r=0; r<height; r++)
    {
        for(c=0; c<width; c++)
        {
            setPixel(row+r, col+c, color);
        }
    }
}

void drawImage3(int row, int col, int width, int height, const u16* image) {
    for (int r = 0; r < height; r++) {
        DMA[3].src = &image[r*width ]; // This is always an address!
        DMA[3].dst = &videoBuffer[OFFSET(row+r, col, 240)]; // This is always an address!
        //REG_DMA3DAD = (u32)(videoBuffer + OFFSET(row+r, col, 240) ); // This is always an address!
        DMA[3].cnt = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;


    }
}




void drawChar(int row, int col, char ch, unsigned short color)
{
    int r,c;
    for(r=0; r<8; r++)
    {
        for(c=0; c< 6; c++)
        {
            if(fontdata_6x8[OFFSET(r, c, 6)+48*ch])
            {
                setPixel(r+row, c+col, color);
            }
        }
    }
    
}

void drawString(int row, int col, char *str, unsigned short color)
{
    while(*str)
    {
        drawChar(row, col, *str++, color);
        col += 6;
    }
}
     

void waitForVblank()
{
    while(SCANLINECOUNTER > 160);
    while(SCANLINECOUNTER < 161);
}


      