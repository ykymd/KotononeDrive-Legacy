#include "CBMS.h"
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <string>
#include "DxLib.h"

// ����(�`�����l���ԍ�)
// ���[��1~4�̓����O�m�[�c�I�����[�Ȃ̂Ń`�����l���ԍ���50�ԑ�
// ���[��1:56
// ���[��2:51
// ���[��3:52
// ���[��4:53
//
// ���[��5:14
// ���[��6:15
// ���[��7:18
// ���[��8:19

#define FREE(x)		{ if(x) {free(x); x=NULL;} }

using namespace Kotone;

CBMS::CBMS()
{
	bHeader.Player = 1;
	bHeader.bpm = 120;

	bmsBar = NULL;
	numBmsBar = 0;

	barMagni = NULL;
	numBarMagni = 0;

	ZeroMemory(&bHeader, sizeof(bHeader));
	ZeroMemory(&bmsData, sizeof(bmsData));
	ZeroMemory(&numBmsData, sizeof(numBmsData));
	ZeroMemory(&bmsBar, sizeof(bmsBar));
	ZeroMemory(&barMagni, sizeof(barMagni));
}


CBMS::~CBMS()
{
	for (int i = 0; i<BMSMAXBUFFER; i++) {
		FREE(bmsData[i]);		// BMS�f�[�^�̈���N���A
		numBmsData[i] = 0;			// �f�[�^�̐����N���A
	}
}

Command CBMS::GetCommand(const std::string cmd)
{
	const char *command[15] = {
		"PLAYER\0",
		"GENRE\0",
		"TITLE\0",
		"ARTIST\0",
		"BPM\0",
		"MIDIFILE\0",
		"PLAYLEVEL\0",
		"RANK\0",
		"VOLWAV\0",
		"TOTAL\0",
		"StageFile\0",
		"WAV\0",
		"BMP\0",
		"GUID\0",
		"MOVIE",
	};

	// �������[�`��
	int i;
	for (i = 0; i<15; i++) {
		if (_strnicmp(cmd.data()+1, command[i], strlen(command[i])) == 0)
			return Command(i);	// �R�}���h�Ȃ炻�̔ԍ���Ԃ�
	}

	// �擪��'#iiiii'��
	BOOL obj = TRUE;
	for (i = 0; i<5; i++) {
		if (cmd[i+1]<'0' || cmd[i+1]>'9') {
			obj = FALSE;
			break;
		}
	}

	// �I�u�W�F�z�u������
	if (obj) return OBJECT;

	// ���ʕs�\�����Ȃ�
	return UNKNOWN;
}

// �t�@�C���A�h���X����f�[�^�����[�h����
bool CBMS::Load( LPSTR file )
{
	if (!GetHeader(file)){
		std::cout << "�w�b�_�ǂݎ��G���[" << std::endl;
		return false;
	}
	std::cout << "�w�b�_�ǂݎ�萬���I" << std::endl;

	if (!GetData(file)){
		std::cout << "�f�[�^�ǂݎ��G���[" << std::endl;
		return false;
	}
	std::cout << "�f�[�^�ǂݎ�萬���I" << std::endl;

	return true;
}

bool CBMS::GetCommandString(std::string src, std::string* dst)
{
	int i = 0;

	// �f�[�^�����܂ł̃|�C���^���擾
	while (true){
		if (src[i] == ' ' || src[i] == '�@' || src[i] == ':'){
			i++;
			break;
		}
		else if (src[i] == '\n' || src[i] == NULL){
			return false;
		}
		i++;
	}

	*dst = src.substr(i).c_str();

	return true;
}

