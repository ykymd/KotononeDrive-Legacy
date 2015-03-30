//�@�ȑI����ʂ̃N���X
Select <- {
    buttonClicked = FALSE,
    soundHandle = 0,    //�����p�̃n���h��
    speedType = 3,
    isDiffMode = false,
    // �Z���N�g��ʂ̃E�C���h�E���i�[����e�[�u��
    SongWindows = {},
    centerWndKey = 0,   //���S�̗v�f
    //stri = array("a",0),
};

function Select::SetAvailableSong()
{
    // �Ȑ����擾
    g_songNum = g_SongData.len();
}

function Select::SetSongWindow()
{
    foreach(key, val in g_SongData) {
        local tmp = SongWnd(val);
        tmp.setID(key);
        SongWindows[key] <- tmp;
    }

    centerWndKey = 0;
}

// �I��������Փx�����ɁA�������t�@�C�����𓱂��o��
function Select::setFileName()
{
    g_SongTitle = "";
    g_SongTitle = SongWindows[centerWndKey].getSongFileName();
    switch (g_selectedLevel)
    {
    case 0:
        //Easy
        g_SongTitle += "_easy";
        break;
    case 1:
        //Normal
        g_SongTitle += "_norm";
        break;
    case 2:
        //hard
        g_SongTitle += "_hard";
        break;
    }

    g_SongTitle += ".bme";
}

function Select::initialize()
{
    print("Select init");

    isDiffMode = false;

    // �t�F�[�h�C��/�A�E�g�p�̃p�����[�^�̏�����
    FadeIO.stop();

    // �^�C�g����ʗp�̃X���b�h���쐬����
    Thread.createThread(threadFunction);

    //g_bms.GetSelectSceneData();   //���p�\�Ȃ��擾
    SetAvailableSong(); //�e�[�u�����Ɋi�[����
    SetSongWindow();    //�ȕ\���E�C���h�E���쐬

    // SE��ǂݍ���
    LoadSE(0);
    LoadSE(1);
    LoadSE(2);
    LoadSE(3);

    // BGM�̍Đ�
    PlaySoundFile(".\\data\\snd\\select.mp3", DX_PLAYTYPE_BACK);
}

function Select::finalize()
{
    print("Select final");

    Thread.stopThread();
}

function Select::threadFunction()
{
    // �t�F�[�h�C�����J�n����
    FadeIO.startFadeIn(30,0,0,0);

    // ���f�������s��
    if (Thread.wait(30)) return true;

    while (true)
    {
        if (GetJoypadInputStatePushed(INPUT_TYPE.LEFT) && !Select.buttonClicked)
        {
            PlaySE(2);
            if (!Select.isDiffMode && Select.SongWindows[0].id < 0){
                foreach(key, val in Select.SongWindows) val.setID(val.id+1);
                Select.centerWndKey--;
            }else{
                // ��Փx�I�����͑��x����
                Select.speedType--;
                if (Select.speedType < HIGHSPEED_MIN) Select.speedType = HIGHSPEED_MIN;
            }
        }
        if (GetJoypadInputStatePushed(INPUT_TYPE.RIGHT) && !Select.buttonClicked)
        {
            PlaySE(2);
            if (!Select.isDiffMode && Select.SongWindows[g_songNum-1].id > 0){
                foreach(key, val in Select.SongWindows) val.setID(val.id-1);
                Select.centerWndKey++;
            }else{
                // ��Փx�I�����͑��x����
                Select.speedType++;
                if (Select.speedType > HIGHSPEED_MAX) Select.speedType = HIGHSPEED_MAX;
            }
        }
        if (GetJoypadInputStatePushed(INPUT_TYPE.UP) && !Select.buttonClicked)
        {
            PlaySE(2);
            if (!Select.isDiffMode){
                Select.speedType++;
                if (Select.speedType > HIGHSPEED_MAX) Select.speedType = HIGHSPEED_MAX;
            }else{
                g_selectedLevel--;
                if (g_selectedLevel < 0) g_selectedLevel=0;
            }
        }
        if (GetJoypadInputStatePushed(INPUT_TYPE.DOWN) && !Select.buttonClicked)
        {
            PlaySE(2);
            if (!Select.isDiffMode){
                Select.speedType--;
                if (Select.speedType < HIGHSPEED_MIN) Select.speedType = HIGHSPEED_MIN;
            }else{
                g_selectedLevel++;
                if (g_selectedLevel > 2) g_selectedLevel=2;
            }
        }
        if (GetJoypadInputStatePushed(INPUT_TYPE.SPACE) && !Select.buttonClicked)
        {
            if (!Select.isDiffMode){
                print("KEY SPACE Pushed");
                PlaySE(2);
                Select.isDiffMode = true;
            }else{
                //Select.GetFileName();
                PlaySE(3);
                break;
            }
        }
        if (GetJoypadInputStatePushed(INPUT_TYPE.ESC) && !Select.buttonClicked)
        {
            PlaySE(1);
            if (Select.isDiffMode) {
                Select.isDiffMode = false;
            }else{
                BackScene("Title");
            }
        }

        if (suspend(false)) return true;
    }

    Select.setFileName();

    // �n�C�X�s�[�h�l���Z�b�g
    g_bms.SetHighSpeed(Select.speedType*0.5);

    // �t�F�[�h�A�E�g�������J�n����
    FadeIO.startFadeOut(45, 255, 255, 255);

    // ���΂炭�҂�
    if( Thread.wait(120) ) return true;

    // �Q�[���V�[���ւƐ؂�ւ���
    ChangeScene("GamePlay");

    StopSoundFile();

    return true;
}

