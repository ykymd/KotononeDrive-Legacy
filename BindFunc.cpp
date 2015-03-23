#include "BindFunc.h"
#include <sqrat.h>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "DxLib.h"

#include "SceneController.h"

int titleFontHandle = -1;
int subFontHandle = -1;

// �e���ʉ��̃n���h��
int EnterSEHandle = 0;
int CancelSEHandle = 0;
int CursorSEHandle = 0;
int DecideSEHandle = 0;

const std::string folderParentAddress = ".\\data\\";
const std::string scriptFolderName = "Scripts\\";

// SquirrelScript��ǂݍ���Ŏ��s����
bool OpenSquirrelFile(char* file_name)
{
	try {
		// �t�H���_�p�X�ƌ�������
		std::string file_path(_SC(folderParentAddress + scriptFolderName));
		file_path += file_name;

		// �X�N���v�g���������߂̃N���X�C���X�^���X���쐬����
		Sqrat::Script script;

		// �X�N���v�g�t�@�C����ǂݍ���ŃR���p�C������
		script.CompileFile(file_path.c_str());

		// �R���p�C�������X�N���v�g�����s����
		script.Run();

		return true;
	}
	catch (const Sqrat::Exception& ex){
		// ��O���b�Z�[�W���o��
		printf("%s\n", ex.Message().data());

		return false;
	}
}

// initialize, finalize�֐����Ăяo�����ǂ����̃t���O
bool g_InitializeFlag = false;
bool g_FinalizeFlag = false;
bool g_FirstSceneFlag = true;

// �V�[����(�e�[�u����)��\��������

// �V�[����؂�ւ���֐�
void ChangeScene(const SQChar* scene_name)
{
	// initialize�֐����Ăяo���t���O�𗧂Ă�
	g_InitializeFlag = true;

	// finalize�֐��́A2��ڈȍ~�̃V�[���؂�ւ����ɂ̂݌Ăяo�����悤�ɂ���
	if (g_FirstSceneFlag)
	{
		g_FinalizeFlag = false;
		g_FirstSceneFlag = false;
	}
	else
	{
		g_FinalizeFlag = true;
	}

	// �J�ڐ�V�[���𐶐�
	auto nextScene = Scene(scene_name);

	// �V�[�������Z�b�g����
	SceneController::getInstance()->pushScene(nextScene);
}

// 0��1�����݂ɃZ�b�g����ϐ��Bg_JoypadInputState�̃C���f�b�N�X�Ɏw�肵�A
// �O��̏�Ԃƍ���̏�Ԃ����݂ɕێ����邽�߂ɗ��p�����
int g_FlipIndex = 0;

// �L�[�������ꂽ��Ԃ�ێ�����ϐ��B����̏�ԂƑO��̏�Ԃ�ێ�����
BOOL g_JoypadInputState[enableKeyNum][2];

// �L�[�{�[�h�̃L�[�ƃW���C�p�b�h1�̃{�^���̉�����Ԃ𒲂ׂāA�����Ɏw�肵���L�[��{�^���������ꂽ�u�Ԃ̂�true��Ԃ�
bool GetJoypadInputStatePushed(int input_type)
{
	// �L�[�{�[�h�̃L�[���W���C�p�b�h1�̃{�^����������Ă���A���O�񉟂���Ă��Ȃ����ǂ���
	if (g_JoypadInputState[input_type][g_FlipIndex] &&
		!g_JoypadInputState[input_type][g_FlipIndex ^ 1])
	{
		//�����ꂽ�u�ԂȂ�true��Ԃ�
		return true;
	}
	else
	{
		//�����ꂽ�u�ԂłȂ����false��Ԃ�
		return false;
	}
}

// �L�[�{�[�h�̃L�[�ƃW���C�p�b�h1�̃{�^���̉�����Ԃ𒲂ׂāA�����Ɏw�肵���L�[��{�^����������Ă����true��Ԃ�
bool GetJoypadInputStatePressed(int input_type)
{
	// �L�[�{�[�h�̃L�[���W���C�p�b�h1�̃{�^����������Ă��邩�ǂ���
	if (g_JoypadInputState[input_type][g_FlipIndex])
	{
		// ������Ă�����true��Ԃ�
		return true;
	}
	else
	{
		// ������Ă��Ȃ����false��Ԃ�
		return false;
	}
}

// �摜�t�@�C����ǂݍ��ރ��b�p�[�֐�
int SQ_LoadGraph(const TCHAR *file_name)
{
	// ��2�����Ƀf�t�H���g�����������Ă���̂ŁADxLib::LoadGraph�𒼐ړo�^�����Ƀ��b�p�[�֐���Sqrat�œo�^����
	return DxLib::LoadGraph(file_name);
}

// �ǂݍ��񂾉摜�t�@�C����`�悷�郉�b�p�[�֐�
int SQ_DrawGraph(int x, int y, int handle)
{
	// ��4����(���߃t���O)��TRUE�𖈉�w�肷��̂��ʓ|�Ȃ̂ŁA���b�p�[�֐���Sqrat�œo�^����
	return DxLib::DrawGraph(x, y, handle, TRUE);
}

