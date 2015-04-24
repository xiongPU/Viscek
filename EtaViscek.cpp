//
//  EtaViscek.cpp
//  Plot Va v.s. Eta
//
//  Created by Jun Xiong on 4/23/15.
//
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Viscek.hpp"
#include "Measurement.hpp"
using namespace std;

int main(){
    ofstream fout("Viscek_Data/Va_eta_t5E2_N100.txt");
    const INT t_total=500, t_cal=100, N=100;
    const FLOAT rho=4, r=1, v0=0.03, d_eta=0.1;
  //  const FLOAT Pi=4*atan(1);
  //  INT t_warm;
 //   FLOAT eta_mean, eta_std;
    vector<FLOAT> eta, va;
  //  Viscek *V1;
    for(INT i=0; i<3; i++){
        eta.push_back(i*d_eta);
    }
    vaEta(rho, r, v0, t_total, N, t_cal, eta, va);
    for(INT i=0; i<(INT)va.size(); i++){
        fout<<eta[i]<<" "<<va[i]<<endl;
    }
    fout.close();
    //V1.oneSimulation();
   /* eta_mean=V1.meanAllTheta();
    eta_std=V1.stdAllTheta();
    va=V1.getVa();
    cout<<"eta_mean   eta_std   va  getVa"<<endl;
    cout<<eta_mean<<"   "<<eta_std<<"   "<<V1.va_<<"  "<<va<<" "<<V1.N_<<" "<<V1.v0_<<" "<<Pi<<endl;*/
    return 0;
}


