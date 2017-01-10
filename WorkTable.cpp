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
#include <QKeyEvent>
#include <QKeySequence>

const int BOXWIDTH = 100;
const int BOXHEIGHT = 120;

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
	_totalStep = 15;
	_wi = QString("Ready to Start!");
	_newComponentPosition = 0;
	entered = 0;

	component = 0;
	frameNumber = 0;
	stepped = false;

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateView()));

	QTimer *stepControlTimer = new QTimer(this);
	connect(stepControlTimer, SIGNAL(timeout()), this, SLOT(stepControl()) );
	stepControlTimer->start(10);

}

void WorkTable::next(int step){
	switch(step){
	case 1:
		highBackground(1);
		break;
	case 3:
		highBackground(5);
		break;
	case 5:
		highBackground(2);
		break;
	case 6:
		highBackground(4);
		break;
	case 11:
		highBackground(3);
		highBackground(6);
		break;
	}

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

	/* disable the previous button and next button
	// create next button
	Button *nextButton = new Button(QString("NEXT"), 100, Qt::darkMagenta);
	nextButton->setPos(_workingArea->boundingRect().width()-110+25, height()-70);
	connect(nextButton, SIGNAL(clicked()), this, SLOT(nextStep()) );
	_scene->addItem(nextButton);

	// create previous button
	Button *preButton = new Button(QString("PREV"), 100, Qt::blue);
	preButton->setPos(35, height()-70);
	connect(preButton, SIGNAL(clicked()), this, SLOT(preStep()) );
	_scene->addItem(preButton);

	// create build component button
	/* disable the build component button
	Button *addNewComponentButton = new Button(QString("ADD COMP"), 100, Qt::blue);
	addNewComponentButton->setPos(buttonX-250, buttonY);
	connect(addNewComponentButton, SIGNAL(clicked()), this, SLOT(addNewComponentBox()) );
	_scene->addItem(addNewComponentButton);
	*/

	/* Disable this, manually create component box;
	for(int i = 0;i< _componentBoxPositionList.size(); i++ ){
		QString num = QString::number(i+1);
		QString name = QString("Component ") + num;
		// qDebug() << name;
		// qDebug() << name << _componentBoxPositionList[i].x() << " " << _componentBoxPositionList[i].y();
		ApplicationArea *componentBox = new ApplicationArea(_componentBoxPositionList[i].x(), _componentBoxPositionList[i].y(), 120, 100, name);
		_scene->addItem(componentBox);
	}*/

	componentBox1 = new ApplicationArea(25, 20, BOXWIDTH, BOXHEIGHT, "FIN");
	_scene->addItem(componentBox1);

	componentBox2 = new ApplicationArea( 25 +(25+BOXWIDTH)*1, 20, BOXWIDTH, BOXHEIGHT, "TRIGGER" );
	componentBox3 = new ApplicationArea( 25+ (25+BOXWIDTH)*2, 20, BOXWIDTH, BOXHEIGHT, "SCREWS" );
	componentBox4 = new ApplicationArea( 25+ (25+BOXWIDTH)*3, 20, BOXWIDTH, BOXHEIGHT, "FAN ASSY" );
	_scene->addItem(componentBox2);
	_scene->addItem(componentBox3);
	_scene->addItem(componentBox4);

	toolBox1 = new ApplicationArea(50 + ( 25 + BOXWIDTH) *4, 20, BOXWIDTH/2, BOXHEIGHT, "CALLIPER" );
	toolBox2 = new ApplicationArea(100 + (25 + BOXWIDTH) *5, 20, BOXWIDTH/2, BOXHEIGHT, "SCREW DRIVER" );

	_scene->addItem(toolBox1);
	_scene->addItem(toolBox2);


	stepTimer = new QTimer(this);
	connect(stepTimer, SIGNAL(timeout()), _textArea, SLOT(stepIn()));
	connect(stepTimer, SIGNAL(timeout()), _videoArea, SLOT(stepIn()));
	stepTimer->start(3000);

	entered = 1;
	highlightTimer = new QTimer();
	connect(highlightTimer, SIGNAL(timeout()), this, SLOT(deHighLightAll()) );

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

//void WorkTable::mousePressEvent(QMouseEvent *event){
//	// below mouse event just for create a new component box;
//	if(_newComponentPosition == 1){
//		// add a new component box into the table;
//		_componentBoxPositionList.append(QPointF(event->pos()));
//		// set the _newComponent to 00
//		_newComponentPosition = 0;
//		_scene->clear();
//		drawGUI();
//	}
//}

void WorkTable::keyPressEvent(QKeyEvent *event) {
	switch (event->key()){
	case Qt::Key_Right:
		nextStep();
		break;

	case Qt::Key_Left:
		preStep();
		break;
	}
}

void WorkTable::highBackground(int item){
	QBrush backgroundBrush;
	backgroundBrush.setStyle(Qt::SolidPattern);
	backgroundBrush.setColor(Qt::green);

	QBrush blackBrush;
	blackBrush.setStyle(Qt::SolidPattern);
	blackBrush.setColor(Qt::black);

	switch (item){
	case 1:
		componentBox1->setBrush(backgroundBrush);
		break;
	case 2:
		componentBox2->setBrush(backgroundBrush);
		break;
	case 3:
		componentBox3->setBrush(backgroundBrush);
		break;
	case 4:
		componentBox4->setBrush(backgroundBrush);
		break;
	case 5:
		toolBox1->setBrush(backgroundBrush);
		break;
	case 6:
		toolBox2->setBrush(backgroundBrush);
		break;
	case 7:
		componentBox1->setBrush(blackBrush);
		componentBox2->setBrush(blackBrush);
		componentBox3->setBrush(blackBrush);
		componentBox4->setBrush(blackBrush);
		toolBox1->setBrush(blackBrush);
		toolBox2->setBrush(blackBrush);
		break;
	}
}

void WorkTable::setImage(QImage im){
	_image = im;

	_scene->update();
}

void WorkTable::stepInfo(int a, int b){
	component = a;
	frameNumber = b;
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
	if (_step < _totalStep) _step++;
	highBackground(7);
	_textArea->displayNext();
	_textArea->highLight();

	_videoArea->displayNext();
	_videoArea->highLight();

	highlightTimer->start(2000);

	stepTimer->start(3000);
	next(_step);
	_scene->update();

}

void WorkTable::preStep(){
	if (!entered) return;

	if (_step > 0) _step--;
	highBackground(7);
	_textArea->displayPre();
	_textArea->highLight();

	_videoArea->displayPre();
	_videoArea->highLight();

	highlightTimer->start(2000);

	stepTimer->start(3000);

	newDialog->setStaut( _step);

	next(_step);
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


//void WorkTable::addNewComponentBox(){
//	_newComponentPosition = 1;
//}

void WorkTable::showCVDetail(){
	qDebug() << "test 1";
	newDialog = new OpenCVWindow(this);
	newDialog->show();
}

void WorkTable::temp() {
	qDebug() << "debug temp slot!";
}

void WorkTable::stepControl() {
	/*
	qDebug() << "component ->" << component;
	qDebug() << "stepped -> " << stepped;
	if (component == 99 && !stepped) {
		nextStep();
		stepped == true;
		return;
	}
	if (component == 88 && !stepped) {
		preStep();
		stepped = true;
		return;
	}

	component = 0;
	stepped = false;
	*/
	int need = 0;
	switch(_step){
	case 1:
		need = 1;
		break;
	case 3:
		need = 5;
		break;
	case 5:
		need = 2;
		break;
	case 6:
		need = 4;
		break;
	case 11:
		need = 6;
		// also need the trigger from the grab screws;
		break;

	}
	if (_step == 11){
		if (newDialog->getStatus(need) && newDialog->getStatus(3)) nextStep();
	} else {
		if (newDialog->getStatus(need)) {
			nextStep();
			qDebug() << "success go to next step";
		}
	}
}


