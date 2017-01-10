#include "OpenCVWindow.h"
#include "ui_OpenCVWindow.h"
#include <QMessageBox>
#include <vector>
#include <QPalette>
#include <QDebug>
#include "WorkTable.h"
#include <stdlib.h>
#include <QString>



#define VIDEO_ID  1

#define H_MAX 136
#define H_MIN 67
#define S_MAX 175
#define S_MIN 92
#define V_MAX 248
#define V_MIN 82
#define SENSITIVITY_VALUE 20
#define BLUR_SIZE 10

const int MAX_NUM_OBJECTS=5;
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;

using std::vector;
using namespace cv;

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
	connect (repeatTimer, SIGNAL(timeout()), this, SLOT(videoProcessTracking()));
	repeatTimer->start(40);

	//	readFileTimer = new QTimer(this);
	//	connect(readFileTimer, SIGNAL(timeout()), this, SLOT(readText()) );
	//	readFileTimer->start(500);

	//	eularAngleText = new QString();

	stepped = 0;
	getComp1 = false;
	getComp2 = false;
	getComp3 = false;
	getComp4 = false;
	getTool1 = false;
	getTool1 = false;
	returnTool1 = false;
	returnTool2 = false;
	moveFlag = 0;
	moveTicker = 0;
}

bool OpenCVWindow::getStatus(int n){
	switch(n){
	case 0:
		return false;
		break;
	case 1:
		return getComp1;
		break;
	case 2:
		return getComp2;
		break;
	case 3:
		return getComp3;
		break;
	case 4:
		return getComp4;
		break;
	case 5:
		return returnTool1;
		break;
	case 6:
		return returnTool2;
		break;
	default:
		return false;
	}

}

void OpenCVWindow::setStaut(int n) {
	switch(n){
	case 0:
		break;
	case 1:
		getComp1 = false;
		break;
	case 2:
		getComp2 = false;
		break;
	case 3:
		getComp3 = false;
		break;
	case 4:
		getComp4 = false;
		break;
	case 5:
		returnTool1 = false;
		break;
	case 6:
		returnTool2 = false;
		break;
	}

	returnTool2 = false;
//	if (n = )
}

OpenCVWindow::~OpenCVWindow(){
	delete ui;
}

/* use the pure tracking function
 * */
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
	//	trackFilteredObject(xPos, yPos, thresholdImage, frame1);

	// SHOW IMAGE;
	cv::cvtColor(frame1, frame1,cv::COLOR_BGR2RGB);
	QImage qimgOriginal((uchar*)frame1.data, frame1.cols, frame1.rows, frame1.step, QImage::Format_RGB888);
	QImage qimgProcessed((uchar*)differenceImage.data, differenceImage.cols, differenceImage.rows, differenceImage.step, QImage::Format_Indexed8);

	ui->labelOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));
	ui->labelProcessed->setPixmap(QPixmap::fromImage(qimgProcessed));
}



void OpenCVWindow::videoProcessTracking(){
	bool trackObjects = true;
	bool useMorphOps = true;

	inputVideo.read(frame1);

	cv::cvtColor(frame1, HSV1, cv::COLOR_BGR2HSV);

	cv::inRange(HSV1, Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),grayImage1);

	morphOps(grayImage1);
	trackFilteredObject(xPos,yPos,grayImage1,frame1);

	setReachStatus();

	cv::cvtColor(frame1, frame1,cv::COLOR_BGR2RGB);
	QImage qimgOriginal((uchar*)frame1.data, frame1.cols, frame1.rows, frame1.step, QImage::Format_RGB888);
	QImage qimgProcessed((uchar*)grayImage1.data, grayImage1.cols, grayImage1.rows, grayImage1.step, QImage::Format_Indexed8);

	ui->labelOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));
	ui->labelProcessed->setPixmap(QPixmap::fromImage(qimgProcessed));
}


void OpenCVWindow::readText(){
	inFile.open("/home/semiartist/Desktop/amir_libximu/build/eular_angle.txt");
	if(inFile.is_open()) std::getline(inFile, line);
	else QMessageBox::warning(this, "File Does Not Exist", "Can not open file");

	eularAngle = std::stod(line);
	eularAngleText = QString::number(eularAngle);

	ui->eularLabel->setText(eularAngleText);

	//	qDebug() << eularAngle << "<- double || string->" <<  eularAngleText << '\n';

	qDebug() << "stepped ->" << stepped << '\n';
	qDebug() << "angle   ->" << eularAngle << '\n';
	inFile.close();

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
	//	qDebug() << objectBoundingRectangle.width << "<- width || height ->" << objectBoundingRectangle.height << '\n';
	if (objectBoundingRectangle.width > 25 || objectBoundingRectangle.height > 25){
		cv::circle(cameraFeed,cv::Point(x,y),20,cv::Scalar(0,255,0),2);
		cv::line(cameraFeed,cv::Point(x,y),cv::Point(x,y-25),cv::Scalar(0,255,0),2);
		cv::line(cameraFeed,cv::Point(x,y),cv::Point(x,y+25),cv::Scalar(0,255,0),2);
		cv::line(cameraFeed,cv::Point(x,y),cv::Point(x-25,y),cv::Scalar(0,255,0),2);
		cv::line(cameraFeed,cv::Point(x,y),cv::Point(x+25,y),cv::Scalar(0,255,0),2);
		cv::putText(cameraFeed,"Tracking object at (" + intToString(x)+","+intToString(y)+")",cv::Point(x,y),1,1,cv::Scalar(255,0,0),2);

		trackLocation(x, y);
		setStep(x,y);
	}

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
	if (xPos > 200 && yPos > 150 && std::abs(eularAngle) > 150 && stepped == 0){
		//		qDebug() << "more than threshold!";

		ui->textEditFlag->setPalette(QPalette(Qt::green));
		table->nextStep();
		stepped = 1;
	} else {
		ui->textEditFlag->setPalette(QPalette(Qt::red));
	}
}