function Select::update()
{
    //�E�C���h�E�̍X�V
    foreach(key, val in SongWindows)
        val.onUpdate();

    // �X���b�h�����s����
    Thread.executeThread();

    // �t�F�[�h�C��/�A�E�g�p�̏������s��
    FadeIO.update();
}

function Select::ShowSong()
{
    local str = "";
    local y = 0;
    foreach (key, val in g_SongData){
        str = "TITLE :"+val.title+"\n";
        DrawString(0,y,str,GetColor(255,255,255),12,0);

        y+=14;

        str = "GENRE :"+val.genre+"\n";
        DrawString(0,y,str,GetColor(255,255,255),12,0);
        y+=14;

        str = "ARTIST:"+val.composer+"\n";
        DrawString(0,y,str,GetColor(255,255,255),12,0);
        y+=14;

        str = "BPM   :"+val.bpm+"\n"+"\n";
        DrawString(0,y,str,GetColor(255,255,255),12,0);
        y+=20;
    }
}
function Select::draw()
{
    // �w�i�̕`��
    DrawGraph(0,0,getImgID("bg_gamescene.jpg"));

    // �E�C���h�E�̕`��
    foreach(key, val in SongWindows)
        val.onDraw();

    // ���o�[(����)��\��
    DrawGraph(240-10,420-10,getImgID("infobanner1_select.png"));

    DrawString(240-10+140,420-10-15, speedType,GetColor(255,255,255),30,0);

    if (isDiffMode){
        DrawGraph(0,150,getImgID("levelselect2.png"));

        // ��Փx��`��
        drawDiffStars();

        //DrawString(0,150+40, "TEST", GetColor(255,255,255), 30, 0);

    }

    // �㕔�o�i�[��\��
    if (!isDiffMode){
        DrawGraph(0,0,getImgID("infobanner2_mode_mu.png"));
    }else{
        DrawGraph(0,0,getImgID("infobanner2_mode_lv.png"));
    }

    //DrawVString(0,0,g_SongData[0].title, GREEN, fontHandle);

    //ShowSong();

    // �t�F�[�h�C��/�A�E�g�p�̏������s��
    FadeIO.draw();

}
//��Փx�̐��̕`��
function drawDiffStars()
{
    //��Փx�̐��̕`��
    for (local i=0; i<30; i++){
        local imdID = getImgID((i%10 < SongWindows[centerWndKey].SongData.level[i/10]) ? "star_ch_w.png" : "star_ep_w.png");
        DrawGraph(DIFFSTAR_X+STAR_W*(i%10),g_DIFFSTAR_Y[(i/10).tointeger()], imdID);
    }

    // �I�𒆃��C���𔭌�
    for (local i=0; i<10; i++)
        DrawGraph(DIFFSTAR_X+STAR_W*(i%10),g_DIFFSTAR_Y[g_selectedLevel],getImgID((i%10 < SongWindows[centerWndKey].SongData.level[g_selectedLevel]) ? "star_ch_y.png" : "star_ep_y.png"));
}