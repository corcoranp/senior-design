/* 
 * File:   QRCodeFinder.cpp
 * Author: Pieter Loubser
 * 
 * Created on February 7, 2016, 11:34 AM
 */
#include "../../include/model/QRCodeFinder.h"


bool sortBlocks(QRCode a, QRCode b){
	return a.center.y < b.center.y;
}


vector<QRCode> getQRCodes(Mat image){
    vector <QRCode> QRList;
    ImageScanner scanner;
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
    Mat grey;
    cvtColor(image, grey, CV_BGR2GRAY);
    int width = image.cols;
    int height = image.rows;
    uchar *raw = (uchar *) grey.data;
    // wrap image data   
    Image img(width, height, "Y800", raw, width * height);
    // scan the image for barcodes   
    int n = scanner.scan(img);
    if(n == 0){
        return QRList;
    }
    for (Image::SymbolIterator symbol = img.symbol_begin();
            symbol != img.symbol_end();
            ++symbol) {
        QRCode tmp;
        vector<Point> vp;
        int size = symbol->get_location_size();
        for (int i = 0; i < size; i++) {
            vp.push_back(Point(
                                symbol->get_location_x(i), 
                                symbol->get_location_y(i)));
        }
        RotatedRect r = minAreaRect(vp);
        Point2f pts[4];
        r.points(pts);
        cout << "x: " << std::dec << r.boundingRect().x;
        cout << " y: " << std::dec << r.boundingRect().y << endl;
        tmp.setRectangle(Rect(
                r.boundingRect().x, 
                r.boundingRect().y, 
                r.boundingRect().width, 
                r.boundingRect().height));
        if(string("green") == symbol->get_data()){
            tmp.setColor(GREEN);
        } else if(string("red") == symbol->get_data()){
            tmp.setColor(RED);
        } else if(string("blue") == symbol->get_data()){
            tmp.setColor(BLUE);
        } else if(string("yellow") == symbol->get_data()){
            tmp.setColor(YELLOW);
        }
        QRList.push_back(tmp);
    }
    
    sort(QRList.begin(), QRList.end(), sortBlocks);
    return QRList;
}
