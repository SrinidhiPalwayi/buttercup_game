#include <stdio.h>

#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3

#define SCANLINECOUNTER *(volatile unsigned short *) 0x4000006

#define SCREENHEIGHT 149

#define BG2_ENABLE (1<<10)
#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define YELLOW COLOR(31, 31,0)
#define OFFSET(r, c, rowlen) ((r)*(rowlen)+(c))
#define BLACK 0

//store some 32 bit type


extern const unsigned char fontdata_6x8[12288];

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

#define BUTTONS *(volatile unsigned int *)0x4000130





unsigned short *videoBuffer = (unsigned short *)0x6000000;

// Prototypes
void setPixel(int row, int col, unsigned short color);
void drawRect(int row, int col, int height, int width, unsigned short color);
void delay(int n);
void waitForVblank();
void drawChar(int row, int col, char ch, unsigned short color);
void drawString(int row, int col, char str[], unsigned short color);

int main()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	//* (OFFSET(80, 120, 240)+videoBuffer) = YELLOW;
	//
	int row = 80;
	int col = 120;
	int rdel = 1;
	int cdel = 1;
	int size = 5;
	int oldrow = row;
	int oldcol = col;
	int oldsize = size;

	int rowFood = (rand() % SCREENHEIGHT/5) * 5;
	int colFood = (rand() % 240/5) * 5;
	int size = 5;
/*
	char ch = 0;
	for(int r=0; r<16; r++)
	{
		for(int c=0; c<16; c++)
		{
			drawChar(r*9, c*9, ch++, YELLOW);
		}
	}
	while(1);
*/
	int score = 453;
	char buffer[41];
	sprintf(buffer, "Score: %d", score);
	//char message[] = "Dear Mom, Send Money!";
	//char *mess = "Dear Mom, Send Money!";
	drawString(150, 5, buffer, YELLOW);

	while(1)
	{
		if(KEY_DOWN_NOW(BUTTON_UP))
		{
			size = size + 1;
			if(size>150)
			{
				size = 150;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_DOWN))
		{
			size = size - 1;
			if(size<3)
			{
				size=3;
			}
		}
		row = row + rdel;
		col = col + cdel;
		if(row < 0)
		{
			row = 0;
			rdel = -rdel;
		}
		if(row > SCREENHEIGHT-size+1)
		{
			row = SCREENHEIGHT-size+1;
			rdel = -rdel;
		}
		if(col < 0)
		{
			col = 0;
			cdel = -cdel;
		}
		if(col > 239-size+1)
		{
			col = 239-size+1;
			cdel = -cdel;
		}
		waitForVblank();
		drawRect(150, 5, 10, 70, BLACK);
				score++;
		sprintf(buffer, "Score: %d", score);
		drawString(150, 5, buffer, YELLOW);

		drawRect(oldrow, oldcol, oldsize, oldsize, BLACK);
		drawRect(row, col, size, size, YELLOW);
		oldrow = row;
		oldcol = col;
		oldsize = size;
		
		
	}

}

void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color)
{
	for(int r=0; r<height; r++)
	{
		for(int c=0; c<width; c++)
		{
			setPixel(row+r, col+c, color);
		}
	}
}


void delay(int n)
{
	volatile int x = 0;
	for(int i=0; i<n*5000; i++)
	{
		x = x + 1;
	}
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void drawChar(int row, int col, char ch, unsigned short color)
{
	for(int r = 0; r< 8; r++)
	{
		for(int c=0; c< 6; c++)
		{
			if( fontdata_6x8[OFFSET(r, c, 6) + ch*48] == 1)
			{
				setPixel(r+row, c+col, color);
			}
		}

	}
}

void drawString(int row, int col, char str[], unsigned short color)
{
	while(*str)
	{
		drawChar(row, col, *str++, color);
		col += 6;
	}
}


