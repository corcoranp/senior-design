/*
 * BlueVision.h
 *
 *  Created on: Mar 17, 2016
 *      Author: Tobie
 */

#ifndef BLUEVISION_H_
#define BLUEVISION_H_
#include "../BasicFunctions.h"
#include "BlueBlock.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//Color threshold values
#define iLowH 75
#define iHighH 130
#define iLowS 0
#define iHighS 255
#define iLowV 0
#define iHighV 255
//Block Area restriction
#define BLOCK_AREA 1000

#ifndef ContourBlock
typedef struct ContourBlock_t {
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
} ContourBlock;
#endif

#ifdef __cplusplus
extern "C" {
#endif

vector<BlueBlock> getBlueBlocks(Mat image);

Mat convertImageToHSV(Mat image);

Mat thresholdHSVImage(Mat image, Scalar low, Scalar high);

ContourBlock getContours(Mat image);

#ifdef __cplusplus
}
#endif

#endif /* BLUEVISION_H_ */
