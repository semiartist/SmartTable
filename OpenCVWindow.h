#ifndef OPENCVWINDOW_H
#define OPENCVWINDOW_H

#include <QDialog>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <QImage>
#include <QTimer>
#include <string>

namespace Ui {
class OpenCVWindow;
}

class OpenCVWindow : public QDialog
{
	Q_OBJECT

public:
	explicit OpenCVWindow(QWidget *parent = 0);
	~OpenCVWindow();

private slots:
	void videoProcess();

private:
	Ui::OpenCVWindow *ui;
	cv::VideoCapture inputVideo;

	cv::Mat frame1, frame2;
	cv::Mat HSV1, HSV2;
	cv::Mat grayImage1, grayImage2;
	cv::Mat differenceImage, thresholdImage;

	QImage *original;
	QImage *processed;

	QTimer *repeatTimer;

	int xPos, yPos;
	// private functions
	void furtherImageProcess(cv::Mat &input);
	void findMovement(cv::Mat thresholdImage, cv::Mat &cameraFeed);
	std::string intToString(int number);
	void trackLocation(int xPos, int yPos);

};

#endif // OPENCVWINDOW_H
