#include "SceneController.h"

#include <squirrel.h>

// �W�F�l���[�^
SceneController* SceneController::getInstance(void)
{
	static SceneController Instance;
	return &Instance;
}

SceneController::SceneController(void)
	:g_SceneName(_SC(""))
{
}


SceneController::~SceneController()
{
}

// ���݂̃V�[��
const tstring SceneController::currentScene(void)
{
	return g_SceneName;
}

// �V�[���J��
void SceneController::pushScene(const char* scene_name)
{
	g_SceneName = scene_name;
}
