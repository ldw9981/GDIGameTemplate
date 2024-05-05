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
		float length = sqrt(x*x + y*y);	// 피타고라스 정리를 이용하여 벡터의 길이(0,0 부터의 거리)를 구한다.
		x /= length;	// 길이로 나누어 길이 1에 해당되는 x,y 성분을 구한다. 
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

