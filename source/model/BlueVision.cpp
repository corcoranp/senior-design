#include "../../include/model/BlueVision.h"

bool sortBlocks(BlueBlock a, BlueBlock b){
	return a.getCenter().y < b.getCenter().y;
}

vector<BlueBlock> getBlueBlocks(Mat image) {
	vector<BlueBlock> blocks;

	Mat hsv = convertImageToHSV(image);
	Scalar low(iLowH, iLowS, iLowV);
	Scalar high(iHighH, iHighS, iHighV);
	Mat thres = thresholdHSVImage(hsv, low, high);
	ContourBlock blk = getContours(thres);

	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contours_poly(blk.contours.size());
	vector<Rect> boundRect(blk.contours.size());

	//Extract rectangles of a large enough area from the image.
	for (int i = 0; i < ((int)blk.contours.size()); i++) {
		approxPolyDP(Mat(blk.contours[i]), contours_poly[i], 3, true);
		Rect rect = boundingRect(Mat(contours_poly[i]));
		double area = contourArea(contours_poly[i]);
		if(area > BLOCK_AREA){
			boundRect[i] = rect;
			//check if rect is two blocks stacked on top of each other
			//it is 2 blocks if the height of the rect is at least 1.5
			//times larger then the width of the rect.
			if(rect.height > (rect.width*1.5)){
				//split rect into two recs
				BlueBlock b1, b2;
				b1.setRectangle(rect);
				rect.y += rect.height;
				b2.setRectangle(rect);
				blocks.push_back(b1);
				blocks.push_back(b2);
			} else {
				BlueBlock b1;
				b1.setRectangle(rect);
				blocks.push_back(b1);
			}
		}
	}
	sort(blocks.begin(), blocks.end(), sortBlocks);
	return blocks;
}

Mat convertImageToHSV(Mat image){
	Mat imgHSV;
	cvtColor(image, imgHSV, COLOR_BGR2HSV);
	return imgHSV;
}

Mat thresholdHSVImage(Mat image, Scalar low, Scalar high){
	Mat imgThres;
	inRange(image, low, high, imgThres);
	   //morphological closing (fill small holes in the foreground)
	  dilate( imgThres, imgThres, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	  erode(imgThres, imgThres, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

		//morphological opening (remove small objects from the foreground)
		  erode(imgThres, imgThres, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		  dilate( imgThres, imgThres, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	  return imgThres;
}

ContourBlock getContours(Mat image){
	ContourBlock ctBlk;

	//Find contours
	findContours( image, ctBlk.contours, ctBlk.hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	return ctBlk;
}
