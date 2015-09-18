#include"BlankDemo.h"
#include<directxmath.h>

BlankDemo::BlankDemo()
{
}

BlankDemo::~BlankDemo()
{
}

bool BlankDemo::LoadContent()
{
	return true;
}

void BlankDemo::UnloadContent()
{
}

void BlankDemo::Update(float dt)
{
}

void BlankDemo::Render()
{
	if (d3dContext_ == 0)
		return;
	float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f }; // Define color
	d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor); // Prepare render target with new color values
	swapChain_->Present(0, 0); // Presents the contents of the next buffer in sequence of back buffer order
}
