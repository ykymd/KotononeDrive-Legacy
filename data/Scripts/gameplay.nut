//�@�Q�[����ʂ̃N���X
GamePlay <-{
    bmsNum = 0,
    noteNum = array(8,0),
    judgeNum = array(8,0),
    numAllNote = 0,    //�S�m�[�c��
    beforePress = array(4,FALSE),   //���[��L1����L4�܂ł𒼑O�t���[���ŉ�������
    songTitle = "",
    bpmLabel = 0,
    highSpeed = 0,
    scoreFinished = false,
    operationLock = false,
    showJudge = array(2,0), //���߂̔�����i�[����(0:���A1:�E)
};

function GamePlay::initialize()
{
    print("GamePlay init");

    // �t�F�[�h�C��/�A�E�g�p�̃p�����[�^�̏�����
    FadeIO.stop();

    // �X���b�h���쐬����
    Thread.createThread(threadFunction);

    g_bms.Load(".\\data\\snd\\"+g_SongTitle);

    bmsNum = 0;

    // �ݐσm�[�c���̏�����
    g_NotesNum = 0;

    // �Ȗ��̃Z�b�g
    songTitle = g_bms.GetTitle();

    // BPM�̃Z�b�g
    bpmLabel = g_bms.GetBPM();

    // �n�C�X�s�[�h�l���擾
    highSpeed = g_bms.GetHighSpeed();

    //�ݐσm�[�c���̏�����
    for (local i=0; i<8; i++){
        noteNum[i] = 0;
        judgeNum[i] = 0;
    }

    g_numAllNote = 0;

    // �S�m�[�c�̍ő吔���L�^
    for (local i=0; i<4; i++){
        g_noteSize[i] = g_bms.GetObjNum(g_LaneIndex[i]);
        g_numAllNote += g_noteSize[i]/2;
        print("noteSize["+i+"]="+g_noteSize[i]);
    }
    for (local i=4; i<8; i++){
        g_noteSize[i] = g_bms.GetObjNum(g_LaneIndex[i]);
        g_numAllNote += g_noteSize[i];
        print("noteSize["+i+"]="+g_noteSize[i]);
    }

    print("�S�m�[�c��:"+g_numAllNote);

    showJudge[0] = 0;
    showJudge[1] = 0;

    operationLock = false;

    //�Đ��J�n
    g_bms.Play();
}

function GamePlay::finalize()
{
    print("GamePlay final");

    Thread.stopThread();
}

