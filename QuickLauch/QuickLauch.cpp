// QuickLauch.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "QuickLauch.h"

vector<WinApp*> g_AppsList;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    MainDialogProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	MainSubclass(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
INT_PTR CALLBACK    ScanDialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DeleteDialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    StatDialogProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_QUICKLAUCH, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUICKLAUCH));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
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
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKLAUCH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_QUICKLAUCH));

    return RegisterClassExW(&wcex);
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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, SW_SHOW);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_SYSCOMMAND, OnSysCommand);
		case WM_TRAYICON:
			{
				switch (lParam)
				{
				case WM_LBUTTONDBLCLK:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_MAINDIALOG), hWnd, MainDialogProc);
					break;
				case WM_RBUTTONDOWN:
					{
						HMENU hPopupMenu = CreatePopupMenu();
						InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, IDM_SCAN_EDIT, _T("Scan to build database"));
						InsertMenu(hPopupMenu, 1, MF_BYPOSITION | MF_STRING, IDM_STATS, _T("View statitistics"));
						InsertMenu(hPopupMenu, 2, MF_BYPOSITION | MF_STRING, IDM_EXIT, _T("Exit"));
						SetForegroundWindow(hWnd);
						POINT pCursor;
						GetCursorPos(&pCursor);
						TrackPopupMenu(hPopupMenu, TPM_LEFTBUTTON | TPM_RIGHTALIGN, pCursor.x, pCursor.y, 0, hWnd, NULL);
					}
					break;
				}
			}
			break;

		case WM_HOTKEY:
			if (wParam == IDC_HOTKEY)
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_MAINDIALOG), hWnd, MainDialogProc);
			}
			break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	g_niData.cbSize = sizeof(NOTIFYICONDATA);
	g_niData.hWnd = hWnd;
	g_niData.uID = IDI_QUICKLAUCH;
	g_niData.uVersion = NOTIFYICON_VERSION;
	g_niData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	g_niData.uCallbackMessage = WM_TRAYICON;
	g_niData.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_QUICKLAUCH));
	wcscpy_s(g_niData.szTip, L"QuickLaunch");
	Shell_NotifyIcon(NIM_ADD, &g_niData);
	// Ctrl + H
	RegisterHotKey(hWnd, IDC_HOTKEY, MOD_CONTROL | MOD_NOREPEAT, 0x48);
	ReadFile(hWnd);
	return true;
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDM_SCAN_EDIT:
		{
			HMENU hPopupMenu = CreatePopupMenu();
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, IDM_SCAN, _T("Scan"));
			InsertMenu(hPopupMenu, 1, MF_BYPOSITION | MF_STRING, IDM_EDIT, _T("Edit"));
			SetForegroundWindow(hWnd);
			POINT pCursor;
			GetCursorPos(&pCursor);
			TrackPopupMenu(hPopupMenu, TPM_LEFTBUTTON | TPM_RIGHTALIGN, pCursor.x, pCursor.y, 0, hWnd, NULL);
		}
		break;

	case IDM_SCAN:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_SCANDIALOG), hWnd, ScanDialogProc);
		break;

	case IDM_EDIT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DELETEDIALOG), hWnd, DeleteDialogProc);
		break;

	case IDM_STATS:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_STATDIALOG), hWnd, StatDialogProc);
		break;

	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	}
}

void OnSysCommand(HWND hwnd, UINT cmd, int x, int y)
{
	switch (cmd)
	{
	case SC_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		break;
	}
}

void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);


	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hWnd)
{	
	Shell_NotifyIcon(NIM_DELETE, &g_niData);
	UnregisterHotKey(hWnd, IDC_HOTKEY);
	WriteFile();
	PostQuitMessage(0);
}

