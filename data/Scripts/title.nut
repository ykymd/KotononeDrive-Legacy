//�@���S�\����ʂ̃N���X
Title <- {};

function Title::initialize()
{
    print("Title init");

    // �t�F�[�h�C��/�A�E�g�p�̃p�����[�^�̏�����
    FadeIO.stop();

    // �^�C�g����ʗp�̃X���b�h���쐬����
    Thread.createThread(threadFunction);

    // SE��ǂݍ���
    LoadSE(0);
    LoadSE(1);

    //BGM�̍Đ�
    PlaySoundFile(".\\data\\snd\\�^�C�g��BGM.mp3", DX_PLAYTYPE_LOOP);
}

function Title::finalize()
{
    print("Title final");

    Thread.stopThread();
}

function Title::threadFunction()
{
    //print("Title Thread");
    // �t�F�[�h�C�����J�n����
    FadeIO.startFadeIn(30,0,0,0);

    // ���f�������s��
    if (Thread.wait(30)) return true;

    while (true)
    {
    	if (GetJoypadInputStatePushed(INPUT_TYPE.SPACE))
    	{
    		print("KEY Pushed");
            PlaySE(0);
    		break;
    	}

        if (GetJoypadInputStatePushed(INPUT_TYPE.ESC))
        {
            PlaySE(1);
            EXIT();
        }

    	if (suspend(false)) return true;
    }

    // �t�F�[�h�A�E�g�������J�n����
    FadeIO.startFadeOut(45, 255, 255, 255);

    // ���΂炭�҂�
    if( Thread.wait(90) ) return true;

    //StopSoundMem(soundHandle);
    //DeleteSoundMem(soundHandle);

    // �Q�[���V�[���ւƐ؂�ւ���
    ChangeScene("Select");

    return true;
}

function Title::update()
{
    // �X���b�h�����s����
    Thread.executeThread();

    // �t�F�[�h�C��/�A�E�g�p�̏������s��
    FadeIO.update();
}

function Title::draw()
{
    // ���S�̕`��
    DrawGraph(0,0,getImgID("title.png"));

    // �t�F�[�h�C��/�A�E�g�p�̏������s��
    FadeIO.draw();
}
