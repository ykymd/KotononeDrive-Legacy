#include "CGame.h"
#include <stdio.h>
#include "resource.h"

#define DEBUGMODE
#include "libfiles/debug.h"

#define FREE(x) { if(x) { free(x); x=NULL; } }

/////////////////////////////////////////////////////////////////////////
// �R���X�g���N�^
/////////////////////////////////////////////////////////////////////////
CGame::CGame()
{
	dwState			= G_INIT;
	fScrMulti		= 1.0f;
	llStartTime		= 0;
	llGlobalFreq	= 0;
	iImageID		= 0;
	ZeroMemory( &mBmsNum,sizeof(mBmsNum) );

	ZeroMemory( &bOnKey,sizeof(bOnKey) );
	iScratchStatus	= 0;
	ZeroMemory( &mFlashIndex,sizeof(mFlashIndex) );
	ZeroMemory( &mFlashCount,sizeof(mFlashCount) );
	ZeroMemory( &mBackKeyCount,sizeof(mBackKeyCount) );
}

/////////////////////////////////////////////////////////////////////////
// �f�X�g���N�^
/////////////////////////////////////////////////////////////////////////
CGame::~CGame()
{
	Flash();
}

///////////////////////////////////////////////////////
// ���������Q�[���`��
///////////////////////////////////////////////////////
BOOL CGame::Init( HINSTANCE hinst )
{
	// �E�C���h�E����
	win.SetWindowStyle( WS_POPUP );				// �g�����E�C���h�E()
	win.SetIcon( MAKEINTRESOURCE(IDI_ICON1) );	// �A�C�R���ݒ�
	if( !win.Create(hinst,"BMEmu2005") ) {
		DEBUG("Window create error\n","");
		return FALSE;
	}

	// Direct3D����
	// �t���X�N���[����640x480��32bit�J���[�ɃZ�b�g����B
	// ��2�ڂ̈�����FALSE�ɂ���ƃE�C���h�E���[�h�ɏo����
	if( !dd.Create(win.hWnd,FALSE,640,480,32,0,FALSE) ) {
		DEBUG( "Direct3D create error\n","" );
		return FALSE;
	}

	// DirectSound����
	if( !ds.Create(win.hWnd) ) {
		DEBUG( "DirectAudio create error\n","" );
		return FALSE;
	}

	// DirectInput����
	if( !di.Create(win.hWnd,win.hInstance) ){
		DEBUG( "DirectInput�������s\n","" );
		return FALSE;
	}

	// �L�[�{�[�h���g��
	if( !di.CreateKeyboard() ) {
		DEBUG( "�L�[�{�[�h�g�p�s��\n","" );
//		return FALSE;				// �L�[�{�[�h���g�p�ł��Ȃ��Ă��N���\�Ƃ���
	}

	// �W���C�X�e�B�b�N���g��
	if( !di.CreateJoystick() ) {
		DEBUG( "�W���C�X�e�B�b�N�g�p�s��\n","" );
//		return FALSE;				// �W���C�X�e�B�b�N���g�p�ł��Ȃ��Ă��N���\�Ƃ���
	}

	// �}�E�X�J�[�\��OFF
	win.ShowCursor( FALSE );

	return TRUE;
}

