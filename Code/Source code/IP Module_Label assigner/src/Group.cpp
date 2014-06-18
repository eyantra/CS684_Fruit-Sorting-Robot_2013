/*
 * Group.cpp
 *
 *  Created on: 30-Oct-2013
 *      Author: prpankajsingh
 */
#include <iostream>

#include "Group.h"
using namespace std;

Group::Group():myAttribute(-1,0){
	id=0;
	currentCount=0;
	isTrained=false;
}

Group::Group(int id):myAttribute(-1,0) {
	id=id;
	currentCount=0;
	isTrained=false;
}

Group::Group(int i,FruitAttribute f):myAttribute(f.hueVal,f.area){
	id=i;
	currentCount=0;
	isTrained=false;
}

void Group::assignIt(int num){
	if(num>=0)
	{
		currentCount=currentCount+num;
	}
	else
	{
		cout<<"wrong count(i.e. count is negative) is assigned to Group";
	}
}


void Group::setMyAttribute(FruitAttribute fa){
	myAttribute.hueVal=fa.hueVal;
	myAttribute.area=fa.area;
}

void Group::trainMe(FruitAttribute fa){
	this->setMyAttribute(fa);
	this->setIsTrained(true);
}

FruitAttribute Group::getMyAttribute() const
{
    return myAttribute;
}

int Group::getCurrentCount() const
{
    return currentCount;
}


int Group::getId() const
{
    return id;
}

bool Group::getIsTrained() const
{
    return isTrained;
}


void Group::setCurrentCount(int currentCount)
{
    this->currentCount = currentCount;
}


void Group::setId(int id)
{
    this->id = id;
}

void Group::setIsTrained(bool isTrained)
{
    this->isTrained = isTrained;
}

