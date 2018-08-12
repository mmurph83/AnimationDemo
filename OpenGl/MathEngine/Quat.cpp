//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Gam575
//----------------------------------------------------------------------------- 

#include <math.h>
#include <assert.h>

#include "MathEngine.h"

// Do your magic here
Quat::Quat():_qx(0.f), _qy(0.f), _qz(0.f), _qw(1.f)
{

}
Quat::Quat(float x, float y, float z, float w):_qx(x),_qy(y),_qz(z),_qw(w)
{

}
Quat::Quat(const Vect & v, float w): _qx(v.x()), _qy(v.y()), _qz(v.z()), _qw(w)
{
	
}
Quat::Quat(const Quat &q):_mq(q._mq)
{

}
Quat::Quat(const Matrix & m)
{
	MATH_UNUSED(m);
	float tr = m.m0() + m.m5() + m.m10();
	float s;
	if (tr > 0.0f)
	{
		s = sqrtf(1.0f + tr) * 2.0f;
		_qw = 0.25f * s;
		_qx = (-m.m9() + m.m6()) / s;
		_qy = (-m.m2() + m.m8()) / s;
		_qz = (-m.m4() + m.m1()) / s;
	}
	else if (m[m0] > m[m5] && m[m0] > m[m10])
	{
		s = sqrtf(1.0f + m.m0() - m.m5() - m.m10()) * 2.0f;
		_qx = 0.25f * s;
		_qw = -(m.m9() - m.m6()) / s;
		_qz = (m.m2() + m.m8()) / s;
		_qy = (m.m4() + m.m1()) / s;
	}
	else if (m[m5] > m[m10])
	{
		s = sqrtf(1.0f - m.m0() + m.m5() - m.m10()) * 2.0f;
		_qy = 0.25f * s;
		_qz = (m.m9() + m.m6()) / s;
		_qw = -(m.m2() - m.m8()) / s;
		_qx = (m.m4() + m.m1()) / s;
	}
	else
	{
		//ToDo
		s = sqrtf(1.0f + m[m10] - m[m0] - m[m5]) * 2.0f;
		_qw = -(m[m4] - m[m1]) / s;
		_qx = (m[m2] + m[m8]) / s;
		_qy = (m[m6] + m[m9]) / s;
		_qz = 0.25f *s;
	}
}
Quat::Quat(const RotType& type, const float& angle)
{
	switch (type)
	{
	case ROT_X:
		_qx = sinf(angle *0.5f);
		_qy = 0.0f;
		_qz = 0.0f;
		_qw = cosf(angle * 0.5f);
		break;
	case ROT_Y:
		_qy = sinf(angle *0.5f);
		_qx = 0.0f;
		_qz = 0.0f;
		_qw = cosf(angle * 0.5f);
		break;
	case ROT_Z:
		_qz = sinf(angle *0.5f);
		_qy = 0.0f;
		_qx = 0.0f;
		_qw = cosf(angle * 0.5f);
		break;
	default:
		break;
	}
}
Quat::Quat(const RotAxisAngleType type, const Vect & v, float angle)
{
	this->set(type, v, angle);
}
Quat::Quat(RotOrientType type, const Vect &v1, const Vect &v2)
{
	this->set(Matrix(type, v1, v2));
}
Quat::Quat(Rot3AxisType type, float x, float y, float z)
{
	this->set(Matrix(type, x, y, z));
}
void Quat::set(RotType type, float angle)
{
	MATH_UNUSED(angle);
	//Matrix m(type, angle);
	switch (type)
	{
	case ROT_X:
		_qx = sinf(angle *0.5f);
		_qy = 0.0f;
		_qz = 0.0f;
		_qw = cosf(angle * 0.5f);
		break;
	case ROT_Y:
		_qy = sinf(angle *0.5f);
		_qx = 0.0f;
		_qz = 0.0f;
		_qw = cosf(angle * 0.5f);
		break;
	case ROT_Z:
		_qz = sinf(angle *0.5f);
		_qy = 0.0f;
		_qx = 0.0f;
		_qw = cosf(angle * 0.5f);
		break;
	default:
		break;
	}
}

