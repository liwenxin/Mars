/* ************************************
PrintScreen.cpp
ʵ�ֹ��ܣ�����������Ϊ�ļ�������ʾ�ڴ�����
**************************************/
/* ͷ�ļ� */
#include <windows.h>
#include "stdafx.h"
#include "GameExe.h"
#include "AviFile.h"
/* �������� */
#define PALVERSION 0x300
#define CAP_SHOW_MODE_STRETCH 1
#define CAP_SHOW_MODE_NOSTRETCH 0
/* ȫ�ֱ��� */
HBITMAP ghBitmap=NULL;//������������ȡ����λͼ����Ϣ
RECT rectShow;//��ȡ����
int nCounts=300;//��ȡ60֡��ֹͣ

/////////////////////////////////////////////////////////////
///          Create the Movie File Object                ///
///////////////////////////////////////////////////////////
CAviFile	avi("Output.Avi", /*mmioFOURCC('M','S','V','C')*/NULL, 15);	// Use MSVC codec with 1 FPS

//�޸������ȡ��ͬ�Ĵ��ڣ����ΪNULL�����ȡ������Ļ
GameInfo* gameInfo=new GameInfo("Warcraft III",false);
//LPSTR szCaptureWindowName="Warcraft III";
/* �������� */
int WINAPI WinMain(HINSTANCE, HINSTANCE,LPSTR ,int);
LRESULT CALLBACK MainWndProc(HWND,UINT,WPARAM,LPARAM);
HBITMAP ScreenCapture(/*LPSTR filename,WORD BitCount,*/LPRECT lpRect);
VOID DoPaint(HWND hWnd,DWORD dwMode);

