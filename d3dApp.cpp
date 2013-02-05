#include "stdafx.h"
#include "d3dApp.h"

d3dApp::d3dApp(void)
{
}
d3dApp::~d3dApp(void)
{
}

void d3dApp::Init(HINSTANCE hinstance, HWND hwnd, bool vsync, bool fullscreen, float screenDepth, float screenNear)
{
	// hresult for bugtesting
	HRESULT hr; 

	// Get screenSize
	RECT rc;
	GetClientRect( hwnd, &rc );
	int screenWidth = rc.right - rc.left;
	int screenHeight = rc.bottom - rc.top;

	// DeviceFlags
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Drivertype
	D3D_DRIVER_TYPE driverType;
	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]); //same as ARRAYSIZE(x) macro

	// Create the Swap chain description
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof(swapChainDesc) );
	swapChainDesc.BufferDesc.Width							= screenWidth;
	swapChainDesc.BufferDesc.Height							= screenHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator			= 60;				// ?
	swapChainDesc.BufferDesc.RefreshRate.Denominator		= 1;				// ?
	swapChainDesc.BufferDesc.Format							= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count							= 1;
	swapChainDesc.SampleDesc.Quality						= 0;
	swapChainDesc.BufferUsage								= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow								= hwnd;
	swapChainDesc.Windowed									= !fullscreen;
	swapChainDesc.BufferCount								= 1;

	// Check for DX drivers
	D3D_FEATURE_LEVEL featureLevelsToTry[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	// Setup driver
	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			createDeviceFlags,
			featureLevelsToTry,
			ARRAYSIZE(featureLevelsToTry),
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&m_SwapChain,
			&m_Device,
			&initiatedFeatureLevel,
			&m_DeviceContext);
		// Stops when success. If not it will try a new Type
		if( SUCCEEDED( hr ) )
			break;
	}
	if( FAILED(hr) )
		MessageBoxA(0, "Failed CreateDeviceAndSwapChain", 0, 0);

	// Create a render target view
	SetRenderTargetView();
	// Create depth stencil texture and the depth stencil view
	SetDepthStencil(screenWidth, screenHeight);
	// Setup the viewport
	SetViewPort((float)screenWidth, (float)screenHeight, screenDepth, screenNear);

	InitCamera(screenWidth, screenHeight);

}

void d3dApp::InitCamera( int screenWidth, int screenHeight )
{
	mCamera.SetLens(0.25f*D3DX_PI, (float)screenWidth / (float)screenHeight, 1.0f, 1000.0f);

	mCamera.LookAt(D3DXVECTOR3(410, 210, 135), D3DXVECTOR3(200,0,135), D3DXVECTOR3(0,1,0));
	SetCursorPos(600,500);
	GetCursorPos(&mLastMousePos);

	mCamRotP = 0;
	mCamRotY = 0;
}


void d3dApp::Update(float dt)
{
 	OnMouseMove();
 	Keyboards();
}
void d3dApp::DrawBegin()
{
	static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_DeviceContext->ClearRenderTargetView( m_RenderTargetView, ClearColor );	

	m_DeviceContext->ClearDepthStencilView( m_DepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0 );
}
void d3dApp::DrawEnd()
{
	m_SwapChain->Present( 1, 0 );
}

void d3dApp::SetRenderTargetView()
{
	// hresult for bugtesting
	HRESULT hr; 

	ID3D11Texture2D* pBackBuffer;
	hr = m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer );
	if( FAILED(hr) )
		MessageBoxA(0, "Failed GetBuffer", 0, 0);

	hr = m_Device->CreateRenderTargetView( pBackBuffer, 0, &m_RenderTargetView );
	pBackBuffer->Release();
	if( FAILED(hr) )
		MessageBoxA(0, "Failed RenderView", 0, 0);
}
void d3dApp::SetDepthStencil(int width, int height)
{
	// hresult for bugtesting
	HRESULT hr; 

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_Device->CreateTexture2D( &descDepth, NULL, &m_DepthStencil );
	if( FAILED(hr) )
		MessageBoxA(0, "Failed CreateTexture2D", 0, 0);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = m_Device->CreateDepthStencilView( m_DepthStencil, 0, &m_DepthStencilView );
	if( FAILED(hr) )
		MessageBoxA(0, "Failed CreateDepthStencilView", 0, 0);

	m_DeviceContext->OMSetRenderTargets( 1, &m_RenderTargetView, m_DepthStencilView );
}
void d3dApp::SetViewPort(float width, float height, float screenDepth, float screenNear)
{
	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = screenNear;
	vp.MaxDepth = screenDepth;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_DeviceContext->RSSetViewports( 1, &vp );
}

void d3dApp::OnMouseMove()
{
	GetCursorPos(&mMousePos);
	float dx = D3DX_PI/180*0.25f*(mMousePos.x - mLastMousePos.x);
	float dy = D3DX_PI/180*0.25f*(mMousePos.y - mLastMousePos.y);

	mCamRotY += dx;

	mCamRotP += dy;
	mCamera.Pitch(mCamRotP);
	mCamera.RotateY(mCamRotY);
	
	mCamera.UpdateMatrix();

	SetCursorPos(600, 500);
	mLastMousePos = mMousePos;
}

void d3dApp::Keyboards()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		PostQuitMessage(0);
}

