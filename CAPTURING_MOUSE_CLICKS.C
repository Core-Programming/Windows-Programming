#include <Windows.h> 

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("The Standard Window"); 
    static TCHAR szWindowTitle[] = TEXT("HelloWin"); 

    HWND hwnd = NULL; 

    WNDCLASSEX wnd; 
    MSG msg; 

    ZeroMemory(&wnd, sizeof(WNDCLASSEX)); 
    ZeroMemory(&msg, sizeof(MSG)); 

    wnd.cbSize = sizeof(WNDCLASSEX); 
    wnd.cbClsExtra = 0; 
    wnd.cbWndExtra = 0; 
    wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
    wnd.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW); 
    wnd.hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION); 
    wnd.hIconSm = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION); 
    wnd.hInstance = hInstance; 
    wnd.lpfnWndProc = WndProc;  
    wnd.lpszClassName = szClassName; 
    wnd.lpszMenuName = NULL; 
    wnd.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;  

    if(!RegisterClassEx(&wnd))
    {
        MessageBox(
            NULL, 
            TEXT("Failed to register a window class"), 
            TEXT("RegisterClassEx"), 
            MB_ICONERROR | MB_TOPMOST
        ); 
        ExitProcess(EXIT_FAILURE); 
    }

    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW, 
        szClassName, 
        szWindowTitle, 
        WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 
        CW_USEDEFAULT,
        CW_USEDEFAULT, 
        CW_USEDEFAULT, 
        CW_USEDEFAULT, 
        (HWND)NULL, 
        (HMENU)NULL, 
        hInstance, 
        (LPVOID)NULL
    ); 

    if(hwnd == NULL)
    {
        MessageBox(
            (HWND)NULL, 
            TEXT("Failed to create a window"), 
            TEXT("CreateWindowEx"), 
            MB_ICONERROR | MB_TOPMOST
        ); 
        ExitProcess(EXIT_FAILURE); 
    }

    ShowWindow(hwnd, nShowCmd); 
    UpdateWindow(hwnd); 

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }

    return ((int)msg.wParam); 
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static int cxClient, cyClient;  
    static int iLength; 

    PAINTSTRUCT ps;
    TEXTMETRIC tm; 
    HDC hdc; 

    static TCHAR szLeftDown[] = TEXT("LEFT CLICK:DOWN:x=%hu, y=%hu"); 
    static TCHAR szRightDown[] = TEXT("RIGHT CLICK:DOWN:x=%hu, y=%hu"); 
    static TCHAR szLeftUp[] = TEXT("LEFT CLICK:UP:x=%hu, y=%hu"); 
    static TCHAR szRightUp[] = TEXT("RIGHT CLICK:UP:x=%hu, y=%hu"); 
    static TCHAR szLeftDblClk[] = TEXT("LEFT CLICK:DOUBLE CLICK:x=%hu, y=%hu"); 
    static TCHAR szRightDblClk[] = TEXT("RIGHT CLICK:DOUBLE CLICK:x=%hu, y=%hu"); 
    static TCHAR szNCLeftDown[] = TEXT("LEFT CLICK:DOWN:NON-CLIENT AREA"); 
    static TCHAR szNCLeftUp[] = TEXT("LEFT CLICK:UP:NON-CLIENT AREA"); 
    static TCHAR szNCRightDown[] = TEXT("RIGHT CLICK:DOWN:ON NON-CLIENT AREA"); 
    static TCHAR szNCRightUp[] = TEXT("RIGHT CLICK UP NON-CLIENT AREA"); 
    static TCHAR szNCLeftDblClk[] = TEXT("LEFT DOUBLE CLICK ON NON-CLIENT AREA"); 
    static TCHAR szNCRightDblClk[] = TEXT("RIGHT DOUBLE CLICK ON NON-CLIENT AREA"); 

    static TCHAR szOutput[48]; 

    switch(uMsg)
    {
        case WM_LBUTTONDOWN: 
            ZeroMemory(szOutput, 48); 
            iLength = wsprintf(szOutput, szLeftDown, LOWORD(lParam), HIWORD(lParam)); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break; 

        case WM_RBUTTONDOWN: 
            ZeroMemory(szOutput, 48); 
            iLength = wsprintf(szOutput, szRightDown, LOWORD(lParam), HIWORD(lParam)); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break; 

        case WM_LBUTTONUP: 
            ZeroMemory(szOutput, 48); 
            iLength = wsprintf(szOutput, szLeftUp, LOWORD(lParam), HIWORD(lParam)); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break; 

        case WM_RBUTTONUP: 
            ZeroMemory(szOutput, 48); 
            iLength = wsprintf(szOutput, szRightUp, LOWORD(lParam), HIWORD(lParam)); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break; 

        case WM_LBUTTONDBLCLK: 
            ZeroMemory(szOutput, 48); 
            iLength = wsprintf(szOutput, szLeftDblClk, LOWORD(lParam), HIWORD(lParam)); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break; 

        case WM_RBUTTONDBLCLK: 
            ZeroMemory(szOutput, 48); 
            iLength = wsprintf(szOutput, szRightDblClk, LOWORD(lParam), HIWORD(lParam)); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break; 

        case WM_NCLBUTTONDOWN: 
            iLength = wsprintf(szOutput, szNCLeftDown); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break; 

        case WM_NCLBUTTONUP: 
            iLength = wsprintf(szOutput, szNCLeftUp); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break; 

        case WM_NCLBUTTONDBLCLK: 
            iLength = wsprintf(szOutput, szNCLeftDblClk); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break; 

        case WM_NCRBUTTONDOWN: 
            iLength = wsprintf(szOutput, szNCRightDown); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break; 

        case WM_NCRBUTTONUP:
            iLength = wsprintf(szOutput, szNCRightUp); 
            InvalidateRect(hwnd, NULL, TRUE);  
            break; 

        case WM_NCRBUTTONDBLCLK: 
            iLength = wsprintf(szOutput, szNCRightDblClk); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break; 
          
        case WM_SIZE:
            cxClient = LOWORD(lParam); 
            cyClient = HIWORD(lParam); 
            break; 

        case WM_PAINT: 
            hdc = BeginPaint(hwnd, &ps); 
            SetTextAlign(hdc, TA_TOP | TA_CENTER); 
            TextOut(hdc, cxClient/2, cyClient/2, szOutput, iLength); 
            EndPaint(hwnd, &ps); 
            break; 
        
        case WM_DESTROY: 
            PostQuitMessage(EXIT_SUCCESS); 
            break; 
    }   

    return (DefWindowProc(hwnd, uMsg, wParam, lParam)); 
}