#ifndef _DEBUG_H
#define _DEBUG_H
///////////////////////////////////////////////////////////////////////////////////
// DEBUG : �f�o�b�O�e�L�X�g�o�̓��C�u���� v1.00                                  //
//   �f�o�b�O���[�h���n�m�ɂ���ɂ͂��̃t�@�C�����C���N���[�h����O�ɁA          //
//   �u#define DEBUGMODE�v���`���Ă��������B                                   //
//   �܂��A���̒�`���R�����g�����邱�ƂŁA�o�͂𖳌��ɂ��邱�Ƃ��\�ł��B      //  
//                                                                               //
// ���̃\�[�X�R�[�h�͎��R�ɉ��ς��Ďg�p�\�ł��B                                //
// �܂����p���p���\�ł����A���ׂĂ̊��Ő��������삷��ۏ�͂���܂���B      //
//                          http://www.charatsoft.com/                           //
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
// ���L��#undef��L���ɂ��邱�ƂŁA�ecpp�Œ�`���ꂽDEBUGMODE���A                //
// ���ׂĖ��������邱�Ƃ��o���܂��B                                              //
// �ŏI�����[�X�ł��쐬���鎞�́A�f�o�b�O�o�͂𖳌������Ă��������B              //
///////////////////////////////////////////////////////////////////////////////////

//#undef DEBUGMODE
#define DEBUGMODE

// �f�o�b�O���[�h�Ȃ�
#ifdef	DEBUGMODE
#include <stdio.h>
#include <time.h>
#define INITDEBUG {\
time_t aclock;\
FILE *_fp;\
_fp=fopen("DEBUG.TXT","w");\
if(_fp) {\
time(&aclock);\
fputs(ctime(&aclock) ,_fp);\
fclose(_fp);\
}\
}

// �f�o�b�O����ǉ�����
#define DEBUG(x,y) {\
FILE *_fp;\
_fp=fopen("DEBUG.TXT","a");\
if(_fp){\
fprintf(_fp,x,y);\
fclose(_fp);\
}\
}

// �f�o�b�O����ǉ�����
#define DEBUG2(x,y,z) {\
FILE *_fp;\
_fp=fopen("DEBUG.TXT","a");\
if(_fp){\
fprintf(_fp,x,y,z);\
fclose(_fp);\
}\
}

#else

// �f�o�b�O���[�h�łȂ���Ή������Ȃ�
#define INITDEBUG		{;}
#define DEBUG(x,y)		{;}
#define DEBUG2(x,y,z)	{;}

#endif

#endif

