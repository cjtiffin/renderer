#ifndef _ANGLE_H_
#define _ANGLE_H_
#pragma once

template<class T> T rad_to_deg(const T t) { return t * 57.295779513082320876798154814105; }
template<class T> T deg_to_rad(const T t) { return t * 0.017453292519943295769236907684886; }

class Angle
{
public:
	enum TYPE
	{
		TYPE_RAD, TYPE_DEG
	};

	Angle() : angle(0) {}
	Angle(float rad) : angle(rad) {}
	Angle(float a, TYPE t) { angle = (t == TYPE_RAD) ? a : deg_to_rad(a); }

	operator float() const { return angle; }

	float sin() { return std::sin(angle); }
	float cos() { return std::cos(angle); }
	float tan() { return std::tan(angle); }

	float rad() { return angle; }
	float deg() { return rad_to_deg(angle); }

	Angle & rad(float rad) { angle = rad; return *this; }
	Angle & deg(float deg) { angle = deg_to_rad(deg); return *this; }

	static Angle RAD(float a) { return a; }
	static Angle DEG(float a) { return deg_to_rad(a); }

	float angle;
};

#endif //_ANGLE_H_