function GamePlay::threadFunction()
{
    //print("Title Thread");
    // �t�F�[�h�C�����J�n����
    FadeIO.startFadeIn(30,0,0,0);

    // ���f�������s��
    if (Thread.wait(30)) return true;

    while (true)
    {
        if (GetJoypadInputStatePushed(INPUT_TYPE.ESC))
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
    g_bms.Stop();

    // �Q�[���V�[���ւƐ؂�ւ���
    ChangeScene("Result");

    return true;
}

// ���L�[���ꂼ�ꂪ������Ă��邩���ׂ�
function GamePlay::getHitArrowKey()
{
    if (GetJoypadInputStatePushed(INPUT_TYPE.LEFT))
    {
        print("LEFT Pushed");
    }
    else if (GetJoypadInputStatePushed(INPUT_TYPE.UP))
    {
        print("UP Pushed");
    }
    else if (GetJoypadInputStatePushed(INPUT_TYPE.RIGHT))
    {
        print("RIGHT Pushed");
    }
    else if (GetJoypadInputStatePushed(INPUT_TYPE.DOWN))
    {
        print("DOWN Pushed");
    }
}

// �e�p�b�h�L�[���ꂼ�ꂪ������Ă��邩���ׂ�
function GamePlay::getHitPadKey()
{
    for (local i=0; i<4; i++){
        if (GetJoypadInputStatePressed(g_LaneKey[i]))
        {
            // �{�^���̉����n�߂̎�
            if (!beforePress[i]){
                print("KEY "+(i+1)+" onPress");
                beforePress[i] = true;

                for (local j=judgeNum[i]; j<g_noteSize[i]; j+=2){
                    local ext = g_bms.NoteJudge(g_LaneIndex[i],j);

                    switch (ext)
                    {
                    case 1:
                        // ����
                        showJudge[0] = 0;
                        break;
                    case 2:
                        // ������
                        /*
                        if (g_bms.Score >= 0.2) g_bms.Score -= 0.2;
                        print ("Score:-0.2");
                        */
                        showJudge[0] = 1;   //���s
                        break;
                    case 3:
                        // ��₸��
                        g_bms.Score += 0.8;
                        print ("Score:+0.8");
                        showJudge[0] = 2;   //�߂�
                        break;
                    case 4:
                        // �s�b�^��
                        g_bms.Score += 1.0;
                        print ("Score:+1.0");
                        showJudge[0] = 3;
                        break;
                    }

                    // ��ԋ߂��m�[�c���������茗���Ȃ烋�[�v���f
                    if (ext != 0) break;
                }
            }
        }
        // �{�^������w�𗣂�����
        else if (beforePress[i]) {
            print("KEY "+(i+1)+" onUp");
            beforePress[i] = false;

            for (local j=judgeNum[i]; j<g_noteSize[i]; j++){
                local ext = g_bms.LongNoteJudge(g_LaneIndex[i],judgeNum[i]);

                // ��ԋ߂��m�[�c���������茗���Ȃ烋�[�v���f
                if (ext == 1) break;
            }
        }
    }

    for (local i=4; i<8; i++){
        if (GetJoypadInputStatePushed(g_LaneKey[i]))
        {
            //print("KEY "+(i+1)+" Pushed");
            for (local j=judgeNum[i]; j<g_noteSize[i]; j++){
                local ext = g_bms.NoteJudge(g_LaneIndex[i],j);

                switch (ext)
                {
                case 1:
                    // ����
                    showJudge[1] = 0;
                    break;
                case 2:
                    // ������
                    /*
                    if (g_bms.Score >= 0.2) g_bms.Score -= 0.2;
                    print ("Score:-0.2");
                    */
                    showJudge[1] = 1;
                    break;
                case 3:
                    // ��₸��
                    g_bms.Score += 0.8;
                    showJudge[1] = 2;
                    break;
                case 4:
                    // �s�b�^��
                    g_bms.Score += 1.0;
                    print ("Score:+1.0");
                    showJudge[1] = 3;
                    break;
                }

                // ��ԋ߂��m�[�c���������茗���Ȃ烋�[�v���f
                if (ext != 0) break;
            }
        }
    }
}

function GamePlay::getScoreData()
{
    local cnt;
    if (cnt in g_ScoreTable)
    {
        foreach (val in g_ScoreTable[cnt]){
            newNote(val);
        }
    }
}

function GamePlay::judgeNote()
{
    if (operationLock)
        return;

    // ����������
    for (local j=0; j<4; j++){
        for (local i=judgeNum[j]; i<g_noteSize[j]; i+=2){
            local tmp = g_bms.LongNoteJudgeOver(g_LaneIndex[j],i);
            if (tmp == 0) {
                showJudge[0] = 0;
                judgeNum[j] = i+2;
            }else{
                break;
            }
        }
    }

    for (local j=4; j<8; j++){
        for (local i=judgeNum[j]; i<g_noteSize[j]; i++){
            local tmp = g_bms.NoteJudgeOver(g_LaneIndex[j],i);
            switch(tmp)
            {
            // �I�u�W�F�N�g�����݂��Ȃ�
            case 0:
                break;
            // �I�u�W�F�N�g�͑��݂��Ă邪�A�܂����s���肪�o��ʒu�ł͂Ȃ�
            case 1:
                break;
            // �I�u�W�F�N�g�����݂��A�����s���肪�o��ʒu�ł���
            case 2:
                showJudge[1] = 1;
                break;
            }
            if (tmp == 0 || tmp == 2) judgeNum[j] = i+1;
            break;
        }
    }
}

// �X�V����(���t���[���Ă΂��)
function GamePlay::update()
{
    //if (!g_bms.isStart()){
    //    //�Đ��J�n
    //    g_bms.Play();
    //}

    judgeNote();

    getScoreData();
    getHitArrowKey();
    getHitPadKey();

    if (g_bms.isStart() && g_bms.isFinished()){
        print("Song Finished.");
        ChangeScene("Result");
    }

    // �X���b�h�����s����
    Thread.executeThread();

    // �t�F�[�h�C��/�A�E�g�p�̏������s��
    FadeIO.update();
}

// �Q�[���̈�̕`����s��
function GamePlay::drawGameArea()
{
    // �X�L���摜��\��
    DrawGraph(0, HALF_HEIGHT-381/2+15, getImgID("skin_game.png"));

    DrawGraph(GAME_AREA.LEFT, GAME_AREA.TOP, getImgID("lane_test.png"));

    /*
    // ���[������`�悷��
    for (local i=0; i<=LANE_NUM; i++){
        DrawLine(GAME_AREA.LEFT, GAME_AREA.TOP+NOTE_H*i, GAME_AREA.RIGHT, GAME_AREA.TOP+NOTE_H*i, BLUE);
    }
    */

    /*
    // ���[����`�悷��
    for (local i=0; i<LANE_NUM; i++){
        DrawLine(GAME_AREA.LEFT, GAME_AREA.TOP+NOTE_H*i+NOTE_H/2, GAME_AREA.RIGHT, GAME_AREA.TOP+NOTE_H*i+NOTE_H/2, GREEN);
    }
    */

    /*
    // �m�[�c�o�����C����`�悷��
    DrawBox(GAME_AREA.LEFT, GAME_AREA.TOP, GAME_AREA.LEFT+5, GAME_AREA.BOTTOM, GetColor(120,120,120), TRUE);
    DrawBox(GAME_AREA.RIGHT-4, GAME_AREA.TOP, GAME_AREA.RIGHT+1, GAME_AREA.BOTTOM, GetColor(120,120,120), TRUE);

    // ���[����`�悷��
    for (local i=0; i<LANE_NUM; i++){
        DrawLine(GAME_AREA.LEFT, GAME_AREA.TOP+NOTE_H*i+NOTE_H/2, GAME_AREA.RIGHT, GAME_AREA.TOP+NOTE_H*i+NOTE_H/2, GREEN);
    }

    // ���胉�C����`�悷��
    DrawBox(HALF_LINE - 3, GAME_AREA.TOP, HALF_LINE + 3, GAME_AREA.BOTTOM, RED, TRUE);
    */
}

function DrawNotes()
{
    local yy;//, index;

    // ����(�����O�m�[�c�I�����[)
    for (local i=0; i<4;i++)
    {
        //index = g_LaneIndex[i];
        //size = bms.GetBarNum(index);

        yy = LANE_Y[i]-30;

        //local startPos = (noteNum[i] > judgeNum[i]) ? noteNum[i] : judgeNum[i];

        for (local j=noteNum[i]; j<g_noteSize[i]; j+=2)
        {
            local note1_x = g_bms.GetObject_X(g_LaneIndex[i], j);
            //if (j+1 >= size) break;
            local note2_x = g_bms.GetObject_X(g_LaneIndex[i], j+1);

            // ���胉�C���𒴂����Ȃ玟�񂩂�͎��̏��߂���n�߂�
            if (note2_x < 0)
            {
                noteNum[i] = j+2;
                continue;
            }

            // �Q�[����ʊO�ɋ���Ȃ�A����ȍ~�̕`�ʂ̓X�L�b�v
            if (note1_x > GAME_AREA_WIDTH/2)
                break;

            // �����O�m�[�c�̐�[�����ɉ�ʊO�̎��͉����[�ɂ���
            if (note1_x < 0) note1_x = 0;

            // �����O�m�[�c�̖������܂���ʊO�̎��͉��𖖔��ɂ���
            if (note2_x > GAME_AREA_WIDTH/2) note2_x = GAME_AREA_WIDTH/2;

            // ��ʓ��Ȃ�`�悷��
            DrawBox(note1_x+GAME_AREA.LEFT, yy, note2_x+GAME_AREA.LEFT, yy+NOTE_H, RED,TRUE);
        }
    }

    // �E��
    for (local i=4; i<8; i++)
    {
        //index = g_LaneIndex[i];
        //size = bms.GetObjNum(index);

        yy = LANE_Y[i-4];

        local startPos = (noteNum[i] > judgeNum[i]) ? noteNum[i] : judgeNum[i];

        //print("ObjNum:"+(i+11)+"->"+bms.GetObjNum(index));
        for (local j=startPos; j<g_noteSize[i]; j++)
        {
            local note_x = g_bms.GetObject_X(g_LaneIndex[i], j);

            // ���胉�C���𒴂����Ȃ玟�񂩂�͎��̏��߂���n�߂�
            if (GAME_AREA.RIGHT-note_x > GAME_AREA.RIGHT)
            {
                noteNum[i] = j+1;
                continue;
            }

            // �Q�[����ʊO�ɋ���Ȃ�A����ȍ~�̕`�ʂ̓X�L�b�v
            if (note_x > GAME_AREA_WIDTH/2)
                break;

            // ��ʓ��Ȃ�`�悷��
            local xx = GAME_AREA.RIGHT-note_x;
            //DrawLine(GAME_AREA.RIGHT-note_x, yy, GAME_AREA.RIGHT-note_x, yy+NOTE_H, RED);
            //DrawBox(xx-NOTE_W/2, yy, xx+NOTE_W/2, yy+NOTE_H, RED, TRUE);
            DrawGraph(xx-NOTES_W/2, yy-NOTES_H/2, getImgID("note1.png"));
        }
    }
}

//���ߐ��̕`����s��
function DrawBarLine()
{
    for (local i=bmsNum; i<g_bms.GetBarNum(); i++)
    {
        local xx = g_bms.GetBarLine_X(i);
        //print("xx="+xx);

        // ���胉�C���𒴂����Ȃ玟�񂩂�͎��̏��߂���n�߂�
        if (xx < 0) {
            bmsNum = i+1;
            continue;
        }

        // �Q�[����ʊO�ɋ���Ȃ�A����ȍ~�̕`�ʂ̓X�L�b�v
        if (xx > GAME_AREA_WIDTH/2)
            break;

        // ��ʓ��Ȃ�`�悷��
        // ���֗����
        DrawLine(xx+GAME_AREA.LEFT, GAME_AREA.TOP, xx+GAME_AREA.LEFT, GAME_AREA.BOTTOM, GetColor(255,255,255));
        // �E�֗����
        DrawLine(GAME_AREA.RIGHT-xx, GAME_AREA.TOP, GAME_AREA.RIGHT-xx, GAME_AREA.BOTTOM, GetColor(255,255,255));
    }
}

function GamePlay::drawInfoBanner()
{
    // �Q�[�W�̕`��
    DrawGraph(WINDOW_WIDTH-632, WINDOW_HEIGHT-120, getImgID("infogauge_play.png"));

    // �Q�[�W�摜�̕`��
    for (local i=0; i<20; i++){
        DrawGraph(WINDOW_WIDTH-608+(25+4)*i, WINDOW_HEIGHT-48, getImgID((i<11) ? "gauge_ep_w.png" : "gauge_ep_r.png"));
    }

    // �X�R�A�̎擾
    local sc = (g_bms.Score/g_numAllNote*100).tointeger();

    // �擾�ς݃Q�[�W�摜�̕`��
    for (local i=0; i<(sc/5).tointeger(); i++)
    {
        DrawGraph(WINDOW_WIDTH-608+(25+4)*i, WINDOW_HEIGHT-48, getImgID((i<11) ? "gauge_ch_w.png" : "gauge_ch_r.png"));
    }

    // �B�����̕`��
    DrawString(WINDOW_WIDTH-82, WINDOW_HEIGHT-120, sc, GetColor(255,255,255), 30, 0);

    // ���o�i�[�̕`��
    DrawGraph(WINDOW_WIDTH-570, WINDOW_HEIGHT-130, getImgID("infobanner1_play.png"));

    // ��Փx�摜�̕`��
    switch(g_selectedLevel){
        case 0:
            DrawGraph(WINDOW_WIDTH-520, WINDOW_HEIGHT-115, getImgID("levelE.png"));
            break;
        case 1:
            DrawGraph(WINDOW_WIDTH-520, WINDOW_HEIGHT-115, getImgID("levelN.png"));
            break;
        case 2:
            DrawGraph(WINDOW_WIDTH-520, WINDOW_HEIGHT-115, getImgID("levelH.png"));
            break;
    }

    //�@���x�̕`��
    DrawString(380,WINDOW_HEIGHT-130+28,highSpeed/0.5,GetColor(255,255,255),20,0);

    //�@BPM�̕`��
    DrawString(380,WINDOW_HEIGHT-130,bpmLabel,GetColor(255,255,255),20,0);
}

// �`�揈��(���t���[���Ă΂��)
function GamePlay::draw()
{
    //�w�i��`�悷��
    DrawGraph(0, 0, getImgID("bg_gamescene.jpg"));

    // �v���C�̈��`��
    drawGameArea();

    // �L�[�r�[���̕`��
    for (local i=0; i<8; i++){
        if (GetJoypadInputStatePressed(g_LaneKey[i])){
            // ������
            if (i < 4)
                DrawGraph(GAME_AREA.LEFT, GAME_AREA.TOP+NOTE_H*i, getImgID("keybeam.png"));
            else
                DrawTurnGraph(GAME_AREA.RIGHT-135, GAME_AREA.TOP+NOTE_H*(i-4), getImgID("keybeam.png"), TRUE);
        }
    }

    if(!operationLock){
        // ���ߐ���`��
        DrawBarLine();

        // �m�[�c�̕`��
        DrawNotes();
    }

    // ���o�[��`��
    drawInfoBanner();

    // �Ȗ��̕`��
    //DrawString(20,20,songTitle,GetColor(255,255,255),20,0);

    // ���蕶���̕`��
    switch (showJudge[0])
    {
        case 1:
            DrawGraph(10,10,getImgID("judge_miss.png"));
            break;
        case 2:
            DrawGraph(10,10,getImgID("judge_near.png"));
            break;
        case 3:
            DrawGraph(10,10,getImgID("judge_great.png"));
            break;
    }
    switch (showJudge[1])
    {
        case 1:
            DrawGraph(WINDOW_WIDTH-10-90,10,getImgID("judge_miss.png"));
            break;
        case 2:
            DrawGraph(WINDOW_WIDTH-10-90,10,getImgID("judge_near.png"));
            break;
        case 3:
            DrawGraph(WINDOW_WIDTH-10-90,10,getImgID("judge_great.png"));
            break;
    }

    // �t�F�[�h�C��/�A�E�g�p�̏������s��
    FadeIO.draw();
}
