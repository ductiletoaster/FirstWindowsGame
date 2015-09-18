#include"Dx11DemoBase.h"
#include"stdio.h"

// Constructor with member initialization list to initilize member variables
Dx11DemoBase::Dx11DemoBase() : driverType_(D3D_DRIVER_TYPE_NULL), featureLevel_(D3D_FEATURE_LEVEL_11_0), d3dDevice_(0), d3dContext_(0), swapChain_(0), backBufferTarget_(0)
{

}

// Basic destructor that calls member function shutdown
Dx11DemoBase::~Dx11DemoBase()
{
	Shutdown();
}

// Simulates constructor delegation
// HINSTANCE handle to an instance
// HWND handle to a window
bool Dx11DemoBase::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	hInstance_ = hInstance; // Save instance locally
	hwnd_ = hwnd; // Save window handle locally
	RECT dimensions; // Create rectagle object
	GetClientRect(hwnd, &dimensions); // Retrieves the coordinates of a window's client area
	unsigned int width = dimensions.right - dimensions.left; // Calc width of window area
	unsigned int height = dimensions.bottom - dimensions.top; // Calc length of window area

	// Enumeration of driver type options
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, 
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE, 
		D3D_DRIVER_TYPE_SOFTWARE
	};

	// Macro for returning the number of elements in the array
	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

	// Enumeration of targeted features for D3D
	// Specifcally allowing the targeting of specific versions of D3D
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	// Macro for returning the number of elements in the array
	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

	// Allows for the creation of a buffer(s)
	// These buffers also have the ability to be swapped
	// Mutliple buffers have the potential to increase performance
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc)); // Allocates memory and fills the block with 0s
	swapChainDesc.BufferCount = 1; // Number of buffers
	swapChainDesc.BufferDesc.Width = width; // Width of buffer
	swapChainDesc.BufferDesc.Height = height; // Height of buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Determines the data format type (A four-component, 32-bit unsigned-normalized-integer format that supports 8 bits per channel including alpha) -- from docs
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60; // The refresh rate is how many times a second it draws the back buffer to the front buffer
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Use the surface or resource as an output render target
	swapChainDesc.OutputWindow = hwnd; // Supply the window handle for output
	swapChainDesc.Windowed = true; // Window mode YES
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	unsigned int creationFlags = 0;

	// Defines a debug flag that if set adds the D3D debug flag on the creation of D3D11CreateDeviceAndSwapChain
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result; // Gives detailed info about the return set specifically any information regarding errors or warnings
	unsigned int driver = 0; // Index for looping through driver types

	// Determine the Driver support level
	// Simultaneously create Swap Chain for given device
	for (driver = 0; driver < totalDriverTypes; ++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(
			0, 
			driverTypes[driver], 
			0, 
			creationFlags, 
			featureLevels, 
			totalFeatureLevels, 
			D3D11_SDK_VERSION, 
			&swapChainDesc,
			&swapChain_,
			&d3dDevice_, &featureLevel_,
			&d3dContext_
		);

		// Check if result was succesful
		if (SUCCEEDED(result))
		{
			driverType_ = driverTypes[driver];
			break;
		}
	}

	// All attempts failed to create D3D device. Exit now
	if (FAILED(result))
	{
		wprintf(L"Failed to create the Direct3D device!"); // Print to stdout (L stands for long character)
		//DXTRACE_MSG( "Failed to create the Direct3D device!" );
		return false;
	}

	ID3D11Texture2D* backBufferTexture;
	result = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	if (FAILED(result))
	{
		wprintf(L"Failed to get the swap chain back buffer!"); // Print to stdout (L stands for long character)
		//DXTRACE_MSG( "Failed to get the swap chain back buffer!" );
		return false;
	}

	// Creates a render view for accessing resource data
	result = d3dDevice_->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget_);
	if (backBufferTexture)
		backBufferTexture->Release(); // Deallocate a resource
	// Failed to create render view
	if (FAILED(result))
	{
		wprintf(L"Failed to create the render target view!"); // Print to stdout (L stands for long character)
		//DXTRACE_MSG( "Failed to create the render target view!" );
		return false;
	}

	// Bind one or more render targets atomically
	d3dContext_->OMSetRenderTargets(1, &backBufferTarget_, 0);

	// Allows for the definition of viewport dimensions
	D3D11_VIEWPORT viewport; 
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	d3dContext_->RSSetViewports(1, &viewport); // Bind viewport to the rasterizer stage of the pipeline
	return LoadContent();
}

bool Dx11DemoBase::LoadContent()
{
	// Override with demo specifics, if any...
	return true;
}
void Dx11DemoBase::UnloadContent()
{
	// Override with demo specifics, if any...
}

// Called by destructor to unload and release allocated memory
void Dx11DemoBase::Shutdown()
{
	UnloadContent();
	if (backBufferTarget_) backBufferTarget_->Release();
	if (swapChain_) swapChain_->Release();
	if (d3dContext_) d3dContext_->Release();
	if (d3dDevice_) d3dDevice_->Release();
	backBufferTarget_ = 0;
	swapChain_ = 0;
	d3dContext_ = 0;
	d3dDevice_ = 0;
}
