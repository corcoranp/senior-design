/*
 * qr_detector.cpp
 *
 *  Created on: Jan 31, 2016
 *      Author: Peter
 */


#include "../../include/controllers/QRDetector.h"
#include <string>

using namespace blaze;
using namespace std;


/*
 * PUBLIC PROPERTIES
 */




	int QRDetector::canny_thresh = 100;
	int QRDetector::canny_max_thresh  = 255;
	int QRDetector::kernel_size = 5;  //Gaussian kernel - The larger the size is, the lower the detectorÕs sensitivity to noise

	//Median Blur Settings
	int QRDetector::medianBlurSize = 3;


	//HSV Correction Settings....
	int QRDetector::iLowH = 0;
	int QRDetector::iHighH = 179;
	int QRDetector::iLowS = 0;
	int QRDetector::iHighS = 255;
	int QRDetector::iLowV = 200;
	int QRDetector::iHighV = 255;

	//Erode & Dilate Settings
	Size QRDetector::erodeSize = Size(2, 2);
	Size QRDetector::dilateSize = Size(2, 2);

	//QR Detection Padding - the expanded area around the qr code
	double QRDetector::padding = .3;

	/*
	* QR Direction Identifiers
	*/
	const int QRDetector::QR_NORTH 	= 0;
	const int QRDetector::QR_EAST 	= 1;
	const int QRDetector::QR_SOUTH 	= 2;
	const int QRDetector::QR_WEST 	= 3;

	/*
	* Lines & Colors Generic Properties
	*/
	//Line Properties
	const int QRDetector::thickness = 2;
	const int QRDetector::lineType = 8;

	//Colors
	Scalar QRDetector::red = Scalar( 0, 0, 255 );
	Scalar QRDetector::green = Scalar( 0, 255, 0 );
	Scalar QRDetector::blue = Scalar( 255, 0, 0 );


	//Window Properties
	string QRDetector::src_window 		= "Source window";
	string QRDetector::control_window 	= "Control Bar";
	string QRDetector::thresh_window 	= "Thresh window";
	string QRDetector::contour_window 	= "Contour window";
	string QRDetector::result_window	 = "Result window";
	string QRDetector::median_win		= "Median Window";

	int QRDetector::window_posx 		= 10;
	int QRDetector::window_posy 		= 10;

	bool QRDetector::showThreholdWindow = false;
	bool QRDetector::showContours 		= false;



	/*
	 * PUBLIC FUNCTIONS
	 */

	//	Function for video debug
	void QRDetector::enableVideoDebug(bool showControls){
		this->windowedDebug = true;
		Mat m1 =  Mat::zeros(1, 512, CV_64F);

		if(!showControls) return;
		namedWindow( this->control_window, WINDOW_NORMAL);

		//namedWindow( this->median_win, WINDOW_NORMAL );
		createTrackbar( "Median Blur", control_window, &medianBlurSize, 15, &QRDetector::medianBlurCallback, this );

		//namedWindow( this->thresh_window, WINDOW_NORMAL);
		createTrackbar( "High H", control_window, &iHighH, 179, &QRDetector::hsvThresholdCallback, this );
		createTrackbar( "Low H", control_window, &iLowH, 179, &QRDetector::hsvThresholdCallback, this );

		createTrackbar( "High S", control_window, &iHighS, 255, &QRDetector::hsvThresholdCallback, this );
		createTrackbar( "Low S", control_window, &iLowS, 255, &QRDetector::hsvThresholdCallback, this );

		createTrackbar( "Low V", control_window, &iLowV, 255, &QRDetector::hsvThresholdCallback, this );
		createTrackbar( "High V", control_window, &iHighV, 255, &QRDetector::hsvThresholdCallback, this );


		//namedWindow(this->src_window, WINDOW_NORMAL);
		createTrackbar( "Brightness", control_window, &brightness, 100, &QRDetector::brightnessCallback, this );
		createTrackbar( "Contrast", control_window, &contrast, 100, &QRDetector::contrastCallback, this );
		createTrackbar( "QR Detection:", control_window, 0, 1, &QRDetector::qrDetectionCallback, this );


		imshow(control_window,m1);

	}


	// Call back method for callingfindQRCodesUsingCanny
	void QRDetector::qrDetectionCallback(int, void* object){
		QRDetector* qrd = (QRDetector*) object;
		qrd->findQRCodesUsingCanny( 0, 0);
	}


	/*
	 * Median Blur funcitons
	 */
	//Call back method for calling medianBlurImage
	void QRDetector::medianBlurCallback(int, void* object){
		QRDetector* qrd = (QRDetector*) object;
		qrd->medianBlurImage();
	}
	void QRDetector::medianBlurImage(){
		//STEP: Median blur creates a intense blur image
		try{

			medianBlur(adjusted_img, median_img, medianBlurSize);


			if(windowedDebug)
				showWindow(this->median_win, this->median_img);

			hsvThresholdImage();
			waitKey(0);
		}catch(int err){

			debug("Error in median blur method [" + std::to_string(err) + "]");
		}
	}

	/*
	 * Brightness & Contrast Functions
	 */
	void QRDetector::brightnessCallback(int, void* object){
		QRDetector* qrd = (QRDetector*) object;
		qrd->brightenImage();
	}
	void QRDetector::contrastCallback(int, void* object){
		QRDetector* qrd = (QRDetector*) object;
		qrd->brightenImage();
	}
	void QRDetector::brightenImage(){
		try{
			double dContrast = contrast / 50.0;
			int iBrightness  = brightness - 50;
			src_img.convertTo(adjusted_img, -1, dContrast, iBrightness);

			if(windowedDebug)
				showWindow(src_window, adjusted_img);

			medianBlurImage();
			waitKey(0);
		}catch(int err){
			debug("Error in median blur method [" + to_string(err) + "]");
		}
	}

	/*
	 * Threshold Adjustment Functions
	 */
	void QRDetector::hsvThresholdCallback(int, void* object){
		QRDetector* qrd = (QRDetector*) object;
		qrd->hsvThresholdImage();
	}
	void QRDetector::hsvThresholdImage(){
		//Convert the captured frame from BGR to HSV
		cvtColor(median_img, hsv_img, COLOR_BGR2HSV);
		inRange(hsv_img, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), thresh_img); //Threshold the image


		//Modify image...
		//morphological opening (remove small objects from the foreground)
		erode(thresh_img, thresh_img, getStructuringElement(MORPH_ELLIPSE, erodeSize) );
		dilate( thresh_img, thresh_img, getStructuringElement(MORPH_ELLIPSE, dilateSize) );

		//morphological closing (fill small holes in the foreground)
		dilate( thresh_img, thresh_img, getStructuringElement(MORPH_ELLIPSE, erodeSize) );
		erode(thresh_img, thresh_img, getStructuringElement(MORPH_ELLIPSE, dilateSize) );

		if(windowedDebug)
			showWindow(this->thresh_window, this->thresh_img);

		waitKey(0);
	}

	/*
	 * Function for finding QR codes on Blocks
	 */
	void QRDetector::findQRCodesOnBlocks(Mat &img ){
		enhanceSourceImageForMultiBlockDetection(img);

		findImageContours();
		findQRCorners();
		groupQRCorners();
		return;
		//raw color image, modifies for edge detection.
		enhanceSourceImageForMultiBlockDetection(img);
		debug("Enhancing Source Image Completed");

		//Find all QR codes using Canny thresholds
	    findQRCodesUsingCanny(0, 0);
	    debug("Find QR using Canny Edge detection");

	    if(this->windowedDebug)
	    	waitKey(0);
		return;
	}



	/*
	 * Callback function that can be used when creating a trackbar
	 * (for canny thresholds)
	 */
	void findContoursCallback(int, void* object){
		QRDetector* qrd = (QRDetector*) object;
		qrd->findImageContours();
	}


	void QRDetector::findImageContours(){
		debug("Detect Canny Edges");
		// Detect edges using canny on our modified image
		Canny( this->thresh_img, this->cannyoutput_img, canny_thresh, canny_max_thresh, kernel_size );

		debug("Find Contours");
		// Find contours
		findContours( cannyoutput_img, this->m_contours, this->m_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	}

	void QRDetector::findQRCorners(){
		debug("Clone SRC Image to results_img");
		this->results_img = this->src_img.clone();

		int high_y = 0;
		int low_y = 65000;
		int parent_idx = 0;

		//Children Tracking Array
		vector<Vec4i> children;
		vector<int> children_contour_idx;  //indexes of all last children

		if(m_contours.empty() && m_hierarchy.empty()){
			//No data to find qr corners...technically this should throw an err...
			debug("No contour or hierarchy data found");
			return;
		}

		vector<Moments> mu(m_contours.size()); //moments
		vector<Point2f> mc(m_contours.size()); // mass centers

		//loop through all contours
		for( int i = 0; i< m_contours.size(); i++)	{

			/*
			 * DEBUG
			 */

			/*
			 * Process
			 */
			Rect r = boundingRect(m_contours[i]);		//Create bounding rectangle for current contour
			mu[i] = moments( m_contours[i], false );  //calculate moment
			mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );	//calculate mass center


			//Add children to array, clear otherwise, basically keep history of children...
			//if(m_hierarchy[i][0] == -1 && m_hierarchy[i][1] == -1){ only children
			if(m_hierarchy[i][2] > -1) {    //entire hierarchy
				//Is child
				//Write Hierarchy Data
				if(children.size() == 0){
					parent_idx = i;
					cout << "Parent:      / [" + to_string(i) + "] " + "[" + to_string(m_hierarchy[i][0]) + "," + to_string(m_hierarchy[i][1]) + "," + to_string(m_hierarchy[i][2]) + "," + to_string(m_hierarchy[i][3]) + "]" << endl;
				}else{
					cout << "Child:       | [" + to_string(i) + "] " + "[" + to_string(m_hierarchy[i][0]) + "," + to_string(m_hierarchy[i][1]) + "," + to_string(m_hierarchy[i][2]) + "," + to_string(m_hierarchy[i][3]) + "]" << endl;
				}
				children.push_back(m_hierarchy[i]);
				children_contour_idx.push_back(i);
			}

			//find last child - but keep in mind loop has been capturing children since the last one...
			//if(m_hierarchy[i][1] == -1 && m_hierarchy[i][2] == -1){ //last child
			if(m_hierarchy[i][2] == -1){ //last child has no children
				if(m_hierarchy[i][1] == -1 && m_hierarchy[i][2] == -1){
					cout << "Last:        \\ [" + to_string(i) + "] " + "[" + to_string(m_hierarchy[i][0]) + "," + to_string(m_hierarchy[i][1]) + "," + to_string(m_hierarchy[i][2]) + "," + to_string(m_hierarchy[i][3]) + "]" << endl;
				}else{
					//Single item
					cout << "Single Node: - [" + to_string(i) + "] " + "[" + to_string(m_hierarchy[i][0]) + "," + to_string(m_hierarchy[i][1]) + "," + to_string(m_hierarchy[i][2]) + "," + to_string(m_hierarchy[i][3]) + "]" << endl;
				}

				if(children.size() > 2) { //look at last child with more than 2 previous children
					// area ratio filter...
					double largest_ratio = 0;
					for(int n=0; n< children.size(); n++ ) //loop through child elements...
					{
						Rect rc = boundingRect(m_contours[children_contour_idx[n]]); //bound child contour
						//rectangle(results,Point(rc.x,rc.y), Point(rc.x+rc.width,rc.y+rc.height), child_color ,2,8,0);
						double child_ratio = rc.area()/r.area();

						if(child_ratio > largest_ratio) //get largest ratio difference
							largest_ratio = child_ratio;
					}
					if(largest_ratio < 6){ //child area ratio should be less than 6x exclude other items with larger ratios
						//YOU FOUND A CENTER!!!!!!!!!
						//we need to get the highest and lowest y of the centers...
						if(r.y > high_y)
							high_y = r.y;
						if(r.y < low_y)
							low_y = r.y - r.height;

						Rect rp = boundingRect(m_contours[parent_idx]);
						rectangle(results_img,Point(rp.x,rp.y), Point(rp.x+rp.width,rp.y+rp.height), red ,2,8,0);
						rectangle(results_img,Point(r.x,r.y), Point(r.x+r.width,r.y+r.height), red ,2,8,0);

						putText(results_img, to_string(i), Point(r.x,r.y-15), FONT_HERSHEY_COMPLEX_SMALL, 0.8, blue, 1, CV_AA);

							//track all centers as results....
						m_results_index.push_back(i);
						m_result_child_indexes.push_back(children_contour_idx);  //track reverse child
					}
				}

				//This was the last child, all detection is completed, so delete history
				children.clear();
				children_contour_idx.clear();
			}
		}

		//mid line should be between all the blocks
		this->image_midline = ((high_y - low_y)/2) + low_y ;
		//debug("Image Midline: " + to_string(image_midline));

		if(this->windowedDebug){
		  	namedWindow( result_window, WINDOW_NORMAL );
			imshow(result_window, results_img );

		}
	}

	void QRDetector::groupQRCorners(){

		// next we need to identify the groups of points that are related...
		vector<vector<int> > block_groups;
		int first_marker; int second_marker =0; int third_marker=0;
		vector<int> block_tracker;

		debug("results count: " + to_string(m_results_index.size()));
		//loop through results
		for(int j = 0; j<m_results_index.size(); j++)
		{
			first_marker = 0; second_marker =0; third_marker=0;
			int cidx = m_results_index[j];
			int tidx = m_result_child_indexes[j][0];

			//add top level block...
			vector<int> block_idx;
			first_marker = tidx; //this is the first contour marker

			Rect rec = boundingRect(m_contours[cidx]);	// inner bounding box
			bool isOnTop = (rec.y > image_midline) ? true : false;

			// loop through each point so we can compare it to the current contour
			for(int l=0; l<m_results_index.size(); l++){
				//Point measuring to...

				Rect box = boundingRect(m_contours[m_results_index[l]]);
				bool isToBlockOnTop = (box.y > image_midline) ? true : false; //this is our top/bottom filter

				if((isToBlockOnTop && isOnTop) || (!isToBlockOnTop && !isOnTop) ) {
					//both blocks are on top
					double distance = cv_distance(Point(rec.x,rec.y), Point(box.x, box.y));

					if(distance < rec.height*7 && distance > 0) //ratio height to 6* distance
					{
						if(second_marker == 0)
							second_marker = m_result_child_indexes[l][0]; //top level contour
						else
							third_marker = m_result_child_indexes[l][0]; //top level contour

						//cout << "Distance: "  + to_string(dist) ;
						//cout << "\tHeight: " + to_string(rec.height) << std::endl;
						//line( results, Point(rec.x,rec.y),Point(box.x, box.y), Scalar( 0, 0, 0 ), 4, 8 );
					}
				}
			}



			//Add the three markers to the vector
			block_idx.push_back(first_marker);
			block_idx.push_back(second_marker);
			block_idx.push_back(third_marker);

			//sort the vector so the smallest index is first
			sort (block_idx.begin(), block_idx.end());

			//check to see if the vector is already a block:
			vector<vector<int> >::iterator it;
			it = find (block_groups.begin(), block_groups.end(), block_idx);
			if(it != block_groups.end()){

			}else{
				//NOT an existing block so we add it to the array.
				if(block_idx[0] != 0)
					block_groups.push_back(block_idx);
			}
			//circle( results, Point(r.x,r.y), 10.0, child_color, thickness, lineType );
			//putText(results, to_string(i), Point(r.x,r.y-25), FONT_HERSHEY_COMPLEX_SMALL, 0.8, child_color, 1, CV_AA);
		}


		//last loop
		for(int n=0;n<block_groups.size(); n++ )
		{
			cout << "Block Group:  [" + to_string(block_groups[n][0]) + ","  + to_string(block_groups[n][1]) + ","  + to_string(block_groups[n][2]) + "]" << endl;

			Rect r1 = boundingRect(m_contours[block_groups[n][0]]);
			Rect r2 = boundingRect(m_contours[block_groups[n][1]]);
			Rect r3 = boundingRect(m_contours[block_groups[n][2]]);

			//find top left, bottom right...
			double x1 = (r1.x > r2.x) ? (r1.x > r3.x) ? r1.x : r3.x : (r2.x > r3.x) ? r2.x : r3.x;
			double y1 = (r1.y > r2.y) ? (r1.y > r3.y) ? r1.y : r3.y : (r2.y > r3.y) ? r2.y : r3.y;
			double x2 = (r1.x < r2.x) ? (r1.x < r3.x) ? r1.x : r3.x : (r2.x < r3.x) ? r2.x : r3.x;
			double y2 = (r1.y < r2.y) ? (r1.y < r3.y) ? r1.y : r3.y : (r2.y < r3.y) ? r2.y : r3.y;

			debug("here");
			Point bottom_p(x1 + r1.width+(r1.width*padding),y1+r1.height+(r1.height*padding));
			Point top_p(x2-(r1.width*padding),y2-(r1.height*padding));
			rectangle(results_img,top_p, bottom_p, blue, 2,8,0);

			Rect qr_boundary(top_p, bottom_p);


			if(this->windowedDebug){
				Mat pimg;
				pimg = src_img(qr_boundary).clone();

				string qr_window = "QR Code: " + to_string(n);
				showWindow(qr_window, pimg);


				namedWindow( result_window, WINDOW_NORMAL );
				imshow(result_window, results_img );



			}

		}

		if(windowedDebug){
			waitKey(0);
		}
	}



	/** @function thresh_callback */
	void QRDetector::findQRCodesUsingCanny(int, void* )
	{
		Mat results = src_img.clone();  //clone src image...
		Mat drawing;
		int high_y = 0;
		int low_y = 65000;

		//Canny Variables:
		Mat canny_output;
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		//Children Tracking Array
		vector<Vec4i> children;
		vector<int> children_contour_idx;  //indexes of all last children

		//Results Variables
		vector<int> results_index;
		vector<vector<int> > result_child_indexes;

		// Detect edges using canny on our modified image
		Canny( thresh_img, canny_output, canny_thresh, canny_max_thresh, kernel_size );

		// Find contours
		findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );


		vector<Moments> mu(contours.size()); //moments
		vector<Point2f> mc(contours.size()); // mass centers

		/*if(showContours){
			//tracking for contours
			drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
		}*/

		if ( !contours.empty() && !hierarchy.empty() ) {
			for( int i = 0; i< contours.size(); i++)	{ //loop through all contours

				mu[i] = moments( contours[i], false );
				mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );

				//Create bounding rectangle for current contour
				Rect r = boundingRect(contours[i]);
				//Draw Contours:
				//if(showContours){
				//	Scalar color = Scalar( 255,255,255);//Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
				//	drawContours( drawing, contours, i, color, 1, 8, hierarchy, 6, Point() );
				//}

				cout << "Hierarchy [" + to_string(i) + "] " + "[" + to_string(hierarchy[i][0]) + ","
						+ to_string(hierarchy[i][1]) + ","
						+ to_string(hierarchy[i][2]) + ","
						+ to_string(hierarchy[i][3]) + "]" << endl;

				//Add children to array, clear otherwise, basically keep history of children...
				if(hierarchy[i][0] == -1 && hierarchy[i][1] == -1){
					children.push_back(hierarchy[i]);
					children_contour_idx.push_back(i);
				}else{
					children.clear();
					children_contour_idx.clear();
				}

				//find last child - but keep in mind loop has been capturing children since the last one...
				if(hierarchy[i][1] == -1 && hierarchy[i][2] == -1){ //last child

					if(children.size() > 2){ //look at last child with more than 2 previous children
						// area ratio filter...
						double largest_ratio = 0;
						for(int n=0; n<children.size(); n++ ) //loop through child elements...
						{
							Rect rc = boundingRect(contours[children_contour_idx[n]]); //bound child contour
							//rectangle(results,Point(rc.x,rc.y), Point(rc.x+rc.width,rc.y+rc.height), child_color ,2,8,0);
							double child_ratio = rc.area()/r.area();

							if(child_ratio > largest_ratio) //get largest ratio difference
								largest_ratio = child_ratio;
						}
						if(largest_ratio < 6){ //child area ratio should be less than 6x exclude other items with larger ratios
							//YOU FOUND A CENTER!!!!!!!!!
							//we need to get the highest and lowest y of the centers...
							if(r.y > high_y)
								high_y = r.y;
							if(r.y < low_y)
								low_y = r.y - r.height;

							rectangle(results,Point(r.x,r.y), Point(r.x+r.width,r.y+r.height), red ,2,8,0);
							putText(results, to_string(i), Point(r.x,r.y-15), FONT_HERSHEY_COMPLEX_SMALL, 0.8, blue, 1, CV_AA);

							//track all centers as results....
							results_index.push_back(i);
							result_child_indexes.push_back(children_contour_idx);  //track reverse child
						}
					}
				}
			}
		}


		//mid line should be between all the blocks
		int mid_line = ((high_y - low_y)/2) + low_y ;


		// next we need to identify the groups of points that are related...
		vector<vector<int> > block_groups;
		int first_marker; int second_marker =0; int third_marker=0;
		vector<int> block_tracker;

		//loop through results
		for(int j = 0; j<results_index.size(); j++)
		{
			first_marker = 0; second_marker =0; third_marker=0;
			int cidx = results_index[j];
			int tidx = result_child_indexes[j][0];


			//add top level block...
			vector<int> block_idx;
			first_marker = tidx; //this is the first contour marker

			Rect rec = boundingRect(contours[cidx]);	// inner bounding box

			bool isOnTop = (rec.y > mid_line) ? true : false;

			//cout << "\tLocation: (" + to_string(rec.x) + "," + to_string(rec.y) + ")" ;
			//cout << "\tIs On Top: " + to_string(isOnTop) << endl;

			//create rectangle around current
			//rectangle(results,Point(rec.x,rec.y), Point(rec.x+rec.width,rec.y+rec.height), red, 2,8,0);
			//rectangle(results,Point(rec_out.x,rec_out.y), Point(rec_out.x+rec_out.width,rec_out.y+rec_out.height), red, 2,8,0);

			// loop through each point so we can compare it to the current contour
			for(int l=0; l<results_index.size(); l++){
				//Point measuring to...

				Rect box = boundingRect(contours[results_index[l]]);
				bool isToBlockOnTop = (box.y > mid_line) ? true : false; //this is our top/bottom filter

				if((isToBlockOnTop && isOnTop) || (!isToBlockOnTop && !isOnTop) ) {
					//both blocks are on top
					double distance = cv_distance(Point(rec.x,rec.y), Point(box.x, box.y));

					if(distance < rec.height*7 && distance > 0) //ratio height to 6* distance
					{
						if(second_marker == 0)
							second_marker = result_child_indexes[l][0]; //top level contour
						else
							third_marker = result_child_indexes[l][0]; //top level contour

						//cout << "Distance: "  + to_string(dist) ;
						//cout << "\tHeight: " + to_string(rec.height) << std::endl;
						//line( results, Point(rec.x,rec.y),Point(box.x, box.y), Scalar( 0, 0, 0 ), 4, 8 );
					}
				}
			}
			//Add the three markers to the vector
			block_idx.push_back(first_marker);
			block_idx.push_back(second_marker);
			block_idx.push_back(third_marker);

			//sort the vector so the smallest index is first
			sort (block_idx.begin(), block_idx.end());

			//check to see if the vector is already a block:
			vector<vector<int> >::iterator it;
			it = find (block_groups.begin(), block_groups.end(), block_idx);
			if(it != block_groups.end()){

			}else{
				//NOT an existing block so we add it to the array.
				if(block_idx[0] != 0)
					block_groups.push_back(block_idx);
			}
			//circle( results, Point(r.x,r.y), 10.0, child_color, thickness, lineType );
			//putText(results, to_string(i), Point(r.x,r.y-25), FONT_HERSHEY_COMPLEX_SMALL, 0.8, child_color, 1, CV_AA);
		}



		Mat qr,qr_raw,qr_gray,qr_thres;



		//last loop
		for(int n=0;n<block_groups.size(); n++ )
		{
			cout << "Block Group:  [" + to_string(block_groups[n][0]) + ","  + to_string(block_groups[n][1]) + ","  + to_string(block_groups[n][2]) + "]" << endl;
	try{
			Rect r1 = boundingRect(contours[block_groups[n][0]]);
			Rect r2 = boundingRect(contours[block_groups[n][1]]);
			Rect r3 = boundingRect(contours[block_groups[n][2]]);
			//find top left, bottom right...
			double x1 = (r1.x > r2.x) ? (r1.x > r3.x) ? r1.x : r3.x : (r2.x > r3.x) ? r2.x : r3.x;
			double y1 = (r1.y > r2.y) ? (r1.y > r3.y) ? r1.y : r3.y : (r2.y > r3.y) ? r2.y : r3.y;
			double x2 = (r1.x < r2.x) ? (r1.x < r3.x) ? r1.x : r3.x : (r2.x < r3.x) ? r2.x : r3.x;
			double y2 = (r1.y < r2.y) ? (r1.y < r3.y) ? r1.y : r3.y : (r2.y < r3.y) ? r2.y : r3.y;

			Point bottom_p(x1 + r1.width+(r1.width*padding),y1+r1.height+(r1.height*padding));
			Point top_p(x2-(r1.width*padding),y2-(r1.height*padding));
			rectangle(results,top_p, bottom_p, blue, 2,8,0);

			Rect qr_boundary(top_p, bottom_p);
			Mat image; Mat gray; Mat edges; Mat pimg;

			pimg = src_img(qr_boundary).clone();

			string qr_window = "QR Code: " + to_string(n);

			//showWindow(qr_window, pimg);



	/*
	 * START OF QR MOD
	 */
			vector<vector<Point> > contours1;
			vector<Vec4i> hierarchy1;
			int mark;
			int A,B,C,top,right,bottom,median1,median2;
			int outlier;
			float AB,BC,CA, dist,slope, areat,arear,areab, large, padding;
			int align,orientation;


			cv::medianBlur(pimg, image, 3);
			cvtColor(image,gray,CV_RGB2GRAY);		// Convert Image captured from Image Input to GrayScale

			//Convert the captured frame from BGR to HSV
			/*cvtColor(pimg, image, COLOR_BGR2HSV);
			inRange(image, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), gray); //Threshold the image


			erode(gray, gray, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
			dilate( gray, gray, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
			dilate( gray, gray, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
			erode(gray, gray, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
	*/
			//cvtColor(image,gray,CV_RGB2GRAY);		// Convert Image captured from Image Input to GrayScale
			Canny(gray, edges, 100 , 200, 3);		// Apply Canny edge detection on the gray image
			findContours( edges, contours1, hierarchy1, RETR_TREE, CHAIN_APPROX_SIMPLE); // Find contours with hierarchy

			mark = 0;
			Mat traces(image.size(), CV_8UC3);
			traces = Scalar(0,0,0);


			qr_raw = Mat::zeros(100, 100, CV_8UC3 );
		   	qr = Mat::zeros(100, 100, CV_8UC3 );
			qr_gray = Mat::zeros(100, 100, CV_8UC1);
		   	qr_thres = Mat::zeros(100, 100, CV_8UC1);




			vector<Moments> mu(contours1.size());
	  		vector<Point2f> mc(contours1.size());

			for( int i = 0; i < contours1.size(); i++ )
			{	mu[i] = moments( contours1[i], false );
				mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
			}


			for( int i = 0; i < contours1.size(); i++ )
			{
				int k=i;
				int c=0;

				while(hierarchy1[k][2] != -1)
				{
					k = hierarchy1[k][2] ;
					c = c+1;
				}
				if(hierarchy1[k][2] != -1)
				c = c+1;

				if (c >= 5)
				{
					if (mark == 0)		A = i;
					else if  (mark == 1)	B = i;		// i.e., A is already found, assign current contour to B
					else if  (mark == 2)	C = i;		// i.e., A and B are already found, assign current contour to C
					mark = mark + 1 ;
				}
			}

			if (mark >= 2)		// Ensure we have (atleast 3; namely A,B,C) 'Alignment Markers' discovered
			{


				// We have found the 3 markers for the QR code; Now we need to determine which of them are 'top', 'right' and 'bottom' markers

				// Determining the 'top' marker
				// Vertex of the triangle NOT involved in the longest side is the 'outlier'


				//A = block_groups[n][0];
				//B = block_groups[n][1];
				//C = block_groups[n][2];

				//cout << "block: [" + to_string(A) + "," + to_string(B) + "," + to_string(C) + "]" << endl;

				//AB = cv_distance(Point(r1.x,r1.y),Point(r2.x,r2.y));
				//BC = cv_distance(Point(r2.x,r2.y),Point(r3.x,r3.y));
				//CA = cv_distance(Point(r3.x,r3.y),Point(r1.x,r1.y));
				AB = cv_distance(mc[A],mc[B]);
				BC = cv_distance(mc[B],mc[C]);
				CA = cv_distance(mc[C],mc[A]);

				cout << "Distances: [" + to_string(AB) + "," + to_string(BC) + "," + to_string(CA) + "]" << endl;

				if ( AB > BC && AB > CA )
				{
					outlier = C; median1=A; median2=B;
				}
				else if ( CA > AB && CA > BC )
				{
					outlier = B; median1=A; median2=C;
				}
				else if ( BC > AB && BC > CA )
				{
					outlier = A;  median1=B; median2=C;
				}
				top = outlier;							// The obvious choice

				cout << "Outlier: " + to_string(outlier) + "," + to_string(median1) + "," + to_string(median2) << endl;

				dist = cv_lineEquation(mc[median1], mc[median2], mc[outlier]);	// Get the Perpendicular distance of the outlier from the longest side
				slope = cv_lineSlope(mc[median1], mc[median2],align);		// Also calculate the slope of the longest side

				// Now that we have the orientation of the line formed median1 & median2 and we also have the position of the outlier w.r.t. the line
				// Determine the 'right' and 'bottom' markers

				if (align == 0)
				{
					bottom = median1;
					right = median2;
				}
				else if (slope < 0 && dist < 0 )		// Orientation - North
				{
					bottom = median1;
					right = median2;
					orientation = QR_NORTH;
				}
				else if (slope > 0 && dist < 0 )		// Orientation - East
				{
					right = median1;
					bottom = median2;
					orientation = QR_EAST;
				}
				else if (slope < 0 && dist > 0 )		// Orientation - South
				{
					right = median1;
					bottom = median2;
					orientation = QR_SOUTH;
				}

				else if (slope > 0 && dist > 0 )		// Orientation - West
				{
					bottom = median1;
					right = median2;
					orientation = QR_WEST;
				}

				cout << "Orientation: " + to_string(orientation) << endl;



				vector<Point2f> L,M,O, tempL,tempM,tempO;
				Point2f N;

				vector<Point2f> src,dst;		// src - Source Points basically the 4 end co-ordinates of the overlay image
												// dst - Destination Points to transform overlay image

				Mat warp_matrix;

				cv_getVertices(contours1,top,slope,tempL);
				cv_getVertices(contours1,right,slope,tempM);
				cv_getVertices(contours1,bottom,slope,tempO);

				cv_updateCornerOr(orientation, tempL, L); 			// Re-arrange marker corners w.r.t orientation of the QR code
				cv_updateCornerOr(orientation, tempM, M); 			// Re-arrange marker corners w.r.t orientation of the QR code
				cv_updateCornerOr(orientation, tempO, O); 			// Re-arrange marker corners w.r.t orientation of the QR code

				int iflag = getIntersectionPoint(M[1],M[2],O[3],O[2],N);


				src.push_back(L[0]);
				src.push_back(M[1]);
				src.push_back(N);
				src.push_back(O[3]);

				//Mat qr,qr_raw,qr_gray,qr_thres;

				dst.push_back(Point2f(0,0));
				dst.push_back(Point2f(qr.cols,0));
				dst.push_back(Point2f(qr.cols, qr.rows));
				dst.push_back(Point2f(0, qr.rows));

				if (src.size() == 4 && dst.size() == 4 )			// Failsafe for WarpMatrix Calculation to have only 4 Points with src and dst
				{
					warp_matrix = getPerspectiveTransform(src, dst);
					warpPerspective(image, qr_raw, warp_matrix, Size(qr.cols, qr.rows));
					copyMakeBorder( qr_raw, qr, 10, 10, 10, 10,BORDER_CONSTANT, Scalar(255,255,255) );

					cvtColor(qr,qr_thres,CV_RGB2GRAY);
					//threshold(qr_gray, qr_thres, 127, 255, CV_THRESH_BINARY);
					imshow("qr thres" + to_string(n), qr_thres);

				}
				//Draw contours on the image
				//drawContours( image, contours1, top , Scalar(255,200,0), 2, 8, hierarchy1, 0 );
				//drawContours( image, contours1, right , Scalar(0,0,255), 2, 8, hierarchy1, 0 );
				//drawContours( image, contours1, bottom , Scalar(255,0,100), 2, 8, hierarchy1, 0 );


				// Debug Prints
									// Visualizations for ease of understanding
									if (slope > 5)
										circle( traces, Point(10,20) , 5 ,  Scalar(0,0,255), -1, 8, 0 );
									else if (slope < -5)
										circle( traces, Point(10,20) , 5 ,  Scalar(255,255,255), -1, 8, 0 );

									// Draw contours on Trace image for analysis
									drawContours( traces, contours, top , Scalar(255,0,100), 1, 8, hierarchy, 0 );
									drawContours( traces, contours, right , Scalar(255,0,100), 1, 8, hierarchy, 0 );
									drawContours( traces, contours, bottom , Scalar(255,0,100), 1, 8, hierarchy, 0 );

									// Draw points (4 corners) on Trace image for each Identification marker
									circle( traces, L[0], 2,  Scalar(255,255,0), -1, 8, 0 );
									circle( traces, L[1], 2,  Scalar(0,255,0), -1, 8, 0 );
									circle( traces, L[2], 2,  Scalar(0,0,255), -1, 8, 0 );
									circle( traces, L[3], 2,  Scalar(128,128,128), -1, 8, 0 );

									circle( traces, M[0], 2,  Scalar(255,255,0), -1, 8, 0 );
									circle( traces, M[1], 2,  Scalar(0,255,0), -1, 8, 0 );
									circle( traces, M[2], 2,  Scalar(0,0,255), -1, 8, 0 );
									circle( traces, M[3], 2,  Scalar(128,128,128), -1, 8, 0 );

									circle( traces, O[0], 2,  Scalar(255,255,0), -1, 8, 0 );
									circle( traces, O[1], 2,  Scalar(0,255,0), -1, 8, 0 );
									circle( traces, O[2], 2,  Scalar(0,0,255), -1, 8, 0 );
									circle( traces, O[3], 2,  Scalar(128,128,128), -1, 8, 0 );

									// Draw point of the estimated 4th Corner of (entire) QR Code
									circle( traces, N, 2,  Scalar(255,255,255), -1, 8, 0 );

									// Draw the lines used for estimating the 4th Corner of QR Code
									line(traces,M[1],N,Scalar(0,0,255),1,8,0);
									line(traces,O[3],N,Scalar(0,0,255),1,8,0);


				// Show the Orientation of the QR Code wrt to 2D Image Space
				int fontFace = FONT_HERSHEY_PLAIN;

				if(orientation == QR_NORTH)
				{
					putText(traces, "NORTH", Point(20,30), fontFace, 1, Scalar(0, 255, 0), 2, 8);
				}
				else if (orientation == QR_EAST)
				{

					putText(traces, "EAST", Point(20,30), fontFace, 1, Scalar(0, 255, 0), 2, 8);
				}
				else if (orientation == QR_SOUTH)
				{

					putText(traces, "SOUTH", Point(20,30), fontFace, 1, Scalar(0, 255, 0), 2, 8);
				}
				else if (orientation == QR_WEST)
				{
					putText(traces, "WEST", Point(20,30), fontFace, 1, Scalar(0, 255, 0), 2, 8);
				}
			}

			imshow ( "Image" + to_string(n), image );
			imshow ( "Traces"+ to_string(n), traces );

	}catch(int e){
		cout << "An exception occurred. Exception Nr. " << e << endl;
	}
		}

		/// Show in a window
	//	if(showContours){
			//namedWindow( contour_window, WINDOW_NORMAL );
			//imshow( contour_window, drawing );
	//	}
	  	namedWindow( result_window, WINDOW_NORMAL );
		imshow(result_window, results );

		waitKey(4);
	}











	//PRIVATE FUNCTIONS

	void QRDetector::debug(string msg){
		cout << msg << endl;
	}

	/**********************************************
	 *
	 *	Function code - Mat operations
	 *
	 **********************************************
	 */

	void QRDetector::enhanceSourceImageForMultiBlockDetection(Mat &img){

		//white balance the image...
		this->balance_white(img);

		//STEP: Median blur creates a intense blur image
		medianBlur(src_img, median_img, medianBlurSize);

		//Convert the captured frame from BGR to HSV
		cvtColor(median_img, hsv_img, COLOR_BGR2HSV);
		inRange(hsv_img, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), thresh_img); //Threshold the image

		//Modify image...
		//morphological opening (remove small objects from the foreground)
		erode(thresh_img, thresh_img, getStructuringElement(MORPH_ELLIPSE, erodeSize) );
		dilate( thresh_img, thresh_img, getStructuringElement(MORPH_ELLIPSE, dilateSize) );

		//morphological closing (fill small holes in the foreground)
		dilate( thresh_img, thresh_img, getStructuringElement(MORPH_ELLIPSE, erodeSize) );
		erode(thresh_img, thresh_img, getStructuringElement(MORPH_ELLIPSE, dilateSize) );

		//showWindow(thresh_window, src_thresh);
	}

	/*
	 * Function for showing widows
	 */
	void QRDetector::showWindow(string name, Mat image){
		  // Create Window
		if(!debugFlag) return;
		window_posx = window_posx+25;
		window_posy = window_posy+25;

		string source_window = name;
		imshow( source_window, image );
	}




	// Function: Routine to get Distance between two points
	// Description: Given 2 points, the function returns the distance

	float QRDetector::cv_distance(Point2f P, Point2f Q)
	{
		return sqrt(pow(abs(P.x - Q.x),2) + pow(abs(P.y - Q.y),2)) ;
	}


	// Function: Perpendicular Distance of a Point J from line formed by Points L and M; Equation of the line ax+by+c=0
	// Description: Given 3 points, the function derives the line quation of the first two points,
	//	  calculates and returns the perpendicular distance of the the 3rd point from this line.

	float QRDetector::cv_lineEquation(Point2f L, Point2f M, Point2f J)
	{
		float a,b,c,pdist;

		a = -((M.y - L.y) / (M.x - L.x));
		b = 1.0;
		c = (((M.y - L.y) /(M.x - L.x)) * L.x) - L.y;

		// Now that we have a, b, c from the equation ax + by + c, time to substitute (x,y) by values from the Point J

		pdist = (a * J.x + (b * J.y) + c) / sqrt((a * a) + (b * b));
		return pdist;
	}

	// Function: Slope of a line by two Points L and M on it; Slope of line, S = (x1 -x2) / (y1- y2)
	// Description: Function returns the slope of the line formed by given 2 points, the alignement flag
	//	  indicates the line is vertical and the slope is infinity.

	float QRDetector::cv_lineSlope(Point2f L, Point2f M, int& alignement)
	{
		float dx,dy;
		dx = M.x - L.x;
		dy = M.y - L.y;

		if ( dy != 0)
		{
			alignement = 1;
			return (dy / dx);
		}
		else				// Make sure we are not dividing by zero; so use 'alignement' flag
		{
			alignement = 0;
			return 0.0;
		}
	}



	// Function: Routine to calculate 4 Corners of the Marker in Image Space using Region partitioning
	// Theory: OpenCV Contours stores all points that describe it and these points lie the perimeter of the polygon.
	//	The below function chooses the farthest points of the polygon since they form the vertices of that polygon,
	//	exactly the points we are looking for. To choose the farthest point, the polygon is divided/partitioned into
	//	4 regions equal regions using bounding box. Distance algorithm is applied between the centre of bounding box
	//	every contour point in that region, the farthest point is deemed as the vertex of that region. Calculating
	//	for all 4 regions we obtain the 4 corners of the polygon ( - quadrilateral).
	void QRDetector::cv_getVertices(vector<vector<Point> > contours, int c_id, float slope, vector<Point2f>& quad)
	{
		Rect box;
		box = boundingRect( contours[c_id]);

		Point2f M0,M1,M2,M3;
		Point2f A, B, C, D, W, X, Y, Z;

		A =  box.tl();
		B.x = box.br().x;
		B.y = box.tl().y;
		C = box.br();
		D.x = box.tl().x;
		D.y = box.br().y;


		W.x = (A.x + B.x) / 2;
		W.y = A.y;

		X.x = B.x;
		X.y = (B.y + C.y) / 2;

		Y.x = (C.x + D.x) / 2;
		Y.y = C.y;

		Z.x = D.x;
		Z.y = (D.y + A.y) / 2;

		float dmax[4];
		dmax[0]=0.0;
		dmax[1]=0.0;
		dmax[2]=0.0;
		dmax[3]=0.0;

		float pd1 = 0.0;
		float pd2 = 0.0;

		if (slope > 5 || slope < -5 )
		{

			for( int i = 0; i < contours[c_id].size(); i++ )
			{
			pd1 = cv_lineEquation(C,A,contours[c_id][i]);	// Position of point w.r.t the diagonal AC
			pd2 = cv_lineEquation(B,D,contours[c_id][i]);	// Position of point w.r.t the diagonal BD

			if((pd1 >= 0.0) && (pd2 > 0.0))
			{
				cv_updateCorner(contours[c_id][i],W,dmax[1],M1);
			}
			else if((pd1 > 0.0) && (pd2 <= 0.0))
			{
				cv_updateCorner(contours[c_id][i],X,dmax[2],M2);
			}
			else if((pd1 <= 0.0) && (pd2 < 0.0))
			{
				cv_updateCorner(contours[c_id][i],Y,dmax[3],M3);
			}
			else if((pd1 < 0.0) && (pd2 >= 0.0))
			{
				cv_updateCorner(contours[c_id][i],Z,dmax[0],M0);
			}
			else
				continue;
				 }
		}
		else
		{
			int halfx = (A.x + B.x) / 2;
			int halfy = (A.y + D.y) / 2;

			for( int i = 0; i < contours[c_id].size(); i++ )
			{
				if((contours[c_id][i].x < halfx) && (contours[c_id][i].y <= halfy))
				{
					cv_updateCorner(contours[c_id][i],C,dmax[2],M0);
				}
				else if((contours[c_id][i].x >= halfx) && (contours[c_id][i].y < halfy))
				{
					cv_updateCorner(contours[c_id][i],D,dmax[3],M1);
				}
				else if((contours[c_id][i].x > halfx) && (contours[c_id][i].y >= halfy))
				{
					cv_updateCorner(contours[c_id][i],A,dmax[0],M2);
				}
				else if((contours[c_id][i].x <= halfx) && (contours[c_id][i].y > halfy))
				{
					cv_updateCorner(contours[c_id][i],B,dmax[1],M3);
				}
				}
		}

		quad.push_back(M0);
		quad.push_back(M1);
		quad.push_back(M2);
		quad.push_back(M3);

	}

	// Function: Compare a point if it more far than previously recorded farthest distance
	// Description: Farthest Point detection using reference point and baseline distance
	void QRDetector::cv_updateCorner(Point2f P, Point2f ref , float& baseline,  Point2f& corner)
	{
		float temp_dist;
		temp_dist = cv_distance(P,ref);

		if(temp_dist > baseline)
		{
			baseline = temp_dist;			// The farthest distance is the new baseline
			corner = P;						// P is now the farthest point
		}

	}

	// Function: Sequence the Corners wrt to the orientation of the QR Code
	void QRDetector::cv_updateCornerOr(int orientation, vector<Point2f> IN,vector<Point2f> &OUT)
	{
		Point2f M0,M1,M2,M3;
			if(orientation == QR_NORTH)
		{
			M0 = IN[0];
			M1 = IN[1];
			M2 = IN[2];
			M3 = IN[3];
		}
		else if (orientation == QR_EAST)
		{
			M0 = IN[1];
			M1 = IN[2];
			M2 = IN[3];
			M3 = IN[0];
		}
		else if (orientation == QR_SOUTH)
		{
			M0 = IN[2];
			M1 = IN[3];
			M2 = IN[0];
			M3 = IN[1];
		}
		else if (orientation == QR_WEST)
		{
			M0 = IN[3];
			M1 = IN[0];
			M2 = IN[1];
			M3 = IN[2];
		}

		OUT.push_back(M0);
		OUT.push_back(M1);
		OUT.push_back(M2);
		OUT.push_back(M3);
	}

	// Function: Get the Intersection Point of the lines formed by sets of two points
	bool QRDetector::getIntersectionPoint(Point2f a1, Point2f a2, Point2f b1, Point2f b2, Point2f& intersection)
	{
		Point2f p = a1;
		Point2f q = b1;
		Point2f r(a2-a1);
		Point2f s(b2-b1);

		if(cross(r,s) == 0) {return false;}

		float t = cross(q-p,s)/cross(r,s);

		intersection = p + t*r;
		return true;
	}

	float QRDetector::cross(Point2f v1,Point2f v2)
	{
		return v1.x*v2.y - v1.y*v2.x;
	}

	void QRDetector::rotate90(cv::Mat &matImage, int rotflag){
	  //1=CW, 2=CCW, 3=180
	  if (rotflag == 1){
		transpose(matImage, matImage);
		flip(matImage, matImage,1); //transpose+flip(1)=CW
	  } else if (rotflag == 2) {
		transpose(matImage, matImage);
		flip(matImage, matImage,0); //transpose+flip(0)=CCW
	  } else if (rotflag ==3){
		flip(matImage, matImage,-1);    //flip(-1)=180
	  } else if (rotflag != 0){ //if not 0,1,2,3:
		cout  << "Unknown rotation flag(" << rotflag << ")" << endl;
	  }
	}






	// The MIT License (MIT)
	// Copyright (c) 2015 tomykaira
	//
	// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
	//
	// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
	//
	// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

	// reference http://www.ipol.im/pub/art/2011/llmps-scb/
	void QRDetector::balance_white(cv::Mat mat) {
	  double discard_ratio = 0.05;
	  int hists[3][256];
	  memset(hists, 0, 3*256*sizeof(int));

	  for (int y = 0; y < mat.rows; ++y) {
	    uchar* ptr = mat.ptr<uchar>(y);
	    for (int x = 0; x < mat.cols; ++x) {
	      for (int j = 0; j < 3; ++j) {
	        hists[j][ptr[x * 3 + j]] += 1;
	      }
	    }
	  }

	  // cumulative hist
	  int total = mat.cols*mat.rows;
	  int vmin[3], vmax[3];
	  for (int i = 0; i < 3; ++i) {
	    for (int j = 0; j < 255; ++j) {
	      hists[i][j + 1] += hists[i][j];
	    }
	    vmin[i] = 0;
	    vmax[i] = 255;
	    while (hists[i][vmin[i]] < discard_ratio * total)
	      vmin[i] += 1;
	    while (hists[i][vmax[i]] > (1 - discard_ratio) * total)
	      vmax[i] -= 1;
	    if (vmax[i] < 255 - 1)
	      vmax[i] += 1;
	  }


	  for (int y = 0; y < mat.rows; ++y) {
	    uchar* ptr = mat.ptr<uchar>(y);
	    for (int x = 0; x < mat.cols; ++x) {
	      for (int j = 0; j < 3; ++j) {
	        int val = ptr[x * 3 + j];
	        if (val < vmin[j])
	          val = vmin[j];
	        if (val > vmax[j])
	          val = vmax[j];
	        ptr[x * 3 + j] = static_cast<uchar>((val - vmin[j]) * 255.0 / (vmax[j] - vmin[j]));
	      }
	    }
	  }
	}
