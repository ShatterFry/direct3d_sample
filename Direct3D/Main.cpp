#ifndef UNICDE
#define UNICDE
#endif

#include <windows.h>
#include <string>

HINSTANCE m_hInstance;
//std::string m_WindowClassName;
std::wstring m_WindowClassName = L"Sample Window Class";

LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CLOSE:
		{
			HMENU hMenu;
			hMenu = GetMenu(hwnd);
			if (hMenu != NULL)
			{
				DestroyMenu(hMenu);
			}
			DestroyWindow(hwnd);

			// LPCWSTR (Long Pointer to constant Wide String)
			UnregisterClass(m_WindowClassName.c_str(), m_hInstance);

			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	m_hInstance = hInstance;

	if (m_hInstance == NULL)
	{
		m_hInstance = (HINSTANCE)GetModuleHandle(NULL);
	}

	HICON hIcon = NULL;
	WCHAR szExePath[MAX_PATH];
	GetModuleFileName(NULL, szExePath, MAX_PATH);

	if (hIcon == NULL)
	{
		hIcon = ExtractIcon(m_hInstance, szExePath, 0);
	}

	WNDCLASS wndclass;
	wndclass.style = CS_DBLCLKS;
	wndclass.lpfnWndProc = StaticWindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = m_hInstance;
	wndclass.hIcon = hIcon;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = m_WindowClassName.c_str();

	if (!RegisterClass(&wndclass))
	{
		DWORD dwError = GetLastError();
		if (dwError != ERROR_CLASS_ALREADY_EXISTS)
		{
			return HRESULT_FROM_WIN32(dwError);
		}
	}

	RECT m_rc;
	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;

	HMENU m_hMenu = NULL;

	int nDefaultWidth = 640;
	int nDefaultHeight = 480;
	SetRect(&m_rc, 0, 0, nDefaultWidth, nDefaultHeight);
	AdjustWindowRect(&m_rc, WS_OVERLAPPEDWINDOW, (m_hMenu != NULL) ? true : false);

	HWND m_hWnd = CreateWindow(m_WindowClassName.c_str(), L"Cube11", WS_OVERLAPPEDWINDOW, x, y, (m_rc.right - m_rc.left), (m_rc.bottom - m_rc.top), 0, m_hMenu, m_hInstance, 0);

	if (m_hWnd == NULL)
	{
		DWORD dwError = GetLastError();
		return HRESULT_FROM_WIN32(dwError);
	}

	bool bGotMsg;
	MSG msg;
	msg.message = WM_NULL;
	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			renderer->Update();
			renderer->Render();

			std::shared_ptr < DX::DeviceResources > deviceResources;
			deviceResources->Present();
		}
	}

	return 0;
}