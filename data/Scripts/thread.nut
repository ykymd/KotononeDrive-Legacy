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
