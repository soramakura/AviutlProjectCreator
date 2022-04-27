#include <string>

#include <Windows.h>
#include "resource.h"

#include "MainWindow.h"

int APIENTRY wWinMain
( _In_     HINSTANCE hInstance
, _In_opt_ HINSTANCE //hPrevInstance
, _In_     LPWSTR    //lpCmdLine
, _In_     int       nCmdShow
)
{
    MainWindow window;
    DialogBoxParamW
    ( /*_In_opt_ HINSTANCE hInstance     */nullptr
    , /*_In_     LPCWSTR   lpTemplateName*/MAKEINTRESOURCEW(DIALOG1)
    , /*_In_opt_ HWND      hWndParent    */nullptr
    , /*_In_opt_ DLGPROC   lpDialogFunc  */MainWindow::dialogProc
    , /*_In_     LPARAM    dwInitParam   */reinterpret_cast<LPARAM>(&window)
    );

    return 0;
}