INT_PTR CALLBACK DeleteDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hList = GetDlgItem(hDlg, IDC_LIST);
	HWND hBtnDel = GetDlgItem(hDlg, IDC_BTN_DELETE);

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		for (int i = 0; i < g_AppsList.size(); i++)
			ListBox_AddString(hList, g_AppsList[i]->AppName);
	}
	return (INT_PTR)TRUE;

	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN_DELETE:
			{
				int result = MessageBox(hDlg, L"Are you sure to delete selected apps from the list?",
					L"Confirm", MB_OKCANCEL);
				if (result == IDOK)
				{
					TCHAR *buffer;
					int *aSelItems, len;
					int size = ListBox_GetSelCount(hList);
					aSelItems = new int[size];
					ListBox_GetSelItems(hList, MAX_FILES, aSelItems);

					for (int i = 0; i < size; i++)
					{
						len = ListBox_GetTextLen(hList, aSelItems[i]);
						buffer = new TCHAR[len + 1];
						ListBox_GetText(hList, aSelItems[i], buffer);
						for (vector<WinApp*>::iterator it = g_AppsList.begin(); it != g_AppsList.end(); it++)
						{
							if (!StrCmp(buffer, (*it)->AppName))
							{
								g_AppsList.erase(it);
								break;
							}
						}
						delete buffer;
					}
					delete aSelItems;

					ListBox_ResetContent(hList);
					for (int i = 0; i < g_AppsList.size(); i++)
						ListBox_AddString(hList, g_AppsList[i]->AppName);
				}
			}
			break;

		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ScanDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hEdit = GetDlgItem(hDlg, IDC_EDIT1);
	HWND hBtn_Browse = GetDlgItem(hDlg, IDC_BTN_BROWSE);
	HWND hBtn_Scan = GetDlgItem(hDlg, IDC_BTN_SCAN);
	HWND hProgress = GetDlgItem(hDlg, IDC_PROGRESSBAR);
	int CurrentStep;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{
			SendMessage(hProgress, PBM_SETSTEP, (WPARAM)100, 0);
		}
		return (INT_PTR)TRUE;

	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			if (g_AppsList.size() == 0)
			{
				MessageBox(hDlg, L"You need to scan for apps to run this program!", 0, MB_OK);
			}
			else EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			if (g_AppsList.size() == 0)
			{
				MessageBox(hDlg, L"You need to scan for apps to run this program!", 0, MB_OK);
			}
			else EndDialog(hDlg, LOWORD(wParam));
			break;
		case IDC_RADIO1:
			{
				SendMessage(hProgress, PBM_SETPOS, (WPARAM)0, 0);
				Edit_Enable(hEdit, FALSE);
				Button_Enable(hBtn_Browse, FALSE);
				Button_Enable(hBtn_Scan, TRUE);
			}
			break;
		case IDC_RADIO2:
			{
				SendMessage(hProgress, PBM_SETPOS, (WPARAM)0, 0);
				Edit_Enable(hEdit, TRUE);
				Button_Enable(hBtn_Browse, TRUE);
				Button_Enable(hBtn_Scan, FALSE);
			}
			break;
		case IDC_BTN_BROWSE:
			SendMessage(hProgress, PBM_SETPOS, (WPARAM)0, 0);
			BrowseFolder(hEdit);
			Button_Enable(hBtn_Scan, TRUE);
			break;
		case IDC_BTN_SCAN:
			{
				if (IsDlgButtonChecked(hDlg, IDC_RADIO1))
					BuildDatabase(hDlg, _T("C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs"));
				else if (IsDlgButtonChecked(hDlg, IDC_RADIO2))
				{
					TCHAR *SelFolder;
					int size = GetWindowTextLength(hEdit);
					SelFolder = new TCHAR[size + 1];
					GetWindowText(hEdit, SelFolder, size + 1);
					if (!StrCmp(SelFolder, L""))
					{
						MessageBox(0, L"Please select a folder.", L"Error", MB_OK);
						Button_Enable(hBtn_Scan, FALSE);
						break;
					}
					else
					{
						if (StrStrI(SelFolder, L"C:\\") != NULL)
							BuildDatabase(hDlg, _T("C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs"));
						else BuildDatabase(hDlg, SelFolder);
					}
					delete SelFolder;
				}
				SendMessage(hProgress, PBM_STEPIT, 0, 0);
			}
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK StatDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{
			if (g_AppsList[0]->Priority == 0)
			{
				MessageBox(hDlg, L"No program has been run yet!", L"Error", MB_OK);
				EndDialog(hDlg, LOWORD(wParam));
			}
			else GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		}
		return (INT_PTR)TRUE;

	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			GdiplusShutdown(gdiplusToken);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hDlg, &ps);
			TCHAR temp[MAX_NAME_LEN];

			vector<Color> ListColor;
			ListColor.push_back(Color::Blue);
			ListColor.push_back(Color::Red);
			ListColor.push_back(Color::Purple);
			ListColor.push_back(Color::Green);
			ListColor.push_back(Color::Yellow);
			ListColor.push_back(Color::Aquamarine);
			RECT WinRect;
			GetClientRect(hDlg, &WinRect);
			REAL average, sweepAngle, startAngle = -90.f;

			Graphics *graph = new Graphics(hdc);
			SolidBrush Sbrush(Color::Black);

			FontFamily  fontFamily(L"Segoe UI");
			Font        font(&fontFamily, 20, FontStyleBold, UnitPixel);

			RectF PieRect(20, 150, 300, 300);
			RectF ArcRect(320, 20, 100, 100);

			PointF textpoint(440, 30);

			int Total = 0, Others = 0;
			for (int i = 0; i < g_AppsList.size(); i++)
			{
				Total += g_AppsList[i]->Priority;
				if (i > 4)
					Others += g_AppsList[i]->Priority;
			}

			for (int i = 0; i < 5; i++)
			{
				Sbrush.SetColor(ListColor[i]);
				average = (REAL)g_AppsList[i]->Priority / (REAL)Total;
				sweepAngle = average * 360;
				graph->FillPie(&Sbrush, PieRect, startAngle, sweepAngle);
				startAngle += sweepAngle;

				graph->FillPie(&Sbrush, ArcRect, -40.f, 40.f);

				Sbrush.SetColor(Color::Black);
				graph->DrawString(g_AppsList[i]->AppName, -1, &font, textpoint, &Sbrush);
				ArcRect.Y += 100;
				textpoint.Y += 20;

				StrCpy(temp, Int2Wstring(g_AppsList[i]->Priority).c_str());
				StrCat(temp, L" time(s)");
				graph->DrawString(temp, -1, &font, textpoint, &Sbrush);
				textpoint.Y += 80;
			}

			Sbrush.SetColor(ListColor[5]);
			average = (REAL)Others / (REAL)Total;
			sweepAngle = average * 360;
			graph->FillPie(&Sbrush, PieRect, startAngle, sweepAngle);

			graph->FillPie(&Sbrush, ArcRect, -40.f, 40.f);

			Sbrush.SetColor(Color::Black);
			graph->DrawString(L"Others", -1, &font, textpoint, &Sbrush);
			textpoint.Y += 20;

			StrCpy(temp, Int2Wstring(Others).c_str());
			StrCat(temp, L" time(s)");
			graph->DrawString(temp, -1, &font, textpoint, &Sbrush);

			delete graph;
			EndPaint(hDlg, &ps);
		}
		break;
	}

	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hTxtBox = GetDlgItem(hDlg, IDC_TEXTBOX);
	HWND hListBox = GetDlgItem(hDlg, IDC_LISTBOX);
	HWND hButton = GetDlgItem(hDlg, IDC_BTN_RUN);
	HWND hImage = GetDlgItem(hDlg, IDC_ICONSHOW);
	HICON hIcon;
	int index = 0;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{

		}
		return (INT_PTR)TRUE;

	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_TEXTBOX:
			{
				if (HIWORD(wParam) == EN_CHANGE)
				{
					ListBox_ResetContent(hListBox);
					TCHAR *buffer;
					int len = GetWindowTextLength(hTxtBox);
					buffer = new TCHAR[len + 1];
					GetWindowText(hTxtBox, buffer, MAX_PATH_LEN);
					for (int i = 0; i < g_AppsList.size(); i++)
					{
						TCHAR AltName[MAX_NAME_LEN];
						StrCpy(AltName, PathFindFileName(g_AppsList[i]->AppPath));
						if (StrStrI(g_AppsList[i]->AppName, buffer) != NULL || StrStrI(AltName, buffer) != NULL)
							ListBox_AddString(hListBox, g_AppsList[i]->AppName);
					}
					delete buffer;
				}
			}
			break;
		case IDC_LISTBOX:
			SetWindowSubclass(hListBox, MainSubclass, ID_SUBSCLASS, NULL);
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				index = ListBox_GetCurSel(hListBox);
				ShowAppIcon(hDlg, hListBox, hIcon, index);
			}
			break;
		case IDC_BTN_RUN:
			{
				index = ListBox_GetCurSel(hListBox);
				int len = ListBox_GetTextLen(hListBox, index);
				TCHAR *buffer = new TCHAR[len + 1];
				ListBox_GetText(hListBox, index, buffer);
				for (int i = 0; i < g_AppsList.size(); i++)
				{
					if (!StrCmp(g_AppsList[i]->AppName, buffer))
					{
						g_AppsList[i]->Priority++;
						ShellExecute(hDlg, L"open", g_AppsList[i]->AppPath, NULL, NULL, SW_SHOW);
						break;
					}
				}
				delete buffer;
				QuickSort(g_AppsList, 0, g_AppsList.size() - 1);
			}
			break;
		default:
			return DefWindowProc(hDlg, message, wParam, lParam);
		}
	}
	return (INT_PTR)FALSE;
}

