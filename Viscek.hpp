//
//  Viscek.h
//  Viscek
//
//  Created by Jun Xiong on 4/18/15.
//
//

#ifndef __Viscek__Viscek__
#define __Viscek__Viscek__

#include <stdio.h>
#include <vector>
using namespace std;
typedef double FLOAT;
typedef int INT;

//record position of one worm
class Worm{
public:
    const FLOAT v0_, L_, L_left_, L_right_, L_up_, L_down_;
  //  const INT L_;
    FLOAT x_, y_, theta_;
    Worm(FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT);
    ~Worm();
};

class Viscek{

public:
    const FLOAT rho_, eta_, v0_, L_;
    FLOAT va_, r_;  //r_ is the distance for interaction
    const INT time_total_, N_;
    INT t_;
    
    Viscek(const FLOAT rho, FLOAT r, const FLOAT eta, const FLOAT v0, const INT time_total, const INT N);
    ~Viscek();
    FLOAT getVa();
    FLOAT meanAllTheta();  //calculate mean theta for all worms
    FLOAT stdAllTheta();  //calculate std for all worms' theta
    void step();
    void oneSimulation();
    void reInitialize();
    vector<Worm *> worm_;
    void findNeighbor(INT i, vector<INT> & neighbor);  //find neighbors for worm[i]
    
private:
    FLOAT L_left_, L_right_, L_up_, L_down_;

    FLOAT thetaMeanNeighbor(INT);
    void findEdgeNeighbor(INT i, vector<INT> & neighbor);
    FLOAT latticeDist(FLOAT x0, FLOAT y0, FLOAT x, FLOAT y, int a, int b); //(a,b) = (x-x0, y-y0) on lattice
};

#endif /* defined(__Viscek__Viscek__) */
