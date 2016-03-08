#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
    board = new Board();
    playerSide = side;
    otherSide = (playerSide == BLACK) ? WHITE : BLACK;
    
    srand(time(NULL)); //one seed for the entire game
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}


/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move* Player::doMove(Move *opponentsMove, int msLeft) {
    //make opponent's move on the board
    board->doMove(opponentsMove, otherSide);
    
    
    //get all possible moves for the player
    std::vector<Move> legalMoves = board->getLegalMoves(playerSide);
	
	if (legalMoves.size() == 0) { //if no legal moves, pass
		return NULL;
	}
    
    //spawn copy of a board for testing
    int bestScore = INT_MIN;
    Move bestMove = legalMoves[0];
    Board * testBoard = board->copy();
    for (unsigned int i = 0; i < legalMoves.size(); i++) {
		Move candidateMove = legalMoves[i];
		testBoard->doMoveUnchecked(&candidateMove, playerSide);
		int score = testBoard->getStoneDifference(playerSide);
		if (score > bestScore) {
			bestScore = score;
			bestMove = candidateMove;
		}
		testBoard->undoMove(&candidateMove);
	}
	delete testBoard;
	
	Move * finalMove = new Move(bestMove.getX(), bestMove.getY());
	
    //do my own move
    board->doMove(finalMove, playerSide);
    
    return finalMove;
}

/*
 * helper method for recursion of doMove()
 */
Move* Player::getBestMove(Board * board, Side side, int lookDepth) {
	return NULL;
}
