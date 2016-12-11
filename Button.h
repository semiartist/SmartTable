#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QString>
#include <QObject>
#include <QColor>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

class Button:public QObject, public QGraphicsRectItem{
	Q_OBJECT
public:
	// constructor
	Button(QString name, double width, QColor color, QGraphicsItem *parent = NULL);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
	void clicked();

private:
	QGraphicsTextItem *_name;

};

#endif // BUTTON_H
