#pragma once
#include <d3dx9math.h>
#include <istream>
#include <ostream>

namespace bs
{
	class CFloat2 : public D3DXVECTOR2
	{
	public:
		CFloat2(){};
		CFloat2(const D3DXVECTOR2& vec){ x = vec.x; y = vec.y; };
		CFloat2(const float* arr) : D3DXVECTOR2(arr) {}
		CFloat2(const D3DXFLOAT16* mat) : D3DXVECTOR2(mat) {}
		CFloat2(float x, float y) : D3DXVECTOR2(x, y) {}

		// assignment operators
		using D3DXVECTOR2::operator+=;
		using D3DXVECTOR2::operator-=;
		using D3DXVECTOR2::operator*=;
		using D3DXVECTOR2::operator/=;

		CFloat2&	operator = (const D3DXVECTOR2& rhs){ x = rhs.x; y = rhs.y; return *this; }
		CFloat2&	normalize(){ return *reinterpret_cast<CFloat2*>(D3DXVec2Normalize(this, this)); }
		CFloat2		getNormalized(){ return *reinterpret_cast<CFloat2*>(D3DXVec2Normalize(&D3DXVECTOR2(), this)); }
		float		dot(const CFloat2& vec) const { return D3DXVec2Dot(this, &vec); }
		float		lenghtSq() const { return D3DXVec2LengthSq(this); }
		float		lenght() const { return D3DXVec2Length(this); }

		friend std::ostream &operator << (std::ostream &lhs, const CFloat2& rhs);
		friend std::istream& operator >> (std::istream& lhs, CFloat2& rhs);
	};
}
#include "Float2.inl"