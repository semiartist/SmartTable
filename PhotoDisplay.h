#ifndef PHOTODISPLAY
#define PHOTODISPLAY

#include "ApplicationArea.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QString>
#include <QLabel>

class PhotoDisplay: public ApplicationArea{
	Q_OBJECT

public:
	PhotoDisplay(double xPos, double yPos, double width, double height, QString name, QList<QPixmap> *inputList = NULL, QGraphicsItem *parent=NULL);

private:
	QGraphicsPixmapItem *_displayPhoto;
	QList<QPixmap > _photoList;
	int _step;
	//void test();

public slots:
	void displayNext();
};

#endif // PHOTODISPLAY

