#ifndef _D3DAPP_H_
#define _D3DAPP_H_

#include "stdafx.h"
#include "Camera.h"

class d3dApp
{
public:
	d3dApp(void);
	~d3dApp(void);

	virtual void Init(HINSTANCE, HWND, bool, bool, float, float);
	virtual void Update(float dt);
	virtual void Draw() = 0;
protected:
	void DrawBegin();
	void DrawEnd();
	
	ID3D11Device*		 m_Device;
	ID3D11DeviceContext* m_DeviceContext;

	int screenWidth;
	int screenHeight;

	Camera mCamera;

	void Keyboards();
	void OnMouseMove();

	void TurnOnZBuffer();
	void TurnOffZBuffer();

private:
	void SetDriverType();
	void SetRenderTargetView();
	void SetDepthStencil(int, int);
	void SetViewPort(float, float, float, float);
	void InitCamera( int screenWidth, int screenHeight );
	HRESULT InitDepthStencilState();
	IDXGISwapChain*         m_SwapChain;
	ID3D11RenderTargetView* m_RenderTargetView;
	ID3D11Texture2D*        m_DepthStencil;
	ID3D11DepthStencilView* m_DepthStencilView;

	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;

	// inte i kamera classen? Update(dt); som kör dessa inne från kameran? Alternativt game?
	POINT mLastMousePos;
	POINT mMousePos;

	// Varför ligger detta inte i kameraklassen?
	float mCamRotP;
	float mCamRotY;
};

#endif