void OpenCVWindow::setStep(int x, int y){
	if (x < 20) {
		stepped = 0;
	}
	if (y < 20) stepped = 0;

	//	qDebug() << "stepped -> " << stepped << '\n';
}

void OpenCVWindow::drawObject(int x, int y, Mat & frame){
	cv::circle(frame,Point(x,y),20,Scalar(0,255,0),2);
	//	if(y-25>0)
	//		cv::line(frame,cv::Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
	//	else cv::line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
	//	if(y+25<FRAME_HEIGHT)
	//		cv::line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
	//	else cv::line(frame,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
	//	if(x-25>0)
	//		line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
	//	else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
	//	if(x+25<FRAME_WIDTH)
	//		line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
	//	else line(frame,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);

	putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);

}

void OpenCVWindow::setReachStatus() {
	int number = 0;
	int x1 = 520;
	int y1 = 208;
	int x2 = 456;
	int y2 = 209;
	int x3 = 389;
	int y3 = 206;
	int x4 = 308;
	int y4 = 175;
	int x5 = 252;
	int y5 = 167;
	int x6 = 215;
	int y6 = 158;

	if(xPos< x1 + 10 && xPos > x1 -10 && yPos > y1-10 && yPos < y1+10) number = 1;
	if(xPos< x2 + 10 && xPos > x2 -10 && yPos > y2-10 && yPos < y2+10) number = 2;
	if(xPos< x3 + 10 && xPos > x3 -10 && yPos > y3-10 && yPos < y3+10) number = 3;
	if(xPos< x4 + 10 && xPos > x4 -10 && yPos > y4-10 && yPos < y4+10) number = 4;
	if(xPos< x5 + 10 && xPos > x5 -30 && yPos > y5-20 && yPos < y5+20) number = 5;
	if(xPos< x6 + 10 && xPos > x6 -10 && yPos > y6-20 && yPos < y6+20) number = 6;
	//	if(xPos<=50 && yPos <= 50) number = 99;
	//	if(xPos <=50 && yPos >= 580) number = 88;

	if (moveFlag == number && number!=0) {
		++moveTicker;
	}
	else {
		moveTicker = 0;
		moveFlag = number;
		moveOut = true;

		return;
	}
	table->stepInfo(moveFlag, moveTicker);

	if (moveTicker > 7){
		switch(number) {
		case 1:
			getComp1 = true;
			moveOut = false;
			break;
		case 2:
			getComp2 = true;
			moveOut = false;
			break;
		case 3:
			getComp3 = true;
			moveOut = false;
			break;
		case 4:
			getComp4 = true;
			moveOut = false;
			break;
		case 5:
			if (getTool1 && moveOut){
				returnTool1 = true;
				moveOut = false;
				break;
			}
			getTool1  = true;
			moveOut = false;
			break;
		case 6:
			if (getTool2 && moveOut){
				returnTool2 = true;
				moveOut = false;
				break;
			}
			getTool2 = true;
			moveOut = false;
			break;

		}

	}

	qDebug() << "Move Flag -> " << moveFlag;
	qDebug() << "Ticker-> " << moveTicker;

}

void OpenCVWindow::trackFilteredObject(int &x, int &y, cv::Mat threshold, cv::Mat &cameraFeed){
	cv::Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );// retrieves external contours
	//use moments method to find our filtered object

	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
					x = moment.m10/area;
					y = moment.m01/area;
					objectFound = true;
					refArea = area;
				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
				putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
				//draw object location on screen
				drawObject(x,y,cameraFeed);}

		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}

void OpenCVWindow::morphOps(cv::Mat &thresh){
	cv::Mat erodeElement = cv::getStructuringElement( MORPH_RECT,Size(3,3));
	//dilate with larger element so make sure object is nicely visible
	cv::Mat dilateElement = cv::getStructuringElement( MORPH_RECT,Size(8,8));

	cv::erode(thresh,thresh,erodeElement);
	cv::erode(thresh,thresh,erodeElement);

	cv::dilate(thresh,thresh,dilateElement);
	cv::dilate(thresh,thresh,dilateElement);
}
