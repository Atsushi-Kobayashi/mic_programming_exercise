#include "MyVec3d.h"
#include <vector>
#include <iostream>


MyVec3d::MyVec3d(): x(0),y(0),z(0){}

MyVec3d::MyVec3d(double x, double y, double z) : x(x), y(y), z(z) {}

MyVec3d::~MyVec3d() {}

MyVec3d &MyVec3d::operator=(const MyVec3d &vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return (*this);
}

MyVec3d MyVec3d::operator+(const MyVec3d & vec)
{
	MyVec3d result;
	result.x = x + vec.x;
	result.y = y + vec.y;
	result.z = z + vec.z;
	return result;
}

MyVec3d MyVec3d::operator-(const MyVec3d & vec)
{
	MyVec3d result;
	result.x = x - vec.x;
	result.y = y - vec.y;
	result.z = z - vec.z;
	return result;
}

MyVec3d MyVec3d::operator*(const MyVec3d & vec)
{
	MyVec3d result;
	result.x = x * vec.x;
	result.y = y * vec.y;
	result.z = z * vec.z;
	return result;
}

MyVec3d MyVec3d::operator/(const MyVec3d & vec)
{
	MyVec3d result;
	result.x = x / vec.x;
	result.y = y / vec.y;
	result.z = z / vec.z;
	return result;
}

int MyVec3d::matProduct(std::vector<double> vec)
{
	if (vec.size() != 9) {
		std::cout << "Error: Mat product\n";
		return -1;
	}

	double x_dst = 0, y_dst = 0, z_dst = 0;
	x_dst = vec[0] * x + vec[1] * y + vec[2] * z;
	y_dst = vec[3] * x + vec[4] * y + vec[5] * z;
	z_dst = vec[6] * x + vec[7] * y + vec[8] * z;

	x = x_dst;
	y = y_dst;
	z = z_dst;
	return 1;
}

void MyVec3d::scalarMultiple(double val)
{
	x *= val;
	y *= val;
	z *= val;
}

void MyVec3d::scalarMultiple(int val)
{
	x *= val;
	y *= val;
	z *= val;
}

