//
// BGM�̍Đ�����ђ�~���s���N���X
//

#pragma once

#include <string>

namespace Kotone{
	class Music
	{
	public:
		static Music* getInstance(){
			static Music instance;
			return &instance;
		}
		~Music();
		void loadBGM(std::string filename);
		int playBackBGM();
		int stopBGM();
	private:
		Music();

		int BGMHandle;
	};
}
