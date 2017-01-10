#ifndef PHOTODISPLAY
#define PHOTODISPLAY

#include "ApplicationArea.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QString>
#include <QLabel>
#include <QTimer>

class PhotoDisplay: public ApplicationArea{
	Q_OBJECT

public:
	PhotoDisplay(double xPos, double yPos, double width, double height, QString name, QList<QPixmap> *inputList = NULL, QGraphicsItem *parent=NULL);
	~PhotoDisplay();

private:
	QGraphicsPixmapItem *_displayPhoto;
	QList<QPixmap > _photoList;
	int _step;
	int _totalStep;
	int _index;
	double _width;
	int _in;
	QTimer *stepTimer;

	QList<int> _stepSize;
	//void test();

public slots:
	void displayNext();
	void displayPre();
	void stepIn();
};

#endif // PHOTODISPLAY

