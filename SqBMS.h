#pragma once
#include "CBMS.h"

#include <vector>

namespace Kotone{
	// �o�C���h�pKotononeBMS�Ǘ��N���X
	// �Ȃ�ׂ�SquirrelVM��C++�N���X�Ɉˑ������f�[�^��n���Ȃ��悤�ɂ���
	class SqBMS
	{
	public:
		float Score = 0.0f;

		SqBMS();
		~SqBMS();
		bool SQ_Load(char* file_name);	// ���ʃf�[�^�����[�h����
		int SQ_GetBarLine_Y(int num);
		long SQ_GetBarNum();
		void SQ_Play();	//�y�ȍĐ�
		void SQ_Stop();
		int SQ_GetObject_X(int ch, int num);
		long SQ_GetObjeNum(int ch);
		bool SQ_GetSelectSceneData();
		std::string SQ_GetTitle() { return bms.bHeader.Title; };
		int SQ_GetBPM() { return (int)bms.bHeader.bpm; };
		int SQ_Judge(int ch, int num);
		bool SQ_JudgeOver(int ch, int num);
		int SQ_LongNoteJudge(int ch, int num);
		bool SQ_LongNoteJudgeOver(int ch, int num);
		int SQ_GetAvailableSoungNum() { return songNum; };	// ���p�\�Ȑ����擾
		std::string SQ_GetAvailableSongTitle(int i){ return availableSongs[i].Title; }
		std::string SQ_GetAvailableSongComposer(int i){ return availableSongs[i].Composer; }
		std::string SQ_GetAvailableSongGenre(int i){ return availableSongs[i].Genre; }
		int SQ_GetAvailableSongBPM(int i){ return availableSongs[i].bpm; }
		std::string SQ_GetAvailableSongFilename(int i){ return availableSongs[i].fileName; }
		int SQ_GetAvailableSongDifficulty(int i);
		float SQ_GetHighSpeed() { return bms.ScrMulti; };	// �n�C�X�s�[�h�l���擾
		void SQ_SetHighSpeed(float spd) { bms.ScrMulti = spd; };	// �n�C�X�s�[�h�l���X�V
		float SQ_GetScore(int ch, int num);
		bool SQ_isStart();
		bool SQ_isFinished();
		bool SQ_Clear(void);


	private:
		bool isStart;

		static const int GREAT_JUDGE_COUNT = 750;	//GREAT������o���͈͂̃J�E���g
		static const int NEAR_JUDGE_COUNT = 1250;	//NEAR������o���͈͂̃J�E���g
		static const int EARLY_JUDGE_COUNT = 2500;		//EARLY_FAILED������o���͈͂̃J�E���g

		LONGLONG		llStartTime;		// �Q�[���J�n���̎���(���𑜓x�^�C�}�[)
		LONGLONG		llGlobalFreq;		// �}�V���̂P�b�Ԃ̃J�E���g��(���𑜓x�^�C�}�[)
		int songNum = 0;	//availableSongs�̖����̃C���f�b�N�X

		// �I���\�ȃ��X�g
		Kotone::SelectSceneData availableSongs[BMSMAXBUFFER];
		Kotone::CBMS bms;
		Kotone::LPBMSBAR bar;

		static std::vector<std::string> readDir(std::string folder);
	};
}
