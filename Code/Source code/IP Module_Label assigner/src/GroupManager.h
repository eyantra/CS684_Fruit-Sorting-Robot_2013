/*
 * GroupManager.h
 *
 *  Created on: 30-Oct-2013
 *      Author: prpankajsingh
 */

#ifndef GROUPMANAGER_H_
#define GROUPMANAGER_H_
#include "Group.h"
#include "constants.h"
#include "FruitAttribute.h"
class GroupManager {
	//group with id=0 will not be used anywhere........
	//So group count will count the relevant group only
	Group group[(MAXNUMBEROFGROUPS+1)];
	bool isTrained[(MAXNUMBEROFGROUPS+1)];
	//Maximum Value is 10 and it denotes active numbers only
	int activeGroupCount;

public:
	int totalFruitCount;
	GroupManager(int activeGroup);

	bool isGroupTrained(int id);
	void TrainGroup(int groupid,FruitAttribute f);
	int getActiveGroupCount() const;
    bool getIsTrained() const;
    int getTotalFruitCount() const;
	/**
	 * calculate distance between two given attribute object using alpha as a weight and beta
	 * this function is not general
	 * hence if attribute increases need to modify this function
	 * alpha is 0 for hue only and 1 for area only.......
	 * Beta is scaling factor for area.....
	 */
	int calcDistant(FruitAttribute f1,FruitAttribute f2,double alpha,double beta);
	// return closest active group number from this property.
	int AssignGroup(FruitAttribute f,double alpha,double beta);
	//returns the percentage value
	int calculateConfidence(FruitAttribute f,double alpha,double beta);
};

#endif /* GROUPMANAGER_H_ */
