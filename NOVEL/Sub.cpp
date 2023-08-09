#include "Sub.h"

//フォント作成関数
HFONT Font(int w, int h, LPCTSTR fontName)
{
	HFONT hFont = CreateFont(
		h,//文字の大きさ
		w,//文字幅
		0,//角度
		0,//ベースライン角度
		FW_REGULAR, //太さ（太字；FW_BOLD)
		FALSE,
		FALSE,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		VARIABLE_PITCH | FF_ROMAN,
		fontName);
	if (hFont == NULL)
	{
		MessageBox(NULL,
			"文字フォントの作成に失敗",
			fontName, MB_OK);
	}
	return hFont;
}

//テキストを表記する関数
void text(HDC hdc, int x, int y,const char* text) 
{
	TextOut(hdc,x, y, text, lstrlen(text));
	SetBkMode(hdc, TRANSPARENT);
}

//logテキストの表記を送る
void LogTextD(HDC hdc, const char* text,int ty)
{
	RECT Trc;
	Trc.left = 60; Trc.right = 740;
	Trc.top = 0 + ty; Trc.bottom = 600;
	
	DrawText(hdc, text, lstrlen(text), &Trc, DT_WORDBREAK);
}

//テキストの表記を送る
void TextD(HDC hdc, const char* text, int Tln, bool* Tcheck)
{
	RECT Trc;
	Trc.left = 60;Trc.right = 740;
	Trc.top = 460;Trc.bottom = 590;
	if (*Tcheck == true)Tln = lstrlen(text);
	else if (*Tcheck == false)
	{
		if (lstrlen(text) < Tln) {	*Tcheck = true; Tln = lstrlen(text);}
	}
	DrawText(hdc, text, Tln, &Trc, DT_WORDBREAK);
}

//画像の読み取り関数
bool ReadBmp(HBITMAP* hBmp,LPCTSTR FName)
{
		*hBmp = (HBITMAP)LoadImage(
		NULL,	//インスタンス番号（省略）
			FName,	//画像ファイル名
		IMAGE_BITMAP,	//ビットマップ画像
		0, 0,	//画像内の読み込み開始座標
		LR_LOADFROMFILE);	//ファイルから読み込む
	if (*hBmp == NULL)
	{
		MessageBox(NULL,
			"画像ファイルの読み込みに失敗",
			FName, MB_YESNO + MB_ICONEXCLAMATION);
		return false;

	}
	return true;
}

//画像表示関数
void BBlt(HDC hdc, HDC hMemDC, HBITMAP hBmp)
{
	SelectObject(hMemDC, hBmp);
	BitBlt(
		hdc,
		0, 0,
		WND_W, WND_H,
		hMemDC,
		0, 0,
		SRCCOPY);
}

//メニュー
void TBltS(HDC hdc, HDC hMemDC,int x,int y, HBITMAP* hBmp)
{
	SelectObject(hMemDC, *hBmp);
	TransparentBlt(
		hdc,
		x,
		y,
		MENU_SIZE,
		hMemDC,
		0, 0,MENU_SIZE,
		TRANSPARENT_COLOR
	);
}

//透過男の子
void TBltM(HDC hdc, HDC hMemDC, HBITMAP* hBmp, int x, int y)
{
	SelectObject(hMemDC, *hBmp);
	TransparentBlt(
		hdc,
		x,
		y,
		300,
		600,
		hMemDC,
		0, 0, MEN_CHAR,
		TRANSPARENT_COLOR
	);
}

//透過女の子
void TBltG(HDC hdc, HDC hMemDC, HBITMAP* hBmp, int x, int y)
{
	SelectObject(hMemDC, *hBmp);
	TransparentBlt(
		hdc,
		x,
		y,
		320,
		1020,
		hMemDC,
		0, 0, GIRL_CHAR,
		TRANSPARENT_COLOR
	);
}

