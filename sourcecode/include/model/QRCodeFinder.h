/* 
 * File:   QRCodeFinder.h
 * Author: Pieter Loubser
 *
 * Created on February 7, 2016, 11:34 AM
 */

#ifndef QRCODEFINDER_H
#define	QRCODEFINDER_H
#include <opencv2/opencv.hpp>
#include <zbar.h>
#include <iostream>  
#include "../BasicFunctions.h"
#include "qrcode.h"
using namespace cv;  
using namespace std;  
using namespace zbar;

#ifdef	__cplusplus
extern "C" {
#endif

    vector<QRCode> getQRCodes(Mat image);


#ifdef	__cplusplus
}
#endif

#endif	/* QRCODEFINDER_H */

