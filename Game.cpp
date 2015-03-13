#include "Game.h"
#include "BindFunc.h"
#include "SceneController.h"

using namespace Kotone;

// Squirrel���̊֐����擾����
Sqrat::Function Game::GetSquirrelFunction(const SQChar* callback_func)
{
	// "Reload.seriaize" �̂悤�ɁA�e�[�u�����̊֐����w�������񂪓n���ꂽ�ꍇ��
	// �h�b�g��؂�ŕ������āAvec�Ɋi�[���Ă���
	std::vector<tstring> vec;
	boost::split(vec, callback_func, boost::is_any_of(_T(".")));

	// �ŏ��̓��[�g�e�[�u������
	Sqrat::Table tbl = Sqrat::RootTable();

	// �Ō�̃e�[�u��(vec�̖������ЂƂ�O)���擾����
	for (int i = 1, n = vec.size(); i<n; i++)
	{
		tbl = tbl.GetSlot(vec[i - 1].c_str());
	}

	// �֐�(vec�̖���)���擾����
	return Sqrat::Function(tbl, vec[vec.size() - 1].c_str());
}

// �L�[�{�[�h�̃L�[�ƃW���C�p�b�h�̃{�^���̉�����Ԃ�����������
void Game::InitializeJoypadInputState()
{
	for (int i = 0; i<enableKeyNum; ++i)
		for (int j = 0; j<2; ++j)
			g_JoypadInputState[i][j] = false;
}

// �L�[�{�[�h�̃L�[�ƃW���C�p�b�h�̃{�^���̉�����Ԃ�����������
void Game::UpdateJoypadInputState()
{
	// g_FlipIndex��0�Ȃ�1�ɁA1�Ȃ�0�ɂ���
	g_FlipIndex = g_FlipIndex ^ 1;

	// �L�[�{�[�h��W���C�p�b�h�̉�����Ԃ𒲂ׂăZ�b�g����(DX���C�u����API)
	g_JoypadInputState[INPUT_TYPE::UP][g_FlipIndex] =
		(DxLib::GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_UP) != 0;

	g_JoypadInputState[INPUT_TYPE::DOWN][g_FlipIndex] =
		(DxLib::GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_DOWN) != 0;

	g_JoypadInputState[INPUT_TYPE::LEFT][g_FlipIndex] =
		(DxLib::GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_LEFT) != 0;

	g_JoypadInputState[INPUT_TYPE::RIGHT][g_FlipIndex] =
		(DxLib::GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_RIGHT) != 0;

	g_JoypadInputState[INPUT_TYPE::ONE][g_FlipIndex] =
		CheckHitKey(KEY_INPUT_3); //||
	//((DxLib::GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1) != 0);

	g_JoypadInputState[INPUT_TYPE::TWO][g_FlipIndex] =
		CheckHitKey(KEY_INPUT_E); //||
	//((DxLib::GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_2) != 0);

	g_JoypadInputState[INPUT_TYPE::THREE][g_FlipIndex] =
		CheckHitKey(KEY_INPUT_D); //||
	//((DxLib::GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_3) != 0);

	g_JoypadInputState[INPUT_TYPE::FOUR][g_FlipIndex] =
		CheckHitKey(KEY_INPUT_C); //||
	//((DxLib::GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_4) != 0);

	g_JoypadInputState[INPUT_TYPE::FIVE][g_FlipIndex] =
		CheckHitKey(KEY_INPUT_0); //||
	//((DxLib::GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_5) != 0);

	g_JoypadInputState[INPUT_TYPE::SIX][g_FlipIndex] =
		CheckHitKey(KEY_INPUT_O); //||
	//((DxLib::GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_6) != 0);

	g_JoypadInputState[INPUT_TYPE::SEVEN][g_FlipIndex] =
		CheckHitKey(KEY_INPUT_K); //||
	//((DxLib::GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_7) != 0);

	g_JoypadInputState[INPUT_TYPE::EIGHT][g_FlipIndex] =
		CheckHitKey(KEY_INPUT_M);// ||
	//((DxLib::GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_8) != 0);

	g_JoypadInputState[INPUT_TYPE::ESC][g_FlipIndex] =
		CheckHitKey(KEY_INPUT_ESCAPE);
	//(DxLib::GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_9) != 0;

	g_JoypadInputState[INPUT_TYPE::SPACE][g_FlipIndex] =
		CheckHitKey(KEY_INPUT_SPACE);
	//(DxLib::GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_10) != 0;
}

