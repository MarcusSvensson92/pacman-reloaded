#include "stdafx.h"
#include "d3dApp.h"

d3dApp::d3dApp(void)
{
}
d3dApp::~d3dApp(void)
{
}

Obj3D test;

void d3dApp::Init(HINSTANCE hinstance, HWND hwnd, bool vsync, bool fullscreen, float screenDepth, float screenNear)
{
	mCamera = Camera();

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
			&g_SwapChain,
			&g_Device,
			&initiatedFeatureLevel,
			&g_DeviceContext);
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

	InitMouse(screenWidth, screenHeight);

}

void d3dApp::InitMouse( int screenWidth, int screenHeight )
{
	mCamera.SetLens(0.45f*D3DX_PI, screenWidth / screenHeight, 1.0f, 1000.0f);

	test = Obj3D(g_Device,g_DeviceContext,D3DXVECTOR3(0,0,0), D3DXVECTOR3(1,1,1));

	mCamera.LookAt(D3DXVECTOR3(0, 0,-5), D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,1,0));
	SetCursorPos(600,500);
	GetCursorPos(&mLastMousePos);

	mCamRotP = 0;
	mCamRotY = 0;
}


void d3dApp::Update(float dt)
{
	// calculate WVP matrix
	//D3DXMatrixIdentity(&g_World);
	//g_WVP = g_World;// * m_cam.getView() * m_cam.getProj();
 	OnMouseMove();
 	Keyboards();
	// calculate iWVP matrix
	//float det = D3DXMatrixDeterminant(&g_World);
	//D3DXMatrixInverse(&g_iWorld,&det,&g_World);
	//D3DXMatrixTranspose(&g_iWorld, &g_iWorld);
}
void d3dApp::DrawBegin()
{
	static float ClearColor[4] = { 0.5f, 0.7f, 1.0f, 1.0f };
	g_DeviceContext->ClearRenderTargetView( g_RenderTargetView, ClearColor );	

	g_DeviceContext->ClearDepthStencilView( g_DepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0 );
}
void d3dApp::DrawEnd()
{
	g_SwapChain->Present( 1, 0 );
}

void d3dApp::SetRenderTargetView()
{
	// hresult for bugtesting
	HRESULT hr; 

	ID3D11Texture2D* pBackBuffer;
	hr = g_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer );
	if( FAILED(hr) )
		MessageBoxA(0, "Failed GetBuffer", 0, 0);

	hr = g_Device->CreateRenderTargetView( pBackBuffer, 0, &g_RenderTargetView );
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
	hr = g_Device->CreateTexture2D( &descDepth, NULL, &g_DepthStencil );
	if( FAILED(hr) )
		MessageBoxA(0, "Failed CreateTexture2D", 0, 0);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = g_Device->CreateDepthStencilView( g_DepthStencil, 0, &g_DepthStencilView );
	if( FAILED(hr) )
		MessageBoxA(0, "Failed CreateDepthStencilView", 0, 0);

	g_DeviceContext->OMSetRenderTargets( 1, &g_RenderTargetView, g_DepthStencilView );
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
	g_DeviceContext->RSSetViewports( 1, &vp );
}

void d3dApp::Render()
{

	DrawBegin();

	test.Draw(g_DeviceContext,mCamera);

	DrawEnd();

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
	if (GetAsyncKeyState('W') & 0x8000)
		mCamera.Walk(0.5f);
	if (GetAsyncKeyState('S') & 0x8000)
		mCamera.Walk(-0.5f);
	if (GetAsyncKeyState('A') & 0x8000)
		mCamera.Strafe(-0.5f,0.001f);
	if (GetAsyncKeyState('D') & 0x8000)
		mCamera.Strafe(0.5f, -0.001f);
}

