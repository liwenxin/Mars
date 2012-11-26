/* ************************************
PrintScreen.cpp
实现功能：截屏，保存为文件，并显示在窗口中
**************************************/
/* 头文件 */
#include <windows.h>
#include "stdafx.h"
#include "GameExe.h"
#include "AviFile.h"
/* 常量定义 */
#define PALVERSION 0x300
#define CAP_SHOW_MODE_STRETCH 1
#define CAP_SHOW_MODE_NOSTRETCH 0
/* 全局变量 */
HBITMAP ghBitmap=NULL;//用来保存所截取到的位图的信息
RECT rectShow;//截取区域
int nCounts=300;//截取60帧就停止

/////////////////////////////////////////////////////////////
///          Create the Movie File Object                ///
///////////////////////////////////////////////////////////
CAviFile	avi("Output.Avi", /*mmioFOURCC('M','S','V','C')*/NULL, 15);	// Use MSVC codec with 1 FPS

//修改这里截取不同的窗口，如果为NULL，则截取整个屏幕
GameInfo* gameInfo=new GameInfo("Warcraft III",false);
//LPSTR szCaptureWindowName="Warcraft III";
/* 函数声明 */
int WINAPI WinMain(HINSTANCE, HINSTANCE,LPSTR ,int);
LRESULT CALLBACK MainWndProc(HWND,UINT,WPARAM,LPARAM);
HBITMAP ScreenCapture(/*LPSTR filename,WORD BitCount,*/LPRECT lpRect);
VOID DoPaint(HWND hWnd,DWORD dwMode);

