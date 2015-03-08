//const.nut
//
// const.nut
// ���ꂼ��̒萔�l��񋓒l���L�q����
//

// �E�C���h�E�̃T�C�Y
const WINDOW_WIDTH = 640;
const WINDOW_HEIGHT = 480;

// �v���C�̈�̃T�C�Y
enum GAME_AREA
{
    LEFT = 50,
    TOP = 80,
    RIGHT = 590,
    BOTTOM = 320,
};

// �v���C�̈�̕�
const GAME_AREA_WIDTH = 540;
const GAME_AREA_HEIGHT = 240;

// �Q�[����ʂ̒��S���C��
const HALF_LINE = 320;

// �Q�[����ʂ̐^�񒆂̉�����Y���W
const HALF_HEIGHT = 200;

// �e���[����ID�BLR�͂��ꂼ�ꍶ�E�A�����͏ォ�牽�Ԗڂ̃��[������\���Ă���
enum LANE_ID
{
    L1 = 0,
    L2 = 1,
    L3 = 2,
    L4 = 3,
    R1 = 4,
    R2 = 5,
    R3 = 6,
    R4 = 7,
    NONE = -1,
};

// ���[���̐�
const LANE_NUM = 4;

// �e���[����Y���W
const LANE1_Y = 80;
const LANE2_Y = 140;
const LANE3_Y = 200;
const LANE4_Y = 260;

// �m�[�c�̉摜��
const NOTE_W = 15.0;
const NOTE_H = 60.0;

const NOTES_W = 30.0;
const NOTES_H = 80.0;

LANE_Y <- [110, 170, 230, 290];

/*
// ���茋��
enum JUDGE
{
    Critical,       // �s�b�^��
    Near,           // ��₸��Ă�
    Failed,         // ���s
    None,           //�@����
}
*/

// �Z���N�g��ʂ̋ȕ\���E�C���h�E
const WND_W=90;
const WND_H=286;

// �ȕ\���E�C���h�E�̊Ԋu
const WND_INTERVAL = 10;

// �Z���N�g��ʂ̐��̃T�C�Y
const STAR_W = 20;
const STAR_H = 20;

// ��Փx�\���̐��̈ʒu
const DIFFSTAR_X = 220;
const DIFFSTAR_Y1 = 200;
const DIFFSTAR_Y2 = 233;
const DIFFSTAR_Y3 = 265;

// �n�C�X�s�[�h�̌��E�l
const HIGHSPEED_MAX = 9;
const HIGHSPEED_MIN = 1;


//var.nut
//
// var.nut
// �O���[�o���ϐ����L�q����
// �ϐ����̐擪��"g_"��t����
//

RED <- GetColor(255,0,0);
GREEN <- GetColor(0,255,0);
BLUE <- GetColor(0,0,255);

// �m�[�c�̐�(g_notesTable�̔ԍ��Ɏg��)
// 1�v���C���ɏo�Ă����ݐσm�[�c��
g_NotesNum <- 0;

// �S�m�[�c���i�[����e�[�u��(Note�N���X)
// ���݉�ʂɂ���m�[�c�̃C���X�^���X�̂݊i�[
g_NotesTable <- {};

// �y���f�[�^���i�[����z��
g_ScoreTable <- {};

g_LaneIndex <- [56, 51, 52, 53, 14, 15, 18, 19];

// �y�Ȉꗗ�̃f�[�^
//g_SongData <- {};

// �y�Ȉꗗ�̗v�f��
g_songNum <- 0;

// �e���[���ɑΉ����Ă���L�[
g_LaneKey <- [
    INPUT_TYPE.ONE,
    INPUT_TYPE.TWO,
    INPUT_TYPE.THREE,
    INPUT_TYPE.FOUR,
    INPUT_TYPE.FIVE,
    INPUT_TYPE.SIX,
    INPUT_TYPE.SEVEN,
    INPUT_TYPE.EIGHT,
    ];

// �Đ�����y�Ȃ̃^�C�g��
g_SongTitle <- "kotonone_the_hard.bme";

g_bms <- KotononeBMS();

// ��Փx�\���̐��̈ʒu
g_DIFFSTAR_Y <- [DIFFSTAR_Y1,DIFFSTAR_Y2,DIFFSTAR_Y3];

