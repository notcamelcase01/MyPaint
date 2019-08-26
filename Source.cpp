#include "Header.h"

SHORT isSave = 0,FDraw = 0; //0-> Do Nothing 1->Drag , 2->Draw
bool isLoad = false;
int sa = 150,st = 20,ff=10;
POINT homeCord,finalCord;
WPARAM  buttonId;
enum Shape { DO_NOTHING = 100,ELLIPSE = 0, RECTANGLE = 1, ERASE = 2, SAVE = 3, LOAD = 4 };
LRESULT CALLBACK w(HWND h, UINT i, WPARAM wp, LPARAM lp)
{
	HDC hdc;
	switch (i)
	{

	

	case WM_LBUTTONDOWN:
	{
		homeCord.x = GET_X_LPARAM(lp);
		homeCord.y = GET_Y_LPARAM(lp);
		FDraw = 1;
		cout << FDraw << endl;
		break;
	}
	case WM_LBUTTONUP:
	{
		if (FDraw == 1) {
			finalCord.x = GET_X_LPARAM(lp);
			finalCord.y = GET_Y_LPARAM(lp);
			FDraw = 2;
			cout << FDraw << endl;
			break;
		}
	}

	
	case WM_PAINT:
	{

		
		if (FDraw == 0) {
			break;
		}

	
		if (buttonId == (WPARAM)LOAD && !isLoad) {
			PAINTSTRUCT ps;
			hdc = BeginPaint(h, &ps);
			ifstream file;
			file.open("file.txt");
			if (file.is_open()) {

				int num;
				char ch;
				for (int i = st; i < sa; i++) {
					for (int j = st; j < sa; j++) {
						file >> num >> ch;
						
						SetPixel(hdc,i, j, (COLORREF)num);
						cout<<i<<" "<<j<<" : "<< SetPixel(hdc, i, j, (COLORREF)num)<<endl;

					}
				}
			}
			file.close();
			
			EndPaint(h, &ps);
			UpdateWindow(h);
			RedrawWindow(h, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
			buttonId == (WPARAM)DO_NOTHING;
			isLoad = true;
			break;
		}

		if (buttonId == (WPARAM)SAVE && isSave == 0) {
			PAINTSTRUCT ps;
			hdc = BeginPaint(h, &ps);
			ofstream file;
			file.open("file.txt");
			for (int i = st; i < sa; i++) {
				for (int j = st; j < sa;j++) {
					file << SetPixel(hdc,i,j,GetPixel(hdc, i, j))<<",";
					cout << GetPixel(hdc, i, j) << "SAVE" << i<< " " << j<< "\n";
				}
				cout << endl;
			}
			file.close();
			EndPaint(h, &ps);
			isSave = 1;
			UpdateWindow(h);
			RedrawWindow(h, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
			buttonId == (WPARAM)DO_NOTHING;
			break;
		}
		if (FDraw == 2) {
			PAINTSTRUCT ps;
			hdc = BeginPaint(h, &ps);
			FDraw = 0;
	
			
			if (buttonId == (WPARAM)ELLIPSE) {
				HPEN hpen = CreatePen(PS_SOLID, 1, 0xFF0000);
				SelectObject(hdc, hpen);
				Ellipse(hdc, homeCord.x, homeCord.y, finalCord.x, finalCord.y);
				DeleteObject(hpen);
			}
			if (buttonId == (WPARAM)RECTANGLE) {
				HPEN hpen = CreatePen(PS_SOLID, 1, 0xFF0000);
				SelectObject(hdc, hpen);
				Rectangle(hdc, homeCord.x, homeCord.y, finalCord.x, finalCord.y);
				DeleteObject(hpen);
			}
			if (buttonId == (WPARAM)ERASE) {
				RECT rect = { homeCord.x , homeCord.y , finalCord.x,finalCord.y };
				HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
				FillRect(hdc, &rect, brush);
				DeleteObject(brush);
				cout << "ERASE\n";
			}
			
			
			
			EndPaint(h, &ps);
			
			UpdateWindow(h);
			RedrawWindow(h, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
			break;
		}
		
		break;
	}

	case WM_COMMAND:
	{
		buttonId = wp;
		if (buttonId == (WPARAM)LOAD) {
			isLoad = false;
		}
	
	}
	default:
	{
		return DefWindowProc(h, i, wp, lp);
	}
	}
	return 0L;
}


int main() {
	MSG msg;
	WNDCLASSEXW MainClass = {};
	MainClass.cbSize = sizeof(WNDCLASSEXW);
	MainClass.lpfnWndProc = w;
	MainClass.lpszClassName = L"edit";
	RegisterClassExW(&MainClass);
	HWND mainWindow = CreateWindowExW(0L, L"edit", L"window", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 0, 0,1000, 1000, NULL, NULL, NULL, NULL);
	HWND buttomE = CreateWindowExW(0L, L"button", L"E", WS_VISIBLE | WS_BORDER | WS_CHILDWINDOW, 10, 10, 20, 20, mainWindow, (HMENU)ELLIPSE, NULL, NULL);
	HWND buttonR = CreateWindowExW(0L, L"button", L"R", WS_VISIBLE | WS_BORDER | WS_CHILDWINDOW, 10, 35,20, 20, mainWindow, (HMENU)RECTANGLE, NULL, NULL);
	HWND buttonErase = CreateWindowExW(0L, L"button", L"Erase", WS_VISIBLE | WS_BORDER | WS_CHILDWINDOW, 900, 10, 70, 20, mainWindow, (HMENU)ERASE, NULL, NULL);
	HWND buttontSave = CreateWindowExW(0L, L"button", L"S", WS_VISIBLE | WS_BORDER | WS_CHILDWINDOW, 10, 60, 20, 20, mainWindow, (HMENU)SAVE, NULL, NULL);
	HWND buttontload = CreateWindowExW(0L, L"button", L"L", WS_VISIBLE | WS_BORDER | WS_CHILDWINDOW, 10, 85, 20, 20, mainWindow, (HMENU)LOAD, NULL, NULL);

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		//cout << msg.message <<endl;
		DispatchMessage(&msg);
	}
	return EXIT_SUCCESS;
}