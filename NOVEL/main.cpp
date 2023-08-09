#define _CRT_SECURE_NO_WARNINGS
#include"Sub.h"
#include"text.h"

//�E�B���h�E�֐�
LRESULT CALLBACK WindowProc(
	HWND		hWnd,	//�E�B���h�E�n���h��
	UINT		uMsg,	//���b�Z�[�W�i�L�[�������ꂽ�Ƃ��j
	WPARAM		wParam,	//���b�Z�[�W�ɑΉ��������
	LPARAM		lParam)	//���b�Z�[�W�ɑΉ��������	
												
{	//�ϐ��̐錾
#pragma region �ϐ�
	HDC		             hdc;			      //�f�o�C�X�R���e�L�X�g�i�ȉ�DC�j
	PAINTSTRUCT          ps;			      //�`��\����
	static HDC           hMemDC;		      //�������f�o�C�X�R���e�L�X�g�i������DC�j
	static HBITMAP       hBack[Scene::Max];   //�摜�̃n���h��
	static HBITMAP       hChar[CHARACTER::CMA];
	static HBITMAP       hText[Flerm::Fmax];
	static HFONT         hFont;
	static BLENDFUNCTION Bfunk;
	static bool          Tcheck            = false;
	static bool          fullScreenMode    = false;//�t���X�N���[�����[�h
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

	DEVMODE              devmode;			//�f�o�C�X���[�h�\����
	LONG                 lResult;	        //�֐����A�l
#pragma endregion 
	
	//���b�Z�[�W�ʂ̏���
	switch (uMsg)
	{
	case WM_CREATE:
		//�������� 
		hFont = Font(15, 32, "HGP�s����");
		if (hFont == NULL)return 0;

		//text�̔���ǂݍ���
		for (int i = 0; i < (int)Flerm::Fmax; i++)
		{
			CHAR wNum[3] = { NULL };
			_itoa_s(i + 1, wNum, 10);
			CHAR wStr[64] = "�f��/�e�L�X�g/�e�L�X�g";
			strcat_s(wStr, wNum);
			strcat_s(wStr, ".bmp");
			if (ReadBmp(&hText[i], wStr) == false)return 0;
		}

		//�w�i�摜�̓ǂݍ���
		for (int i = 0; i < (int)Scene::Max; i++)
		{
			CHAR wNum[3] = {NULL};
			_itoa_s(i+1, wNum, 10);
			CHAR wStr[64] = "�f��/�w�i/�w�i";
			strcat_s(wStr,wNum);
			strcat_s(wStr, ".bmp");
			if (ReadBmp(&hBack[i],wStr) == false)return 0;
		}

		//�L�����摜�̓ǂݍ���
		for (int i = 0; i < (int)CHARACTER::CMA; i++)
		{
			CHAR wNum[3] = { NULL };
			_itoa_s(i + 1, wNum, 10);
			CHAR wStr[64] = "�f��/�L����/�L����";
			strcat_s(wStr, wNum);
			strcat_s(wStr, ".bmp");
			if (ReadBmp(&hChar[i], wStr) == false)return 0;
		}

		//���y�̓ǂݍ���
		for (int i = 0; i < (int)BGM::Ma; i++)
		{			
			CHAR wNum[3] = {NULL};//����
			_itoa_s(i+1, wNum, 10);
			CHAR filePath[64] = "open ";
			CHAR wBody[64] = "�f��/���y/BGM";
			CHAR wBody2[64] = ".mp3 alias BGM";
			strcat_s(filePath, wBody);		//"open �f��/���y/BGM"	
			strcat_s(filePath, wNum);		//"open �f��/���y/BGM(i)"	
			strcat_s(filePath, wBody2);//"open �f��/���y/BGM(i) alias BGM(i)"
			strcat_s(filePath, wNum);

			BGM_OPEN(hWnd, filePath);
		}

		//������DC�̍쐬
		hMemDC = CreateCompatibleDC(NULL);

//Alpha�l
#pragma region alpha

		Bfunk.BlendOp = AC_SRC_OVER;
		Bfunk.BlendFlags = 0;
		Bfunk.SourceConstantAlpha = 200;
		Bfunk.AlphaFormat = 0;

#pragma endregion

//�^�C�}�[�̐ݒ�
#pragma region timer

		SetTimer(hWnd,1,200,NULL);		
		SetTimer(hWnd, 2, 20, NULL);
		SetTimer(hWnd, 3, 240, NULL);

#pragma endregion

		return 0;	//WinMain�ɖ߂�
	case WM_TIMER:	//�����L�[���������Ƃ�
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
	case WM_KEYDOWN://�����L�[�������ꂽ��vertialekey
		if (data[now_no].self || opFlag||escFlag) {
			if (data[now_no].selt[2][0^2] == NULL)selmax = 1;
			else if (data[now_no].selt[3][0^2] == NULL)selmax = 2;
			else if (data[now_no].selt[4][0^2] == NULL)selmax = 3;
		}
		switch (wParam)
		{
		case VK_ESCAPE://esc�L�[
			if (!escFlag) {
				escFlag = true;
				PlaySoundA("�f��/���ʉ�/���ʉ�2.wav", NULL, SND_ASYNC | SND_FILENAME);
				back_no = now_no;
				now_no = 141;
			}
			else { PostQuitMessage(0); }
			break;
		case VK_F11://F11�L�[
			if (!fullScreenMode)
			{
				//===�t���X�N���[���\��===
				SetWindowLong(hWnd, GWL_STYLE,
					WS_POPUP);//�E�B���h�E�X�^�C����popup�ɕύX
				//�f�o�C�X���[�h�̐ݒ�
				devmode.dmSize = sizeof(DEVMODE);
				devmode.dmFields =
					DM_PELSWIDTH| DM_PELSHEIGHT;//���A�����̐ݒ������
				devmode.dmPelsWidth = WND_W;//�E�B���h�E�̕�
				devmode.dmPelsHeight = WND_H;//�E�B���h�E�̍���
				//�𑜓x�̐ݒ�
				lResult = ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);
				if (lResult != DISP_CHANGE_SUCCESSFUL) {
					MessageBox(hWnd, "�f�B�X�v���C�̐ݒ���o���܂���", NULL, MB_OK);}
				ShowWindow(hWnd, SW_SHOW);//��ʍX�V
				//�E�B���h�E�̑傫����ύX
				SetWindowPos(hWnd, HWND_TOPMOST,//�őO�ʂ̃E�B���h�E�ɂ���
					0, 0,//�n�_
					WND_W, WND_H,//�I�_�i���A�����j
					SWP_NOSIZE);//�I�v�V����
				fullScreenMode = true;
			}
			else
			{
				//===�t���X�N���[������===
				SetWindowLong(hWnd, GWL_STYLE,
					WS_OVERLAPPEDWINDOW);//�E�B���h�E�̃X�^�C����W���ɕύX
				lResult = ChangeDisplaySettings(NULL, 0);//�𑜓x��߂�
				ShowWindow(hWnd, SW_SHOW); //��ʍX�V
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);//�E�B���h�E���W��0,0�ɂ���
				fullScreenMode = false;//�E�B���h�E��Ԃ�false�ɂ���
			}
			break;
		case VK_RETURN://�G���^�[�L�[
#pragma region enter

			timecheck = false;
			wtimecheck = false;
			Tln = 0;
			sce = data[now_no];

#pragma endregion
					  //�u���b�N�A�E�g���ɃG���^�[�������Ȃ��悤��
			if (!bkFlag&&!logFlag) {     
	                                        //�V�ѕ�
			if (opFlag && playFlag)
			{
				PlaySoundA("�f��/���ʉ�/���ʉ�3.wav", NULL, SND_ASYNC | SND_FILENAME);
				now_no = 0;
				playFlag = false;
				cplayFlag = true;
			} 
                                            //��ʓI�I����
			if (Tcheck&&!opFlag&&!escFlag&&!logFlag) {
			  back_no = now_no;
			  if (data[now_no].self) {
				 PlaySoundA("�f��/���ʉ�/���ʉ�3.wav", NULL, SND_ASYNC | SND_FILENAME);
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
                                            //���j���[�E�B���h�E
			if (menuFlag&&!escFlag&&!opFlag)
			{
					if (menu_no == 2) {
						PlaySoundA("�f��/���ʉ�/���ʉ�3.wav", NULL, SND_ASYNC | SND_FILENAME);
						FILE* fp1 = fopen("xxxx.dat", "wb");
						fread(&sce, sizeof(sce), 1, fp1);
						//�\�[�g����
						fwrite(&sce, sizeof(sce), 1, fp1);
						//	���s
						fclose(fp1);
						menuFlag = false;
						menu_no = 0;
					}
					else if (menu_no == 1) {
						PlaySoundA("�f��/���ʉ�/���ʉ�3.wav", NULL, SND_ASYNC | SND_FILENAME);
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
						PlaySoundA("�f��/���ʉ�/���ʉ�3.wav", NULL, SND_ASYNC | SND_FILENAME);
						lcount = count;
					}
			}	
	                                        //�X�R�A�����N
			if (rancFlag&&!menuFlag)
			{
				back_no = now_no;
				now_no = data[now_no].next;
				rancFlag = false;
				wtimecheck = false;
				A_no = 0;
			}
	                                        //�I�[�v�j���O���
			if (opFlag&&!cplayFlag&&!playFlag)
			{
				if (cur_no == 0) {
					PlaySoundA("�f��/���ʉ�/���ʉ�3.wav", NULL, SND_ASYNC | SND_FILENAME);
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
					PlaySoundA("�f��/���ʉ�/���ʉ�3.wav", NULL, SND_ASYNC | SND_FILENAME);
					now_no = data[now_no].jampN[cur_no];
					cur_no = 0;
					logt[count] = data[now_no].td;
					opFlag = false;
				}
				else if (cur_no == 2) {
					PlaySoundA("�f��/���ʉ�/���ʉ�3.wav", NULL, SND_ASYNC | SND_FILENAME);
					back_no = now_no;
					now_no = data[now_no].jampN[cur_no];
					playFlag = true;
					cur_no = 0;
				}
			}
	                                        //�G�X�P�[�v���		
			if (escFlag) {
				if (cur_no == 0)
				{
					PlaySoundA("�f��/���ʉ�/���ʉ�3.wav", NULL, SND_ASYNC | SND_FILENAME);
					PostQuitMessage(0);
				}
				else if (cur_no == 1) {
					now_no = back_no;
					PlaySoundA("�f��/���ʉ�/���ʉ�3.wav", NULL, SND_ASYNC | SND_FILENAME);
					escFlag = false;
				}
			}
	                                        //�A���T�[���
			if (AFlag)
			{
				PlaySoundA("�f��/���ʉ�/���ʉ�3.wav", NULL, SND_ASYNC | SND_FILENAME);
				logt[count] = &data[now_no].selt[cur_no - 1][0 ^ MSGT];
				now_no = data[now_no].jampN[cur_no];
				cur_no = 0;
				count++;
				A_no++;
				AFlag = false;
			}
											//�G���f�B���O���
			if (endFlag)
			{
				PlaySoundA("�f��/���ʉ�/���ʉ�3.wav", NULL, SND_ASYNC | SND_FILENAME);
				endFlag = false;
				BGM_Close(End, hWnd);
				opFlag = true;
				now_no = 0;
			}
	                                        //��������I���I�t		
			if (!Tcheck)Tcheck = true;
			else if(Tcheck)Tcheck = false;
			}
			break;
		case VK_UP:   //�I����
			if (!bkFlag) {
				if (!logFlag) {
					if (data[now_no].self||escFlag)
					{
						PlaySoundA("�f��/���ʉ�/���ʉ�1.wav", NULL, SND_ASYNC | SND_FILENAME);
						cur_no--;
						if (cur_no < 0)cur_no = selmax;
					}
				}
				else {//���O�J�E���g
					lcount--;
					if (lcount < 1)
					{
						lcount = 1;
					}
				}
			}
			break;
		case VK_DOWN: //�I����
			if (!bkFlag) {
				if (!logFlag) {
					if (data[now_no].self||escFlag)
					{
						PlaySoundA("�f��/���ʉ�/���ʉ�1.wav", NULL, SND_ASYNC | SND_FILENAME);
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
		case VK_RIGHT:  //���j���[
			if (!bkFlag&&!opFlag&&!endFlag&&!rancFlag) {
				menu_no++;
				PlaySoundA("�f��/���ʉ�/���ʉ�1.wav", NULL, SND_ASYNC | SND_FILENAME);
				if (menu_no == 1)
				{
					menuFlag = true;
				}
				if (menu_no > 3)menu_no = 1;
			}
			break;
		case VK_LEFT:   //���j���[
		    if(!bkFlag && !opFlag && !endFlag && !rancFlag){
			menu_no--;
			PlaySoundA("�f��/���ʉ�/���ʉ�1.wav", NULL, SND_ASYNC | SND_FILENAME);
			if (menu_no == 0)
			{
				menuFlag = false;
			}
			if (menu_no <= -1)menu_no = 0;
		}
		    break;
		case VK_BACK:   //log����
			if (logFlag) {
				menu_no = 0;
				logFlag = false;
				menuFlag = false;
			}
			break;
		}
//�����ǂ̃V�[����
#pragma region �t���O
		cplayFlag = false;
		if (data[now_no].nn == 0)opFlag = true;
		if (now_no == 142)endFlag = true;
		if (now_no == 14||now_no == 35||now_no==62||now_no==88||now_no==114||now_no==130)AFlag = true;
		if (now_no == 23||now_no == 43||now_no==71||now_no==96)rancFlag = true;
		
		//BGM�؂�ւ�
		if (data[now_no].bgm != data[back_no].bgm && !bkFlag) {
			BGM_Close(data[back_no].bgm, hWnd);
			for (int i = data[back_no].bgm; i <= data[back_no].bgm; i++) {
				CHAR wNum[3] = { NULL };//����
				_itoa_s(i + 1, wNum, 10);
				CHAR filePath[64] = "open ";
				CHAR wBody[64] = "�f��/���y/BGM";
				CHAR wBody2[64] = ".mp3 alias BGM";
				strcat_s(filePath, wBody);		//"open �f��/���y/BGM"	
				strcat_s(filePath, wNum);		//"open �f��/���y/BGM(i)"	
				strcat_s(filePath, wBody2);//"open �f��/���y/BGM(i) alias BGM(i)"
				strcat_s(filePath, wNum);
				BGM_OPEN(hWnd, filePath);
			}
			bgm_no = data[now_no].bgm;
		}
		if(!escFlag)BGM_Play(data[now_no].bgm, hWnd);
#pragma endregion

		InvalidateRect(hWnd, NULL, FALSE);
	case WM_PAINT:	//�\������Ƃ��i�ŏ���Windows���X�V���ꂽ�Ƃ��j
	        hdc = BeginPaint(hWnd, &ps);//�\���J�n
		    
			//��������v�Z
			Timer %= 1;
		    if (Timer== 0)Tln++;
			if (Tln > TLN_MAX)Tln = 0;	
	
#pragma region �C�x���g

			//�I�[�v�j���O��ʏ���
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

			//�G�X�P�[�v����
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
			
		    //�t�F�[�h�C���A�t�F�[�h�A�E�g
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

			//�G���f�B���O��ʏ���
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

			//�]����ʍ쐬
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

			//��ʂ̐؂�ւ����ɏ������N����Ȃ��悤��
			if (!opFlag&&!bkFlag&&!endFlag&&!escFlag&&!rancFlag) {
				BGM_Play(data[now_no].bgm, hWnd);
				if (data[now_no].next == 1)BBlt(hdc, hMemDC, hBack[data[now_no].hk]);
				else if (data[now_no].hk != data[back_no].hk) {
					BBlt(hdc, hMemDC, hBack[data[now_no].hk]);//�w�i�̕\��
				}
				else BBlt(hdc, hMemDC, hBack[data[now_no].hk]);

				if (data[now_no].cg % 2 == 0)TBltM(hdc, hMemDC, &hChar[data[now_no].cg], 240, 100);
				else if (data[now_no].cg % 2 == 1)TBltG(hdc, hMemDC, &hChar[data[now_no].cg], 215, 110);

				TBltS(hdc, hMemDC, 610, 400, &hText[menu]); //menuicon�̐���
				MTBlt(hdc, hMemDC, &hText[main], Bfunk, MAIN_BOX_S, MAIN_BOX);//�e�L�X�g�{�b�N�X�i���C���j
				MTBlt(hdc, hMemDC, &hText[name], Bfunk, NAME_POS, NAME_BOX);//�e�L�X�g�{�b�N�X�i���C���j
			    SetBkMode(hdc, TRANSPARENT);//���̉摜�Ɠ����F�ɂȂ蓧���ɂȂ�߂��GOPAQUE�I�y�[�N
		        SelectObject(hdc, hFont);   //�t�H���g����
		        if(!AFlag)TextD(hdc, data[now_no].td,Tln,&Tcheck);
			}
			
			//�𓚂̐���
			if (AFlag)
			{
				MTBlt(hdc, hMemDC, &hBack[Black], Bfunk, BLACK_SIZE);
			}

			//�I�����̐���
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
		
			//���j���[�̓W�J
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
			
			//log�̐���
			if (logFlag&&!endFlag) {
				MTBlt(hdc, hMemDC, &hBack[Black], Bfunk,BLACK_SIZE);
						SelectObject(hdc, hFont);
						SetTextColor(hdc, RGB(255, 255, 255));
						text(hdc, 590, 10, "UP.DOWN�Ŋm�F");
						text(hdc, 590, 35, "BackSpace�ŏI��");
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

			//�\���I��
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:	//Window���I�����ꂽ�Ƃ�
		//===�I����������//
		for (int i = 0; i < Scene::Max; i++)
		{
			BGM_Close(i, hWnd);DeleteObject(hBack[i]);	//�摜�̃n���h��
		}
		DeleteDC(hMemDC);
		PostQuitMessage(0);
		return 0;	//Window�ɖ߂�
	}
	//���C���ɕԂ����
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//���C��
int WINAPI WinMain(HINSTANCE hInstance,					//�C���X�^���X�ԍ��������瓮���n�߂�
	HINSTANCE, PSTR, int)
{
	WNDCLASS  wc;										//Windows�N���X
	HWND      hWnd;										//windows�n���h��
	MSG		  msg;										//Window���b�Z�[�W
	//Windows�i�E�B���h�E�̏��j�̓o�^
	wc.style = CS_HREDRAW | CS_VREDRAW;					//�c���Ɖ����̍ĕ`��
	wc.lpfnWndProc = WindowProc;						//�E�B���h�E�Y�֐���
	wc.cbClsExtra = 0;									//�g���܂���	�g���@�\
	wc.cbWndExtra = 0;									//�g���܂���	�g���@�\
	wc.hInstance = hInstance;							//�C���X�^���X�ԍ�
	wc.hIcon = NULL;									//�A�C�R���i�f�t�H���g�ɂ���j
	wc.hCursor = NULL;									//�J�[�\���i�f�t�H���g���g�p�j
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND + 1;	//�w�i�F
	wc.lpszMenuName = NULL;								//���j���[�Ȃ�
	wc.lpszClassName = CLASS_NAME;							//�A�v���P�[�V������
	//Windows���炷��Windows�ɓo�^����
	if (RegisterClass(&wc) == false)
	{
		//�G���[���b�Z�[�W�̕\��
		MessageBox(NULL,
			"�E�B���h�E�Y�N���X�̓o�^�Ɏ��s",
			"�G���[���b�Z�[�W", MB_YESNO + MB_ICONEXCLAMATION);
		return 0;
	}
	//Windows�̍쐬
	hWnd = CreateWindow(
		CLASS_NAME,				//�A�v���P�[�V������
		TITLE_NAME,		//�^�C�g��
		WS_OVERLAPPED,	//���ʂ�Window
		100, 100,				//�\������ʒu�̂��A�����W
		WND_W,WND_H,				//Window�̕��A����
		NULL,					//�e�E�B���h�E�n���h���i�������e�Ȃ̂łȂ��j
		NULL,					//���j���[�̐ݒ�i�g��Ȃ��j
		hInstance,				//�C���X�^���X�ԍ�
		NULL);
	if (hWnd == NULL)
	{
		MessageBox(NULL,
			"�E�B���h�E�Y�̍쐬�Ɏ��s",
			"�G���[���b�Z�[�W", MB_YESNO + MB_ICONEXCLAMATION);
		return 0;
	}


	RECT rc, rw;
	GetWindowRect(hWnd, &rw);
	GetClientRect(hWnd, &rc);

	int nw = (rw.right - rw.left) - (rc.right - rc.left)+WND_W;
	int nh = (rw.bottom - rw.top) - (rc.bottom - rc.top) + WND_H;

	SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_COMPOSITED);

	SetWindowPos(hWnd, 0, 0, 0, nw, nh, SWP_NOMOVE | SWP_NOZORDER);
	//�E�B���h�E��\��
	ShowWindow(hWnd, SW_SHOW);			//SW_HIDE�B��
	//���b�Z�[�W���[�v�iWindow��������܂Ń��[�v�j

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		//Window�֐��ɑ���
		DispatchMessage(&msg);
	}
	return 0;
}