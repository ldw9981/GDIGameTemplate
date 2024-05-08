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
	/*		   x,y
	  |		    +  	
	  |         
	  |	       
	 -+---------->
	 0,0	   
	*/
	void Normalize()
	{
		float length = sqrt(x*x + y*y);	// 피타고라스 정리를 이용하여 벡터의 길이(0,0 -> x,y 까지의 거리)를 구한다.
		x /= length;	// 길이로 나누어 길이 1에 해당되는 x,y 성분을 구한다.   즉 벡터의 크기가 1인 순수한 방향을 나타내는 벡터가 된다.
		y /= length;
	}

	float Length()
	{
		return sqrt(x*x + y*y);
	}
	// a + b;
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
	//  a += b;
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

