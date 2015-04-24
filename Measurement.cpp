//
//  Measurement.cpp
//  
//
//  Created by Jun Xiong on 4/23/15.
//
//

#include "Measurement.hpp"
#include "Viscek.hpp"
#include <vector>
using namespace std;

void vaEta(FLOAT rho, FLOAT r, FLOAT v0, INT t_total, INT N, INT t_cal, const vector<FLOAT> &eta, vector<FLOAT> &va){
    Viscek *V1;
    FLOAT va_sum=0, t_warm;
    va.resize(eta.size());
    for(INT j=0; j<(INT)eta.size(); j++){
        va_sum=0;
        V1=new Viscek(rho, r, eta[j], v0, t_total, N);
        t_warm=t_total-t_cal;
        for(INT i=0; i<t_warm; i++){
            V1->step();
        }//get equilibrium first
        for(INT i=0; i<t_cal; i++){
            V1->step();
            va_sum+=V1->getVa();
        }
        va[j]=va_sum/t_cal;
        delete V1;
    }
    
}