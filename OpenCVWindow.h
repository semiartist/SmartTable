#ifndef OPENCVWINDOW_H
#define OPENCVWINDOW_H

#include <QDialog>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv/cv.h"
#include <QImage>
#include <QTimer>
#include <string>
#include <fstream>

namespace Ui {
class OpenCVWindow;
}

class OpenCVWindow : public QDialog
{
	Q_OBJECT

public:
	explicit OpenCVWindow(QWidget *parent = 0);
	bool getStatus(int n);
	void setStaut(int n);
	~OpenCVWindow();

private slots:
	void videoProcess();
	void videoProcessTracking();
	void readText();

private:
	Ui::OpenCVWindow *ui;
	cv::VideoCapture inputVideo;

	cv::Mat frame1, frame2;
	cv::Mat HSV1, HSV2;
	cv::Mat grayImage1, grayImage2;
	cv::Mat differenceImage, thresholdImage;
	std::string line;
	std::ifstream inFile;

	bool getComp1, getComp2, getComp3, getComp4;
	bool getTool1, getTool2, returnTool1, returnTool2;
	bool moveOut;
	int moveFlag, moveTicker;

	QString eularAngleText;
	QImage *original;
	QImage *processed;
	QTimer *readFileTimer;
	QTimer *repeatTimer;
	double eularAngle;

	int xPos, yPos;
	int stepped;
	// private functions
	void furtherImageProcess(cv::Mat &input);
	void findMovement(cv::Mat thresholdImage, cv::Mat &cameraFeed);
	std::string intToString(int number);
	void trackLocation(int xPos, int yPos);
	void setStep(int, int);
	void drawObject(int, int, cv::Mat &);
	void setReachStatus();

	void trackFilteredObject(int&x, int&y, cv::Mat threshold, cv::Mat & cameraFeed);
	void morphOps(cv::Mat &);

};

#endif // OPENCVWINDOW_H