// �m�[�c�̃f�[�^���L���[�Ɋi�[����
bool CBMS::AddNoteData(unsigned int ch_num, long long startpos, long dat)
{
	// �`�����l���ԍ���0~255�ȓ����`�F�b�N
	if (ch_num & ~255) {
		std::cout << "�`�����l����:" << ch_num << "�̓T�|�[�g�O�ł�(0~255)" << std::endl;
		return false;
	}

	if (ch_num == BMS_STRETCH) return false;

	if (dat == 0) return true;

	switch (ch_num)
	{
	case BMS_BPMINDEX:
		numBmsData[BMS_TEMPO]++;
		bmsData[BMS_TEMPO] = (LPBMSDATA)realloc(bmsData[BMS_TEMPO], numBmsData[BMS_TEMPO] * sizeof(BMSDATA));
		ZeroMemory(&bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1], sizeof(BMSDATA));
		//bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1] = 0;
		bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1].flag = true;
		bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1].time = startpos;
		bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1].data = (long)bHeader.BPMIndex[dat].Bpm;
		bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1].fData = bHeader.BPMIndex[dat].Bpm;
		bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1].judge = None;		//����͖���������f�t�H�Ƃ���
		break;
	default:
		// �f�[�^��ǉ�
		numBmsData[ch_num]++;
		bmsData[ch_num] = (LPBMSDATA)realloc(bmsData[ch_num], numBmsData[ch_num] * sizeof(BMSDATA));
		ZeroMemory(&bmsData[ch_num][numBmsData[ch_num] - 1], sizeof(BMSDATA));
		bmsData[ch_num][numBmsData[ch_num] - 1].flag = true;
		bmsData[ch_num][numBmsData[ch_num] - 1].time = startpos;
		bmsData[ch_num][numBmsData[ch_num] - 1].data = dat;
		bmsData[ch_num][numBmsData[ch_num] - 1].fData = (float)dat;
		break;
	}

	return true;
}

// ��Փx���A�b�v�f�[�g����
bool CBMS::UpdateSelectSceneData(std::string file, SelectSceneData* header)
{
	std::ifstream ifs(file);
	std::string str;

	if (ifs.fail() || header == NULL)
	{
		std::cerr << "���s�I" << std::endl;
		return false;
	}

	// 1�s���ǂݍ���
	while (getline(ifs, str))
	{
		// �R�}���h�ȊO�͓ǂݔ�΂�
		if (str[0] != '#') continue;

		// ���s����������
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = NULL;

		// ���ߗ񂩂�R�}���h�������擾
		Command cmd = GetCommand(str);
		if (cmd == UNKNOWN)  {
			std::cout << "���ʕs�\����:" << str << std::endl;
			continue;
		}

		// ���ߗ񂩂�f�[�^�������擾
		std::string data;
		GetCommandString(str, &data);

		if (cmd == PLAYLEVEL){
			std::string tmp = file.substr(file.size() - 8, 4);
			if (tmp == "easy") header->levels[0] = std::stoi(data);
			else if (tmp == "norm") header->levels[1] = std::stoi(data);
			else if (tmp == "hard") header->levels[2] = std::stoi(data);
			header->levelNum++;
			break;
		}
	}

	return true;
}

// �Z���N�g��ʗp�̃f�[�^���擾
bool CBMS::GetSelectSceneData(std::string file, SelectSceneData* header)
{
	std::ifstream ifs(file);
	std::string str;

	if (ifs.fail() || header == NULL)
	{
		std::cerr << "���s�I" << std::endl;
		return false;
	}

	// 1�s���ǂݍ���
	while (getline(ifs, str))
	{
		// �R�}���h�ȊO�͓ǂݔ�΂�
		if (str[0] != '#') continue;

		// ���s����������
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = NULL;

		// ���ߗ񂩂�R�}���h�������擾
		Command cmd = GetCommand(str);
		if (cmd == UNKNOWN)  {
			std::cout << "���ʕs�\����:" << str << std::endl;
			continue;
		}

		// ���ߗ񂩂�f�[�^�������擾
		std::string data;
		GetCommandString(str, &data);

		// �p�����[�^�̑��
		switch (cmd)
		{
		case GENRE:
			header->Genre = data;
			std::cout << "�W������     " << data << std::endl;
			break;
		case TITLE:
			header->Title = data;
			std::cout << "�^�C�g��     " << data << std::endl;
			break;
		case ARTIST:
			header->Composer = data;
			std::cout << "�A�[�e�B�X�g " << data << std::endl;
			break;
		case BPM:		// BPM
			if (str[4] == ' ' || str[4] == '�@') {
				// �w�b�_�Ȃ�
				header->bpm = std::stoi(data);
				std::cout << "�x�[�XBPM    " << data << std::endl;
			}
			break;
		case PLAYLEVEL:
			// ������Փx�����Ȃ������Ƃ�
			std::string tmp = file.substr(file.size() - 8, 4);
			std::cout << "LEVEL:" << data << std::endl;
			if (tmp == "easy") header->levels[0] = std::stoi(data);
			else if (tmp == "norm") header->levels[1] = std::stoi(data);
			else if (tmp == "hard") header->levels[2] = std::stoi(data);
			header->levelNum++;
			break;
		}
	}
	return true;
}

