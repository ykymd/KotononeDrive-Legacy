#pragma once

#include "DxLib.h"
#include <sqrat.h>
#include <vector>
#include <boost/algorithm/string.hpp>

namespace Kotone {
	class Game
	{
	public:
		// �X�V�����ƕ`�揈���p��Squirrel�֐�
		Sqrat::Function update_function;
		Sqrat::Function draw_function;

		// F5�[��������Ă��邩�ǂ����̃t���O
		int CheckHitF5KeyNow = false;
		int CheckHitF5keyPrev = false;

		/* �^�C�}�[�֘A�̃p�����[�^ */

		// �o�ߎ���(�~���b)���i�[����ϐ�
		int ElapsedTime = 0;

		// Sleep���鎞�Ԃ��i�[����ϐ�
		int SleepTime = 0;

		// 1�t���[�������~���b��(60FPS)
		float FrameTime = 1000.0f / 60.0f;

		// �������ǂ����Ȃ������ۂɁA�t���[���X�L�b�v���s�����ǂ���
		bool FrameSkipFlag = true;

		// �O����s���̎���(�}�C�N���b)���i�[����ϐ�
		LONGLONG OldTime = 0;

		bool Init();
		void Run();
		void Finalize();

		Sqrat::Function GetSquirrelFunction(const SQChar* callback_func);
		void InitializeJoypadInputState();
		void UpdateJoypadInputState();
	};
}