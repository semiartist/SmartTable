#include "TextDisplay.h"

TextDisplay::TextDisplay(double xPos, double yPos, double width, double height, QString name, QList<QString> *inputList, QGraphicsItem *parent):
	ApplicationArea(xPos, yPos, width, height, name, parent){
	// simulate initialize the part;
	if(inputList == NULL){
	_instructionList << QString("STEP1: \nReady your working area;");
	_instructionList << QString("STEP2: \nGet the green panel, and the red corner bracket;");
	_instructionList << QString("STEP3: \nGet the black bracket and insert it onto the cross corner of the red piece");
	_instructionList << QString("STEP4: \nGet the white window piece and install it next to the black bracket you just added");
	_instructionList << QString("STEP5: \nGet the white block and install it next to the white window bracket you just installed");
	_instructionList << QString("STEP6: \nGet the black block and install it on top of the white block you just installed");
	}else{
		_instructionList = *inputList;
	}

	// initialize some variales;
	_step = 0;

	// setup display text
	_displayText = new QGraphicsTextItem(this);
	_displayText->setDefaultTextColor(Qt::white);

	_displayText->setTextWidth(boundingRect().width()-30);
	_displayText->setPos(rect().x()+25, rect().y()+40);

	displayNext();
}

void TextDisplay::displayNext(){
	if(_step < _instructionList.size()){
		_displayText->setPlainText(_instructionList[_step]);
		_step++;
	}
}
