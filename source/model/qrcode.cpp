/*
 * QRCode
 * By: Peter Corcoran
 * Copyright: © 2015
 *
 * Jun 25, 2015 - Initial Coding
 *
 */
#include <iostream>

#include "../../include/model/qrcode.h"

using namespace std;
using namespace blaze;

string QRCode::getColorStr() {
	switch (color) {
	case RED:
		return string("RED");
	case BLUE:
		return string("BLUE");
	case YELLOW:
		return string("YELLOW");
	case GREEN:
		return string("GREEN");
	default:
		return string("");
	}
}

double QRCode::getArea() {
	return area;
}

const Point2D QRCode::getCenter() {
	return center;
}

const Point2D QRCode::getCenterDst() {
	return centerDst;
}

void QRCode::setCenterDst(Point2D centerDst) {
	this->centerDst = centerDst;
}

Color QRCode::getColor() {
	return color;
}

void QRCode::setColor(Color color) {
	this->color = color;
}

Rect QRCode::getRectangle() {
	return rectangle;
}

void QRCode::setRectangle(Rect rectangle) {
	this->rectangle = rectangle;

	this->center.x = this->rectangle.x
					+ (this->rectangle.width / 2);
	this->center.y = this->rectangle.y
					+ (this->rectangle.height / 2);
	this->area = this->rectangle.area();
}

QRCode::QRCode() {
	this->rectangle = Rect(0,0,0,0);
	this->center.x = 0;
	this->center.y = 0;
	this->area = 0;
	this->color = Color::BLUE;
}

QRCode::QRCode(Rect rect, Color color) {
	this->color = color;
	this->center.x = this->rectangle.x
					+ (this->rectangle.width / 2);
	this->center.y = this->rectangle.y
					+ (this->rectangle.height / 2);
	this->area = this->rectangle.area();
}

QRCode::~QRCode() {
	//Do nothing
}
