// �萔����уO���[�o���ϐ��̃��[�h
SQInclude("const.nut");
SQInclude("var.nut");

// ���C�u�����̃��[�h
SQInclude("FadeIO.nut");
SQInclude("thread.nut");
SQInclude("resources.nut");

//SQInclude("note_table.nut");
//SQInclude("note.nut");
//SQInclude("notesmanager.nut");

//�@���\�[�X�̃��[�h
SQInclude("SongList.nut");

// �e�N���X�̃��[�h
SQInclude("songwnd.nut");

// �e�V�[���N���X�̃��[�h
SQInclude("logo.nut");
SQInclude("title.nut");
SQInclude("select.nut");
SQInclude("gameplay.nut");
SQInclude("result.nut");

// �Q�[���N�����ɌĂяo�����֐�
function boot()
{
    // �摜�t�@�C���Q��ǂݍ���
    loadResource();

    // �����t�@�C��������������
    InitSoundMem();

    // �^�C�g���V�[���ւƐ؂�ւ���
    ChangeScene("Select");
}
