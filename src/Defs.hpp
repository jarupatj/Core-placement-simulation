#ifndef DEFS_HPP
#define DEFS_HPP

#define NO_ERR             0
#define FILE_OPEN_ERR      1
#define ILLEGAL_STATE_ERR  2

//Default value
#define ALPHA  1
#define BETA   1
#define GAMMA  0.2
#define DELTA  0.04
#define S_TEMP 1000
#define E_TEMP 0.1
#define RATE   0.9
#define ITER   400
#define REJECT 200 
#define ACCEPT 100 

//operation for update cost
#define REMOVE	0
#define ADD	1

struct Coordinate {
   int x;
   int y;
};

#define MAX_TURNS 8
#define NO_CORE -1

enum Turn {
   TOP_BOTTOM,
   BOTTOM_TOP,
   LEFT_RIGHT,
   RIGHT_LEFT,
   LEFT_BOTTOM,
   LEFT_TOP,
   RIGHT_BOTTOM,
   RIGHT_TOP
};

enum Direction {
   NO_DIR = -1,
   TOP,
   BOTTOM,
   LEFT,
   RIGHT
};

#endif
