#pragma once
#pragma comment(lib,"winmm.lib")//����炷���C�u����
#pragma comment(lib, "Msimg32.lib")	//TBlt�g�p�̍ەK�v
#ifndef _SUB_
#define _SUB_

//include�錾
#include<Windows.h>
#include<wchar.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <assert.h>

//�}�N����`
#define WND_W	800 //�E�B���h�E�̕�(width)
#define WND_H	600 //�E�B���h�E�̍���(height)
#define CLASS_NAME "Q&A"//�A�v���P�[�V������
#define TITLE_NAME "Q&A"

//��������̌��x
#define TLN_MAX 100

//�摜�̂ł���
#define MEN_CHAR 400,740
#define GIRL_CHAR 360,1130
#define MENU_SIZE 52,55
#define BLACK_SIZE 0,0,800,600
#define SEL_FLAME 600,50

//�摜�̏ꏊ
#define MAIN_BOX 750,150  //memdc
#define MAIN_BOX_S 20,450 //hdc
#define NAME_BOX 200,44
#define NAME_POS 20,400

//���ߐF
#define TRANSPARENT_COLOR RGB(0,255,0)	

//�񋓏���
enum Scene
{
	geta,
	koumon,
	ura,
	rouka,
	Black,
	Opning,
	kyou,
	asobi,
	endg,
	white,
	ranca,
	rancb,
	rancs,
	ranc,

	Max
};

enum BGM
{
	Op,
	bone,
	btwo,
	bfor,
	bthree,
	End,
	bfive,
	lust,

	Ma
};

enum CHARACTER
{
	Pl,
	Yu1,
	Yu2,
	He,
	Yn,

	CMA
};

enum Flerm {
	main,
	selv,
	sentaku,
	name,
	menu,
	menus,
	load,
	loads,
	save,
	saves,
	log,
	logs,


	Fmax
};

#pragma region �v���g�^�C�v

void MTBlt(HDC hdc, HDC hMemDC, HBITMAP* hBmp, BLENDFUNCTION Bfunk, int px, int py, int sx, int sy);
void text(HDC hdc, int x, int y,const char* text);
bool ReadBmp(HBITMAP* hBmp, LPCTSTR FName);
void LogTextD(HDC hdc, const char* text, int ty);
void TextD(HDC hdc, const char* text, int Tln, bool* Tcheck);
void BO(HDC hdc, HDC hMemDC, int bk, bool* bkFlag, HBITMAP* hBmp);
void WO(HDC hdc, HDC hMemDC, int bk, bool* bkFlag, HBITMAP* hBmp);
void BBlt(HDC hdc, HDC hMemDC, HBITMAP hBmp);
HFONT Font(int w, int h, LPCTSTR FName);
void TBltS(HDC hdc, HDC hMemDC, int x, int y, HBITMAP* hBmp);
void TBltM(HDC hdc, HDC hMemDC, HBITMAP* hBmp, int x, int y);
void TBltG(HDC hdc, HDC hMemDC, HBITMAP* hBmp, int x, int y);
void TBlt(HDC hdc, HDC hMemDC, HBITMAP* hBmp, int x, int y, int w = 64, int h = 64);
void BGM_Play(int sn, HWND hWnd);
void BGM_Close(int sn, HWND hWnd);
void BGM_OPEN(HWND hWnd, LPCSTR FName);
void BGM_CHECK(int sn, HWND hWnd);

#pragma endregion

#endif // !_SUB_