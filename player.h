#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <vector>
#include <climits>
#include "common.h"
#include "board.h"
#include <cstdlib>
using namespace std;



class MoveInfo {
public:
	Move * move;
	int score;
	MoveInfo(Move * move, int score) {
		this->move = move;
		this->score = score;
	}
	
	~MoveInfo() {}
};

class Player {
	
	
private:
	Board * board;
	Side playerSide;
	Side otherSide;
	

public:

    Player(Side side);
    ~Player();
    
    void setBoard(Board * otherBoard);
    
    Move * doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    
    MoveInfo* getBestMove(Board * board, Side side, int lookDepth); 
    
    std::vector<Move*> getLegalMoves(Board * board, Side side);
    
};

#endif