// ���ʃf�[�^�̃w�b�_�[���擾����
bool CBMS::GetHeader( LPSTR file )
{
	//std::ifstream ifs(file);
	std::string str;

	char tmp[BMSMAXBUFFER];

	int FileHandle = DxLib::FileRead_open(file);
	//FileRead_gets(str, BMSMAXBUFFER, FileHandle);

	while (DxLib::FileRead_eof(FileHandle) == 0)
	{
		FileRead_gets(tmp, BMSMAXBUFFER, FileHandle);
		str = std::string(tmp);

		// �R�}���h�ȊO�͓ǂݔ�΂�
		if (str[0] != '#') continue;

		// ���s����������
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = NULL;

		// ���ߗ񂩂�R�}���h�������擾
		Command cmd = GetCommand(str);
		if (cmd == UNKNOWN)  {
			std::cout << "���ʕs�\����:" << str << std::endl;
			continue;
		}

		// ���ߗ񂩂�f�[�^�������擾
		std::string data;
		GetCommandString(str, &data);

		// �p�����[�^�̑��
		switch (cmd)
		{
		case PLAYER:
			bHeader.Player = std::stoi(data);
			std::cout << "PLAYER       " << bHeader.Player << std::endl;
			break;
		case GENRE:
			bHeader.Genre = data;
			std::cout << "�W������     " << bHeader.Genre << std::endl;
			break;
		case TITLE:
			bHeader.Title = data;
			std::cout << "�^�C�g��     " << bHeader.Title << std::endl;
			break;
		case ARTIST:
			bHeader.Artist = data;
			std::cout << "�A�[�e�B�X�g " << bHeader.Artist << std::endl;
			break;
		case BPM:		// BPM
			if (str[4] == ' ' || str[4] == '�@') {
				// �w�b�_�Ȃ�
				bHeader.bpm = (float)std::stof(data);
				//AddData(BMS_TEMPO, 0, (LONG)mBH.fBpm);
				AddNoteData(BMS_TEMPO, 0, (long)bHeader.bpm);
				std::cout << "�x�[�XBPM    " << bHeader.bpm << std::endl;
			}
			else {
				// �C���f�b�N�X�Ȃ�
				bHeader.BPMIndex[std::stoi(str.substr(4, 2))].Bpm = std::stof(data);
				std::cout << "�C���f�b�N�XBPM " << bHeader.BPMIndex[std::stoi(str.substr(4, 2))].Bpm << std::endl;
			}
			break;
		case MIDIFILE:
			bHeader.midifile = data;
			std::cout << "MIDIFILE      " << bHeader.midifile << std::endl;
			break;
		case PLAYLEVEL:
			bHeader.PlayLevel = std::stoi(data);
			break;
		case RANK:
			bHeader.Rank = std::stoi(data);
			break;
		case VOLWAV:
			bHeader.WavVol = std::stoi(data);
			break;
		case TOTAL:		// TOTAL
			bHeader.Total = std::stoi(data);
			break;
		case STAGEFILE:	// StageFile
			//DEBUG("StageImage-[%s]\n", mBH.mStagePic);
			bHeader.StagePic = data;
			break;
		case WAV:
			wavData = data;
			std::cout << "�y��:" << data << std::endl;
			break;
		case BMP:
			break;
		case SONG_GUID:
			break;
		case MOVIE:	// MOVIE
			break;
		default:
			int iLines;
			int iChannel;
			// ���ߔԍ��̎擾
			iLines = std::stoi(str.substr(1, 3));
			// �`�����l���ԍ��̎擾
			iChannel = std::stoi(str.substr(4, 2));	// ������
			if (iChannel == BMS_STRETCH) {
				// ���߂̔{���ύX���߂̏ꍇ
				//				DEBUG("���ߔ{���ϊ� [%s]�{\n",str);
				numBarMagni++;
				barMagni = (LPBARMAGNI)realloc(barMagni, sizeof(BARMAGNI)*numBarMagni);
				barMagni[numBarMagni - 1].lines = iLines;
				barMagni[numBarMagni - 1].magni = std::stof(data);
			}
			// ���ߔԍ��̍ő�l���L������
			if (bHeader.EndBar < iLines) bHeader.EndBar = iLines;

			break;
		}
	}

	FileRead_close(FileHandle);

	// ���ߔ{���f�[�^�����ɏ��߃o�[���쐬
	long long lastCount = 0;
	for (int i = 0; i <= bHeader.EndBar + 1; i++)
	{
		// ���߂�ǉ�
		numBmsBar++;
		bmsBar = (LPBMSBAR)realloc(bmsBar, sizeof(BMSBAR)*numBmsBar);
		bmsBar[numBmsBar - 1].flag = true;
		bmsBar[numBmsBar - 1].time = lastCount;
		long long add = BMSDATA_RESOLUTION;	//���Z�����`

		// ���݂̏��߂Ŕ{����ϊ����Ă��邩
		for (int j = 0; j < numBarMagni; j++)
		{
			if (barMagni[j].lines == i)
			{
				// �{���ύX���߂����݂���ꍇ�A1���߂�����̃J�E���g���Z�l��ω�������
				add = (long long)((float)BMSDATA_RESOLUTION * barMagni[j].magni);
				break;
			}
		}

		lastCount += add;	//�����߂̊J�n�ʒu�܂ŃJ�E���g�l�����Z����

		// �ő�̔������v�Z
		if (i < bHeader.EndBar && bHeader.MaxCount < lastCount)
			bHeader.MaxCount = lastCount;
	}

	std::cout << "NumBmsBar:" << numBmsBar << std::endl;

	return true;
}

