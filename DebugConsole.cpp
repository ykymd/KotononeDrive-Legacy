#include "DebugConsole.h"

#include <Windows.h>
#include <iostream>
#include "DxLib.h"

using namespace Kotone;

void DebugConsole::setConsole()
{
	// �R���\�[�������蓖�Ă�
	console_enable = AllocConsole();

	if (console_enable)
	{
		// �W���o�͂̏o�͐���R���\�[���ɐݒ肷��
		freopen("CONOUT$", "w", stdout);

		// �R���\�[���̃E�C���h�E�n���h�����擾����
		HWND console_handle = GetConsoleWindow();

		// DX���C�u�����̃E�C���h�E�n���h�����擾����(DX���C�u����API)
		HWND dxlib_handle = DxLib::GetMainWindowHandle();

		// �R���\�[���EDX���C�u�������ꂼ��̃E�C���h�E�n���h���̎擾�ɐ���������
		if (console_handle != NULL && dxlib_handle != NULL)
		{
			// DX���C�u�����̃E�C���h�E�ʒu�E�T�C�Y���i�[����ϐ�
			RECT tmp_rect;

			// DX���C�u�����̃E�C���h�E�ʒu���擾����
			GetWindowRect(dxlib_handle, &tmp_rect);

			// DX���C�u�����̃E�C���h�E�̕��ƍ������Z�o����
			int dxlib_width = tmp_rect.right - tmp_rect.left;
			int dxlib_height = tmp_rect.bottom - tmp_rect.top;

			// DX���C�u�����̃E�C���h�E�ʒu�����ɂ��炷
			SetWindowPos(
				dxlib_handle, NULL,
				tmp_rect.left - dxlib_width / 2, tmp_rect.top,
				dxlib_width, dxlib_height, 0);

			// �R���\�[���E�C���h�E���ADX���C�u�����̃E�C���h�E�̉E���ɂ��炷
			SetWindowPos(
				console_handle, NULL,
				tmp_rect.left + dxlib_width / 2, tmp_rect.top,
				dxlib_width, dxlib_height, 0);

			// DX���C�u�����̃E�C���h�E��O�ʂɕ\�����ăA�N�e�B�u�ɂ���
			SetForegroundWindow(dxlib_handle);
		}
	}
}

void DebugConsole::Finalize()
{
	// �R���\�[���̊��蓖�Ăɐ������Ă����
	if (console_enable)
	{
		// ���蓖�Ă��Ă���R���\�[�����������
		FreeConsole();
	}
}