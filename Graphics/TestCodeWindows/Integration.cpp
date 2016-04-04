#include "stdafx.h"
#include "Integration.h"
#include "../Drawing/scale.h"
#include "../framebuf/bitmapfb.h"
#include "../framebuf/framebuf.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::Windows;

#define X_SCALE 1
#define Y_SCALE 1

Componentality::Graphics::BitmapSurface framebuffer(1280, 600);
Componentality::Graphics::JetCat::WindowManager* Componentality::Graphics::Windows::DEFAULT_WINDOW_MANAGER = NULL;

void Componentality::Graphics::Windows::init(HWND wnd)
{
	DEFAULT_WINDOW_MANAGER = new WindowManager(framebuffer, wnd);
}

void Componentality::Graphics::Windows::draw(HDC hdc)
{
	Componentality::Graphics::Windows::draw(hdc, *DEFAULT_WINDOW_MANAGER->getSurface());
}

void Componentality::Graphics::Windows::draw(HDC hdc, ISurface& surface)
{
	BitmapSurface temp_surface(surface.getWidth(), surface.getHeight());
	temp_surface.apply(surface);

	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biWidth = surface.getWidth();
	bmi.bmiHeader.biHeight = surface.getHeight();;
	bmi.bmiHeader.biPlanes = 1;

	void *bits;

	HDC dc = CreateCompatibleDC(NULL);
	HBITMAP hbitmap = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
	BITMAP bm;
	memcpy(bits, temp_surface.getColorMemory(), temp_surface.getWidth() * temp_surface.getHeight() * 3);

	HBITMAP hbmOld = (HBITMAP) SelectObject(dc, (HGDIOBJ) hbitmap);

	GetObject(hbitmap, sizeof(bm), &bm);

	BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, dc, 0, 0, SRCCOPY);

	SelectObject(dc, hbmOld);
	DeleteObject(hbitmap);
	DeleteDC(dc);
}

/////////////////////////////////////////////////////////////////////

WindowManager::WindowManager(ISurface& surface, HWND dc) : Componentality::Graphics::JetCat::WindowManager(surface), mWnd(dc)
{
}

WindowManager::~WindowManager()
{

}

void WindowManager::draw()
{
	Componentality::Graphics::JetCat::WindowManager::draw();
	HDC hdc = GetDC(mWnd);
	Componentality::Graphics::Windows::draw(hdc);
	ReleaseDC(mWnd, hdc);
}

bool WindowManager::onEvent(CST::Common::Event& event)
{
	bool result = Componentality::Graphics::JetCat::WindowManager::onEvent(event);
	if (event.getType() == Componentality::Graphics::JetCat::EVENT_TYPE_DRAW_REQUEST)
	{
		HDC hdc = GetDC(mWnd);
		Componentality::Graphics::Windows::draw(hdc);
		ReleaseDC(mWnd, hdc);
	}
	return result;
}
