/*
 * FruitAttribute.cpp
 *
 *  Created on: 30-Oct-2013
 *      Author: prpankajsingh
 */

#include "FruitAttribute.h"

FruitAttribute::FruitAttribute() {
	hueVal=-1;
	area=0;
}

FruitAttribute::FruitAttribute(int hue,int a) {
	hueVal=hue;
	area=a;
}
FruitAttribute::FruitAttribute(const FruitAttribute &obj){
	this->area=obj.area;
	this->hueVal=obj.hueVal;
}