// �m�[�c�̃n�C�X�s�[�h
//g_highSpeed <- 1.0;

// ��Փx�̉摜
//g_levelImage = ["levelE.png", "levelN.png", "levelH.png"];

//�I�����ꂽ��Փx
g_selectedLevel <- 0;

//�ǂݍ��񂾕��ʂ̑S�m�[�c��
g_numAllNote <- 0;

//�e���[���̑S�m�[�c��
g_noteSize <- array(8,0);

// ���U���g��ʂ̕\��
g_resultNum <- [0, 0, 0, 0];

// ���U���g�摜
g_resultStill <- ["result1.png", "result2.png", "result3.png"];
g_nasubiStill <- ["nasubi1.png", "nasubi2.png"];


//FadeIO.nut
/* �t�F�[�h�C��/�A�E�g���� */

// ���O��ԗp�̃e�[�u���ƕϐ����`����
FadeIO <- {
    // �t�F�[�h�C��/�A�E�g���������s�����ۂ�
    FadeIn_Flag = false,
    FadeOut_Flag = false,

    // �t�F�[�h�C��/�A�E�g�p�̃J�E���^
    FadeIO_Counter = 0,

    // �t�F�[�h�C��/�A�E�g�̐F
    FadeIO_Color = 0,

    // �t�F�[�h�C��/�A�E�g�̏����Ŗ�������Z����l
    FadeIO_Num = 0,
};

// �t�F�[�h�C���������J�n����֐�
function FadeIO::startFadeIn(frame, r, g, b)
{
    FadeIn_Flag = true;
    FadeOut_Flag = false;
    FadeIO_Counter = 0;
    FadeIO_Color = GetColor(r, g, b);
    FadeIO_Num = 255 / frame;
}

// �t�F�[�h�A�E�g�������J�n����֐�
function FadeIO::startFadeOut(frame, r, g, b)
{
    FadeIn_Flag = false;
    FadeOut_Flag = true;
    FadeIO_Counter = 0;
    FadeIO_Color = GetColor(r, g, b);
    FadeIO_Num = 255 / frame;
}

// �t�F�[�h�C��/�A�E�g�������I������֐�
function FadeIO::stop()
{
    FadeIn_Flag = false;
    FadeOut_Flag = false;
    FadeIO_Counter = 0;
    FadeIO_Color = 0;
    FadeIO_Num = 0;
}

// �t�F�[�h�C��/�A�E�g�p�̍X�V����
function FadeIO::update()
{
    // �t�F�[�h�C��/�A�E�g�p�̃J�E���^���C���N�������g����
    if(FadeIn_Flag || FadeOut_Flag)
        FadeIO_Counter++;
}

// �t�F�[�h�C��/�A�E�g�p�̕`�揈��
function FadeIO::draw()
{
    // �t�F�[�h�C��/�A�E�g�p�̋�`�ŉ�ʂ�h��Ԃ�
    if(FadeIn_Flag)
    {
        local param = 255 - FadeIO_Num * FadeIO_Counter;
        if(param > 0)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, param);
            DrawBox(0, 0, 640, 480, FadeIO_Color, TRUE);

            // �u�����h���[�h�̃p�����[�^�����ɖ߂�
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        }
    }
    else if(FadeOut_Flag)
    {
        local param = FadeIO_Num * FadeIO_Counter;
        if(param > 255) param = 255;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, param);
        DrawBox(0, 0, 640, 480, FadeIO_Color, TRUE);

        // �u�����h���[�h�̃p�����[�^�����ɖ߂�
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    }
}


//thread.nut
// �X���b�h�p�̖��O���
Thread <- {
    Thread_Object = null,
};

// �X���b�h���쐬����
function Thread::createThread(func)
{
    // ���f��Ԃł���΃X���b�h���I��
    stopThread();

    // �I�u�W�F�N�g�̍쐬
    Thread_Object = newthread(func);
}

// �X���b�h���I������
function Thread::stopThread()
{
    if(Thread_Object != null)
    {
        if(Thread_Object.getstatus() != "idle")
        {
            Thread_Object.wakeup(true);
        }
    }
}

