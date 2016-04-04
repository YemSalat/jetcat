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
#include "../JetCat/menu.h"
#include "../JetCat/keypad.h"
#include "../JetCat/editbox.h"
#include "../JetCat/messagebox.h"
#include "../framebuf/bitmapfb.h"
#include "../framebuf/paw.h"
#include "../JetCat-Modules/touch.h"
#include "Integration.h"
#include <list>

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

class MessageShow : public ActiveArea
{
protected:
	std::list<::Point> mPoints;
	StdEventFilter filter;
public:
	MessageShow(IElement* owner) { setEventFilter(&filter); };
	virtual ~MessageShow() {};
	Margins draw(::ISurface& surface)
	{
		Drawer drawer(surface);
		for (std::list<::Point>::iterator i = mPoints.begin(); i != mPoints.end(); i++)
		{
			Point point = *i;
			Point shift = getTopLeft();
			point = point - shift;
			drawer.point(point, Color(0xFF, 0xFF, 0xFF));
		}
		return IElement::draw(surface);
	}
	bool onEvent(CST::Common::Event& event)
	{
/*		if (event.getType() == EVENT_TYPE_POINTER)
		{
			::PointerEvent& _event = (::PointerEvent&) event;
			mPoints.push_back(_event.get());
			raise(*new DrawRequestEvent(this));
		}
*/		return IElement::onEvent(event);
	}
	protected: 
		void onTimer() {}
};

IElement& makeCursorButtons()
{
	std::string path = "C:\\Componentality\\mhui\\Resources\\Icons";
	Placement& placement_left = *new Placement;
	Placement& placement_right = *new Placement;
	Placement& placement_up = *new Placement;
	Placement& placement_down = *new Placement;

	KeyButton& left = *new KeyButton(
		CST::Common::fileJoinPaths(path, "left.bmp"),
		CST::Common::fileJoinPaths(path, "left_pressed.bmp"),
		CST::Common::fileJoinPaths(path, "left.bmp"),
		Color(0, 0, 0),
		Color(0, 0, 0),
		Color(0, 0, 0),
		KeyEvent::SPECIAL,
		KeyEvent::VKEY_LEFT,
		""
		);

	KeyButton& right = *new KeyButton(
		CST::Common::fileJoinPaths(path, "right.bmp"),
		CST::Common::fileJoinPaths(path, "right_pressed.bmp"),
		CST::Common::fileJoinPaths(path, "right.bmp"),
		Color(0, 0, 0),
		Color(0, 0, 0),
		Color(0, 0, 0),
		KeyEvent::SPECIAL,
		KeyEvent::VKEY_RIGHT,
		""
		);

	KeyButton& up = *new KeyButton(
		CST::Common::fileJoinPaths(path, "up.bmp"),
		CST::Common::fileJoinPaths(path, "up_pressed.bmp"),
		CST::Common::fileJoinPaths(path, "up.bmp"),
		Color(0, 0, 0),
		Color(0, 0, 0),
		Color(0, 0, 0),
		KeyEvent::SPECIAL,
		KeyEvent::VKEY_UP,
		""
		);

	KeyButton& down = *new KeyButton(
		CST::Common::fileJoinPaths(path, "down.bmp"),
		CST::Common::fileJoinPaths(path, "down_pressed.bmp"),
		CST::Common::fileJoinPaths(path, "down.bmp"),
		Color(0, 0, 0),
		Color(0, 0, 0),
		Color(0, 0, 0),
		KeyEvent::SPECIAL,
		KeyEvent::VKEY_DOWN,
		""
		);

	placement_left.add("button", left);
	placement_right.add("button", right);
	placement_up.add("button", up);
	placement_down.add("button", down);

	Placement& placement = *new Placement;
	placement.add("left", placement_left);
	placement.add("right", placement_right);
	placement.add("up", placement_up);
	placement.add("down", placement_down);

	placement_left.move(0, 28);
	placement_right.move(140, 28);
	placement_up.move(70, 0);
	placement_down.move(70, 70);

	return placement;
}

