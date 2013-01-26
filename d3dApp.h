#ifndef _D3DAPP_H_
#define _D3DAPP_H_

#include "stdafx.h"
#include "Obj3D.h"
#include "Camera.h"

class d3dApp
{
public:
	d3dApp(void);
	~d3dApp(void);

	virtual void Init(HINSTANCE, HWND, bool, bool, float, float);

	void InitMouse( int screenWidth, int screenHeight );

	virtual void Update(float dt);
	virtual void Draw() = 0;
	void Render();

	Camera mCamera;

protected:
	void DrawBegin();
	void DrawEnd();
	
	ID3D11Device*		 g_Device;
	ID3D11DeviceContext* g_DeviceContext;
private:
	void SetDriverType();
	void SetRenderTargetView();
	void SetDepthStencil(int, int);
	void SetViewPort(float, float, float, float);
	void Keyboards();
	void OnMouseMove();
	IDXGISwapChain*         g_SwapChain;
	ID3D11RenderTargetView* g_RenderTargetView;
	ID3D11Texture2D*        g_DepthStencil;
	ID3D11DepthStencilView* g_DepthStencilView;

	D3DXMATRIX				g_World;
	D3DXMATRIX				g_WVP;
	D3DXMATRIX				g_iWorld;

	POINT mLastMousePos;
	POINT mMousePos;

	float mCamRotP;
	float mCamRotY;
};

#endif