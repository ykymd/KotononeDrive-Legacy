#pragma once
#include <Windows.h>

namespace Kotone {
	class DebugConsole
	{
	public:
		void setConsole();
		void Finalize();

		// �R���\�[�������蓖�Ă�
		BOOL console_enable;
	};
}