/* ************************************
* WinMain
* 参数	hinstance，应用程序本次运行实例
*			hPrevInstance，应用程序之前的实例，始终为NULL
*			lpCmdLine，命令行参数
*			nCmdShow，如果窗口，SW_SHOW
* 返回值	失败返回FALSE
**************************************/
int WINAPI WinMain(
				   HINSTANCE hinstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	//如果游戏没有启动，则启动游戏
	if (gameInfo->getFlag()==false)
	{
		WinExec( "E:\\Warcraft III-1.24\\War3.exe", SW_SHOW);
		gameInfo->setFlag(true);
	}	
	Sleep(5000);
	WNDCLASSEX wcx;
	HWND hwnd;
	MSG msg;
	WORD wport=80;
	BOOL fGotMessage;
	HWND hwndCap=NULL;
	if (gameInfo->getFlag())
	{
		//截取全屏幕还是窗口
		if (gameInfo->getGameName()!=NULL)
		{
			hwndCap=FindWindow(NULL,gameInfo->getGameName());
			if(!GetWindowRect(hwndCap,&rectShow))
			{
				MessageBox(NULL,"Can not find window to capture","error",MB_OK);
				return 0;
			}
		}
	}
	
	//注册窗口类，并创建窗口，用于显示截取的位图
	wcx.cbSize = sizeof(wcx); // WNDCLASSEX结构的大小
	wcx.style = CS_HREDRAW | CS_VREDRAW; // 如果大小改变了重绘窗口
	wcx.lpfnWndProc = MainWndProc; // 窗口消息处理函数
	wcx.cbClsExtra = 0; // 无附加窗口类内存
	wcx.cbWndExtra = 0; // 无附加窗口内存
	wcx.hInstance = hinstance; // 应用程序实例
	wcx.hIcon = NULL; // 图标
	wcx.hCursor = LoadCursor(NULL,	IDC_ARROW); // 鼠标指针
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 背景画刷
	wcx.lpszMenuName = NULL; // 菜单资源
	wcx.lpszClassName = "MainWClass"; // 窗口类名
	wcx.hIconSm = NULL;
	if(!RegisterClassEx(&wcx))
	{
		return 1;
	}
	//创建窗口
	hwnd = CreateWindow(
		"MainWClass", // 窗口类名，使用之前注册的主窗口类
		"Mars", // 窗口名，显示在窗口标题栏上的字符串
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |WS_MAXIMIZE|WS_POPUPWINDOW, 	// 窗口样式
		CW_USEDEFAULT, // 水平位置（默认）
		CW_USEDEFAULT, // 垂直位置（默认）
		CW_USEDEFAULT, // 宽
		CW_USEDEFAULT, // 高
		(HWND) NULL, // 无父窗口
		(HMENU) NULL, // 菜单
		hinstance, //应用程序实例
		(LPVOID) NULL);
	if(!hwnd)
	{
		return 1;
	}
	for (int i=0;i<nCounts;i++)
	{
		//截屏
		ghBitmap=ScreenCapture(&rectShow);
		avi.AppendNewFrame(ghBitmap);
		//Sleep(500);
	}
	
	////将一帧的数据保存到avi视频中
	//if(FAILED(avi.AppendNewFrame(ghBitmap)))	//avi.AppendNewFrame(320, 240, pBits, 32)))
	//{
	//	MessageBox(hwnd, avi.GetLastErrorMessage(), "Error Occured", MB_OK | MB_ICONERROR);
	//}
	//avi.~CAviFile();
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);
	// 消息循环
	while ((fGotMessage = GetMessage(&msg, (HWND) NULL, 0, 0)) != 0 && fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
	UNREFERENCED_PARAMETER(lpCmdLine);
}
HBITMAP ScreenCapture(/*LPSTR filename,WORD BitCount,*/LPRECT lpRect)
{
	HBITMAP hBitmap;
	//显示器屏幕DC
	HDC hScreenDC=CreateDC("DISPLAY",NULL,NULL,NULL);
	HDC hmemDC= CreateCompatibleDC(hScreenDC);
	//显示器屏幕的宽和高
	int ScreenWidth=GetDeviceCaps(hScreenDC,HORZRES);
	int ScreenHeight=GetDeviceCaps(hScreenDC,VERTRES);
	//旧的BITMAP，用于与所需截取的位置交换
	HBITMAP hOldBM;
	//保存位图数据
	//PVOID lpvpxldata;
	//截屏获取的长度和起点
	INT ixStart;
	INT iyStart;
	INT iX;
	INT iY;
	////位图数据大小
	//DWORD dwBitmapArrsySize;
	////几个大小
	//DWORD nBitsOffset;
	//DWORD lImageSize;
	//DWORD lFizeSize;
	////位图信息头
	//BITMAPINFO bmInfo;
	////位图文件头
	//BITMAPFILEHEADER bmFileHeader;
	////写文件用
	//HANDLE hbmfile;
	//DWORD dwWritten;
	//如果LPRECT为NULL，截取整个屏幕
	if(lpRect==NULL)
	{
		ixStart=iyStart=0;
		iX=ScreenWidth;
		iY=ScreenHeight;
	}
	else
	{
		ixStart=lpRect->left;
		iyStart=lpRect->top;
		iX=lpRect->right-lpRect->left;
		iY=lpRect->bottom-lpRect->top;
	}
	//创建BTIMAP
	hBitmap=CreateCompatibleBitmap(hScreenDC,iX,iY);
	//将BITMAP选择入内存
	hOldBM=(HBITMAP)SelectObject(hmemDC,hBitmap);
	//BitBlt屏幕DC到内存DC，根据所需截取的获取设置参数
	BitBlt(hmemDC,0,0,iX,iY,hScreenDC,ixStart,iyStart,SRCCOPY);
	//将旧的BITMAP对象选择回内存DC，返回值为被替换的对象，即所获取的位图
	hBitmap=(HBITMAP)SelectObject(hmemDC,hOldBM);


	//if (filename==NULL)
	//{
	//	DeleteDC(hScreenDC);
	//	DeleteDC(hmemDC);
	//	return hBitmap;
	//}
	////为位图数据申请内存空间
	//dwBitmapArrsySize=((((iX*32)+31)&~31)>>3)*iY;
	//lpvpxldata=HeapAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,dwBitmapArrsySize);
	//ZeroMemory(lpvpxldata,dwBitmapArrsySize);

	////填充BITMAPINFO结构
	//ZeroMemory(&bmInfo,sizeof(BITMAPINFO));
	//bmInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	//bmInfo.bmiHeader.biWidth=iX;
	//bmInfo.bmiHeader.biHeight=iY;
	//bmInfo.bmiHeader.biPlanes=1;
	//bmInfo.bmiHeader.biBitCount=BitCount;
	//bmInfo.bmiHeader.biCompression=BI_RGB;
	//
	////填充BITMAPFILEHEADER结构
	//ZeroMemory(&bmFileHeader,sizeof(BITMAPFILEHEADER));
	//nBitsOffset=sizeof(BITMAPFILEHEADER)+bmInfo.bmiHeader.biSize;
	//lImageSize=((((bmInfo.bmiHeader.biWidth*bmInfo.bmiHeader.biBitCount)+31)&~31)>>3)*bmInfo.bmiHeader.biHeight;
	//lFizeSize=nBitsOffset+lImageSize;
	//bmFileHeader.bfType='B'+('M'<<8);
	//bmFileHeader.bfSize=lFizeSize;
	//bmFileHeader.bfOffBits=nBitsOffset;
	////获取DIB用于写入到文件,按行扫描
	//GetDIBits(hmemDC,hBitmap,0,bmInfo.bmiHeader.biHeight,lpvpxldata,&bmInfo,DIB_RGB_COLORS);
	////写文件
	//hbmfile=CreateFile(filename,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	//if (hbmfile==INVALID_HANDLE_VALUE)
	//{
	//	MessageBox(NULL,"Create file error","error",MB_OK);
	//}
	//WriteFile(hbmfile,&bmFileHeader,sizeof(BITMAPFILEHEADER),&dwWritten,NULL);
	//WriteFile(hbmfile,&bmInfo,sizeof(BITMAPINFO),&dwWritten,NULL);
	//WriteFile(hbmfile,lpvpxldata,lImageSize,&dwWritten,NULL);
	//CloseHandle(hbmfile);
	//释放内存，清除不同的DC
	//这里没有删除BITMAP对象，需要在显示完成后删除
	//HeapFree(GetProcessHeap(),HEAP_NO_SERIALIZE,lpvpxldata);
	ReleaseDC(0,hScreenDC);
	DeleteDC(hmemDC);
	return hBitmap;


}
/* ************************************
* MainWndProc
* 功能	主窗口消息处理函数
**************************************/
LRESULT CALLBACK MainWndProc(
							 HWND hwnd, // 窗口句柄
							 UINT uMsg, // 消息标识符
							 WPARAM wParam, // 消息的第一个参数
							 LPARAM lParam)// 消息的第二个参数
{

	switch (uMsg)
	{
	case WM_CREATE:	// 窗口创建时收到此消息
		break;

	case WM_PAINT:	// 窗口被绘制时收到此消息
		//显示
		DoPaint(hwnd,CAP_SHOW_MODE_STRETCH);
		break;

	case WM_DESTROY:
		DeleteObject(ghBitmap);
		PostQuitMessage(0);
		break;
		// 可以在此处理其他消息
	default:
		break;
	}
	// 有很多消息未做处理，需要由默认窗口消息处理函数来处理
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
/************************************************************************/
/*功能：将位图全局变量(ghBitmap)显示在界面上
* 参数：HWND hWnd，用于显示位图的窗口
  DWORD dwMode，模式，是否拉伸
*/
/************************************************************************/
VOID DoPaint(HWND hWnd,DWORD dwMode)
{
	PAINTSTRUCT ps;
	RECT rect;
	HDC hdcMem;
	BITMAP bm;
	DIBSECTION ds;
	//开始绘制
	HDC hDC=BeginPaint(hWnd,&ps);
	//获取窗口的Client区域，用于显示位图
	GetClientRect(hWnd,&rect);
	//设置拉伸模式
	SetStretchBltMode(hDC,HALFTONE);
	//将BITMAP对象选入内存DC
	hdcMem=CreateCompatibleDC(hDC);
	SelectObject(hdcMem,ghBitmap);
	if (ghBitmap)
	{
		//获取DIB属性
		if (GetObject(ghBitmap,sizeof(BITMAP),&bm))
		{
			//判断参数：是否根据显示窗口大小拉升位图
			//采用不同的方式将内存DC StretchBlt至窗口Client区域DC
			if (dwMode==CAP_SHOW_MODE_STRETCH)
			{
				StretchBlt(hDC,0,0,rect.right,rect.bottom,hdcMem,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
			}
			else
			{
				//不拉伸，计算显示的位图，将其显示在Client的中央
				INT ixStart=(rect.right-rect.left-bm.bmWidth)/2;
				INT iyStart=(rect.bottom-rect.top-bm.bmHeight)/2;
				ixStart=ixStart<0 ? 0:ixStart;
				iyStart=iyStart<0 ? 0:iyStart;
				BitBlt(hDC,0,0,rect.right,rect.bottom,hdcMem,-ixStart,-iyStart,SRCCOPY);
			}
			DeleteDC(hdcMem);
		}
	}
	//如果没有位图，则使用Brush填充
	else{
		PatBlt(hDC,0,0,rect.right,rect.bottom,BLACKNESS);
	}
	EndPaint(hWnd,&ps);
}