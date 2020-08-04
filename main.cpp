//
//  main.cpp
//  A Simple Balanced Random Poisson Spiking Network
//
//  Created by Mahdi Heidarpoor on 10/2/19.
//  Copyright © 2019 Mahdi Heidarpoor. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <math.h>
#include <random>

using namespace std;

//    Global Network Variables
const double c=0.2;    //   likelyhood
const double ci=0.25;  //   Probability of being inhibitory
const int nsize=40;    //   Network Size
const int nssize=10;   //   Number of stimule Neurons
const int rtime=1000;  //   Runing time ms
const int stime=100;   //   Stimulate time ms
const double ew=1;     //   Excitory connection wight
const double iw=-1;    //   Inhibitory connection wight
const double tao=0.10;   //   Tao constant


int main(void)
{
    srand(time(0));
    
//    Random wights declaration
    double w[nsize][nsize];
    for(int i=0;i<nsize;i++)
        for(int j=0;j<nsize;j++)
        {
            double r1=(double)rand()/RAND_MAX;
            if (r1<c)
            {
                double r2=(double)rand()/RAND_MAX;
                if(r2<ci) w[i][j]=iw;
                else w[i][j]=ew;
            }
        }
    
 
//    Network defining
    double r[nsize][rtime];
    bool spike[nsize][rtime];
    double s[nsize][rtime];
    for (int i=0; i<nsize; i++)
        for(int t=0; t<rtime;t++)
        {
            r[i][t]=0;
            s[i][t]=0;
            spike[i][t]=0;
        }

//      Network Stimulate
    for(int t=0;t<stime;t++)
    {
//      Stimule Part
        for(int i=0;i<nssize;i++)
        {
            spike[i][t]=1;
        }
        for(int i=0;i<nsize;i++)
        {
            double spikesnumber=0;
            for(int k=0;k<=t;k++)
            { if(spike[i][k]==1) spikesnumber=spikesnumber + exp(k-t);}
//     Synaptic activation
            s[i][t]=exp((-1/tao)*t)+tao*spikesnumber;
        }
//     Others
        for(int i=nssize; i<nsize;i++)
        {
            double temp=0;
            for(int j=0; j<nsize; j++)
            {
                temp=temp+w[i][j]*s[j][t-1];
            }
//     Firerate
            r[i][t-1]=(1/tao)*0.4*(1+tanh(0.4*temp-3));
            double temp2=1-exp(-r[i][t-1]);
            double r=(double)rand()/RAND_MAX;
//     Spike
            if(r<temp2) spike[i][t]=1;
            double spikesnumber=0;
            for(int k=0;k<=t;k++)
            { if(spike[i][k]==1) spikesnumber=spikesnumber+exp(k-t);}
//    Synaptic activation
            s[i][t]=exp((-1/tao)*t)+tao*spikesnumber;
        }
    }
    
//     Network Run
    for(int t=stime;t<rtime;t++) //time
    {
        if(t==0)t++;
        for(int i=0; i<nsize;i++)
        {
            double temp=0;
            for(int j=0; j<nsize; j++)
            {
                temp=temp+w[i][j]*s[j][t-1];
            }
//     Firerate
            r[i][t-1]=0.1*0.4*(1+tanh(0.4*temp-3));
            double temp2=r[i][t-1]*exp(-r[i][t-1]);
            double r=(double)rand()/RAND_MAX;
//     Spike
            if(r<temp2) spike[i][t]=1;
            int spikesnumber=0;
            for(int k=0;k<=t;k++)
                { if(spike[i][k]==1) spikesnumber+=1;}
//    Synaptic activation Supposed tao=10
            s[i][t]=exp((-1/tao)*t)+tao*spikesnumber;
        }
    }
    
//    Plot Spikes
    for (int i=0; i<nsize; i+=1)
    {
        for(int t=0; t<rtime;t+=5)
        {
            if(spike[i][t]==0) cout<<" ";
            else
            {
                cout<<".";
            }
        }
        cout<<"\n";
    }
    
//    create text file for output
    
    fstream myfile;
    myfile.open ("raster_plot.txt");
    for (int i=0; i<nsize; i+=1)
     {
         for(int t=0; t<rtime;t+=5)
         {
             if(spike[i][t]==0) myfile<<" ";
             else
             {
                 myfile<<".";
             }
         }
         myfile<<"\n";
     }
    myfile.close();
    
//  Graphics

    return 0;
}

//  End