LRESULT CALLBACK MainSubclass(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	HICON hIcon;
	int index;
	HWND hParent = GetParent(hDlg);

	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			{
				index = ListBox_GetCurSel(hDlg);
				if (index > 0)
				{
					index--;
					ListBox_SetCurSel(hDlg, index);
				}
				ShowAppIcon(hParent, hDlg, hIcon, index);
				return TRUE;
			}

		case VK_DOWN:
			{
				int count = ListBox_GetCount(hDlg);
				index = ListBox_GetCurSel(hDlg);
				if (index < count - 1)
				{
					index++;
					ListBox_SetCurSel(hDlg, index);
				}
				ShowAppIcon(hParent, hDlg, hIcon, index);
				return TRUE;
			}
		}
		break;

	}

	return DefSubclassProc(hDlg, message, wParam, lParam);
}

void BuildDatabase(HWND hWnd, TCHAR* path)
{
	TCHAR buffer[MAX_PATH_LEN];
	StrCpy(buffer, path);		//Copy the path of item (include drive letter path)
	StrCat(buffer, _T("\\*"));	//Add to find all item in directory

	WIN32_FIND_DATA ffd; //Contains information about the file that is found by Find first file and Find next file
	HANDLE hFind = FindFirstFileW(buffer, &ffd);

	//If the function fails or fails to locate files from the search string
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	TCHAR* folderPath;

	do
	{
		folderPath = new TCHAR[wcslen(path) + wcslen(ffd.cFileName) + 2];

		//Set path
		StrCpy(folderPath, path);
		if (wcslen(path) != 3)
			StrCat(folderPath, _T("\\"));
		StrCat(folderPath, ffd.cFileName);

		DWORD fileAttribute = ffd.dwFileAttributes;
		if ((fileAttribute & FILE_ATTRIBUTE_DIRECTORY)) //Get only directory and folder
		{
			if ((_tcscmp(ffd.cFileName, _T(".")) != 0) && (_tcscmp(ffd.cFileName, _T("..")) != 0)) //Ignore . (curr dir) and .. (parent dir))
				BuildDatabase(hWnd, folderPath);
		}
		else if (((ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != FILE_ATTRIBUTE_SYSTEM))
		{
			
			if (!StrCmpI(PathFindExtension(folderPath), _T(".lnk")))
			{
				TCHAR shortcutPath[MAX_PATH_LEN];
				if (ResolveIt(hWnd, folderPath, shortcutPath, MAX_PATH_LEN) == S_OK)
					StrCpy(folderPath, shortcutPath);
			}
			if (!StrCmpI(PathFindExtension(folderPath), _T(".exe")))
			{
				WinApp* winApp = new WinApp;
				winApp->AppName = new TCHAR[wcslen(ffd.cFileName) - 2];
				winApp->AppPath = new TCHAR[wcslen(folderPath) + 1];
				StrNCpy(winApp->AppName, ffd.cFileName, wcslen(ffd.cFileName) - 3);
				StrCpy(winApp->AppPath, folderPath);
				if (isDuplicate(winApp))
					delete winApp;
				else g_AppsList.push_back(winApp);				
			}
		}
	} while (FindNextFileW(hFind, &ffd));

	FindClose(hFind);
}

wstring Int2Wstring(int value)
{
	wostringstream wss;
	wss << value;
	return wss.str();
}

int Wstring2Int(wstring ws)
{
	int value = 0;
	for (int i = 0; i < ws.length(); i++)
	{
		value *= 10;
		value += ws[i] - '0';
	}
	return value;
}

BOOL isDuplicate(WinApp* thisapp)
{
	for (int i = 0; i < g_AppsList.size(); i++)
	{
		if (!StrCmp(thisapp->AppName, g_AppsList[i]->AppName) && !StrCmp(thisapp->AppPath, g_AppsList[i]->AppPath))
			return TRUE;
	}
	return FALSE;
}

void QuickSort(vector<WinApp*> &AppsList, int left, int right)
{
	if (left < right)
	{
		int pivot = AppsList[(left + right) / 2]->Priority;

		int i = left, j = right;

		while (i <= j)
		{
			while (AppsList[i]->Priority > pivot)
				i++;
			while (AppsList[j]->Priority < pivot)
				j--;

			if (i <= j)
			{
				WinApp *tmp;
				tmp = AppsList[i];
				AppsList[i] = AppsList[j];
				AppsList[j] = tmp;
				i++;
				j--;
			}
		}

		if (left < j)
			QuickSort(AppsList, left, j);
		if (right > i)
			QuickSort(AppsList, i, right);
	}
}

void ShowAppIcon(HWND &hDlg, HWND &hListBox, HICON &hIcon, int iSelItem)
{
	int len = ListBox_GetTextLen(hListBox, iSelItem);
	TCHAR *buffer = new TCHAR[len + 1];
	ListBox_GetText(hListBox, iSelItem, buffer);
	for (int i = 0; i < g_AppsList.size(); i++)
	{
		if (!StrCmp(g_AppsList[i]->AppName, buffer))
		{
			hIcon = ExtractIcon(hInst, g_AppsList[i]->AppPath, 0);
			if (hIcon != NULL)
				SendDlgItemMessage(hDlg, IDC_ICONSHOW, STM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);
			break;
		}
	}
	delete buffer;
}

void WriteFile()
{
	if (g_AppsList.size() == 0)
		return;
	TCHAR path[MAX_PATH_LEN];
	GetModuleFileName(NULL, path, MAX_PATH_LEN);
	StrNCpy(path, path, wcslen(path) - 13);
	StrCat(path, _T("\\Database.txt"));

	wofstream fout(path, ios::binary);
	fout.imbue(locale(fout.getloc(), new std::codecvt_utf8_utf16<wchar_t>));
	int size = g_AppsList.size();
	for (int i = 0; i < size; i++)
	{
		fout << g_AppsList[i]->AppName;
		fout << _T(",");
		fout << g_AppsList[i]->AppPath;
		fout << _T(",");
		fout << g_AppsList[i]->Priority;
		fout << _T(".\n");
	}
	g_AppsList.clear();

	fout.close();
}

void ReadFile(HWND hWnd)
{
	TCHAR datapath[MAX_PATH_LEN];
	GetModuleFileName(NULL, datapath, MAX_PATH_LEN);
	StrNCpy(datapath, datapath, wcslen(datapath) - 13);
	StrCat(datapath, _T("\\Database.txt"));

	wifstream fin(datapath, ios::binary);
	fin.imbue(locale(fin.getloc(), new std::codecvt_utf8_utf16<wchar_t>));

	if (!fin.is_open())
	{
		int result = MessageBox(0, _T("Seems like this is our first meet. We need to scan your computer for installed apps.")
			, L"Hello", MB_OK);
		if (result == IDOK)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SCANDIALOG), hWnd, ScanDialogProc);
		}
		return;
	}
	
	wstring buffer, name, path, prior;
	int i;
	while (getline(fin, buffer))
	{
		i = 0;
		name = path = prior = L"";
		while (buffer[i] != L',')
		{
			name += buffer[i];
			i++;
		}
		i++;
		while (buffer[i] != L',')
		{
			path += buffer[i];
			i++;
		}
		i++;
		while (buffer[i] != L'.')
		{
			prior += buffer[i];
			i++;
		}
		WinApp *winApp = new WinApp;
		winApp->AppName = new TCHAR[name.size() + 1];
		StrCpy(winApp->AppName, name.c_str());
		winApp->AppPath = new TCHAR[path.size() + 1];
		StrCpy(winApp->AppPath, path.c_str());
		winApp->Priority = Wstring2Int(prior);

		g_AppsList.push_back(winApp);
	}

	fin.close();
}