// �X���b�h�����s/�ĊJ����֐�
function Thread::executeThread()
{
    // �X���b�h�I�u�W�F�N�g���쐬����Ă���A
    if(Thread_Object != null)
    {
        // ���f��ԂȂ�A�ĊJ����
        if( Thread_Object.getstatus() == "suspended" )
        {
            return Thread_Object.wakeup(false);
        }
            // �A�C�h����ԂȂ�A���s����
        else if( Thread_Object.getstatus() == "idle" )
        {
            return Thread_Object.call();
        }
    }
}

// �����Ɏw�肵���񐔁A���f�������s���֐�
function Thread::wait(frame)
{
    for(local i=0; i<frame; i++)
    {
        if( suspend(false) ) return true;
    }
    return false;
}

// �L�[���������܂Œ��f�������s���֐�
function Thread::waitPushKey()
{
    do {
        if( suspend(false) ) return true;
    } while( !GetJoypadInputStatePushed(INPUT_TYPE.ONE) );

    return false;
}


//resource.nut
// �e�[�u��������w��摜��ID���擾����
function getImgID(file_name, index=0)
{
    return g_ResourceList[file_name][index].img_id;
}

// �摜���e�[�u�������ɁA�摜�t�@�C����ǂݍ���
function loadResource()
{
    // �S�Ẳ摜��ǂݍ���
    foreach(file_name, img_chip_arr in g_ResourceList)
    {
        // �摜�t�@�C����ǂݍ���
        local tmp_handle = LoadGraph(".\\data\\img\\" + file_name);

        // �摜��1���������ꍇ�͂��̉摜�݂̂��Z�b�g����
        if( img_chip_arr.len() == 1 )
        {
            img_chip_arr[0].img_id = tmp_handle;
        }
        // �摜������������ꍇ�͈ꕔ�̉摜��img_id�ɃZ�b�g����
        else
        {
            foreach(val in img_chip_arr)
            {
                val.img_id = DerivationGraph(val.x, val.y, val.w, val.h, tmp_handle);
            }
        }
    }

    /*
    // ?A?j???[?V??????mg_id??????????
    foreach(key, anime_group in g_AnimeInfoList)
    {
        foreach(key, anime_info in anime_group)
        {
            foreach(val in anime_info.anime)
            {
                val.img_id = g_ResourceList[val.img_file][val.img_idx].img_id;
            }
        }
    }
    */
}

// �g�p����f�ނ̃��X�g
g_ResourceList <- {
    ["bg_gamescene.jpg"] = [
        { img_id=-1, },
    ],
    ["skin_game.png"] = [
        { img_id=-1, },
    ],
    ["note1.png"] = [
        { img_id=-1, },
    ],
    ["Logo.png"] = [
        { img_id=-1, },
    ],
    ["title.png"] = [
        { img_id=-1, },
    ],
    ["songbanner_selected.png"] = [
        { img_id=-1, },
    ],
    ["songbanner_unselected.png"] = [
        { img_id=-1, },
    ],
    ["infobanner1_select.png"] = [
        { img_id=-1, },
    ],
    ["levelselect2.png"] = [
        { img_id=-1, },
    ],
    ["infogauge_play.png"] = [
        { img_id=-1, },
    ],
    ["infobanner1_play.png"] = [
        { img_id=-1, },
    ],
    ["star_ep_w.png"] = [
        { img_id=-1, },
    ],
    ["star_ep_y.png"] = [
        { img_id=-1, },
    ],
    ["star_ch_w.png"] = [
        { img_id=-1, },
    ],
    ["star_ch_y.png"] = [
        { img_id=-1, },
    ],
    ["lane_test.png"] = [
        { img_id=-1, },
    ],
    ["infobanner2_mode_mu.png"] = [
        { img_id=-1, },
    ],
    ["infobanner2_mode_lv.png"] = [
        { img_id=-1, },
    ],
    ["levelE.png"] = [
        { img_id=-1, },
    ],
    ["levelN.png"] = [
        { img_id=-1, },
    ],
    ["levelH.png"] = [
        { img_id=-1, },
    ],
    ["gauge_ep_w.png"] = [
        { img_id=-1, },
    ],
    ["gauge_ep_r.png"] = [
        { img_id=-1, },
    ],
    ["gauge_ch_w.png"] = [
        { img_id=-1, },
    ],
    ["gauge_ch_r.png"] = [
        { img_id=-1, },
    ],
    ["nasubi1.png"] = [
        { img_id=-1, },
    ],
    ["nasubi2.png"] = [
        { img_id=-1, },
    ],
    ["result1.png"] = [
        { img_id=-1, },
    ],
    ["result2.png"] = [
        { img_id=-1, },
    ],
    ["result3.png"] = [
        { img_id=-1, },
    ],
    ["keybeam.png"] = [
        { img_id=-1, },
    ],
    ["judge_great.png"] = [
        { img_id=-1, },
    ],
    ["judge_miss.png"] = [
        { img_id=-1, },
    ],
    ["judge_near.png"] = [
        { img_id=-1, },
    ]
};


