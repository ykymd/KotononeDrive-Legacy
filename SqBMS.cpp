#include "SqBMS.h"
#include "DxLib.h"
#include <sqrat.h>

#include "Music.h"

#include <iostream>
#include <sstream>

// �^�C�g���A�R���|�[�U�[�A�W�������A�x�[�XBPM

// �Q�ƃC���f�b�N�X���X�g(���[��L1~���[��R4�܂ŏ��Ԃɒ��ׂ���悤��)
const int index[8] = {56, 51, 52, 53, 14, 15, 18, 19};

using namespace Kotone;

SqBMS::SqBMS()
{
	// BMS�Ǘ��N���X�̏�����
	bar = NULL;

	// �I�ȉ\�ȃf�[�^�̏�����
	ZeroMemory(availableSongs, sizeof(availableSongs));

	//chart = nullptr;

	// �}�V���̎��g�����擾
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	llGlobalFreq = freq.QuadPart;
}

SqBMS::~SqBMS()
{
}

void SqBMS::SQ_Play()
{
	// BGM�̍Đ�
	Music::getInstance()->loadBGM(bms.GetWavFile());

	// �ǂ݂��񂾉����m�[�}���Đ����܂�(�wPlaySoundMem�x�֐��g�p)
	Music::getInstance()->playBackBGM();

	Score = 0.0f;
	isStart = true;

	// ���݂̎��Ԃ��L�^
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	llStartTime = li.QuadPart;
}

void SqBMS::SQ_Stop()
{
	// BGM�̍Đ�
	Music::getInstance()->stopBGM();
}

bool SqBMS::SQ_Load(char* file_name)
{
	bms.Load(_SC(file_name));

	isStart = false;
	return true;
}

std::vector<std::string> SqBMS::readDir(std::string folder)
{
	// �t�@�C�����ꗗ�̃��X�g
	std::vector<std::string> fileList;

	//�t�@�C���������̂��߂Ƀ��C���h�J�[�h�ǉ�
	std::stringstream ss;
	ss << folder;
	std::string::iterator itr = folder.end();
	itr--;
	if (*itr != '\\') ss << '\\';
	ss << "*.*";

	// �t�@�C������
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(ss.str().c_str(), &fd);

	// �������s
	if (hFind == INVALID_HANDLE_VALUE)
	{
		std::cout << "�t�@�C���ꗗ���擾�ł��܂���ł���" << std::endl;
		return fileList;
	}

	// �t�@�C���������X�g�Ɋi�[����
	do{
		//�t�H���_�͏���
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		{
			// �t�@�C���������X�g�Ɋi�[
			char *file = fd.cFileName;
			std::string str = file;
			fileList.push_back(str);
		}
	} while (FindNextFile(hFind, &fd));	//���̃t�@�C��������

	// hFind�̃N���[�Y
	FindClose(hFind);

	return fileList;
}

bool SqBMS::SQ_GetSelectSceneData()
{
	// �t�@�C������
	std::string path = ".\\data\\Song\\";	//�����Ώۂ̃p�X
	std::vector<std::string> backfilelist = readDir(path);
	std::vector<std::string> multiDifficulty;	//������Փx�����Ȃ̃t�@�C����
	for (unsigned int i = 0; i < backfilelist.size(); i++){

		// �g���q���m�F���ĕ��ʃf�[�^���𒲂ׂ�
		if (backfilelist[i].substr(backfilelist[i].size() - 3, 3) == "bme")
		{
			std::cout << backfilelist[i] << std::endl;

			std::string fileName = backfilelist[i].substr(0, backfilelist[i].size() - 4);

			// ������Փx�����Ȃ������Ƃ�
			std::string tmp = backfilelist[i].substr(backfilelist[i].size() - 8, 4);
			if (tmp == "easy" || tmp == "norm" || tmp == "hard")
			{
				std::string tmp2 = backfilelist[i].substr(0, backfilelist[i].size() - 9);

				// ����
				int i;
				for (i = 0; i < songNum; i++){
					if (availableSongs[i].fileName == tmp2){
						// �����������A�����̂��̂��X�V����
						bms.UpdateSelectSceneData(path + backfilelist[i], &availableSongs[i]);

						if (songNum >= BMSMAXBUFFER) break;
						break;
					}
				}

				if (i != songNum) continue;

				// ������Ȃ��������A �V�����쐬����
				fileName = tmp2;
			}

			// ���ʃf�[�^�Ȃ�f�[�^�̊i�[���s��
			SelectSceneData data;
			data.levels[0] = 0;
			data.levels[1] = 0;
			data.levels[2] = 0;
			bms.GetSelectSceneData(path + backfilelist[i], &data);
			data.fileName = fileName;
			availableSongs[songNum++] = data;

			if (songNum >= BMSMAXBUFFER) break;
		}
	}

	return true;
}

