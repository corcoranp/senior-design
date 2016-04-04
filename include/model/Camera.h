/* 
 * File:   Camera.h
 * Author: Pieter Loubser
 *
 * Created on February 8, 2016, 3:49 PM
 */

#ifndef CAMERA_H
#define	CAMERA_H
#include <opencv2/opencv.hpp>
#include "../BasicFunctions.h"
//#include "../Navigation/NavigationInterface.h"

using namespace cv;

#define BLOCK_WIDTH   38.1 //Physical width of block in mm
#define BLOCK_HEIGHT  38.1 //Physical height of block in mm
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480


#define CLEAR(x) memset(&(x), 0, sizeof(x))

class Camera {
public:

	enum PIXELFORMAT {
		NONE,
		MJPEG,
		JPEG,
		DV,
		MPEG,
		H264,
		H264_NO_SC,
		H263,
		MPEG1,
		MPEG2,
		MPEG4,
		XVID,
		VC1_ANNEX_G,
		VC1_ANNEX_L
	};

	typedef struct CameraProperties_t {
		int index;
		PIXELFORMAT format;
		int imageWidth;
		int imageHeight;

	} CameraProperties;

	Camera(int index=0) throw (string);
	Camera(const Camera& orig);
	Point2D getObjectCenterDistance(Point2D qr);
	Mat captureImage();
	CameraProperties getCameraProperties() throw (string);

	virtual ~Camera();

private:
	int index;
	int videofd;
	uint8_t *buffer;
	//Private functions
	void openCamera(int index);
	bool init_mmap();
	bool setCameraProperties() throw (string);

};

#endif	/* CAMERA_H */