void Quat::set(const Matrix &m)
{
	float tr = m.m0() + m.m5() + m.m10();
	float s;
	if (m.det() > 1.0f - MATH_TOLERANCE && m.det() < 1.0f + MATH_TOLERANCE)
	{
		if (tr > 0.0f)
		{
			s = sqrtf(1.0f + tr) * 2.0f;
			_qw = 0.25f * s;
			_qx = (-m.m9() + m.m6()) / s;
			_qy = (-m.m2() + m.m8()) / s;
			_qz = (-m.m4() + m.m1()) / s;
		}
		else if (m[m0] > m[m5] && m[m0] > m[m10])
		{
			s = sqrtf(1.0f + m.m0() - m.m5() - m.m10()) * 2.0f;
			_qx = 0.25f * s;
			_qw = -(m.m9() - m.m6()) / s;
			_qz = (m.m2() + m.m8()) / s;
			_qy = (m.m4() + m.m1()) / s;
		}
		else if (m[m5] > m[m10])
		{
			s = sqrtf(1.0f - m.m0() + m.m5() - m.m10()) * 2.0f;
			_qy = 0.25f * s;
			_qz = (m.m9() + m.m6()) / s;
			_qw = -(m.m2() - m.m8()) / s;
			_qx = (m.m4() + m.m1()) / s;
		}
		else
		{
			//ToDo
			s = sqrtf(1.0f + m[m10] - m[m0] - m[m5]) * 2.0f;
			_qw = -(m[m4] - m[m1]) / s;
			_qx = (m[m2] + m[m8]) / s;
			_qy = (m[m6] + m[m9]) / s;
			_qz = 0.25f *s;
		}
	}
	else
	{
		_qx = 0.0f;
		_qy = 0.0f;
		_qz = 0.0f;
		_qw = 0.0f;
	}
	MATH_UNUSED(m);
}

void Quat::set(float x, float y, float z, float w)
{
	_qx = x;
	_qy = y;
	_qz = z;
	_qw = w;
}
void Quat::set(const Quat &q)
{
	_qx = q._qx;
	_qz = q._qz;
	_qy = q._qy;
	_qw = q._qw;
}

void Quat::set(RotAxisAngleType setting, const Vect& v, const float angle)
{
	MATH_UNUSED(setting);
	MATH_UNUSED(v);
	MATH_UNUSED(angle);
	Matrix m(setting, v, angle);
	set(m);
}

void Quat::set(Rot3AxisType type, float x, float y, float z)
{
	this->set(Matrix(type, x, y, z));
}
void Quat::set(const Vect &v, const float fl)
{
	_qx = v[x];
	_qy = v[y];
	_qz = v[z];
	_qw = fl;
}
void Quat::set(RotOrientType type, const Vect &v1, const Vect &v2)
{
	this->set(Matrix(type, v1, v2));
}
void Quat::setVect(const Vect &v)
{
	_qx = v[x];
	_qy = v[y];
	_qz = v[z];
}
void Quat::getVect(Vect &v)
{
	v[x] = _qx;
	v[y] = _qy;
	v[z] = _qz;
}

float Quat::getAngle()
{
	return 2.0f*acosf(_qw);
}

void Quat::getAxis(Vect &v)
{
	v[x] = _qx;
	v[y] = _qy;
	v[z] = _qz;
	v[w] = 1.0f;
}

Quat Quat::operator +(const float& fl)
{
	return Quat(_qx + fl, _qy + fl, _qz + fl, _qw + fl);
}
Quat Quat::operator -(const float& fl)
{
	return Quat(_qx - fl, _qy - fl, _qz - fl, _qw - fl);
}

Quat Quat::operator +(const Quat& q)
{
	return Quat(_qx + q._qx, _qy + q._qy, _qz + q._qz, _qw + q._qw);
}
Quat Quat::operator -(const Quat& q)
{
	return Quat(_qx - q._qx, _qy - q._qy, _qz - q._qz, _qw - q._qw);
}
Quat &Quat::operator +=(const Quat& q)
{
	_qx += q._qx;
	_qy += q._qy;
	_qz += q._qz;
	_qw += q._qw;
	return *this;
}

Quat &Quat::operator -=(const Quat& q)
{
	_qx -= q._qx;
	_qy -= q._qy;
	_qz -= q._qz;
	_qw -= q._qw;
	return *this;
}

Quat & Quat::operator+=(const float &fl)
{
	_qx += fl;
	_qy += fl;
	_qz += fl;
	_qw += fl;
	return *this;
}
Quat & Quat::operator-=(const float &fl)
{
	_qx -= fl;
	_qy -= fl;
	_qz -= fl;
	_qw -= fl;
	return *this;
}

Quat &Quat::operator *=(const Quat& q)
{
	//ToDo
	*this = Quat((_qw * q._qx) + (_qx * q._qw) - (_qy * q._qz) + (_qz * q._qy), (_qw * q._qy) + (_qx * q._qz) + (_qy * q._qw) - (_qz * q._qx), (_qw * q._qz) - (_qx * q._qy) + (_qy * q._qx) + (_qz * q._qw), (_qw * q._qw) - (_qx * q._qx) - (_qy * q._qy) - (_qz * q._qz));
	return *this;
}
Quat Quat::operator /(const Quat& q)
{
	return Quat(_qx / q._qx,_qy / q._qy, _qz / q._qz, _qw / q._qw);
}

