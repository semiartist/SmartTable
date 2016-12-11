#ifndef APPILCATIONAREA
#define APPILCATIONAREA

#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QObject>
#include <QPen>

class ApplicationArea:public QObject, public QGraphicsRectItem{
	Q_OBJECT
public:
	// constructor
	ApplicationArea(double xPos, double yPos, double width, double height, QString name = QString("Empty Name"), QGraphicsItem *parent = NULL);

	// public functions;

	// instance variables;

private:
	// private functions;

	// priavete instance variables;
	QGraphicsTextItem *_title;
	QGraphicsRectItem *_titleBase;
	QPen _highLightPen;
	QPen _normalPen;

public slots:
	void highLight();
	void deHighLight();
//	void testSlot();
};

#endif // APPILCATIONAREA

