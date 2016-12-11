#include "WorkTable.h"
#include "Button.h"
#include <QDebug>
#include <QString>
#include "ApplicationArea.h"
#include "WorkTable.h"
#include <QTimer>
#include <string>
#include <stdlib.h>
#include <QBrush>
#include <QGraphicsRectItem>
#include "Dialog.h"
#include "OpenCVWindow.h"

WorkTable::WorkTable(int w, int h, QWidget *parent): _width(w-40), _height(h-40){
	// set view
	setFixedSize(_width, _height);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// set scene;
	_scene = new QGraphicsScene();
	_scene->setSceneRect(0, 0, _width, _height);
	setScene(_scene);

	// setup some parameters;

	// initilize some variables;
	_step = 0;
	_totalStep = 3;
	_wi = QString("Ready to Start!");
	_newComponentPosition = 0;
	entered = 0;

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateView()));

}

void WorkTable::next(){
	_step++;
	_wi = *_wiList[_step];

}

void WorkTable::goToStep(int step){
	if (step <= _totalStep){
		_step = step;
	}
}

void WorkTable::drawGUI() {
	// parameter for working area size;
	double WORKING_AREA_PORTION = 0.7;

	// create objects
	QGraphicsRectItem *backBlack = new QGraphicsRectItem(0, 0, _width, _height);
	QBrush blackBlock;
	blackBlock.setColor(Qt::black);
	blackBlock.setStyle(Qt::SolidPattern);
	backBlack->setBrush(blackBlock);


	_workingArea = new ApplicationArea(0+25, height()*(1-WORKING_AREA_PORTION)-25, width()*WORKING_AREA_PORTION,
									   height()*WORKING_AREA_PORTION, QString("Working Area"));
	_textArea = new TextDisplay(775, 400, 200, 150, QString("Working Instruction"));
	_videoArea = new PhotoDisplay(775, 200, 200, 150, QString("Instruction VIdeo"));

	_scene->addItem(backBlack);
	_scene->addItem(_workingArea);
	_scene->addItem(_videoArea);
	_scene->addItem(_textArea);

	int buttonX = rect().width()-30;
	int buttonY = 20;

	// create exit button
	Button *exitButton = new Button(QString("EXIT"), 100, Qt::red);
	exitButton->setPos(buttonX -100, buttonY);
	connect(exitButton,SIGNAL(clicked()), this, SLOT(close()) );
	_scene->addItem(exitButton);

	// create next button
	Button *nextButton = new Button(QString("NEXT"), 100, Qt::darkMagenta);
	nextButton->setPos(_workingArea->boundingRect().width()-110+25, height()-70);
	connect(nextButton, SIGNAL(clicked()), this, SLOT(nextStep()) );
	_scene->addItem(nextButton);

	// create previous button
	Button *preButton = new Button(QString("PREV"), 100, Qt::blue);
	preButton->setPos(35, height()-70);
	connect(preButton, SIGNAL(clicked()), _workingArea, SLOT(deHighLight()) );
	_scene->addItem(preButton);

	// create build component button
	Button *addNewComponentButton = new Button(QString("ADD COMP"), 100, Qt::blue);
	addNewComponentButton->setPos(buttonX-250, buttonY);
	connect(addNewComponentButton, SIGNAL(clicked()), this, SLOT(addNewComponentBox()) );
	_scene->addItem(addNewComponentButton);

	for(int i = 0;i< _componentBoxPositionList.size(); i++ ){
		QString num = QString::number(i+1);
		QString name = QString("Component ") + num;
		// qDebug() << name;
		// qDebug() << name << _componentBoxPositionList[i].x() << " " << _componentBoxPositionList[i].y();
		ApplicationArea *componentBox = new ApplicationArea(_componentBoxPositionList[i].x(), _componentBoxPositionList[i].y(), 120, 100, name);
		_scene->addItem(componentBox);
	}

	entered = 1;
}

void WorkTable::displayMainMenu() {
	// create the main menu
	QGraphicsTextItem *title = new QGraphicsTextItem (QString("Intelligent Working Environment"));
	QFont titleFont("comic sans" , 30);
	title->setFont(titleFont);

	title->setPos(rect().width()/2 - title->boundingRect().width()/2 , 200);

	// create 3 buttons;
	QList<Button *> buttonList;
	Button *loadButton = new Button(QString("LOAD PROJECT"), 120, Qt::cyan);
	connect(loadButton, SIGNAL(clicked()), this, SLOT(temp()));
	Button *startButton = new Button(QString("START"), 100, Qt::green);
	connect(startButton, SIGNAL(clicked()), this, SLOT(startWork()) );
	Button *exitButton = new Button(QString("EXIT"), 100, Qt::red);
	connect(exitButton, SIGNAL(clicked()) , this, SLOT(close()));

	Button *cvButton = new Button(QString("CV Detail"), 100, Qt::green);
	connect(cvButton, SIGNAL(clicked()), this, SLOT(showCVDetail()) );

	buttonList << loadButton << startButton << exitButton << cvButton;

	// add item
	for(size_t i = 0, n = buttonList.size(); i<n; ++i){
		double buttonX = rect().width()/2 - buttonList[i]->boundingRect().width()/2;
		double buttonY = title->y() + 100*(i+1);
		buttonList[i]->setPos(buttonX, buttonY);
		_scene->addItem(buttonList[i]);
	}
		_scene->addItem(title);

}

void WorkTable::mousePressEvent(QMouseEvent *event){
	// below mouse event just for create a new component box;
	if(_newComponentPosition == 1){
		// add a new component box into the table;
		_componentBoxPositionList.append(QPointF(event->pos()));
		// set the _newComponent to 00
		_newComponentPosition = 0;
		_scene->clear();
		drawGUI();
	}
}

void WorkTable::setImage(QImage im){
	_image = im;

	_scene->update();
}

void WorkTable::paintEvent(QPaintEvent *paintEventInfo){
	QPainter painter(viewport());
	painter.drawImage(QRect(0,0,viewport()->width(),viewport()->height()), _image, QRect(0,0, _image.width(), _image.height()) );
	QGraphicsView::paintEvent(paintEventInfo);
}

void WorkTable::deHighLightAll(){
	_textArea->deHighLight();
	_videoArea->deHighLight();

//	qDebug() << "Try to load module, funtion not finished yet!";
}

void WorkTable::startWork(){
	_scene->clear();
	drawGUI();
}

void WorkTable::nextStep(){
	if (!entered) return;
	_textArea->displayNext();
	_textArea->highLight();

	_videoArea->displayNext();
	_videoArea->highLight();

	QTimer *timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(deHighLightAll()) );
	timer->start(2000);

	_scene->update();

}

// below part for cv stuff
void WorkTable::updateView(){
	/*
	if(!video.isOpened()) return;
    cv::Mat frame;
    while(1){
        video >> frame;
        if(!frame.empty()) break;
    }
    if(frame.empty()) return;
    // _workingArea->setImage(QImage((const unsigned char*)(frame.data), frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped());
	*/
}


void WorkTable::addNewComponentBox(){
	_newComponentPosition = 1;
}

void WorkTable::showCVDetail(){
	qDebug() << "test 1";
	newDialog = new OpenCVWindow(this);
	newDialog->show();
}

void WorkTable::temp() {
	qDebug() << "debug temp slot!";
}


