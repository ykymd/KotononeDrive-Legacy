#include "SceneController.h"

// �W�F�l���[�^
SceneController* SceneController::getInstance(void)
{
	static SceneController Instance;
	return &Instance;
}

SceneController::SceneController(void)
{
	currentScene = new Scene("root");
}


SceneController::~SceneController()
{
}

// ���݂̃V�[��
Scene& SceneController::getCurrentScene(void)
{
	return *currentScene;
}

// �V�[���J��
void SceneController::pushScene(Scene* nextScene)
{
	currentScene = nextScene;
}
