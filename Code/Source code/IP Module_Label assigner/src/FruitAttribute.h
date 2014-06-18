/*
 * FruitAttribute.h
 * This class contains the attributes of fruits
 *  Created on: 30-Oct-2013
 *      Author: prpankajsingh
 */

#ifndef FRUITATTRIBUTE_H_
#define FRUITATTRIBUTE_H_

class FruitAttribute {
public:
	int hueVal;
	int area;
	FruitAttribute();
	FruitAttribute (const FruitAttribute &f);
	FruitAttribute(int hue,int area);
};

#endif /* FRUITATTRIBUTE_H_ */