Quat &Quat::operator /=(const Quat& q)
{
	_qx /= q._qx;
	_qy /= q._qy;
	_qz /= q._qz;
	_qw /= q._qw;
	return *this;
}
Quat &Quat::operator /=(const float &fl)
{
	_qx /= fl;
	_qy /= fl;
	_qz /= fl;
	_qw /= fl;
	return *this;
}

Quat Quat::operator *(const Quat& q) const
{
	float x = (_qw * q._qx) + (_qx * q._qw) - (_qy * q._qz) + (_qz * q._qy);
	return Quat(x, (_qw * q._qy) + (_qx * q._qz) + (_qy * q._qw) - (_qz * q._qx), (_qw * q._qz) - (_qx * q._qy) + (_qy * q._qx) + (_qz * q._qw), (_qw * q._qw) - (_qx * q._qx) - (_qy * q._qy) - (_qz * q._qz));
}

Quat &Quat::operator =(const Quat &q)
{
	_qx = q._qx;
	_qy = q._qy;
	_qz = q._qz;
	_qw = q._qw;
	return *this;
}
Quat &Quat::operator *=(const float& fl)
{
	_qx *= fl;
	_qy *= fl;
	_qz *= fl;
	_qw *= fl;
	return *this;
}
Quat Quat::operator /(const float& fl)
{
	return Quat(_qx / fl, _qy / fl, _qz / fl, _qw / fl);
}


Quat Quat::operator *(const float& fl) const
{
	return Quat(_qx * fl, _qy * fl, _qz * fl, _qw * fl);
}

Quat &Quat::operator *=(const Matrix& m)
{
	//ToDO
	MATH_UNUSED(m);
	Quat q(m);
	*this = *this * q;
	return *this;
}

Matrix Quat::operator *(const Matrix& m)
{
	//ToDo
	MATH_UNUSED(m);
	Matrix tm(*this);
	return tm * m;
}

Quat &Quat::operator =(const Matrix& m)
{
	*this = Quat(m);
	return *this;
}

Quat Quat::operator +()
{
	return Quat(*this);
}

Quat Quat::operator -()
{
	
	return Quat(-_qx,-_qy,-_qz,-_qw);
}


float &Quat::operator[](x_enum e)
{
	MATH_UNUSED(e);
	return _qx;
}
float &Quat::operator[](y_enum e)
{
	MATH_UNUSED(e);
	return _qy;
}
float &Quat::operator[](z_enum e)
{
	MATH_UNUSED(e);
	return _qz;
}
float &Quat::operator[](w_enum e)
{
	MATH_UNUSED(e);
	return _qw;
}
float Quat::operator[](x_enum e) const
{
	MATH_UNUSED(e);
	return _qx;
}
float Quat::operator[](y_enum e) const
{
	MATH_UNUSED(e);
	return _qy;
}
float Quat::operator[](z_enum e) const
{
	MATH_UNUSED(e);
	return _qz;
}
float Quat::operator[](w_enum e) const
{
	MATH_UNUSED(e);
	return _qw;
}


float Quat::qx()const
{
	return _qx;
}
float Quat::qy()const
{
	return _qy;
}
float Quat::qz()const
{
	return _qz;
}
float Quat::real()const
{
	return _qw;
}

float &Quat::qx()
{
	return _qx;
}
float &Quat::qy()
{
	return _qy;
}
float &Quat::qz()
{
	return _qz;
}
float &Quat::real()
{
	return _qw;
}
Quat Quat::multByElement(const Quat &q)
{
	return Quat(_qx * q._qx, _qy * q._qy, _qz * q._qz, _qw * q._qw);
}

bool Quat::isEqual(const Quat& q, const float &f) const
{
	if (fabsf(_qx - q._qx) > f)
	{
		return false;
	}
	if (fabsf(_qy - q._qy) > f)
	{
		return false;
	}
	if (fabsf(_qz - q._qz) > f)
	{
		return false;
	}
	if (fabsf(_qw - q._qw) > f)
	{
		return false;
	}
	return true;
}

