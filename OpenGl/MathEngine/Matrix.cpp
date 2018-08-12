//---------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include <math.h>

#include "MathEngine.h"

// Do your magic here

// ---  End of File ---------------
Matrix::Matrix()
{
	_v0 = Vect(0.0f, 0.0f, 0.0f, 0.0f);
	_v1 = Vect(0.0f, 0.0f, 0.0f, 0.0f);
	_v2 = Vect(0.0f, 0.0f, 0.0f, 0.0f);
	_v3 = Vect(0.0f, 0.0f, 0.0f, 0.0f);
}

Matrix::Matrix(RotType rot, float rotVal)
{
	float sinRot = sinf(rotVal);
	float cosRot = cosf(rotVal);
	switch (rot)
	{
	case RotType::ROT_X:
		_v0 = Vect(1.0f, 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, cosRot, sinRot, 0.0f);
		_v2 = Vect(0.0f, -sinRot, cosRot, 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case RotType::ROT_Y:
		_v0 = Vect(cosRot, 0.0f, -sinRot, 0.0f);
		_v1 = Vect(0.0f, 1.0f, 0.0f, 0.0f);
		_v2 = Vect(sinRot, 0.0f, cosRot, 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case RotType::ROT_Z:
		_v0 = Vect(cosRot, sinRot, 0.0f, 0.0f);
		_v1 = Vect(-sinRot, cosRot, 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, 1.0f, 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
}

Matrix::Matrix(MatrixTransType trans, float x, float y, float z)
{
	switch (trans)
	{
	case MatrixTransType::TRANS:
		_v0 = Vect(1.0f, 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, 1.0f, 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, 1.0f, 0.0f);
		_v3 = Vect(x, y, z, 1.0f);
		break;
	default:
		break;
	}
}

Matrix::Matrix(MatrixScaleType scale, float x, float y, float z)
{
	switch (scale)
	{
	case MatrixScaleType::SCALE:
		_v0 = Vect(x, 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, y, 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, z, 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
}
Matrix::Matrix(MatrixScaleType scale, Vect& v)
{
	switch (scale)
	{
	case MatrixScaleType::SCALE:
		_v0 = Vect(v.x(), 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, v.y(), 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, v.z(), 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
}
Matrix::Matrix(MatrixSpecialType type)
{
	switch (type)
	{
	case MatrixSpecialType::IDENTITY:
		_v0 = Vect(1.0f, 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, 1.0f, 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, 1.0f, 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case MatrixSpecialType::ZERO:
		_v0 = Vect(0.0f, 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, 0.0f, 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, 0.0f, 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 0.0f);
		break;
	default:
		break;
	}
}

Matrix::Matrix(MatrixTransType trans, Vect& v)
{
	switch (trans)
	{
	case MatrixTransType::TRANS:
		_v0 = Vect(1.0f, 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, 1.0f, 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, 1.0f, 0.0f);
		_v3 = Vect(v.x(), v.y(), v.z(), 1.0f);
		break;
	default:
		break;
	}
}

Matrix::Matrix(Vect& v0, Vect& v1, Vect& v2, Vect& v3)
{
	_v0 = v0;
	_v1 = v1;
	_v2 = v2;
	_v3 = v3;
}
Matrix::Matrix(RotOrientType type, const Vect &v1, const Vect & v2)
{
	Vect rz = v1.getNorm();
	Vect rx = v2.cross(rz);
	rx.norm();
	Vect ry = rz.cross(rx);
	ry.norm();
	Vect trans(0, 0, 0);
	set(rx, ry, rz, trans);
	_m3 = 0.0f;
	_m7 = 0.0f;
	_m11 = 0.0f;

	if (RotOrientType::ROT_INVERSE_ORIENT == type)
	{
		this->inv();
	}
	
}
Matrix::Matrix(RotAxisAngleType type, const Vect &v1, const float &angle)
{
	MATH_UNUSED(type);
	Vect v = v1;
	v.norm();
	Vect r1(v.x() * v.x(), v.x() * v.y(), v.x() * v.z(), 0.0f);
	Vect r2(v.y() * v.x(), v.y() * v.y(), v.y() * v.z(), 0.0f);
	Vect r3(v.z() * v.x(), v.z() * v.y(), v.z() * v.z(), 0.0f);
	Vect r4;
	r1 -= Vect(cosf(angle) * r1);
	r2 -= Vect(cosf(angle) * r2);
	r3 -= Vect(cosf(angle) * r3);
	//r4 -= Vect(cosf(angle) * r4);
	r1 += Vect(cosf(angle), sinf(angle)*v.z(), -sinf(angle)*v.y(), 0.0f);
	r2 += Vect(-sinf(angle)*v.z(), cosf(angle), sinf(angle)*v.x(), 0.0f);
	r3 += Vect(sinf(angle)*v.y(), -sinf(angle)*v.x(), cosf(angle), 0.0f);
	r1.w() = 0.0f;
	r2.w() = 0.0f;
	r3.w() = 0.0f;
	set(ROW_0, r1);
	set(ROW_1, r2);
	set(ROW_2, r3);
	set(ROW_3, r4);
}

Matrix::Matrix(Rot3AxisType type, const float x, const float y, const float z)
{
	Matrix mX(ROT_X, x);
	Matrix mY(ROT_Y, y);
	Matrix mZ(ROT_Z, z);
	*this = mX * mY * mZ;
	MATH_UNUSED(type);
	MATH_UNUSED(x);
	MATH_UNUSED(y);
	MATH_UNUSED(z);
}
Matrix::Matrix(const Quat& q)
{
	MATH_UNUSED(q);
	float xy = q.qx()*q.qy();
	float xz = q.qx()*q.qz();
	float xw = q.qx()*q.real();
	float yz = q.qy()*q.qz();
	float yw = q.qy()*q.real();
	float zw = q.qz()*q.real();
	_v0.set(1.0f - (2.0f * (q.qy()*q.qy() + q.qz()*q.qz())), (xy + zw)*2.0f, (xz*2.0f - yw*2.0f), 0.0f);
	_v1.set(2.0f*(xy - zw), 1.0f - (2.0f * (q.qx()*q.qx() + q.qz()*q.qz())), (yz + xw)*2.0f, 0.0f);
	_v2.set((xz + yw)*2.0f, (yz - xw)*2.0f, 1.0f - (2.0f * (q.qx()*q.qx() + q.qy()*q.qy())), 0.0f);
	_v3.set(0.0f, 0.0f, 0.0f, 1.0f);
}

float& Matrix::m0()
{
	return _m0;
}
float& Matrix::m1()
{
	return _m1;
}
float& Matrix::m2()
{
	return _m2;
}
float& Matrix::m3()
{
	return _m3;
}
float& Matrix::m4()
{
	return _m4;
}
float& Matrix::m5()
{
	return _m5;
}
float& Matrix::m6()
{
	return _m6;
}
float& Matrix::m7()
{
	return _m7;
}
float& Matrix::m8()
{
	return _m8;
}
float& Matrix::m9()
{
	return _m9;
}
float& Matrix::m10()
{
	return _m10;
}
float& Matrix::m11()
{
	return _m11;
}
float& Matrix::m12()
{
	return _m12;
}
float& Matrix::m13()
{
	return _m13;
}
float& Matrix::m14()
{
	return _m14;
}
float& Matrix::m15()
{
	return _m15;
}

float Matrix::m0() const
{
	return _m0;
}
float Matrix::m1() const
{
	return _m1;
}
float Matrix::m2() const
{
	return _m2;
}
float Matrix::m3() const
{
	return _m3;
}
float Matrix::m4() const
{
	return _m4;
}
float Matrix::m5() const
{
	return _m5;
}
float Matrix::m6() const
{
	return _m6;
}
float Matrix::m7() const
{
	return _m7;
}
float Matrix::m8() const
{
	return _m8;
}
float Matrix::m9() const
{
	return _m9;
}
float Matrix::m10() const
{
	return _m10;
}
float Matrix::m11() const
{
	return _m11;
}
float Matrix::m12() const
{
	return _m12;
}
float Matrix::m13() const
{
	return _m13;
}
float Matrix::m14() const
{
	return _m14;
}
float Matrix::m15() const
{
	return _m15;
}


float& Matrix::operator[](m0_enum e)
{
	e = e;
	return _m0;
}
float& Matrix::operator[](m1_enum e) {
	e = e;
	return _m1;
}
float& Matrix::operator[](m2_enum e)
{
	e = e;
	return _m2;
}
float& Matrix::operator[](m3_enum e)
{
	e = e;
	return _m3;
}
float& Matrix::operator[](m4_enum e)
{
	e = e;
	return _m4;
}
float& Matrix::operator[](m5_enum e)
{
	e = e;
	return _m5;
}
float& Matrix::operator[](m6_enum e)
{
	e = e;
	return _m6;
}
float& Matrix::operator[](m7_enum e)
{
	e = e;
	return _m7;
}
float& Matrix::operator[](m8_enum e)
{
	e = e;
	return _m8;
}
float& Matrix::operator[](m9_enum e)
{
	e = e;
	return _m9;
}
float& Matrix::operator[](m10_enum e)
{
	e = e;
	return _m10;
}
float& Matrix::operator[](m11_enum e)
{
	e = e;
	return _m11;
}
float& Matrix::operator[](m12_enum e)
{
	e = e;
	return _m12;
}
float& Matrix::operator[](m13_enum e)
{
	e = e;
	return _m13;
}
float& Matrix::operator[](m14_enum e)
{
	e = e;
	return _m14;
}
float& Matrix::operator[](m15_enum e)
{
	e = e;
	return _m15;
}

float Matrix::operator[](m0_enum e) const
{
	e = e;
	return _m0;
}
float Matrix::operator[](m1_enum e)  const
{
	e = e;
	return _m1;
}
float Matrix::operator[](m2_enum e) const
{
	e = e;
	return _m2;
}
float Matrix::operator[](m3_enum e) const
{
	e = e;
	return _m3;
}
float Matrix::operator[](m4_enum e) const
{
	e = e;
	return _m4;
}
float Matrix::operator[](m5_enum e) const
{
	e = e;
	return _m5;
}
float Matrix::operator[](m6_enum e) const
{
	e = e;
	return _m6;
}
float Matrix::operator[](m7_enum e) const
{
	e = e;
	return _m7;
}
float Matrix::operator[](m8_enum e) const
{
	e = e;
	return _m8;
}
float Matrix::operator[](m9_enum e) const
{
	e = e;
	return _m9;
}
float Matrix::operator[](m10_enum e) const
{
	e = e;
	return _m10;
}
float Matrix::operator[](m11_enum e) const
{
	e = e;
	return _m11;
}
float Matrix::operator[](m12_enum e) const
{
	e = e;
	return _m12;
}
float Matrix::operator[](m13_enum e) const
{
	e = e;
	return _m13;
}
float Matrix::operator[](m14_enum e) const
{
	e = e;
	return _m14;
}
float Matrix::operator[](m15_enum e) const
{
	e = e;
	return _m15;
}

Matrix& Matrix::operator=(const Matrix& m)
{
	_v0 = m._v0;
	_v1 = m._v1;
	_v2 = m._v2;
	_v3 = m._v3;
	return *this;
}


Matrix::Matrix(const Matrix& m) :_v0(m._v0), _v1(m._v1), _v2(m._v2), _v3(m._v3)
{

}

Matrix Matrix::operator+(Matrix& m)
{
	Matrix returnMatrix(*this);
	returnMatrix._v0.x() += m._v0.x();
	returnMatrix._v0.y() += m._v0.y();
	returnMatrix._v0.z() += m._v0.z();
	returnMatrix._v0.w() += m._v0.w();

	returnMatrix._v1.x() += m._v1.x();
	returnMatrix._v1.y() += m._v1.y();
	returnMatrix._v1.z() += m._v1.z();
	returnMatrix._v1.w() += m._v1.w();

	returnMatrix._v2.x() += m._v2.x();
	returnMatrix._v2.y() += m._v2.y();
	returnMatrix._v2.z() += m._v2.z();
	returnMatrix._v2.w() += m._v2.w();

	returnMatrix._v3.x() += m._v3.x();
	returnMatrix._v3.y() += m._v3.y();
	returnMatrix._v3.z() += m._v3.z();
	returnMatrix._v3.w() += m._v3.w();
	return returnMatrix;
}

Matrix& Matrix::operator+=(Matrix& m)
{
	_v0.x() += m._v0.x();
	_v0.y() += m._v0.y();
	_v0.z() += m._v0.z();
	_v0.w() += m._v0.w();

	_v1.x() += m._v1.x();
	_v1.y() += m._v1.y();
	_v1.z() += m._v1.z();
	_v1.w() += m._v1.w();

	_v2.x() += m._v2.x();
	_v2.y() += m._v2.y();
	_v2.z() += m._v2.z();
	_v2.w() += m._v2.w();

	_v3.x() += m._v3.x();
	_v3.y() += m._v3.y();
	_v3.z() += m._v3.z();
	_v3.w() += m._v3.w();
	return *this;
}

Matrix Matrix::operator-(Matrix& m)
{
	Matrix returnMatrix(*this);
	returnMatrix._v0.x() -= m._v0.x();
	returnMatrix._v0.y() -= m._v0.y();
	returnMatrix._v0.z() -= m._v0.z();
	returnMatrix._v0.w() -= m._v0.w();

	returnMatrix._v1.x() -= m._v1.x();
	returnMatrix._v1.y() -= m._v1.y();
	returnMatrix._v1.z() -= m._v1.z();
	returnMatrix._v1.w() -= m._v1.w();

	returnMatrix._v2.x() -= m._v2.x();
	returnMatrix._v2.y() -= m._v2.y();
	returnMatrix._v2.z() -= m._v2.z();
	returnMatrix._v2.w() -= m._v2.w();

	returnMatrix._v3.x() -= m._v3.x();
	returnMatrix._v3.y() -= m._v3.y();
	returnMatrix._v3.z() -= m._v3.z();
	returnMatrix._v3.w() -= m._v3.w();
	return returnMatrix;
}

Matrix& Matrix::operator-=(Matrix& m)
{
	_v0.x() -= m._v0.x();
	_v0.y() -= m._v0.y();
	_v0.z() -= m._v0.z();
	_v0.w() -= m._v0.w();

	_v1.x() -= m._v1.x();
	_v1.y() -= m._v1.y();
	_v1.z() -= m._v1.z();
	_v1.w() -= m._v1.w();

	_v2.x() -= m._v2.x();
	_v2.y() -= m._v2.y();
	_v2.z() -= m._v2.z();
	_v2.w() -= m._v2.w();

	_v3.x() -= m._v3.x();
	_v3.y() -= m._v3.y();
	_v3.z() -= m._v3.z();
	_v3.w() -= m._v3.w();

	return *this;
}
Matrix Matrix::getT()
{
	Matrix returnMatrix = *this;
	returnMatrix._m1 = _m4;
	returnMatrix._m2 = _m8;
	returnMatrix._m3 = _m12;

	returnMatrix._m4 = _m1;
	returnMatrix._m8 = _m2;
	returnMatrix._m12 = _m3;

	returnMatrix._m6 = _m9;
	returnMatrix._m9 = _m6;

	returnMatrix._m7 = _m13;
	returnMatrix._m13 = _m7;

	returnMatrix._m14 = _m11;
	returnMatrix._m11 = _m14;
	return returnMatrix;
}

float Matrix::det() const
{
	float c0_1 = (_m8*_m13) - (_m12*_m9);
	float c0_2 = (_m8*_m14) - (_m12*_m10);
	float c0_3 = (_m8*_m15) - (_m12*_m11);

	float c1_2 = (_m9*_m14) - (_m13*_m10);
	float c1_3 = (_m9*_m15) - (_m13*_m11);

	float c2_3 = (_m10*_m15) - (_m14*_m11);
	float returnVal = _m0*((_m5*c2_3) - (_m6*c1_3) + (_m7*c1_2));
	returnVal -= _m1*((_m4*c2_3) - (_m6*c0_3) + (_m7*c0_2));
	returnVal += _m2*((_m4*c1_3) - (_m5*c0_3) + (_m7*c0_1));
	returnVal -= _m3*((_m4*c1_2) - (_m5*c0_2) + (_m6*c0_1));
	return returnVal;
}

void Matrix::T()
{
	Matrix returnMatrix = *this;
	returnMatrix._m1 = _m4;
	returnMatrix._m2 = _m8;
	returnMatrix._m3 = _m12;

	returnMatrix._m4 = _m1;
	returnMatrix._m8 = _m2;
	returnMatrix._m12 = _m3;

	returnMatrix._m6 = _m9;
	returnMatrix._m9 = _m6;

	returnMatrix._m7 = _m13;
	returnMatrix._m13 = _m7;

	returnMatrix._m14 = _m11;
	returnMatrix._m11 = _m14;
	*this = returnMatrix;
}

Matrix Matrix::getInv()
{
	float determinant = 1.0f / det();
	Matrix returnMatrix = *this;
	returnMatrix._m0 = ((_m5*_m10*_m15) - (_m5*_m14*_m11)) - ((_m6*_m9*_m15) - (_m6*_m13*_m11)) + ((_m7*_m9*_m14) - (_m7*_m13*_m10));
	returnMatrix._m1 = ((_m4*_m10*_m15) - (_m4*_m14*_m11)) - ((_m6*_m8*_m15) - (_m6*_m12*_m11)) + ((_m7*_m8*_m14) - (_m7*_m12*_m10));
	returnMatrix._m2 = ((_m4*_m9*_m15) - (_m4*_m13*_m11)) - ((_m5*_m8*_m15) - (_m5*_m12*_m11)) + ((_m7*_m8*_m13) - (_m7*_m12*_m9));
	returnMatrix._m3 = ((_m4*_m9*_m14) - (_m4*_m13*_m10)) - ((_m5*_m8*_m14) - (_m5*_m12*_m10)) + ((_m6*_m8*_m13) - (_m6*_m12*_m9));

	returnMatrix._m4 = ((_m1*_m10*_m15) - (_m1*_m14*_m11)) - ((_m2*_m9*_m15) - (_m2*_m13*_m11)) + ((_m3*_m9*_m14) - (_m3*_m13*_m10));
	returnMatrix._m5 = ((_m0*_m10*_m15) - (_m0*_m14*_m11)) - ((_m2*_m8*_m15) - (_m2*_m12*_m11)) + ((_m3*_m8*_m14) - (_m3*_m12*_m10));
	returnMatrix._m6 = ((_m0*_m9*_m15) - (_m0*_m13*_m11)) - ((_m1*_m8*_m15) - (_m1*_m12*_m11)) + ((_m3*_m8*_m13) - (_m3*_m12*_m9));
	returnMatrix._m7 = ((_m0*_m9*_m14) - (_m0*_m13*_m10)) - ((_m1*_m8*_m14) - (_m1*_m12*_m10)) + ((_m2*_m8*_m13) - (_m2*_m12*_m9));

	returnMatrix._m8 = ((_m1*_m6*_m15) - (_m1*_m14*_m7)) - ((_m2*_m5*_m15) - (_m2*_m13*_m7)) + ((_m3*_m5*_m14) - (_m3*_m13*_m6));
	returnMatrix._m9 = ((_m0*_m6*_m15) - (_m0*_m14*_m7)) - ((_m2*_m4*_m15) - (_m2*_m12*_m7)) + ((_m3*_m4*_m14) - (_m3*_m12*_m6));
	returnMatrix._m10 = ((_m0*_m5*_m15) - (_m0*_m13*_m7)) - ((_m1*_m4*_m15) - (_m1*_m12*_m7)) + ((_m3*_m4*_m13) - (_m3*_m12*_m5));
	returnMatrix._m11 = ((_m0*_m5*_m14) - (_m0*_m13*_m6)) - ((_m1*_m4*_m14) - (_m1*_m12*_m6)) + ((_m2*_m4*_m13) - (_m2*_m12*_m5));

	returnMatrix._m12 = ((_m1*_m6*_m11) - (_m1*_m10*_m7)) - ((_m2*_m5*_m11) - (_m2*_m9*_m7)) + ((_m3*_m5*_m10) - (_m3*_m9*_m6));
	returnMatrix._m13 = ((_m0*_m6*_m11) - (_m0*_m10*_m7)) - ((_m2*_m4*_m11) - (_m2*_m8*_m7)) + ((_m3*_m4*_m10) - (_m3*_m8*_m6));
	returnMatrix._m14 = ((_m0*_m5*_m11) - (_m0*_m9*_m7)) - ((_m1*_m4*_m11) - (_m1*_m8*_m7)) + ((_m3*_m4*_m9) - (_m3*_m8*_m5));
	returnMatrix._m15 = ((_m0*_m5*_m10) - (_m0*_m9*_m6)) - ((_m1*_m4*_m10) - (_m1*_m8*_m6)) + ((_m2*_m4*_m9) - (_m2*_m8*_m5));
	returnMatrix.T();
	returnMatrix._m1 = -returnMatrix._m1;
	returnMatrix._m3 = -returnMatrix._m3;
	returnMatrix._m4 = -returnMatrix._m4;
	returnMatrix._m6 = -returnMatrix._m6;
	returnMatrix._m9 = -returnMatrix._m9;
	returnMatrix._m11 = -returnMatrix._m11;
	returnMatrix._m12 = -returnMatrix._m12;
	returnMatrix._m14 = -returnMatrix._m14;

	returnMatrix._m0 *= determinant;
	returnMatrix._m1 *= determinant;
	returnMatrix._m2 *= determinant;
	returnMatrix._m3 *= determinant;
	returnMatrix._m4 *= determinant;
	returnMatrix._m5 *= determinant;
	returnMatrix._m6 *= determinant;
	returnMatrix._m7 *= determinant;
	returnMatrix._m8 *= determinant;
	returnMatrix._m9 *= determinant;
	returnMatrix._m10 *= determinant;
	returnMatrix._m11 *= determinant;
	returnMatrix._m12 *= determinant;
	returnMatrix._m13 *= determinant;
	returnMatrix._m14 *= determinant;
	returnMatrix._m15 *= determinant;
	return returnMatrix;
}

void Matrix::inv()
{
	float determinant = 1.0f / det();
	Matrix returnMatrix = *this;
	returnMatrix._m0 = ((_m5*_m10*_m15) - (_m5*_m14*_m11)) - ((_m6*_m9*_m15) - (_m6*_m13*_m11)) + ((_m7*_m9*_m14) - (_m7*_m13*_m10));
	returnMatrix._m1 = ((_m4*_m10*_m15) - (_m4*_m14*_m11)) - ((_m6*_m8*_m15) - (_m6*_m12*_m11)) + ((_m7*_m8*_m14) - (_m7*_m12*_m10));
	returnMatrix._m2 = ((_m4*_m9*_m15) - (_m4*_m13*_m11)) - ((_m5*_m8*_m15) - (_m5*_m12*_m11)) + ((_m7*_m8*_m13) - (_m7*_m12*_m9));
	returnMatrix._m3 = ((_m4*_m9*_m14) - (_m4*_m13*_m10)) - ((_m5*_m8*_m14) - (_m5*_m12*_m10)) + ((_m6*_m8*_m13) - (_m6*_m12*_m9));

	returnMatrix._m4 = ((_m1*_m10*_m15) - (_m1*_m14*_m11)) - ((_m2*_m9*_m15) - (_m2*_m13*_m11)) + ((_m3*_m9*_m14) - (_m3*_m13*_m10));
	returnMatrix._m5 = ((_m0*_m10*_m15) - (_m0*_m14*_m11)) - ((_m2*_m8*_m15) - (_m2*_m12*_m11)) + ((_m3*_m8*_m14) - (_m3*_m12*_m10));
	returnMatrix._m6 = ((_m0*_m9*_m15) - (_m0*_m13*_m11)) - ((_m1*_m8*_m15) - (_m1*_m12*_m11)) + ((_m3*_m8*_m13) - (_m3*_m12*_m9));
	returnMatrix._m7 = ((_m0*_m9*_m14) - (_m0*_m13*_m10)) - ((_m1*_m8*_m14) - (_m1*_m12*_m10)) + ((_m2*_m8*_m13) - (_m2*_m12*_m9));

	returnMatrix._m8 = ((_m1*_m6*_m15) - (_m1*_m14*_m7)) - ((_m2*_m5*_m15) - (_m2*_m13*_m7)) + ((_m3*_m5*_m14) - (_m3*_m13*_m6));
	returnMatrix._m9 = ((_m0*_m6*_m15) - (_m0*_m14*_m7)) - ((_m2*_m4*_m15) - (_m2*_m12*_m7)) + ((_m3*_m4*_m14) - (_m3*_m12*_m6));
	returnMatrix._m10 = ((_m0*_m5*_m15) - (_m0*_m13*_m7)) - ((_m1*_m4*_m15) - (_m1*_m12*_m7)) + ((_m3*_m4*_m13) - (_m3*_m12*_m5));
	returnMatrix._m11 = ((_m0*_m5*_m14) - (_m0*_m13*_m6)) - ((_m1*_m4*_m14) - (_m1*_m12*_m6)) + ((_m2*_m4*_m13) - (_m2*_m12*_m5));

	returnMatrix._m12 = ((_m1*_m6*_m11) - (_m1*_m10*_m7)) - ((_m2*_m5*_m11) - (_m2*_m9*_m7)) + ((_m3*_m5*_m10) - (_m3*_m9*_m6));
	returnMatrix._m13 = ((_m0*_m6*_m11) - (_m0*_m10*_m7)) - ((_m2*_m4*_m11) - (_m2*_m8*_m7)) + ((_m3*_m4*_m10) - (_m3*_m8*_m6));
	returnMatrix._m14 = ((_m0*_m5*_m11) - (_m0*_m9*_m7)) - ((_m1*_m4*_m11) - (_m1*_m8*_m7)) + ((_m3*_m4*_m9) - (_m3*_m8*_m5));
	returnMatrix._m15 = ((_m0*_m5*_m10) - (_m0*_m9*_m6)) - ((_m1*_m4*_m10) - (_m1*_m8*_m6)) + ((_m2*_m4*_m9) - (_m2*_m8*_m5));
	returnMatrix.T();
	returnMatrix._m1 = -returnMatrix._m1;
	returnMatrix._m3 = -returnMatrix._m3;
	returnMatrix._m4 = -returnMatrix._m4;
	returnMatrix._m6 = -returnMatrix._m6;
	returnMatrix._m9 = -returnMatrix._m9;
	returnMatrix._m11 = -returnMatrix._m11;
	returnMatrix._m12 = -returnMatrix._m12;
	returnMatrix._m14 = -returnMatrix._m14;

	returnMatrix._m0 *= determinant;
	returnMatrix._m1 *= determinant;
	returnMatrix._m2 *= determinant;
	returnMatrix._m3 *= determinant;
	returnMatrix._m4 *= determinant;
	returnMatrix._m5 *= determinant;
	returnMatrix._m6 *= determinant;
	returnMatrix._m7 *= determinant;
	returnMatrix._m8 *= determinant;
	returnMatrix._m9 *= determinant;
	returnMatrix._m10 *= determinant;
	returnMatrix._m11 *= determinant;
	returnMatrix._m12 *= determinant;
	returnMatrix._m13 *= determinant;
	returnMatrix._m14 *= determinant;
	returnMatrix._m15 *= determinant;
	*this = returnMatrix;
}

Matrix Matrix::operator*(const Matrix& m)
{
	Matrix returnMatrix;

	returnMatrix._m0 = (_m0*m._m0) + (_m1*m._m4) + (_m2*m._m8) + (_m3*m._m12);
	returnMatrix._m1 = (_m0*m._m1) + (_m1*m._m5) + (_m2*m._m9) + (_m3*m._m13);
	returnMatrix._m2 = (_m0*m._m2) + (_m1*m._m6) + (_m2*m._m10) + (_m3*m._m14);
	returnMatrix._m3 = (_m0*m._m3) + (_m1*m._m7) + (_m2*m._m11) + (_m3*m._m15);

	returnMatrix._m4 = (_m4*m._m0) + (_m5*m._m4) + (_m6*m._m8) + (_m7*m._m12);
	returnMatrix._m5 = (_m4*m._m1) + (_m5*m._m5) + (_m6*m._m9) + (_m7*m._m13);
	returnMatrix._m6 = (_m4*m._m2) + (_m5*m._m6) + (_m6*m._m10) + (_m7*m._m14);
	returnMatrix._m7 = (_m4*m._m3) + (_m5*m._m7) + (_m6*m._m11) + (_m7*m._m15);

	returnMatrix._m8 = (_m8*m._m0) + (_m9*m._m4) + (_m10*m._m8) + (_m11*m._m12);
	returnMatrix._m9 = (_m8*m._m1) + (_m9*m._m5) + (_m10*m._m9) + (_m11*m._m13);
	returnMatrix._m10 = (_m8*m._m2) + (_m9*m._m6) + (_m10*m._m10) + (_m11*m._m14);
	returnMatrix._m11 = (_m8*m._m3) + (_m9*m._m7) + (_m10*m._m11) + (_m11*m._m15);

	returnMatrix._m12 = (_m12*m._m0) + (_m13*m._m4) + (_m14*m._m8) + (_m15*m._m12);
	returnMatrix._m13 = (_m12*m._m1) + (_m13*m._m5) + (_m14*m._m9) + (_m15*m._m13);
	returnMatrix._m14 = (_m12*m._m2) + (_m13*m._m6) + (_m14*m._m10) + (_m15*m._m14);
	returnMatrix._m15 = (_m12*m._m3) + (_m13*m._m7) + (_m14*m._m11) + (_m15*m._m15);

	return returnMatrix;
}


Matrix& Matrix::operator*=(Matrix& m)
{
	Matrix returnMatrix;

	returnMatrix._m0 = (_m0*m._m0) + (_m1*m._m4) + (_m2*m._m8) + (_m3*m._m12);
	returnMatrix._m1 = (_m0*m._m1) + (_m1*m._m5) + (_m2*m._m9) + (_m3*m._m13);
	returnMatrix._m2 = (_m0*m._m2) + (_m1*m._m6) + (_m2*m._m10) + (_m3*m._m14);
	returnMatrix._m3 = (_m0*m._m3) + (_m1*m._m7) + (_m2*m._m11) + (_m3*m._m15);

	returnMatrix._m4 = (_m4*m._m0) + (_m5*m._m4) + (_m6*m._m8) + (_m7*m._m12);
	returnMatrix._m5 = (_m4*m._m1) + (_m5*m._m5) + (_m6*m._m9) + (_m7*m._m13);
	returnMatrix._m6 = (_m4*m._m2) + (_m5*m._m6) + (_m6*m._m10) + (_m7*m._m14);
	returnMatrix._m7 = (_m4*m._m3) + (_m5*m._m7) + (_m6*m._m11) + (_m7*m._m15);

	returnMatrix._m8 = (_m8*m._m0) + (_m9*m._m4) + (_m10*m._m8) + (_m11*m._m12);
	returnMatrix._m9 = (_m8*m._m1) + (_m9*m._m5) + (_m10*m._m9) + (_m11*m._m13);
	returnMatrix._m10 = (_m8*m._m2) + (_m9*m._m6) + (_m10*m._m10) + (_m11*m._m14);
	returnMatrix._m11 = (_m8*m._m3) + (_m9*m._m7) + (_m10*m._m11) + (_m11*m._m15);

	returnMatrix._m12 = (_m12*m._m0) + (_m13*m._m4) + (_m14*m._m8) + (_m15*m._m12);
	returnMatrix._m13 = (_m12*m._m1) + (_m13*m._m5) + (_m14*m._m9) + (_m15*m._m13);
	returnMatrix._m14 = (_m12*m._m2) + (_m13*m._m6) + (_m14*m._m10) + (_m15*m._m14);
	returnMatrix._m15 = (_m12*m._m3) + (_m13*m._m7) + (_m14*m._m11) + (_m15*m._m15);

	*this = returnMatrix;
	return *this;
}

Matrix Matrix::operator*(const Quat& m)
{
	//ToDo
	MATH_UNUSED(m);
	Matrix q(m);
	return *this*q;
}

bool Matrix::isIdentity(const float tolerance)
{
	if (_m0 - 1.0f < -tolerance && _m0 - 1.0f > tolerance)
	{
		return false;
	}
	if (_m1 > tolerance)
	{
		return false;
	}
	if (_m2 > tolerance)
	{
		return false;
	}
	if (_m3 > tolerance)
	{
		return false;
	}
	if (_m4 > tolerance)
	{
		return false;
	}
	if (_m5 - 1.0f < -tolerance && _m5 - 1.0f > tolerance)
	{
		return false;
	}
	if (_m6 > tolerance)
	{
		return false;
	}
	if (_m7 > tolerance)
	{
		return false;
	}
	if (_m8 > tolerance)
	{
		return false;
	}
	if (_m9 > tolerance)
	{
		return false;
	}
	if (_m10 - 1.0f < -tolerance && _m10 - 1.0f > tolerance)
	{
		return false;
	}
	if (_m11 > tolerance)
	{
		return false;
	}
	if (_m12 > tolerance)
	{
		return false;
	}
	if (_m13 > tolerance)
	{
		return false;
	}
	if (_m14 > tolerance)
	{
		return false;
	}
	if (_m15 - 1.0f < -tolerance && _m15 - 1.0f > tolerance)
	{
		return false;
	}
	return true;
}



void Matrix::set(MatrixSpecialType matrixType)
{
	switch (matrixType)
	{
	case MatrixSpecialType::IDENTITY:
		_v0 = Vect(1.0f, 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, 1.0f, 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, 1.0f, 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case MatrixSpecialType::ZERO:
		_v0 = Vect(0.0f, 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, 0.0f, 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, 0.0f, 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 0.0f);
		break;
	default:
		break;
	}
}

void Matrix::set(MatrixTransType trans, float x, float y, float z)
{
	switch (trans)
	{
	case MatrixTransType::TRANS:
		_v0 = Vect(1.0f, 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, 1.0f, 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, 1.0f, 0.0f);
		_v3.set(x, y, z);
		break;
	default:
		break;
	}
}

void Matrix::set(MatrixTransType trans, Vect& v)
{
	switch (trans)
	{
	case MatrixTransType::TRANS:
		_v0 = Vect(1.0f, 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, 1.0f, 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, 1.0f, 0.0f);
		_v3 = Vect(v.x(), v.y(), v.z(), 1.0f);
		break;
	default:
		break;
	}
}

void Matrix::set(MatrixScaleType scale, float x, float y, float z)
{
	switch (scale)
	{
	case MatrixScaleType::SCALE:
		_v0 = Vect(x, 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, y, 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, z, 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
}

void Matrix::set(MatrixScaleType scale, Vect& v)
{
	switch (scale)
	{
	case MatrixScaleType::SCALE:
		_v0 = Vect(v.x(), 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, v.y(), 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, v.z(), 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
}

void Matrix::set(RotType rotType, float rot)
{
	float sinRot = sinf(rot);
	float cosRot = cosf(rot);
	switch (rotType)
	{
	case RotType::ROT_X:
		_v0 = Vect(1.0f, 0.0f, 0.0f, 0.0f);
		_v1 = Vect(0.0f, cosRot, sinRot, 0.0f);
		_v2 = Vect(0.0f, -sinRot, cosRot, 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case RotType::ROT_Y:
		_v0 = Vect(cosRot, 0.0f, -sinRot, 0.0f);
		_v1 = Vect(0.0f, 1.0f, 0.0f, 0.0f);
		_v2 = Vect(sinRot, 0.0f, cosRot, 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case RotType::ROT_Z:
		_v0 = Vect(cosRot, sinRot, 0.0f, 0.0f);
		_v1 = Vect(-sinRot, cosRot, 0.0f, 0.0f);
		_v2 = Vect(0.0f, 0.0f, 1.0f, 0.0f);
		_v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
}

void Matrix::set(MatrixRowType rowType, Vect& v)
{
	switch (rowType)
	{
	case MatrixRowType::ROW_0:
		_v0 = v;
		break;
	case MatrixRowType::ROW_1:
		_v1 = v;
		break;
	case MatrixRowType::ROW_2:
		_v2 = v;
		break;
	case MatrixRowType::ROW_3:
		_v3 = v;
		break;
	default:
		break;
	}
}

Vect Matrix::get(MatrixRowType rowType)
{
	switch (rowType)
	{
	case MatrixRowType::ROW_0:
		return _v0;
	case MatrixRowType::ROW_1:
		return _v1;
	case MatrixRowType::ROW_2:
		return _v2;
	case MatrixRowType::ROW_3:
		return _v3;
	default:
		return Vect();
	}
}

void Matrix::set(Vect& v0, Vect& v1, Vect& v2, Vect& v3)
{
	_v0 = v0;
	_v1 = v1;
	_v2 = v2;
	_v3 = v3;
}
void Matrix::set(const Quat &q)
{
	//ToDo
	MATH_UNUSED(q);
	float xy = q.qx()*q.qy();
	float xz = q.qx()*q.qz();
	float xw = q.qx()*q.real();
	float yz = q.qy()*q.qz();
	float yw = q.qy()*q.real();
	float zw = q.qz()*q.real();
	_v0.set(1.0f - (2.0f * (q.qy()*q.qy() + q.qz()*q.qz())), (xy + zw)*2.0f, (xz*2.0f - yw*2.0f), 0.0f);
	_v1.set(2.0f*(xy - zw), 1.0f - (2.0f * (q.qx()*q.qx() + q.qz()*q.qz())), (yz + xw)*2.0f, 0.0f);
	_v2.set((xz + yw)*2.0f, (yz - xw)*2.0f, 1.0f - (2.0f * (q.qx()*q.qx() + q.qy()*q.qy())), 0.0f);
	_v3.set(0.0f, 0.0f, 0.0f, 1.0f);
}

bool Matrix::isEqual(Matrix& m)
{
	if (_m0 != m._m0)
	{
		return false;
	}
	if (_m1 != m._m1)
	{
		return false;
	}
	if (_m2 != m._m2)
	{
		return false;
	}
	if (_m3 != m._m3)
	{
		return false;
	}
	if (_m4 != m._m4)
	{
		return false;
	}
	if (_m5 != m._m5)
	{
		return false;
	}
	if (_m6 != m._m6)
	{
		return false;
	}
	if (_m7 != m._m7)
	{
		return false;
	}
	if (_m8 != m._m8)
	{
		return false;
	}
	if (_m9 != m._m9)
	{
		return false;
	}
	if (_m10 != m._m10)
	{
		return false;
	}
	if (_m11 != m._m11)
	{
		return false;
	}
	if (_m12 != m._m12)
	{
		return false;
	}
	if (_m13 != m._m13)
	{
		return false;
	}
	if (_m14 != m._m14)
	{
		return false;
	}
	if (_m15 != m._m15)
	{
		return false;
	}
	return true;
}

void Matrix::set(Rot3AxisType type, const float x, const float y, const float z)
{

	MATH_UNUSED(type);
	MATH_UNUSED(x);
	MATH_UNUSED(y);
	MATH_UNUSED(z);
	Matrix matX(RotType::ROT_X, x);
	Matrix matY(RotType::ROT_Y, y);
	Matrix matZ(RotType::ROT_Z, z);
	*this = matX * matY * matZ;
}
void Matrix::set(RotOrientType type, const Vect &v1, const Vect & v2)
{
	MATH_UNUSED(type);
	MATH_UNUSED(v1);
	MATH_UNUSED(v2);
	Vect v;
	Vect v1temp = v1;
	v1temp.norm();
	v1temp.w() = 0.0f;
	Vect v2temp = v2;
	v2temp.norm();
	v2temp.w() = 0.0f;
	//if (type == RotOrientType::ROT_ORIENT)
	//{
		v = v2temp;
		v = v.cross(v1temp);
		_v1 = v2temp;
		_v2 = v1temp;
	//}
/*	else
	{
		v = v2temp;
		v = v.cross(v1temp);
		_v1 = v1temp;
		_v2 = v2temp;
	}*/
	v.w() = 0.0f;
	_v0 = v;
	_v3.set(0.0f, 0.0f, 0.0f, 1.0f);
	if (RotOrientType::ROT_INVERSE_ORIENT == type)
	{
		this->inv();
	}
}
void Matrix::set(RotAxisAngleType setting, const Vect& v1, const float angle)
{
	MATH_UNUSED(setting);
	MATH_UNUSED(v1);
	MATH_UNUSED(angle);
	Vect v = v1;
	v.norm();
	Vect r1(v.x() * v.x(), v.x() * v.y(), v.x() * v.z(), 0.0f);
	Vect r2(v.y() * v.x(), v.y() * v.y(), v.y() * v.z(), 0.0f);
	Vect r3(v.z() * v.x(), v.z() * v.y(), v.z() * v.z(), 0.0f);
	Vect r4;
	r1 -= Vect(cosf(angle) * r1);
	r2 -= Vect(cosf(angle) * r2);
	r3 -= Vect(cosf(angle) * r3);
	//r4 -= Vect(cosf(angle) * r4);
	r1 += Vect(cosf(angle), sinf(angle)*v.z(), -sinf(angle)*v.y(), 0.0f);
	r2 += Vect(-sinf(angle)*v.z(), cosf(angle), sinf(angle)*v.x(), 0.0f);
	r3 += Vect(sinf(angle)*v.y(), -sinf(angle)*v.x(), cosf(angle), 0.0f);
	r1.w() = 0.0f;
	r2.w() = 0.0f;
	r3.w() = 0.0f;
	set(ROW_0, r1);
	set(ROW_1, r2);
	set(ROW_2, r3);
	set(ROW_3, r4);
}

bool Matrix::isIdentity()
{
	if (_m0 - 1.0f < -MATH_TOLERANCE && _m0 - 1.0f > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m1 > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m2 > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m3 > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m4 > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m5 - 1.0f < -MATH_TOLERANCE && _m5 - 1.0f > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m6 > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m7 > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m8 > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m9 > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m10 - 1.0f < -MATH_TOLERANCE && _m10 - 1.0f > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m11 > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m12 > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m13 > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m14 > MATH_TOLERANCE)
	{
		return false;
	}
	if (_m15 - 1.0f < -MATH_TOLERANCE && _m15 - 1.0f > MATH_TOLERANCE)
	{
		return false;
	}
	return true;
}

Matrix Matrix::operator+()
{
	Matrix m = *this;
	return m;
}
Matrix Matrix::operator-()
{
	Matrix m = *this;
	m._m0 = -m._m0;
	m._m1 = -m._m1;
	m._m2 = -m._m2;
	m._m3 = -m._m3;
	m._m4 = -m._m4;
	m._m5 = -m._m5;
	m._m6 = -m._m6;
	m._m7 = -m._m7;
	m._m8 = -m._m8;
	m._m9 = -m._m9;
	m._m10 = -m._m10;
	m._m11 = -m._m11;
	m._m12 = -m._m12;
	m._m13 = -m._m13;
	m._m14 = -m._m14;
	m._m15 = -m._m15;
	return m;
}