//SongList.nut
// �y�Ȃ̃��X�g
g_SongData <- {
    [0] =
        {
            title = "7th Cruise",
            composer = "�݂邭",
            genre = "Ambient House",
            bpm = 171,
            filename = "7thCruise",
            level = [4, 8, 10],
        },

    [1] =
        {
            title = "Double Dagger",
            composer = "Trimally",
            genre = "Techno",
            bpm = 212,
            filename = "Double Dagger",
            level = [3, 8, 10],
        },
    [2] =
        {
            title = "RAYNs",
            composer = "�݂邭",
            genre = "Rock",
            bpm = 180,
            filename = "RAYNs",
            level = [3, 5, 9],
        },
    [3] =
        {
            title = "SOLID CITY",
            composer = "Rocky",
            genre = "Rock",
            bpm = 170,
            filename = "SOLID CITY",
            level = [1, 4, 7],
        },

    // SweetPanic
    [4] =
        {
            title = "Sweet�~Panic!",
            composer = "mossari",
            genre = "Happy Hardcore",
            bpm = 190,
            filename = "SweetPanic",
            level = [2, 6, 8],
        },

    [5] =
        {
            title = "Torrid Zone",
            composer = "mossari",
            genre = "Tech Dance",
            bpm = 150,
            filename = "Torrid Zone",
            level = [2, 7, 10],
        },

    [6] =
        {
            title = "�R�g�m�l�h���C�u",
            composer = "Rocky",
            genre = "Theme",
            bpm = 180,
            filename = "kotonone_the",
            level = [1, 5, 9],
        },

    [7] =
        {
            title = "�ՌÎ�Ƃ���",
            composer = "Yokohei",
            genre = "Hard Style",
            bpm = 180,
            filename = "kotohu king",
            level = [4, 8, 10],
        },
};


//songwnd.nut
class SongWnd {
    // ���W(����)
    x = 0.0;
    y = 0.0;

    // �傫��
    w = WND_W;
    h = WND_H;

    // ID (0���^�񒆂ł���I�𒆏�ԁBID�͐�������ő��ΓI�Ȉʒu�֌W������)
    id = 0;

    fontHandle = 0;

    // �ȃf�[�^
    SongData = {};

    // �R���X�g���N�^
    constructor(dat)
    {
        this.SongData = dat;
        fontHandle = CreateFontToHandle("@�l�r ����", -1, -1, -1);
    }

    function setID(i)
    {
        this.id = i;
        x = WINDOW_WIDTH/2-WND_W/2 + WND_INTERVAL*id + WND_W*id;
        y = WINDOW_HEIGHT/2-WND_H/2;

        if (i==0)
            g_SongTitle = SongData.title;
    }

    function getSongFileName()
    {
        return SongData.filename;
    }

    // �X�V����
    function onUpdate()
    {
    }

    //�@�`�揈��
    function onDraw()
    {
        if (id < 4 && id > -4){
            DrawGraph(x, y, getImgID((id==0) ? "songbanner_selected.png" : "songbanner_unselected.png"));
            DrawString(x+10,y+WND_H-45,"BPM",GetColor(255,255,255),20,0);
            DrawString(x+WND_W-40,y+WND_H-45,SongData.bpm,GetColor(255,255,255),20,0);

            // �^�C�g����`��
            DrawSongTitleVString(x+WND_W/2+5, y+10, SongData.title, GetColor(255,255,255));

            // �R���|�[�U�[��`��
            DrawSongSubVString(x+30, y+60, SongData.composer, GetColor(255,255,255));

            // �W��������`��
            DrawSongSubVString(x+5, y+60, SongData.genre, GetColor(255,255,255));
        }
    }
}


