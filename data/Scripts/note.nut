// �Q�[����ʂ̃m�[�c
class Note {
    // ���W
    x = 0.0;
    y = 0.0;

    // �傫��
    w = NOTE_W;
    h = NOTE_H;

    // ���B�t���[��(���胉�C���s�b�^���ɂȂ�Ƃ��̃t���[��)
    frame = 0;

    // ���x(��Βl�w��)
    v = 0.1;

	//���[���ԍ�
	lane = LANE_ID.NONE;

	// ���݃t���O
	isExist = false;


    // �R���X�g���N�^
    constructor(lane, v, frame){
        this.v = abs(v);
		this.lane = lane;
		this.frame = frame;

		setPosition();
    }

    //�@�m�[�c����ʂɕ\��������
    function initNote()
    {
    	this.isExist = true;
    }

    // �����O�m�[�c�̒�����ݒ肷��
    function setNoteWidth(width)
    {
    	this.w = width;
    }

	function setPosition()
	{
		switch (lane){
		case LANE_ID.L1:
			x = HALF_LINE;
			y = LANE1_Y;
			v *= -1;
			break;
		case LANE_ID.L2:
			x = HALF_LINE;
			y = LANE2_Y;
			v *= -1;
			break;
		case LANE_ID.L3:
			x = HALF_LINE;
			y = LANE3_Y;
			v *= -1;
			break;
		case LANE_ID.L4:
			x = HALF_LINE;
			y = LANE4_Y;
			v *= -1;
			break;
		case LANE_ID.R1:
			x = HALF_LINE;
			y = LANE1_Y;
			break;
		case LANE_ID.R2:
			x = HALF_LINE;
			y = LANE2_Y;
			break;
		case LANE_ID.R3:
			x = HALF_LINE;
			y = LANE3_Y;
			break;
		case LANE_ID.R4:
			x = HALF_LINE;
			y = LANE4_Y;
			break;
		case LANE_ID.NONE:
			break;
		}
	}

	function checkOver(cnt)
	{
		if (x+w>=GAME_AREA.RIGHT || x<=GAME_AREA.LEFT)
		{
			print ("Remove:"+cnt);
			this.isExist = false;
		}
	}

	function onUpdate(cnt)
	{
		if (isExist){
			checkOver(cnt);
			x += v;
		}
	}

	function onDraw()
	{
		if (isExist){
			DrawBox(x, y, x + w, y + h, GetColor(255,255,0), TRUE);
		}
	}
};