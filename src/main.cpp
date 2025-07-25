#include <Windows.h>
#include <thread>
#include <chrono>

#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")

using namespace std;
using namespace chrono;
int scale = GetSystemMetrics(SM_CYSCREEN) / 2;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			exit(0);
		case WM_CLOSE:
			DestroyWindow(hwnd);
			exit(0);
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			RECT rect;
			GetClientRect(hwnd, &rect);

			HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, brush);
			FillRect(hdc, &rect, brush);

			DeleteObject(brush);
			EndPaint(hwnd, &ps);
		}
		default: 
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

int main() {

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = "ChessWindow";

	if (!RegisterClass(&wc)) {
		return 1;
	}

	HWND hwnd = CreateWindowExW(
		0, L"ChessWindow", L"Chess Window",
		(WS_OVERLAPPEDWINDOW) & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),
		GetSystemMetrics(SM_CXSCREEN) / 2 - (GetSystemMetrics(SM_CXSCREEN) / 4),
		GetSystemMetrics(SM_CYSCREEN) / 4,
		GetSystemMetrics(SM_CYSCREEN) / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2,
		nullptr, nullptr,
		GetModuleHandle(nullptr), nullptr
	);

	if (!hwnd) {
		return 1;
	}

	HWND button = CreateWindowExW(
		0, L"BUTTON", L"Click Me",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		scale * 0.35, scale * 0.8, scale*0.3, scale * 0.1,
		hwnd, nullptr,
		GetModuleHandle(nullptr), nullptr
	);

	if (!button) {
		return 1;
	}

	ShowWindow(button, SW_SHOW);
	ShowWindow(hwnd, SW_SHOW);

	MSG msg;
	time_point lastTime = steady_clock::now();
	while (hwnd) {

		int peek = PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE);
		if (peek == -1) {

		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}

	return 0;

}