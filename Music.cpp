#include "Music.h"
#include "DxLib.h"

using namespace Kotone;
using namespace DxLib;

Music::Music()
	: BGMHandle(0)
{
}

Music::~Music()
{
}

void Music::loadBGM(std::string filename)
{
	// BGM�n���h�����擾
	auto fileURL = ".\\data\\snd\\" + filename;
	this->BGMHandle = LoadSoundMem(fileURL.c_str());
}

int Music::playBackBGM()
{
	// �ǂ݂��񂾉����m�[�}���Đ����܂�(�wPlaySoundMem�x�֐��g�p)
	return PlaySoundMem(this->BGMHandle, DX_PLAYTYPE_BACK);
}

int Music::stopBGM()
{
	// BGM���~
	return StopSoundMem(this->BGMHandle);
}