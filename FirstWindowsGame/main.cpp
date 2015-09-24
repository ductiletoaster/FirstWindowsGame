/**
 * Brian Gebel (007543547)
 */

// Include dependecies
#include<windows.h>
#include<memory>
#include"TriangleDemo.h"

// Include the Direct3D Library file
// I needed this for compiling in VC2015 community 
// Alternatively you can add the lib under linker "Additional Dependencies"
#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dx11.lib")

// Defines call back function to be used by lpfnWndProc
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// Windows Main function with unicode support
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	// Prevents compiler warnings for unused variables/paramaters
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	WNDCLASSEX wndClass = { 0 }; // Aggregate initialization
	wndClass.cbSize = sizeof(WNDCLASSEX); // Sets to the size in bytes of WNDCLASSEX
	wndClass.style = CS_HREDRAW | CS_VREDRAW; // Bitwise OR operator defining two options
	wndClass.lpfnWndProc = WndProc; // Set program defined callback function
	wndClass.hInstance = hInstance; // Set to given paramater passed in by windows
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // Define cursor options
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // A handle to the class background brush
	wndClass.lpszMenuName = NULL; // Not needed
	wndClass.lpszClassName = "DX11BookWindowClass"; // Specifies the window class name

	// Register window class
	if (!RegisterClassEx(&wndClass))
	{
		return -1; // If registration fails return
	}

	// Create Rectangle and to window
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// Create new handle to new window
	HWND hwnd = CreateWindowA("DX11BookWindowClass", "Blank Win32 Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

	// Check if handle is valid
	if (!hwnd)
	{
		return -1; // If handle is NULL handle function failed and we must exit
	}

	// Display window on screen
	ShowWindow(hwnd, cmdShow);

	// Initialize demo
	TriangleDemo demo;
	bool result = demo.Initialize(hInstance, hwnd); // Call inherited Intialize function in this case from (Dx11DemoBase)
	if (result == false)
		return -1;

	// Create msg struct
	MSG msg = { 0 };

	// Loop until windows boradcasts quit signal
	while (msg.message != WM_QUIT)
	{
		// Retrieves the message if any exist
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); // Translates virtual-key messages into character messages
			DispatchMessage(&msg); // Dispatches a message to a window procedure
		}

		// Update and Draw
		demo.Update(0.0f);
		demo.Render();
	}

	// Demo Shutdown
	demo.Shutdown();

	// Casts and returns additional info about the message
	return static_cast<int>(msg.wParam);
}

// Callback function for std window functionality
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct; // Define struct for painting
	HDC hDC; // Define handle for painting

	switch (message)
	{
		case WM_PAINT:
			hDC = BeginPaint(hwnd, &paintStruct); // Prepares specific window for painting
			EndPaint(hwnd, &paintStruct); // End painting for specific window --  Required for each BeginPaint call
			break;
		case WM_DESTROY:
			PostQuitMessage(0); // Lets the system know that the thread is ready to quit
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam); // Calls the default window procedure
	}
	return 0;
}
