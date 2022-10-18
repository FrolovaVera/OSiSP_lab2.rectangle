#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

#define ROW_COUNT 3
#define COLUMN_COUNT 6

LPCTSTR windowName = TEXT("OSiSP_lab_2");
string  massiveStr[ROW_COUNT * COLUMN_COUNT];
int     coorY[ROW_COUNT * COLUMN_COUNT][2];
int     coorX[ROW_COUNT * COLUMN_COUNT][2];

void    DeleviStr(HANDLE hFile);
void    DrawTable(HDC hDc, HWND hwnd);
void    WriteText(HDC hdc, RECT cells, int id, int lensells);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void    CoorRectangle(HDC hDc, int lenOfCells, int heigOfCells, RECT windowSize);

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR cmdLine, _In_ int showMode)
{
    MSG msg;

    WNDCLASSEX windowClassEx;
    windowClassEx.cbSize = sizeof(WNDCLASSEX);
    windowClassEx.style = CS_HREDRAW | CS_VREDRAW;
    windowClassEx.lpfnWndProc = WndProc;
    windowClassEx.cbClsExtra = 0;
    windowClassEx.cbWndExtra = 0;
    windowClassEx.hInstance = hInstance;
    windowClassEx.hIcon = LoadIcon(0, IDI_WINLOGO);;
    windowClassEx.hCursor = LoadCursor(0, IDC_ARROW);
    windowClassEx.hbrBackground = (HBRUSH)18;
    windowClassEx.lpszMenuName = 0;
    windowClassEx.lpszClassName = windowName;
    windowClassEx.hIconSm = 0;
    RegisterClassEx(&windowClassEx);
    
    HWND hWnd = CreateWindow(windowName,  // Указатель на зарегистрированное имя класса.
        windowName,                       // Указатель на имя окна.
        WS_OVERLAPPEDWINDOW,              // Стиль окна.
        CW_USEDEFAULT,                    // Горизонтальная позиция окна.
        CW_USEDEFAULT,                    // Вертикальная позиция окна.
        CW_USEDEFAULT,                    // Ширина окна.
        CW_USEDEFAULT,                    // Высота окна.
        NULL,                             // Дескриптор родительского или окна владельца.
        NULL,                             // Дескриптор меню или ID дочернего окна.
        hInstance,                        // Дескриптор экземпляра приложения.
        NULL);

    ShowWindow(hWnd, showMode);
    UpdateWindow(hWnd);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
      case WM_CREATE: 
      {
          const char FileName[] = "C:/Users/Vera/Desktop/3 курс/5 сем/осисп/OSiSP_lab2.rectangle/data.txt";
          HANDLE hFile = CreateFile(FileName, // имя файла
              GENERIC_READ,                   // режим доступа(ч/з/ч-з)
              0,                              // совместный доступ(ч/з/ч-з)
              NULL,                           // дескр. защиты
              OPEN_EXISTING,                  // как действовать(Открывает файл)
              FILE_ATTRIBUTE_NORMAL,          // атрибуты файла(У файла нет других установленных атрибутов)
              NULL);                          // дескриптор файла шаблона
              DeleviStr(hFile);             
              CloseHandle(hFile);
      }
      break;

      case (WM_PAINT):
      {
          RECT windowSize;
          GetClientRect(hWnd, &windowSize);

          PAINTSTRUCT ps;
          HDC hdc = BeginPaint(hWnd, &ps);

          HBRUSH brush = CreateSolidBrush(RGB(204, 153, 210));
          SelectObject(hdc, brush);
          Rectangle(hdc, windowSize.left, windowSize.top, windowSize.right, windowSize.bottom);
          DeleteObject(brush);

          SetBkMode(hdc, TRANSPARENT);
          SetTextColor(hdc, RGB(101, 82, 85));
          const auto pen = CreatePen(PS_SOLID, 1, RGB(101, 82, 85));
          SelectObject(hdc, pen);
          DrawTable(hdc, hWnd);
          DeleteObject(hdc);
          EndPaint(hWnd, &ps);
      }
      break;

      case WM_SIZE:
      {
          InvalidateRect(hWnd, NULL, FALSE);
      }
      break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

string ConcatenationStr(char cBufferText[255], const int num) {
    string str;
    for (int i = 0; i < num; i++) {
        str = str + cBufferText[i];
    }
    return str;
}

void DeleviStr(HANDLE hFile) 
{
    int i = 0, id = 0;
    int numRowColumn = ROW_COUNT * COLUMN_COUNT;
    int strLen = GetFileSize(hFile, NULL);
    int numOfCells =ceil(double(strLen) / double(numRowColumn));
    int num = strLen - ((strLen / numRowColumn) * numRowColumn);

    while (true)
    {
        for (int j = 0; j < num; j++) {

            char  cBufferText[255];
            DWORD dwByte;
            ReadFile(hFile, cBufferText, numOfCells, &dwByte, NULL);
            massiveStr[id] = ConcatenationStr(cBufferText,numOfCells); id = id + 1;
        }
        numRowColumn = numRowColumn - num;
        if (numRowColumn == 0)
            break;

        strLen = strLen - num * numOfCells;
        numOfCells = ceil(double(strLen) / double(numRowColumn)); 
        if (numOfCells == 0)
        {
            num = 1;
        }
        else 
        {
            num = strLen - (strLen / numRowColumn * numRowColumn);
        }
        if (num == 0)
            num = numRowColumn;
    }
}

void DrawTable(HDC hDc, HWND hwnd)
{
    RECT windowSize;
    GetClientRect(hwnd, &windowSize);

    int x = 0, y = 0;
    int lenOfCells = (windowSize.right - windowSize.left) / COLUMN_COUNT;
    int heigOfCells = (windowSize.bottom - windowSize.top) / ROW_COUNT;

    for (int i = 0; i < ROW_COUNT; i++) 
    {
        MoveToEx(hDc, windowSize.left,y, nullptr);
        LineTo(hDc, windowSize.right,y);
        y = y + heigOfCells;
    }

    for (int i = 0; i < COLUMN_COUNT; i++)
    {
        MoveToEx(hDc,x,windowSize.top, nullptr);
        LineTo(hDc,x,windowSize.bottom);
        x = x + lenOfCells;
    }

    CoorRectangle(hDc, lenOfCells, heigOfCells, windowSize);
}

void CoorRectangle(HDC hDc, int lenOfCells, int heigOfCells, RECT windowSize)
{
    int id = 0;

    RECT cells;
    for (int y = 0; y < windowSize.bottom - 1; y = y + heigOfCells) {
        for (int x = 0; x < windowSize.right - 1; x = x + lenOfCells) {
            cells.left = x;
            cells.top = y;
            cells.right = x + lenOfCells;
            cells.bottom = y + heigOfCells;
            WriteText(hDc, cells, id, lenOfCells);
            id = id + 1;
        }
    }
}

void WriteText(HDC hdc, RECT cells,int id, int lensells) {

    int sizeOfWord = lensells / 3;
    auto font = CreateFont(sizeOfWord, 0, 0, 0, FW_NORMAL, true, false, false, 0, 0, 0, 0, FF_ROMAN, nullptr);
    SelectObject(hdc, font);
    DrawText(hdc,massiveStr[id].c_str(),-1, &cells, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
    DeleteObject(font);
}