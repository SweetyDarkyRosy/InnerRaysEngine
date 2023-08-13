//-----------------------------------------------------------------------------
// File: x_ray.cpp
//
// Programmers:
//	Oles		- Oles Shishkovtsov
//	AlexMX		- Alexander Maksimchuk
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include <process.h>
#include <locale.h>

#include "CApplication.h"

#include "igame_persistent.h"

#include "dedicated_server_only.h"
#include "no_single.h"

#include "GameFont.h"
#include "xr_input.h"
#include "xrSash.h"
#include "std_classes.h"
#include "resource.h"
#include "LightAnimLibrary.h"
#include "ispatial.h"
#include "Text_Console.h"


#define VIRT_ERROR_SIZE 256
#define VIRT_MESSAGE_SIZE 512


/*[
	Global variables
 ]*/

BOOL							g_bIntroFinished = FALSE;
ENGINE_API bool					g_dedicated_server = false;

ENGINE_API CInifile*			pGameIni = NULL;
ENGINE_API CApplication*		pApp = NULL;
static HWND						logoWindow = NULL;
CTimer							phase_timer;
extern ENGINE_API BOOL			g_appLoaded = FALSE;

HMODULE							hLauncher = NULL;

// For computing build id
XRCORE_API LPCSTR				build_date;
XRCORE_API u32					build_id;

#ifdef MASTER_GOLD
	#define NO_MULTI_INSTANCES
#endif

static int						start_day = 25;
static int						start_month = 2;
static int						start_year = 2023;

