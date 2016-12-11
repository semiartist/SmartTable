#ifndef TEXTDISPLAY
#define TEXTDISPLAY

#include "ApplicationArea.h"
#include <QList>
#include <QString>

class TextDisplay: public ApplicationArea{
	Q_OBJECT
public:
	// constructor
	TextDisplay(double xPos, double yPos, double width, double height, QString name, QList<QString> *inputList = NULL,QGraphicsItem *parent=NULL);
	// TextDisplay(QList<QString> *inputList);

private:
	QGraphicsTextItem *_displayText;
	QList<QString > _instructionList;
	int _step;

public slots:
	void displayNext();

};

#endif // TEXTDISPLAY

