#include "player.h"

#define NORMALIZATION_CONSTANT 10000

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
    Move* selectedMove = getBestMove(board, 6, INT_MIN, INT_MAX, true); //using minimax to find best move
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

/*
 * 
 * HEURISTICS
 * 
 */


/*
 * General heuristic function to avoid replacing the rest of my code whenever I decide to change the heuristic.
 */
int Player::getScore(Board* board) {
	int parityPart = getStoneParity(board);
	//~ int mobilityPart = getMobilityScore(board);
	int mobilityPart = 0;
	int cornerPart = getCornerScore(board);
	int stabilityPart = getStabilityScore(board);
	return parityPart + mobilityPart + cornerPart + stabilityPart;
}

/*
 * Simplest scoring heuristic based on the number of stones of each color.
 */
int Player::getStoneParity(Board* board) {
	int playerStones = (playerSide == BLACK) ? board->countBlack() : board->countWhite();
	int opponentStones = (playerSide == BLACK) ? board->countWhite() : board->countBlack();
	return (NORMALIZATION_CONSTANT * (playerStones - opponentStones)) / (playerStones + opponentStones); 
}

/*
 * Scoring heuristic based on the difference in mobility (# of legal moves) between the player and the opponent.
 */
int Player::getMobilityScore(Board* board) {
	int playerMoves = 0;
	int opponentMoves = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Move move(i,j);
			if (board->checkMove(&move,playerSide))
				playerMoves++;
			if (board->checkMove(&move,otherSide))
				opponentMoves++;
		}
	}
	if (playerMoves + opponentMoves == 0)
		return 0;
	else
		return (NORMALIZATION_CONSTANT * (playerMoves - opponentMoves)) / (playerMoves + opponentMoves);
}

/*
 * Scoring heuristic based on the number of corners captured. Attaching values to squares seems to be a questionable
 * strategy, but one thing that remains constant is that corners are extremely important.
 */ 
int Player::getCornerScore(Board* board) {
	int playerCorners = 0;
	int opponentCorners = 0;
	for (int i = 0; i < 8; i += 7) {
		for (int j = 0; j < 8; j += 7) {
			if (board->get(playerSide, i, j))
				playerCorners++;
			else if (board->get(otherSide, i, j))
				opponentCorners++;
		}
	}
	if (playerCorners + opponentCorners == 0)
		return 0;
	else
		return (NORMALIZATION_CONSTANT * (playerCorners - opponentCorners)) / 4;
}

/*
 * A "stable" stone is one that cannot be overturned. For example, corners are automatically stable.
 * Stability is obtained if, for all four axes (diagonals, horizontal, and vertical), either:
 * (1) The piece is on a boundary.
 * (2) The axis is filled.
 * (3) The piece is next to a stable piece of the same color.
 * 
 * Conditions 1 and 2 are easy to compute. Once we have obtained all the stable pieces from 1 and 2, we can repeatedly 
 * check for more stable pieces given condition 3.
 */
