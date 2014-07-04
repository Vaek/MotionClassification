
#ifndef JOINBASEDATA_H
#define JOINBASEDATA_H

#include <iostream>

class JoinBaseData {
public:
	JoinBaseData(std::string name);
	virtual ~JoinBaseData();
	
	JoinBaseData* setTranslation(double* translation);
	JoinBaseData* setRotation(double* rotation);
	JoinBaseData* setScaling(double* scaling);
	
	const std::string getName();
	double* getTranslation();
	double* getRotation();
	double* getScaling();

private:
	const std::string name;
	double translation[3], rotation[3], scaling[3];
};

#endif JOINBASEDATA