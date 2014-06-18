/*
 * Group.h
 *
 *  Created on: 30-Oct-2013
 *      Author: prpankajsingh
 */

#ifndef GROUP_H_
#define GROUP_H_
#include "FruitAttribute.h"
class Group {
	FruitAttribute myAttribute;
	int id;
	int currentCount;
	bool isTrained;

public:
	Group();
	Group(int id);
	Group(int id,FruitAttribute a);
    int getCurrentCount() const;
    int getId() const;
    FruitAttribute getMyAttribute() const;
    bool getIsTrained() const;
    void setCurrentCount(int currentCount);
    void setId(int id);
    //increment count by num
    void assignIt(int num=1);
    //this will set the attribute and set the isTrain
    void trainMe(FruitAttribute fa);
    //never use this function to train group...rather use "trainme" function
    void setMyAttribute(FruitAttribute fa);
    void setIsTrained(bool isTrained);
};

#endif /* GROUP_H_ */
