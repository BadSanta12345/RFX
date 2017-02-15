#pragma once
#include <d3dx9math.h>
#include <istream>
#include <ostream>

//#include <LinearMath\btVector3.h>

namespace bs
{
	class CQuaternion;
	class CFloat4x4;

	class CFloat3 : public D3DXVECTOR3
	{
	public:
		CFloat3(){};
		CFloat3(const float* arr) : D3DXVECTOR3(arr) {}
		CFloat3(const D3DVECTOR& copy) : D3DXVECTOR3(copy) {}
		CFloat3(const D3DXFLOAT16* mat) : D3DXVECTOR3(mat) {}
		CFloat3(float x, float y, float z) : D3DXVECTOR3(x, y, z) {}

		// inherited assignment operators
		using D3DXVECTOR3::operator+=;
		using D3DXVECTOR3::operator-=;
		using D3DXVECTOR3::operator*=;
		using D3DXVECTOR3::operator/=;

		/*CFloat3 operator - () const
		{
			return CFloat3(-x, -y, -z);
		}*/

		// assignment operators
		CFloat3& operator *= (const CQuaternion& rhs);
		CFloat3& operator *= (const CFloat4x4& rhs);

		// binary operators
		CFloat3	operator * (float rhs) const;
		CFloat3	operator * (const CQuaternion& rhs) const;
		CFloat3	operator * (const CFloat4x4& rhs) const;

		// conversion operators
		//operator btVector3() const { return *reinterpret_cast<const btVector3*>(this); }

		CFloat3&	normalize();
		CFloat3		getNormalized() const;
		CFloat3&	cross(const CFloat3& vec);
		CFloat3		getCross(const CFloat3& vec) const;
		float		dot(const CFloat3& vec) const;
		float		lenghtSq() const;
		float		lenght() const;

		friend std::ostream &operator << (std::ostream &lhs, const CFloat3& rhs);
		friend std::istream& operator >> (std::istream& lhs, CFloat3& rhs);
	};
}
#include "Float3.inl"