bool Game::Init()
{
	// �E�C���h�E�̃^�C�g����ݒ�
	SetMainWindowText(_T("�R�g�m�l�h���C�u�`KottON-pluckin'!�`"));

	//���O�̏o�͂��s��Ȃ�
	DxLib::SetOutApplicationLogValidFlag(false);

	// �E�B���h�E���[�h�ɕύX����(DX���C�u����API)
	DxLib::ChangeWindowMode(TRUE);

	// DX���C�u����������������(DX���C�u����API)
	if (DxLib::DxLib_Init() == -1) {
		// �G���[�ł���ΏI������
		return false;
	}

	//�A�[�J�C�u�t�@�C���̊g���q���w�肷��
	DxLib::SetDXArchiveExtension("dat");

	// DX���C�u�����̕`���̈�𗠉��(�o�b�N�o�b�t�@)�ɐݒ肷��(DX���C�u����API)
	DxLib::SetDrawScreen(DX_SCREEN_BACK);

	// �`�悷�镶����̑������f�t�H���g���ׂ�����(DX���C�u����API)
	DxLib::SetFontThickness(3);

	// ������̕`�揈�����A���`�G�C���A�X����ɕύX����(DX���C�u����API)
	DxLib::ChangeFontType(DX_FONTTYPE_ANTIALIASING);

	return true;
}

