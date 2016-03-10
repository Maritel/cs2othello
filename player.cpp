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
    board->doMove(opponentsMove, otherSide); //make opponent's move on the board
    Move* selectedMove = getBestMove(board, 2, INT_MIN, INT_MAX, true); //using minimax to find best move
    board->doMove(selectedMove, playerSide); //perform my own move
    return selectedMove;
}

Move* Player::getBestMove(Board * board, int depth, int alpha, int beta, bool isPlayerSide) {
	if (depth == 0)
		return NULL; //nothing to do here lol
	if (isPlayerSide) {
		//get all legal moves
		std::vector<Move*> legalMoves = getLegalMoves(board, playerSide);
		if (legalMoves.size() == 0)
			return NULL; //no legal moves, return null
		
		Board * testBoard = board->copy(); //board to test our legal moves on
		int bestScore = INT_MIN; //best score obtained from our legal moves
		Move* bestMove = NULL; //best move obtained from our legal moves
		for (unsigned int i = 0; i < legalMoves.size(); i++) {
			Move* candidateMove = legalMoves[i];
			testBoard->doMoveUnchecked(candidateMove, playerSide);
			//update bestScore, bestMove
			int score = getBestScore(testBoard, depth - 1, alpha, beta, false);
			if (score > bestScore) {
				bestScore = score;
				bestMove = candidateMove;
			}

			alpha = std::max(alpha, bestScore); //update alpha
			
			testBoard->undoMove(candidateMove); //revert the testboard back to the original position
			if (beta <= alpha) //alpha-beta pruning
				break;
		}	
		delete testBoard; //freeing the testboard, as we no longer need it
		for (unsigned int i = 0; i < legalMoves.size(); i++)
			if (legalMoves[i] != bestMove)
				delete legalMoves[i]; //no longer need any of the Move* variables, except for bestMove
		
		return bestMove;
	}
	else { //not the player's turn to move
		std::vector<Move*> legalMoves = getLegalMoves(board, otherSide);
		if (legalMoves.size() == 0)
			return NULL; //no legal moves, return null
		
		Board * testBoard = board->copy(); //board to test our legal moves on
		int worstScore = INT_MAX; //worst score (for playerSide) obtained from our legal moves
		Move* worstMove = NULL; //worst move (for playerSide) obtained from our legal moves
		for (unsigned int i = 0; i < legalMoves.size(); i++) {
			Move* candidateMove = legalMoves[i];
			testBoard->doMoveUnchecked(candidateMove, otherSide);
			//update worstScore, worstMove
			int score = getBestScore(testBoard, depth - 1, alpha, beta, true);
			if (score < worstScore) {
				worstScore = score;
				worstMove = candidateMove;
			}
			
			beta = std::min(beta, worstScore); //update beta
			
			testBoard->undoMove(candidateMove); //revert the testboard back to the original position
			if (beta <= alpha) //alpha-beta pruning
				break;
		}
		delete testBoard; //freeing the testboard, as we no longer need it
		for (unsigned int i = 0; i < legalMoves.size(); i++)
			if (legalMoves[i] != worstMove)
				delete legalMoves[i]; //no longer need any of the Move* variables, except for bestMove
		
		return worstMove;
	}
}

int Player::getBestScore(Board * board, int depth, int alpha, int beta, bool isPlayerSide) {
	if (depth == 0)
		return getScore(board);
	if (isPlayerSide) {
		std::vector<Move*> legalMoves = getLegalMoves(board, playerSide);
		if (legalMoves.size() == 0)
			return getScore(board);
			
		Board * testBoard = board->copy();
		int bestScore = INT_MIN;
		for (unsigned int i = 0; i < legalMoves.size(); i++) {
			Move* candidateMove = legalMoves[i];
			testBoard->doMoveUnchecked(candidateMove, playerSide);
			bestScore = std::max(bestScore, getBestScore(testBoard, depth - 1, alpha, beta, false)); //update bestScore
			alpha = std::max(alpha, bestScore); //update alpha
			
			testBoard->undoMove(candidateMove); //revert position
			if (beta <= alpha) //alpha-beta pruning
				break;
		}
		delete testBoard;
		for (unsigned int i = 0; i < legalMoves.size(); i++)
			delete legalMoves[i];
		
		return bestScore;
	}
	else {
		std::vector<Move*> legalMoves = getLegalMoves(board, otherSide);
		if (legalMoves.size() == 0)
			return getScore(board);
		
		Board * testBoard = board->copy();
		int worstScore = INT_MAX;
		for (unsigned int i = 0; i < legalMoves.size(); i++) {
			Move* candidateMove = legalMoves[i];
			testBoard->doMoveUnchecked(candidateMove, otherSide); 
			worstScore = std::min(worstScore, getBestScore(testBoard, depth - 1, alpha, beta, true)); //update worstScore
			beta = std::min(beta, worstScore); //update beta
			
			testBoard->undoMove(candidateMove);
			if (beta <= alpha) //alpha-beta pruning
				break;
		}
		delete testBoard;
		for (unsigned int i = 0; i < legalMoves.size(); i++)
			delete legalMoves[i];
		
		return worstScore;
	}
}


/*
 * General function which returns an int score for the board position at the current state.
 */
int Player::getScore(Board * board) {
	return getStoneDifference(board);
}

/*
 * Simplest scoring heuristic: (playerSide stones) - (otherSide stones)
 */
int Player::getStoneDifference(Board * board) {
	if (playerSide == BLACK)
		return board->countBlack() - board->countWhite();
	else
		return board->countWhite() - board->countBlack();
}


/*
 * Returns all legal moves for a given side.
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


void Player::setBoard(Board * otherBoard) {
	board = otherBoard;
}