// �ǂݍ��񂾉摜�t�@�C���𔽓]���ĕ`�悷�郉�b�p�[�֐�
int SQ_DrawTurnGraph(int x, int y, int GrHandle, int TransFlag)
{
	return DxLib::DrawTurnGraph(x, y, GrHandle, TransFlag);
}

int SQ_DrawCircle(int x, int y, int r, int Color)
{
	return DxLib::DrawCircle(x, y, r, Color, TRUE);
}

int SQ_DrawBox(int x1, int y1, int x2, int y2, int Color, int FillFlag)
{
	return DxLib::DrawBox(x1, y1, x2, y2, Color, FillFlag);
}

int SQ_DrawLine(int x1, int y1, int x2, int y2, int Color)
{
	return DxLib::DrawLine(x1, y1, x2, y2, Color);
}

int SQ_GetColor(int Red, int Green, int Blue)
{
	return DxLib::GetColor(Red, Green, Blue);
}

void SQ_DrawString(int x, int y, const SQChar* str, int Color, int fontsize, int line_space_size)
{
	// �t�H���g�T�C�Y���Z�b�g
	DxLib::SetFontSize(fontsize);

	// �����̍������擾
	int height = fontsize + line_space_size;

	// ���s��؂�̕�������i�[����
	std::vector<tstring> vec;
	boost::split(vec, str, boost::is_any_of(_T("\n")));

	// ���s���Ƃ�1�i���炷
	for (int i = 0, n = vec.size(); i < n; i++)
	{
		// �������`��
		DxLib::DrawString(x, y + height + i, vec[i].c_str(), Color);
	}
}

int SQ_DrawVString(int x, int y, const SQChar* str, int Color, int FontHandle)
{
	return DxLib::DrawVStringToHandle(x, y, str, Color, FontHandle);
}

int SQ_CreateFontToHandle(const SQChar *FontName, int Size, int Thick, int FontType)
{
	return DxLib::CreateFontToHandle(FontName, Size, Thick, FontType);
}

int SQ_DrawTitleString(int x, int y, const SQChar *str, int Color)
{
	std::string tmp = str;
	return DxLib::DrawVStringToHandle(x, y, ("��" + tmp).c_str(), Color, titleFontHandle);
}

int SQ_DrawSubString(int x, int y, const SQChar *str, int Color)
{
	return DxLib::DrawVStringToHandle(x, y, str, Color, subFontHandle);
}


void SQ_InitSoundMem()
{
	DxLib::InitSoundMem();
}

int SQ_LoadSoundMem(const SQChar* file)
{
	return DxLib::LoadSoundMem(file);
}

int SQ_PlaySoundMem(int SoundHandle, int PlayType, int TopPositionFlag)
{
	return DxLib::PlaySoundMem(SoundHandle, PlayType, TopPositionFlag);
}

int SQ_StopSoundMem(int SoundHandle)
{
	return DxLib::StopSoundMem(SoundHandle);
}

int SQ_DeleteSoundMem(int SoundHandle)
{
	return DxLib::DeleteSoundMem(SoundHandle);
}

int SQ_PlaySoundFile(char *FileName, int PlayType)
{
	return DxLib::PlaySoundFile(FileName, PlayType);
}

int SQ_SetDrawBlendMode(int BlendMode, int Pal)
{
	return DxLib::SetDrawBlendMode(BlendMode, Pal);
}

int SQ_StopSoundFile()
{
	return DxLib::StopSoundFile();
}

// �Q�[�����I������
int SQ_Exit()
{
	exit(0);
}

// ���ʉ�����������ɓǂݍ���
void SQ_LoadSE(int n)
{
	switch (n){
	case 0:
		if (EnterSEHandle == 0) EnterSEHandle = DxLib::LoadSoundMem(".\\data\\se\\kettei.ogg");
	case 1:
		if (CancelSEHandle == 0) CancelSEHandle = DxLib::LoadSoundMem(".\\data\\se\\back.ogg");
	case 2:
		if (CursorSEHandle == 0) CursorSEHandle = DxLib::LoadSoundMem(".\\data\\se\\cursor.mp3");
	case 3:
		if (DecideSEHandle == 0) DecideSEHandle = DxLib::LoadSoundMem(".\\data\\se\\musicdecide.ogg");
	}
}

// ���ʉ����Đ�����
int SQ_PlaySE(int n)
{
	switch (n){
	case 0:
		return DxLib::PlaySoundMem(EnterSEHandle, DX_PLAYTYPE_BACK);
	case 1:
		return DxLib::PlaySoundMem(CancelSEHandle, DX_PLAYTYPE_NORMAL);
	case 2:
		return DxLib::PlaySoundMem(CursorSEHandle, DX_PLAYTYPE_BACK);
	case 3:
		return DxLib::PlaySoundMem(DecideSEHandle, DX_PLAYTYPE_BACK);
	}

	return -1;
}
