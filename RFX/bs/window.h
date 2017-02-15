#pragma once
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <map>
#include <vector>
#include <functional>
#include <assert.h>

// Console window includes
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#include "vectors.h"

#define wndClassName L"bsDefaultWndClass"

namespace bs
{
	/* Wrappers for some WINAPI window creation and management functions needed in game engine. */

	class IWindow
	{
	public:
								IWindow(HWND _hWnd) : m_hWnd(_hWnd){}

		void					setPosition(const TInt2& _position);
		TInt2					getPosition() const;
		void					setSize(const TUInt2& _size);
		TUInt2					getSize() const;
		void					setClientSize(const TUInt2& _clientSize);
		TUInt2					getClientSize() const;
		void					setBorder(bool _border);
		HWND					getHWnd() const;
		void					showWindow(bool _show);
		bool					getHasFocus();
		bool					getIsAlive();
		void					setTopMost(bool _topMost);
		void					setTitle(std::string _title);

	protected:
								IWindow(){}
		HWND					m_hWnd;
	};

	class CWindow : public IWindow
	{
		typedef std::function<void(CWindow&, WPARAM, LPARAM)> TCallBackFunc;
		typedef std::map <UINT, std::vector<TCallBackFunc>> TMsgCallBackMap;
	public:
								CWindow();
								~CWindow();

		size_t					registerMsgCallBack(UINT msg, TCallBackFunc _callBack);
		void					unregisterMsgCallBack(UINT msg, size_t _callBackID);
		bool					getIsAlive();

		static void				initializeWindowClass(HINSTANCE _hInstance);

	private:
		static LRESULT WINAPI	msgDetour(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static CWindow*			getObjectFromWindow(HWND hWnd);

		//static WNDCLASSEX		windowClass;

		LRESULT WINAPI			msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		TMsgCallBackMap			m_msgCallBacks;
		bool					m_isAlive;
	};

	/* Inline functions */
	inline CWindow* CWindow::getObjectFromWindow(HWND hWnd)
	{
		return reinterpret_cast<CWindow*>(GetWindowLong(hWnd, GWL_USERDATA));
	}

	inline void IWindow::setPosition(const TInt2& _position)
	{
		TUInt2 size = getSize();
		MoveWindow(m_hWnd, _position.x(), _position.y(), size.x(), size.y(), TRUE);
	}

	inline TInt2 IWindow::getPosition() const
	{
		RECT wndRect;
		GetWindowRect(m_hWnd, &wndRect);
		return TInt2(wndRect.left, wndRect.top);
	}

	inline void IWindow::setSize(const TUInt2& _size)
	{
		TInt2 position = getPosition();
		MoveWindow(m_hWnd, position.x(), position.y(), _size.x(), _size.y(), TRUE);
	}

	inline TUInt2 IWindow::getSize() const
	{
		RECT wndRect;
		GetWindowRect(m_hWnd, &wndRect);
		return TUInt2(wndRect.right - wndRect.left, wndRect.bottom - wndRect.top);
	}

	inline void IWindow::setClientSize(const TUInt2& _clientSize)
	{
		setSize(_clientSize);
		setSize(_clientSize + (_clientSize - getClientSize()));
	}

	inline TUInt2 IWindow::getClientSize() const
	{
		RECT wndClientRect;
		GetClientRect(m_hWnd, &wndClientRect);
		return TUInt2(wndClientRect.right - wndClientRect.left, wndClientRect.bottom - wndClientRect.top);
	}

	inline void IWindow::setBorder(bool _border)
	{
		SetWindowLong(m_hWnd, GWL_STYLE, _border ? WS_VISIBLE | WS_SYSMENU | WS_CAPTION : WS_VISIBLE);
	}

	inline HWND IWindow::getHWnd() const
	{
		return m_hWnd;
	}

	inline void IWindow::showWindow(bool _show)
	{
		ShowWindow(m_hWnd, _show ? SW_SHOW : SW_HIDE);
	}

	inline bool IWindow::getHasFocus()
	{
		/* Could also be done by maintaining a bool based on WM_ACTIVATE or WM_SETFOCUS/WM_KILLFOCUS.*/
		return GetForegroundWindow() == m_hWnd;
	}

