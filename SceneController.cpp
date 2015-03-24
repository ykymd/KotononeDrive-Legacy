#include "SceneController.h"

// �W�F�l���[�^
SceneController* SceneController::getInstance(void)
{
	static SceneController* Instance = nullptr;
	if (Instance == nullptr)
	{
		Instance = new SceneController();
		Instance->setInitializeFlag(false);
		Instance->setFinalizeFlag(false);
		Instance->setFirstSceneFlag(true);
		Instance->sceneStack.push(Scene("root", SceneType::ROOT));
	}
	return Instance;
}

// ���݂̃V�[��
Scene& SceneController::getCurrentScene(void)
{
	if (sceneStack.empty())
	{
		sceneStack.push(Scene("root", SceneType::ROOT));
	}

	return sceneStack.top();
}

void SceneController::changeScene()
{
	// initialize�֐����Ăяo���t���O�𗧂Ă�
	initializeFlag = true;

	// finalize�֐��́A2��ڈȍ~�̃V�[���؂�ւ����ɂ̂݌Ăяo�����悤�ɂ���
	if (firstSceneFlag)
	{
		finalizeFlag = false;
		firstSceneFlag = false;
	}
	else
	{
		finalizeFlag = true;
	}
}

// �V�[���J��
void SceneController::pushScene(Scene& nextScene)
{
	changeScene();
	sceneStack.push(nextScene);
}

// �O�̃V�[���ɖ߂�
void SceneController::popScene()
{
	changeScene();
	sceneStack.pop();
}
