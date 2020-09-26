#include<windows.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

char szProgName[]="Paint";

int xView, yView;

enum figure{line, brokenLine, rectangle, ellipse, multiAngle, text};
enum figure activeFigure = line;
int angleCount = 0;
int currentAngle = 0;
POINT *points = NULL;
char str[1000];
int startX = 0, startY = 0, endX = 0, endY = 0;
BOOL isClickedMouse = FALSE;


struct figureElem {
    POINT *points;
    enum figure figureName;
    int angleCount;
    char str[100];
} typedef FigureStruct;

FigureStruct figures[1000];
int figureCount = 0;
POINT tempLine[2];


void addFigure(enum figure fName, POINT *points) {

    switch (fName) {
        case line:
            figures[figureCount].points = (POINT*)malloc(sizeof(POINT) * 2);
            figures[figureCount].figureName = line;
        break;
        case rectangle:
            figures[figureCount].points = (POINT*)malloc(sizeof(POINT) * 2);
            figures[figureCount].figureName = rectangle;
        break;
        case ellipse:
            figures[figureCount].points = (POINT*)malloc(sizeof(POINT) * 2);
            figures[figureCount].figureName = ellipse;
        break;
        case text:
            figures[figureCount].points = (POINT*)malloc(sizeof(POINT));
            figures[figureCount].figureName = text;
            figures[figureCount].points[0].x = points[0].x;
            figures[figureCount].points[0].y = points[0].y;
            strcpy(figures[figureCount].str, str);
            figureCount++;
        return;
        case brokenLine:
            figures[figureCount].points = (POINT*)malloc((angleCount + 1) * sizeof(POINT));
            figures[figureCount].figureName = brokenLine;
            figures[figureCount].angleCount = angleCount;
            for (int i = 0; i <= angleCount; i++) {
                figures[figureCount].points[i].x = points[i].x;
                figures[figureCount].points[i].y = points[i].y;
            }
            figureCount++;
        return;
        case multiAngle:
            figures[figureCount].points = (POINT*)malloc((angleCount + 1) * sizeof(POINT));
            figures[figureCount].figureName = multiAngle;
            figures[figureCount].angleCount = angleCount;
            for (int i = 0; i <= angleCount; i++) {
                figures[figureCount].points[i].x = points[i].x;
                figures[figureCount].points[i].y = points[i].y;
            }
            figureCount++;
        return;

    }

    for (int i = 0; i <= sizeof(*points) / sizeof(POINT); i++) {
        figures[figureCount].points[i].x = points[i].x;
        figures[figureCount].points[i].y = points[i].y;
    }
    figureCount++;
}

