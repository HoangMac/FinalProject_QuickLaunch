#pragma once

#include "resource.h"
#include <iostream>
#include <windowsx.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlobj.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "shobjidl.h"
#include "shlguid.h"
#include "strsafe.h"
#include <locale>
#include <codecvt>
#include <ObjIdl.h>
#include <gdiplus.h>

//Dùng để sử dụng hàm StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ComCtl32.lib")
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
using namespace std;

#define WM_TRAYICON (WM_USER + 1)
#define MAX_PATH_LEN 10240
#define MAX_NAME_LEN 256
#define MAX_FILES 10000

class WinApp
{
public:
	TCHAR* AppName;
	TCHAR* AppPath;
	int Priority;

	WinApp()
	{
		Priority = 0;
	}

	~WinApp()
	{
		delete AppName;
		delete AppPath;
	}
};

extern vector<WinApp*> g_AppsList;
NOTIFYICONDATA g_niData;
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;

void BuildDatabase(HWND hWnd, TCHAR* path);
wstring Int2Wstring(int value);
int Wstring2Int(wstring ws);
void QuickSort(vector<WinApp*> &AppsList, int left, int right);
void ShowAppIcon(HWND &hDlg, HWND &hListBox, HICON &hIcon, int iSelItem);
void WriteFile();
void ReadFile(HWND hWnd);
BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void OnSysCommand(HWND hwnd, UINT cmd, int x, int y);
void OnPaint(HWND hWnd);
void OnDestroy(HWND hWnd);
BOOL isDuplicate(WinApp* thisapp);
HRESULT ResolveIt(HWND hwnd, TCHAR* lpszLinkFile, TCHAR* lpszPath, int iPathBufferSize);
void BrowseFolder(HWND hEdit);

