#include<windows.h>
#include<math.h>
#include<string.h>

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
            if (activeFigure == text) {
                InvalidateRect(hWnd, NULL, TRUE);
                UpdateWindow(hWnd);
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
