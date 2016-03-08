#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include <climits>
#include "common.h"
#include "board.h"
#include <cstdlib>
using namespace std;

class Player {
	
	
private:
	Board * board;
	Side playerSide;
	Side otherSide;

public:
    Player(Side side);
    ~Player();
    
    Move * doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    
    Move* getBestMove(Board * board, Side side, int lookDepth); 
    
};

#endif