///////////////////////////////////////////////////////
// ���[�h�ς݃f�[�^�̊J��
///////////////////////////////////////////////////////
BOOL CGame::Flash( void )
{
	ds.Flash();
	dd.Clear();
	bms.Clear();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// �Q�[�����C�����[�`��
/////////////////////////////////////////////////////////////////////////
BOOL CGame::Run( HINSTANCE hinst )
{
	// �Q�[�����C�����[�v
	MSG msg;
	BOOL bLoop=TRUE;
	while( bLoop ) {
		if( PeekMessage(&msg,NULL,0,0,PM_REMOVE) ) {
			if( msg.message==WM_QUIT ) {
				bLoop = FALSE;
				DEBUG( "WM_QUIT\n","" );
				break;
			}
			TranslateMessage( &msg );
        	DispatchMessage( &msg );
		}

		// ���C���Q�[����������
		switch( dwState )
		{
		case G_INIT:
			// ������
			if( !Init(hinst) ) {
				dwState = G_END;
				break;
			}
			dwState = G_MAININIT;
			break;

		case G_MAININIT:
			// ���C���Q�[��������
			if( !GameInit() ) {
				dwState = G_END;
				break;
			}
			dwState = G_MAIN;
			break;

		case G_MAIN:
			// ���C���Q�[��
			switch( GameRun(FALSE) )
			{
			case -1:
				// ESCAPE
				dwState = G_END;
				break;
			case 1:
				// �Q�[���I��
				dwState = G_END;
				break;
			}
			break;

		case G_END:
			// �I������
			Flash();
			bLoop = FALSE;
			break;

		default:
			// ����`�̃X�e�[�g
			DEBUG( "�ُ�I��\n","" );
			return FALSE;
		}


	}

	// �}�E�X�J�[�\��ON
	win.ShowCursor( TRUE );
	win.Delete();

	// �v���O��������I��
	return TRUE;
}

///////////////////////////////////////////////////////
// ���C���Q�[��������
///////////////////////////////////////////////////////
BOOL CGame::GameInit( void )
{
	// �摜���[�h
	if( !dd.AddTexture(0,"FILES\\BM.TGA") ) {
		DEBUG( "BM.TGA load error\n","" );
		return FALSE;
	}

	// �w�i
	dd.SetPutRange( 0,0,0,0,128,480 );		// �w�i
	dd.SetPutRange( 1,0,224,0,256,256 );	// �Z���^�[�A�j���[�V����

	// ���߃��C��
	dd.SetPutRange( 2,0,292,258,126,2 );
 
	// �L�[���������Ƃ��̌��̌�
	dd.SetPutRange( 10,0,130,0,28,419 );	// ��
	dd.SetPutRange( 11,0,160,0,24,419 );	// ��
	dd.SetPutRange( 12,0,186,0,35,419 );	// ��

	// �I�u�W�F
	dd.SetPutRange( 15,0,130,422,28,6 );	// ��
	dd.SetPutRange( 16,0,160,422,24,6 );	// ��
	dd.SetPutRange( 17,0,186,422,35,6 );	// ��

	// �t���b�V��
	dd.SetPutRange( 20,0,224,258,60,50 );	// ��
	dd.SetPutRange( 21,0,224,308,52,50 );	// ��
	dd.SetPutRange( 22,0,224,358,68,50 );	// ��

	// BMS���[�h
	if( !bms.Load("SAMPLE\\SAMPLE.BMS") ) {
		DEBUG( "SAMPLE\\SAMPLE.BMS error\n","" );
		return FALSE;
	}

	// �T�E���h�f�[�^�̃��[�h
	int i,j=-1;
	char buf[MAX_PATH];
	for( i=0;i<BMSMAXBUFFER;i++ ) {
		if( bms.GetWavFile(i) ) {
			if( j==-1 )
				j = i;
			sprintf( buf,"SAMPLE\\%s",bms.GetWavFile(i) );
			ds.AddSound( i,buf );
		}
	}
	if( j!=-1 ) {
		// �L���b�V���O
		ds.SetVolume( j,0 );
		ds.Play( j );
		ds.Stop( j );
		ds.SetVolume( j,1 );
	}

	// �摜�f�[�^�̃��[�h
	for( i=0;i<256;i++ ) {
		if( bms.GetBmpFile(i) ) {
			sprintf( buf,"SAMPLE\\%s",bms.GetBmpFile(i) );
			dd.AddTexture( i+256,buf );
			dd.SetPutRange( i+256,i+256,0,0,256,256 );
		}
	}

	// �ϐ��̏�����
	iImageID		= 0;
	fScrMulti		= 1.0f;
	ZeroMemory( &mBmsNum,sizeof(mBmsNum) );

	ZeroMemory( &bOnKey,sizeof(bOnKey) );
	iScratchStatus = 0;
	ZeroMemory( &mFlashIndex,sizeof(mFlashIndex) );
	ZeroMemory( &mFlashCount,sizeof(mFlashCount) );
	ZeroMemory( &mBackKeyCount,sizeof(mBackKeyCount) );

	// �}�V���̎��g�����擾
	LARGE_INTEGER freq;
	QueryPerformanceFrequency( &freq );
	llGlobalFreq = freq.QuadPart;

	// ���݂̎��Ԃ��L�^
	LARGE_INTEGER li;
	QueryPerformanceCounter( &li );
	llStartTime = li.QuadPart;

	// �^�C�}�[������
	tm.Start( 60 );

	return TRUE;
}

///////////////////////////////////////////////////////
// �Q�[�����s���[�v
///////////////////////////////////////////////////////
HRESULT CGame::GameRun( BOOL demo )
{
	const int index[6]		= { 0,2,4,1,3,5 };				// �Q�ƃC���f�b�N�X���X�g
	const int obj_kind[6]	= { 0,1,0,1,0,2 };				// �I�u�W�F�̎��
	const float obj_x[6]	= { 1,18,30,46,59,92 };			// �I�u�W�F�\��X���W
	const float obj_fx[6]	= { 14,29,44,58,72,109 };		// �t���b�V���\��X���W

	int i,j,k;
	BYTE key[256];
	di.GetKeyboard( key );
	if( key[DIK_ESCAPE]&0x80 )
		return -1;

	// �P�ڂ̃W���C�X�e�B�b�N�̏����擾
	DIJOYSTATE2 js;
	ZeroMemory( &js,sizeof(js) );
	di.GetJoystick( &js,0 );

	// 60FPS�ł̃f�[�^����
	for( k=0;k<tm.Run();k++ ) {
		// �t���b�V����
		for( j=0;j<6;j++ ) {
			for( i=0;i<3;i++ ) {
				if( mFlashCount[j][i]>0 )
					mFlashCount[j][i] -= 2;
			}
		}

		// ���̃o�b�N���C�g���o
		for( i=0;i<6;i++ ) {
			if( mBackKeyCount[i]>0 )
				mBackKeyCount[i]--;
		}

		// �X�N���[�����ύX
		if( key[DIK_ADD]&0x80 ) {
			// �e���L�[�́{
			fScrMulti += 0.05f;
		}
		if( key[DIK_SUBTRACT]&0x80 ) {
			// �e���L�[�́|
			fScrMulti -= 0.05f;
			if( fScrMulti<0.05f )
				fScrMulti = 0.05f;
		}
	}


	// �o�߂������Ԃ���i��BMS�J�E���g�l���Z�o
	LARGE_INTEGER li;
	QueryPerformanceCounter( &li );
	LONGLONG now_count = bms.GetCountFromTime( (double)(li.QuadPart-llStartTime)/llGlobalFreq );

	// BMS�J�E���^���Ȃ̍ő�J�E���g�l(+1���ߕ�)�𒴂�����I��
	if( bms.GetMaxCount()+BMSDATA_RESOLUTION<=now_count )
		return 1;

	// BGM���P�ł��ݒ肳��Ă���ꍇ�̓^�C�~���O�ɂ��킹�čĐ�
	for( i=mBmsNum[BMS_BACKMUSIC];i<bms.GetObjeNum(BMS_BACKMUSIC);i++ ) {
		LPBMSDATA bf = bms.GetObje( BMS_BACKMUSIC,i );
		if( now_count<bf->llTime )
			break;
		if( bf->bFlag ) {
			if( now_count>=bf->llTime ) {
				bf->bFlag = FALSE;
				ds.Reset( bf->lData );
				ds.Play( bf->lData );
				mBmsNum[BMS_BACKMUSIC] = i+1;
			}
		}
	}

	// �A�j���[�V�����摜�̃Z�b�g
	for( i=mBmsNum[BMS_BACKANIME];i<bms.GetObjeNum(BMS_BACKANIME);i++ ) {
		LPBMSDATA bf = bms.GetObje( BMS_BACKANIME,i );
		if( bf->bFlag ) {
			if( now_count>=bf->llTime ) {
				bf->bFlag = FALSE;
				iImageID = bf->lData;
				mBmsNum[BMS_BACKANIME] = i+1;
			}
		}
	}

	if( demo ) {

		/////////////////////////////////////////////////////////////////////////////////////
		// �R���s���[�^�v���C
		/////////////////////////////////////////////////////////////////////////////////////
		for( j=0;j<6;j++ ) {
			for( i=mBmsNum[index[j]+11+20];i<bms.GetObjeNum(index[j]+11);i++ ) {
				LPBMSDATA bf = bms.GetObje( index[j]+11,i );
				if( now_count<bf->llTime )
					break;
				if( bf->bFlag ) {
					if( now_count>=bf->llTime ) {
						// ������
						bf->bFlag = FALSE;				// �t���O������
						ds.Reset( bf->lData );
						ds.Play( bf->lData );
						mBmsNum[index[j]+11+20] = i+1;
						// �t���b�V���J�n
						mFlashCount[index[j]][ mFlashIndex[index[j]] ] = 45;
						// ���̃C���f�b�N�X��
						mFlashIndex[index[j]]++;
						if( mFlashIndex[index[j]]>2 )
							mFlashIndex[index[j]] = 0;
						mBackKeyCount[index[j]] = 30;
					}
				}
			}
		}

	} else {

		/////////////////////////////////////////////////////////////////////////////////////
		// �l�ԃv���C
		/////////////////////////////////////////////////////////////////////////////////////
		const int keynum[6] = { DIK_Z,DIK_S,DIK_X,DIK_D,DIK_C,DIK_SPACE };

		// �S�Ă̓��͋@�킩��P�̃{�^�������Z�o
		BOOL in[7];
		ZeroMemory( &in,sizeof(in) );

		// �L�[�{�[�h���̓`�F�b�N
		if( key[DIK_Z]&0x80 )			in[0] = TRUE;		// Z
		if( key[DIK_S]&0x80 )			in[1] = TRUE;		// S
		if( key[DIK_X]&0x80 )			in[2] = TRUE;		// X
		if( key[DIK_D]&0x80 )			in[3] = TRUE;		// D
		if( key[DIK_C]&0x80 )			in[4] = TRUE;		// C
		if( key[DIK_SPACE]&0x80 )		in[5] = TRUE;		// SPACE
		if( key[DIK_LSHIFT]&0x80 )		in[6] = TRUE;		// ��SHIFT

		// �W���C�X�e�B�b�N���̓`�F�b�N
		if( js.rgbButtons[3-1]&0x80 )	in[0] = TRUE;		// ����(��)
		if( js.rgbButtons[5-1]&0x80 )	in[1] = TRUE;		// ����(��)
		if( js.rgbButtons[1-1]&0x80 )	in[2] = TRUE;		// ����(��)
		if( js.rgbButtons[7-1]&0x80 )	in[3] = TRUE;		// ����(�E)
		if( js.rgbButtons[2-1]&0x80 )	in[4] = TRUE;		// ����(�E)
		if( js.lY>500 )					in[5] = TRUE;		// �E���
		if( js.lY<-500 )				in[6] = TRUE;		// �����

		// ���z���̓n�[�h�E�F�A
		BOOL bButton[6];
		ZeroMemory( &bButton,sizeof(bButton) );
		bButton[0] = in[0];									// ����(��)
		bButton[1] = in[1];									// ����(��)
		bButton[2] = in[2];									// ����(��)
		bButton[3] = in[3];									// ����(�E)
		bButton[4] = in[4];									// ����(�E)
		// �X�N���b�`�̏���
		if( in[5] ) {
			// �E��]
			if( iScratchStatus!=1 ) {
				// �ȑO����~�������Ȃ�X�N���b�`ON
				bButton[5] = TRUE;
				iScratchStatus = 1;
			}
		} else if( in[6] ) {
			// ����]
			if( iScratchStatus!=-1 ) {
				// �ȑO����~���E���Ȃ�X�N���b�`ON
				bButton[5] = TRUE;
				iScratchStatus = -1;
			}
		} else {
			// ��]��~
			iScratchStatus = 0;
		}

		// ���͔���
		for( j=0;j<6;j++ ) {
			// �L�[��������Ă���ꍇ
			if( bButton[j] ) {
				// �L�[�����w�i�̃t�F�[�h�A�E�g�p
				mBackKeyCount[j] = 30;
				// ���łɃL�[��������Ă��Ȃ���
				if( !bOnKey[j] ) {
					// ������Ă��Ȃ������ꍇ�͉����ꂽ���Ƃɂ���
					bOnKey[j] = TRUE;
					// �����蔻��
					for( i=mBmsNum[j+11+20];i<bms.GetObjeNum(j+11);i++ ) {
						LPBMSDATA bm = (LPBMSDATA)bms.GetObje( j+11,i );
						if( bm->bFlag ) {
							// �I�u�W�F�����݂��Ă���
							if( (now_count-625)<bm->llTime && bm->llTime<(now_count+625) ) {
								// �͈͓��Œe�����Ȃ�
								bm->bFlag = FALSE;						// �I�u�W�F������
								ds.Reset( bm->lData );
								ds.Play( bm->lData );					// ���̃I�u�W�F�̉����Đ�
								// �t���b�V���摜�̒�`
								mFlashCount[j][mFlashIndex[j]] = 45;
								mFlashIndex[j]++;
								if( mFlashIndex[j]>2 )
									mFlashIndex[j] = 0;
								mBmsNum[j+11+20] = i+1;
								break;
							}
						}
					}
				} else {
					// ���łɉ�����Ă����牽�����Ȃ�
				}
			} else {
				// �L�[�������N���A
				bOnKey[j] = FALSE;
			}

			// ����������
			for( i=mBmsNum[j+11+20];i<bms.GetObjeNum(j+11);i++ ) {
				LPBMSDATA bm = (LPBMSDATA)bms.GetObje( j+11,i );
				if( bm->bFlag ) {
					// �I�u�W�F�����݂��Ă���
					if( bm->llTime<(now_count-625) ) {
						// ���������ꍇ�͂�����POOR�������s��

						// �I�u�W�F������
						bm->bFlag = FALSE;
						// ����I�u�W�F�����̎�����ɕύX
						mBmsNum[j+11+20] = i+1;
						break;
					}
				}
			}
		}
	}




	////////////////////////////////////////////////////////////////////////////////////
	// �`�揈��
	////////////////////////////////////////////////////////////////////////////////////
	dd.DrawBegin();

	// �A�j���[�V�����\��
	dd.SetRenderState( D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA );	// �ʏ퍇��
	if( iImageID>0 ) {
		// �A�j���[�V�����摜�����݂���ꍇ
		dd.SetPutStatus( iImageID+256,1,480.0f/256.0f );		// ���̉摜����ʂ����ς��Ɋg��
		dd.Put2( iImageID+256,320,240 );						// �^�񒆂ɕ\��
	} else {
		// ������΃_�~�[��\��
		dd.SetPutStatus( 1,1,480.0f/256.0f );					// ��ʂ����ς��Ɋg��
		dd.Put2( 1,320,240 );									// �^�񒆂ɕ\��
	}

	// �w�i
	dd.Put( 0,0,0 );											// �����̃��[���w�i
	dd.Put( 0,512,0 );											// �E���̃��[���w�i

	// �L�[�̃o�b�N���C�g���o
	dd.SetRenderState( D3DRS_DESTBLEND,D3DBLEND_ONE );			// ���Z����
	for( i=0;i<6;i++ ) {
		if( mBackKeyCount[i]>0 ) {
			dd.SetPutStatus( obj_kind[i]+10,(float)mBackKeyCount[i]/30.0f,1.0f,0 );		// ���X�Ƀt�F�[�h�A�E�g
			dd.Put( obj_kind[i]+10,obj_x[i],0 );										// ���[���̎�ނ��ƂɃo�b�N���C�g�摜��\��
		}
	}

	// ����
	dd.SetRenderState( D3DRS_DESTBLEND,D3DBLEND_ONE );			// ���Z����
	for( i=mBmsNum[0];i<bms.GetBarNum();i++ ) {
		LPBMSBAR bar = bms.GetBar(i);
		int yy = (int)((double)( bar->llTime - now_count ) / BMSDATA_RESOLUTION * (int)(fScrMulti * 192));
		// ���胉�C����艺�Ȃ玟�񂩂炻�̎��̏��߂���Q�Ƃ���
		if( yy<0 )
			mBmsNum[0] = i + 1;
		// ��ʂ̏���O�Ȃ�΂��̐�͑S�ĕ`��X�L�b�v
		if( yy>412+2 )
			break;
		// ��ʓ��Ȃ�`��
		dd.Put( 2,1,(float)(413-yy+5) );
	}

	// �I�u�W�F�N�g
	dd.SetRenderState( D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA );	// �ʏ퍇��
	for( j=0;j<6;j++ ) {
		int size = bms.GetObjeNum( index[j]+11 );
		for( i=mBmsNum[index[j]+11];i<size;i++ ) {
			LPBMSDATA bm = bms.GetObje( index[j]+11,i );
			int yy = (int)((double)( bm->llTime - now_count ) / BMSDATA_RESOLUTION * (int)(fScrMulti * 192));
			// ���胉�C����艺�Ȃ玟�񂩂炻�̎��̃I�u�W�F����Q�Ƃ���
			if( yy<0 )
				mBmsNum[index[j]+11] = i + 1;
			// ��ʂ̏���O�Ȃ�΂��̐�͑S�ĕ`��X�L�b�v
			if( yy>412+6 )
				break;
			// ��ʓ��Ȃ�`��
			dd.Put( 15+obj_kind[index[j]],obj_x[index[j]],(float)(413-yy) );
		}
	}

	// �t���b�V���\��
	dd.SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );			// ���Z����
	for( j=0;j<6;j++ ) {
		for( i=0;i<3;i++ ) {
			if( mFlashCount[j][i]>0 ) {
				// ���o�����݂���ꍇ�̂ݕ\��
				dd.SetPutStatus(
					obj_kind[j]+20,
					1-(float)cos( (double)mFlashCount[j][i]*2*3.14159/180 ),
					1+(float)cos( (double)mFlashCount[j][i]*2*3.14159/180 ),
					0 );
				dd.Put2( obj_kind[j]+20,obj_fx[j],416 );
			}
		}
	}

	dd.DrawEnd();

	// �p��
	return 0;
}
