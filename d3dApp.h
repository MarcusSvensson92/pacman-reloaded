#ifndef _D3DAPP_H_
#define _D3DAPP_H_

#include "stdafx.h"

class d3dApp
{
public:
	d3dApp(void);
	~d3dApp(void);

	void Update(float dt);
	void Draw();
	void Init(HINSTANCE, HWND, bool, bool, float, float);

private:
	void SetDriverType();
	void SetRenderTargetView();
	void SetDepthStencil(int, int);
	void SetViewPort(float, float, float, float);

	IDXGISwapChain*         g_SwapChain;
	ID3D11RenderTargetView* g_RenderTargetView;
	ID3D11Texture2D*        g_DepthStencil;
	ID3D11DepthStencilView* g_DepthStencilView;
	ID3D11Device*			g_Device;
	ID3D11DeviceContext*	g_DeviceContext;

	D3DXMATRIX				wvp;
	D3DXMATRIX				world;
	D3DXMATRIX				iwvp;
};

#endif