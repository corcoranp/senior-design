/*
 * ImagingController.h
 *
 *  Created on: Oct 4, 2015
 *      Author: Peter
 */

#ifndef INCLUDE_CONTROLLERS_IMAGINGCONTROLLER_H_
#define INCLUDE_CONTROLLERS_IMAGINGCONTROLLER_H_

#include "../enums.h"
#include "../model/qrcode.h"
#include "../model/QRCodeFinder.h"
#include "../model/BlueVision.h"
#include "../model/Camera.h"

#include <opencv2/opencv.hpp>

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
		ImagingController(int cameraIndex=0);
		ImagingController(ImagingController& orig);
		virtual ~ImagingController();

		vector<QRCode> readQRCodes() throw (string);

		vector<BlueBlocks> readBlueBlocks() throw (string);

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
		Camera* camera;

	};

}
#endif /* INCLUDE_CONTROLLERS_IMAGINGCONTROLLER_H_ */
