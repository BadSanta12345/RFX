#include "Float3.h"
#include "Float4x4.h"

namespace bs
{

	inline CFloat3&	CFloat3::operator *= (const CQuaternion& rhs)
	{
		return *reinterpret_cast<CFloat3*>(D3DXVec3TransformCoord(this, this, &CFloat4x4::rotationQuaternion(rhs)));
	}

	inline CFloat3&	CFloat3::operator *= (const CFloat4x4& rhs)
	{
		return *reinterpret_cast<CFloat3*>(D3DXVec3TransformCoord(this, this, &rhs));
	}

	inline CFloat3 CFloat3::operator * (float rhs) const
	{
		CFloat3 copy = *this;
		return copy *= rhs;
	}

	inline CFloat3 CFloat3::operator * (const CFloat4x4& rhs) const
	{
		return *reinterpret_cast<CFloat3*>(D3DXVec3TransformCoord(&CFloat3(), this, &rhs));
	}

	inline CFloat3 CFloat3::operator * (const CQuaternion& rhs) const
	{
		return *reinterpret_cast<CFloat3*>(D3DXVec3TransformCoord(&CFloat3(), this, &CFloat4x4::rotationQuaternion(rhs)));
	}

	inline CFloat3& CFloat3::normalize()
	{
		return *reinterpret_cast<CFloat3*>(D3DXVec3Normalize(this, this)); 
	}

	inline CFloat3 CFloat3::getNormalized() const
	{ 
		return *reinterpret_cast<CFloat3*>(D3DXVec3Normalize(&D3DXVECTOR3(), this)); 
	}

	inline CFloat3&	CFloat3::cross(const CFloat3& vec)
	{
		return *this = getCross(vec); 
	}

	inline CFloat3 CFloat3::getCross(const CFloat3& vec) const
	{ 
		CFloat3 copy;

		copy.x = y * vec.z - z * vec.y;
		copy.y = z * vec.x - x * vec.z;
		copy.z = x * vec.y - y * vec.x;

		return copy; 
	}

	inline float CFloat3::dot(const CFloat3& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z; 
	}

	inline float CFloat3::lenghtSq() const 
	{
		return x * x + y * y + z * z; 
	}

	inline float CFloat3::lenght() const 
	{
		return sqrt(lenghtSq()); 
	}

	inline std::ostream &operator<<(std::ostream &lhs, const CFloat3& rhs)
	{
		return lhs << rhs.x << '/' << rhs.y << '/' << rhs.z;
	}

	inline std::istream& operator >> (std::istream& lhs, CFloat3& rhs)
	{
		char c;

		lhs >> rhs.x;
		lhs >> c;
		if (c != '/')
		{
			rhs.z = rhs.y = rhs.x;
			return lhs;
		}

		lhs >> rhs.y;
		lhs >> c;
		if (c != '/')
		{
			rhs.z = rhs.y;
			return lhs;
		}

		lhs >> rhs.z;

		return lhs;
	}
}