//Logo.nut
//�@���S�\����ʂ̃N���X
Logo <- {};

function Logo::initialize()
{
    print("Logo init");

    // �t�F�[�h�C��/�A�E�g�p�̃p�����[�^�̏�����
    FadeIO.stop();

    // �^�C�g����ʗp�̃X���b�h���쐬����
    Thread.createThread(threadFunction);
}

function Logo::finalize()
{
    print("Logo final");

    Thread.stopThread();
}

function Logo::threadFunction()
{
    //print("Title Thread");
    // �t�F�[�h�C�����J�n����
    FadeIO.startFadeIn(90,0,0,0);

    // ���f�������s��
    if (Thread.wait(180)) return true;

    // �t�F�[�h�A�E�g�������J�n����
    FadeIO.startFadeOut(45, 255, 255, 255);

    // ���΂炭�҂�
    if( Thread.wait(120) ) return true;

    // �Q�[���V�[���ւƐ؂�ւ���
    ChangeScene("Title");

    return true;
}

function Logo::update()
{
    // �X���b�h�����s����
    Thread.executeThread();

    // �t�F�[�h�C��/�A�E�g�p�̏������s��
    FadeIO.update();
}

function Logo::draw()
{
    // ���S�̕`��
    DrawGraph(0,0,getImgID("Logo.png"));

    // �t�F�[�h�C��/�A�E�g�p�̏������s��
    FadeIO.draw();
}


//Title.nut
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


//Select.nut
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
                ChangeScene("Title");
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


//gameplay.nut
//�@�Q�[����ʂ̃N���X
GamePlay <-{
    cnt = 0,
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

    g_numAllNote = 0;

    // �Ȗ��̃Z�b�g
    songTitle = g_bms.GetTitle();

    // BPM�̃Z�b�g
    bpmLabel = g_bms.GetBPM();

    // �n�C�X�s�[�h�l���擾
    highSpeed = g_bms.GetHighSpeed();

    //���샍�b�N������
    operationLock = false;

    //noteSize�̏�����
    for (local i=0; i<8; i++){
        g_noteSize[i] = 0;
    }

    //�@�m�[�c�̊i�[�e�[�u���̏�����
    //g_NotesTable = {};

    // �^�C�g����ʗp�̃X���b�h���쐬����
    //Thread.createThread(threadFunction);

    //newNote({lane=LANE_ID.L1, speed=1.0, frame=0});
    //newNote({lane=LANE_ID.R2, speed=3.0, frame=0});

    //foreach (key, val in g_NotesTable){
    //    val.initNote();
    //}

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

    //�X�R�A�̃��Z�b�g
    g_bms.Score = 0;

    //�Đ��J�n
    g_bms.Play();
}

function GamePlay::finalize()
{
    print("GamePlay final");
    g_bms.Stop();
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
            print("Song Cancelled");
            g_bms.Stop();
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

    GamePlay.operationLock = true;    //���샍�b�N��������

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
    if (cnt in g_ScoreTable)
    {
        foreach (val in g_ScoreTable[cnt]){
            newNote(val);
        }
    }
}

// �X�V����(���t���[���Ă΂��)
function GamePlay::update()
{
    cnt++;

    if (!operationLock){
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

        getScoreData();
        getHitArrowKey();
        getHitPadKey();
    }

    if (g_bms.isFinished()){
        //g_bms.Clear();
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

    // �f�o�b�O�p�t���[��
    //DrawString(0,0,"Frame:"+cnt,GetColor(255,255,255),20,0);

    // �t�F�[�h�C��/�A�E�g�p�̏������s��
    FadeIO.draw();
}



//result.nut
//�@���S�\����ʂ̃N���X
Result <- {
    //soundHandle = 0,  //�����p�̃n���h��
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



//boot.nut

// �Q�[���N�����ɌĂяo�����֐�
function boot()
{
    // �摜�t�@�C���Q��ǂݍ���
    loadResource();

    // �����t�@�C��������������
    InitSoundMem();

    // �^�C�g���V�[���ւƐ؂�ւ���
    ChangeScene("Logo");
}
