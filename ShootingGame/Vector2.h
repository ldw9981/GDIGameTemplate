#pragma once
struct Vector2
{
	float x,y;

	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}
	Vector2(const Vector2& v) : x(v.x), y(v.y) {}
	
	void Set(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	void Normalize()
	{	
		float length = sqrt(x*x + y*y);	
		x /= length;	
		y /= length;	
	}

	float Length()
	{
		return sqrt(x*x + y*y);
	}
	Vector2 operator+(const Vector2& v)
	{
		return Vector2(x + v.x, y + v.y);
	}
	Vector2 operator-(const Vector2& v)
	{
		return Vector2(x - v.x, y - v.y);
	}
	Vector2 operator*(float f)
	{
		return Vector2(x * f, y * f);
	}
	Vector2 operator/(float f)
	{
		return Vector2(x / f, y / f);
	}
	Vector2& operator+=(const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	Vector2& operator-=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	Vector2& operator*=(float f)
	{
		x *= f;
		y *= f;
		return *this;
	}
	Vector2& operator/=(float f)
	{
		x /= f;
		y /= f;
		return *this;
	}
	bool operator==(const Vector2& v)
	{
		return x == v.x && y == v.y;
	}
	bool operator!=(const Vector2& v)
	{
		return x != v.x || y != v.y;
	}	
};

