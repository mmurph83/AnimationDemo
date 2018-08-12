//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_MATRIX_H
#define ENGINE_MATH_MATRIX_H
#pragma once
// SIMD includes
#include <xmmintrin.h>
#include <smmintrin.h> 

class Vect;

class Matrix
{
public:

	// Do your magic here
	Matrix();
	Matrix(Vect& v0, Vect& v1, Vect& v2, Vect& v3);
	Matrix(MatrixSpecialType type);
	Matrix(RotType rot, float rotVal);
	Matrix(MatrixTransType trans, float x, float y, float z);
	Matrix(MatrixTransType trans, Vect& v);
	Matrix(MatrixScaleType scale, float x, float y, float z);
	Matrix(MatrixScaleType scale, Vect& v);
	Matrix(const Matrix& m);

	Matrix operator+(Matrix& m);
	Matrix& operator+=(Matrix& m);
	Matrix operator-(Matrix& m);
	Matrix& operator-=(Matrix& m);

	inline Matrix operator*(float& val)
	{
		Matrix returnMatrix = *this;
		returnMatrix._m0 *= val;
		returnMatrix._m1 *= val;
		returnMatrix._m2 *= val;
		returnMatrix._m3 *= val;
		returnMatrix._m4 *= val;
		returnMatrix._m5 *= val;
		returnMatrix._m6 *= val;
		returnMatrix._m7 *= val;
		returnMatrix._m8 *= val;
		returnMatrix._m9 *= val;
		returnMatrix._m10 *= val;
		returnMatrix._m11 *= val;
		returnMatrix._m12 *= val;
		returnMatrix._m13 *= val;
		returnMatrix._m14 *= val;
		returnMatrix._m15 *= val;
		return returnMatrix;
	}
	inline Matrix& operator*=(float& val)
	{
		_m0 *= val;
		_m1 *= val;
		_m2 *= val;
		_m3 *= val;
		_m4 *= val;
		_m5 *= val;
		_m6 *= val;
		_m7 *= val;
		_m8 *= val;
		_m9 *= val;
		_m10 *= val;
		_m11 *= val;
		_m12 *= val;
		_m13 *= val;
		_m14 *= val;
		_m15 *= val;
		return *this;
	}

	Matrix operator*(Matrix& m);
	Matrix& operator*=(Matrix& m);

	Matrix& operator=(const Matrix& m);
	

	Matrix operator+();
	Matrix operator-();

	Matrix getT();
	Matrix getInv();
	void inv();

	float det();
	void T();

	void set(MatrixSpecialType matrixType);
	void set(MatrixTransType trans, float x, float y, float z);
	void set(MatrixTransType trans, Vect& v);
	void set(MatrixScaleType scale, float x, float y, float z);
	void set(MatrixScaleType scale, Vect& v);
	void set(RotType rotType, float rot);
	void set(MatrixRowType rowType, Vect& v);

	void set(Vect& v0, Vect& v1, Vect& v2, Vect& v3);

	Vect get(MatrixRowType rowType);

	bool isIdentity(const float tolerance);
	bool isEqual(Matrix& m);
	bool isIdentity();

	float& m0();
	float& m1();
	float& m2();
	float& m3();
	float& m4();
	float& m5();
	float& m6();
	float& m7();
	float& m8();
	float& m9();
	float& m10();
	float& m11();
	float& m12();
	float& m13();
	float& m14();
	float& m15();

	float m0() const;
	float m1() const;
	float m2() const;
	float m3() const;
	float m4() const;
	float m5() const;
	float m6() const;
	float m7() const;
	float m8() const;
	float m9() const;
	float m10() const;
	float m11() const;
	float m12() const;
	float m13() const;
	float m14() const;
	float m15() const;

	float& operator[](m0_enum e);
	float& operator[](m1_enum e);
	float& operator[](m2_enum e);
	float& operator[](m3_enum e);
	float& operator[](m4_enum e);
	float& operator[](m5_enum e);
	float& operator[](m6_enum e);
	float& operator[](m7_enum e);
	float& operator[](m8_enum e);
	float& operator[](m9_enum e);
	float& operator[](m10_enum e);
	float& operator[](m11_enum e);
	float& operator[](m12_enum e);
	float& operator[](m13_enum e);
	float& operator[](m14_enum e);
	float& operator[](m15_enum e);

	float operator[](m0_enum e) const;
	float operator[](m1_enum e) const;
	float operator[](m2_enum e) const;
	float operator[](m3_enum e) const;
	float operator[](m4_enum e) const;
	float operator[](m5_enum e) const;
	float operator[](m6_enum e) const;
	float operator[](m7_enum e) const;
	float operator[](m8_enum e) const;
	float operator[](m9_enum e) const;
	float operator[](m10_enum e) const;
	float operator[](m11_enum e) const;
	float operator[](m12_enum e) const;
	float operator[](m13_enum e) const;
	float operator[](m14_enum e) const;
	float operator[](m15_enum e) const;

	friend Matrix operator*(float val, const Matrix& m)
	{
		Matrix returnMatrix = m;
		returnMatrix *= val;
		return returnMatrix;
	}
private:

	union
	{
		struct
		{
			Vect _v0;
			Vect _v1;
			Vect _v2;
			Vect _v3;
		};

		struct
		{
			float _m0;
			float _m1;
			float _m2;
			float _m3;
			float _m4;
			float _m5;
			float _m6;
			float _m7;
			float _m8;
			float _m9;
			float _m10;
			float _m11;
			float _m12;
			float _m13;
			float _m14;
			float _m15;
		};
	};
};
#endif

// ---  End of File ---------------
