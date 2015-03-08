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