void TBlt(HDC hdc, HDC hMemDC, HBITMAP* hBmp, int w, int h, int x, int y)
{
	SelectObject(hMemDC, *hBmp);
	TransparentBlt(
		hdc,
		x,
		y,
		w,
		h,
		hMemDC,
		0, 0, 
		w, h,
		TRANSPARENT_COLOR
	);
}

//テキストボックス
void MTBlt(HDC hdc, HDC hMemDC, HBITMAP* hBmp,BLENDFUNCTION Bfunk, int px,int py,int sx,int sy)
{
	SelectObject(hMemDC, *hBmp);
	AlphaBlend(hdc, px, py, sx, sy, hMemDC, 0, 0, sx, sy, Bfunk);
}

//ブラックアウト
void BO(HDC hdc, HDC hMemDC,int bk,bool* bkFlag, HBITMAP* hBmp)
{
	*bkFlag = true;	
	static BLENDFUNCTION hBlack;
	hBlack.BlendOp = AC_SRC_OVER;
	hBlack.BlendFlags = 0;
	hBlack.SourceConstantAlpha = bk;
	hBlack.AlphaFormat = 0;
	SelectObject(hMemDC, *hBmp);
	AlphaBlend(hdc, 0, 0, 800, 600, hMemDC, 0, 0, 800, 600, hBlack);
}

//ホワイトアウト
void WO(HDC hdc, HDC hMemDC, int bk, bool* bkFlag, HBITMAP* hBmp)
{
	static BLENDFUNCTION hBlack;
	hBlack.BlendOp = AC_SRC_OVER;
	hBlack.BlendFlags = 0;
	hBlack.SourceConstantAlpha = bk;
	hBlack.AlphaFormat = 0;
	SelectObject(hMemDC, *hBmp);
	AlphaBlend(hdc, 0, 0, 800, 600, hMemDC, 0, 0, 800, 600, hBlack);
}

//BGMファイルの展開
void BGM_Play(int sn, HWND hWnd)
{
	CHAR wNum[3] = {NULL};//桁数
	 _itoa_s(sn+1, wNum, 10);
	 CHAR FilePath[64] = " BGM";
	 strcat_s(FilePath, wNum);
	 CHAR str[128] = {NULL};
	 strcat_s(str,"play");
	 strcat_s(str,FilePath);
	 strcat_s(str," notify");
	 mciSendStringA(str, NULL, 0, hWnd);
}

//BGMファイルをしまう
void BGM_Close(int sn,HWND hWnd)
{
	CHAR wNum[3] = {NULL};//桁数
	_itoa_s(sn+1, wNum, 10);
	CHAR FilePath[64] = " BGM";
	strcat_s(FilePath, wNum);
	CHAR str[128] = {NULL};
	strcat_s(str, "close");
	strcat_s(str, FilePath);
	mciSendStringA(str, NULL, 0, hWnd);
}

//BGMファイルを読み込む
void BGM_OPEN(HWND hWnd, LPCSTR FName)
{
	mciSendStringA(FName, NULL, 0, hWnd);
}

//BGMファイル動作確認
void BGM_CHECK(int sn, HWND hWnd)
{
	CHAR status[256] = {NULL};
	CHAR second[128] = {NULL};
	CHAR wNum[3] = { NULL };//桁数
	_itoa_s(sn + 1, wNum, 10);
	CHAR FilePath[64] = " BGM";
	strcat_s(FilePath, wNum);
	CHAR tmp[64] = {NULL};
	strcpy_s(tmp,FilePath);
	CHAR str[128] = { NULL };
	strcat_s(str, "status");
	strcat_s(str, FilePath);
	strcat_s(str, " mode");
	mciSendStringA(str, status, 255, hWnd);
	if (strcmp(status,"stopped")==0)
		{
			strcat_s(second, "seek ");
			strcat_s(second, tmp);
			strcat_s(second, " to start");
			mciSendStringA(second, NULL,0, hWnd);
			BGM_Play(sn, hWnd);
		}
}


//AudioSource AS;
//
//AS = GetComponent<AudioSource>();
//
//AS.Play();