#pragma once
#include "public.h"

/*
** Fast 1.0/sqrtf(float) routine
*/

inline float InvSqrt(float x)
{
	union
	{
		int intPart;
		float floatPart;
	} convertor;

	convertor.floatPart = x;
	convertor.intPart = 0x5f3759df - (convertor.intPart >> 1);

	return convertor.floatPart*(1.5f - 0.4999f*x*convertor.floatPart*convertor.floatPart);
}

template <typename T>
class Vector2
{
public:
    T x, y;

public:
    inline Vector2()
    {
		x = (T)0;
		y = (T)0;
    }

    inline Vector2(const T fX, const T fY )
        : x( fX ), y( fY )
    {
    }
     inline explicit Vector2( const T scaler )
        : x( scaler), y( scaler )
    {
    }

    inline explicit Vector2( const T afCoordinate[2] )
        : x( afCoordinate[0] ),
          y( afCoordinate[1] )
    {
    }
    /** Assigns the value of the other vector.
        @param
            rkVector The other vector
    */
    inline Vector2<T>& operator = ( const Vector2<T>& rkVector )
    {
        x = rkVector.x;
        y = rkVector.y;

        return *this;
    }

	inline Vector2<T>& operator = ( const T fScalar)
	{
		x = fScalar;
		y = fScalar;

		return *this;
	}

    inline bool operator == ( const Vector2<T>& rkVector ) const
    {
        return ( (int)x == (int)rkVector.x && (int)y == (int)rkVector.y );
    }

    inline bool operator != ( const Vector2<T>& rkVector ) const
    {
		return !this->operator == ( rkVector );
    }

    // arithmetic operations
    inline Vector2<T> operator + ( const Vector2<T>& rkVector ) const
    {
        return Vector2<T>(
            x + rkVector.x,
            y + rkVector.y);
    }

    inline Vector2<T> operator - ( const Vector2<T>& rkVector ) const
    {
        return Vector2<T>(
            x - rkVector.x,
            y - rkVector.y);
    }

    inline Vector2<T> operator * ( const float fScalar ) const
    {
        return Vector2( (T)(x * fScalar), (T)(y * fScalar) );
    }

    inline Vector2 operator * ( const Vector2& rhs) const
    {
        return Vector2(
            x * rhs.x,
            y * rhs.y);
    }

    inline Vector2<T> operator / ( const float fScalar ) const
    {
        assert( fScalar != 0.0 );

        float fInv = 1.0f / fScalar;

        return Vector2<T>(
            x * fInv,
            y * fInv);
    }

    inline Vector2<T> operator / ( const Vector2<T>& rhs) const
    {
        return Vector2<T>( (T)( x / rhs.x ), (T)( y / rhs.y) );
    }
   
     // arithmetic updates
    inline Vector2<T>& operator += ( const Vector2<T>& rkVector )
    {
        x += rkVector.x;
        y += rkVector.y;

        return *this;
    }

    inline Vector2<T>& operator += ( const T fScaler )
    {
        x += fScaler;
        y += fScaler;

        return *this;
    }

    inline Vector2<T>& operator -= ( const Vector2<T>& rkVector )
    {
        x -= rkVector.x;
        y -= rkVector.y;

        return *this;
    }

    inline Vector2<T>& operator -= ( const T fScaler )
    {
        x -= fScaler;
        y -= fScaler;

        return *this;
    }

    inline Vector2& operator *= ( const T fScalar )
    {
        x *= fScalar;
        y *= fScalar;

        return *this;
    }

    inline Vector2<T>& operator *= ( const Vector2<T>& rkVector )
    {
        x *= rkVector.x;
        y *= rkVector.y;

        return *this;
    }

    inline Vector2<T>& operator /= ( const T fScalar )
    {
        assert( fScalar != 0.0 );

        float fInv = 1.0f / fScalar;

        x *= fInv;
        y *= fInv;

        return *this;
    }

    inline Vector2& operator /= ( const Vector2<T>& rkVector )
    {
        x /= rkVector.x;
        y /= rkVector.y;

        return *this;
    }

    /** Returns true if the vector's scalar components are all greater
        that the ones of the vector it is compared against.
    */
    inline bool operator < ( const Vector2<T>& rhs ) const
    {
        if( x < rhs.x && y < rhs.y )
            return true;
        return false;
    }

    /** Returns true if the vector's scalar components are all smaller
        that the ones of the vector it is compared against.
    */
    inline bool operator > ( const Vector2<T>& rhs ) const
    {
        if( x > rhs.x && y > rhs.y )
            return true;
        return false;
    }

    /** Sets this vector's components to the minimum of its own and the
        ones of the passed in vector.
        @remarks
            'Minimum' in this case means the combination of the lowest
            value of x, y and z from both vectors. Lowest is taken just
            numerically, not magnitude, so -1 < 0.
    */
    inline void MakeFloor( const Vector2<T>& cmp )
    {
        if( cmp.x < x ) x = cmp.x;
        if( cmp.y < y ) y = cmp.y;
    }

    /** Sets this vector's components to the maximum of its own and the
        ones of the passed in vector.
        @remarks
            'Maximum' in this case means the combination of the highest
            value of x, y and z from both vectors. Highest is taken just
            numerically, not magnitude, so 1 > -3.
    */
    inline void MakeCeil( const Vector2<T>& cmp )
    {
        if( cmp.x > x ) x = cmp.x;
        if( cmp.y > y ) y = cmp.y;
    }

	float Dot(const Vector2<T>& rhs)
	{
		return this->x * rhs.x + this->y * y;
	}

	Vector2<T>*	Normalize() 
	{ 
		float rc = InvSqrt(Dot(*this)); 
		x*=rc; 
		y*=rc; 

		return this; 
	}
};

typedef Vector2<float>  SizeF;
typedef Vector2<float>	PointF;
