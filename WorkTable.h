#ifndef WORKTABLE
#define WORKTABLE

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QString>
#include <QList>
#include "ApplicationArea.h"
#include <QObject>
#include "TextDisplay.h"
#include "PhotoDisplay.h"
#include <QImage>
#include <opencv2/highgui/highgui.hpp>
#include <QPointF>
#include <QMouseEvent>
#include "OpenCVWindow.h"

class WorkTable: public QGraphicsView{
	 Q_OBJECT
public:
	// constructor;
	WorkTable(int w, int h, QWidget *parent = NULL);

	// instance variables;
	QGraphicsScene *_scene;
	int _step;
	int _totalStep;
	QString _wi;
	QList<QString *> _wiList;
	QList<QPointF> _componentBoxPositionList;
	int _newComponentPosition;
	ApplicationArea *_workingArea;
	PhotoDisplay *_videoArea;
	TextDisplay *_textArea;

	// public functions:
	void next();
	void goToStep(int step);
	void drawGUI();
	void displayMainMenu();
	void mousePressEvent(QMouseEvent *event);

	// functions for opencv
	void setImage(QImage im);


protected :
    void paintEvent(QPaintEvent* paintEventInfo);


public slots:
	void deHighLightAll();
	void startWork();
	void nextStep();
	void updateView();
	void addNewComponentBox();
	void showCVDetail();
	void temp();

private:
	QImage _image;
	int _width;
	int _height;
	int entered;
	OpenCVWindow *newDialog;
	// cv::VideoCapture video;
	// cv::Mat frame;

};

#endif // WORKTABLE

