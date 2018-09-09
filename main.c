
#include <stdio.h>
#include <stdlib.h>
#include "myLib.h"
#include "background.h"
#include "girl.h"
#include "game.h"
#include "jojo.h"
#include "win.h"
#include "df.h"


// Prototypes

void waitForVblank();
int splash();
void winDone();
void game();
void lose();
int whichKey();
int girlWallCollision();
void makeVillain();
int checkCollision();
void deconstructVillain();




enum {SPLASH, GAME, WIN, LOSE};
enum {UP, DOWN, LEFT, RIGHT,STOP};

POWERPUFF buttercup;
POWERPUFF old;
ENEMY newjojo;
ENEMY oldjojo;
int state = SPLASH;
int main()
{
	int seed = 0;
    
    while(1)
    {
        switch (state)
        {
            case SPLASH:
                seed = splash();
                state = GAME;
                break;
            case GAME:
            	while(KEY_DOWN_NOW(BUTTON_SELECT)) {

            	}
            	if(KEY_DOWN_NOW(BUTTON_START)) {
            		game(seed);
            	}
       
                break;
            case WIN:
                winDone();
                state= SPLASH;
            	
                //state = SPLASH;
                break;
            case LOSE:
                lose();
                state = SPLASH;
                break;
            
        }
    }
}

int splash()
{       
    int seed=0;
    REG_DISPCTL = MODE3 | BG2_ENABLE;
    drawImage3(0,0,240,160, background);
    
    drawString(130,10, "PRESS START: TO HELP BUTTERCUP ", BLACK);
    drawString(140,10, "DEFEAT THE MULTIPLYING MOJO JOJOS ", BLACK);
    while(!KEY_DOWN_NOW(BUTTON_START))
        seed++;  // seed will have a value dependant on how long before 
    return seed; // user presses start
}

void winDone()
{
    //volatile unsigned short color = GREEN;
    
    drawImage3(0,0,240,160, win);
   // drawString(130,10, "PRESS START: ", BLACK);
    drawString(140,10, "THANKS FOR HELPING DEFEAT MOJO JOJO!", GREEN);
    while(!KEY_DOWN_NOW(BUTTON_SELECT));
    if(KEY_DOWN_NOW(BUTTON_SELECT)) {
    	state = SPLASH;
    }
    
}
    
void lose()
{
    drawImage3(0,0,240,160, defeated);
   // drawString(130,10, "PRESS START: ", BLACK);
    drawString(40,20, "MOJO JOJO HAS WON!!!", BLACK);
    while(!KEY_DOWN_NOW(BUTTON_SELECT));
    if(KEY_DOWN_NOW(BUTTON_SELECT)) {
    	state = SPLASH;
    }
    //while(KEY_DOWN_NOW(BUTTON_START));
}

