#pragma once
#include "externalCommunication/communicationCore.h"

/*
		This commcore child is masking with commcore interface to store data in file.
		Address is used as file name.

		Update:
			Fixed checkAdrress method - no empty checks
		Update:
			better addressChanged interaction: now address is modified

												Written by Ovidiu, modified by Anke
*/

class toLocalMemory : public communicationCore	//	Stores data into file
{
	Q_OBJECT
public:
	toLocalMemory(Modes cmode);

	virtual bool send(sendingMode, int) override;	//	commcore interface
	virtual void get() override;
private:
	void saveToFile();			//	utility function which is saving data to file
	void applyAddressFix();
public slots:
	void addressChanged(QString address, bool isFrom) override;
};
