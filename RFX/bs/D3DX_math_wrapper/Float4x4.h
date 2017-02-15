#pragma once
#include <d3dx9math.h>

namespace bs
{
	class CFloat2;
	class CFloat3;
	class CQuaternion;
	class CFloat4x4 : public D3DXMATRIX
	{
	public:
		CFloat4x4() {};
		CFloat4x4(const float* arr) : D3DXMATRIX(arr) {}
		CFloat4x4(const D3DMATRIX& mat) : D3DXMATRIX(mat) {}
		CFloat4x4(const D3DXFLOAT16* mat) : D3DXMATRIX(mat) {}
		CFloat4x4(float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44)
			: D3DXMATRIX(_11, _12, _13, _14,
			_21, _22, _23, _24,
			_31, _32, _33, _34,
			_41, _42, _43, _44) {}

		// assignment operators
		using D3DXMATRIX::operator+=;
		using D3DXMATRIX::operator-=;
		using D3DXMATRIX::operator*=;
		using D3DXMATRIX::operator/=;

		bool		isIdentity(){ return D3DXMatrixIsIdentity(this) != 0; }
		float		determinant(){ return D3DXMatrixDeterminant(this); }
		CFloat4x4&	invert(){ if (D3DXMatrixInverse(this, NULL, this) == NULL) return identity(); return *this; }
		CFloat4x4	getInverse() const { CFloat4x4 mat;  if (D3DXMatrixInverse(&mat, NULL, this) == NULL) return mat.identity(); return mat; }

		// Matrix building utility functions
		static CFloat4x4	identity();

		static CFloat4x4	projectionPerspectiveFovLH(float fovy, float Aspect, float zn, float zf);
		static CFloat4x4	projectionPerspectiveFovRH(float fovy, float Aspect, float zn, float zf);

		static CFloat4x4	orthoOffCenterLH(float l, float r, float b, float t, float zn, float zf);
		static CFloat4x4	orthoOffCenterRH(float l, float r, float b, float t, float zn, float zf);

		static CFloat4x4	viewLookAtLH(const CFloat3& Eye, const CFloat3& At, const CFloat3& Up);
		static CFloat4x4	viewLookAtRH(const CFloat3& Eye, const CFloat3& At, const CFloat3& Up);
		static CFloat4x4	viewEyeQuaternionLH(const CFloat3& eye, const CQuaternion& q);
		static CFloat4x4	viewVectorsLH(const CFloat3& eye, const CFloat3& right, const CFloat3& up, const CFloat3& forward);

		static CFloat4x4	rotationQuaternion(const CQuaternion& q);
		static CFloat4x4	rotationAxisAngle(const CFloat3& axis, float angle);
		static CFloat4x4	rotationYawPitchRoll(float yaw, float pitch, float roll);
		static CFloat4x4	rotationYawPitchRoll(const CFloat3& yawPitchRoll);

		static CFloat4x4	translation(float x, float y, float z = 0.0f);
		static CFloat4x4	translation(const CFloat2& vec);
		static CFloat4x4	translation(const CFloat3& vec);

		static CFloat4x4	scaling(float x, float y, float z = 1.0f);
		static CFloat4x4	scaling(float scale);
		static CFloat4x4	scaling(CFloat2& vec);
		static CFloat4x4	scaling(CFloat3& vec);

		/*inline friend std::ostream &operator<<(std::ostream &output, const CFloat4x4& rhs)
		{
		return output << '(' << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w << ')';
		}*/
	};
}
#include "Float4x4.inl"