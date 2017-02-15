#pragma once
#include <d3dx9math.h>
#include <iostream>

#include "Float2.h"
#include "Float3.h"
#include "Quaternion.h"
#include "Float4x4.h"
#include "../vectors.h"

namespace bs
{
	//class CFloat3;
	class CFloat4;
	class CQuaternion;
	class CFloat4x4;

	class CFloat4 : public D3DXVECTOR4
	{
	public:
		CFloat4(){};
		CFloat4(const float* arr) : D3DXVECTOR4(arr) {}
		CFloat4(const D3DVECTOR& copy, float w) : D3DXVECTOR4(copy, w) {}
		CFloat4(const D3DXFLOAT16* mat) : D3DXVECTOR4(mat) {}
		CFloat4(float x, float y, float z, float w) : D3DXVECTOR4(x, y, z, w) {}

		// assignment operators
		using D3DXVECTOR4::operator+=;
		using D3DXVECTOR4::operator-=;
		using D3DXVECTOR4::operator*=;
		using D3DXVECTOR4::operator/=;

		CFloat4&	normalize(){ return *reinterpret_cast<CFloat4*>(D3DXVec4Normalize(this, this)); }
		CFloat4		getNormalized() const { return *reinterpret_cast<CFloat4*>(D3DXVec4Normalize(&D3DXVECTOR4(), this)); }
		CFloat4&	cross(const CFloat4& vec1, const CFloat4& vec2){ return *reinterpret_cast<CFloat4*>(D3DXVec4Cross(this, this, &vec1, &vec2)); }
		CFloat4		getCross(const CFloat4& vec1, const CFloat4& vec2) const { return *reinterpret_cast<CFloat4*>(D3DXVec4Cross(&D3DXVECTOR4(), this, &vec1, &vec2)); }
		float		dot(const CFloat4& vec){ return D3DXVec4Dot(this, &vec); }
		float		lenghtSq(){ return D3DXVec4LengthSq(this); }
		float		lenght(){ return D3DXVec4Length(this); }

		inline friend std::ostream &operator<<(std::ostream &output, const CFloat4& rhs)
		{
			return output << '(' << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w << ')';
		}
	};

	inline float randf() 
	{ 
		return rand() / (float)RAND_MAX; 
	}

	inline float randf(float min, float max)
	{
		return randf() * (max-min) + min;
	}

	inline float round_step(float x, float step)
	{
		if (step == 0.0f)
		{
			return x;
		}
		return x - remainder(x, step);
	}

	template<class T>
	inline T clamp(T const & v, T const & b, T const & c)
	{
		if (v < b) return b;
		if (v > c) return c;
		return v;
	}

	inline float clamp(float v, float b, float c)
	{
		if (v < b) return b;
		if (v > c) return c;
		return v;
	}

	using uint = unsigned int;
	using ushort = unsigned short;
	using uchar = unsigned char;

	// short for clamp( a, 0, 1 )
	template<class T>
	inline T saturate(T const & a)
	{
		return clamp(a, (T)0.0, (T)1.0);
	}

	template<class T, class F>
	inline auto lerp(T const & a, T const & b, F const & f) -> decltype(a + (b - a)*f)
	{
		return a + (b - a)*f;
	}
	
	template<class T>
	inline T blerp(T const & c00, T const & c10, T const & c01, T const & c11, T const & tx, T const & ty)
	{
		return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
	}

	template<class T>
	inline T sqr(T & a)
	{
		return a * a;
	}

	constexpr float PI = 3.141592654f;

	/*inline float wrapAngle(float angle)
	{
		while (angle > D3DX_PI)  angle -= D3DX_PI * 2.0f;
		while (angle < -D3DX_PI) angle += D3DX_PI * 2.0f;
		return angle;
	}*/

	inline float wrapAngle(float x)
	{
		x = fmod(x + PI, 2 * PI);
		if (x < 0)
			x += (2 * PI);
		return x - PI;
	}

	enum class TIntersect : char
	{
		Outside,
		Intersect,
		Inside
	};

	constexpr long double operator"" _deg(long double deg)
	{
		return deg*PI / 180.0f;
	}


	/*struct AABB
	{
		CFloat3       m_min;
		CFloat3       m_max;
		CFloat3       getCenter()   { return (m_min + m_max) * 0.5f; }
		CFloat3       getSize()     { return m_max - m_min; }


		AABB() {}
		AABB(const CFloat3 & _min, const CFloat3 & _max) : m_min(_min), m_max(_max)   { }

		void debugDraw(CDebugDrawer& drawer, bs::TUChar3 color = bs::TUChar3(255, 255, 255));

		std::array<CFloat3, 8> getCornerPoints()
		{
			std::array<CFloat3, 8> corners;

			corners[0] = CFloat3(m_min.x, m_max.y, m_min.z);
			corners[1] = CFloat3(m_max.x, m_max.y, m_min.z);
			corners[2] = CFloat3(m_min.x, m_min.y, m_min.z);
			corners[3] = CFloat3(m_max.x, m_min.y, m_min.z);

			corners[4] = CFloat3(m_min.x, m_max.y, m_max.z);
			corners[5] = CFloat3(m_max.x, m_max.y, m_max.z);
			corners[6] = CFloat3(m_min.x, m_min.y, m_max.z);
			corners[7] = CFloat3(m_max.x, m_min.y, m_max.z);

			return corners;
		}

		bool intersects(const AABB & other)
		{
			return !((other.m_max.x < m_min.x) || (other.m_min.x > m_max.x)
				|| (other.m_max.y < m_min.y) || (other.m_min.y > m_max.y)
				|| (other.m_max.z < m_min.z) || (other.m_min.z > m_max.z));
		}

		bool operator == (const AABB & b)
		{
			return m_min == b.m_min && m_max == b.m_max;
		}

		float getBoundingSphereRadius()
		{
			CFloat3 size = getSize();
			return size.lenght() * 0.5f;
		}

		float getBoundingSphereRadiusSq()
		{
			CFloat3 size = getSize();
			return size.lenghtSq() * 0.5f;
		}

		void expand(float percentage)
		{
			CFloat3 offset = getSize() * percentage;
			m_min -= offset;
			m_max += offset;
		}

		void transform(CFloat4x4 transformation)
		{
			auto corners = getCornerPoints();
			//D3DXVec3TransformCoordArray(&corners[0], sizeof(CFloat3), &corners[0], sizeof(CFloat3), &transformation, 8);
			for (auto& vertex : corners)
			{
				vertex *= transformation;
			}

			D3DXComputeBoundingBox(corners.data(), 8, sizeof(D3DXVECTOR3), &m_min, &m_max);

			/*m_min = CFloat3(FLT_MAX, FLT_MAX, FLT_MAX);
			m_max = CFloat3(FLT_MIN, FLT_MIN, FLT_MIN);

			for (auto& vertex : corners)
			{
				m_min.x = min(m_min.x, vertex.x);
				m_max.x = max(m_max.x, vertex.x);

				m_min.y = min(m_min.y, vertex.y);
				m_max.y = max(m_max.y, vertex.y);

				m_min.z = min(m_min.z, vertex.z);
				m_max.z = max(m_max.z, vertex.z);
			}*//*
		}

	};*/
}