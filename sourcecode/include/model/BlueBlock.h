/*
 * BlueBlock.h
 *
 *  Created on: Apr 4, 2016
 *      Author: Tobie
 */

#ifndef BLUEBLOCK_H_
#define BLUEBLOCK_H_
#include "../BasicFunctions.h"
#include <opencv2/opencv.hpp>

using namespace cv;

class BlueBlock {
public:
	BlueBlock();
	virtual ~BlueBlock();

	double getArea();

	const Point2D getCenter();

	const Point2D getCenterDst();

	Rect getRectangle();

	void setRectangle(Rect rectangle);

private:
	Rect rectangle;
	Point2D center;
	Point2D centerDst;
	double area;

};

#endif /* BLUEBLOCK_H_ */