bool CBMS::GetData( LPSTR file )
{
	std::ifstream ifs(file);
	std::string str;

	char tmp[BMSMAXBUFFER];
	int FileHandle = DxLib::FileRead_open(file);

	// 1�s���ǂݍ���
	while (DxLib::FileRead_eof(FileHandle) == 0)
	{
		FileRead_gets(tmp, BMSMAXBUFFER, FileHandle);
		str = std::string(tmp);

		// �R�}���h�ȊO�͓ǂݔ�΂�
		if (str[0] != '#') continue;

		// ���s����������
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = NULL;

		// ���ߗ񂩂�R�}���h�������擾
		Command cmd = GetCommand(str);
		if (cmd != OBJECT)  continue;

		std::string data;
		if (!GetCommandString(str, &data))  {
			std::cout << "�p�����[�^������擾�G���[" << data << std::endl;
		}

		// �f�[�^�ł���ΈӖ���͂��s��
		int barLines;		// ���݂̏��ߔԍ�
		int channel;		// ���݂̃`�����l���ԍ�
		char num[4] = {0};	//�ėp�o�b�t�@

		// �`�����l���ԍ��̎擾
		channel = std::stoi(str.substr(4,2));

		// ���߂̔{���ύX���߂̏ꍇ�̓L�����Z��
		if (channel == BMS_STRETCH) continue;

		// ���ߔԍ��̎擾
		barLines = std::stoi(str.substr(1, 3));

		// �f�[�^�����݂��邩
		int size = data.size();
		if (size < 1) {
			std::cout << "�f�[�^��������`����Ă��܂���:" << str << std::endl;
			continue;
		}

		// �f�[�^�����̕�������������
		if (size%2 == 1) {
			std::cout << "�f�[�^�����̕������������ł͂���܂���:" << str << std::endl;
			return false;
		}

		// �f�[�^��ǉ�����
		size = size / 2;

		// ���ߋ敪����f�[�^�z�u�ꏊ�����
		long long startCount = 0;	// ���̏��߂̍ŏ��̃J�E���g�l
		int chkLine = 0;					// �v�Z���̏���

		// ���݂̏��߂�1�O�܂ł�����
		while (chkLine < barLines) {
			// ���Z����|�C���^�����`
			long long add = BMSDATA_RESOLUTION;

			// ���݃`�F�b�N���̏��߂ɔ{���ύX���Ȃ�����������
			// �T�|�[�g�O

			//  ���݂܂ł̏��߃J�E���g�����Z
			startCount += add;

			// ���̏��߂�
			chkLine++;
		}

		// ���݂̏��߂̕������߂�
		float currentBarResolution = (float)BMSDATA_RESOLUTION / (float)size;

		// ���̏��߂ɔ{���ύX���߂����邩

		// ���f�[�^��ǉ�
		for (int i = 0; i < size; i++) {
			// 16�i���\�L�̕������10�i���̐��l�ɒ����B
			long hex = std::stoi(data.substr(i * 2, 2), nullptr, 16);
			if (hex > 0) {
				AddNoteData(channel, startCount + (long long)(currentBarResolution*(float)i),hex);
			}
		}
	}

	// �\�[�g
	for (int i = 0; i < BMSMAXBUFFER; i++)
		sort(i);

	return true;
}

