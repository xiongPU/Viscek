//
//  NeighborTest.cpp
//  Test whether the right neighbors are found
//
//  Created by Jun Xiong on 4/22/15.
//
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Viscek.hpp"
using namespace std;

int main(){
    ofstream fout("Viscek_Data/Neighbor1.txt",ios::out);
    const INT t_total=100, row=5, N=row*row;
    const FLOAT L=5, L2=L*L, rho=row*row/L2, r=1.5, eta=1, v0=0.03, dx=1;
    INT pt;
    FLOAT eta_mean, eta_std, va, x_mean,y_mean;
    Viscek V1(rho, r, eta, v0, t_total, N);
    vector<INT> neighbor;
    for(INT i=0; i<row; i++){
        for(INT j=0; j<row; j++){
            V1.worm_[i*row+j]->x_=dx*j-L/2;
            V1.worm_[i*row+j]->y_=dx*i-L/2;
        }
    }
    for(INT i=0; i<L; i++){
        for(INT j=0; j<L; j++){
            pt=i*L+j;
            V1.findNeighbor(pt, neighbor);
            fout<<V1.worm_[pt]->x_<<" "<<V1.worm_[pt]->y_<<" "<<neighbor.size()<<" ";
            x_mean=0;
            y_mean=0;
            cout<<"Neighbors of: ";
            cout<<V1.worm_[pt]->x_<<" "<<V1.worm_[pt]->y_<<endl;
            for(INT nei=0; nei<neighbor.size(); nei++){
                x_mean+=V1.worm_[neighbor[nei]]->x_;
                y_mean+=V1.worm_[neighbor[nei]]->y_;
                cout<<pt<<" "<<neighbor[0]<<" "<<V1.worm_[neighbor[nei]]->x_<<" "<<V1.worm_[neighbor[nei]]->y_<<endl;
            }
            fout<<x_mean/neighbor.size()<<" "<<y_mean/neighbor.size()<<endl;
        }
    }
    fout.close();
    return 0;
}
