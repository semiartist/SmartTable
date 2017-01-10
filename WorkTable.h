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
	void next(int);
	void goToStep(int step);
	void drawGUI();
	void displayMainMenu();
//	void mousePressEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void highBackground(int);

	// functions for opencv
	void setImage(QImage im);
	void stepInfo(int a, int b);



protected :
    void paintEvent(QPaintEvent* paintEventInfo);


public slots:
	void deHighLightAll();
	void startWork();
	void nextStep();
	void preStep();
	void updateView();
//	void addNewComponentBox();
	void showCVDetail();
	void temp();

private slots:
	void stepControl();

private:
	QImage _image;
	int _width;
	int _height;
	int entered;
	int component;
	int frameNumber;
	bool stepped;

	OpenCVWindow *newDialog;
	QTimer *stepTimer;
	QTimer *highlightTimer;
	ApplicationArea *componentBox1;
	ApplicationArea *componentBox2;
	ApplicationArea *componentBox3;
	ApplicationArea *componentBox4;

	ApplicationArea *toolBox1;
	ApplicationArea *toolBox2;

	// cv::VideoCapture video;
	// cv::Mat frame;

};

#endif // WORKTABLE

