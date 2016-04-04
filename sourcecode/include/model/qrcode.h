/*
 * QRCode
 * By: Peter Corcoran
 * Copyright: © 2015
 *
 * Jun 25, 2015 - Initial Coding
 *
 */

#ifndef INCLUDE_QRCODE_H_
#define INCLUDE_QRCODE_H_
#include <opencv2/opencv.hpp>
#include "../enums.h"

namespace blaze {

/*
 * QR Code Class Header that defines the behavior for QR Codes
 *
 * author Peter
 */
class QRCode {
public:
	QRCode();
	QRCode(Rect rect, Color color);

	~QRCode();

	string getColorStr();

	double getArea();

	const Point2D getCenter();

	const Point2D getCenterDst();

	Color getColor();

	void setColor(Color color);

	Rect getRectangle();

	void setRectangle(Rect rectangle);

	void setCenterDst(Point2D centerDst);

private:
	Color color;
	Rect rectangle;
	Point2D center;
	Point2D centerDst;
	double area;

};

}
#endif /* INCLUDE_QRCODE_H_ */
