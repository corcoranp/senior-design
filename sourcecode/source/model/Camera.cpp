/* 
 * File:   Camera.cpp
 * Author: Pieter Loubser
 * 
 * Created on February 8, 2016, 3:50 PM
 */

#include "../../include/model/Camera.h"
#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

//This is required to allocate space of the static pointer.
//Camera *Camera::s_instance = 0;

Camera::Camera(int index) throw (string) {
	this->index = index;
	openCamera(this->index);
	setCameraProperties();
	init_mmap();
}

Camera::Camera(const Camera& orig) {
	index = orig.index;
	buffer = orig.buffer;
	videofd = orig.videofd;
}

static int xioctl(int fd, int request, void *arg) {
	int r;

	do
		r = ioctl(fd, request, arg);
	while (-1 == r && EINTR == errno);

	return r;
}

Camera::~Camera() {
	//Turn off camera stream
	if(-1 == munmap(buffer, (sizeof(buffer)/sizeof(uint8_t)))){
		//TODO: Handle error
	}
	struct v4l2_buffer buf = { 0 };
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = 0;
	xioctl(videofd, VIDIOC_STREAMOFF, &buf.type);
	//Close camera connection
	close(videofd);
}

void Camera::openCamera(int index) {
	stringstream ss;
	ss << "/dev/video" << index;
	videofd = open(ss.str().c_str(), O_RDWR);
	if (videofd == -1) {
		string s("Unable to connect to camera:");
		s.append(ss.str());
		throw s;
	}
	struct v4l2_capability caps = { 0 };
	if (-1 == xioctl(videofd, VIDIOC_QUERYCAP, &caps)) {
		String s("Unsupported camera: ");
		s.append(ss.str());
		throw s;
	}
}

bool Camera::init_mmap() {
	if (videofd == -1) {
		throw string("Camera stream not open.");
	}
	struct v4l2_requestbuffers req = { 0 };
	req.count = 1;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(videofd, VIDIOC_REQBUFS, &req)) {
		perror("Requesting Buffer");
		return false;
	}

	struct v4l2_buffer buf = { 0 };
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = 0;
	if (-1 == xioctl(videofd, VIDIOC_QUERYBUF, &buf)) {
		perror("Querying Buffer");
		return false;
	}

	buffer = (uint8_t *) mmap(NULL, buf.length, PROT_READ | PROT_WRITE,
	MAP_SHARED, videofd, buf.m.offset);

	return true;
}

Mat Camera::captureImage() {
	if (videofd == -1) {
		throw string("Camera stream not open.");
	}
	CameraProperties cam = getCameraProperties();
	bool propSet = false;
	if(cam.imageWidth != IMAGE_WIDTH){
		setCameraProperties();
		propSet = true;
	}
	if((cam.imageWidth != IMAGE_HEIGHT) && !propSet){
		setCameraProperties();
		propSet = true;
	}
	if((cam.format != PIXELFORMAT::MJPEG) && !propSet){
		setCameraProperties();
		propSet = true;
	}
	struct v4l2_buffer buf;
	CLEAR(buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = 0;
	if (-1 == xioctl(videofd, VIDIOC_QBUF, &buf)) {
		throw string("Unable to query for the camera buffer.");
	}

	if (-1 == xioctl(videofd, VIDIOC_STREAMON, &buf.type)) {
		throw string("Unable to start image capture.");
	}

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(videofd, &fds);
	struct timeval tv = { 0 };
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	int r = select(videofd + 1, &fds, NULL, NULL, &tv);
	if (-1 == r) {
//		xioctl(videofd, VIDIOC_STREAMOFF, &buf.type);
		throw string("No available frames.");
	}

	if (-1 == xioctl(videofd, VIDIOC_DQBUF, &buf)) {
//		xioctl(videofd, VIDIOC_STREAMOFF, &buf.type);
		throw string("Unable to retrieve the frame.");
	}

	IplImage* frame;
	CvMat cvmat = cvMat(IMAGE_WIDTH, IMAGE_WIDTH, CV_8UC3, (void*) buffer);
	frame = cvDecodeImage(&cvmat, 1);
	Mat img = cv::cvarrToMat(frame);
//	xioctl(videofd, VIDIOC_STREAMOFF, &buf.type);
	return img;
}

bool Camera::setCameraProperties() throw (string){
	if (videofd == -1) {
		throw string("Camera not open.");
	}
	struct v4l2_format fmt;	// = { 0 };
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = IMAGE_WIDTH;
	fmt.fmt.pix.height = IMAGE_HEIGHT;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	fmt.fmt.pix.field = V4L2_FIELD_NONE;

	if (-1 == xioctl(videofd, VIDIOC_S_FMT, &fmt)) {
		return false;
	}
	return true;
}

Point2D Camera::getObjectCenterDistance(Point2D pt) {
	Point2D p;
	p.x = pt.x - (IMAGE_WIDTH / 2);
	p.x = pt.y - (IMAGE_HEIGHT / 2);
	return p;
}

Camera::CameraProperties Camera::getCameraProperties() throw (string){
	CameraProperties cam;
	if (videofd == -1) {
		throw string("Camera not open.");
	}
	struct v4l2_format fmt;	// = { 0 };
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = IMAGE_WIDTH;
	fmt.fmt.pix.height = IMAGE_HEIGHT;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	fmt.fmt.pix.field = V4L2_FIELD_NONE;

	if (-1 == xioctl(videofd, VIDIOC_G_FMT, &fmt)) {
		throw string("Unable to read settings from camera.");
	}
	cam.imageWidth = fmt.fmt.pix.width;
	cam.imageHeight = fmt.fmt.pix.height;
	switch(fmt.fmt.pix.pixelformat){
	case V4L2_PIX_FMT_MJPEG:
		cam.format = PIXELFORMAT::MJPEG;
		break;
	case V4L2_PIX_FMT_JPEG:
		cam.format = PIXELFORMAT::JPEG;
		break;
	case V4L2_PIX_FMT_DV:
		cam.format = PIXELFORMAT::DV;
		break;
	case V4L2_PIX_FMT_MPEG:
		cam.format = PIXELFORMAT::MPEG;
		break;
	case V4L2_PIX_FMT_H264:
		cam.format = PIXELFORMAT::H264;
		break;
	case V4L2_PIX_FMT_H264_NO_SC:
		cam.format = PIXELFORMAT::H264_NO_SC;
		break;
	case V4L2_PIX_FMT_H263:
		cam.format = PIXELFORMAT::H263;
		break;
	case V4L2_PIX_FMT_MPEG1:
		cam.format = PIXELFORMAT::MPEG1;
		break;
	case V4L2_PIX_FMT_MPEG2:
		cam.format = PIXELFORMAT::MPEG2;
		break;
	case V4L2_PIX_FMT_MPEG4:
		cam.format = PIXELFORMAT::MPEG4;
		break;
	case V4L2_PIX_FMT_XVID:
		cam.format = PIXELFORMAT::XVID;
		break;
	case V4L2_PIX_FMT_VC1_ANNEX_G:
		cam.format = PIXELFORMAT::VC1_ANNEX_G;
		break;
	case V4L2_PIX_FMT_VC1_ANNEX_L:
		cam.format = PIXELFORMAT::VC1_ANNEX_L;
		break;
	default:
		cam.format = PIXELFORMAT::NONE;
	}
	cam.index = index;
	return cam;
}
