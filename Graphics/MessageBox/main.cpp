// TestCodeWindows.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TestCodeWindows.h"
#include "../Surface/bmp_surface.h"
#include "../Drawing/brush.h"
#include "../Drawing/viewport.h"
#include "../JetCat/shadow.h"
#include "../JetCat/frame.h"
#include "../JetCat/background.h"
#include "../JetCat/caption.h"
#include "../JetCat/statusbar.h"
#include "../JetCat/progress.h"
#include "../JetCat/imagebox.h"
#include "../JetCat/inputs.h"
#include "../JetCat/window.h"
#include "../JetCat/evtfilter.h"
#include "../JetCat/placement.h"
#include "../JetCat/buttons.h"
#include "../JetCat/label.h"
#include "../JetCat/menu.h"
#include "../JetCat/cursor.h"
#include "../JetCat/scroller.h"
#include "../JetCat/keypad.h"
#include "../JetCat/editbox.h"
#include "../JetCat/messagebox.h"
#include "../framebuf/bitmapfb.h"
#include "../framebuf/paw.h"
#include "../JetCat-Modules/touch.h"
#include "Integration.h"
#include <list>

using namespace Componentality::Graphics::JetCat;
using namespace Componentality::Graphics;

#define MAX_LOADSTRING 100

std::string global_message = "";
std::string global_title = "";

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

												// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTCODEWINDOWS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTCODEWINDOWS));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTCODEWINDOWS));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TESTCODEWINDOWS);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{
		Componentality::Graphics::Windows::init(hWnd);

		Componentality::Graphics::JetCat::Window& win = *new Componentality::Graphics::JetCat::Window(*Componentality::Graphics::Windows::DEFAULT_WINDOW_MANAGER);
		Componentality::Graphics::JetCat::MessageWindow& mwin = *new Componentality::Graphics::JetCat::MessageWindow(*Componentality::Graphics::Windows::DEFAULT_WINDOW_MANAGER);
		Componentality::Graphics::JetCat::Caption& caption = *new Componentality::Graphics::JetCat::Caption;
		Componentality::Graphics::JetCat::Background& background = *new Componentality::Graphics::JetCat::Background;
		mwin.setText(global_message);
		caption.setTitle(global_title);
		win.add("Background", background);

		mwin.add("Title", caption);
		mwin.show();

		Componentality::Graphics::Windows::DEFAULT_WINDOW_MANAGER->draw();

		break;
	}
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		Componentality::Graphics::Windows::draw(hdc);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		Componentality::Graphics::JetCat::PointerEvent* event = new Componentality::Graphics::JetCat::PointerEvent(
			Componentality::Graphics::Point(xPos, yPos));
		Componentality::Graphics::Windows::DEFAULT_WINDOW_MANAGER->sendEvent(*event);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		Componentality::Graphics::JetCat::PointerEvent* event = new Componentality::Graphics::JetCat::PressEvent(
			Componentality::Graphics::Point(xPos, yPos));
		Componentality::Graphics::Windows::DEFAULT_WINDOW_MANAGER->sendEvent(*event);
		break;
	}
	case WM_LBUTTONUP:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		Componentality::Graphics::JetCat::PointerEvent* event = new Componentality::Graphics::JetCat::DepressEvent(
			Componentality::Graphics::Point(xPos, yPos));
		Componentality::Graphics::Windows::DEFAULT_WINDOW_MANAGER->sendEvent(*event);
		break;
	}
	case WM_KEYUP:
	case WM_KEYDOWN:
	{
		int keycode = 0;
		if (wParam == VK_UP)
			keycode = Componentality::Graphics::JetCat::KeyEvent::VKEY_UP;
		if (wParam == VK_DOWN)
			keycode = Componentality::Graphics::JetCat::KeyEvent::VKEY_DOWN;
		if (wParam == VK_LEFT)
			keycode = Componentality::Graphics::JetCat::KeyEvent::VKEY_LEFT;
		if (wParam == VK_RIGHT)
			keycode = Componentality::Graphics::JetCat::KeyEvent::VKEY_RIGHT;
		if (wParam == VK_BACK)
			keycode = Componentality::Graphics::JetCat::KeyEvent::VKEY_BACKSPACE;
		if (wParam == VK_DELETE)
			keycode = Componentality::Graphics::JetCat::KeyEvent::VKEY_DELETE;
		if (wParam == VK_ESCAPE)
			keycode = Componentality::Graphics::JetCat::KeyEvent::VKEY_ESCAPE;
		if (wParam == VK_RETURN)
			keycode = Componentality::Graphics::JetCat::KeyEvent::VKEY_ENTER;
		if (wParam == VK_TAB)
			keycode = Componentality::Graphics::JetCat::KeyEvent::VKEY_TAB;
		if (keycode)
		{
			Componentality::Graphics::JetCat::KeyEvent* event = new Componentality::Graphics::JetCat::KeyEvent
				(NULL, Componentality::Graphics::JetCat::KeyEvent::SPECIAL,
					message == WM_KEYUP ? Componentality::Graphics::JetCat::KeyEvent::UP : Componentality::Graphics::JetCat::KeyEvent::DOWN,
					keycode);
			Componentality::Graphics::Windows::DEFAULT_WINDOW_MANAGER->sendEvent(*event);
		}

		break;
	}
	case WM_CHAR:
	{
		{
			Componentality::Graphics::JetCat::KeyEvent* event = new Componentality::Graphics::JetCat::KeyEvent
				(NULL, Componentality::Graphics::JetCat::KeyEvent::ALPHABETH,
					message == WM_KEYUP ? Componentality::Graphics::JetCat::KeyEvent::UP : Componentality::Graphics::JetCat::KeyEvent::DOWN,
					wParam);
			Componentality::Graphics::Windows::DEFAULT_WINDOW_MANAGER->sendEvent(*event);
		}
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
