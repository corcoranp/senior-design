/*
 * qr_detector.h
 *
 *  Created on: Jan 31, 2016
 *      Author: Peter
 */

#ifndef QR_DETECTOR_H_
#define QR_DETECTOR_H_

#include <cv.h>
#include <opencv.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>

#include <vector>       // std::vector
#include <iostream>

using namespace cv;
using namespace std;

namespace blaze {

	class  QRDetector {


	public:
		QRDetector(Mat &img, bool debug){
			debugFlag = debug;
			windowedDebug = false;
			this->src_img = img;
			adjusted_img = src_img.clone();
			brightness = 50;
			contrast = 50;
			image_midline = 0;
		}

		bool debugFlag 	;
		bool windowedDebug;

		/*
		 * Canny Edge Detection Setting
		 */
		static int canny_thresh;
		static int canny_max_thresh;
		static int kernel_size;
		/*
		 * Image Correction Settings
		 */
		//Median Blur Settings
		static int medianBlurSize;

		//HSV Correction Settings....
		static int iLowH;
		static int iHighH;
		static int iLowS;
		static int iHighS;
		static int iLowV;
		static int iHighV ;

		int brightness;
		int contrast;

		//Erode & Dilate Settings
		static Size erodeSize ;
		static Size dilateSize ;

		//QR Detection Padding - the expanded area around the qr code
		static double padding ;

	/*
	 * QR Direction Identifiers
	 */
		static const int QR_NORTH ;
		static const int QR_EAST;
		static const int QR_SOUTH ;
		static const int QR_WEST;
	/*
	 * Lines & Colors Generic Properties
	 */

		//Line Properties
		static const int thickness ;
		static const int lineType ;

		//Colors
		static Scalar red; ;
		static Scalar green;
		static Scalar blue;


		void enableVideoDebug(bool showControls);

		/*
		 * QR Code Detection Function Headers
		 */
		void findQRCodesOnBlocks(Mat &img);
		void findQRCodesUsingCanny(int, void* );
		static void qrDetectionCallback(int, void* object);
		void findContoursCallback(int, void*);
		void findImageContours();
		void findQRCorners();
		void groupQRCorners();

		/*
		 * Image Enhancement Functions
		 */
		void enhanceSourceImageForMultiBlockDetection(Mat &img);

		static void medianBlurCallback(int, void*);
		void medianBlurImage();

		static void hsvThresholdCallback(int, void* object);
		void hsvThresholdImage();

		static void brightnessCallback(int, void* object);
		static void contrastCallback(int, void* object);
		void brightenImage();


	private:
		/*
		 * Global Image Mat definitions
		 */
		Mat src_img;		//main source image
		Mat adjusted_img;
		Mat thresh_img;		//threshold modified image
		Mat results_img;
		Mat blur_img;
		Mat median_img;
		Mat hsv_img;
		Mat cannyoutput_img;


		//Window Properties
		static string control_window 	;
		static string src_window 		;
		static string thresh_window 	;
		static string contour_window 	;
		static string result_window	 	;
		static string median_win		;

		static int window_posx ;
		static int window_posy ;

		static bool showThreholdWindow ;
		static bool showContours 		;

		/*
		 * Debug & Window Function Definitions
		 */
		void showWindow(string name, Mat image);


		/*
		 * Contours & Hierarchy
		 *
		 */
		vector<vector<Point> > m_contours;		//source image contour global ref
		vector<Vec4i> m_hierarchy; 				//source image hierarchy




		//Results Variables
		vector<int> m_results_index;
		vector<vector<int> > m_result_child_indexes;

		int image_midline;


		/*
		 * Functions defined in header file:
		 */
		void balance_white(cv::Mat mat) ;
		void debug(string msg);
		void rotate90(cv::Mat &matImage, int rotflag);

		float cv_distance(Point2f P, Point2f Q);					// Get Distance between two points
		float cv_lineEquation(Point2f L, Point2f M, Point2f J);		// Perpendicular Distance of a Point J from line formed by Points L and M; Solution to equation of the line Val = ax+by+c
		float cv_lineSlope(Point2f L, Point2f M, int& alignement);	// Slope of a line by two Points L and M on it; Slope of line, S = (x1 -x2) / (y1- y2)
		void cv_getVertices(vector<vector<Point> > contours, int c_id,float slope, vector<Point2f>& X);
		void cv_updateCorner(Point2f P, Point2f ref ,float& baseline,  Point2f& corner);
		void cv_updateCornerOr(int orientation, vector<Point2f> IN, vector<Point2f> &OUT);
		bool getIntersectionPoint(Point2f a1, Point2f a2, Point2f b1, Point2f b2, Point2f& intersection);
		float cross(Point2f v1,Point2f v2);


	};

}




#endif /* QR_DETECTOR_H_ */
