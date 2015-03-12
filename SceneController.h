#pragma once

#include "tstring.h"

class SceneController
{
public:
	~SceneController();

	// �W�F�l���[�^
	static SceneController* getInstance(void);

	// ���݂̃V�[��
	const tstring SceneController::currentScene(void);

	// �V�[���J��
	void pushScene(const char* scene_name);

private:
	// �R���X�g���N�^�B��
	SceneController(void);

	// �V�[����(�e�[�u����)��\��������
	tstring g_SceneName;
};

