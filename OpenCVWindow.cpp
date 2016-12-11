#include "OpenCVWindow.h"
#include "ui_OpenCVWindow.h"
#include <QMessageBox>
#include <vector>
#include <QPalette>
#include <QDebug>
#include "WorkTable.h"

#define VIDEO_ID  1

#define H_MAX 136
#define H_MIN 102
#define S_MAX 237
#define S_MIN 72
#define V_MAX 253
#define V_MIN 114
#define SENSITIVITY_VALUE 20
#define BLUR_SIZE 10

extern WorkTable *table;

OpenCVWindow::OpenCVWindow(QWidget *parent) : QDialog(parent), ui(new Ui::OpenCVWindow){
	ui->setupUi(this);

	xPos = 0;
	yPos = 0;

	// below are the real deal;

	inputVideo.open(VIDEO_ID);

	if(!inputVideo.isOpened()) {
		QMessageBox::warning(this, "VIDEO IS NOT OPENED", "Your video is not opened");
		return;
	}

	repeatTimer = new QTimer(this);
	connect (repeatTimer, SIGNAL(timeout()), this, SLOT(videoProcess()));
	repeatTimer->start(40);
}

OpenCVWindow::~OpenCVWindow(){
	delete ui;
}

void OpenCVWindow::videoProcess(){
	inputVideo.read(frame1);
	cv::cvtColor(frame1,HSV1, cv::COLOR_BGR2HSV);
	cv::inRange(HSV1, cv::Scalar(H_MIN, S_MIN, V_MIN), cv::Scalar(H_MAX, S_MAX, V_MAX), grayImage1);
	furtherImageProcess(grayImage1);
	inputVideo.read(frame2);
	cv::cvtColor(frame2, HSV2, cv::COLOR_BGR2HSV);
	cv::inRange(HSV2, cv::Scalar(H_MIN, S_MIN, V_MIN), cv::Scalar(H_MAX, S_MAX, V_MAX), grayImage2);
	furtherImageProcess(grayImage2);

	cv::absdiff(grayImage1, grayImage2, differenceImage);
	cv::threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, cv::THRESH_BINARY);
	cv::blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE));
	cv::threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, cv::THRESH_BINARY);

	findMovement(thresholdImage, frame1);

	// SHOW IMAGE;
	cv::cvtColor(frame1, frame1,cv::COLOR_BGR2RGB);
	QImage qimgOriginal((uchar*)frame1.data, frame1.cols, frame1.rows, frame1.step, QImage::Format_RGB888);
	QImage qimgProcessed((uchar*)differenceImage.data, differenceImage.cols, differenceImage.rows, differenceImage.step, QImage::Format_Indexed8);

	ui->labelOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));
	ui->labelProcessed->setPixmap(QPixmap::fromImage(qimgProcessed));


}

void OpenCVWindow::furtherImageProcess(cv::Mat &input) {
	cv::Mat erodeImage = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
	cv::Mat dilateImage = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(8,8));

	cv::erode(input,input, erodeImage);
	cv::dilate(input, input, dilateImage);
}

void OpenCVWindow::findMovement(cv::Mat thresholdImage, cv::Mat &cameraFeed){
	bool objectDetected=false;
	cv::Mat temp;
	cv::Rect objectBoundingRectangle = cv::Rect(0,0,0,0);
	int theObject[2] = {0,0};

	thresholdImage.copyTo(temp);

	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );// retrieves external contours

	//if contours vector is not empty, we have found some objects
	if(contours.size()>0)objectDetected=true;
	else objectDetected = false;

	if(objectDetected){
		std::vector< std::vector<cv::Point> > largestContourVec;
		largestContourVec.push_back(contours.at(contours.size()-1));
		objectBoundingRectangle = cv::boundingRect(largestContourVec.at(0));
		int xpos = objectBoundingRectangle.x+objectBoundingRectangle.width/2;
		int ypos = objectBoundingRectangle.y+objectBoundingRectangle.height/2;

		//update the objects positions by changing the 'theObject' array values
		theObject[0] = xpos , theObject[1] = ypos;
	}
	//make some temp x and y variables so we dont have to type out so much
	int x = theObject[0];
	int y = theObject[1];
	//draw some crosshairs on the object
	cv::circle(cameraFeed,cv::Point(x,y),20,cv::Scalar(0,255,0),2);
	cv::line(cameraFeed,cv::Point(x,y),cv::Point(x,y-25),cv::Scalar(0,255,0),2);
	cv::line(cameraFeed,cv::Point(x,y),cv::Point(x,y+25),cv::Scalar(0,255,0),2);
	cv::line(cameraFeed,cv::Point(x,y),cv::Point(x-25,y),cv::Scalar(0,255,0),2);
	cv::line(cameraFeed,cv::Point(x,y),cv::Point(x+25,y),cv::Scalar(0,255,0),2);
	cv::putText(cameraFeed,"Tracking object at (" + intToString(x)+","+intToString(y)+")",cv::Point(x,y),1,1,cv::Scalar(255,0,0),2);


	trackLocation(x, y);


}

std::string OpenCVWindow::intToString(int number){

	//this function has a number input and string output
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void OpenCVWindow::trackLocation(int x, int y) {
	xPos = x;
	yPos = y;
	if (xPos > 200 && yPos > 150){
		qDebug() << "more than threshold!";
		ui->textEditFlag->setPalette(QPalette(Qt::green));
		table->nextStep();
	} else {
		ui->textEditFlag->setPalette(QPalette(Qt::red));
	}
}