	inline void IWindow::setTopMost(bool _topMost)
	{
		SetWindowPos(m_hWnd, _topMost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	inline void IWindow::setTitle(std::string _title)
	{
		SetWindowTextA(m_hWnd, _title.c_str());
	}

	inline size_t CWindow::registerMsgCallBack(UINT msg, TCallBackFunc _callBack)
	{
		auto& vec = m_msgCallBacks[msg];
		vec.push_back(_callBack);
		return vec.size() - 1;
	}

	inline bool CWindow::getIsAlive()
	{
		/*return IsWindow(m_hWnd) != FALSE;*/
		return m_isAlive;
	}
	
	inline CWindow::CWindow()
	{
		/* Make sure the window class has been properly initialized. */
		//assert(windowClass.cbSize && "Window class has to be initialized before creating a window.");

		m_hWnd = CreateWindowW(wndClassName, L"bsGameG", WS_VISIBLE | WS_SYSMENU | WS_CAPTION, CW_USEDEFAULT, CW_USEDEFAULT,
								CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, 0,
								reinterpret_cast<void*>(this));
		assert(m_hWnd && "Failed to create window.");
	}

	inline CWindow::~CWindow()
	{
		if (m_hWnd)
		{
			DestroyWindow(m_hWnd);
			m_hWnd = NULL;
		}
	}

	inline LRESULT WINAPI CWindow::msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CREATE:
			m_isAlive = true;
			break;
		case WM_DESTROY:
			m_isAlive = false;
			break;
		}

		if (!m_msgCallBacks.empty())
		{
			auto it = m_msgCallBacks.find(msg);
			if (it != m_msgCallBacks.end())
			{
				for (auto callBack : it->second)
				{
					callBack(*this, wParam, lParam);
				}
			}
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	/* Ugly part. */
	//WNDCLASSEX CWindow::windowClass = {0};

	inline void CWindow::initializeWindowClass(HINSTANCE _hInstance)
	{
		WNDCLASSEX windowClass = { 0 };
		ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_CLASSDC;
		windowClass.lpfnWndProc = msgDetour;
		windowClass.hInstance = _hInstance;
		windowClass.lpszClassName = wndClassName;
		windowClass.hbrBackground = HBRUSH(COLOR_BACKGROUND);
		//windowClass.hIconSm = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_SNAKE_ICON));
		//windowClass.hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_SNAKE_ICON));
		//windowClass.hCursor = LoadCursor(NULL, IDC_CROSS);
		RegisterClassEx(&windowClass);
	}

	inline LRESULT WINAPI CWindow::msgDetour(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			SetWindowLong(hWnd, GWL_USERDATA, (long)(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams));
		}
		CWindow* pThis = getObjectFromWindow(hWnd);
		if (pThis)
		{
			return pThis->msgProc(hWnd, msg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	namespace debug 
	{
		// maximum mumber of lines the output console should have
		static const WORD MAX_CONSOLE_LINES = 512;

		inline IWindow createConsoleWindow()
		{
			int hConHandle;
			long lStdHandle;
			CONSOLE_SCREEN_BUFFER_INFO coninfo;
			FILE *fp;

			// allocate a console for this app
			AllocConsole();

			// set the screen buffer to be big enough to let us scroll text
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
			coninfo.dwSize.Y = MAX_CONSOLE_LINES;
			SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

			// redirect unbuffered STDOUT to the console
			lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

			fp = _fdopen(hConHandle, "w");

			*stdout = *fp;

			setvbuf(stdout, NULL, _IONBF, 0);

			// redirect unbuffered STDIN to the console

			lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

			fp = _fdopen(hConHandle, "r");
			*stdin = *fp;
			setvbuf(stdin, NULL, _IONBF, 0);

			// redirect unbuffered STDERR to the console
			lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

			fp = _fdopen(hConHandle, "w");

			*stderr = *fp;

			setvbuf(stderr, NULL, _IONBF, 0);

			// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
			// point to console as well
			std::ios::sync_with_stdio();
			FILE* pp;
			freopen_s(&pp, "CONOUT$", "w", stdout);

			return IWindow(GetConsoleWindow());
		}
	}
}
