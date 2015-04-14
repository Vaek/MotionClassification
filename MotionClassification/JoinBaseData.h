
#ifndef JOINBASEDATA_H
#define JOINBASEDATA_H

#include <iostream>
#include <array>

class JoinBaseData {
public:
	JoinBaseData(std::string name);
	virtual ~JoinBaseData() ;
	
	JoinBaseData* setTranslation(std::array<double,3> translation);
	JoinBaseData* setRotation(std::array<double,3> rotation);
	JoinBaseData* setScaling(std::array<double,3> scaling);
	
	std::string getName() const;
	const std::array<double,3> getTranslation() const;
	const std::array<double,3> getRotation() const;
	const std::array<double,3> getScaling() const;

	static double getSize(std::array<double, 3> arr3d);

private:
	const std::string name;
	std::array<double,3> translation, rotation, scaling;
};

#endif JOINBASEDATA
