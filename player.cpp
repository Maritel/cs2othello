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
    
    Move* alphaBetaMove = alphaBeta(board, 4, INT_MIN, INT_MAX, playerSide)->move;
    
    //do my own move
	board->doMove(alphaBetaMove, playerSide);
    return alphaBetaMove;
}

void Player::setBoard(Board * otherBoard) {
	board = otherBoard;
}

MoveInfo* Player::alphaBeta(Board * board, int depth, int alpha, int beta, bool isPlayerSide) {
	if (depth == 0) 
		return new MoveInfo(NULL, getScore(board));
	
	if (isPlayerSide) {
		std::vector<Move*> legalMoves = getLegalMoves(board, playerSide);
		if (legalMoves.size() == 0)
			return new MoveInfo(NULL, getScore(board));
		
		Board * testBoard = board->copy();
		Move * bestMove = NULL;
		int bestScore = INT_MIN;
		for (unsigned int i = 0; i < legalMoves.size(); i++) {
			Move * candidateMove = legalMoves[i];
			testBoard->doMoveUnchecked(candidateMove, playerSide);
			int score = alphaBeta(testBoard, depth - 1, alpha, beta, false)->score;
			if (score > bestScore) {
				bestScore = score;
				bestMove = candidateMove;
			}
			alpha = std::max(alpha, bestScore);
			testBoard->undoMove(candidateMove);
			if (alpha >= beta)
				break;
		}
		delete testBoard;
		for (unsigned int i = 0; i < legalMoves.size(); i++)
			if (legalMoves[i] != bestMove)
				delete legalMoves[i];
		
		return new MoveInfo(bestMove, bestScore);
	}
	else {
		std::vector<Move*> legalMoves = getLegalMoves(board, otherSide);
		if (legalMoves.size() == 0)
			return new MoveInfo(NULL, getScore(board));
		
		Board * testBoard = board->copy();
		Move * worstMove = NULL;
		int worstScore = INT_MAX;
		for (unsigned int i = 0; i < legalMoves.size(); i++) {
			Move * candidateMove = legalMoves[i];
			testBoard->doMoveUnchecked(candidateMove, otherSide);
			int score = alphaBeta(testBoard, depth - 1, alpha, beta, true)->score;
			if (score < worstScore) {
				worstScore = score;
				worstMove = candidateMove;
			}
			beta = std::min(beta, worstScore);
			testBoard->undoMove(candidateMove);
			if (alpha >= beta)
				break;
		}
		delete testBoard;
		for (unsigned int i = 0; i < legalMoves.size(); i++)
			if (legalMoves[i] != worstMove)
				delete legalMoves[i];
		
		return new MoveInfo(worstMove, worstScore);
	}
}

int Player::getScore(Board * board) {
	return getStoneDifference(board);
}

int Player::getStoneDifference(Board * board) {
	if (playerSide == BLACK)
		return board->countBlack() - board->countWhite();
	else
		return board->countWhite() - board->countBlack();
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
