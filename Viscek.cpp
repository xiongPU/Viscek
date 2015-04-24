//
//  Viscek.cpp
//  Viscek
//
//  Created by Jun Xiong on 4/18/15.
//
//

#include "Viscek.hpp"
#include <vector>
#include <chrono>
#include <cmath>
#include <random>
#include <iostream>
using namespace std;

//generate a worm randomly within the right range
// it seems a vector of worms will be generated with the same random
Worm::Worm(FLOAT v, FLOAT L, FLOAT L_left, FLOAT L_right, FLOAT L_up, FLOAT L_down):v0_(v), L_(L), L_left_(L_left), L_right_(L_right), L_up_(L_up), L_down_(L_down){
    mt19937 eng(chrono::high_resolution_clock::now()
                .time_since_epoch().count());
    const FLOAT Pi=4*atan(1);
    uniform_real_distribution<FLOAT> unif_x(L_left_, L_right_);
    uniform_real_distribution<FLOAT> unif_y(L_down_, L_up_);
    uniform_real_distribution<FLOAT> unif_theta(-Pi/2, 3*Pi/2);
    //the range of theta should be compatible to the func. that changes it
    x_=unif_x(eng);
    y_=unif_y(eng);
    theta_=unif_theta(eng);
}

Worm::~Worm(){
}

Viscek::Viscek(const FLOAT rho, FLOAT r, const FLOAT eta, const FLOAT v0, const INT time_total, const INT N):rho_(rho), r_(r), eta_(eta), v0_(v0), time_total_(time_total), N_(N), L_(sqrt(N/rho)){
    L_left_=-L_/2.0, L_right_=L_/2.0, L_up_=L_/2.0, L_down_=-L_/2.0;
    Worm *worm_pt;
    for(INT i=0; i<N_; i++){
        worm_pt = new Worm(v0,L_, L_left_, L_right_, L_up_, L_down_);
        worm_.push_back(worm_pt);
    }
    
    /*cout<<N_<<" "<<L_<<endl;
    for(INT i=0; i<N_; i++){
        cout<<i<<" "<<worm_[i]->x_<<" "<<worm_[i]->y_<<" "<<worm_[i]->theta_*180/3.14159<<endl;
 //   cout<<worm_[N_-1]->x_<<" "<<worm_[N_-1]->y_<<endl;
    }
    cout<<worm_.size()<<endl;*/
//    cout<<"L left right up down and r"<<endl;
 //   cout<<L_left_<<" "<<L_right_<<" "<<L_up_<<" "<<L_down_<<" "<<r_<<endl;
}

Viscek::~Viscek(){
    for(INT i=0; i<(INT)worm_.size(); i++){
        delete worm_[i];
    }
}

FLOAT Viscek::getVa(){
    FLOAT sum_vx=0, sum_vy=0;
    for(int i=0;i<(INT)worm_.size();i++){
        sum_vx+=v0_*cos(worm_[i]->theta_);
        sum_vy+=v0_*sin(worm_[i]->theta_);
    }
    return sqrt(sum_vx*sum_vx+sum_vy*sum_vy)/(N_*v0_);
}

/*
FLOAT Viscek::meanAllTheta(){
    FLOAT sum_cos=0, sum_sin=0, mean_theta;
    const FLOAT Pi=4*atan(1);
    for(int i=0; i<(int)worm_.size();i++){
        sum_cos+=cos(worm_[i]->theta_);
        sum_sin+=sin(worm_[i]->theta_);
    }
    mean_theta=atan(sum_sin/sum_cos);
    if(sum_cos<0) mean_theta+=Pi;
    return mean_theta;
}*/

FLOAT Viscek::meanAllTheta(){
    FLOAT sum_theta=0;
    for(int i=0; i<(INT)worm_.size();i++){
        sum_theta+=worm_[i]->theta_;
    }
    return sum_theta/worm_.size();
}

FLOAT Viscek::stdAllTheta(){
    FLOAT sum=0, mean=meanAllTheta();
    for(int i=0; i<(int)worm_.size();i++){
        sum+=(worm_[i]->theta_-mean)*(worm_[i]->theta_-mean);
    }
    return sum/(N_-1);
}

FLOAT Viscek::thetaMeanNeighbor(INT center){
    FLOAT mean_theta, sum_cos=0, sum_sin=0;
    const FLOAT Pi=4*atan(1);
    vector<INT> neighbor;  //record the index of the neighbors
    findNeighbor(center, neighbor);
    auto n_neighbor=neighbor.size();
    mt19937 eng(chrono::high_resolution_clock::now()
                .time_since_epoch().count()+1);
    uniform_real_distribution<FLOAT> unif(-eta_/2, eta_/2);
    auto theta_noise=unif(eng);
    for(int i =0; i<(int)n_neighbor; i++){
  //      theta_noise=unif(eng);
        sum_cos+=cos(worm_[neighbor[i]]->theta_);
        sum_sin+=sin(worm_[neighbor[i]]->theta_);
    }
    mean_theta=atan(sum_sin/sum_cos);
    if(sum_cos<0) mean_theta+=Pi;
    return mean_theta+theta_noise;
}

