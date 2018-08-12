//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include <math.h>

#include "MathEngine.h"
#include "Matrix.h"

// Do your magic here

// ---  End of File ---------------
Vect::Vect() :_vx(0.0f), _vy(0.0f), _vz(0.0f), _vw(1.0f)
{
}


Vect::Vect(float x, float y, float z, float w) : _vx(x), _vy(y), _vz(z), _vw(w)
{
}



float & Vect::operator[](x_enum x)
{
	x = x;
	return _vx;
}
float & Vect::operator[](y_enum y)
{
	y = y;
	return _vy;
}
float & Vect::operator[](z_enum z)
{
	z = z;
	return _vz;
}
float & Vect::operator[](w_enum w)
{
	w = w;
	return _vw;
}

float Vect::operator[](x_enum x) const
{
	x = x;
	return _vx;
}
float Vect::operator[](y_enum y) const
{
	y = y;
	return _vy;
}
float Vect::operator[](z_enum z) const
{
	z = z;
	return _vz;
}
float Vect::operator[](w_enum w) const
{
	w = w;
	return _vw;
}

Vect Vect::operator*(const Matrix& m)
{

	float rx = _vx*m[m0];
	float ry = _vx*m[m1];
	float rz = _vx*m[m2];
	float rw = _vx*m[m3];


	rx += _vy*m[m4];
	ry += _vy*m[m5];
	rz += _vy*m[m6];
	rw += _vy*m[m7];


	rx += _vz*m[m8];
	ry += _vz*m[m9];
	rz += _vz*m[m10];
	rw += _vz*m[m11];

	rx += _vw*m[m12];
	ry += _vw*m[m13];
	rz += _vw*m[m14];
	rw += _vw*m[m15];


	return Vect(rx, ry, rz, rw);
}
Vect& Vect::operator*=(const Matrix& m)
{

	float rx = _vx*m[m0];
	float ry = _vx*m[m1];
	float rz = _vx*m[m2];
	float rw = _vx*m[m3];


	rx += _vy*m[m4];
	ry += _vy*m[m5];
	rz += _vy*m[m6];
	rw += _vy*m[m7];


	rx += _vz*m[m8];
	ry += _vz*m[m9];
	rz += _vz*m[m10];
	rw += _vz*m[m11];

	rx += _vw*m[m12];
	ry += _vw*m[m13];
	rz += _vw*m[m14];
	rw += _vw*m[m15];

	set(rx, ry, rz, rw);
	return *this;
}

Vect Vect::operator+(const Vect& v)
{
	return Vect(_vx + v._vx, _vy + v._vy, _vz + v._vz, 1.0f);
}
Vect Vect::operator-(const Vect& v) const
{
	return Vect(_vx - v._vx, _vy - v._vy, _vz - v._vz, 1.0f);
}


float Vect::dot(const Vect& v) const
{
	float returnVal = v._vx*_vx;
	returnVal += v._vy*_vy;
	returnVal += v._vz*_vz;
	return returnVal;
}

Vect& Vect::operator+=(const Vect& v)
{
	_vx += v._vx;
	_vy += v._vy;
	_vz += v._vz;
	_vw = 1.0f;
	return *this;
}

Vect& Vect::operator-=(const Vect& v)
{
	_vx -= v._vx;
	_vy -= v._vy;
	_vz -= v._vz;
	_vw = 1.0f;
	return *this;
}

bool Vect::isEqual(const Vect& v, float val)
{
	val = val;
	if (v._vx - _vx > val)
	{
		return false;
	}
	if (v._vy - _vy > val)
	{
		return false;
	}
	if (v._vz - _vz > val)
	{
		return false;
	}
	if (v._vw - _vw > val)
	{
		return false;
	}
	return true;
}
bool Vect::isEqual(const Vect& v)
{
	if (v._vx != _vx)
	{
		return false;
	}
	if (v._vy != _vy)
	{
		return false;
	}
	if (v._vz != _vz)
	{
		return false;
	}
	if (v._vw != _vw)
	{
		return false;
	}
	return true;
}

bool Vect::isZero(float val)
{
	if (val < _vx)
	{
		return false;
	}
	if (val <  _vy)
	{
		return false;
	}
	if (val < _vz)
	{
		return false;
	}
	if (val < (_vw - 1.0f))
	{
		return false;
	}
	return true;
}
bool Vect::isZero()
{
	return isZero(0.0f);
}

Vect Vect::operator+()
{
	return Vect(_vx, _vy, _vz);
}
Vect Vect::operator-()
{
	return Vect(-_vx, -_vy, -_vz);
}

Vect& Vect::operator=(const Vect& v)
{
	_vx = v._vx;
	_vy = v._vy;
	_vz = v._vz;
	_vw = v._vw;
	return *this;
}

Vect Vect::operator*(const Quat& q)
{
	//ToDo
	MATH_UNUSED(q);
	Matrix m(q);
	return *this * m;
}
Vect& Vect::operator*=(const Quat& q)
{
	//ToDo
	MATH_UNUSED(q);
	Matrix m(q);
	*this *= m;
	return *this;
}

void Vect::set(float x, float y, float z, float w)
{
	_vx = x;
	_vy = y;
	_vz = z;
	_vw = w;
}

void Vect::set(Vect& v)
{
	*this = v;
}



void Vect::norm()
{
	float magnitude = mag();
	if (magnitude > 0.00001f)
	{
		_vx /= magnitude;
		_vy /= magnitude;
		_vz /= magnitude;
	}
	else
	{
		_vx = 0.0f;
		_vy = 0.0f;
		_vz = 0.0f;
	}
	_vw = 1.0f;
}

Vect Vect::getNorm() const
{
	Vect returnVect = *this;
	returnVect.norm();
	return returnVect;
}
Vect Vect::cross(const Vect& v2) const
{
	return Vect((_vy*v2._vz) - (v2._vy*_vz), (_vz*v2._vx) - (_vx*v2._vz), (_vx*v2._vy) - (_vy*v2._vx));
}

float Vect::mag() const
{
	return sqrtf((_vx*_vx) + (_vy*_vy) + (_vz*_vz));

}
float Vect::magSqr() const
{
	float magnitude = mag();
	magnitude *= magnitude;
	return magnitude;
}

float Vect::getAngle(Vect& v2)
{
	float lengthA = v2.mag();
	float lengthB = mag();
	lengthA *= lengthB;
	lengthA = dot(v2) / lengthA;
	lengthA = acosf(lengthA);
	return lengthA;
}

float& Vect::x()
{
	return _vx;
}
float& Vect::y()
{
	return _vy;
}
float& Vect::z()
{
	return _vz;
}
float& Vect::w()
{
	return _vw;
}

float Vect::x() const
{
	return _vx;
}
float Vect::y() const
{
	return _vy;
}
float Vect::z() const
{
	return _vz;
}
float Vect::w() const
{
	return _vw;
}