HRESULT ResolveIt(HWND hwnd, TCHAR* lpszLinkFile, TCHAR* lpszPath, int iPathBufferSize)
{
	HRESULT hres;
	IShellLink* psl;
	TCHAR szGotPath[MAX_PATH_LEN];
	WIN32_FIND_DATA wfd;

	*lpszPath = 0; // Assume failure 

				   // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
				   // has already been called. 
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;

		// Get a pointer to the IPersistFile interface. 
		hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);

		if (SUCCEEDED(hres))
		{
			// Load the shortcut. 
			hres = ppf->Load(lpszLinkFile, STGM_READ);

			if (SUCCEEDED(hres))
			{
				// Resolve the link. 
				hres = psl->Resolve(hwnd, SLR_NO_UI);

				if (SUCCEEDED(hres))
				{
					// Get the path to the link target. 
					hres = psl->GetPath(szGotPath, MAX_PATH, (WIN32_FIND_DATA*)&wfd, SLGP_SHORTPATH);

					if (SUCCEEDED(hres))
					{
						hres = StringCbCopy(lpszPath, iPathBufferSize, szGotPath);
					}
				}
			}

			// Release the pointer to the IPersistFile interface. 
			ppf->Release();
		}

		// Release the pointer to the IShellLink interface. 
		psl->Release();
	}
	return hres;
}

void BrowseFolder(HWND hEdit)
{
	TCHAR path[MAX_PATH];
	BROWSEINFO bi = { 0 };
	//bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = _T("Choose a folder:");
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != 0)
	{
		// get the name of the folder and put it in path
		SHGetPathFromIDList(pidl, path);

		//Set the current directory to path
		SetCurrentDirectory(path);

		SetWindowText(hEdit, path);


		// free memory used
		IMalloc * imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}
	}
}//BROWSE FOLDER

