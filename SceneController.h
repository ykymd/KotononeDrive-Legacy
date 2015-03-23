#pragma once

#include "tstring.h"
#include "Scene.h"
#include <stack>

class SceneController
{
public:
	~SceneController();

	// �W�F�l���[�^
	static SceneController* getInstance(void);

	// ���݂̃V�[��
	Scene& getCurrentScene(void);

	// �V�[���J��
	void pushScene(Scene& nextScene);

private:
	// �R���X�g���N�^�B��
	SceneController(void);

	std::stack<Scene&> sceneStack;
};

