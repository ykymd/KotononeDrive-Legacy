#ifndef _CGAME_H
#define _CGAME_H
////////////////////////////////////////////////////////////////////////////
// CGame : �Q�[�����C�����[�`�� v1.00
////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "./libfiles/CDDPro90.h"
#include "./libfiles/CDSPro81.h"
#include "./libfiles/CDIPro81.h"
#include "./libfiles/CWindow.h"
#include "./libfiles/CBmsPro.h"
#include "./libfiles/CTimer.h"



// �X�e�[�g
enum STATE {
	G_INIT=0,		// ���������[�h
	G_MAININIT,		// ���C�����[�`��������
	G_MAIN,			// ���C�����[�v
	G_END			// �S�ẴQ�[���I������
};



class CGame {

	// ���C�u�����Ȃ�
	CWindow			win;				// �E�C���h�E�Ǘ�
	CDDPro90		dd;					// Direct3D�Ǘ��N���X
	CDSPro81		ds;					// DirectSound�Ǘ�
	CDIPro81		di;					// DirectInput�S��
	CTimer			tm;					// 60FPS����
	CBmsPro			bms;				// BMS�t�@�C���Ǘ��p

	// �Q�[���̃��C�������Ŏg�p����ϐ�
	STATE			dwState;			// �Q�[���̃X�e�[�g��
	float			fScrMulti;			// ���ߊԂ̕��̔{��
	LONGLONG		llStartTime;		// �Q�[���J�n���̎���(���𑜓x�^�C�}�[)
	LONGLONG		llGlobalFreq;		// �}�V���̂P�b�Ԃ̃J�E���g��(���𑜓x�^�C�}�[)
	int				iImageID;			// ���ݕ\������Ă���A�j���[�V�����C���f�b�N�X�ԍ�
	int				mBmsNum[256];		// BMS���t�Ōv�Z�J�n����z��ԍ�(�������y�����邽��)

	// ���o�Ȃ�
	BOOL			bOnKey[6];			// �L�[���������܂܂�
	int				iScratchStatus;		// �X�N���b�`�̉�]����(-1=��,0=��~,1=�E)
	int				mFlashIndex[6];		// ���Ɏg�p�����t���b�V���J�E���^�̃C���f�b�N�X
	int				mFlashCount[6][3];	// �t���b�V���U���R���̃J�E���^
	int				mBackKeyCount[6];	// �L�[�𗣂������̌��̃o�b�N���C�g�̉��o�p�J�E���^

private:
	// ������
	BOOL Init( HINSTANCE hinst );		// ���������Q�[���`��
	BOOL Flash( void );					// ���[�h�ς݃f�[�^�̊J��

	// �Q�[������
	BOOL GameInit( void );				// ���C���Q�[��������
	HRESULT GameRun( BOOL demo=FALSE );	// �Q�[�����s���[�v

public:
	// ���J�֐�
	CGame();
	virtual ~CGame();
	BOOL Run( HINSTANCE hinst );		// �Q�[�����C�����[�`��
};

#endif