void game(int seed)
{
    REG_DISPCTL = MODE3 | BG2_ENABLE;
    //char buffer[41];
    //int deltas[] = {-3, -2, -1, 1, 2, 3};
    // This calculates number of elements in deltas so if someone
    // adds more values in curly braces calc is done automatically
    
    srand(seed); // seed from user generates a different unique set of rand nos    

    buttercup.row = (rand() % (SCREENHEIGHT -CHARSIZE));
   	buttercup.col = (rand() % (SCREENWIDTH - CHARSIZE));
   	old.row = buttercup.row;
   	old.col = buttercup.col;
   	old.size = CHARSIZE;
   	newjojo.row = (rand() % (SCREENHEIGHT -CHARSIZE));
   	newjojo.col = (rand() % (SCREENWIDTH - CHARSIZE));
   	newjojo.size = CHARSIZE;

   	oldjojo.row = newjojo.row;
   	oldjojo.col = newjojo.col;
   	oldjojo.size = CHARSIZE;

   	int score = 0;
   	char buffer[41];
   	int temp = 1000;




   	buttercup.size = CHARSIZE;
   	buttercup.alive = 1;
    buttercup.cDel = CHARSIZE;
    buttercup.rDel = CHARSIZE;



    int tracker = STOP;


    u16 bgcolor = BLACK;
    
    DMA[3].src = &bgcolor;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = (240*160) | DMA_SOURCE_FIXED | DMA_ON;
    sprintf(buffer, "Score: %d", score);
   	drawString(150, 5, buffer, YELLOW);	
    makeVillain();
        
        
    while(1)
    {	
    	
    	old.row = buttercup.row;
   		old.col = buttercup.col;
   		tracker = whichKey(tracker);


   		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
   			state = SPLASH;
   			break;
   			//splash();
   		}



		
		if(tracker == UP)
		{
			buttercup.row = buttercup.row - buttercup.rDel;	
			
		}
		if(tracker == RIGHT)
		{
			buttercup.col = buttercup.col + buttercup.cDel;
			
		}
		if(tracker == DOWN)
		{
			buttercup.row = buttercup.row + buttercup.rDel;
		}
		if(tracker == LEFT)
		{
			buttercup.col = buttercup.col - buttercup.cDel;
		}
		
		for (int i = 0; i < 7; i++) {
			waitForVblank();
		}
		temp = temp -10;

		if (girlWallCollision(&buttercup) == 0) {
			state = LOSE;
			break;
		}

		if (checkCollision(&buttercup, &newjojo) == 1 ) {
			score = score + temp;
			drawRect(150, 5, 9, 78, bgcolor); // Erase prev score
        	sprintf(buffer, "Score: %d", score);
        	drawString(150, 5, buffer, YELLOW);	
        	temp = 1000;

        	if (score >= 5000) {
        		state = WIN;
        		break;

        	}
		
			deconstructVillain();

		}
		
		drawImage3(buttercup.row,buttercup.col, GIRL_WIDTH, GIRL_HEIGHT, girl);
		//drawImage3(newjojo.row,newjojo.col, JOJO_WIDTH, JOJO_HEIGHT, jojo);

		if (old.row != buttercup.row || old.col != buttercup.col) {
			drawRect(old.row, old.col, buttercup.size, buttercup.size, BLACK);

		}		
		
	}   
            
}

int whichKey(int t) {
	int tracker = t;

	if(KEY_DOWN_NOW(BUTTON_UP))
	{
		//buttercup.row = buttercup.row - buttercup.rDel;	
		tracker = UP;		
	}
	else if(KEY_DOWN_NOW(BUTTON_DOWN))
	{
		//buttercup.row = buttercup.row + buttercup.rDel;
		tracker = DOWN;
	}

	else if(KEY_DOWN_NOW(BUTTON_RIGHT))
	{
		//buttercup.col = buttercup.col + buttercup.cDel;
		tracker = RIGHT;
	}

	else if(KEY_DOWN_NOW(BUTTON_LEFT))
	{
		//buttercup.col = buttercup.col - buttercup.cDel;
		tracker = LEFT;
	}
	return tracker;

}

int girlWallCollision(POWERPUFF *girl) {

	if( (girl->row < 0) || (girl->row > SCREENHEIGHT-CHARSIZE+1) || (girl->col < 0) || (girl->col > SCREENWIDTH-CHARSIZE+1))
	{
		girl-> alive  = 0;
		return 0;
	}
	return 1;


}

int checkCollision(POWERPUFF *girl, ENEMY *jojo) {
	if( (girl -> col >= jojo -> col  && girl -> col  <= jojo -> col + JOJO_WIDTH -1) 
		|| (girl -> col + GIRL_WIDTH -1  >= jojo -> col  && girl -> col + GIRL_WIDTH -1 <= jojo -> col + JOJO_WIDTH -1 ) ){
		
		if ((girl -> row >= jojo -> row && girl -> row <= (jojo -> row + JOJO_HEIGHT -1)) || 
			(girl -> row + GIRL_HEIGHT >= jojo -> row && girl -> row + GIRL_HEIGHT <= (jojo -> row + JOJO_HEIGHT -1)))
		 {
			return 1;
		}
	}
	return 0;
}

void deconstructVillain() {
	oldjojo.row = newjojo.row;
   	oldjojo.col = newjojo.col;
   	drawRect(oldjojo.row, oldjojo.col, JOJO_WIDTH, JOJO_HEIGHT, BLACK);
   	newjojo.row = (rand() % (SCREENHEIGHT -CHARSIZE));
   	newjojo.col = (rand() % (SCREENWIDTH - CHARSIZE));
   	makeVillain();
}


void makeVillain() {
	drawImage3(newjojo.row,newjojo.col, newjojo.size, newjojo.size, jojo);
}


	

	
