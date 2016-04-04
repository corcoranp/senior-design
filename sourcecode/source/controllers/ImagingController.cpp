
#include "../../include/controllers/ImagingController.h"

using namespace blaze;


void ImagingController::reset(){
	int index = this->camera->getCameraProperties().index;
	this->camera->~Camera();
	this->camera = new Camera(index);
}

vector<BlueBlock> ImagingController::readBlueBlocks() throw (string) {
	vector<BlueBlock> list;
	Mat src, scaleMatrix, cm;

	//Get image from camera
	src = captureImage();
	list = getBlueBlocks(src);
	if (!list.empty()) {
		for (unsigned int i = 0; i < list.size(); i++) {
			//Calculate blocks coordinates in relation to arm base.
			list[i].setCenterDst(getObjectCenterDistance(list[i].getCenter()));
			rectangle(src, list[i].getRectangle().tl(), list[i].getRectangle().br(),
					Scalar(0, 255, 0), 2, 8, 0);
			std::ostringstream text;
			text << "<" << list[i].getCenter().x << ", " << list[i].getCenter().y << ">";
			putText(src, text.str().c_str(), list[i].getRectangle().tl(),
					FONT_HERSHEY_PLAIN, 0.75, Scalar::all(255), 1, 8);
		}
	}
	imwrite("./BlueBlocks.jpg", src);

	return list;
}

vector<QRCode> ImagingController::readQRCodes() throw (string) {
	vector<QRCode> list;
	Mat src;

	//Capture image from camera
	src = captureImage();

	list = getQRCodes(src);
	if (!list.empty()) {
		for (unsigned int i = 0; i < list.size(); i++) {
			//Calculate blocks coordinates in relation to arm base.
			list[i].setCenterDst(getObjectCenterDistance(list[i].getCenter()));
			rectangle(src, list[i].getRectangle().tl(), list[i].getRectangle().br(),
					Scalar(0, 255, 0), 2, 8, 0);
			std::ostringstream text;
			switch (list[i].getColor()) {
			case COLOR::RED:
				text << "RED ";
				break;
			case COLOR::BLUE:
				text << "BLUE ";
				break;
			case COLOR::YELLOW:
				text << "YELLOW ";
				break;
			case COLOR::GREEN:
				text << "GREEN ";
				break;
			}
			text << "<" << list[i].getCenter().x << ", " << list[i].getCenter().y << ">";
			putText(src, text.str().c_str(), list[i].getRectangle().tl(),
					FONT_HERSHEY_PLAIN, 0.75, Scalar::all(255), 1, 8);
		}
	}

	imwrite("QR_Code image.jpg", src);
	return list;
}

ImagingController::ImagingController(int cameraIndex) {
	camera = new Camera(cameraIndex);
}

ImagingController::ImagingController(ImagingController& orig) {
	this->camera = orig.camera;
}

ImagingController::~ImagingController() {
	this->camera->~Camera();
}

void ImagingController::identifyRailColor(Mat image) {
	//FIXME: add code to ID rail cars
}

void ImagingController::enableCamera(bool state) {
	//FIXME: add code to enable/Disable camera vision
}

void ImagingController::enableLights(bool state) {
	//FIXME: add code to enable/Disbale camera lights if needed.
}

Mat ImagingController::takeImage() {
	return camera->captureImage();
}
