#pragma once
#include "tstring.h"
#include <squirrel.h>

class Scene
{
public:
	Scene(const char* sceneName);
	~Scene();

	std::string Initialize();
	std::string Update();
	std::string Draw();
	std::string Finalize();

private:
	tstring getName();

	// �V�[����(�e�[�u����)��\��������
	tstring name;
};
