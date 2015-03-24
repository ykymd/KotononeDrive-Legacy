#pragma once

#include "tstring.h"
#include "Scene.h"
#include <stack>

class SceneController
{
public:
	// �W�F�l���[�^
	static SceneController* getInstance(void);

	// ���݂̃V�[��
	Scene& getCurrentScene(void);

	// �V�[���J��
	void pushScene(Scene& nextScene);

	// �O�̃V�[���ɖ߂�
	void popScene();

	void setInitializeFlag(bool flag){ initializeFlag = flag; }
	void setFinalizeFlag(bool flag){ finalizeFlag = flag; }
	void setFirstSceneFlag(bool flag){ firstSceneFlag = flag; }

	bool getInitializeFlag(){ return initializeFlag; }
	bool getFinalizeFlag(){ return finalizeFlag; }
	bool getFirstSceneFlag(){ return firstSceneFlag; }

private:
	// initialize, finalize���Ăяo�����̃t���O
	bool initializeFlag;
	bool finalizeFlag;
	bool firstSceneFlag;

	std::stack<Scene> sceneStack;

	void changeScene();
};

