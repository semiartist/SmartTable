
#include "PhotoDisplay.h"
#include <QDebug>

PhotoDisplay::PhotoDisplay(double xPos, double yPos, double width, double height, QString name, QList<QPixmap> *inputList, QGraphicsItem *parent):
	ApplicationArea(xPos, yPos, width, height, name, parent){
	/*

	if (inputList = NULL){
		_photoList << QPixmap(":/wiPhoto/step1.jpg");
		_photoList << QPixmap(":/wiPhoto/step2.jpg");
	}else{
		_photoList = *inputList;
	}

	// initialize some variable
	_step = 0;

	_displayPhoto = new QGraphicsPixmapItem(this);
	*/

	_width  = width;

	_step = 0;
	_index = 0;
	_totalStep = 15;
	_photoList << QPixmap(":/wiPhoto/photos/step1_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step2_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step3_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step3_2.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step4_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step5_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step5_2.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step6_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step6_2.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step7_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step8_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step8_2.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step9_3.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step9_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step9_2.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step10_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step11_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step11_2.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step12_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step13_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step13_2.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step14_1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step14_2.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step15_1.jpg");

	_stepSize << 1 <<1 <<2 << 1 << 2 <<2 << 1 << 2 << 3 << 1 << 2 << 1 << 2 << 2 << 1 << 1; // the last one just incase of error report;


	_displayPhoto = new QGraphicsPixmapItem(this);
	//	_displayPhoto->setPixmap(QPixmap(":/wiPhoto/photos/step1.jpg"));
	//	_displayPhoto->setPixmap(_photoList[0]);
	_displayPhoto->setPos(rect().x(), rect().y()+25);
	displayNext();

//	stepTimer = new QTimer(this);
//	connect(stepTimer, SIGNAL(timeout()), this, SLOT(stepIn()) );
//	stepTimer->start(3000);
}

PhotoDisplay::~PhotoDisplay(){
	//	delete[] _stepSize;
}

void PhotoDisplay::displayNext(){
//	disconnect(stepTimer, SIGNAL(timeout()), this, SLOT(stepIn()) );
	_in = 0;
	qDebug() << "Total Step -> " <<_totalStep;
	qDebug() << "Current Step ->" << _step;
	qDebug() << "Index ->" << _index << " Size -> " << _photoList.size() << '\n' ;

	if(_index < _photoList.size() && _step < _totalStep){
		_displayPhoto->setPixmap(_photoList[_index].scaledToWidth(_width));
		_index += _stepSize[_step];
		_step++;
	}

}

void PhotoDisplay::displayPre(){
	if(_step <= 1){
		_step = 0;
		_index = 0;
	}else{
		_index -= _stepSize[_step-1];
		_index-=_stepSize[_step-2];
		_step -= 2;
	}

	displayNext();
}

void PhotoDisplay::stepIn(){
	int currentStepSize = _stepSize[_step-1];
	int currentIndex = _index-currentStepSize;
	if(currentStepSize > 1){
		if(_in < currentStepSize - 1){
			++_in;
		}else{
			_in = 0;
		}
		_displayPhoto->setPixmap(_photoList[currentIndex+_in].scaledToWidth(_width));

	}else return;
}
