#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QImage>
#include <QTimer>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();


public slots:
	void processFrameAndUpdateGUI();


private slots:
	void on_mainButton_clicked();

private:
	Ui::Dialog *ui;

	cv::VideoCapture capWebcam;

	cv::Mat matOriginal;
	cv::Mat matProcessed;

	QImage qimgOriginal;
	QImage qimgProcessed;

	std::vector <cv::Vec3f> vecCircles;
	std::vector<cv::Vec3f>::iterator itrCircles;

	QTimer *tmrTimer;

};

#endif // DIALOG_H