void Game::Run()
{
	// �L�[�{�[�h�ƃW���C�p�b�h�̓��͏�Ԃ�����������
	InitializeJoypadInputState();

	// Squirrel����boot�֐����Ăяo��
	GetSquirrelFunction(_SC("boot")).Execute();

	// ���݂̎���(�}�C�N���b)���擾���AOldTime�ɃZ�b�g����(DX���C�u����API)
	OldTime = DxLib::GetNowHiPerformanceCount();

	// �t�H���g�n���h�����Z�b�g
	titleFontHandle = CreateFontToHandle("@�l�r ����", 18, -1, DX_FONTTYPE_ANTIALIASING);
	subFontHandle = CreateFontToHandle("@�l�r ����", 16, -1, DX_FONTTYPE_ANTIALIASING);

	auto sceneController = SceneController::getInstance();

	while (DxLib::ProcessMessage() == 0){
		// ���݂̃V�[�����擾
		auto currentScene = sceneController->getCurrentScene();

		/* �V�[���ؑ֎��̏��� */

		// �V�[����.finalize�֐����Ăяo��
		if (g_FinalizeFlag)
		{
			GetSquirrelFunction(currentScene.Finalize().c_str()).Execute();
			g_FinalizeFlag = false;
		}

		if (g_InitializeFlag)
		{
			// �V�[����.initialize�֐����Ăяo��
			GetSquirrelFunction(currentScene.Initialize().c_str()).Execute();

			// �X�V�����p�̊֐��Ƃ��āA�V�[����.update�֐����Z�b�g����
			update_function = GetSquirrelFunction(currentScene.Update().c_str());

			// �`�揈���p�̊֐��Ƃ��āA�V�[����.draw�֐����Z�b�g����
			draw_function = GetSquirrelFunction(currentScene.Draw().c_str());
		}


		// �O���F5�L�[�t���O���Z�b�g����
		CheckHitF5keyPrev = CheckHitF5KeyNow;

		// F5�L�[��������Ă��邩�ǂ������ׂ�
		CheckHitF5KeyNow = CheckHitKey(KEY_INPUT_F5);

		//�ēǂݍ��݂����s����
		/*
		if (CheckHitF5KeyNow && !CheckHitF5keyPrev){
		if (OpenSquirrelFile("title.nut"))
		{
		printf("Succes File Reloaded\n");
		}
		else{
		printf("Failed File Reloaded\n");
		}
		}
		*/

		// �������Ԃ��v��
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);

		// ���݂̎���(�}�C�N���b)���擾����(DX���C�u����API)
		LONGLONG NowTime = DxLib::GetNowHiPerformanceCount();

		// ���݂̎��ԂƁA�O����s���̎��ԂƂ̍������v�Z����
		ElapsedTime = (int)((NowTime - OldTime) / 1000);

		// ����v�Z�p�ɁA���݂̎��Ԃ�ێ�����
		OldTime = NowTime;


		/* Sleep�����ƁAUpdate�̎��O���� */

		// Update����񐔁B1�t���[���Ԃ�̉񐔂ŏ���������
		int UpdateCount = 1;

		// �o�ߎ��Ԃɂ͑O��Sleep�������̎��Ԃ��܂܂�Ă���̂ŁA���̕�������
		ElapsedTime -= SleepTime;

		// �o�ߎ��Ԃ��A1�t���[��������̒P�ʎ��Ԃ�菬�������
		if (ElapsedTime < FrameTime)
		{
			// �P�ʎ��� - �o�ߎ��� �Ԃ񂾂��X���[�v����
			SleepTime = (int)FrameTime - ElapsedTime;
			Sleep(SleepTime);
		}
		// 1�t���[��������̒P�ʎ��Ԃ��o�ߎ��Ԃ��傫����΁A�������x��Ă���
		else
		{
			// �X���[�v�����͍s��Ȃ��̂ŁA�X���[�v���Ԃ�0�ɂ���
			SleepTime = 0;

			// 1�t���[�����̎��Ԃ�����
			ElapsedTime -= (int)FrameTime;

			// �t���[���X�L�b�v(�ړ�������S����x�Ɏ��s����)
			if (FrameSkipFlag)
			{
				// Update����񐔂����Z����
				for (; ElapsedTime >= (int)FrameTime; ElapsedTime -= (int)FrameTime)
				{
					UpdateCount++;
				}
			}
			// FrameSkipFlag��false�Ȃ珈������(�X�V������1�񂾂����s�����)
			// UpdateCount��1�̂܂�
		}

		// �V�[���ؑ֎��ɂ́A�X�V������1�񂾂��s��
		if (g_InitializeFlag)
		{
			g_InitializeFlag = false;
			UpdateCount = 1;
		}

		// �X�V������K�v�ȉ񐔂����s��
		for (int i = 0; i<UpdateCount; ++i)
		{
			// �L�[�{�[�h�ƃW���C�p�b�h�̓��͏�Ԃ��X�V����
			UpdateJoypadInputState();

			// update�֐����Ăяo��
			update_function.Execute();
		}

		// ��ʂɕ`�悳��Ă�����e���N���A����(DX���C�u����API)
		DxLib::ClearDrawScreen();

		// draw�֐����Ăяo��
		draw_function.Execute();

		//DrawVStringToHandle(0, 0, "��ՌÎ�Ƃ���", GetColor(255, 255, 255), FontHandle);

		// �����(�o�b�N�o�b�t�@)�ɕ`�悳��Ă��钆�g���A�\��ʂƓ���ւ���(DX���C�u����API)
		DxLib::ScreenFlip();
	}
}

void Game::Finalize()
{
	// finalize�֐����Ăяo��
	if (g_FirstSceneFlag)
		GetSquirrelFunction(SceneController::getInstance()->getCurrentScene().Finalize().c_str()).Execute();

	// �֐��I�u�W�F�N�g���������
	// (Sqrat::Function�^�̃f�X�g���N�^���ŉ��z�}�V���ɃA�N�Z�X���Ă���̂ŁA
	// sq_close�֐��ŉ��z�}�V�����������O�ɁARelease�֐��ŉ�����Ȃ���΃G���[�ƂȂ�)
	update_function.Release();
	draw_function.Release();

	DxLib::DxLib_End();				// �c�w���C�u�����g�p�̏I������
}
