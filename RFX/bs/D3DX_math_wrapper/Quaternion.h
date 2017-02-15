#pragma once
#include <d3dx9math.h>

//#include <LinearMath\btQuaternion.h>

namespace bs
{
	class CQuaternion : public D3DXQUATERNION
	{
	public:
		CQuaternion() {}
		CQuaternion(const float* arr) : D3DXQUATERNION(arr) {}
		CQuaternion(const D3DXFLOAT16 * arr) : D3DXQUATERNION(arr) {}
		CQuaternion(float x, float y, float z, float w) : D3DXQUATERNION(x, y, z, w) {}

		// assignment operators
		using D3DXQUATERNION::operator+=;
		using D3DXQUATERNION::operator-=;
		using D3DXQUATERNION::operator*=;
		using D3DXQUATERNION::operator/=;

		// conversion operators
		//operator btQuaternion() const { return *reinterpret_cast<const btQuaternion*>(this); }

		CQuaternion&	normalize(){ return *reinterpret_cast<CQuaternion*>(D3DXQuaternionNormalize(this, this)); }
		CQuaternion		getNormalized() const { return *reinterpret_cast<CQuaternion*>(D3DXQuaternionNormalize(&D3DXQUATERNION(), this)); }

		// quaternion building utility functions
		static CQuaternion	axisAngle(const CFloat3& axis, float angle){ return *reinterpret_cast<CQuaternion*>(D3DXQuaternionRotationAxis(&D3DXQUATERNION(), &axis, angle)); }
	};

}

#include "Quaternion.inl"