// EMF.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "EMF.h"
#include <commdlg.h>
//#include "EMF8.h"
//#include "EMF9.h"
//#include "EMF10.h"
//#include "EMF11.h"
//#include "EMF12.h"
#include "EMF13.h"

#define MAX_LOADSTRING 100

extern void CreateRoutine(HWND);
extern void PaintRoutine(HWND, HDC, int, int);

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EMF, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EMF));

    MSG msg;

    // メイン メッセージ ループ:
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

BOOL PrintRoutine(HWND hwnd)
{
	static DOCINFO di;
	static PRINTDLG printdlg = { sizeof(PRINTDLG) };
	static TCHAR szMessage[32];
	BOOL bSuccess = FALSE;
	HDC hdcPrn;
	int cxPage, cyPage;
	printdlg.Flags = PD_RETURNDC | PD_NOPAGENUMS | PD_NOSELECTION;
	if (!PrintDlg(&printdlg))
		return TRUE;
	if (NULL == (hdcPrn = printdlg.hDC))
		return FALSE;
	cxPage = GetDeviceCaps(hdcPrn, HORZRES);
	cyPage = GetDeviceCaps(hdcPrn, VERTRES);
	lstrcpy(szMessage, szWindowClass);
	lstrcat(szMessage, TEXT(": Printing"));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = szMessage;
	if (StartDoc(hdcPrn, &di) > 0)
	{
		if (StartPage(hdcPrn) > 0)
		{
			PaintRoutine(hwnd, hdcPrn, cxPage, cyPage);
			if (EndPage(hdcPrn) > 0)
			{
				EndDoc(hdcPrn);
				bSuccess = TRUE;
			}
		}
	}
	DeleteDC(hdcPrn);
	return bSuccess;
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EMF));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EMF);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL bSuccess;
	static int cxClient, cyClient;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_CREATE:
		CreateRoutine(hwnd);
		return 0;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDM_PRINT:
			SetCursor(LoadCursor(NULL, IDC_WAIT));
			ShowCursor(TRUE);

			bSuccess = PrintRoutine(hwnd);

			ShowCursor(FALSE);
			SetCursor(LoadCursor(NULL, IDC_ARROW));

			if (!bSuccess)
				MessageBox(hwnd, TEXT("Error encountered during printing"), szWindowClass, MB_ICONASTERISK | MB_OK);
			return 0;

		case IDM_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;

		case IDM_ABOUT:
			MessageBox(hwnd, TEXT("Enhanced Metafile Demo Program\n") TEXT("Copyright (c) Charles Petzold, 1998"), szWindowClass, MB_ICONINFORMATION | MB_OK);
			return 0;
		}
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		PaintRoutine(hwnd, hdc, cxClient, cyClient);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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
