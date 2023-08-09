#pragma once
#pragma comment(lib,"winmm.lib")//音を鳴らすライブラリ
#pragma comment(lib, "Msimg32.lib")	//TBlt使用の際必要
#ifndef _SUB_
#define _SUB_

//include宣言
#include<Windows.h>
#include<wchar.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <assert.h>

//マクロ定義
#define WND_W	800 //ウィンドウの幅(width)
#define WND_H	600 //ウィンドウの高さ(height)
#define CLASS_NAME "Q&A"//アプリケーション名
#define TITLE_NAME "Q&A"

//文字送りの限度
#define TLN_MAX 100

//画像のでかさ
#define MEN_CHAR 400,740
#define GIRL_CHAR 360,1130
#define MENU_SIZE 52,55
#define BLACK_SIZE 0,0,800,600
#define SEL_FLAME 600,50

//画像の場所
#define MAIN_BOX 750,150  //memdc
#define MAIN_BOX_S 20,450 //hdc
#define NAME_BOX 200,44
#define NAME_POS 20,400

//透過色
#define TRANSPARENT_COLOR RGB(0,255,0)	

//列挙所得
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

#pragma region プロトタイプ

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