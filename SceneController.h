#pragma once

#include "tstring.h"
#include "Scene.h"

class SceneController
{
public:
	~SceneController();

	// �W�F�l���[�^
	static SceneController* getInstance(void);

	// ���݂̃V�[��
	Scene& SceneController::getCurrentScene(void);

	// �V�[���J��
	void pushScene(Scene* nextScene);

private:
	// �R���X�g���N�^�B��
	SceneController(void);

	Scene* currentScene;
};

