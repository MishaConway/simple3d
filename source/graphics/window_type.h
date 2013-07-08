#pragma once

#define WINDOWS_STORE_APP 1

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
	#ifdef WINDOWS_STORE_APP
		#include <wrl/client.h>
		#include <ppl.h>
		#include <ppltasks.h>
		#include <agile.h>
		using namespace Windows::UI::Core;

		typedef CoreWindow^ WINDOW_TYPE;
	#else
		typedef HWND WINDOW_TYPE;
	#endif
#else
typedef int WINDOW_TYPE;
#endif