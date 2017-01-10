#ifndef TEXTDISPLAY
#define TEXTDISPLAY

#include "ApplicationArea.h"
#include <QList>
#include <QString>
#include <QTimer>

class TextDisplay: public ApplicationArea{
	Q_OBJECT
public:
	// constructor
	TextDisplay(double xPos, double yPos, double width, double height, QString name, QList<QString> *inputList = NULL,QGraphicsItem *parent=NULL);
	// TextDisplay(QList<QString> *inputList);

private:
	QGraphicsTextItem *_displayText;
	QList<QString > _instructionList;
	QList<int> _stepSize;

	int _step;
	int _index;
	int _totalStep;
	int _in;
	QTimer *stepTimer;

public slots:
	void displayNext();
	void displayPre();
	void stepIn();
};

#endif // TEXTDISPLAY

