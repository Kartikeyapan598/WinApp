#pragma once

#include <iostream>

#define D3DX_PI 3.1415926535

#define ReleaseCom(x) { if(x) { x->Release(); x=0; } }

#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

static IDXGISwapChain* m_swapchain = NULL;             // the pointer to the swap chain interface
static ID3D11Device* m_d3d_device = NULL;                     // the pointer to our Direct3D device interface
static ID3D11DeviceContext* m_deviceContext = NULL;
static bool m_vsync_enabled = false;
static int m_videoCardMemory = NULL;
static char m_videoCardDescription[128];
static ID3D11RenderTargetView* m_renderTargetView = nullptr;
static ID3D11Texture2D* m_depthStencilBuffer = nullptr;
static ID3D11DepthStencilState* m_depthStencilState = nullptr;
static ID3D11DepthStencilView* m_depthStencilView = nullptr;
static ID3D11RasterizerState* m_rasterState = nullptr;

static bool fullscreen = false;

void SetVSync(bool value)
{
	if (m_vsync_enabled != value)
	{
		m_vsync_enabled = value;
	}
}


void CreateResources(void)
{
	IDXGISwapChain* m_swapchain = nullptr;             // the pointer to the swap chain interface
	ID3D11Device* m_d3d_device = nullptr;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext* m_deviceContext = nullptr;
	bool m_vsync_enabled = false;
	m_renderTargetView = nullptr;
	m_depthStencilBuffer = nullptr;
	m_depthStencilState = nullptr;
	m_depthStencilView = nullptr;
	m_rasterState = nullptr;
}

ID3D11Device* GetDevice()
{
	return m_d3d_device;
}

ID3D11DeviceContext* GetDeviceContext()
{
	return m_deviceContext;
}
void GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

bool RenderInit(unsigned int Width, unsigned int Height, HWND hwnd)
{
	CreateResources();

	HRESULT hr;
	IDXGIFactory* m_device_factory;
	IDXGIAdapter* m_adapter;
	IDXGIOutput* m_adapter_output;
	unsigned int numModes = NULL , i, numerator, denominator;
	unsigned int* stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;						
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// Create a DirectX graphics interface factory.
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_device_factory);
	if (FAILED(hr))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	hr = m_device_factory->EnumAdapters(0, &m_adapter);
	if (FAILED(hr))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	hr = m_adapter->EnumOutputs(0, &m_adapter_output);
	if (FAILED(hr))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	hr = m_adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	hr = m_adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(hr))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)Width)
		{
			if (displayModeList[i].Height == (unsigned int)Height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	hr = m_adapter->GetDesc(&adapterDesc);
	if (FAILED(hr))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	//error = wcstombs_s(stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	//if (error != 0)
	//{
	//	return false;
	//}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	m_adapter_output->Release();
	m_adapter_output = 0;

	// Release the adapter.
	m_adapter->Release();
	m_adapter = 0;

	// Release the factory.
	m_device_factory->Release();
	m_device_factory = 0;

	// Initialize the swap chain description.
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;															// Set to a single back buffer.
	swapChainDesc.BufferDesc.Width = Width;													// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Height = Height;												// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;							// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.RefreshRate.Numerator = RefreshRate;							// Set the refresh rate of the back buffer.
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;							// Set the usage of the back buffer.
	swapChainDesc.OutputWindow = hwnd;														// Set the handle for the window to render to.
	swapChainDesc.SampleDesc.Count = 1;														// Turn multisampling off.
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;						// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;									// Discard the back buffer contents after presenting.
	swapChainDesc.Flags = 0;																// Don't set the advanced flags.

	// Set to full screen or windowed mode.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	featureLevel = D3D_FEATURE_LEVEL_11_0;													// Set the feature level to DirectX 11.

	// Create the swap chain, Direct3D device, and Direct3D device context.
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapchain, &m_d3d_device, NULL, &m_deviceContext);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);		// Get the pointer to the back buffer.
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_d3d_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);	// Create the render target view with the back buffer pointer.
	if (FAILED(hr))
	{
		return false;
	}

	backBufferPtr->Release();																// Release pointer to the back buffer as we no longer need it.
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = Width;
	depthBufferDesc.Height = Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	hr = m_d3d_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	
	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;													// Set up the description of the stencil state.
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	
	// Create the depth stencil state.
	hr = m_d3d_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(hr))
	{
		return false;
	}

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	
	//Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hr = m_d3d_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(hr))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hr = m_d3d_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(hr))
	{
		return false;
	}

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)Width;
	viewport.Height = (float)Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)Width / (float)Height;
	
	//// Create the projection matrix for 3D rendering.
	//D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);
	return true;
}

void ClearResources()
{
	m_swapchain ->Release();             // the pointer to the swap chain interface
	m_d3d_device->Release();                     // the pointer to our Direct3D device interface
	m_deviceContext->Release();
	m_renderTargetView->Release();
	m_depthStencilBuffer->Release();
	m_depthStencilState->Release();
	m_depthStencilView->Release();
	m_rasterState->Release();
}