
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

	_step = 0;
	_photoList << QPixmap(":/wiPhoto/photos/step1.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step2.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step3.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step4.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step5.jpg");
	_photoList << QPixmap(":/wiPhoto/photos/step6.jpg");



	_displayPhoto = new QGraphicsPixmapItem(this);
//	_displayPhoto->setPixmap(QPixmap(":/wiPhoto/photos/step1.jpg"));
//	_displayPhoto->setPixmap(_photoList[0]);
	_displayPhoto->setPos(rect().x(), rect().y()+25);
	displayNext();
}

void PhotoDisplay::displayNext(){
	qDebug() << _photoList.size();
	qDebug() << _step;

	if(_step < _photoList.size()){
		_displayPhoto->setPixmap(_photoList[_step]);
		_step++;
	}
}
