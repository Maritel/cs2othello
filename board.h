#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.h"
#include <vector>
using namespace std;

class Board {
	
	friend class Player; //more convenient this way
   
private:
    bitset<64> black;
    bitset<64> taken;    
       
    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);
      
public:
    Board();
    ~Board();
    Board *copy();
        
    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();

    void setBoard(char data[]);
    
    /*
     * EXTRA METHODS
     */   
    
    // unsafe move methods
    void doMoveUnchecked(Move *m, Side side);
    void undoMove(Move *m);
    
    // helper methods
    int countEmpty();
};

#endif
