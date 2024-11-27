#include <Windows.h> 

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("The Standard Window"); 
    static TCHAR szWindowTitle[] = TEXT("Scroll Bar Demo"); 

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
    wnd.style = CS_HREDRAW | CS_VREDRAW; 

    if(!RegisterClassEx(&wnd))
    {
        MessageBox(
            (HWND)NULL, 
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
        WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | 
        WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | 
        WS_HSCROLL | WS_VSCROLL, 
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
            TEXT("Failed to create an application window"), 
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
    #define NR_MAX_CHARS    48 
    #define NR_ROWS         64 
    #define NR_COLUMNS      8 

    static int cxScreen, cyScreen; 
    static int cxChar, cyChar; 
    static int iHScrollPosition, iVScrollPosition; 
    static TCHAR szFormatMessage[] = TEXT("row number = %d, column number = %d"); 
    static TCHAR szOutputMessage[NR_MAX_CHARS]; 

    int iLength, iRowCount, iColumnCount; 
    int iPaintBeginVertical, iPaintEndVertical; 
   
    HDC hdc = NULL; 

    PAINTSTRUCT ps; 
    TEXTMETRIC tm; 
    SCROLLINFO si; 

    switch(uMsg)
    {
        case WM_CREATE:
            hdc = GetDC(hwnd); 
            GetTextMetrics(hdc, &tm); 
            ReleaseDC(hwnd, hdc);  
            cxChar = tm.tmAveCharWidth; 
            cyChar = tm.tmHeight + tm.tmExternalLeading; 

            break; 

        case WM_SIZE: 
            cxScreen = LOWORD(lParam); 
            cyScreen = HIWORD(lParam); 

            si.cbSize = sizeof(si); 
            si.fMask = SIF_RANGE | SIF_PAGE; 
            si.nMin = 0; 
            si.nMax = NR_ROWS-1; 
            si.nPage = cyScreen / cyChar; 
            SetScrollInfo(hwnd, SB_VERT, &si, TRUE); 

            si.cbSize = sizeof(si); 
            si.fMask = SIF_RANGE | SIF_PAGE; 
            si.nMin = 0; 
            si.nMax = (NR_MAX_CHARS * NR_COLUMNS) - (cxScreen/cxChar); 
            si.nPage = cxScreen / (NR_MAX_CHARS * cxChar); 
            SetScrollInfo(hwnd, SB_HORZ, &si, TRUE); 

            break; 

        case WM_PAINT: 
            hdc = BeginPaint(hwnd, &ps); 

            si.cbSize = sizeof(si); 
            si.fMask = SIF_POS; 
            GetScrollInfo(hwnd, SB_VERT, &si);
            iVScrollPosition = si.nPos; 
            GetScrollInfo(hwnd, SB_HORZ, &si); 
            iHScrollPosition = si.nPos; 

            iPaintBeginVertical = max(
                                        0, 
                                        iVScrollPosition + ps.rcPaint.top / cyChar
                                    ); 
            iPaintEndVertical = min(
                                    NR_ROWS-1, 
                                    iVScrollPosition + ps.rcPaint.bottom / cyChar
                                );

            for(iRowCount = iPaintBeginVertical; 
                iRowCount <= iPaintEndVertical;
                ++iRowCount
            )
            {
                for(iColumnCount = 0; 
                    iColumnCount < NR_COLUMNS; 
                    ++iColumnCount 
                )
                {
                    iLength = wsprintf(szOutputMessage, szFormatMessage, iRowCount, iColumnCount); 
                    TextOut(hdc, 
                            (NR_MAX_CHARS * iColumnCount - iHScrollPosition) * cxChar, 
                            (iRowCount - iVScrollPosition) * cyChar, 
                            szOutputMessage, 
                            iLength
                        ); 
                }
            }

      
            EndPaint(hwnd, &ps);
            break;  

        case WM_HSCROLL:

            si.fMask = SIF_ALL; 
            GetScrollInfo(hwnd, SB_HORZ, &si); 
            iHScrollPosition = si.nPos; 

            switch(LOWORD(wParam))
            {
                case SB_LINELEFT: 
                    si.nPos -= 1; 
                    break; 

                case SB_PAGELEFT: 
                    si.nPos -= si.nPage; 
                    break; 

                case SB_LINERIGHT: 
                    si.nPos += 1; 
                    break; 

                case SB_PAGERIGHT: 
                    si.nPos += si.nPage; 
                    break; 

                case SB_THUMBPOSITION: 
                    si.nPos = si.nTrackPos; 
                    break; 
            } 

            si.fMask = SIF_POS; 
            SetScrollInfo(hwnd, SB_HORZ, &si, TRUE); 
            GetScrollInfo(hwnd, SB_HORZ, &si); 

            if(iHScrollPosition != si.nPos)
            {
                ScrollWindow(hwnd, (iHScrollPosition - si.nPos) * cxChar, 0, NULL, NULL); 
            }

            break; 

        case WM_VSCROLL: 
            
            si.cbSize = sizeof(si); 
            si.fMask = SIF_ALL; 
            GetScrollInfo(hwnd, SB_VERT, &si); 
            iVScrollPosition = si.nPos; 

            switch(LOWORD(wParam))
            {
                case SB_LINEUP: 
                    si.nPos -= 1; 
                    break; 

                case SB_PAGEUP: 
                    si.nPos -= si.nPage; 
                    break; 

                case SB_LINEDOWN: 
                    si.nPos += 1; 
                    break; 

                case SB_PAGEDOWN: 
                    si.nPos += si.nPage; 
                    break; 

                case SB_TOP: 
                    si.nPos = si.nMin; 
                    break; 

                case SB_BOTTOM: 
                    si.nPos = si.nMax; 
                    break; 

                case SB_THUMBPOSITION: 
                    si.nPos = si.nTrackPos; 
                    break; 
            }

            si.fMask = SIF_POS; 
            SetScrollInfo(hwnd, SB_VERT, &si, TRUE); 
            GetScrollInfo(hwnd, SB_VERT, &si); 

            if(si.nPos != iVScrollPosition)
            {
                ScrollWindow(hwnd, 0, (iVScrollPosition - si.nPos) * cyChar, NULL, NULL); 
                UpdateWindow(hwnd); 
            }
       
            break;    

        case WM_DESTROY: 
            PostQuitMessage(EXIT_SUCCESS); 
            break; 
    }

    return (DefWindowProc(hwnd, uMsg, wParam, lParam)); 
}