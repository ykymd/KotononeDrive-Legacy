#include "ScriptLoader.h"
#include <vector>
#include "tstring.h"
#include <string>
//#include <boost/algorithm/string.hpp>


ScriptLoader::ScriptLoader()
{
}


ScriptLoader::~ScriptLoader()
{
}

// Squirrel���̊֐����擾����
Sqrat::Function ScriptLoader::getFunction(const SQChar* callback_func)
{
	// "Reload.seriaize" �̂悤�ɁA�e�[�u�����̊֐����w�������񂪓n���ꂽ�ꍇ��
	// �h�b�g��؂�ŕ������āAwords�Ɋi�[���Ă���
	std::vector<tstring> words = split(callback_func, '.');
	//boost::split(words, callback_func, boost::is_any_of("."));

	// �ŏ��̓��[�g�e�[�u������
	Sqrat::Table tbl = Sqrat::RootTable();

	// �Ō�̃e�[�u��(vec�̖������ЂƂ�O)���擾����
	for (int i = 1, n = words.size(); i<n; i++)
	{
		tbl = tbl.GetSlot(words[i - 1].c_str());
	}

	// �֐�(vec�̖���)���擾����
	return Sqrat::Function(tbl, words[words.size() - 1].c_str());
}

std::vector<std::string> split(const std::string &str, char delim) {
	std::vector<std::string> res;
	size_t current = 0, found;
	while ((found = str.find_first_of(delim, current)) != std::string::npos) {
		res.push_back(std::string(str, current, found - current));
		current = found + 1;
	}
	res.push_back(std::string(str, current, str.size() - current));
	return res;
}