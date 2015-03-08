#include <Windows.h>
#include <string>

#pragma once

#define BMSMAXBUFFER 256

// BMS�`�����l����`
#define BMS_BACKMUSIC		0x01		// ���̈ʒu�ɂ�����A�����I�ɍĐ������WAV���w�肵�܂�
#define BMS_STRETCH			0x02		// ���̏��߂̒������`�����f�[�^�{���܂��i�P�O�i���A�����Ƃ��Ɏg�p�j
#define BMS_TEMPO			0x03		// �Đ��e���|�i�a�o�l / �P���Ԃ̎l���������j�̓r���ύX�i�P�U�i���j
#define BMS_BACKANIME		0x04		// �o�b�N�O���E���h�A�j���[�V�����@�\
#define BMS_EXTENEDOBJ		0x05		// �������Ă���I�u�W�F��ʂ̃L�����N�^�[�i���o�[�̂��̂ɂ���ւ���@�\
#define BMS_CHGPOORANI		0x06		// POOR���o�����Ƃ��ɕ\�������摜��ύX
#define BMS_LAYERANIME		0x07		// Ch.04�Ŏw�肵��BGA�̏�ɂ��Ԃ���BMP���w��ł��܂�
#define BMS_BPMINDEX		0x08		// BPM�̃C���f�b�N�X�w��(�V)

const long long BMSDATA_RESOLUTION = 9600;	//1���߂̃J�E���g��

namespace Kotone {

	enum Command {
		PLAYER = 0,
		GENRE = 1,
		TITLE = 2,
		ARTIST = 3,
		BPM = 4,
		MIDIFILE = 5,
		PLAYLEVEL = 6,
		RANK = 7,
		VOLWAV = 8,
		TOTAL = 9,
		STAGEFILE = 10,
		WAV = 11,
		BMP = 12,
		SONG_GUID = 13,
		MOVIE = 14,
		OBJECT = -1,
		UNKNOWN = -2,
	};

	enum JUDGE_TYPE
	{
		Critical,       // �s�b�^��
		Near,           // ��₸��Ă�
		Failed,         // ���s
		None,           //�@����
	};

	// BMS�f�[�^���
	typedef struct _BMSDATA {
		bool flag;				//���݂��Ă��邩
		long long time;	//�炷�^�C�~���O
		long data;			//�炷�f�[�^
		float fData;			//���ߓ_�f�[�^
		JUDGE_TYPE judge;	//����
	} BMSDATA, *LPBMSDATA;

	// ���߃f�[�^
	typedef struct _BMSBAR
	{
		bool flag;				//���݂��邩
		long long time;	//�\�����鎞��
		long data;			//���߂ɂ��Ă̒ǉ����
	}BMSBAR, *LPBMSBAR;

	// BPM�C���f�b�N�X
	typedef struct _BPMINDEX {
		float		Bpm;					// BPM
	} BPMINDEX;

	// ���߂̒����Ǘ�
	typedef struct _BARMAGNI {
		long lines;		//���ߔԍ�
		float magni;		//�{��
	}BARMAGNI, *LPBARMAGNI;

	typedef struct _BMSHEADER
	{
		long Player;
		std::string Genre;
		std::string Title;
		std::string Artist;
		float bpm;
		std::string midifile;
		long PlayLevel;
		long Rank;
		long WavVol;
		long Total;
		std::string StagePic;
		std::string Movie;
		GUID guid;
		long EndBar;
		LONGLONG MaxCount;
		BPMINDEX BPMIndex[BMSMAXBUFFER];

	} BMSHEADER;

	// �Z���N�g��ʂɕK�v�ȃf�[�^
	// �Z���N�g��ʂŃf�������͗����Ȃ�
	typedef struct _SelectSceneData
	{
		std::string Title;			//�^�C�g��
		std::string Composer;	//�R���|�[�U�[
		std::string Genre;		//�W������
		int bpm = 0;				//BPM
		std::string fileName;	// �t�@�C����
		int levels[3];				//��Փx
		int levelNum = 0;
	} SelectSceneData;

	class CBMS
	{
	public:
		BMSHEADER bHeader;	//�w�b�_�f�[�^

		LPBMSDATA bmsData[BMSMAXBUFFER];	//���f�[�^
		int numBmsData[BMSMAXBUFFER];			//���f�[�^�̃`�����l�����ꂼ��̐�

		//int numPastNote[8];	//���ɔ���ς݂̃m�[�c�̐�

		LPBMSBAR bmsBar;
		int numBmsBar;	//���߂̌�

		LPBARMAGNI barMagni;	//���߂̔{���f�[�^
		int numBarMagni;	//���ߔ{���̌�

		std::string wavData;
		float ScrMulti = 1.0f;	//�X�N���[�����x

		CBMS();
		~CBMS();
		bool Load(LPSTR file);
		bool UpdateSelectSceneData(std::string file, SelectSceneData* header);
		bool GetSelectSceneData(std::string file, SelectSceneData* dat);
		bool GetHeader(LPSTR file);
		bool GetData(LPSTR file);
		Command GetCommand(std::string cmd);
		bool GetCommandString(std::string src, std::string* dst);
		bool AddNoteData(unsigned int ch_num, long long startpos, long dat);
		bool sort(int ch);
		long long GetCountFromTime(double sec);

		inline long long GetMaxCount(void) { return bHeader.MaxCount; }
		inline LPBMSBAR GetBar(int num) { return &bmsBar[num]; }
		inline long GetBarNum(void) { return numBmsBar; }
		inline LPBMSDATA GetObj(int ch, int num) { return &bmsData[ch][num]; }
		inline long GetObjeNum(int ch) { return numBmsData[ch]; }
		inline std::string GetWavFile(void) { return wavData; }
	};
}
