#include "stdafx.h"
#include "Camera.h"

Camera::Camera(void)
{
}

Camera::~Camera(void)
{
}

#pragma region Get

D3DXVECTOR3 Camera::GetPosition() const
{
	return mPosition;
}

void Camera::SetPosition( float x, float y, float z )
{
	mPosition = D3DXVECTOR3(x,y,z);
}

void Camera::SetPosition( const D3DXVECTOR3& v )
{
	mPosition = v;
}

D3DXVECTOR3 Camera::GetRight() const
{
	return mRight;
}

D3DXVECTOR3 Camera::GetUp() const
{
	return mUp;
}

D3DXVECTOR3 Camera::GetLook() const
{
	return mLook;
}

float Camera::GetNearZ() const
{
	return mNearZ;
}

float Camera::GetFarZ() const
{
	return mFarZ;
}

float Camera::GetAspect() const
{
	return mAspect;
}

float Camera::GetFovY() const
{
	return mFovY;
}

float Camera::GetFovX() const
{
	float halfWidth = 0.5f*GetNearWindowWidth();
	return 2.0f*atan(halfWidth/mNearZ);
}

float Camera::GetNearWindowWidth() const
{
	return mAspect * mNearWindowHeight;
}

float Camera::GetNearWindowHeight() const
{
	return mNearWindowHeight;
}

float Camera::GetFarWindowWidth() const
{
	return mAspect * mFarWindowHeight;
}

float Camera::GetFarWindowHeight() const
{
	return mFarWindowHeight;
}

#pragma endregion Getters

void Camera::SetLens( float fovY, float aspect, float zn, float zf )
{
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f*mNearZ*tanf(0.5f*mFovY);
	mFarWindowHeight = 2.0f*mFarZ*tanf(0.5f*mFovY);

	D3DXMatrixPerspectiveFovLH(&mProj, mFovY, mAspect, mNearZ, mFarZ);
}

void Camera::LookAt(D3DXVECTOR3& pos, D3DXVECTOR3& target, D3DXVECTOR3& worldUp )
{
	mPosition = pos;
	D3DXVECTOR3 temp;
	D3DXVec3Subtract(&temp, &target, &pos);
	D3DXVec3Normalize(&mLook, &temp);
	D3DXVec3Cross(&temp, &worldUp, &mLook);
	D3DXVec3Normalize(&mRight, &temp);
	D3DXVec3Cross(&mUp, &mLook, &mRight);
}

D3DXMATRIX Camera::View() const
{
	return mView;
}

D3DXMATRIX Camera::Proj() const
{
	return mProj;
}

D3DXMATRIX Camera::ViewProj() const
{
	return mView*mProj;
}

void Camera::Strafe( float d ,bool jet,float angle)
{
	if(!jet)
	mPosition += d*mRight;
	else
	{
		D3DXMATRIX R;
		D3DXMatrixRotationAxis(&R, &mUp, -angle);
		D3DXVec3TransformNormal(&mLook, &mLook, &R);
		D3DXVec3TransformNormal(&mRight, &mRight, &R);
	}
}

void Camera::Walk( float d,bool jet )
{
	//jet = true;

	//if(jet)
	mPosition += d*mLook;
	//else
	//	mPosition += D3DXVECTOR3(d*mLook.x,0,d*mLook.z);

}
void Camera::Roll( float angle )
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &mLook, angle);
	D3DXVec3TransformNormal(&mRight, &mRight, &R);
	D3DXVec3TransformNormal(&mUp, &mUp, &R);
	
}


void Camera::Pitch( float angle )
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &mRight, angle);
	D3DXVec3TransformNormal(&mUp, &mUp, &R);
	D3DXVec3TransformNormal(&mLook, &mLook, &R);
}

void Camera::RotateY( float angle )
{
	D3DXMATRIX R;
	D3DXMatrixRotationY(&R, angle);
	D3DXVec3TransformNormal(&mRight, &mRight, &R);
	D3DXVec3TransformNormal(&mUp, &mUp, &R);
	D3DXVec3TransformNormal(&mLook, &mLook, &R);
}

void Camera::UpdateMatrix()
{
	D3DXVECTOR3 R = mRight;
	D3DXVECTOR3 U = mUp;
	D3DXVECTOR3 L = mLook;
	D3DXVECTOR3 P = mPosition;

	D3DXVec3Normalize(&L, &L);
	D3DXVECTOR3 temp;
	D3DXVec3Cross(&temp, &L, &R);
	D3DXVec3Normalize(&U, &temp);
	D3DXVec3Cross(&R, &U, &L);

	
	float x = -D3DXVec3Dot(&P, &R);
	float y = -D3DXVec3Dot(&P, &U);
	float z = -D3DXVec3Dot(&P, &L);

	mRight = R;
	mUp = U;
	mLook = L;

	mView(0, 0) = mRight.x;
	mView(1, 0) = mRight.y;
	mView(2, 0) = mRight.z;
	mView(3, 0) = x;

	mView(0, 1) = mUp.x;
	mView(1, 1) = mUp.y;
	mView(2, 1) = mUp.z;
	mView(3, 1) = y;

	mView(0, 2) = mLook.x;
	mView(1, 2) = mLook.y;
	mView(2, 2) = mLook.z;
	mView(3, 2) = z;

	mView(0, 3) = 0.0f;
	mView(1, 3) = 0.0f;
	mView(2, 3) = 0.0f;
	mView(3, 3) = 1.0f;
}
