#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <vector>
#include <climits>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include "common.h"
#include "board.h"
using namespace std;




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
    
    std::vector<Move*> getLegalMoves(Board * board, Side side);
    
    // two versions of alpha-beta; one version to return Move* and one to return int (scores) 
    Move * getBestMove(Board * board, int depth, int alpha, int beta, bool isPlayerSide);
    int getBestScore(Board * board, int depth, int alpha, int beta, bool isPlayerSide);
    
    
    // heuristics
    int getScore(Board* board);
    int getStoneParity(Board* board);
    int getMobilityScore(Board* board);
    int getCornerScore(Board* board);
    int getStabilityScore(Board* board);
    
    
};

#endif
