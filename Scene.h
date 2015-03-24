#pragma once
#include "tstring.h"
#include <sqrat.h>
#include "SceneUtil.h"

using namespace Sqrat;

class Scene
{
public:
	Scene(const char* sceneName, SceneType type = SceneType::VIEW);
	~Scene();

	Function Initialize();
	Function Update();
	Function Draw();
	Function Finalize();

	SceneType getType();
	void setType(SceneType type);

private:
	// �V�[����(�e�[�u����)��\��������
	tstring name;

	// �V�[���̎��
	SceneType sceneType;
};
