#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>

class BMSHeader
{
public:
	BMSHeader();
	~BMSHeader();

	long		player;			// �v���C���[�h
	std::string	genre;			// �f�[�^�̃W������
	std::string	title;			// �f�[�^�̃^�C�g��
	std::string	artist;			// �f�[�^�̐����
	float		bpm;			// �f�[�^�̃e���|�i�����l��130�j
	std::string	midiFile;		// �o�b�N�O���E���h�ŗ���MIDI�t�@�C��
	long		playLevel;		// �f�[�^�̓�Փx
	long		rank;			// �f�[�^�̔��胉���N
	long		wavVol;			// ���ʂ����̉����ɂ��邩
	long		total;			// �Q�[�W�̑��ʂ�ݒ�
	std::string	stagePic;		// �ȊJ�n���ɕ\������摜
	std::string	movie;			// ���[�r�[�t�@�C����
	GUID		guid;			// �Ȃ�GUID(����d�l)
	long		endBar;			// �I������
	LONGLONG	maxCount;		// �ő�̃J�E���g��
	std::unordered_map<int,float> bpmIndex;	// �e���|�C���f�b�N�X
};