/* ************************************
* WinMain
* ����	hinstance��Ӧ�ó��򱾴�����ʵ��
*			hPrevInstance��Ӧ�ó���֮ǰ��ʵ����ʼ��ΪNULL
*			lpCmdLine�������в���
*			nCmdShow��������ڣ�SW_SHOW
* ����ֵ	ʧ�ܷ���FALSE
**************************************/
int WINAPI WinMain(
				   HINSTANCE hinstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	//�����Ϸû����������������Ϸ
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
		//��ȡȫ��Ļ���Ǵ���
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
	
	//ע�ᴰ���࣬���������ڣ�������ʾ��ȡ��λͼ
	wcx.cbSize = sizeof(wcx); // WNDCLASSEX�ṹ�Ĵ�С
	wcx.style = CS_HREDRAW | CS_VREDRAW; // �����С�ı����ػ洰��
	wcx.lpfnWndProc = MainWndProc; // ������Ϣ������
	wcx.cbClsExtra = 0; // �޸��Ӵ������ڴ�
	wcx.cbWndExtra = 0; // �޸��Ӵ����ڴ�
	wcx.hInstance = hinstance; // Ӧ�ó���ʵ��
	wcx.hIcon = NULL; // ͼ��
	wcx.hCursor = LoadCursor(NULL,	IDC_ARROW); // ���ָ��
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ������ˢ
	wcx.lpszMenuName = NULL; // �˵���Դ
	wcx.lpszClassName = "MainWClass"; // ��������
	wcx.hIconSm = NULL;
	if(!RegisterClassEx(&wcx))
	{
		return 1;
	}
	//��������
	hwnd = CreateWindow(
		"MainWClass", // ����������ʹ��֮ǰע�����������
		"Mars", // ����������ʾ�ڴ��ڱ������ϵ��ַ���
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |WS_MAXIMIZE|WS_POPUPWINDOW, 	// ������ʽ
		CW_USEDEFAULT, // ˮƽλ�ã�Ĭ�ϣ�
		CW_USEDEFAULT, // ��ֱλ�ã�Ĭ�ϣ�
		CW_USEDEFAULT, // ��
		CW_USEDEFAULT, // ��
		(HWND) NULL, // �޸�����
		(HMENU) NULL, // �˵�
		hinstance, //Ӧ�ó���ʵ��
		(LPVOID) NULL);
	if(!hwnd)
	{
		return 1;
	}
	for (int i=0;i<nCounts;i++)
	{
		//����
		ghBitmap=ScreenCapture(&rectShow);
		avi.AppendNewFrame(ghBitmap);
		//Sleep(500);
	}
	
	////��һ֡�����ݱ��浽avi��Ƶ��
	//if(FAILED(avi.AppendNewFrame(ghBitmap)))	//avi.AppendNewFrame(320, 240, pBits, 32)))
	//{
	//	MessageBox(hwnd, avi.GetLastErrorMessage(), "Error Occured", MB_OK | MB_ICONERROR);
	//}
	//avi.~CAviFile();
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);
	// ��Ϣѭ��
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
	//��ʾ����ĻDC
	HDC hScreenDC=CreateDC("DISPLAY",NULL,NULL,NULL);
	HDC hmemDC= CreateCompatibleDC(hScreenDC);
	//��ʾ����Ļ�Ŀ�͸�
	int ScreenWidth=GetDeviceCaps(hScreenDC,HORZRES);
	int ScreenHeight=GetDeviceCaps(hScreenDC,VERTRES);
	//�ɵ�BITMAP�������������ȡ��λ�ý���
	HBITMAP hOldBM;
	//����λͼ����
	//PVOID lpvpxldata;
	//������ȡ�ĳ��Ⱥ����
	INT ixStart;
	INT iyStart;
	INT iX;
	INT iY;
	////λͼ���ݴ�С
	//DWORD dwBitmapArrsySize;
	////������С
	//DWORD nBitsOffset;
	//DWORD lImageSize;
	//DWORD lFizeSize;
	////λͼ��Ϣͷ
	//BITMAPINFO bmInfo;
	////λͼ�ļ�ͷ
	//BITMAPFILEHEADER bmFileHeader;
	////д�ļ���
	//HANDLE hbmfile;
	//DWORD dwWritten;
	//���LPRECTΪNULL����ȡ������Ļ
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
	//����BTIMAP
	hBitmap=CreateCompatibleBitmap(hScreenDC,iX,iY);
	//��BITMAPѡ�����ڴ�
	hOldBM=(HBITMAP)SelectObject(hmemDC,hBitmap);
	//BitBlt��ĻDC���ڴ�DC�����������ȡ�Ļ�ȡ���ò���
	BitBlt(hmemDC,0,0,iX,iY,hScreenDC,ixStart,iyStart,SRCCOPY);
	//���ɵ�BITMAP����ѡ����ڴ�DC������ֵΪ���滻�Ķ��󣬼�����ȡ��λͼ
	hBitmap=(HBITMAP)SelectObject(hmemDC,hOldBM);


	//if (filename==NULL)
	//{
	//	DeleteDC(hScreenDC);
	//	DeleteDC(hmemDC);
	//	return hBitmap;
	//}
	////Ϊλͼ���������ڴ�ռ�
	//dwBitmapArrsySize=((((iX*32)+31)&~31)>>3)*iY;
	//lpvpxldata=HeapAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,dwBitmapArrsySize);
	//ZeroMemory(lpvpxldata,dwBitmapArrsySize);

	////���BITMAPINFO�ṹ
	//ZeroMemory(&bmInfo,sizeof(BITMAPINFO));
	//bmInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	//bmInfo.bmiHeader.biWidth=iX;
	//bmInfo.bmiHeader.biHeight=iY;
	//bmInfo.bmiHeader.biPlanes=1;
	//bmInfo.bmiHeader.biBitCount=BitCount;
	//bmInfo.bmiHeader.biCompression=BI_RGB;
	//
	////���BITMAPFILEHEADER�ṹ
	//ZeroMemory(&bmFileHeader,sizeof(BITMAPFILEHEADER));
	//nBitsOffset=sizeof(BITMAPFILEHEADER)+bmInfo.bmiHeader.biSize;
	//lImageSize=((((bmInfo.bmiHeader.biWidth*bmInfo.bmiHeader.biBitCount)+31)&~31)>>3)*bmInfo.bmiHeader.biHeight;
	//lFizeSize=nBitsOffset+lImageSize;
	//bmFileHeader.bfType='B'+('M'<<8);
	//bmFileHeader.bfSize=lFizeSize;
	//bmFileHeader.bfOffBits=nBitsOffset;
	////��ȡDIB����д�뵽�ļ�,����ɨ��
	//GetDIBits(hmemDC,hBitmap,0,bmInfo.bmiHeader.biHeight,lpvpxldata,&bmInfo,DIB_RGB_COLORS);
	////д�ļ�
	//hbmfile=CreateFile(filename,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	//if (hbmfile==INVALID_HANDLE_VALUE)
	//{
	//	MessageBox(NULL,"Create file error","error",MB_OK);
	//}
	//WriteFile(hbmfile,&bmFileHeader,sizeof(BITMAPFILEHEADER),&dwWritten,NULL);
	//WriteFile(hbmfile,&bmInfo,sizeof(BITMAPINFO),&dwWritten,NULL);
	//WriteFile(hbmfile,lpvpxldata,lImageSize,&dwWritten,NULL);
	//CloseHandle(hbmfile);
	//�ͷ��ڴ棬�����ͬ��DC
	//����û��ɾ��BITMAP������Ҫ����ʾ��ɺ�ɾ��
	//HeapFree(GetProcessHeap(),HEAP_NO_SERIALIZE,lpvpxldata);
	ReleaseDC(0,hScreenDC);
	DeleteDC(hmemDC);
	return hBitmap;


}
/* ************************************
* MainWndProc
* ����	��������Ϣ������
**************************************/
LRESULT CALLBACK MainWndProc(
							 HWND hwnd, // ���ھ��
							 UINT uMsg, // ��Ϣ��ʶ��
							 WPARAM wParam, // ��Ϣ�ĵ�һ������
							 LPARAM lParam)// ��Ϣ�ĵڶ�������
{

	switch (uMsg)
	{
	case WM_CREATE:	// ���ڴ���ʱ�յ�����Ϣ
		break;

	case WM_PAINT:	// ���ڱ�����ʱ�յ�����Ϣ
		//��ʾ
		DoPaint(hwnd,CAP_SHOW_MODE_STRETCH);
		break;

	case WM_DESTROY:
		DeleteObject(ghBitmap);
		PostQuitMessage(0);
		break;
		// �����ڴ˴���������Ϣ
	default:
		break;
	}
	// �кܶ���Ϣδ��������Ҫ��Ĭ�ϴ�����Ϣ������������
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
/************************************************************************/
/*���ܣ���λͼȫ�ֱ���(ghBitmap)��ʾ�ڽ�����
* ������HWND hWnd��������ʾλͼ�Ĵ���
  DWORD dwMode��ģʽ���Ƿ�����
*/
/************************************************************************/
VOID DoPaint(HWND hWnd,DWORD dwMode)
{
	PAINTSTRUCT ps;
	RECT rect;
	HDC hdcMem;
	BITMAP bm;
	DIBSECTION ds;
	//��ʼ����
	HDC hDC=BeginPaint(hWnd,&ps);
	//��ȡ���ڵ�Client����������ʾλͼ
	GetClientRect(hWnd,&rect);
	//��������ģʽ
	SetStretchBltMode(hDC,HALFTONE);
	//��BITMAP����ѡ���ڴ�DC
	hdcMem=CreateCompatibleDC(hDC);
	SelectObject(hdcMem,ghBitmap);
	if (ghBitmap)
	{
		//��ȡDIB����
		if (GetObject(ghBitmap,sizeof(BITMAP),&bm))
		{
			//�жϲ������Ƿ������ʾ���ڴ�С����λͼ
			//���ò�ͬ�ķ�ʽ���ڴ�DC StretchBlt������Client����DC
			if (dwMode==CAP_SHOW_MODE_STRETCH)
			{
				StretchBlt(hDC,0,0,rect.right,rect.bottom,hdcMem,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
			}
			else
			{
				//�����죬������ʾ��λͼ��������ʾ��Client������
				INT ixStart=(rect.right-rect.left-bm.bmWidth)/2;
				INT iyStart=(rect.bottom-rect.top-bm.bmHeight)/2;
				ixStart=ixStart<0 ? 0:ixStart;
				iyStart=iyStart<0 ? 0:iyStart;
				BitBlt(hDC,0,0,rect.right,rect.bottom,hdcMem,-ixStart,-iyStart,SRCCOPY);
			}
			DeleteDC(hdcMem);
		}
	}
	//���û��λͼ����ʹ��Brush���
	else{
		PatBlt(hDC,0,0,rect.right,rect.bottom,BLACKNESS);
	}
	EndPaint(hWnd,&ps);
}