int SqBMS::SQ_GetBarLine_Y(int num)
{
	// ���݂̎��Ԃ��L�^
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);

	LPBMSBAR bar = bms.GetBar(num);
	return (int)((double)(bar->time - bms.GetCountFromTime((double)(li.QuadPart - llStartTime) / llGlobalFreq)) / BMSDATA_RESOLUTION * (int)(bms.ScrMulti * 192));
}

long SqBMS::SQ_GetBarNum()
{
	return bms.GetBarNum();
}

int SqBMS::SQ_GetObject_X(int ch, int num)
{
	// ���݂̎��Ԃ��L�^
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	LPBMSDATA obj = NULL;
	obj = bms.GetObj(ch, num);
	if (obj != NULL)
		return (int)((double)(obj->time - bms.GetCountFromTime((double)(li.QuadPart - llStartTime) / llGlobalFreq)) / BMSDATA_RESOLUTION * (int)(bms.ScrMulti * 192));
	return 0;
}

long SqBMS::SQ_GetObjeNum(int ch)
{
	return bms.GetObjeNum(ch);
}

int SqBMS::SQ_Judge(int ch, int num)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	LONGLONG now_count = bms.GetCountFromTime((double)(li.QuadPart - llStartTime) / llGlobalFreq);
	LPBMSDATA bm = (LPBMSDATA)bms.GetObj(ch, num);

	// �I�u�W�F�N�g�����݂��Ă��邩
	if (bm->flag){
		// �قڃs�b�^���̂Ƃ�
		if ((now_count - GREAT_JUDGE_COUNT / bms.ScrMulti) < (bm->time) && (bm->time) < (now_count + GREAT_JUDGE_COUNT / bms.ScrMulti))
		{
			bm->flag = false;
			std::cout << "[" << ch << "]GREAT:" << bm->time << std::endl;
			bm->judge = Critical;
			return 4;
		}

		// ��₸��Ă���Ƃ�(2�{�̋��e����)
		if ((now_count - NEAR_JUDGE_COUNT / bms.ScrMulti) <= (bm->time) && (bm->time) <= (now_count + NEAR_JUDGE_COUNT / bms.ScrMulti))
		{
			bm->flag = false;
			std::cout << "[" << ch << "]NEAR:" << bm->time << std::endl;
			bm->judge = Near;
			return 3;
		}

		// ��������Ƃ�(2�{�ȏ㑁���Ƃ�)s
		if ((now_count + NEAR_JUDGE_COUNT / bms.ScrMulti) < (bm->time) && (now_count + 2500) > (bm->time))
		{
			bm->flag = false;
			std::cout << "[" << ch << "]FAILED(too early):" << bm->time << std::endl;
			bm->judge = Failed;
			return 2;
		}
		
		// ����ȊO(���܂�ɂ��������鎞)�ł���Ζ���
		bm->flag = true;
		bm->judge = None;
		return 1;
	}

	return 0;
}

bool SqBMS::SQ_JudgeOver(int ch, int num)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	LONGLONG now_count = bms.GetCountFromTime((double)(li.QuadPart - llStartTime) / llGlobalFreq);

	LPBMSDATA bm = (LPBMSDATA)bms.GetObj(ch, num);
	if (bm != NULL && bm->flag){
		// �I�u�W�F�N�g�����݂��Ă��邩
		if (now_count  > bm->time + NEAR_JUDGE_COUNT / bms.ScrMulti)
		{
			// �͈͊O�ł����
			bm->flag = false;
			std::cout << "[" << ch << "]FAILED(too late):" << bm->time << std::endl;
			bm->judge = Failed;
			return false;
		}
		return true;
	}

	return false;
}

