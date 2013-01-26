#pragma once
#include "stdafx.h"

class Camera
{
public:
	Camera(void);
	~Camera(void);

	D3DXVECTOR3 GetPosition() const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const D3DXVECTOR3& v);

	D3DXVECTOR3 GetRight() const;
	D3DXVECTOR3 GetUp() const;
	D3DXVECTOR3 GetLook() const;

	float GetNearZ() const;
	float GetFarZ() const;
	float GetAspect() const;
	float GetFovY() const;
	float GetFovX() const;

	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	void SetLens(float fovY, float aspect, float zn, float zf);

	void LookAt(D3DXVECTOR3& pos, D3DXVECTOR3& target, D3DXVECTOR3& worldUp);

	D3DXMATRIX View() const;
	D3DXMATRIX Proj() const;
	D3DXMATRIX ViewProj() const;

	void Strafe(float d,bool jet,float angle);
	void Walk(float d,bool jet);

	void Pitch(float angle);
	void RotateY(float angle);

	void UpdateMatrix();
	void Roll( float angle );
private:

	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mRight;
	D3DXVECTOR3 mUp;
	D3DXVECTOR3 mLook;

	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
};

