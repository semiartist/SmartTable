#include "TextDisplay.h"

TextDisplay::TextDisplay(double xPos, double yPos, double width, double height, QString name, QList<QString> *inputList, QGraphicsItem *parent):
	ApplicationArea(xPos, yPos, width, height, name, parent){
	// simulate initialize the part;
	if(inputList == NULL){
		_instructionList << QString("STEP1(1/1): \nReady your working area;\nThen go to next step;");
		_instructionList << QString("STEP2(1/1): \nGet the blower enclosure and the sheet metal fin part;");
		_instructionList << QString("STEP3(1/2): \nPut the fin inside of the blower enclosure;");
		_instructionList << QString("STEP3(2/2): \nWith the fin extrusion facing down;");
		_instructionList << QString("STEP4(1/1): \nUse the piller to adjust the fin, make sure the cutout in fin match the rib inside of the enclosure");
		_instructionList << QString("STEP5(1/2): \nPut the enclosure on the table, facing down;\nGrab the insulator;");
		_instructionList << QString("STEP5(2/2): \nRoll the insulator to put the slot through the cutout;");
		_instructionList << QString("STEP6(1/2): \nPut the insulator calfully inside the enclosure;");
		_instructionList << QString("STEP6(2/2): \nPick the trigger button put it inside the button hole in the enclusure handle;");
		_instructionList << QString("STEP7(1/1): \nPut the enclusure sub-assembly aside;\nGrab the fan sub-assembly, make sure the cut out on the fan case facing you;");
		_instructionList << QString("STEP8(1/2): \nGrab the thermal sub-assembly, insert it on top of the fan sub-assembly, the cable can fit the cutout on the fan case;");
		_instructionList << QString("STEP8(2/2): \nMake sure the fin of the thermal assembly will fit the middle cutout on the morter case;");
		_instructionList << QString("STEP9(1/3): \nPut the fan-thermal assembly into the enclosure, make sure the cable align the handle direction;");
		_instructionList << QString("STEP9(2/3): \nMake sure the 2 component on the cable will fit into the component slot;");
		_instructionList << QString("STEP9(3/3): \nDouble confirm the Component will fit the component slot;");
		_instructionList << QString("STEP10(1/1): \nPress the cable inside the curve slot, and the rubber case head inside the slot;");
		_instructionList << QString("STEP11(1/2): \nGrab the back case of enclosure, press them togher;");
		_instructionList << QString("STEP11(2/2): \nAfter press together, you should see this thing;");
		_instructionList << QString("STEP12(1/1): \nGrab the 3 screws from the component box, and screw it into the screw hole from top to bottom;");
		_instructionList << QString("STEP13(1/2): \nGrab the rear cover of the blower, put it on;");
		_instructionList << QString("STEP13(2/2): \nMake sure the pin goes into the top screw hole;");
		_instructionList << QString("STEP14(1/2): \nGrab the air guider, and press it onto the enclosure;");
		_instructionList << QString("STEP14(2/3): \nYou can see how it looks after put on;");
		_instructionList << QString("STEP15(1/1): \nDone Assembly\nThank you for your paticipation!");

	}else{
		_instructionList = *inputList;
	}

	// initialize some variales;
	_step = 0;
	_index = 0;
	_totalStep = 15;
	_stepSize << 1 <<1 <<2 << 1 << 2 <<2 << 1 << 2 << 3 << 1 << 2 << 1 << 2 << 2 << 1 << 1; // the last one just incase of error report;

	// setup display text
	_displayText = new QGraphicsTextItem(this);
	_displayText->setDefaultTextColor(Qt::white);

	_displayText->setTextWidth(boundingRect().width()-30);
	_displayText->setPos(rect().x()+25, rect().y()+40);

	//	stepTimer = new QTimer(this);
	//	connect(stepTimer, SIGNAL(timeout()), this, SLOT(stepIn()) );
	//	stepTimer->start(3000);

	displayNext();
}

void TextDisplay::displayNext(){
	_in = 0;
	if(_index < _instructionList.size() && _step < _totalStep){
		_displayText->setPlainText(_instructionList[_index]);
		_index+= _stepSize[_step];
		_step++;
	}
}

void TextDisplay::displayPre(){
	if(_step <= 1){
		_step = 0;
		_index = 0;
	}else{
		_index -= _stepSize[_step-1];
		_index-=_stepSize[_step-2];
		_step -= 2;
	}
	displayNext();
}

void TextDisplay::stepIn(){
	int currentStepSize = _stepSize[_step-1];
	int currentIndex = _index-currentStepSize;
	if(currentStepSize > 1){
		if(_in < currentStepSize - 1){
			++_in;
		}else{
			_in = 0;
		}
		_displayText->setPlainText(_instructionList[currentIndex+_in]);

	}else return;
}