#define MAX_LOADSTRING 100


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
	if (!InitInstance (hInstance, nCmdShow))
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

	return (int) msg.wParam;
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

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTCODEWINDOWS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TESTCODEWINDOWS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
		Windows::init(hWnd);

		Shadow& shadow = *new Shadow;
		Frame& frame = *new Frame;
		Background& background = *new Background;
		Caption& caption = *new Caption;
		StatusBar& status = *new StatusBar;
		caption.setTitle("Test Window: it is a very-very long header to be cut off");
		status.setTitle("Status line");
		Window& painter = *new Window(*::Windows::DEFAULT_WINDOW_MANAGER);
		TouchCalibrator& tcal = *new TouchCalibrator;
		TouchEventsProcessor& tep = *new TouchEventsProcessor(300);
		painter.add("touch", tcal);
		tcal.add("touch", tep);

		tep.add("shadow", shadow);
		tep.add("frame", frame);
		tep.add("caption", caption);
		tep.add("status", status);
		tep.add("background", background);

		((Background*)tep.get("background"))->setColor(Color(50, 50, 50));
		((Shadow*)tep.get("shadow"))->setWidth(3);
		((Frame*)tep.get("frame"))->setWidth(1);

//		tep.add("active", *new MessageShow(NULL));

		painter.move(0, 0);

		Placement& placement1 = *new Placement;
		tep.add("placement1", placement1);
		placement1.move(200, 200);
		placement1.size(200, 200);

		Image& image = *new Image("c:\\temp\\test.bmp");
		placement1.add("image0", image);

		Placement& multiline_placement = *new Placement(50, 50, 100, 100);
		MultilineLabel& mlabel = *new MultilineLabel("Multiline\nLabel\n#1");
		mlabel.setColor(::Color(255, 255, 255));
		mlabel.setAlignment(IAlignmentDefinitions::Alignments(ITextOutput::H_CENTER, ITextOutput::V_TOP));
		multiline_placement.add("mll", mlabel);
		placement1.add("mlp", multiline_placement);

		Placement& placement2 = *new Scroller(10, 10);
		background.add("placement2", placement2);
		placement2.move(50, 100);
		placement2.size(100, 30);
		Placement& placement3 = *new Placement;
		background.add("placement3", placement3);
		placement3.move(50, 200);
		placement3.size(30, 100);

		ProgressIndicator& progress = *new ProgressIndicator(
			IProgressIndicator::HORIZONTAL,
			Color(255, 255, 255),
			Color(80, 80, 80),
			Margins(10, 5, 10, 5),
			Color(0, 0, 0),
			Color(80, 80, 80));
		progress.indicate(50);
		ProgressIndicator& progressv = *new ProgressIndicator(
			IProgressIndicator::VERTICAL,
			Color(255, 255, 255),
			Color(80, 80, 80),
			Margins(5, 5, 5, 5),
			Color(0, 0, 0),
			Color(255, 255, 255));
		progressv.indicate(35);

		placement2.add("progress", progress);
		placement3.add("progressv", progressv);

		{
			TextButton& tbutton = *new TextButton(
				"OK",
				Color(255, 255, 255),
				Color(0, 255, 0),
				Color(0, 0, 0));
			Placement& tb_placement = *new Placement;
			tb_placement.add("button", tbutton);
			tb_placement.move(250, 250);
			tb_placement.size(80, 30);

			TextButton& cbutton = *new TextButton(
				"Cancel",
				Color(255, 255, 255),
				Color(255, 0, 0),
				Color(0, 0, 0));
			MessageWindow* mbox = new MessageWindow(
				*Windows::DEFAULT_WINDOW_MANAGER, true);
			mbox->setText("New device found");
			mbox->addButton("OK", tbutton);
			mbox->addButton("Cancel", cbutton);
			mbox->hide();
		}

		if (1)
		{
			Placement& menuholder = *new Placement;
			VerticalMenu &menu = *new VerticalMenu(*::Windows::DEFAULT_WINDOW_MANAGER, 150);
			TextMenuItem& item1_unsel = *new TextMenuItem(menu,
				Color(255, 255, 255), Color(0, 0, 0), "Item 1");
			TextMenuItem& item1_sel = *new TextMenuItem(menu,
				Color(0, 0, 0), Color(255, 255, 255), "Item 1");
			TextMenuItem& item2_unsel = *new TextMenuItem(menu,
				Color(255, 255, 255), Color(0, 0, 0), "Item 2");
			TextMenuItem& item2_sel = *new TextMenuItem(menu,
				Color(0, 0, 0), Color(255, 255, 255), "Item 2");
			TextMenuItem& item3_unsel = *new TextMenuItem(menu,
				Color(255, 255, 255), Color(0, 0, 0), "Item 3");
			TextMenuItem& item3_sel = *new TextMenuItem(menu,
				Color(0, 0, 0), Color(255, 255, 255), "Item 3");
			TextMenuItem& item4_unsel = *new TextMenuItem(menu,
				Color(255, 255, 255), Color(0, 0, 0), "Item 4");
			TextMenuItem& item4_sel = *new TextMenuItem(menu,
				Color(0, 0, 0), Color(255, 255, 255), "Item 4");
			menuholder.add("menu", menu);
			menu.add("item 1", item1_unsel, item1_sel, 40);
			menu.add("item 2", item2_unsel, item2_sel, 40);
			menu.add("item 3", item3_unsel, item3_sel, 40);
			menu.add("item 4", item4_unsel, item4_sel, 40);
			menu.select("item 3");
			menuholder.move(150, 150);
			menuholder.size(100, 140);

//				item2_sel.hide();
//				item2_unsel.hide();

			Placement& hmenuholder = *new Placement;
			IHorizontalMenu &hmenu = *new HorizontalMenu(*::Windows::DEFAULT_WINDOW_MANAGER, 30);
			TextMenuItem& hitem1_unsel = *new TextMenuItem(hmenu,
				Color(255, 255, 255), Color(0, 0, 0), "Item 1");
			TextMenuItem& hitem1_sel = *new TextMenuItem(hmenu,
				Color(0, 0, 0), Color(255, 255, 255), "Item 1");
			TextMenuItem& hitem2_unsel = *new TextMenuItem(hmenu,
				Color(255, 255, 255), Color(0, 0, 0), "Item 2");
			TextMenuItem& hitem2_sel = *new TextMenuItem(hmenu,
				Color(0, 0, 0), Color(255, 255, 255), "Item 2");
			TextMenuItem& hitem3_unsel = *new TextMenuItem(hmenu,
				Color(255, 255, 255), Color(0, 0, 0), "Item 3");
			TextMenuItem& hitem3_sel = *new TextMenuItem(hmenu,
				Color(0, 0, 0), Color(255, 255, 255), "Item 3");
			hmenuholder.add("hmenu", hmenu);
			hmenu.add("item 1", hitem1_unsel, hitem1_sel, 140);
			hmenu.add("item 2", hitem2_unsel, hitem2_sel, 240);
			hmenu.add("item 3", hitem3_unsel, hitem3_sel, 340);
			hmenu.select("item 2");
			hmenuholder.move(50, 50);
			hmenuholder.size(600, 30);

			//menu.setSelectionStrategy(::IMenu::MULTIPLE);

			tep.add("menuholder", menuholder);
			tep.add("hmenuholder", hmenuholder);

			Placement& placement_l = *new Placement;
			placement_l.move(500, 300);
			placement_l.size(80, 28);
			tep.add("placement_l", placement_l);
			EditBox& eb = *new EditBox(*Windows::DEFAULT_WINDOW_MANAGER, "Default Text",
				Color(0, 0, 0));
			eb.setCursorColor(Color(0, 0, 0));
			eb.setFrameColor(Color(0, 0, 0));
			eb.setBackgroundColor(Color(128, 128, 128));
			eb.setPosition(3);
			placement_l.add("editbox", eb);
		}

		if (0)
		{
			std::string path = "C:\\Componentality\\mhui\\Resources\\Icons";
			StandardKeypad* ak = new StandardKeypad
				(40, 35, 20,
					CST::Common::fileJoinPaths(path, "button.bmp"),
					CST::Common::fileJoinPaths(path, "button.bmp"),
					CST::Common::fileJoinPaths(path, "button.bmp"),
					CST::Common::fileJoinPaths(path, "space_button.bmp"),
					CST::Common::fileJoinPaths(path, "space_button.bmp"),
					CST::Common::fileJoinPaths(path, "space_button.bmp"),
					CST::Common::fileJoinPaths(path, "special_button.bmp"),
					CST::Common::fileJoinPaths(path, "special_button.bmp"),
					CST::Common::fileJoinPaths(path, "special_button.bmp"),
					Color(255, 255, 255),
					Color(80, 80, 80),
					Color(80, 80, 80));
			Placement* keyplace = new Placement;
			tep.add("keypad", *keyplace);
			keyplace->add("key1", *ak);

			tep.add("cursor", makeCursorButtons());
		}


		Windows::DEFAULT_WINDOW_MANAGER->draw();

		painter.setEventFilter(new StdEventFilter());

		break;
	}
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
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
		
		Windows::draw(hdc);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		PointerEvent* event = new PointerEvent(
			Point(xPos, yPos));
		Windows::DEFAULT_WINDOW_MANAGER->sendEvent(*event);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		::PointerEvent* event = new PressEvent(
			::Point(xPos, yPos));
		::Windows::DEFAULT_WINDOW_MANAGER->sendEvent(*event);
		break;
	}
	case WM_LBUTTONUP:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		PointerEvent* event = new DepressEvent(
			Point(xPos, yPos));
		Windows::DEFAULT_WINDOW_MANAGER->sendEvent(*event);
		break;
	}
	case WM_KEYUP:
	case WM_KEYDOWN:
	{
		int keycode = 0;
		if (wParam == VK_UP)
			keycode = KeyEvent::VKEY_UP;
		if (wParam == VK_DOWN)
			keycode = KeyEvent::VKEY_DOWN;
		if (wParam == VK_LEFT)
			keycode = KeyEvent::VKEY_LEFT;
		if (wParam == VK_RIGHT)
			keycode = KeyEvent::VKEY_RIGHT;
		if (wParam == VK_BACK)
			keycode = KeyEvent::VKEY_BACKSPACE;
		if (wParam == VK_DELETE)
			keycode = KeyEvent::VKEY_DELETE;
		if (wParam == VK_ESCAPE)
			keycode = KeyEvent::VKEY_ESCAPE;
		if (wParam == VK_RETURN)
			keycode = KeyEvent::VKEY_ENTER;
		if (wParam == VK_TAB)
			keycode = KeyEvent::VKEY_TAB;
		if (keycode)
		{
			KeyEvent* event = new KeyEvent
				(NULL, KeyEvent::SPECIAL,
					message == WM_KEYUP ? KeyEvent::UP : KeyEvent::DOWN,
					keycode);
			Windows::DEFAULT_WINDOW_MANAGER->sendEvent(*event);
		}

		break;
	}
	case WM_CHAR:
	{
		{
			KeyEvent* event = new KeyEvent
				(NULL, KeyEvent::ALPHABETH,
					message == WM_KEYUP ? KeyEvent::UP : KeyEvent::DOWN,
					wParam);
			Windows::DEFAULT_WINDOW_MANAGER->sendEvent(*event);
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
