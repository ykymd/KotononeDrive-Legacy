#pragma once

#include <string>
#include <sqrat.h>
#include <Windows.h>

#ifdef UNICODE
#define tstring ::std::wstring
#define tvsnprintf  vswprintf
#else
#define tstring ::std::string
#define tvsnprintf  vsnprintf
#endif

enum INPUT_TYPE
{
	UP,     // ��{�^��/�L�[
	DOWN,   // ���{�^��/�L�[
	LEFT,   // ���{�^��/�L�[
	RIGHT,  // �E�{�^��/�L�[
	ONE,    // 1�{�^����Z�L�[
	TWO,    // 2�{�^����X�L�[
	THREE,  // 3�{�^����C�L�[
	FOUR,	// 4�{�^����V�L�[
	FIVE,	// 5�{�^��
	SIX,    // 6�{�^��
	SEVEN,	// 7�{�^��
	EIGHT,	// 8�{�^��
	ESC,		// ESC�L�[
	SPACE,	 //�X�y�[�X�L�[
};

enum SYSTEM_SE {
	ENTER,	//����
	CANCEL,	//�L�����Z��
	CURSOR,	//�J�[�\��
	DECIDE,	//�y�Ȍ���
};

extern int titleFontHandle;
extern int subFontHandle;

// SquirrelScript��ǂݍ���Ŏ��s����
extern bool OpenSquirrelFile(char* file_name);

// initialize, finalize�֐����Ăяo�����ǂ����̃t���O
extern bool g_InitializeFlag;
extern bool g_FinalizeFlag;
extern bool g_FirstSceneFlag;

// �V�[����(�e�[�u����)��\��������
//extern tstring g_SceneName;

// �V�[����؂�ւ���֐�
extern void ChangeScene(const SQChar* scene_name);

// 0��1�����݂ɃZ�b�g����ϐ��Bg_JoypadInputState�̃C���f�b�N�X�Ɏw�肵�A
// �O��̏�Ԃƍ���̏�Ԃ����݂ɕێ����邽�߂ɗ��p�����
extern int g_FlipIndex;

// �g�p�ł���L�[�̎�ނ̐�
static const int enableKeyNum = 14;

// �L�[�������ꂽ��Ԃ�ێ�����ϐ��B����̏�ԂƑO��̏�Ԃ�ێ�����
extern BOOL g_JoypadInputState[enableKeyNum][2];

// �L�[�{�[�h�̃L�[�ƃW���C�p�b�h1�̃{�^���̉�����Ԃ𒲂ׂāA�����Ɏw�肵���L�[��{�^���������ꂽ�u�Ԃ̂�true��Ԃ�
extern bool GetJoypadInputStatePushed(int input_type);

// �L�[�{�[�h�̃L�[�ƃW���C�p�b�h1�̃{�^���̉�����Ԃ𒲂ׂāA�����Ɏw�肵���L�[��{�^����������Ă����true��Ԃ�
extern bool GetJoypadInputStatePressed(int input_type);

// �摜�t�@�C����ǂݍ��ރ��b�p�[�֐�
extern int SQ_LoadGraph(const TCHAR *file_name);

// �ǂݍ��񂾉摜�t�@�C����`�悷�郉�b�p�[�֐�
extern int SQ_DrawGraph(int x, int y, int handle);

extern int SQ_DrawTurnGraph(int x, int y, int GrHandle, int TransFlag);

extern int SQ_DrawCircle(int x, int y, int r, int Color);

extern int SQ_DrawBox(int x1, int y1, int x2, int y2, int Color, int FillFlag);

extern int SQ_DrawLine(int x1, int y1, int x2, int y2, int Color);

extern int SQ_GetColor(int Red, int Green, int Blue);

extern void SQ_DrawString(int x, int y, const SQChar* str, int Color, int fontsize, int line_space_size);
extern int SQ_DrawVString(int x, int y, const SQChar* str, int Color, int FontHandle);

extern int SQ_CreateFontToHandle(const SQChar *FontName, int Size, int Thick, int FontType);

extern int SQ_DrawTitleString(int x, int y, const SQChar *str, int Color);
extern int SQ_DrawSubString(int x, int y, const SQChar *str, int Color);

extern int SQ_SetDrawBlendMode(int BlendMode, int Pal);


extern void SQ_InitSoundMem();
extern int SQ_LoadSoundMem(const SQChar* file);
extern int SQ_PlaySoundMem(int SoundHandle, int PlayType, int TopPositionFlag);
extern int SQ_StopSoundMem(int SoundHandle);
extern int SQ_DeleteSoundMem(int SoundHandle);
extern int SQ_PlaySoundFile(char *FileName, int PlayType);
extern int SQ_StopSoundFile();

extern int SQ_Exit();

// ���ʉ��̃n���h��
extern int EnterSEHandle;
extern int CancelSEHandle;

// ���ʉ���ǂݍ���
extern void SQ_LoadSE(int n);

// ���ʉ����Đ�����
extern int SQ_PlaySE(int n);