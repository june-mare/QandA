#define _CRT_SECURE_NO_WARNINGS
#include"Sub.h"
#include"text.h"

//ウィンドウ関数
LRESULT CALLBACK WindowProc(
	HWND		hWnd,	//ウィンドウハンドル
	UINT		uMsg,	//メッセージ（キーが押されたとき）
	WPARAM		wParam,	//メッセージに対応した情報
	LPARAM		lParam)	//メッセージに対応した情報	
												
{	//変数の宣言
#pragma region 変数
	HDC		             hdc;			      //デバイスコンテキスト（以下DC）
	PAINTSTRUCT          ps;			      //描画構造体
	static HDC           hMemDC;		      //メモリデバイスコンテキスト（メモリDC）
	static HBITMAP       hBack[Scene::Max];   //画像のハンドル
	static HBITMAP       hChar[CHARACTER::CMA];
	static HBITMAP       hText[Flerm::Fmax];
	static HFONT         hFont;
	static BLENDFUNCTION Bfunk;
	static bool          Tcheck            = false;
	static bool          fullScreenMode    = false;//フルスクリーンモード
	static bool          menuFlag          = false;
	static bool          logFlag           = false;
	static bool          bkFlag            = false;
	static bool          timecheck         = false;
	static bool          wtimecheck        = false;
	static bool          opFlag            = true;
	static bool          endFlag           = false;
	static bool          escFlag           = false;
	static bool          playFlag          = false;
	static bool          cplayFlag         = false;
	static bool          checkFlag         = true;
	static bool          AFlag             = false;
	static bool          rancFlag          = false;
	static int           A_no              = 0;
	static int           cur_no            = 0;
	static int           selmax            = 0;
	static int           menu_no           = 0;
	static int           Timer             = 0;
	static int           BTimer            = 0;
	static int           ETimer            = 0;
	static int           Tln               = 0;
	static int           count             = 0;
	static int           lcount            = 0;
	static int           bk                = 0;
	static int           px                = 0;
	static int           py                = 650;
	static char*         logt[LOG]         = { NULL };
	static const char*   charname[CHARACTER::CMA] = {"bdi","ko","na","la","oo"};
	static SCE           sce;

	DEVMODE              devmode;			//デバイスモード構造体
	LONG                 lResult;	        //関数復帰値
#pragma endregion 
	
	//メッセージ別の処理
	switch (uMsg)
	{
	case WM_CREATE:
		//初期処理 
		hFont = Font(15, 32, "HGP行書体");
		if (hFont == NULL)return 0;

		//textの箱を読み込み
		for (int i = 0; i < (int)Flerm::Fmax; i++)
		{
			CHAR wNum[3] = { NULL };
			_itoa_s(i + 1, wNum, 10);
			CHAR wStr[64] = "素材/テキスト/テキスト";
			strcat_s(wStr, wNum);
			strcat_s(wStr, ".bmp");
			if (ReadBmp(&hText[i], wStr) == false)return 0;
		}

		//背景画像の読み込み
		for (int i = 0; i < (int)Scene::Max; i++)
		{
			CHAR wNum[3] = {NULL};
			_itoa_s(i+1, wNum, 10);
			CHAR wStr[64] = "素材/背景/背景";
			strcat_s(wStr,wNum);
			strcat_s(wStr, ".bmp");
			if (ReadBmp(&hBack[i],wStr) == false)return 0;
		}

		//キャラ画像の読み込み
		for (int i = 0; i < (int)CHARACTER::CMA; i++)
		{
			CHAR wNum[3] = { NULL };
			_itoa_s(i + 1, wNum, 10);
			CHAR wStr[64] = "素材/キャラ/キャラ";
			strcat_s(wStr, wNum);
			strcat_s(wStr, ".bmp");
			if (ReadBmp(&hChar[i], wStr) == false)return 0;
		}

		//音楽の読み込み
		for (int i = 0; i < (int)BGM::Ma; i++)
		{			
			CHAR wNum[3] = {NULL};//桁数
			_itoa_s(i+1, wNum, 10);
			CHAR filePath[64] = "open ";
			CHAR wBody[64] = "素材/音楽/BGM";
			CHAR wBody2[64] = ".mp3 alias BGM";
			strcat_s(filePath, wBody);		//"open 素材/音楽/BGM"	
			strcat_s(filePath, wNum);		//"open 素材/音楽/BGM(i)"	
			strcat_s(filePath, wBody2);//"open 素材/音楽/BGM(i) alias BGM(i)"
			strcat_s(filePath, wNum);

			BGM_OPEN(hWnd, filePath);
		}

		//メモリDCの作成
		hMemDC = CreateCompatibleDC(NULL);

//Alpha値
#pragma region alpha

		Bfunk.BlendOp = AC_SRC_OVER;
		Bfunk.BlendFlags = 0;
		Bfunk.SourceConstantAlpha = 200;
		Bfunk.AlphaFormat = 0;

#pragma endregion

//タイマーの設定
#pragma region timer

		SetTimer(hWnd,1,200,NULL);		
		SetTimer(hWnd, 2, 20, NULL);
		SetTimer(hWnd, 3, 240, NULL);

#pragma endregion

		return 0;	//WinMainに戻る
	case WM_TIMER:	//何かキーを押したとき
		switch (wParam)
		{
		case 1:
			if (!bkFlag) {
				BGM_CHECK(data[now_no].bgm, hWnd);
				Timer++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case 2:
			if (bkFlag) {
				BTimer++;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case 3:
			if (endFlag) {
				ETimer++;
				InvalidateRect(hWnd, NULL, TRUE);
				if (py < -774) {
					endFlag = false;
					BGM_Close(End, hWnd);
					opFlag = true;
					now_no = 0;
				}
			}
			break;
		}
		
		return 0;
	case WM_KEYDOWN://何かキーが押された時vertialekey
		if (data[now_no].self || opFlag||escFlag) {
			if (data[now_no].selt[2][0^2] == NULL)selmax = 1;
			else if (data[now_no].selt[3][0^2] == NULL)selmax = 2;
			else if (data[now_no].selt[4][0^2] == NULL)selmax = 3;
		}
		switch (wParam)
		{
		case VK_ESCAPE://escキー
			if (!escFlag) {
				escFlag = true;
				PlaySoundA("素材/効果音/効果音2.wav", NULL, SND_ASYNC | SND_FILENAME);
				back_no = now_no;
				now_no = 141;
			}
			else { PostQuitMessage(0); }
			break;
		case VK_F11://F11キー
			if (!fullScreenMode)
			{
				//===フルスクリーン表示===
				SetWindowLong(hWnd, GWL_STYLE,
					WS_POPUP);//ウィンドウスタイルをpopupに変更
				//デバイスモードの設定
				devmode.dmSize = sizeof(DEVMODE);
				devmode.dmFields =
					DM_PELSWIDTH| DM_PELSHEIGHT;//幅、高さの設定をする
				devmode.dmPelsWidth = WND_W;//ウィンドウの幅
				devmode.dmPelsHeight = WND_H;//ウィンドウの高さ
				//解像度の設定
				lResult = ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);
				if (lResult != DISP_CHANGE_SUCCESSFUL) {
					MessageBox(hWnd, "ディスプレイの設定を出来ません", NULL, MB_OK);}
				ShowWindow(hWnd, SW_SHOW);//画面更新
				//ウィンドウの大きさを変更
				SetWindowPos(hWnd, HWND_TOPMOST,//最前面のウィンドウにする
					0, 0,//始点
					WND_W, WND_H,//終点（幅、高さ）
					SWP_NOSIZE);//オプション
				fullScreenMode = true;
			}
			else
			{
				//===フルスクリーン解除===
				SetWindowLong(hWnd, GWL_STYLE,
					WS_OVERLAPPEDWINDOW);//ウィンドウのスタイルを標準に変更
				lResult = ChangeDisplaySettings(NULL, 0);//解像度を戻す
				ShowWindow(hWnd, SW_SHOW); //画面更新
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);//ウィンドウ座標を0,0にする
				fullScreenMode = false;//ウィンドウ状態をfalseにする
			}
			break;
		case VK_RETURN://エンターキー
#pragma region enter

			timecheck = false;
			wtimecheck = false;
			Tln = 0;
			sce = data[now_no];

#pragma endregion
					  //ブラックアウト中にエンターを押せないように
			if (!bkFlag&&!logFlag) {     
	                                        //遊び方
			if (opFlag && playFlag)
			{
				PlaySoundA("素材/効果音/効果音3.wav", NULL, SND_ASYNC | SND_FILENAME);
				now_no = 0;
				playFlag = false;
				cplayFlag = true;
			} 
                                            //一般的選択肢
			if (Tcheck&&!opFlag&&!escFlag&&!logFlag) {
			  back_no = now_no;
			  if (data[now_no].self) {
				 PlaySoundA("素材/効果音/効果音3.wav", NULL, SND_ASYNC | SND_FILENAME);
				logt[count] = &data[now_no].selt[cur_no][0 ^ MSGT];
				now_no = data[now_no].jampN[cur_no];
				count++;
				cur_no = 0;
				}
			  else {
					now_no = data[now_no].next;
					logt[count] = data[now_no].td;
					count++;
				}
			  if (now_no == -1)now_no = 0;
			}
                                            //メニューウィンドウ
			if (menuFlag&&!escFlag&&!opFlag)
			{
					if (menu_no == 2) {
						PlaySoundA("素材/効果音/効果音3.wav", NULL, SND_ASYNC | SND_FILENAME);
						FILE* fp1 = fopen("xxxx.dat", "wb");
						fread(&sce, sizeof(sce), 1, fp1);
						//ソート処理
						fwrite(&sce, sizeof(sce), 1, fp1);
						//	改行
						fclose(fp1);
						menuFlag = false;
						menu_no = 0;
					}
					else if (menu_no == 1) {
						PlaySoundA("素材/効果音/効果音3.wav", NULL, SND_ASYNC | SND_FILENAME);
						FILE* fp1 = fopen("xxxx.dat", "rb");
						fread(&sce, sizeof(sce), 1, fp1);
						fclose(fp1);
						now_no = sce.nn;
						BGM_Play(data[now_no].bgm, hWnd);
						menuFlag = false;
						menu_no = 0;
					}
					else if (menu_no == 3)
					{
						logFlag = true;
						PlaySoundA("素材/効果音/効果音3.wav", NULL, SND_ASYNC | SND_FILENAME);
						lcount = count;
					}
			}	
	                                        //スコアランク
			if (rancFlag&&!menuFlag)
			{
				back_no = now_no;
				now_no = data[now_no].next;
				rancFlag = false;
				wtimecheck = false;
				A_no = 0;
			}
	                                        //オープニング画面
			if (opFlag&&!cplayFlag&&!playFlag)
			{
				if (cur_no == 0) {
					PlaySoundA("素材/効果音/効果音3.wav", NULL, SND_ASYNC | SND_FILENAME);
					FILE* fp1 = fopen("xxxx.dat", "rb");
					fread(&sce, sizeof(sce), 1, fp1);
					fclose(fp1);
					now_no = sce.nn;
					BGM_Play(data[now_no].bgm, hWnd);
					cur_no = 0;
					opFlag = false;
				}
				else if (cur_no == 1) {
					back_no = now_no;
					PlaySoundA("素材/効果音/効果音3.wav", NULL, SND_ASYNC | SND_FILENAME);
					now_no = data[now_no].jampN[cur_no];
					cur_no = 0;
					logt[count] = data[now_no].td;
					opFlag = false;
				}
				else if (cur_no == 2) {
					PlaySoundA("素材/効果音/効果音3.wav", NULL, SND_ASYNC | SND_FILENAME);
					back_no = now_no;
					now_no = data[now_no].jampN[cur_no];
					playFlag = true;
					cur_no = 0;
				}
			}
	                                        //エスケープ画面		
			if (escFlag) {
				if (cur_no == 0)
				{
					PlaySoundA("素材/効果音/効果音3.wav", NULL, SND_ASYNC | SND_FILENAME);
					PostQuitMessage(0);
				}
				else if (cur_no == 1) {
					now_no = back_no;
					PlaySoundA("素材/効果音/効果音3.wav", NULL, SND_ASYNC | SND_FILENAME);
					escFlag = false;
				}
			}
	                                        //アンサー画面
			if (AFlag)
			{
				PlaySoundA("素材/効果音/効果音3.wav", NULL, SND_ASYNC | SND_FILENAME);
				logt[count] = &data[now_no].selt[cur_no - 1][0 ^ MSGT];
				now_no = data[now_no].jampN[cur_no];
				cur_no = 0;
				count++;
				A_no++;
				AFlag = false;
			}
											//エンディング画面
			if (endFlag)
			{
				PlaySoundA("素材/効果音/効果音3.wav", NULL, SND_ASYNC | SND_FILENAME);
				endFlag = false;
				BGM_Close(End, hWnd);
				opFlag = true;
				now_no = 0;
			}
	                                        //文字送りオンオフ		
			if (!Tcheck)Tcheck = true;
			else if(Tcheck)Tcheck = false;
			}
			break;
		case VK_UP:   //選択肢
			if (!bkFlag) {
				if (!logFlag) {
					if (data[now_no].self||escFlag)
					{
						PlaySoundA("素材/効果音/効果音1.wav", NULL, SND_ASYNC | SND_FILENAME);
						cur_no--;
						if (cur_no < 0)cur_no = selmax;
					}
				}
				else {//ログカウント
					lcount--;
					if (lcount < 1)
					{
						lcount = 1;
					}
				}
			}
			break;
		case VK_DOWN: //選択肢
			if (!bkFlag) {
				if (!logFlag) {
					if (data[now_no].self||escFlag)
					{
						PlaySoundA("素材/効果音/効果音1.wav", NULL, SND_ASYNC | SND_FILENAME);
						cur_no++;
						if (cur_no > selmax)cur_no = 0;
					}
					
				}
				else {
					lcount++;
					if (lcount > count)
					{
						lcount = count;
					}
				}
			}
				break;
		case VK_RIGHT:  //メニュー
			if (!bkFlag&&!opFlag&&!endFlag&&!rancFlag) {
				menu_no++;
				PlaySoundA("素材/効果音/効果音1.wav", NULL, SND_ASYNC | SND_FILENAME);
				if (menu_no == 1)
				{
					menuFlag = true;
				}
				if (menu_no > 3)menu_no = 1;
			}
			break;
		case VK_LEFT:   //メニュー
		    if(!bkFlag && !opFlag && !endFlag && !rancFlag){
			menu_no--;
			PlaySoundA("素材/効果音/効果音1.wav", NULL, SND_ASYNC | SND_FILENAME);
			if (menu_no == 0)
			{
				menuFlag = false;
			}
			if (menu_no <= -1)menu_no = 0;
		}
		    break;
		case VK_BACK:   //log解除
			if (logFlag) {
				menu_no = 0;
				logFlag = false;
				menuFlag = false;
			}
			break;
		}
//今がどのシーンか
#pragma region フラグ
		cplayFlag = false;
		if (data[now_no].nn == 0)opFlag = true;
		if (now_no == 142)endFlag = true;
		if (now_no == 14||now_no == 35||now_no==62||now_no==88||now_no==114||now_no==130)AFlag = true;
		if (now_no == 23||now_no == 43||now_no==71||now_no==96)rancFlag = true;
		
		//BGM切り替え
		if (data[now_no].bgm != data[back_no].bgm && !bkFlag) {
			BGM_Close(data[back_no].bgm, hWnd);
			for (int i = data[back_no].bgm; i <= data[back_no].bgm; i++) {
				CHAR wNum[3] = { NULL };//桁数
				_itoa_s(i + 1, wNum, 10);
				CHAR filePath[64] = "open ";
				CHAR wBody[64] = "素材/音楽/BGM";
				CHAR wBody2[64] = ".mp3 alias BGM";
				strcat_s(filePath, wBody);		//"open 素材/音楽/BGM"	
				strcat_s(filePath, wNum);		//"open 素材/音楽/BGM(i)"	
				strcat_s(filePath, wBody2);//"open 素材/音楽/BGM(i) alias BGM(i)"
				strcat_s(filePath, wNum);
				BGM_OPEN(hWnd, filePath);
			}
			bgm_no = data[now_no].bgm;
		}
		if(!escFlag)BGM_Play(data[now_no].bgm, hWnd);
#pragma endregion

		InvalidateRect(hWnd, NULL, FALSE);
	case WM_PAINT:	//表示するとき（最初とWindowsが更新されたとき）
	        hdc = BeginPaint(hWnd, &ps);//表示開始
		    
			//文字送り計算
			Timer %= 1;
		    if (Timer== 0)Tln++;
			if (Tln > TLN_MAX)Tln = 0;	
	
#pragma region イベント

			//オープニング画面処理
			if (opFlag&&checkFlag) {
				BBlt(hdc, hMemDC, hBack[data[now_no].hk]);
				for (int i = 0; i < SELMSG; i++) {
					if (i == cur_no) {
						if (data[now_no].selt[i][2] == NULL) {
							break;
						}
						MTBlt(hdc, hMemDC, &hText[sentaku], Bfunk, 100, 380 + i * 60, SEL_FLAME);
						SetBkMode(hdc, TRANSPARENT);
					}
					else {
						if (data[now_no].selt[i][2] == NULL) {
							break;
						}
						MTBlt(hdc, hMemDC, &hText[selv], Bfunk, 100, 380 + i * 60, SEL_FLAME);
						SetBkMode(hdc, TRANSPARENT);
					}
					SelectObject(hdc, hFont);
					if (data[now_no].selt[i][2] == NULL)break;
					text(hdc, 140, 390 + i * 60, data[now_no].selt[i]);
				}
			}

			//エスケープ処理
			if(data[now_no].hk==endg&&escFlag){
				BBlt(hdc, hMemDC, hBack[data[now_no].hk]);
				for (int i = 0; i < SELMSG; i++) {
					if (i == cur_no) {
						if (data[now_no].selt[i][2] == NULL) {
							break;
						}
						MTBlt(hdc, hMemDC, &hText[sentaku], Bfunk, 100, 380 + i * 60, SEL_FLAME);
						SetBkMode(hdc, TRANSPARENT);
					}
					else {
						if (data[now_no].selt[i][2] == NULL) {
							break;
						}
						MTBlt(hdc, hMemDC, &hText[selv], Bfunk, 100, 380 + i * 60, SEL_FLAME);
						SetBkMode(hdc, TRANSPARENT);
					}
					SelectObject(hdc, hFont);
					if (data[now_no].selt[i][2] == NULL)break;
					text(hdc, 370, 390 + i * 60, data[now_no].selt[i]);
				}
			}
			
		    //フェードイン、フェードアウト
			if (!playFlag&&!opFlag&&data[now_no].hk != data[back_no].hk && !timecheck&&!escFlag) {
				BO(hdc, hMemDC, bk, &bkFlag, &hBack[Black]);
				if (!endFlag) {
					if (BTimer % 1 == 0) {
						bk += 15;
						if (bk >= 255)timecheck = true;
					}
				}
				else if(endFlag)
				{
					if (ETimer % 1 == 0) {
						bk += 15;
						if (bk >= 255)timecheck = true;
					}
				}
			}
			if (data[now_no].hk != data[back_no].hk && timecheck&&!wtimecheck) {
				WO(hdc, hMemDC, bk, &bkFlag, &hBack[Black]);
				if (!endFlag) {
					if (BTimer % 1 == 0) {
						bk -= 51;
						if (bk < 16) {
							bkFlag = false;
							wtimecheck = true;
						}
					}
				}
				else if (endFlag)
				{
					if (ETimer % 1 == 0) {
						bk -= 51;
						if (bk < 16) {
							bkFlag = false;
							wtimecheck = true;
						}
					}
				}
			}	

			//エンディング画面処理
			if (data[now_no].hk == Black && endFlag) {
				ETimer %= 1;
				if (ETimer == 0)py -= 2; 
				for (int i = 0; i < ENDMSGT; i++)
				{
					SetBkMode(hdc, TRANSPARENT);
					SelectObject(hdc, hFont);
					text(hdc, (WND_W - 15 * (strlen(endroll[i].e))) / 2, py + (30 * i),endroll[i].e);
				}
			}

			//評価画面作成
			if (rancFlag)
			{
				if (A_no == 1) {
					data[now_no].hk = rancs;
					BBlt(hdc, hMemDC, hBack[rancs]);
				}
				else if (A_no == 2)
				{
					data[now_no].hk = ranca;
					BBlt(hdc, hMemDC, hBack[ranca]);
				}
				else if (A_no == 3)
				{
					data[now_no].hk = rancb;
					BBlt(hdc, hMemDC, hBack[rancb]);
				}
				else if (A_no > 3)
				{
					data[now_no].hk = ranc;
					BBlt(hdc, hMemDC, hBack[ranc]);
				}
			}

			//画面の切り替え中に処理が起こらないように
			if (!opFlag&&!bkFlag&&!endFlag&&!escFlag&&!rancFlag) {
				BGM_Play(data[now_no].bgm, hWnd);
				if (data[now_no].next == 1)BBlt(hdc, hMemDC, hBack[data[now_no].hk]);
				else if (data[now_no].hk != data[back_no].hk) {
					BBlt(hdc, hMemDC, hBack[data[now_no].hk]);//背景の表示
				}
				else BBlt(hdc, hMemDC, hBack[data[now_no].hk]);

				if (data[now_no].cg % 2 == 0)TBltM(hdc, hMemDC, &hChar[data[now_no].cg], 240, 100);
				else if (data[now_no].cg % 2 == 1)TBltG(hdc, hMemDC, &hChar[data[now_no].cg], 215, 110);

				TBltS(hdc, hMemDC, 610, 400, &hText[menu]); //menuiconの生成
				MTBlt(hdc, hMemDC, &hText[main], Bfunk, MAIN_BOX_S, MAIN_BOX);//テキストボックス（メイン）
				MTBlt(hdc, hMemDC, &hText[name], Bfunk, NAME_POS, NAME_BOX);//テキストボックス（メイン）
			    SetBkMode(hdc, TRANSPARENT);//元の画像と同じ色になり透明になる戻す；OPAQUEオペーク
		        SelectObject(hdc, hFont);   //フォント生成
		        if(!AFlag)TextD(hdc, data[now_no].td,Tln,&Tcheck);
			}
			
			//解答の生成
			if (AFlag)
			{
				MTBlt(hdc, hMemDC, &hBack[Black], Bfunk, BLACK_SIZE);
			}

			//選択肢の生成
			if (data[now_no].self&&!opFlag&&!endFlag) {
				for (int i = 0; i < SELMSG; i++) {
					if (i == cur_no) {
						if (data[now_no].selt[i][2] == NULL) {
							break;
						}
						MTBlt(hdc, hMemDC, &hText[sentaku], Bfunk, 100, 100 + i * 60, SEL_FLAME);
						SetBkMode(hdc, TRANSPARENT);
					}
					else {
						if (data[now_no].selt[i][2] == NULL) {
							break;
						}
						MTBlt(hdc, hMemDC, &hText[selv], Bfunk, 100, 100 + i * 60, SEL_FLAME);
						SetBkMode(hdc, TRANSPARENT);
					}
					SelectObject(hdc, hFont);
					if (data[now_no].selt[i][2] == NULL)break;
					text(hdc, 140, 110 + i * 60, data[now_no].selt[i]);
				}
			}
		
			//メニューの展開
			if (menuFlag&&!opFlag&&!endFlag&&!AFlag) {
				for (int i = 4; i < 11; i+=2) {
					if (i == 4) {
						TBltS(hdc, hMemDC, 610 , 400, &hText[i]);
						SetBkMode(hdc, TRANSPARENT);
					}
					else if (i ==4+ menu_no*2) {
						TBltS(hdc, hMemDC, 445 +((i/2)*55) , 400, & hText[i + 1]);
						SetBkMode(hdc, TRANSPARENT);
					}
					else {
						TBltS(hdc, hMemDC, 445 + ((i / 2) * 55), 400, &hText[i]);
						SetBkMode(hdc, TRANSPARENT);
					}
				}
			}  
			
			//logの生成
			if (logFlag&&!endFlag) {
				MTBlt(hdc, hMemDC, &hBack[Black], Bfunk,BLACK_SIZE);
						SelectObject(hdc, hFont);
						SetTextColor(hdc, RGB(255, 255, 255));
						text(hdc, 590, 10, "UP.DOWNで確認");
						text(hdc, 590, 35, "BackSpaceで終了");
					for (int i = 0; i <= count;i++)
					{
						if (i>12) {
							LogTextD(hdc,logt[lcount-1], ((lcount%12) * 50));
						}
						else if (i == 0)
						{
							LogTextD(hdc, logt[lcount - 1], (lcount * 50));
						}
						else {
							LogTextD(hdc, logt[lcount - 1], (lcount * 50));
						}
					}
			} 

#pragma endregion

			//表示終了
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:	//Windowが終了されたとき
		//===終了処理＝＝//
		for (int i = 0; i < Scene::Max; i++)
		{
			BGM_Close(i, hWnd);DeleteObject(hBack[i]);	//画像のハンドル
		}
		DeleteDC(hMemDC);
		PostQuitMessage(0);
		return 0;	//Windowに戻る
	}
	//メインに返す情報
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//メイン
int WINAPI WinMain(HINSTANCE hInstance,					//インスタンス番号こいつから動き始める
	HINSTANCE, PSTR, int)
{
	WNDCLASS  wc;										//Windowsクラス
	HWND      hWnd;										//windowsハンドル
	MSG		  msg;										//Windowメッセージ
	//Windows（ウィンドウの情報）の登録
	wc.style = CS_HREDRAW | CS_VREDRAW;					//縦軸と横軸の再描画
	wc.lpfnWndProc = WindowProc;						//ウィンドウズ関数名
	wc.cbClsExtra = 0;									//使いません	拡張機能
	wc.cbWndExtra = 0;									//使いません	拡張機能
	wc.hInstance = hInstance;							//インスタンス番号
	wc.hIcon = NULL;									//アイコン（デフォルトにする）
	wc.hCursor = NULL;									//カーソル（デフォルトを使用）
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND + 1;	//背景色
	wc.lpszMenuName = NULL;								//メニューなし
	wc.lpszClassName = CLASS_NAME;							//アプリケーション名
	//WindowsくらすをWindowsに登録する
	if (RegisterClass(&wc) == false)
	{
		//エラーメッセージの表示
		MessageBox(NULL,
			"ウィンドウズクラスの登録に失敗",
			"エラーメッセージ", MB_YESNO + MB_ICONEXCLAMATION);
		return 0;
	}
	//Windowsの作成
	hWnd = CreateWindow(
		CLASS_NAME,				//アプリケーション名
		TITLE_NAME,		//タイトル
		WS_OVERLAPPED,	//普通のWindow
		100, 100,				//表示する位置のｘ、ｙ座標
		WND_W,WND_H,				//Windowの幅、高さ
		NULL,					//親ウィンドウハンドル（自分が親なのでなし）
		NULL,					//メニューの設定（使わない）
		hInstance,				//インスタンス番号
		NULL);
	if (hWnd == NULL)
	{
		MessageBox(NULL,
			"ウィンドウズの作成に失敗",
			"エラーメッセージ", MB_YESNO + MB_ICONEXCLAMATION);
		return 0;
	}


	RECT rc, rw;
	GetWindowRect(hWnd, &rw);
	GetClientRect(hWnd, &rc);

	int nw = (rw.right - rw.left) - (rc.right - rc.left)+WND_W;
	int nh = (rw.bottom - rw.top) - (rc.bottom - rc.top) + WND_H;

	SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_COMPOSITED);

	SetWindowPos(hWnd, 0, 0, 0, nw, nh, SWP_NOMOVE | SWP_NOZORDER);
	//ウィンドウを表示
	ShowWindow(hWnd, SW_SHOW);			//SW_HIDE隠す
	//メッセージループ（Windowが閉じられるまでループ）

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		//Window関数に送る
		DispatchMessage(&msg);
	}
	return 0;
}