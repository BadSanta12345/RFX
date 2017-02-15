#pragma once
#include "Float4x4.h"
#include "Float2.h"
#include "Float3.h"
#include "Quaternion.h"

namespace bs
{
	inline CFloat4x4 CFloat4x4::identity()
	{ 
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixIdentity(&D3DXMATRIX()));
	}

	inline CFloat4x4 CFloat4x4::projectionPerspectiveFovLH(float fovy, float Aspect, float zn, float zf)
	{ 
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixPerspectiveFovLH(&D3DXMATRIX(), fovy, Aspect, zn, zf)); 
	}

	inline CFloat4x4 CFloat4x4::projectionPerspectiveFovRH(float fovy, float Aspect, float zn, float zf)
	{ 
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixPerspectiveFovRH(&D3DXMATRIX(), fovy, Aspect, zn, zf)); 
	}

	inline CFloat4x4 CFloat4x4::orthoOffCenterLH(float l, float r, float b, float t, float zn, float zf)
	{
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixOrthoOffCenterLH(&D3DXMATRIX(), l, r, b, t, zn, zf));
	}

	inline CFloat4x4 CFloat4x4::orthoOffCenterRH(float l, float r, float b, float t, float zn, float zf)
	{
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixOrthoOffCenterRH(&D3DXMATRIX(), l, r, b, t, zn, zf));
	}

	inline CFloat4x4 CFloat4x4::viewLookAtLH(const CFloat3& Eye, const CFloat3& At, const CFloat3& Up)
	{ 
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixLookAtLH(&D3DXMATRIX(), &Eye, &At, &Up)); 
	}

	inline CFloat4x4 CFloat4x4::viewLookAtRH(const CFloat3& Eye, const CFloat3& At, const CFloat3& Up) 
	{ 
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixLookAtRH(&D3DXMATRIX(), &Eye, &At, &Up));
	}

	inline CFloat4x4 CFloat4x4::viewEyeQuaternionLH(const CFloat3& eye, const CQuaternion& q)
	{
		CFloat4x4 rotationMatrix = CFloat4x4::rotationQuaternion(q);
		CFloat3 localRight = CFloat3(1.0f, 0.0f, 0.0f);
		CFloat3 localUp = CFloat3(0.0f, 1.0f, 0.0f);
		CFloat3 localForward = CFloat3(0.0f, 0.0f, 1.0f);

		D3DXVec3TransformCoord(&localRight, &localRight, &rotationMatrix);
		D3DXVec3TransformCoord(&localUp, &localUp, &rotationMatrix);
		D3DXVec3TransformCoord(&localForward, &localForward, &rotationMatrix);

		return viewVectorsLH(eye, localRight, localUp, localForward);
	}

	inline CFloat4x4 CFloat4x4::viewVectorsLH(const CFloat3& eye, const CFloat3& right, const CFloat3& up, const CFloat3& forward)
	{
		CFloat4x4 view;

		view._11 = right.x;			view._12 = up.x;		 view._13 = forward.x;			view._14 = 0.0f;
		view._21 = right.y;			view._22 = up.y;		 view._23 = forward.y;			view._24 = 0.0f;
		view._31 = right.z;			view._32 = up.z;		 view._33 = forward.z;			view._34 = 0.0f;
		view._41 = -eye.dot(right); view._42 = -eye.dot(up); view._43 = -eye.dot(forward);  view._44 = 1.0f;

		return view;
	}

	inline CFloat4x4 CFloat4x4::rotationQuaternion(const CQuaternion& q)
	{ 
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixRotationQuaternion(&D3DXMATRIX(), &q)); 
	}

	inline CFloat4x4 CFloat4x4::rotationAxisAngle(const CFloat3& axis, float angle) 
	{ 
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixRotationAxis(&D3DXMATRIX(), &axis, angle)); 
	}

	inline CFloat4x4 CFloat4x4::rotationYawPitchRoll(float yaw, float pitch, float roll)
	{
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixRotationYawPitchRoll(&D3DXMATRIX(), yaw, pitch, roll));
	}

	inline CFloat4x4 CFloat4x4::rotationYawPitchRoll(const CFloat3& yawPitchRoll)
	{
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixRotationYawPitchRoll(&D3DXMATRIX(), yawPitchRoll.x, yawPitchRoll.y, yawPitchRoll.z));
	}

	inline CFloat4x4 CFloat4x4::translation(float x, float y, float z) 
	{ 
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixTranslation(&D3DXMATRIX(), x, y, z));
	}

	inline CFloat4x4 CFloat4x4::translation(const CFloat2& vec) 
	{ 
		return translation(vec.x, vec.y); 
	}

	inline CFloat4x4 CFloat4x4::translation(const CFloat3& vec) 
	{ 
		return translation(vec.x, vec.y, vec.z); 
	}

	inline CFloat4x4 CFloat4x4::scaling(float x, float y, float z) 
	{
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixScaling(&D3DXMATRIX(), x, y, z)); 
	}

	inline CFloat4x4 CFloat4x4::scaling(float scale)
	{
		return *reinterpret_cast<CFloat4x4*>(D3DXMatrixScaling(&D3DXMATRIX(), scale, scale, scale));
	}

	inline CFloat4x4 CFloat4x4::scaling(CFloat2& vec) 
	{ 
		return scaling(vec.x, vec.y); 
	}

	inline CFloat4x4 CFloat4x4::scaling(CFloat3& vec) 
	{ 
		return scaling(vec.x, vec.y, vec.z); 
	}
}