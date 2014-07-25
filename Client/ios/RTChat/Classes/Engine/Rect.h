#pragma once
#include "public.h"
#include "Vector2.h"
#include  <math.h> 


struct RectF
{
	static const RectF NULLRECT;
	float left, top, right, bottom;
	RectF() 
	{
		left = top = right = bottom = 0;
	}
	RectF( float const & l, float const & t, float const & r, float const & b )
    : left( l ), top( t ), right( r ), bottom( b )
	{
	}
	RectF( const RectF & o )
    : left( o.left ), top( o.top ), right( o.right ), bottom( o.bottom )
	{
	}
	RectF & operator = ( const RectF & o )
	{
		left = o.left;
		top = o.top;
		right = o.right;
		bottom = o.bottom;
		return *this;
	}
	RectF & operator + ( const RectF & o )
	{
		left += o.left;
		top += o.top;
		right += o.right;
		bottom += o.bottom;
		return *this;
	}
	bool operator == ( RectF const & o ) const
	{
		return (left - o.left) < MINFLOAT && (top - o.top) < MINFLOAT && (right - o.right) < MINFLOAT && (bottom - o.bottom) < MINFLOAT;
	}
	bool operator != ( RectF const & o ) const
	{
		return !this->operator ==( o );
	}
	void SetRect( float const & l, float const & t, float const & r, float const & b )
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
	float Width() const
	{
		return right - left;
	}
	float Height() const
	{
		return bottom - top;
	}
	RectF Intersects( const RectF & o )const
	{
		if (fabs((left + right) / 2.0f - (o.left + o.right) / 2.0f) > (Width() + o.Width()) / 2.0f ||
			fabs((top + bottom) / 2.0f - (o.top + o.bottom) / 2.0f) > (Height() + o.Height()) / 2.0f)
			return NULLRECT;

		//计算相交的区域
		RectF intersetRect;
		intersetRect.left = MAX(o.left,left);
		intersetRect.right = MIN(o.right,right);
		intersetRect.top = MAX(o.top,top);
		intersetRect.bottom = MIN(o.bottom,bottom);

		return intersetRect;
	}
	bool Intersects( const PointF &pos )const
	{
		return Intersects( pos.x, pos.y );
	}
	//Rect Intersects( const RectI & o )const
	//{
	//	Rect rect = o;
	//	return Intersects( rect );
	//}
	RectF &Offset(float x,float y)
	{
		this->left += x;
		this->right += x;
		this->top += y;
		this->bottom += y;

		return *this;
	}
	RectF &Offset(const PointF& pos)
	{
		Offset(pos.x * 1.0f, pos.y * 1.0f);
		return *this;
	}
	bool Intersects( float x, float y ) const
	{
		if (x >= left && x < right && y >= top && y < bottom)
			return true;
		return false;
	}
	RectF & operator / (float scale)
	{
		left = left / scale;
		top = top / scale;
		right = right / scale;
		bottom = bottom / scale;
		return *this;
	}	
	RectF & operator * (float scale)
	{
		left = left * scale;
		top = top * scale;
		right = right * scale;
		bottom = bottom * scale;
		return *this;
	}

	RectF &Div( float x, float y) 
	{
		left = left / x;
		top = top / y;
		right = right / x;
		bottom = bottom / y;

		return *this;
	}

	RectF &Mul( float x, float y) 
	{
		left = left * x;
		top = top * y;
		right = right * x;
		bottom = bottom * y;

		return *this;
	}

	RectF &InflateRect( float x, float y )
	{
		left = left - x;
		top = top - y;
		right = right + x;
		bottom = bottom + y;

		return *this;
	}	
	RectF &InflateRect( float x, float y, float x1, float y1 )
	{
		left = left - x;
		top = top - y;
		right = right + x1;
		bottom = bottom + y1;

		return *this;
	}
};

struct Region
{
	int from;
	int to;
	Region( int iFrom = 0, int iTo = 0 )
		:from( iFrom ), to( iTo )
	{
	
	}
	int Size() { return to - from + 1; }
};