// �����O�m�[�c�ɂ����Ďw�𗣂����u�Ԃ̖�������
int SqBMS::SQ_LongNoteJudge(int ch, int num)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	LONGLONG now_count = bms.GetCountFromTime((double)(li.QuadPart - llStartTime) / llGlobalFreq);
	LPBMSDATA bm1 = (LPBMSDATA)bms.GetObj(ch, num);
	LPBMSDATA bm2 = (LPBMSDATA)bms.GetObj(ch, num+1);

	// �I�u�W�F�N�g�����݂��Ă��邩
	if (bm2->flag){
		// ���e�͈͂���������������
		if (now_count > (bm1->time) && (bm2->time) > (now_count + NEAR_JUDGE_COUNT / bms.ScrMulti))
		{
			bm2->flag = false;
			std::cout << "[" << ch << "]UPFAILED:" << bm2->time << std::endl;
			bm2->judge = Failed;
			return 1;
		}

		// �������܂ܔ��胉�C����������Ǝ����I��Critical�ɂȂ�
		// �����͑��֐���
	}

	return 0;
}

bool SqBMS::SQ_LongNoteJudgeOver(int ch, int num)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	LONGLONG now_count = bms.GetCountFromTime((double)(li.QuadPart - llStartTime) / llGlobalFreq);

	if (num % 2 == 1) {
		std::cout << "��Ԗڂ̎Q��" << std::endl;
		return false;
	}

	LPBMSDATA bm1 = (LPBMSDATA)bms.GetObj(ch, num);
	LPBMSDATA bm2 = (LPBMSDATA)bms.GetObj(ch, num+1);
	if (bm1 != NULL && bm1->flag){
		// �I�u�W�F�N�g�����݂��Ă��邩
		if (now_count  > bm1->time + NEAR_JUDGE_COUNT / bms.ScrMulti)
		{
			// �͈͊O�ł����
			bm1->flag = false;
			std::cout << "[" << ch << "]FAILED(too early):" << bm1->time << std::endl;
			bm1->judge = Failed;
		}
	}

	if (bm2 != NULL && bm2->flag) {
		// �I�u�W�F�N�g�����݂��Ă��邩
		if (now_count  > bm2->time + NEAR_JUDGE_COUNT / bms.ScrMulti)
		{
			// �͈͊O�ł����
			bm2->flag = false;
			bm2->judge = Critical;
			return false;
		}
		return true;
	}

	return false;
}

// �X�R�A���擾
float SqBMS::SQ_GetScore(int ch, int num)
{
	LPBMSDATA bm = (LPBMSDATA)bms.GetObj(ch, num);
	switch (bm->judge)
	{
	case Critical:
		return 1.0f;
	case Near:
		return 0.6f;
	case Failed:
		return 0.0f;
	}
	return 0.0f;
}

int SqBMS::SQ_GetAvailableSongDifficulty(int i)
{
	int level = 0;
	for (int j = 0; j < 3; j++){
		level += availableSongs[i].levels[j];
		level *= 10;
	}

	return level;
}

bool SqBMS::SQ_isStart()
{
	return isStart;
}

// ����ŏI�����邩
bool SqBMS::SQ_isFinished()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	LONGLONG now_count = bms.GetCountFromTime((double)(li.QuadPart - llStartTime) / llGlobalFreq);

	// BMS�J�E���^���Ȃ̍ő�J�E���g�l(+1���ߕ�)�𒴂�����I��
	if (bms.GetMaxCount() + BMSDATA_RESOLUTION*2 <= now_count)
		return true;

	return false;
}

bool SqBMS::SQ_Clear(void)
{
	// �Ȗ������Z�b�g
	bms.wavData = "";
	ZeroMemory(&bms.bmsData, sizeof(LPBMSDATA));
	for (int i = 0; i<BMSMAXBUFFER; i++) {
		bms.bHeader.BPMIndex[i].Bpm = 120.0f;
		bms.bmsData[i] = NULL;		// BMS�f�[�^�̈���N���A
		bms.numBmsData[i] = 0;			// �f�[�^�̐����N���A
	}

	bms.barMagni = NULL;
	bms.numBarMagni = 0;

	bms.bmsBar = NULL;
	bms.numBmsBar = 0;

	ZeroMemory(&bms.bHeader, sizeof(BMSHEADER));
	// �f�t�H���g�Z�b�g
	bms.bHeader.Player = 1;
	bms.bHeader.bpm = 130;

	return TRUE;
}