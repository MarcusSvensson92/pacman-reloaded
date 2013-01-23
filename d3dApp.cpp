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

	D3D_DRIVER_TYPE driverType;
	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
	};
		UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]); //same as ARRAYSIZE(x) macro

	UINT m4xMsaaQuality = 4;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof(swapChainDesc) );
	swapChainDesc.BufferDesc.Width							= screenWidth;
	swapChainDesc.BufferDesc.Height							= screenHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator			= 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator		= 1;
	swapChainDesc.BufferDesc.Format							= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering				= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling						= DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count							= 1;
	swapChainDesc.SampleDesc.Quality						= 0;
	swapChainDesc.BufferUsage								= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow								= hwnd;
	swapChainDesc.Windowed									= !fullscreen;
	swapChainDesc.SwapEffect								= DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags										= 0;

	swapChainDesc.BufferCount = 1;

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
	SetDepthStencil((float)screenWidth, (float)screenHeight);
	// Setup the viewport
	SetViewPort((float)screenWidth, (float)screenHeight, screenDepth, screenNear);
}

void d3dApp::Update(float dt)
{
}
void d3dApp::Draw()
{
}


void d3dApp::SetRenderTargetView()
{
}
void d3dApp::SetDepthStencil(float width, float heigth)
{
}
void d3dApp::SetViewPort(float width, float heigth, float screenDepth, float screenNear)
{
}