void Viscek::findNeighbor(INT center, vector<INT> & neighbor){
    neighbor.resize(1, center);
    neighbor[0]=center;
    FLOAT dist, x0=worm_[center]->x_, y0=worm_[center]->y_;
    INT j=(INT)worm_.size();
    if(x0>L_left_+r_ && x0<L_right_-r_ && y0>L_down_+r_ && y0<L_up_-r_){
        for(j=0;j<(INT)worm_.size();j++){
            dist=sqrt((worm_[j]->x_-x0)*(worm_[j]->x_-x0)+(worm_[j]->y_-y0)*(worm_[j]->y_-y0));
            if(dist<=r_ && j!=center){
                neighbor.push_back(j);
            }
        }
    }
    else findEdgeNeighbor(center, neighbor);
   /*  cout<<i<<endl;
    for(INT k; k<(INT)neighbor.size();k++){
        cout<<" "<<neighbor[k];
    }
    cout<<endl;*/
}

void Viscek::findEdgeNeighbor(INT center, vector<INT> & neighbor){
    FLOAT dist, x, y, x0=worm_[center]->x_, y0=worm_[center]->y_;
    for(int j=0;j<(INT)worm_.size();j++){
        if(j!=center){
            x=worm_[j]->x_;
            y=worm_[j]->y_;
            dist=sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0));
            if(dist<=r_ && j!=center){
                neighbor.push_back(j);
                continue;
            }
            if(x0<L_left_+r_){
//                dist=sqrt((x-L_-x0)*(x-L_-x0)+(y-y0)*(y-y0));
//                if(dist<=r_) neighbor.push_back(j);
//                if(
                if(x>L_right_-r_){
                    if(latticeDist(x0, y0, x, y, 1, 0)<r_){  // judge x-L-x0
                        neighbor.push_back(j);
                    }
                    else if(y0>L_up_-r_ && y<L_down_+r_ && latticeDist(x0, y0, x, y, 1, -1)<r_){
                        neighbor.push_back(j);
                    }
                    else if(y0<L_down_+r_ && y>L_up_-r_ && latticeDist(x0, y0, x, y, 1, 1)<r_){
                        neighbor.push_back(j);
                    }
                }
            }
            else if(x0>L_right_-r_){
//                if(x<L_left+r_ && latticeDist(x0, y0, x, y, -1, 0)){
//                    neighbor.push_back(j);
//                }
                if(x<L_left_+r_){
                    if(latticeDist(x0, y0, x, y, -1, 0)<r_){  // judge x-L-x0
                        neighbor.push_back(j);
                    }
                    else if(y0>L_up_-r_ && y<L_down_+r_ && latticeDist(x0, y0, x, y, -1, -1)<r_){
                        neighbor.push_back(j);
                    }
                    else if(y0<L_down_+r_ && y>L_up_-r_ && latticeDist(x0, y0, x, y, -1, 1)<r_){
                        neighbor.push_back(j);
                    }
                }
            }
            if(y0<L_down_+r_){
                if(y>L_up_-r_ && latticeDist(x0, y0, x, y, 0, 1)<r_){
                    neighbor.push_back(j);
                }
            }
            else if(y0>L_up_-r_){
                if(y<L_down_+r_ && latticeDist(x0, y0, x, y, 0, -1)<r_){
                    neighbor.push_back(j);
                }
            }
        }
    }
}

FLOAT Viscek::latticeDist(FLOAT x0, FLOAT y0, FLOAT x, FLOAT y, int a, int b){
    FLOAT dx, dy;
    dx=x-a*L_-x0;
    dy=y-b*L_-y0;
    return sqrt(dx*dx+dy*dy);
}

void Viscek::step(){
    vector<FLOAT> theta_new(worm_.size());
    for(int i=0; i<(int)worm_.size(); i++){
        theta_new[i]=thetaMeanNeighbor(i);
   //     cout<<worm_[i]->theta_<<endl;
    }
    for(int i=0; i<(int)worm_.size(); i++){
        worm_[i]->theta_=theta_new[i];
    }
    
    for(int i=0; i<(int)worm_.size(); i++){
        worm_[i]->x_+=v0_*cos(worm_[i]->theta_);
        worm_[i]->y_+=v0_*sin(worm_[i]->theta_);
        if(worm_[i]->x_<L_left_) worm_[i]->x_+=L_;
        else if(worm_[i]->x_ > L_right_) worm_[i]->x_-=L_;
        if(worm_[i]->y_<L_down_) worm_[i]->y_+=L_;
        else if(worm_[i]->y_>L_up_) worm_[i]->y_-=L_;
    }
}

void Viscek::oneSimulation(){
    for(INT i=0; i<time_total_; i++){
        step();
        va_=getVa();
        if(i>80){
            cout<<"time    theta mean    std theta    va"<<endl;
            cout<<i<<" "<<meanAllTheta()<<" "<<stdAllTheta()<<" "<<va_<<" "<<v0_<<endl;
        }
    }
}

void Viscek::reInitialize(){
    mt19937 eng(chrono::high_resolution_clock::now()
                .time_since_epoch().count()+1);
    const FLOAT Pi=4*atan(1);
    uniform_real_distribution<FLOAT> unif_x(L_left_, L_right_);
    uniform_real_distribution<FLOAT> unif_y(L_down_, L_up_);
    uniform_real_distribution<FLOAT> unif_theta(0, 2*Pi);
    auto i=worm_.size();
    for(i=0; i<worm_.size(); i++){
        worm_[i]->x_=unif_x(eng);
        worm_[i]->y_=unif_y(eng);
        worm_[i]->theta_=unif_theta(eng);
    }
}









































