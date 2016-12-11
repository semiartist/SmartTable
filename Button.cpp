#include "Button.h"
#include <QBrush>
#include <QFont>


Button::Button(QString name, double width, QColor color, QGraphicsItem *parent):QObject(), QGraphicsRectItem(parent){
	// set rect
	setRect(0, 0, width, 35);

	// set text
	_name = new QGraphicsTextItem(name, this);
	QFont thisFont("comic sans");
	_name->setFont(thisFont);
	_name->setDefaultTextColor(Qt::white);

	double xPos = rect().width()/2 - _name->boundingRect().width()/2;
	double yPos = rect().height()/2 - _name->boundingRect().height()/2;
	_name->setPos(xPos, yPos);

	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	brush.setColor(color);
	setBrush(brush);

	setAcceptHoverEvents(true);


}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event){
	emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
	_name->setDefaultTextColor(Qt::green);
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
	_name->setDefaultTextColor(Qt::white);
}