bool Quat::isNegEqual(const Quat& q, const float &f) const
{
	if (fabsf(_qx + q._qx) > f)
	{
		return false;
	}
	if (fabsf(_qy + q._qy) > f)
	{
		return false;
	}
	if (fabsf(_qz + q._qz) > f)
	{
		return false;
	}
	if (fabsf(_qw + q._qw) > f)
	{
		return false;
	}
	return true;
}
bool Quat::isEquivalent(const Quat& q, const float &f) const
{
	float t1 = _qx;
	t1 += _qy;
	t1 += _qz;
	t1 += _qw;
	float t2 = q._qx;
	t2 += q._qy;
	t2 += q._qz;
	t2 += q._qw;
	t1 = fabsf(t1);
	t2 = fabsf(t2);
	if (t1 - t2 > f || t2 - t1 > f)
	{
		return false;
	}
	return true;
}
bool Quat::isConjugateEqual(const Quat& q, const float &f) const
{
	if (_qx + q._qx > f || _qx + q._qx < -f)
	{
		return false;
	}
	if (_qy + q._qy > f || _qy + q._qy < -f)
	{
		return false;
	}
	if (_qz + q._qz > f || _qz + q._qz < -f)
	{
		return false;
	}
	if (_qw - q._qw > f || _qw - q._qw < -f)
	{
		return false;
	}
	return true;
}

bool Quat::isIdentity(const float &f) const
{
	if (fabsf(_qx) > f && _qx > 0.0f)
	{
		return false;
	}
	if (fabsf(_qy) > f && _qy > 0.0f)
	{
		return false;
	}
	if (fabsf(_qz) > f && _qz > 0.0f)
	{
		return false;
	}
	if (fabsf(_qw) > 1.0f + f &&  _qw > 0.0f)
	{
		return false;
	}
	return true;
}

bool Quat::isNormalized(const float &f) const
{
	float norm = this->mag();
	if (norm > f + 1.0f)
	{
		return false;
	}
	return true;
}
bool Quat::isZero(const float &f) const
{
	if (fabsf(_qx) > f && _qx > 0.0f)
	{
		return false;
	}
	if (fabsf(_qy) > f && _qy > 0.0f)
	{
		return false;
	}
	if (fabsf(_qz) > f && _qz > 0.0f)
	{
		return false;
	}
	if (fabsf(_qw) > 1.0f + f &&  _qw > 0.0f)
	{
		return false;
	}
	return true;
}

Quat Quat::getConj()
{
	//ToDo
	return Quat(-_qx,-_qy,-_qz,_qw);
}
void Quat::conj()
{
	//ToDo
	_qx = -_qx;
	_qy = -_qy;
	_qz = -_qz;
}
Quat Quat::getT()
{
	//ToDo
	return getConj();
}
void Quat::T()
{
	//ToDo
	//return *this;
	conj();
}
float Quat::mag() const
{
	//ToDo
	float t = _qx*_qx;
	t += _qy*_qy;
	t += _qz*_qz;
	t += _qw*_qw;
	return sqrtf(t);
}

float Quat::magSquared()
{
	float t = _qx*_qx;
	t += _qy*_qy;
	t += _qz*_qz;
	t += _qw*_qw;
	t = sqrtf(t);
	t *= t;
	return t;
}

float Quat::invMag()
{
	//ToDo
	return 1.0f / mag();
}

Quat Quat::norm()
{
	//ToDo
	float m = mag();
	Quat retQ(_qx / m, _qy / m, _qz / m, _qw / m);
	*this = retQ;
	return retQ;
}

Quat Quat::getNorm()
{
	float m = mag();
	return Quat(_qx / m, _qy / m, _qz / m, _qw / m);
}
Quat Quat::getInv()
{
	//ToDo
	float m = this->magSquared();
	return Quat(-_qx / m, -_qy / m,-_qz / m, _qw / m);
}

void Quat::inv()
{
	float m = this->magSquared();
	_qx = -_qx / m;
	_qy = -_qy / m;
	_qz = -_qz / m;
	_qw = _qw / m;
}

void Quat::Lqcvq(const Vect& v1, Vect& vout)
{
	//ToDo
	MATH_UNUSED(v1);
	MATH_UNUSED(vout);
	Vect temp(_qx, _qy, _qz, _qw);
	vout = 2.0f*_qw* ( temp.cross(v1)) + ((_qw*_qw) - temp.dot(temp))*v1 + 2.0f*(temp.dot(v1))*temp;
}

void Quat::Lqvqc(const Vect& v1, Vect& vout)
{
	//ToDo
	MATH_UNUSED(v1);
	MATH_UNUSED(vout);
	Quat q = this->getT();
	Vect temp(q._qx, q._qy, q._qz, q._qw);
	vout = 2.0f*_qw* (temp.cross(v1)) + ((_qw*_qw) - temp.dot(temp))*v1 + 2.0f*(temp.dot(v1))*temp;
}

float Quat::dot(const Quat &q) const
{
	//ToDo
	MATH_UNUSED(q);
	float ret = _qx*q._qx;
	ret += _qy*q._qy;
	ret += _qz*q._qz;
	ret += _qw*q._qw;
	return ret;
}
// ---  End of File ---------------
