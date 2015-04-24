//
//  ViscekSimulate.cpp
//  Viscek
//
//  Created by Jun Xiong on 4/19/15.
//
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Viscek.hpp"
using namespace std;

int main(){
    ofstream fo1("Viscek_Data/Va_Time1E3_N4E3_rho4_eta3.txt",ios::out);
    const INT t_total=1000, N=4000;
    const FLOAT rho=4, r=1, eta=3, v0=0.03;
    const FLOAT Pi=4*atan(1);
    FLOAT eta_mean, eta_std, va;
    Viscek V1(rho, r, eta, v0, t_total, N);
    for(INT i=0; i<(INT)V1.time_total_; i++){
        V1.step();
        V1.va_=V1.getVa();
        fo1<<i<<" "<<V1.meanAllTheta()<<" "<<V1.stdAllTheta()<<" "<<V1.va_<<" "<<endl;
    }
    fo1.close();
    //V1.oneSimulation();
    eta_mean=V1.meanAllTheta();
    eta_std=V1.stdAllTheta();
    va=V1.getVa();
    cout<<"eta_mean   eta_std   va  getVa"<<endl;
    cout<<eta_mean<<"   "<<eta_std<<"   "<<V1.va_<<"  "<<va<<" "<<V1.N_<<" "<<V1.v0_<<" "<<Pi<<endl;
    return 0;
}
