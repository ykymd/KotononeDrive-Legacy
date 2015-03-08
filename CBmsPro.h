#ifndef _CBMSPRO_H
#define _CBMSPRO_H
///////////////////////////////////////////////////////////////////////////////////
// CBmsPro : BMS�N���X �`���[�g���A���o�[�W���� v3.0                             //
//                                                                               //
// ���̃\�[�X�R�[�h�͎��R�ɉ��ς��Ďg�p�\�ł��B                                //
// �܂����p���p���\�ł����A���ׂĂ̊��Ő��������삷��ۏ�͂���܂���B      //
//                          http://www.charatsoft.com/                           //
///////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <stdio.h>
#include <unordered_map>
#include <vector>

#include "BMSHeader.h"

const LONG BMSDATA_RESOLUTION = 9600;		// �P���߂̃J�E���g�l
const LONG BMSMAXBUFFER = 256;				// 00�`FF�܂ł̃o�b�t�@��

// BMS�`�����l����`
enum class CHANNEL
{
	BACKMUSIC	= 0x01,		// ���̈ʒu�ɂ�����A�����I�ɍĐ������WAV���w�肵�܂�
	STRETCH		= 0x02,		// ���̏��߂̒������`�����f�[�^�{���܂��i�P�O�i���A�����Ƃ��Ɏg�p�j
	TEMPO		= 0x03,		// �Đ��e���|�i�a�o�l / �P���Ԃ̎l���������j�̓r���ύX�i�P�U�i���j
	BACKANIME	= 0x04,		// �o�b�N�O���E���h�A�j���[�V�����@�\
	EXTENEDOBJ	= 0x05,		// �������Ă���I�u�W�F��ʂ̃L�����N�^�[�i���o�[�̂��̂ɂ���ւ���@�\
	CHGPOORANI	= 0x06,		// POOR���o�����Ƃ��ɕ\�������摜��ύX
	LAYERANIME	= 0x07,		// Ch.04�Ŏw�肵��BGA�̏�ɂ��Ԃ���BMP���w��ł��܂�
	BPMINDEX	= 0x08,		// BPM�̃C���f�b�N�X�w��(�V)
};

// BMS�R�}���h�̈ꗗ
enum class Command {
	PLAYER,
	GENRE,
	TITLE,
	ARTIST,
	BPM,
	MIDIFILE,
	PLAYLEVEL,
	RANK,
	VOLWAV,
	TOTAL,
	STAGEFILE,
	WAV,
	BMP,
	SONG_GUID,
	MOVIE,
	OBJECT,
	UNKNOWN,
};

// BMS�f�[�^���
typedef struct _BMSDATA {
	BOOL		bFlag;					// ���݂��Ă��邩
	LONGLONG	llTime;					// �炷�^�C�~���O�f�[�^
	LONG		lData;					// �炷�f�[�^�i0�`255�j
	float		fData;					// �����_�f�[�^�i�e���|�Ȃǁj
} BMSDATA,*LPBMSDATA;

// ���ߏ��
typedef struct _BMSBAR {
	BOOL		bFlag;					// ���݂��邩�i�����j
	LONGLONG	llTime;					// �\�����鎞��
	LONG		lData;					// ���߂ɂ��Ă̒ǉ����i�����j
} BMSBAR,*LPBMSBAR;

// BPM�C���f�b�N�X
typedef struct _BPMINDEX {
	float		fBpm;					// BPM
} BPMINDEX,*LPBPMINDEX;

// ���߂̒����Ǘ�
typedef struct _BARMAGNI {
	long	iLines;						// ���ߔԍ�
	float	fMagni;						// ���̔{��
} BARMAGNI,*LPBARMAGNI;

// BMS�t�@�C���w�b�_���
/*
typedef struct _BMSHEADER {
	long		Player;					// �v���C���[�h
	string		Genre;					// �f�[�^�̃W������
	string		Title;					// �f�[�^�̃^�C�g��
	string		Artist;					// �f�[�^�̐����
	float		Bpm;					// �f�[�^�̃e���|�i�����l��130�j
	char		Midifile[MAX_PATH];		// �o�b�N�O���E���h�ŗ���MIDI�t�@�C��
	long		Playlevel;				// �f�[�^�̓�Փx
	long		Rank;					// �f�[�^�̔��胉���N
	long		WavVol;					// ���ʂ����̉����ɂ��邩
	long		Total;					// �Q�[�W�̑��ʂ�ݒ�
	char		StagePic[MAX_PATH];		// �ȊJ�n���ɕ\������摜
	char		Movie[MAX_PATH];		// ���[�r�[�t�@�C����
	GUID		guid;					// �Ȃ�GUID(����d�l)
	long		EndBar;					// �I������
	LONGLONG	MaxCount;				// �ő�̃J�E���g��
	BPMINDEX	mBpmIndex[BMSMAXBUFFER];// �e���|�C���f�b�N�X
} BMSHEADER,*LPBMSHEADER;
*/




////////////////////////////////////////////////////////////////////////////////////
// BMS�N���X
////////////////////////////////////////////////////////////////////////////////////
class CBmsPro {
protected:
	BMSHeader*			bmsHeader;						// �w�b�_�f�[�^

	LPBMSDATA			mBmsData[BMSMAXBUFFER];			// ���f�[�^
	int					iBmsData[BMSMAXBUFFER];			// ���f�[�^�̂��ꂼ��̐�

