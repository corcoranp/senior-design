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

const Point BlueBlock::getCenter() {
	return center;
}

const Point2D BlueBlock::getCenterDst() {
	return centerDst;
}

Rect BlueBlock::getRectangle() {
	return rectangle;
}

void BlueBlock::setRectangle(Rect rectangle) {
	this->rectangle = rectangle;

	this->center.x = this->rectangle.boundingRect().x
					+ (this->rectangle.boundingRect().width / 2);
	this->center.y = this->rectangle.boundingRect().y
					+ (this->rectangle.boundingRect().height / 2);
	this->area = this->rectangle.size.area();
}

