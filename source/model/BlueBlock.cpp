/*
 * BlueBlock.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: Tobie
 */

#include "../../include/model/BlueBlock.h"

BlueBlock::BlueBlock() {
	this->rectangle = Rect(0,0,0,0);
	this->center.x = 0;
	this->center.y = 0;
	this->area = 0;
}

BlueBlock::~BlueBlock() {
	//Do nothing
}

double BlueBlock::getArea() {
	return area;
}

Point2D BlueBlock::getCenter() {
	return center;
}

Point2D BlueBlock::getCenterDst() {
	return centerDst;
}

Rect BlueBlock::getRectangle() {
	return rectangle;
}

void BlueBlock::setRectangle(Rect rectangle) {
	this->rectangle = rectangle;

	this->center.x = this->rectangle.x
					+ (this->rectangle.width / 2);
	this->center.y = this->rectangle.y
					+ (this->rectangle.height / 2);
	this->area = this->rectangle.area();
}

