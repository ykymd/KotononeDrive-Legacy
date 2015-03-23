#include "SceneController.h"

// �W�F�l���[�^
SceneController* SceneController::getInstance(void)
{
	static SceneController Instance;
	return &Instance;
}

SceneController::SceneController(void)
{
	sceneStack.push(Scene("root"));
}


SceneController::~SceneController()
{
}

// ���݂̃V�[��
Scene& SceneController::getCurrentScene(void)
{
	if (!sceneStack.empty())
	{
		return sceneStack.top();
	}
	else{
		return Scene("root");
	}
}

// �V�[���J��
void SceneController::pushScene(Scene& nextScene)
{
	sceneStack.push(nextScene);
}
