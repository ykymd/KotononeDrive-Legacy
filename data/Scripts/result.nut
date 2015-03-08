//�@���S�\����ʂ̃N���X
Result <- {
	//soundHandle = 0,	//�����p�̃n���h��
    songTitle = "",
    score = 0,
};

function Result::initialize()
{
    print("Result init");

    // �t�F�[�h�C��/�A�E�g�p�̃p�����[�^�̏�����
    FadeIO.stop();

    // �^�C�g����ʗp�̃X���b�h���쐬����
    Thread.createThread(threadFunction);

    songTitle = g_bms.GetTitle();
    score = (g_bms.Score/g_numAllNote*100).tointeger();
    //score = 1;
    PlaySoundFile(".\\data\\snd\\���U���gBGM.mp3", DX_PLAYTYPE_LOOP);
}

function Result::finalize()
{
    print("Result final");

    Thread.stopThread();
}

function Result::threadFunction()
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
    		break;
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
    // ���ʃf�[�^�����Z�b�g
    g_bms.Clear();
    //EXIT();
    ChangeScene("Select");

    return true;
}

function Result::update()
{
    // �X���b�h�����s����
    Thread.executeThread();

    // �t�F�[�h�C��/�A�E�g�p�̏������s��
    FadeIO.update();
}

function Result::draw()
{
    // ���S�̕`��
    //SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
    if (score < 60) {
        DrawGraph(0,0,getImgID(g_nasubiStill[score%2]));
    }else{
        DrawGraph(0,0,getImgID(g_resultStill[score%3]));
    }
    //SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);

    // �Ȗ��̕`��
    DrawString(20,0,songTitle,GetColor(0,0,0),40,0);

    //
    switch(g_selectedLevel){
        case 0:
            DrawString(20,40,"Easy",GetColor(0,0,0),40,0);
            break;
        case 1:
            DrawString(20,40,"Normal",GetColor(0,0,0),40,0);
            break;
        case 2:
            DrawString(20,40,"Hard",GetColor(0,0,0),40,0);
            break;
    }

    // �B�����̕`��
    DrawString(20,80,"Score:"+score+"%",GetColor(0,0,0),40,0);

    // �t�F�[�h�C��/�A�E�g�p�̏������s��
    FadeIO.draw();
}
