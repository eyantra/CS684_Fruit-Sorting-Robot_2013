/*
 * GroupManager.cpp
 *
 *  Created on: 30-Oct-2013
 *      Author: prpankajsingh
 */

#include "GroupManager.h"
#include <cmath>
#include <iostream>

int GroupManager::getActiveGroupCount() const
{
    return activeGroupCount;
}

bool GroupManager::getIsTrained() const
{
    return isTrained;
}

int GroupManager::getTotalFruitCount() const
{
    return totalFruitCount;
}

 GroupManager::GroupManager(int activeGroup) {
	this->totalFruitCount=0;
	this->activeGroupCount =activeGroup;
	for(int i=1;i<=activeGroupCount;i++){
		isTrained[i]=false;
		this->group[i].setCurrentCount(0);
		this->group[i].setId(i);
		this->group[i].setMyAttribute(FruitAttribute(-1,0));
		this->group[i].setIsTrained(false);
	}

}

void GroupManager::TrainGroup(int groupid, FruitAttribute f)
{
	this->group[groupid].trainMe(f);
	this->isTrained[groupid]=true;
}

bool GroupManager::isGroupTrained(int groupid)
{
	return this->group[groupid].getIsTrained();
}

int GroupManager::AssignGroup(FruitAttribute f,double alpha,double beta)
{
	int assignedGroupId=1;
	for(int i=1;i<=(this->activeGroupCount);i++){
		if(calcDistant(f,this->group[i].getMyAttribute(),alpha,beta)<calcDistant(f,this->group[assignedGroupId].getMyAttribute(),alpha,beta)){
			assignedGroupId=i;
		}
	}
	this->group[assignedGroupId].assignIt(1);
	this->totalFruitCount++;
	return assignedGroupId;
}


//returns in percentage value
int GroupManager::calculateConfidence(FruitAttribute f,double alpha,double beta)
{
	int confidence=0;
	std::cout<<"Confidence calculation is pending...........";
	return confidence;
}

int GroupManager::calcDistant(FruitAttribute f1, FruitAttribute f2, double alpha, double beta)
{
	double dist=0;

	double hue_distsqr=pow((f1.hueVal-f2.hueVal),2);
	double areasqr=pow((f1.area-f2.area),2);
	dist=((1-alpha)*hue_distsqr)+(alpha*beta*areasqr);
	dist=sqrt(dist);
	return dist;
}



