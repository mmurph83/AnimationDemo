//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 
#pragma once
#ifndef ENGINE_MATH_VECT_H
#define ENGINE_MATH_VECT_H

// Includes to handle SIMD register types
#include <xmmintrin.h>
#include <smmintrin.h> 

class Matrix;


class Vect
{
public:

	// Do your magic here
	Vect();
	Vect(float x, float y, float z, float w = 1.0f);
	

	float & operator[](x_enum x);
	float & operator[](y_enum y);
	float & operator[](z_enum z);
	float & operator[](w_enum w);

	float operator[](x_enum x) const;
	float operator[](y_enum y) const;
	float operator[](z_enum z) const;
	float operator[](w_enum w) const;

	Vect operator*(const Matrix& m);
	Vect& operator*=(const Matrix& m);

	Vect operator+(const Vect& v);
	Vect& operator+=(const Vect& v);
	Vect& operator-=(const Vect& v);
	Vect operator-(const Vect& v);
	inline Vect operator*(float& val)
	{
		return Vect(_vx*val, _vy*val, _vz*val);
	}
	inline Vect& operator*=(float& val)
	{
		_vx *= val;
		_vy *= val;
		_vz *= val;
		_vw = 1.0f;
		return *this;
	}
	
	void norm();
	Vect getNorm();
	Vect cross(const Vect& v2);

	float mag();
	float magSqr();
	float getAngle(Vect& v2);

	Vect operator+();
	Vect operator-();
	Vect& operator=(const Vect& v);

	float dot(const Vect& v);
	bool isEqual(const Vect& v, float val);
	bool isEqual(const Vect& v);
	bool isZero(float val);
	bool isZero();

	void set(float x, float y, float z, float w = 1.0f);
	
	void set(Vect& v);

	float& x();
	float& y();
	float& z();
	float& w();

	float x() const;
	float y() const;
	float z() const;
	float w() const;
	friend Vect operator*(float val, const Vect& v)
	{
		Vect returnVect = v;
		returnVect *= val;
		return returnVect;
	}
private:

	union
	{
		__m128	_m;

		// anonymous struct
		struct
		{
			float _vx;
			float _vy;
			float _vz;
			float _vw;
		};
	};

};

#endif

// ---  End of File ---------------
