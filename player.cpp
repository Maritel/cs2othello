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
    
    //initialize the list of possible moves
    
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
    
    MoveInfo * moveInfo = getBestMove(board, playerSide, 2);
    
    
    //do my own move
	board->doMove(moveInfo->move, playerSide);
	
    return moveInfo->move;
}

void Player::setBoard(Board * otherBoard) {
	board = otherBoard;
}

/*
 * helper method for recursion of doMove()
 * 
 * lookDepth must be positive
 */
MoveInfo* Player::getBestMove(Board * board, Side side, int lookDepth) {
	if (lookDepth == 1) {
		std::vector<Move*> legalMoves = getLegalMoves(board, side); //get all legal moves
		if (legalMoves.size() == 0)
			return new MoveInfo(NULL, board->getScore(side)); //if there are no legal moves, pass
		
		//spawn copy of a board for testing
		Board * testBoard = board->copy();
		int bestScore = INT_MIN; //best score so far
		Move * bestMove = NULL;
		for (unsigned int i = 0; i < legalMoves.size(); i++) {
			Move * candidateMove = legalMoves[i];
			testBoard->doMoveUnchecked(candidateMove, side);
			int score = testBoard->getScore(side); //what's the score for this move?
			if (score > bestScore) {
				bestScore = score;
				bestMove = candidateMove;
			}
			testBoard->undoMove(candidateMove);
		}
		
		
		delete testBoard;
		
		for (unsigned int i = 0; i < legalMoves.size(); i++) {
			if (legalMoves[i] != bestMove) {
				delete legalMoves[i]; //we no longer need this anymore, so we deallocate it
			}
		}
		return new MoveInfo(bestMove, bestScore);
	}
	
	//assuming lookDepth > 1
	std::vector<Move*> legalMoves = getLegalMoves(board, side);
	if (legalMoves.size() == 0)
		return new MoveInfo(NULL, board->getScore(side));
	Side otherSide = (side == BLACK) ? WHITE : BLACK;
	
	//spawn copy of a board for testing
	Board * testBoard = board->copy();
	int bestScore = INT_MIN;
	Move * bestMove = NULL;
	for (unsigned int i = 0; i < legalMoves.size(); i++) {
		Move * candidateMove = legalMoves[i];
		testBoard->doMoveUnchecked(candidateMove,side);
		//now we want to minimize it, so we just make the score negative for our purposes
		int score = -(getBestMove(testBoard, otherSide, lookDepth - 1)->score); 
		if (score > bestScore) {
			bestScore = score;
			bestMove = candidateMove;
		}
		testBoard->undoMove(candidateMove);
	}
	delete testBoard;
	for (unsigned int i = 0; i < legalMoves.size(); i++) {
		if (legalMoves[i] != bestMove) {
			delete legalMoves[i]; //we no longer need this anymore, so we deallocate it
		}
	}
	return new MoveInfo(bestMove, bestScore);
}

/*
 * helper method which gives all the legal moves
 */
std::vector<Move*> Player::getLegalMoves(Board * board, Side side) {
	std::vector<Move*> legalMoves;
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			Move candidateMove(r,c);
			if (board->checkMove(&candidateMove, side))
				legalMoves.push_back(new Move(candidateMove.getX(), candidateMove.getY()));
		}
	}
	return legalMoves;
}
