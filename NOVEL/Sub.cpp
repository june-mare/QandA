#include "Sub.h"

//�t�H���g�쐬�֐�
HFONT Font(int w, int h, LPCTSTR fontName)
{
	HFONT hFont = CreateFont(
		h,//�����̑傫��
		w,//������
		0,//�p�x
		0,//�x�[�X���C���p�x
		FW_REGULAR, //�����i�����GFW_BOLD)
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
			"�����t�H���g�̍쐬�Ɏ��s",
			fontName, MB_OK);
	}
	return hFont;
}

//�e�L�X�g��\�L����֐�
void text(HDC hdc, int x, int y,const char* text) 
{
	TextOut(hdc,x, y, text, lstrlen(text));
	SetBkMode(hdc, TRANSPARENT);
}

//log�e�L�X�g�̕\�L�𑗂�
void LogTextD(HDC hdc, const char* text,int ty)
{
	RECT Trc;
	Trc.left = 60; Trc.right = 740;
	Trc.top = 0 + ty; Trc.bottom = 600;
	
	DrawText(hdc, text, lstrlen(text), &Trc, DT_WORDBREAK);
}

//�e�L�X�g�̕\�L�𑗂�
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

//�摜�̓ǂݎ��֐�
bool ReadBmp(HBITMAP* hBmp,LPCTSTR FName)
{
		*hBmp = (HBITMAP)LoadImage(
		NULL,	//�C���X�^���X�ԍ��i�ȗ��j
			FName,	//�摜�t�@�C����
		IMAGE_BITMAP,	//�r�b�g�}�b�v�摜
		0, 0,	//�摜���̓ǂݍ��݊J�n���W
		LR_LOADFROMFILE);	//�t�@�C������ǂݍ���
	if (*hBmp == NULL)
	{
		MessageBox(NULL,
			"�摜�t�@�C���̓ǂݍ��݂Ɏ��s",
			FName, MB_YESNO + MB_ICONEXCLAMATION);
		return false;

	}
	return true;
}

//�摜�\���֐�
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

//���j���[
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

//���ߒj�̎q
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

//���ߏ��̎q
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

//�e�L�X�g�{�b�N�X
void MTBlt(HDC hdc, HDC hMemDC, HBITMAP* hBmp,BLENDFUNCTION Bfunk, int px,int py,int sx,int sy)
{
	SelectObject(hMemDC, *hBmp);
	AlphaBlend(hdc, px, py, sx, sy, hMemDC, 0, 0, sx, sy, Bfunk);
}

//�u���b�N�A�E�g
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

//�z���C�g�A�E�g
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

//BGM�t�@�C���̓W�J
void BGM_Play(int sn, HWND hWnd)
{
	CHAR wNum[3] = {NULL};//����
	 _itoa_s(sn+1, wNum, 10);
	 CHAR FilePath[64] = " BGM";
	 strcat_s(FilePath, wNum);
	 CHAR str[128] = {NULL};
	 strcat_s(str,"play");
	 strcat_s(str,FilePath);
	 strcat_s(str," notify");
	 mciSendStringA(str, NULL, 0, hWnd);
}

//BGM�t�@�C�������܂�
void BGM_Close(int sn,HWND hWnd)
{
	CHAR wNum[3] = {NULL};//����
	_itoa_s(sn+1, wNum, 10);
	CHAR FilePath[64] = " BGM";
	strcat_s(FilePath, wNum);
	CHAR str[128] = {NULL};
	strcat_s(str, "close");
	strcat_s(str, FilePath);
	mciSendStringA(str, NULL, 0, hWnd);
}

//BGM�t�@�C����ǂݍ���
void BGM_OPEN(HWND hWnd, LPCSTR FName)
{
	mciSendStringA(FName, NULL, 0, hWnd);
}

//BGM�t�@�C������m�F
void BGM_CHECK(int sn, HWND hWnd)
{
	CHAR status[256] = {NULL};
	CHAR second[128] = {NULL};
	CHAR wNum[3] = { NULL };//����
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