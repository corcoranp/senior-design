/*
 * ImagingController.h
 *
 *  Created on: Oct 4, 2015
 *      Author: Peter
 */

#ifndef INCLUDE_CONTROLLERS_IMAGINGCONTROLLER_H_
#define INCLUDE_CONTROLLERS_IMAGINGCONTROLLER_H_

#include "enums.h"
#include "../model/qrcode.h"

#include <cv.h>
#include <opencv.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>

#include <vector>       // std::vector
#include <iostream>

using namespace cv;
using namespace std;

namespace blaze {
	/*
	 * The Image Controller is responsible for interacting with the
	 * camera, and processing images.
	 */
	class ImagingController {
	public:
		/*
		 * Global Image Mat definitions
		 */
		cv::Mat src_img;		//main source image
		Mat src_thresh;		//threshold modified image


		/*
		 * Contours & Hierarchy
		 *
		 */
			vector<vector<Point> > s_cntrs;		//source image contour global ref
			vector<Vec4i> s_hrchy; 				//source image hierarchy



		/*
		 * Fuctions used for Image Processing...
		 */
		Color getBlockColor(Mat image);
		Size getBlockSize(Mat image);
		//qrcode getQRCode(Mat image);
		void getBlockPosition(Mat image);

		void identifyRailColor(Mat image);

		//function used to enable/disable camera
		void enableCamera(bool state);

		//function used to enable/disable lights
		void enableLights(bool state);

		/*
		 * Functions used for Image Capture
		 */

		Mat takeImage();

		void reset();
	private:

	};

}
#endif /* INCLUDE_CONTROLLERS_IMAGINGCONTROLLER_H_ */
