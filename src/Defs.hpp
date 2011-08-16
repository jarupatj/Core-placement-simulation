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
#define ITER   200
#define REJECT 50

//operation for update cost
#define REMOVE	0
#define ADD	1

struct Coordinate {
   int x;
   int y;
};

#endif
