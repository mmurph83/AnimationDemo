//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Gam575
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_QUAT_H
#define ENGINE_MATH_QUAT_H


//------------------------------------------------------------------------
// 
//  This class contains functions and data structure for a 4-dimensional
//  Quaternions.  The quaternions are internally stored interms of the
//  imaginary components (x,y,z) and the real components (real).
// 
//  In the Math library, quaternions can be thought of as "rotational"
//  matrices, everything that you can represent with rotational matrices
//  you can use with quaternions.  Such as quaternion concatenation,
//  inverses, transpose, normalize, multiplying with matrices, and with
//  vectors.
// 
//------------------------------------------------------------------------

class Matrix;
class Vect;

class Quat final : public Align16
{
public:
	Quat();
	Quat(Rot3AxisType type, float x, float y, float z);
	Quat(float x, float y, float z, float w);
	Quat(const Vect & v, float w);
	Quat(const Quat &q);
	explicit Quat(const Matrix & m);
	Quat(const RotType& type, const float& angle);
	Quat(const RotAxisAngleType type, const Vect & v, float angle);
	Quat(RotOrientType type, const Vect &v1, const Vect &v2);
	// Do your magic here
	void set(RotAxisAngleType setting, const Vect& v, const float angle);
	void set(RotType type, float angle);
	void set(const Matrix &m);
	void set(const Quat &q);
	void set(float x, float y, float z, float w);
	void set(Rot3AxisType type, float x, float y, float z);
	void set(const Vect &v, const float fl);
	void set(RotOrientType type, const Vect &v1, const Vect &v2);
	void setVect(const Vect &v);

	void getVect(Vect &v);
	float getAngle();
	void getAxis(Vect &v);
	Quat &operator =(const Quat &q);
	Quat &operator +=(const Quat& q);
	Quat &operator -=(const Quat& q);
	Quat operator -(const Quat& q);
	Quat operator +(const Quat& q);
	Quat &operator *=(const Quat& q);
	Quat operator *(const Quat& q) const;
	Quat operator /(const Quat& q);
	Quat &operator /=(const Quat& q);
	Quat &operator *=(const float& fl);
	Quat operator *(const float& fl) const;
	Quat &operator /=(const float& fl);
	Quat operator /(const float& fl);
	Quat &operator *=(const Matrix& m);
	Matrix operator *(const Matrix& m);
	Quat &operator =(const Matrix& m);

	Quat operator +();
	Quat operator -();
	Quat operator +(const float& fl);
	Quat &operator +=(const float& fl);
	Quat operator -(const float& fl);
	Quat &operator -=(const float& fl);
	float &operator[](x_enum e);
	float &operator[](y_enum e);
	float &operator[](z_enum e);
	float &operator[](w_enum e);
	float operator[](x_enum e) const;
	float operator[](y_enum e) const;
	float operator[](z_enum e) const;
	float operator[](w_enum e) const;

	float qx()const;
	float qy()const;
	float qz()const;
	float real()const;
	float &qx();
	float &qy();
	float &qz();
	float &real();

	bool isEqual(const Quat& q, const float &f) const;
	bool isNegEqual(const Quat& q, const float &f) const;
	bool isEquivalent(const Quat& q, const float &f) const;
	bool isConjugateEqual(const Quat& q, const float &f) const;
	bool isIdentity(const float &f) const;
	bool isNormalized(const float &f) const;
	bool isZero(const float &f) const;

	Quat multByElement(const Quat &q);
	Quat getConj();
	void conj();
	Quat getT();
	void T();
	float mag() const;
	float magSquared();
	float invMag();
	Quat norm();
	Quat getNorm();
	Quat getInv();
	void inv();
	void Lqcvq(const Vect& v1, Vect& vout);
	void Lqvqc(const Vect& v1, Vect& vout);
	float dot(const Quat &q) const;
	friend Quat operator+(const float& val, const Quat& v)
	{
		Quat returnQuat = v;
		returnQuat += val;
		return returnQuat;
	}
	friend Quat operator-(const float& val, const Quat& v)
	{
		Quat returnQuat(val, val, val, val);
		returnQuat -= v;
		return returnQuat;
	}
	friend Quat operator*(const float& val, const Quat& v)
	{
		Quat returnQuat = v;
		returnQuat *= val;
		return returnQuat;
	}
	friend Quat operator/(const float& val, const Quat& v)
	{
		return Quat(val / v._qx, val / v._qy, val / v._qz, val / v._qw);
	}
	
private:

	// anonymous union
	union
	{
		__m128	_mq;

		// anonymous struct
		struct
		{
			float _qx;
			float _qy;
			float _qz;
			float _qw;
		};
	};

};


#endif 

// ---  End of File ---------------