	std::unordered_map<int, std::string> wavData;		// WAV�̃t�@�C����
	std::unordered_map<int, std::string> bmpData;		// BMP�̃t�@�C����

	std::vector<LONGLONG> barData;	// ���߂̃f�[�^
	int					iBmsBar;						// ���߂̌�

	std::vector<std::tuple<long, float>> barMagni;		// ���߂̔{��
	int					iBarMagni;						// ���ߔ{���̌�

	char				mLastError[1024];				// �G���[�����������ꍇ�̕�����

private:
	int		atoi1610( char *s );													// 16�i��������𐔎��ɕϊ�
	Command	getCommandPart(const std::string line);										// �P�s���̃f�[�^���̃R�}���h�ԍ���Ԃ�	
	std::string getCommandString(std::string line);									// �R�}���h�̕����񕔕���Ԃ��i'\n'�͍폜 ':'����؂�Ƃ��ď����j
	BOOL	LoadBmsData( LPSTR file );												// BMS�f�[�^�̓ǂݍ���
	GUID	GetGuidFromString( const char* mGuid );										// GUID���������͂���
	BOOL	NumToStr36( int num,char *dst,int keta=-1 );							// 10�i���l�����t����36�i�������֕ϊ�
	BOOL	LineCompact( char *src,char *dst );										// �f�[�^���R���p�N�g�ɂ���

public:
	CBmsPro();																		// �R���X�g���N�^
	virtual ~CBmsPro();																// �f�X�g���N�^

	// ��{���\�b�h
	BOOL	Clear( void );															// �f�[�^�̏�����
	BOOL	GetHeader( LPSTR file );												// BMS�w�b�_��񂾂������o��
	BOOL	Load( LPSTR file );														// BMS�t�@�C���̃��[�h
	BOOL	AddNoteData( CHANNEL ch,LONGLONG startpos,LONG dat );							// �P�̃f�[�^��ǉ��i�\�[�g�͂���Ȃ��j
	BOOL	Sort( int ch );															// �w��`�����l�����\�[�g����
	BOOL	Save( LPSTR file,BOOL same=TRUE );										// �f�[�^���Z�[�u����
	BOOL	Restart( void );														// ���X�^�[�g�p�ɃI�u�W�F�̃t���O�����ɖ߂�
	LONGLONG GetCountFromTime( double sec );										// ���Ԃ�������J�E���g�l���v�Z

public:
	inline char	*GetLastError( void ) { return mLastError; }

	/*
	// �Q�[�����K�{���\�b�h
	inline LONGLONG		GetMaxCount( void ) { return mBH.llMaxCount; }				// �Q�[�����̍ő�̃J�E���g�l
	inline LONG			GetObjeNum( int ch ) { return iBmsData[ch]; }				// �w��`���l���̃f�[�^����Ԃ�
	inline LPBMSDATA	GetObje( int ch,int num ) { return &mBmsData[ch][num]; }	// �`���l���Ɣz��ԍ��Ńf�[�^���擾����
	inline LPBMSHEADER	GetHeaderInfo( void ) { return &mBH; }						// �w�b�_���\���̂�Ԃ�

	// �w�b�_�����ʂɎ擾
	inline LPSTR		GetArtist( void ) { return mBH.mArtist; }					// �A�[�e�B�X�g��
	inline LONG			GetBarNum( void ) { return iBmsBar; }						// ���߃o�[�̌�
	inline LPBMSBAR		GetBar( int num ) { return &mBmsBar[num]; }					// ���߃o�[�̃f�[�^
	inline LPSTR		GetBmpFile( int num ) { return mBmp[num]; }					// �g�p���Ă���BMP�t�@�C����
	inline float		GetBpm( void ) { return mBH.fBpm; }							// �������̃e���|��
	inline LONG			GetGageIncrease( void ) { return mBH.lTotal; }				// �Q�[�W�̑���
	inline LPSTR		GetGenre( void ) { return mBH.mGenre; }						// �W��������
	inline GUID*		GetGuid( void ) { return &mBH.guid; }						// GUID�̎擾
	inline LONG			GetRank( void ) { return mBH.lRank; }						// �����N
	inline LPSTR		GetMidiFile( void ) { return mBH.mMidifile; }				// MIDI�t�@�C����
	inline LPSTR		GetMovieFile( void ) { return mBH.mMovie; }					// MOVIE�t�@�C����
	inline LONG			GetPlayer( void ) { return mBH.lPlayer; }					// �v���C���[�i���o�[
	inline LONG			GetPlayLevel( void ) { return mBH.lPlaylevel; }				// �v���C���x��
	inline LPSTR		GetStagePic( void ) { return mBH.mStagePic; }				// �X�e�[�W�摜��
	inline LPSTR		GetTitle( void ) { return mBH.mTitle; }						// �^�C�g����
	inline LONG			GetWavVol( void ) { return mBH.lWavVol; }					// ���ʂ����̉����ɂ��邩
	inline LPSTR		GetWavFile( int num ) { return mWav[num]; }					// �g�p���Ă���WAV�t�@�C����
	inline LONG			GetBarMagniNum( void ) { return iBarMagni; }				// ���ߔ{���ύX�f�[�^�̐�
	*/
};

#endif
