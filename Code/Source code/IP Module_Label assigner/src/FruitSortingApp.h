/*
 * FruitSortingApp.h
 *
 *  Created on: 31-Oct-2013
 *      Author: prpankajsingh
 */

#ifndef SUSH_H_
#define SUSH_H_
#include "constants.h"
//#define MAXNUMBEROFGROUPS 5
#include "GroupManager.h"
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include "FSVision.h"
#include "mySig.h"


using namespace std;


class FruitSortingApp {
public:
	int COMPORT=0;
	FSVision lemonSortingVision;
	int num_of_groups;
	int hue[MAXNUMBEROFGROUPS],area[MAXNUMBEROFGROUPS];
	double alpha,beta;
	FruitSortingApp();
    ~FruitSortingApp();
	FruitSortingApp(int n);
	void run();
private:
    void loadProfile(string & prof_file_path, FILE *& prof_file);
    void createProfile(string & prof_file_path, FILE *& prof_file);
    /**
     * Send Init signal to Firebird
     * and wait for ACK signal to recieve for atmost timeout of 2 sec period
     * This function returns 0 on success(READY) else return appropriate signal value defined in mySig.h
     */
    int initFB();
    /**
     * Initialize IP module with camera number n and returns 0 for sucessful initialization
     * else return appropriate signal defined in mySig.h
     */
    int initIPModule(int n=1);
    /**
     * Send signal(defined in mySig.h) to Firebired and
     * return 0 for successful transmission else return 1
     * NOTE: add this signal to mySig.h header file
     */
    int sendSignalToFB(unsigned char signal);

    //This is a signal(defined in mySig.h) from FB
    unsigned char receiveSignalFromFB();

    //send groupNo to FB using protocol.....and return 0 for successful transmission
    int sendGroupInfoToFB(int groupNo);

};

#endif /* SUSH_H_ */


