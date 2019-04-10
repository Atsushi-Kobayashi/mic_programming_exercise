#pragma once
#include <vector>
#include <iostream>

class MyVec3d {
public:
	MyVec3d();
	MyVec3d(double x, double y, double z);
	~MyVec3d();
	MyVec3d &operator=(const MyVec3d &vec);

	//—v‘f‚²‚Æ‚ÌŽl‘¥‰‰ŽZ+,-,*,/
	MyVec3d operator+(const MyVec3d &vec);
	MyVec3d operator-(const MyVec3d &vec);
	MyVec3d operator*(const MyVec3d &vec);
	MyVec3d operator/(const MyVec3d &vec);
	double x;
	double y;
	double z;
	int matProduct(std::vector<double> vec);
	void scalarMultiple(double val);
	void scalarMultiple(int val);

private:
	
};