static LPSTR month_id[12] =
{
	"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};

static int days_in_month[12] =
{
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

ENGINE_API bool					g_bBenchmark = false;
string512						g_sBenchmarkName;

ENGINE_API string512			g_sLaunchOnExit_params;
ENGINE_API string512			g_sLaunchOnExit_app;
ENGINE_API string_path			g_sLaunchWorkingFolder;


extern "C"
{
	typedef int __cdecl LauncherFunc(int);
}

LauncherFunc*					pLauncher = NULL;


extern void Intro(void* fn);
extern void Intro_DSHOW(void* fn);
extern int PASCAL IntroDSHOW_wnd(HINSTANCE hInstC, HINSTANCE hInstP, LPSTR lpCmdLine, int nCmdShow);
void doBenchmark(LPCSTR name);
extern void testbed(void);

void compute_build_id()
{
	build_date = __DATE__;

	int days;
	int months = 0;
	int years;
	string16 month;
	string256 buffer;
	strcpy_s(buffer, __DATE__);
	sscanf(buffer, "%s %d %d", month, &days, &years);

	for (int i = 0; i < 12; i++)
	{
		if (_stricmp(month_id[i], month))
		{
			continue;
		}

		months = i;
		break;
	}

	build_id = (years - start_year) * 365 + days - start_day;

	for (int i = 0; i < months; ++i)
	{
		build_id += days_in_month[i];
	}

	for (int i = 0; i < start_month - 1; ++i)
	{
		build_id -= days_in_month[i];
	}
}

void InitEngine()
{
	Engine.Initialize();
	while (!g_bIntroFinished)
	{
		Sleep(100);
	}

	Device.Initialize();
}

PROTECT_API void InitSettings()
{
	string_path fname;
	FS.update_path(fname, "$game_config$", "system.ltx");
#ifdef DEBUG
	Msg("Updated path to system.ltx is %s", fname);
#endif
	pSettings = xr_new<CInifile>(fname, TRUE);
	CHECK_OR_EXIT(!pSettings->sections().empty(), make_string("Cannot find file %s.\nReinstalling application may fix this problem.", fname));

	FS.update_path(fname, "$game_config$", "game.ltx");
	pGameIni = xr_new<CInifile>(fname, TRUE);
	CHECK_OR_EXIT(!pGameIni->sections().empty(), make_string("Cannot find file %s.\nReinstalling application may fix this problem.", fname));
}

PROTECT_API void InitConsole()
{
#ifdef DEDICATED_SERVER
	{
		Console = xr_new<CTextConsole>();
	}
#else
	{
		Console = xr_new<CConsole>();
	}
#endif
	Console->Initialize();

	strcpy_s(Console->ConfigFile, "user.ltx");
	if (strstr(Core.Params, "-ltx "))
	{
		string64 c_name;
		sscanf(strstr(Core.Params, "-ltx ") + 5, "%[^ ] ", c_name);
		strcpy_s(Console->ConfigFile, c_name);
	}
}

PROTECT_API void InitInput()
{
	BOOL bCaptureInput = !strstr(Core.Params, "-i");

	pInput = xr_new<CInput>(bCaptureInput);
}

void DestroyInput()
{
	xr_delete(pInput);
}

PROTECT_API void InitSound1()
{
	CSound_manager_interface::_create(0);
}

PROTECT_API void InitSound2()
{
	CSound_manager_interface::_create(1);
}

void DestroySound()
{
	CSound_manager_interface::_destroy();
}

void DestroySettings()
{
	xr_delete(pSettings);
	xr_delete(pGameIni);
}

void DestroyConsole()
{
	Console->Execute("cfg_save");
	Console->Destroy();
	xr_delete(Console);
}

void DestroyEngine()
{
	Device.Destroy();
	Engine.Destroy();
}

void ExecuteUserScript()
{
	// Execute script

	Console->Execute("unbindall");
	Console->ExecuteScript(Console->ConfigFile);
}

void slowdownthread(void*)
{
	//Sleep(30 * 1000);
	for (; ; )
	{
		if (Device.Statistic->fFPS < 30)
		{
			Sleep(1);
		}

		if (Device.mt_bMustExit)
		{
			return;
		}

		if (0 == pSettings)
		{
			return;
		}

		if (0 == Console)
		{
			return;
		}

		if (0 == pInput)
		{
			return;
		}

		if (0 == pApp)
		{
			return;
		}
	}
}

void CheckPrivilegySlowdown()
{
#ifdef DEBUG
	if (strstr(Core.Params, "-slowdown"))
	{
		thread_spawn(slowdownthread, "slowdown", 0, 0);
	}

	if (strstr(Core.Params, "-slowdown2x"))
	{
		thread_spawn(slowdownthread, "slowdown", 0, 0);
		thread_spawn(slowdownthread, "slowdown", 0, 0);
	}
#endif
}

void Startup()
{
	InitSound1();
	ExecuteUserScript();
	InitSound2();

	// Command line for auto start
	{
		LPCSTR pStartup = strstr(Core.Params, "-start ");
		if (pStartup)
		{
			Console->Execute(pStartup + 1);
		}
	}

	{
		LPCSTR pStartup = strstr(Core.Params, "-load ");
		if (pStartup)
		{
			Console->Execute(pStartup + 1);
		}
	}

	// Initialize APP
//#ifndef DEDICATED_SERVER
	ShowWindow(Device.m_hWnd, SW_SHOWNORMAL);
	Device.Create();
	//#endif
	LALib.OnCreate();
	pApp = xr_new<CApplication>();
	g_pGamePersistent = (IGame_Persistent*)NEW_INSTANCE(CLSID_GAME_PERSISTANT);
	g_SpatialSpace = xr_new<ISpatial_DB>();
	g_SpatialSpacePhysic = xr_new<ISpatial_DB>();

	// Destroy LOGO
	DestroyWindow(logoWindow);
	logoWindow = NULL;

	// Main cycle
	Memory.mem_usage();
	Device.Run();

	// Destroy APP
	xr_delete(g_SpatialSpacePhysic);
	xr_delete(g_SpatialSpace);
	DEL_INSTANCE(g_pGamePersistent);
	xr_delete(pApp);
	Engine.Event.Dump();

	DestroyInput();

	if (!g_bBenchmark && !g_SASH.IsRunning())
	{
		DestroySettings();
	}

	LALib.OnDestroy();

	if (!g_bBenchmark && !g_SASH.IsRunning())
	{
		DestroyConsole();
	}
	else
	{
		Console->Destroy();
	}

	DestroySound();
	DestroyEngine();
}

static BOOL CALLBACK logDlgProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_DESTROY:
		{
			break;
		}

		case WM_CLOSE:
		{
			DestroyWindow(hw);
			break;
		}

		case WM_COMMAND:
		{
			if (LOWORD(wp) == IDCANCEL)
			{
				DestroyWindow(hw);
			}

			break;
		}

		default:
		{
			return FALSE;
		}
	}

	return TRUE;
}


