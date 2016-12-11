#include "ApplicationArea.h"
#include <QBrush>
#include "WorkTable.h"
#include <QStyle>

// extern WorkTable *table;

ApplicationArea::ApplicationArea(double xPos, double yPos, double width, double height, QString name, QGraphicsItem *parent): QGraphicsRectItem(parent){
	// create pen
	_highLightPen.setWidth(10);
	_highLightPen.setColor(Qt::red);
	_highLightPen.setStyle(Qt::SolidLine);

	_normalPen.setColor(Qt::black);
	_normalPen.setWidth(1);
	_normalPen.setStyle(Qt::DotLine);
	// set basic information
	setRect(xPos, yPos-25, width, height+25);
	setPen(_normalPen);

	// deal some GUI objects
	_titleBase = new QGraphicsRectItem(this);
	_titleBase->setRect(rect().x(),rect().y(),width, 25);
	QBrush titleBaseBrush;
	titleBaseBrush.setStyle(Qt::SolidPattern);
	titleBaseBrush.setColor(Qt::darkYellow);
	_titleBase->setBrush(titleBaseBrush);

	QBrush backgroundBrush;
	backgroundBrush.setStyle(Qt::SolidPattern);
	backgroundBrush.setColor(Qt::black);
	this->setBrush(backgroundBrush);

	// then do the text item
	_title = new QGraphicsTextItem(name, this);
	_title->setPos(rect().x()+rect().width()/2 - _title->boundingRect().width()/2, rect().y());

}

void ApplicationArea::highLight(){

	setPen(_highLightPen);
}

void ApplicationArea::deHighLight(){

	setPen(_normalPen);
}
