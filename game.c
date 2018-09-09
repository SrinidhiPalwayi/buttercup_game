#include "game.h"

int rDel;
int cDel;


bool girlWallCollision(GIRL *girl) {
	girl->row = girl->row + girl->rdel;
	girl->col = girl->col + girl->cdel;
}