#include "xr_ioc_cmd.h"


int APIENTRY WinMain_impl(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
#ifdef DEDICATED_SERVER
	Debug._initialize(true);
#else
	Debug._initialize(false);
#endif

	if (!IsDebuggerPresent())
	{

		HMODULE const kernel32 = LoadLibrary("kernel32.dll");
		R_ASSERT(kernel32);

		typedef BOOL(__stdcall* HeapSetInformation_type) (HANDLE, HEAP_INFORMATION_CLASS, PVOID, SIZE_T);
		HeapSetInformation_type const heap_set_information =
			(HeapSetInformation_type)GetProcAddress(kernel32, "HeapSetInformation");
		if (heap_set_information)
		{
			ULONG HeapFragValue = 2;
		#ifdef DEBUG
			BOOL const result =
		#endif
				heap_set_information(
					GetProcessHeap(),
					HeapCompatibilityInformation,
					&HeapFragValue,
					sizeof(HeapFragValue));
			VERIFY2(result, "can't set process heap low fragmentation");
		}
	}

#ifndef DEDICATED_SERVER
	// Check for another instance
#ifdef NO_MULTI_INSTANCES
	HANDLE hCheckPresenceMutex = INVALID_HANDLE_VALUE;
	hCheckPresenceMutex = OpenMutex(READ_CONTROL, FALSE, "InnerRaysEngine-S.T.A.L.K.E.R.");
	if (hCheckPresenceMutex == NULL)
	{
		// New mutex
		hCheckPresenceMutex = CreateMutex(NULL, FALSE, "InnerRaysEngine-S.T.A.L.K.E.R.");
		if (hCheckPresenceMutex == NULL)
		{
			// Shit happens
			return 2;
		}
	}
	else
	{
		// Already running
		CloseHandle(hCheckPresenceMutex);
		return 1;
	}
#endif
#else
	g_dedicated_server = true;
#endif

	SetThreadAffinityMask(GetCurrentThread(), 1);

	// Title window
	logoWindow = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_STARTUP), 0, logDlgProc);

	HWND logoPicture = GetDlgItem(logoWindow, IDC_STATIC_LOGO);
	RECT logoRect;
	GetWindowRect(logoPicture, &logoRect);

	SetWindowPos(
		logoWindow,
	#ifndef DEBUG
		HWND_TOPMOST,
	#else
		HWND_NOTOPMOST,
	#endif
		0, 0, logoRect.right - logoRect.left, logoRect.bottom - logoRect.top, SWP_NOMOVE | SWP_SHOWWINDOW /*| SWP_NOSIZE*/);
	UpdateWindow(logoWindow);

	// AVI
	g_bIntroFinished = TRUE;
	g_sLaunchOnExit_app[0] = NULL;
	g_sLaunchOnExit_params[0] = NULL;

	LPCSTR fsgame_ltx_name = "-fsltx ";
	string_path fsgame = "";
	if (strstr(lpCmdLine, fsgame_ltx_name))
	{
		int sz = xr_strlen(fsgame_ltx_name);
		sscanf(strstr(lpCmdLine, fsgame_ltx_name) + sz, "%[^ ] ", fsgame);
	}

	compute_build_id();
	Core._initialize("xray", NULL, TRUE, fsgame[0] ? fsgame : NULL);
	InitSettings();

	// Adjust player & computer name for Asian
	if (pSettings->line_exist("string_table", "no_native_input"))
	{
		strcpy_s(Core.UserName, sizeof(Core.UserName), "Player");
		strcpy_s(Core.CompName, sizeof(Core.CompName), "Computer");
	}

	#ifndef DEDICATED_SERVER
	{
	#endif

		FPU::m24r();
		InitEngine();
		InitInput();
		InitConsole();

		Engine.External.CreateRendererList();

		LPCSTR benchName = "-batch_benchmark ";
		if (strstr(lpCmdLine, benchName))
		{
			int sz = xr_strlen(benchName);
			string64 b_name;
			sscanf(strstr(Core.Params, benchName) + sz, "%[^ ] ", b_name);
			doBenchmark(b_name);
			return 0;
		}

		Msg("command line %s", lpCmdLine);
		LPCSTR sashName = "-openautomate ";
		if (strstr(lpCmdLine, sashName))
		{
			int sz = xr_strlen(sashName);
			string512 sash_arg;
			sscanf(strstr(Core.Params, sashName) + sz, "%[^ ] ", sash_arg);
			//doBenchmark(sash_arg);
			g_SASH.Init(sash_arg);
			g_SASH.MainLoop();
			return 0;
		}

	#ifndef DEDICATED_SERVER
		if (strstr(Core.Params, "-r2a"))
		{
			Console->Execute("renderer renderer_r2a");
		}
		else
		{
			if (strstr(Core.Params, "-r2"))
			{
				Console->Execute("renderer renderer_r2");
			}
			else
			{
				CCC_LoadCFG_custom* pTmp = xr_new<CCC_LoadCFG_custom>("renderer ");
				pTmp->Execute(Console->ConfigFile);
				xr_delete(pTmp);
			}
		}
	#else
		Console->Execute("renderer renderer_r1");
	#endif
		//InitInput();
		Engine.External.Initialize();
		Console->Execute("stat_memory");

		Startup();
		Core._destroy();

		// Check for need to execute something external
		if (xr_strlen(g_sLaunchOnExit_app))
		{
			// CreateProcess needs to return results to next two structures
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));
			// We use CreateProcess to setup working folder
			char const* temp_wf = (xr_strlen(g_sLaunchWorkingFolder) > 0) ? g_sLaunchWorkingFolder : NULL;
			CreateProcess(g_sLaunchOnExit_app, g_sLaunchOnExit_params, NULL, NULL, FALSE, 0, NULL, temp_wf, &si, &pi);

		}
	#ifndef DEDICATED_SERVER

	#ifdef NO_MULTI_INSTANCES
		// Delete application presence mutex
		CloseHandle(hCheckPresenceMutex);
	#endif
	}