// �w��`�����l���̃f�[�^�������\�[�g����
bool CBMS::sort(int ch)
{
	if (ch < 0 || ch > BMSMAXBUFFER - 1)
		return false;

	// ���ёւ���
	for (int i = 0; i < numBmsData[ch] - 1; i++)
	{
		for (int j = i + 1; j < numBmsData[ch]; j++)
		{
			if (bmsData[ch][i].time > bmsData[ch][j].time)
			{
				BMSDATA tmp;
				memcpy(&tmp, &bmsData[ch][i], sizeof(BMSDATA));		//tmp = i
				memcpy(&bmsData[ch][i], &bmsData[ch][j], sizeof(BMSDATA));	// i = j
				memcpy(&bmsData[ch][i], &tmp, sizeof(BMSDATA));		// j = tmp
			}
		}
	}

	return true;
}

long long CBMS::GetCountFromTime(double sec)
{
	long long bmscnt = 0;	//BMS�J�E���g
	double t = 0;				//BMS��̎���
	double bpm = bHeader.bpm;

	if (numBmsData[BMS_TEMPO] > 0)
		bpm = bmsData[BMS_TEMPO][0].data;	//����BPM

	//std::cout << "bpm:" << bpm << std::endl;

	if (sec < 0) return 0;

	// �w�莞�Ԃ𒴂���܂Ń^�C�������Z
	for (int i = 0; i < numBmsData[BMS_TEMPO]; i++)
	{
		// 1�O�̎��ԂƐV�������ԂƂ̌o�ߎ��Ԃ���b���Z�o
		double add = (double)(bmsData[BMS_TEMPO][i].time - bmscnt) / (bpm / 60) / (BMSDATA_RESOLUTION / 4);

		// ���݂̃e���|�l�Ŏ��Ԃ��߂����甲����
		if (t + add>sec) break;

		t += add;
		bpm = (double)bmsData[BMS_TEMPO][i].data;	//����BPM���Z�b�g
		bmscnt = bmsData[BMS_TEMPO][i].time;				//�v�Z�ς݂̃J�E���g���Z�b�g
	}

	// �w�莞�Ԃ�1�O�܂ł̎��Ԃ̍���
	double sub = sec - t;

	// ��������BMS�J�E���g�����Z�o
	long long cnt = (long long)(sub * (BMSDATA_RESOLUTION / 4) * (bpm / 60));

	// BMS�J�E���g�l�ɉ��Z
	bmscnt += cnt;

	return cnt;
}