void updateWND(HDC hdc) {
    for (int i = 0; i < figureCount; i++) {
        switch (figures[i].figureName) {
            case line:
                MoveToEx(hdc, figures[i].points[0].x, figures[i].points[0].y, NULL);
                LineTo(hdc, figures[i].points[1].x, figures[i].points[1].y);
            break;
            case rectangle:
                Rectangle(hdc, figures[i].points[0].x, figures[i].points[0].y, figures[i].points[1].x, figures[i].points[1].y);
            break;
            case ellipse:
                Ellipse(hdc, figures[i].points[0].x, figures[i].points[0].y, figures[i].points[1].x, figures[i].points[1].y);
            break;
            case multiAngle:
                Polyline(hdc, figures[i].points, figures[i].angleCount + 1);
            break;
            case brokenLine:
                Polyline(hdc, figures[i].points, figures[i].angleCount);
            break;
            case text:
                TextOutA(hdc, figures[i].points[0].x, figures[i].points[0].y, figures[i].str, strlen(figures[i].str));
            break;
        }
    }
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    HWND hWnd;
    MSG lpMsg;
    WNDCLASS w;

    w.lpszClassName=szProgName;
    w.hInstance=hInstance;
    w.lpfnWndProc=WndProc;
    w.hCursor=LoadCursor(NULL, IDC_ARROW);
    w.hIcon=0;
    w.lpszMenuName=0;
    w.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
    w.style=CS_HREDRAW|CS_VREDRAW;
    w.cbClsExtra=0;
    w.cbWndExtra=0;

    if(!RegisterClass(&w))
        return 0;

    hWnd=CreateWindow(szProgName,
        "Фигуры",
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        500,
        400,
        (HWND)NULL,
        (HMENU)NULL,
        (HINSTANCE)hInstance,
        (HINSTANCE)NULL);

    ShowWindow(hWnd, nCmdShow);
    HMENU hMenu = CreateMenu();
    HMENU hPopMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopMenu, "Фигуры");
    {
        AppendMenu(hPopMenu, MF_STRING, 1000, "Прямая");
        AppendMenu(hPopMenu, MF_STRING, 1001, "Ломаная");
        AppendMenu(hPopMenu, MF_STRING, 1002, "Прямоугольник");
        AppendMenu(hPopMenu, MF_STRING, 1003, "Многоугольник");
        AppendMenu(hPopMenu, MF_STRING, 1004, "Эллипс");
        AppendMenu(hPopMenu, MF_STRING, 1005, "Текст");
    }
    SetMenu(hWnd, hMenu);
    UpdateWindow(hWnd);

    while(GetMessage(&lpMsg, NULL, 0, 0)) {
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }
    return(lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg,
WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch(messg)
    {

        case WM_SIZE:
            xView=LOWORD(lParam);
            yView=HIWORD(lParam);
        break;


        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            switch (activeFigure) {
                case line:
                    MoveToEx(hdc, startX, startY, NULL);
                    LineTo(hdc, endX, endY);
                break;
                case rectangle:
                    Rectangle(hdc, startX, startY, endX, endY);
                break;
                case ellipse:
                    Ellipse(hdc, startX, startY, endX, endY);
                break;
                case multiAngle:
                    Polyline(hdc, points, angleCount + 1);
                break;
                case brokenLine:
                    Polyline(hdc, points, angleCount);
                break;
                case text:
                    TextOutA(hdc,endX, endY, str, strlen(str));
                break;
            }
            updateWND(hdc);
            EndPaint(hWnd, &ps);
        break;
        case WM_LBUTTONDOWN:
            isClickedMouse = TRUE;
            hdc = GetDC(hWnd);

            startX=LOWORD(lParam);
            startY=HIWORD(lParam);
            if (activeFigure == multiAngle || activeFigure == brokenLine) {
                if (points == NULL) {
                    points = (POINT*)malloc((angleCount + 1) * sizeof(POINT));
                    currentAngle = 0;
                }
                if (currentAngle == (angleCount - 1)) {
                    points[currentAngle].x = startX;
                    points[currentAngle].y = startY;
                    points[currentAngle + 1].x = points[0].x;
                    points[currentAngle + 1].y = points[0].y;
                    addFigure(activeFigure, points);
                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
                    currentAngle = 0;
                    free(points);
                    points = NULL;
                }else {
                    points[currentAngle].x = startX;
                    points[currentAngle].y = startY;
                    currentAngle++;
                }
            }
        break;
        case WM_LBUTTONUP:
            isClickedMouse = FALSE;
            endX=LOWORD(lParam);
            endY=HIWORD(lParam);

            switch (activeFigure) {
                case line:
                {
                    tempLine[0].x = startX;
                    tempLine[0].y = startY;

                    tempLine[1].x = endX;
                    tempLine[1].y = endY;
                    addFigure(line, tempLine);
                }
                break;
                case rectangle:
                    tempLine[0].x = startX;
                    tempLine[0].y = startY;

                    tempLine[1].x = endX;
                    tempLine[1].y = endY;
                    addFigure(rectangle, tempLine);
                break;
                case ellipse:
                    tempLine[0].x = startX;
                    tempLine[0].y = startY;

                    tempLine[1].x = endX;
                    tempLine[1].y = endY;
                    addFigure(ellipse, tempLine);
                break;
                case text:
                    TextOutA(hdc,endX, endY, str, strlen(str));
                    POINT tempTextPoint;
                    tempLine[0].x = endX;
                    tempLine[0].y = endY;
                    addFigure(text, tempLine);
                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
                break;
            }
        break;
        case WM_MOUSEMOVE:
            hdc = GetDC(hWnd);
            if (isClickedMouse) {
                endX=LOWORD(lParam);
                endY=HIWORD(lParam);
            }
            if (activeFigure != multiAngle && activeFigure != brokenLine && activeFigure != text) {
                InvalidateRect(hWnd, NULL, TRUE);
                UpdateWindow(hWnd);
            }
        break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 1000:
                    activeFigure = line;
                break;
                case 1001:
                    activeFigure = brokenLine;
                    printf("Angle count: ");
                    scanf("%d", &angleCount);
                break;
                case 1002:
                    activeFigure = rectangle;
                break;
                case 1003:
                    activeFigure = multiAngle;
                    printf("Angle count: ");
                    scanf("%d", &angleCount);
                break;
                case 1004:
                    activeFigure = ellipse;
                break;
                case 1005:
                    printf("Enter string: ");
                    fflush(stdin);
                    gets(str);
                    activeFigure = text;
                break;
            }
            startX = 0;
            startY = 0;
            endX = 0;
            endY = 0;
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;

        default:
            return(DefWindowProc(hWnd, messg, wParam, lParam));
    }
    return 0;
}