#endif

	return 0;
}

int stack_overflow_exception_filter(int exception_code)
{
	if (exception_code == EXCEPTION_STACK_OVERFLOW)
	{
		// Do not call _resetstkoflw here, because at this point, the stack is not yet unwound. Instead, signal that the handler (the __except block) is to be executed
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
	__try
	{
		WinMain_impl(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	}
	__except (stack_overflow_exception_filter(GetExceptionCode()))
	{
		_resetstkoflw();
		FATAL("stack overflow");
	}

	return 0;
}

void InitLauncher()
{
	if (hLauncher)
	{
		return;
	}

	hLauncher = LoadLibrary("xrLauncher.dll");
	if (0 == hLauncher)
	{
		R_CHK(GetLastError());
	}

	R_ASSERT2(hLauncher, "xrLauncher DLL raised exception during loading or there is no xrLauncher.dll at all");

	pLauncher = (LauncherFunc*)GetProcAddress(hLauncher, "RunXRLauncher");
	R_ASSERT2(pLauncher, "Cannot obtain RunXRLauncher function from xrLauncher.dll");
}

void FreeLauncher()
{
	if (hLauncher)
	{
		FreeLibrary(hLauncher);
		hLauncher = NULL;
		pLauncher = NULL;
	}
}

void doBenchmark(LPCSTR name)
{
	g_bBenchmark = true;
	string_path in_file;
	FS.update_path(in_file, "$app_data_root$", name);
	CInifile ini(in_file);
	int test_count = ini.line_count("benchmark");
	LPCSTR test_name, t;
	shared_str test_command;
	for (int i = 0; i < test_count; ++i)
	{
		ini.r_line("benchmark", i, &test_name, &t);
		strcpy_s(g_sBenchmarkName, test_name);

		test_command = ini.r_string_wb("benchmark", test_name);
		strcpy_s(Core.Params, *test_command);
		_strlwr_s(Core.Params);

		InitInput();
		if (i)
		{
			//ZeroMemory(&HW,sizeof(CHW));
			
			/*[
				TODO: KILL HW here!
			 ]*/

			//pApp->m_pRender->KillHW();
			InitEngine();
		}

		Engine.External.Initialize();

		strcpy_s(Console->ConfigFile, "user.ltx");
		if (strstr(Core.Params, "-ltx "))
		{
			string64 c_name;
			sscanf(strstr(Core.Params, "-ltx ") + 5, "%[^ ] ", c_name);
			strcpy_s(Console->ConfigFile, c_name);
		}

		Startup();
	}
}