int Player::getStabilityScore(Board* board) {
	std::vector<bool> stablePlayer(64, false); //Position i,j will be indexed 8*i + j
	std::vector<bool> stableOpponent(64, false);
	/*
	 * Check for filled rows, columns, and diagonals.
	 */
	std::vector<bool> filledRows(8,true);
	std::vector<bool> filledCols(8,true);
	//a positive diagonal's index is indicated by i+j, which can take values from 0 to 14
	std::vector<bool> filledPosDiags(15,true);
	//a negative diagonal's index is indicated by i-j+7
	std::vector<bool> filledNegDiags(15,true); 
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!(board->occupied(i,j))) {
				filledRows[i] = false;
				filledCols[j] = false;
				filledPosDiags[i+j] = false;
				filledNegDiags[i-j+7] = false;
			}
		}
	}
	
	/*
	 * Get all Type 1 and Type 2 stable disks
	 */
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			bool occupied = board->occupied(i,j);
			bool horizontalBoundary = (i == 0) || (i == 7);
			bool verticalBoundary = (j == 0) || (j == 7);
			bool horizontal = horizontalBoundary || filledRows[i];
			bool vertical = verticalBoundary || filledCols[j];
			bool posDiag = horizontalBoundary || verticalBoundary || filledPosDiags[i+j];
			bool negDiag = horizontalBoundary || verticalBoundary || filledNegDiags[i-j+7];
			if(occupied && horizontal && vertical && posDiag && negDiag) {
				if (board->get(playerSide, i, j))
					stablePlayer[8*i + j] = true;
				else
					stableOpponent[8*i + j] = true;
			}
		}
	}
	
	/*
	 * Seek disks with type 3 requirement
	 */
	int improvement = INT_MAX;
	while(improvement > 0) {
		improvement = 0;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board->get(playerSide, i, j) && !stablePlayer[8*i + j]) {
					 
					bool horizontalBoundary = (i == 0) || (i == 7);
					bool verticalBoundary = (j == 0) || (j == 7);
					bool boundary = horizontalBoundary || verticalBoundary;
					
					bool horizontalNeighbor = horizontalBoundary || stablePlayer[8*(i-1) + j] || stablePlayer[8*(i+1) + j];
					bool verticalNeighbor = verticalBoundary || stablePlayer[8 * i + (j-1)] || stablePlayer[8 * i + (j+1)];
					bool posDiagNeighbor = boundary || stablePlayer[8*(i+1) + (j-1)] || stablePlayer[8*(i-1) + (j+1)];
					bool negDiagNeighbor = boundary || stablePlayer[8*(i-1) + (j-1)] || stablePlayer[8*(i+1) + (j+1)];
					
					bool horizontal = filledRows[i] || horizontalNeighbor;
					bool vertical = filledCols[j] || verticalNeighbor;
					bool posDiag = posDiagNeighbor || filledPosDiags[i+j];
					bool negDiag = negDiagNeighbor || filledNegDiags[i-j+7];
					if (horizontal && vertical && posDiag && negDiag) {
						stablePlayer[8*i + j] = true;
						improvement++;
					}
				}
				else if (board->get(otherSide, i, j) && !stableOpponent[8*i + j]) {
					bool horizontalBoundary = (i == 0) || (i == 7);
					bool verticalBoundary = (j == 0) || (j == 7);
					bool boundary = horizontalBoundary || verticalBoundary;
					
					bool horizontalNeighbor = horizontalBoundary || stableOpponent[8*(i-1) + j] || stableOpponent[8*(i+1) + j];
					bool verticalNeighbor = verticalBoundary || stableOpponent[8 * i + (j-1)] || stableOpponent[8 * i + (j+1)];
					bool posDiagNeighbor = boundary || stableOpponent[8*(i+1) + (j-1)] || stableOpponent[8*(i-1) + (j+1)];
					bool negDiagNeighbor = boundary || stableOpponent[8*(i-1) + (j-1)] || stableOpponent[8*(i+1) + (j+1)];
					
					bool horizontal = filledRows[i] || horizontalNeighbor;
					bool vertical = filledCols[j] || verticalNeighbor;
					bool posDiag = posDiagNeighbor || filledPosDiags[i+j];
					bool negDiag = negDiagNeighbor || filledNegDiags[i-j+7];
					if (horizontal && vertical && posDiag && negDiag) {
						stableOpponent[8*i + j] = true;
						improvement++;
					}
				}
			}
		}
	}
	
	int playerStableCount = 0;
	int opponentStableCount = 0;
	for (unsigned int i = 0; i < stablePlayer.size(); i++) {
		if (stablePlayer[i])
			playerStableCount++;
		if (stableOpponent[i])
			opponentStableCount++;
	}
	
	if (playerStableCount + opponentStableCount == 0)
		return 0;
	else 
		return (NORMALIZATION_CONSTANT * (playerStableCount - opponentStableCount)) / 64;
	
}
