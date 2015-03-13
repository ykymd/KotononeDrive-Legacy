#pragma once
#include "tstring.h"
#include <sqrat.h>

using namespace Sqrat;

class Scene
{
public:
	Scene(const char* sceneName);
	~Scene();

	Function Initialize();
	Function Update();
	Function Draw();
	Function Finalize();

private:
	tstring getName();

	// �V�[����(�e�[�u����)��\��������
	tstring name;
};
