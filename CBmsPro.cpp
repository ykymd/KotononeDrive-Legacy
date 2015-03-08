///////////////////////////////////////////////////////////////////////////////////
//                    BMS�N���X �`���[�g���A���o�[�W���� v3.0                    //
//                          http://www.charatsoft.com/                           //
//                             THIS CODE IS FREE.                                //
///////////////////////////////////////////////////////////////////////////////////
#pragma warning( disable : 4996 )

#include "CBmsPro.h"
#include <stdio.h>

//#define DEBUGMODE
//#include "debug.h"

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

#define FREE(x)		{ if(x) {free(x); x=NULL;} }
#define BINCLOSE(x)	{ if(x) {fclose(x); x=NULL;} pBinSrc=NULL; }

using namespace std;

////////////////////////////////////////////
// �R���X�g���N�^
////////////////////////////////////////////
CBmsPro::CBmsPro( void )
{
	//ZeroMemory( &mBH,sizeof(mBH) );
	ZeroMemory( &mBmsData,sizeof(mBmsData) );
	ZeroMemory( &iBmsData,sizeof(iBmsData) );
	//ZeroMemory( &mWav,sizeof(mWav) );
	//ZeroMemory( &mBmp,sizeof(mBmp) );

	ZeroMemory( &mLastError,sizeof(mLastError) );

	// �f�t�H���g�Z�b�g
	/*
	mBH.lPlayer = 1;
	mBH.fBpm = 130;

	iBarMagni = 0;
	mBarMagni = NULL;
	iBmsBar = 0;
	mBmsBar = NULL;
	*/
}

////////////////////////////////////////////
// �f�X�g���N�^
////////////////////////////////////////////
CBmsPro::~CBmsPro()
{
	Clear();
}

////////////////////////////////////////////
// 16�i��������𐔎��ɕϊ�(0�`F)
////////////////////////////////////////////
int CBmsPro::atoi1610(char *s)
{
	int i = 0;	// �|�C���^
	int x = 0;	// 10�i���ɕϊ�������
	while(1)
	{
		if(s[i] == NULL)
			break;
		if( !(s[i]>='0' && s[i]<='9') &&
			!(s[i]>='A' && s[i]<='Z') &&
			!(s[i]>='a' && s[i]<='z') )
			return 0;

		x *= 16;	// 16�{
		int n = s[i] - '0';
		if(n>9)
			n -= 7;
		if(n>15)
			n -= 0x20;
		x += n;
		i++;
	}
	return x;
}

////////////////////////////////////////////
// �f�[�^�̃N���A
////////////////////////////////////////////
BOOL CBmsPro::Clear( void )
{
	int i;
	for( i=0;i<BMSMAXBUFFER;i++ ) {
		//FREE( mWav[i] );
		//FREE( mBmp[i] );
		//mBH.mBpmIndex[i].fBpm = 120.0f;
	}
	for( i=0;i<BMSMAXBUFFER;i++ ) {
		//FREE( mBmsData[i] );		// BMS�f�[�^�̈���N���A
		//iBmsData[i] = 0;			// �f�[�^�̐����N���A
	}

	//FREE( mBarMagni );
	iBarMagni = 0;

	//FREE( mBmsBar );
	iBmsBar = 0;

	//ZeroMemory( &mBH,sizeof(BMSHEADER) );
	// �f�t�H���g�Z�b�g
	//mBH.lPlayer = 1;
	//mBH.fBpm = 130;

	return TRUE;
}

////////////////////////////////////////////
// �f�[�^���[�h
////////////////////////////////////////////
BOOL CBmsPro::Load( LPSTR file )
{
	// �w�b�_�����ߔ{���̓ǂݍ���
	if( !GetHeader( file ) ) {
		cout << "�w�b�_�ǂݎ��G���[" << endl;
		return FALSE;
	}
	cout << "HEADER OK" << endl;

	// ���f�[�^�̓ǂݍ���
	if( !LoadBmsData( file ) ) {
		cout << "�f�[�^�ǂݍ��݃G���[" << endl;
		return FALSE;
	}
	cout << "DATA OK" << endl;

	return TRUE;
}

////////////////////////////////////////////
// �P�s���̃f�[�^���̃R�}���h�ԍ���Ԃ�	
////////////////////////////////////////////
Command CBmsPro::getCommandPart(const string line)
{
	const string command[] = {
		"PLAYER",
		"GENRE",
		"TITLE",
		"ARTIST",
		"BPM",
		"MIDIFILE",
		"PLAYLEVEL",
		"RANK",
		"VOLWAV",
		"TOTAL",
		"STAGEFILE",
		"WAV",
		"BMP",
		"GUID",
		"MOVIE",
	};

	// �����񒆂̃R�}���h�������A�w��̃R�}���h�Q�ƈ�v���Ă��邩
	auto cnt = 0;
	for (auto pattern : command){
		// �����ł��Ȃ�I�[�o�[�w�b�h���N���邩������Ȃ�
		auto cmdpart = line.substr(1, pattern.size());
		if (cmdpart == pattern){
			return Command(cnt);
		}

		cnt++;
	}

	// ���ߕ������I�u�W�F�z�u�̌`('#iiiii')��
	// �����񒆂̃R�}���h�����𒊏o
	auto cmd = line.substr(1, 5);

	// �R�}���h���������ׂĐ��l�ł���Ȃ�I�u�W�F�z�u�Ƃ��ĕԂ�
	if (all_of(cmd.cbegin(), cmd.cend(), isdigit)){
		return Command::OBJECT;
	}
	else {
		// ����ȊO�̏ꍇ�A���ʕs�\�����Ƃ��ĕԂ�
		return Command::UNKNOWN;
	}
}

// �R�}���h�̕����񕔕���Ԃ��i'\n'�͍폜 ':'����؂�Ƃ��ď����j
string	CBmsPro::getCommandString(std::string line)
{
	auto i = 1;

	// �X�y�[�X��:�̕���������܂œǂݎ��A����ȍ~�̕������Ԃ�
	for (auto let : line){
		if (let == ' ' || let == '�@' || let == ':'){
			return line.substr(i);
		}
		else if (let == NULL){
			break;
		}

		i++;
	}

	return NULL;
}


//////////////////////////////////////////////////////////////////
// GUID���������͂���(�ėp�֐�)
// �`��="{5BCECBC6-6C0F-452c-AF8D-C6CE6BC0BDA0}"
//////////////////////////////////////////////////////////////////
GUID CBmsPro::GetGuidFromString( const char* mGuid )
{
	static GUID guid={0};
	
	// �܂��n�߂�'{'��'}'���`�F�b�N
	if( mGuid[0]!='{' || mGuid[37]!='}' ) {
		ZeroMemory( &guid,sizeof(GUID) );
		return guid;
	}
	int i,j;
	BYTE c;
	// Data1 
	for( i=0;i<8;i++ ) {
		c = mGuid[i+1]-'0';
		if( c>9 )
			c-=7;
		if( c>15 )
			c-=0x20;
		guid.Data1 <<= 4;
		guid.Data1 |= c;
	}
	// Data2
	for( i=0;i<4;i++ ) {
		c = mGuid[i+10]-'0';
		if( c>9 )
			c-=7;
		if( c>15 )
			c-=0x20;
		guid.Data2 <<= 4;
		guid.Data2 |= c;
	}
	// Data3
	for( i=0;i<4;i++ ) {
		c = mGuid[i+15]-'0';
		if( c>9 )
			c-=7;
		if( c>15 )
			c-=0x20;
		guid.Data3 <<= 4;
		guid.Data3 |= c;
	}
	// BYTE 2
	for( j=0;j<2;j++ ) {
		for( i=0;i<2;i++ ) {
			c = mGuid[i+20+j*2]-'0';
			if( c>9 )
				c-=7;
			if( c>15 )
				c-=0x20;
			guid.Data4[j] <<= 4;
			guid.Data4[j] |= c;
		}
	}
	// BYTE 6
	for( j=0;j<6;j++ ) {
		for( i=0;i<2;i++ ) {
			c = mGuid[i+25+j*2]-'0';
			if( c>9 )
				c-=7;
			if( c>15 )
				c-=0x20;
			guid.Data4[j+2] <<= 4;
			guid.Data4[j+2] |= c;
		}
	}
	return guid;
}

BOOL CBmsPro::NumToStr36( int num,char *dst,int keta )
{
	if( num<0 )
		return FALSE;

	itoa( num,dst,36 );

	// �������`�F�b�N
	int i;
	for( i=0;i<(int)strlen(dst);i++ ) {
		if( dst[i]>='a' && dst[i]<='z' ) {
			dst[i] -= 0x20;
		}
	}


	if( keta>=0 ) {
		// �����w��̏ꍇ
//		DEBUG( "���w��\n","" );
		if( (int)strlen(dst)<keta ) {
			// ����������Ȃ��ꍇ
//			DEBUG( "����������Ȃ��ꍇ %d\n",keta-strlen(dst) );
			char dmy[1024];
			ZeroMemory( dmy,sizeof(dmy) );
			for( i=0;i<keta-(int)strlen(dst);i++ ) {
				dmy[i] = '0';			// �擪�ɑ���Ȃ�����0��ǉ�
			}
			strcpy( &dmy[i],dst );		// ���̎��ɎZ�o���ꂽ��������Ȃ���
			strcpy( dst,dmy );			// �o�͌��ʂɃ_�~�[��������R�s�[		
		}
	}

	return TRUE;
}

BOOL CBmsPro::LineCompact( char *src,char *dst )
{
	int i;
	char buf[1024];		// �o�b�t�@
	strcpy( buf,src );

	while(1) {
		// �f�[�^�̐������[�v
		int l = strlen( buf ) / 2;	// �f�[�^���ɕϊ�
		if( l==0 ) {
//			DEBUG( "�f�[�^�Ȃ�\n","" );
			return FALSE;
		}


		if( (l%2)==1 ) {
			//DEBUG( "� [%d]\n",l );
			BOOL ok = FALSE;
			// �f�[�^���P�ł����݂��邩
			for( i=0;i<l;i++ ) {
				if( buf[i*2]!='0' || buf[i*2+1]!='0' ) {
					ok = TRUE;
					break;
				}
			}
			if( !ok )
				return FALSE;

			break;					// ��Ȃ�ϊ��Ȃ�
		}

		// �P�����Ńf�[�^�����邩�`�F�b�N
		BOOL ok = FALSE;
		for( i=0;i<l;i+=2 ) {
			if( buf[i*2+2]!='0' || buf[i*2+3]!='0' ) {
				ok = TRUE;
				break;
			}
		}

		// �����I���Ȃ�
		if( ok )
			break;

		// �o�b�t�@���Q��������
		char tmp[1024];
		strcpy( tmp,buf );
		ZeroMemory( buf,sizeof(buf) );
		for( i=0;i<l/2;i++ ) {
			buf[i*2  ] = tmp[i*4  ];
			buf[i*2+1] = tmp[i*4+1];
		}
		//DEBUG( "CONVERT [%s]\n",buf );
	}

	strcpy( dst,buf );
	//DEBUG( "dst=[%s]\n",dst );

	return TRUE;
}

////////////////////////////////////////////
// �w�b�_��񂾂������o��
////////////////////////////////////////////
BOOL CBmsPro::GetHeader( LPSTR file )
{
	Clear();

	auto bh = new BMSHeader();

	// �ǂݍ��񂾃t�@�C����1�s���̃f�[�^���i�[���镶����
	string str;

#ifndef __DXLIB
	// �w�肳�ꂽ�A�h���X����t�@�C����ǂݍ���
	ifstream ifs(file);

	// ���s���̏���
	if (ifs.fail())
	{
		cerr << "���s" << endl;
		//return EXIT_FAILURE;
		return FALSE;
	}
#endif

	// �w��̃t�@�C������1�s������ǂ݂���
#ifdef __DXLIB
	char tmp[BMSMAXBUFFER];
	int FileHandle = DxLib::FileRead_open(file);

	// DX���C�u�����𗘗p���Ă���ۂ́ADX�A�[�J�C�o�ɑΉ������֐��ɐ؂�ւ���
	while (DxLib::FileRead_eof(FileHandle) == 0){
		DxLib::FileRead_gets(tmp, BMSMAXBUFFER, FileHandle);
		str = std::string(tmp);
#else
	while (getline(ifs, str)) {
#endif
		// �R�}���h�ȊO�̕����͓ǂݔ�΂�
		if (str[0] != '#') continue;

		cout << "�o��:" << str << endl;

		// ���s����������
		if (str.back() == '\n')
			str.back() = '\0';

		// 1�s���̃f�[�^����R�}���h�ɑ������镔�����擾
		auto command = getCommandPart(str);

		// ���ʕs�\�����Ȃ�x��
		if (command == Command::UNKNOWN)  {
			std::cout << "���ʕs�\����:" << str << std::endl;
			continue;
		}

		// 1�s���̃f�[�^����f�[�^�������擾
		auto dataString = getCommandString(str);
		int num = 0;

		// �R�}���h�̎�ނɉ������p�����[�^�ɕ��������
		switch (command)
		{
		case Command::PLAYER:
			bh->player = stoi(dataString);
			cout << "PLAYER       " << bh->player << endl;
			break;
		case Command::GENRE:
			bh->genre = dataString;
			cout << "�W������     " << bh->genre << endl;
			break;
		case Command::TITLE:
			bh->title = dataString;
			cout << "�^�C�g��     " << bh->title << endl;
			break;
		case Command::ARTIST:
			bh->artist = dataString;
			cout << "�A�[�e�B�X�g " << bh->artist << endl;
			break;
		case Command::BPM:
			if (str[4] == ' ' || str[4] == '�@') {
				// �w�b�_�Ȃ�
				bh->bpm = stof(dataString);
				AddData(CHANNEL::TEMPO, 0, (LONG)bh->bpm);
				cout << "�x�[�XBPM    " << bh->bpm << endl;
			}
			else {
				// �C���f�b�N�X�Ȃ�
				auto index = stoi(str.substr(4, 2));
				bh->bpmIndex[index] = stof(dataString);
				cout << "�C���f�b�N�XBPM " << bh->bpmIndex.at(index) << endl;
			}
			break;
		case Command::MIDIFILE:
			bh->midiFile = dataString;
			cout << "MIDIFILE      " << bh->midiFile << endl;
			break;
		case Command::PLAYLEVEL:
			bh->playLevel = stoi(dataString);
			cout << "PLAYLEVEL     " << bh->playLevel << endl;
			break;
		case Command::RANK:
			bh->rank = stoi(dataString);
			cout << "RANK          " << bh->rank << endl;
			break;
		case Command::VOLWAV:
			bh->wavVol = stoi(dataString);
			cout << "WAVVOL        " << bh->wavVol << endl;
			break;
		case Command::TOTAL:
			bh->total = stoi(dataString);
			cout << "TOTAL         " << bh->total << endl;
			break;
		case Command::STAGEFILE:
			cout << "StageImage-" << bh->stagePic << endl;
			bh->stagePic = dataString;
			break;
		case Command::WAV:
			num = stoi(str.substr(4, 2), nullptr, 16);
			wavData[num] = dataString;

			cout << "WAV%04d-" << num << endl;
			cout << wavData.at(num) << endl;
			break;
		case Command::BMP:
			num = stoi(str.substr(4, 2), nullptr, 16);
			bmpData[num] = dataString;

			cout << "BMP%04d-" << num << endl;
			cout << bmpData.at(num) << endl;
			break;
		case Command::SONG_GUID:
			bh->guid = GetGuidFromString(dataString.c_str());
			cout << "BMS : GUID=" << bh->guid.Data1 << endl;
			break;
		case Command::MOVIE:
			bh->movie = dataString;
			cout << "MOVIE=" << bh->movie << endl;
			break;
		default:
			// ���ߔԍ��̎擾
			auto iLines = stoi(str.substr(1, 3));
			// �`�����l���ԍ��̎擾
			auto iChannel = stoi(str.substr(4, 2));	// ������
			auto channel = static_cast<CHANNEL>(iChannel);

			if (channel == CHANNEL::STRETCH) {
				// ���߂̔{���ύX���߂̏ꍇ
				//				DEBUG("���ߔ{���ϊ� [%s]�{\n",str);
				iBarMagni++;

				auto magni = make_tuple(iLines, stof(dataString));
				barMagni.push_back(magni);
			}

			// ���ߔԍ��̍ő�l���L������
			if (bh->endBar < iLines)
				bh->endBar = iLines;

			break;
		}
	}

#ifdef __DXLIB
	FileRead_close(FileHandle);
#endif


	// ���ߕ����{���f�[�^�����ɏ��߃o�[�𐶐�
	LONGLONG lastCount = 0;
	for(auto i=0; i <= bh->endBar+1; i++) {				// �Ō�̏��߂܂ł�����
		// ���߂�ǉ�
		iBmsBar++;
		
		barData.push_back(lastCount);
		auto add = BMSDATA_RESOLUTION;			// ���Z����|�C���^�����`

		// ���݂̏��߂Ŕ{����ϊ����Ă��邩
		for (auto magni : barMagni) {
			if( get<0>(magni) == i ) {
				// �{���ύX���߂����݂���ꍇ
				add = (float)BMSDATA_RESOLUTION * get<1>(magni);	// ���Z�l��n�{����
				break;	// ���̏��߂̔{���ϊ��͏I���
			}
		}
		lastCount += add;	// �|�W�V������ǉ�

		// �ő�̔��������łɊ��Z
		if (i < bh->endBar && bh->maxCount < lastCount)
			bh->maxCount = lastCount;
	}

	cout << "NumBmsBar:" << barData.size() << endl;

	this->bmsHeader = bh;

	return TRUE;
}

////////////////////////////////////////////
// �P�̃f�[�^��ǉ�
////////////////////////////////////////////
BOOL CBmsPro::AddData( CHANNEL ch,LONGLONG startpos,LONG dat )
{
	/*
	// �`�����l���ԍ����`�F�b�N
	//if( ch<0 || ch>255 )
	//	return FALSE;

	if( ch==CHANNEL::STRETCH )
		return FALSE;

	if( dat==0 )
		return TRUE;

	switch( ch )
	{
	case CHANNEL::BPMINDEX:
		auto tempo = static_cast<int>(CHANNEL::TEMPO);
		iBmsData[tempo]++;
		mBmsData[tempo] = (LPBMSDATA)realloc( mBmsData[tempo],iBmsData[tempo]*sizeof(BMSDATA) );
		ZeroMemory( &mBmsData[tempo][ iBmsData[tempo]-1 ],sizeof(BMSDATA) );	// ���ꂢ��
		mBmsData[tempo][iBmsData[tempo]-1].bFlag = TRUE;
		mBmsData[tempo][iBmsData[tempo]-1].llTime = startpos;
		mBmsData[tempo][iBmsData[tempo]-1].lData = (long)mBH.mBpmIndex[dat].fBpm;
		mBmsData[tempo][iBmsData[tempo]-1].fData = mBH.mBpmIndex[dat].fBpm;
		break;
	default:
		// �f�[�^��ǉ�
		auto channel = static_cast<int>(ch);
		iBmsData[channel]++;
		mBmsData[channel] = (LPBMSDATA)realloc( mBmsData[channel],iBmsData[channel]*sizeof(BMSDATA) );
		ZeroMemory( &mBmsData[channel][ iBmsData[channel]-1 ],sizeof(BMSDATA) );	// ���ꂢ��
		mBmsData[channel][iBmsData[channel]-1].bFlag = TRUE;
		mBmsData[channel][iBmsData[channel]-1].llTime = startpos;
		mBmsData[channel][iBmsData[channel]-1].lData = dat;
		mBmsData[channel][iBmsData[channel]-1].fData = (float)dat;		// float�^������̐��l��
		break;
	}
	*/

	return TRUE;
}

// �m�[�c�̃f�[�^���L���[�Ɋi�[����
bool CBmsPro::AddNoteData(unsigned int ch_num, long long startpos, long dat)
{
	// �`�����l���ԍ���0~255�ȓ����`�F�b�N
	if (ch_num & ~255) {
		std::cout << "�`�����l����:" << ch_num << "�̓T�|�[�g�O�ł�(0~255)" << std::endl;
		return false;
	}

	if (ch_num == BMS_STRETCH) return false;

	if (dat == 0) return true;

	switch (ch_num)
	{
	case BMS_BPMINDEX:
		numBmsData[BMS_TEMPO]++;
		bmsData[BMS_TEMPO] = (LPBMSDATA)realloc(bmsData[BMS_TEMPO], numBmsData[BMS_TEMPO] * sizeof(BMSDATA));
		ZeroMemory(&bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1], sizeof(BMSDATA));
		//bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1] = 0;
		bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1].flag = true;
		bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1].time = startpos;
		bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1].data = (long)bHeader.BPMIndex[dat].Bpm;
		bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1].fData = bHeader.BPMIndex[dat].Bpm;
		bmsData[BMS_TEMPO][numBmsData[BMS_TEMPO] - 1].judge = None;		//����͖���������f�t�H�Ƃ���
		break;
	default:
		// �f�[�^��ǉ�
		numBmsData[ch_num]++;
		bmsData[ch_num] = (LPBMSDATA)realloc(bmsData[ch_num], numBmsData[ch_num] * sizeof(BMSDATA));
		ZeroMemory(&bmsData[ch_num][numBmsData[ch_num] - 1], sizeof(BMSDATA));
		bmsData[ch_num][numBmsData[ch_num] - 1].flag = true;
		bmsData[ch_num][numBmsData[ch_num] - 1].time = startpos;
		bmsData[ch_num][numBmsData[ch_num] - 1].data = dat;
		bmsData[ch_num][numBmsData[ch_num] - 1].fData = (float)dat;
		break;
	}

	return true;
}

////////////////////////////////////////////
// BMS�f�[�^�̓ǂݍ���
////////////////////////////////////////////
BOOL CBmsPro::LoadBmsData( LPSTR file )
{
	// �ǂݍ��񂾃t�@�C����1�s���̃f�[�^���i�[���镶����
	string str;

#ifndef __DXLIB
	// �w�肳�ꂽ�A�h���X����t�@�C����ǂݍ���
	ifstream ifs(file);

	// ���s���̏���
	if (ifs.fail())
	{
		cerr << "���s" << endl;
		//return EXIT_FAILURE;
		return FALSE;
	}
#endif

	// �w��̃t�@�C������1�s������ǂ݂���
#ifdef __DXLIB
	char tmp[BMSMAXBUFFER];
	int FileHandle = DxLib::FileRead_open(file);

	// DX���C�u�����𗘗p���Ă���ۂ́ADX�A�[�J�C�o�ɑΉ������֐��ɐ؂�ւ���
	while (DxLib::FileRead_eof(FileHandle) == 0){
		DxLib::FileRead_gets(tmp, BMSMAXBUFFER, FileHandle);
		str = std::string(tmp);
#else
	while (getline(ifs, str)) {
#endif
		// �R�}���h�ȊO�͓ǂݔ�΂�
		if (str[0] != '#') continue;

		// ���s����������
		if (str[str.size() - 1] == '\n')
			str[str.size() - 1] = NULL;

		// ���ߗ񂩂�R�}���h�������擾
		Command cmd = getCommandPart(str);
		if (cmd != Command::OBJECT)  continue;

		// 
		auto dataString = getCommandString(str);

		// �f�[�^�ł���ΈӖ���͂��s��
		int barLines;		// ���݂̏��ߔԍ�
		//char num[4] = { 0 };	//�ėp�o�b�t�@

		// �`�����l���ԍ��̎擾
		auto numChannel = std::stoi(str.substr(4, 2));
		// �񋓒l�ɕϊ�
		auto channel = static_cast<CHANNEL>(numChannel);

		// ���߂̔{���ύX���߂̏ꍇ�̓L�����Z��
		if (channel == CHANNEL::STRETCH) continue;

		// ���ߔԍ��̎擾
		barLines = std::stoi(str.substr(1, 3));

		// �f�[�^�����݂��邩
		int size = dataString.size();
		if (size < 1) {
			std::cout << "�f�[�^��������`����Ă��܂���:" << str << std::endl;
			continue;
		}

		// �f�[�^�����̕�������������
		if (size % 2 == 1) {
			std::cout << "�f�[�^�����̕������������ł͂���܂���:" << str << std::endl;
			return false;
		}

		// �f�[�^��ǉ�����
		size = size / 2;

		// ���ߋ敪����f�[�^�z�u�ꏊ�����
		long long startCount = 0;	// ���̏��߂̍ŏ��̃J�E���g�l
		int chkLine = 0;					// �v�Z���̏���

		// ���݂̏��߂�1�O�܂ł�����
		while (chkLine < barLines) {
			// ���Z����|�C���^�����`
			long long add = BMSDATA_RESOLUTION;

			// ���݃`�F�b�N���̏��߂ɔ{���ύX���Ȃ�����������
			// �T�|�[�g�O

			//  ���݂܂ł̏��߃J�E���g�����Z
			startCount += add;

			// ���̏��߂�
			chkLine++;
		}

		// ���݂̏��߂̕������߂�
		float currentBarResolution = (float)BMSDATA_RESOLUTION / (float)size;

		// ���̏��߂ɔ{���ύX���߂����邩

		// ���f�[�^��ǉ�
		for (int i = 0; i < size; i++) {
			// 16�i���\�L�̕������10�i���̐��l�ɒ����B
			long hex = std::stoi(dataString.substr(i * 2, 2), nullptr, 16);
			if (hex > 0) {
				AddNoteData(channel, startCount + (long long)(currentBarResolution*(float)i), hex);
			}
		}
	}
#ifdef __DXLIB
	FileRead_close(FileHandle);
#endif




	int i;
	char data[1024];

	//FILE *fp;
	auto fp = fopen( file,"r" );
	if( !fp ) {
		sprintf( mLastError,"[%s] �t�@�C���I�[�v���G���[",file );
		return FALSE;
	}

	char buf[1024];
	while(1) {
		// �P�s��ǂ݂���
		ZeroMemory( buf,1024 );
		fgets( buf,1024,fp );
		if( buf[0]==NULL && feof(fp) )	// �t�@�C���̏I�[�Ȃ猟���I���
			break;

//		DEBUG( "%s",buf );

		// �R�}���h�ȊO�Ȃ玟�̍s��
		if( buf[0]!='#' )
			continue;

		// �Ō�̉��s������
		if( buf[strlen(buf)-1]=='\n' )
			buf[strlen(buf)-1] = NULL;

		// �R�}���h�̎�ނ��擾
		int com = GetCommand( buf );

		// �I�u�W�F�ȊO�͑S�Ė���
		if( com!=-1 )
			continue;

		// �p�����[�^��������擾
		ZeroMemory(data,1024);
		if( !GetCommandString(buf,data) ) {
			DEBUG( "�p�����[�^������擾�G���[\n","" );
			fclose(fp);
			sprintf( mLastError,"[%s] �p�����[�^������擾�G���[",buf );
			return FALSE;
		}
		
		// �f�[�^�ł���Ή��
		int iLines;					// ���݂̏��ߔԍ�
		int iChannel;				// ���݂̃`�����l���ԍ�
		char mNum[4];				// �ėp�o�b�t�@

		// �`�����l���ԍ��̎擾
		ZeroMemory( &mNum,sizeof(mNum) );
		mNum[0] = buf[4];			// �`�����l���ԍ�
		mNum[1] = buf[5];
		iChannel = atoi( mNum );	// ������

		auto channel = static_cast<CHANNEL>(iChannel);
		// ���߂̔{���ύX���߂̏ꍇ�̓L�����Z��
		if( channel==CHANNEL::STRETCH )	
			continue;

		// ���ߔԍ����擾
		ZeroMemory( &mNum,sizeof(mNum) );
		memcpy( mNum,buf+1,3 );		// ���ߕ������擾
		iLines = atoi( mNum );		// ������

		// �f�[�^�����݂��邩�`�F�b�N
		if( strlen(data)<1 ) {
			DEBUG( "�f�[�^����`����Ă��Ȃ��ӏ�������\n%s",buf );
			continue;
		}

		// �f�[�^���������`�F�b�N
		if( strlen(data)%2==1 ) {
			DEBUG( "�f�[�^�������ł͂Ȃ� [%s]\n",buf );
			fclose( fp );
			sprintf( mLastError,"[%s] �f�[�^�������ł͂Ȃ�",buf );
			return FALSE;
		}

		// �f�[�^��ǉ�����
		int len = strlen(data) / 2;			// ���l�̐�
	
		// ���ߋ敪����f�[�^�z�u�ꏊ�����
		LONGLONG llStartCount = 0;					// ���̏��߂̍ŏ��̃J�E���g�l
		int chkline = 0;							// �v�Z���̏��߁i0���ߖڂ��猻�݂�1�O�܂ł����Z���Ă����j

		while( chkline<iLines ) {					// ���݂̏��߂̂P�O�܂ł�����

			LONGLONG add = BMSDATA_RESOLUTION;		// ���Z����|�C���^�����`

			// ���݃`�F�b�N���̏��߂ɔ{���ύX�����݂��Ȃ�����������
			for( i=0;i<iBarMagni;i++ ) {
				if( mBarMagni[i].iLines==chkline ) {									// �{���ύX���߂����݂���ꍇ
					add = (LONGLONG)((float)BMSDATA_RESOLUTION * mBarMagni[i].fMagni);	// ���Z�l��f�{����(�܂�0.5�Ȃ甼���ɂȂ�)
					break;																// ���������̂ŏI���
				}
			}

			// ���݂܂ł̏��߃J�E���g�����Z
			llStartCount += add;

			// ���̏��߂�
			chkline++;		
		}

		// ���݂̏��߂̕������߂�
		float fNowBarReso = (float)BMSDATA_RESOLUTION / (float)len;	// �f�t�H���g�̂P�������̃J�E���g�l

		for( i=0;i<iBarMagni;i++ ) {
			// ���̏��߂ɔ{���ύX���߂����݂��邩
			if( mBarMagni[i].iLines==chkline ) {
				// ���݂����ꍇ�͕��������Čv�Z
				fNowBarReso = ((float)BMSDATA_RESOLUTION * mBarMagni[i].fMagni) / len;
				break;					// ���������̂ŏI���
			}
		}

		// ���f�[�^��ǉ�
		ZeroMemory( &mNum,sizeof(mNum) );
		for( i=0;i<len;i++ ) {
			mNum[0] = data[i*2];
			mNum[1] = data[i*2+1];
			long hex = atoi1610( mNum );			// �P�U�i���𐔒l�֕ϊ�
			if( hex>0 ) {
				// �f�[�^�����݂���ꍇ
				AddData( channel, llStartCount + (LONGLONG)(fNowBarReso*(float)i),hex );
			}
		}
	}


//	DEBUG( "�f�[�^���[�hOK1\n","") ;
	// �\�[�g
	for( i=0;i<BMSMAXBUFFER;i++ )
		Sort( i );
//	DEBUG( "�f�[�^���[�hOK2\n","") ;

	fclose( fp );

/*	for( i=0;i<iBmsData[BMS_TEMPO];i++ ) {
		char s[256];
		sprintf( s,"[%08d] %f",(int)mBmsData[BMS_TEMPO][i].llTime,mBmsData[BMS_TEMPO][i].fData );
		DEBUG( "%s\n",s )
	}/**/

	return TRUE;
}

////////////////////////////////////////////
// �w��`�����l���̃f�[�^�𑁂����Ƀ\�[�g����
////////////////////////////////////////////
BOOL CBmsPro::Sort( int ch )
{
	if( ch<0 || ch>BMSMAXBUFFER-1 )
		return FALSE;

//	DEBUG( "SORT [%d]-",ch );
//	DEBUG( "[%d]\n",iBmsData[ch] );

	int i,j;
	// �����ɕ��ёւ���
	for( i=0;i<iBmsData[ch]-1;i++ ) {
		for( j=i+1;j<iBmsData[ch];j++ ) {
			if( mBmsData[ch][i].llTime > mBmsData[ch][j].llTime ) {
				BMSDATA dmy;
				memcpy( &dmy,&mBmsData[ch][i],sizeof(BMSDATA) );					// �_�~�[�ɕۑ�
				memcpy( &mBmsData[ch][i],&mBmsData[ch][j],sizeof(BMSDATA) );		// i��j������
				memcpy( &mBmsData[ch][j],&dmy,sizeof(BMSDATA) );					// j�Ƀ_�~�[������
			}
		}
	}
	return TRUE;
}


////////////////////////////////////////////
// �w��`�����l���̃f�[�^�𑁂����Ƀ\�[�g����
////////////////////////////////////////////
BOOL CBmsPro::Save( LPSTR file,BOOL same )
{
	int i,j;
	FILE *fp;
	fp = fopen( file,"wb" );
	if( !fp ) {
		sprintf( mLastError,"[%s] �t�@�C���������݃G���[",file );
		return FALSE;
	}


	// BGM�`�����l���ȊO�̓���ꏊ�ɑ��݂���f�[�^�������I�ɂ��炷
	if( same ) {
		//DEBUG( "����`�����l���ύX����\n","" );
		const int start[2] = { 11,21 };
		const int end[2] = { 19,29 };
		const int not[2] = { 17,27 };
		int lp;
		for( lp=0;lp<2;lp++ ) {
			for( i=start[lp];i<=end[lp];i++ ) {
				//DEBUG( "\n�`�F�b�N [%02d]\n",i );
				LONGLONG old = 0;
				j = 0;
				while( j<iBmsData[i] ) {
					LONGLONG tm = mBmsData[i][j].llTime;
					//DEBUG( "check %d",tm );
					if( old==tm ) {
						// �P�O�Ɠ���̎��Ԃ������ꍇ�͂����Ă���`�����l��������
						//DEBUG( " > �P�O�Ɠ��ꎞ��","" );
						BOOL ok = FALSE;
						int k = i + 1;		// ���̃`�����l�����猟��
						if( k==not[lp] )
							k++;
						if( k>end[lp] )
							k = start[lp];

						while(1) {
							// �`�����l��������
							int l = 0;
							while( l<iBmsData[k] ) {
								if( mBmsData[k][l].llTime==tm ) {
									//DEBUG( " > %02dNG",k );
									break;
								}
								l++;
							}

							// �󂫃`�����l���Ȃ�
							if( l>=iBmsData[k] ) {
								//DEBUG( " > %02d OK",k );
								ok = TRUE;
								break;
							}

							// ���̃`�����l���̃`�F�b�N
							k++;
							if( k==not[lp] )
								k++;
							if( k>end[lp] )
								k = start[lp];
							if( i==k ) {
								//DEBUG( " > ALL ERROR(16������UP)","" );
								mBmsData[i][j].llTime += BMSDATA_RESOLUTION / 16;		// 16���������炷
								break;
							}
						}

						// �`�����l����ύX
						if( ok ) {
							AddData( static_cast<CHANNEL>(k),mBmsData[i][j].llTime,mBmsData[i][j].lData );
							memmove( &mBmsData[i][j],&mBmsData[i][j+1],sizeof(BMSDATA)*(iBmsData[i]-j-1) );
							iBmsData[i]--;
							// ���̃|�C���^�ɂ͍s���Ȃ�
							//DEBUG( "\n","" );
							continue;
						}

						//DEBUG( "\n","" );

					} else {
						old = tm;
					}
					//DEBUG( "\n","" );
					j++;
				}
			}

		}

		// �ŏI�\�[�g
		for( i=0;i<BMSMAXBUFFER;i++ )
			Sort( i );

		for( j=0;j<5;j++ ) {
			//DEBUG( "\n[%d]\n",j+11 );
			for( i=0;i<iBmsData[11+j];i++ ) {
				//DEBUG( "%d:",mBmsData[11+j][i].llTime );
				//DEBUG( "%d\n",mBmsData[11+j][i].lData );
			}
		}

	}

	/*
	// �w�b�_�̏����o��
	fprintf( fp,"\n" );
	fprintf( fp,"#PLAYER %d\n",mBH.lPlayer );
	fprintf( fp,"#GENRE %s\n",mBH.mGenre );
	fprintf( fp,"#TITLE %s\n",mBH.mTitle );
	fprintf( fp,"#ARTIST %s\n",mBH.mArtist );
	fprintf( fp,"#BPM %5.2f\n",mBH.fBpm );
	fprintf( fp,"#PLAYLEVEL %d\n",mBH.lPlaylevel );
	fprintf( fp,"#STAGEFILE %s\n",mBH.mStagePic );
	fprintf( fp,"#MOVIE %s\n",mBH.mMovie );
	fprintf( fp,"#MIDIFILE %s\n",mBH.mMidifile );
	fprintf( fp,"\n" );

	char buf[1024];

	// WAV�t�@�C���̏����o��
	for( i=0;i<BMSMAXBUFFER;i++ ) {
		if( mWav[i] ) {
			NumToStr36( i,buf,2 );
			fprintf( fp,"#WAV%s %s\n",buf,mWav[i] );
		}
	}
	fprintf( fp,"\n" );

	// BMP�t�@�C���̏����o��
	for( i=0;i<BMSMAXBUFFER;i++ ) {
		if( mBmp[i] ) {
			NumToStr36( i,buf,2 );
			fprintf( fp,"#BMP%s %s\n",buf,mBmp[i] );
		}
	}
	fprintf( fp,"\n" );

	// �f�[�^�̏����o��
	const int ch[] = {
		static_cast<int>(CHANNEL::BACKMUSIC),
//		BMS_TEMPO,
//		BMS_BACKANIME,
//		BMS_BPMINDEX,
		11,12,13,14,15,16,18,19,
		21,22,23,24,25,26,28,29,
		-1,
	};


	int n = 0;
	int index;
	int line;
	char s[1024];
	char dst[1024];

	while( ch[n]!=-1 ) {

		DEBUG( "ch [%02d]\n",ch[n] );

		index = 0;
		line = 0;

		// �S���߂̏���
		while( index<iBmsData[ch[n]] ) {
			DEBUG( "LINE %d\n",line );
			strcpy( s,"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );

			// ���݂̏��ߓ��̃f�[�^������
			while( index<iBmsData[ch[n]] ) {
				if( (line+1)*BMSDATA_RESOLUTION<=mBmsData[ch[n]][index].llTime )
					break;
//				DEBUG( "%d-",mBmsData[ch[n]][index].llTime );

				// ���Ԃ��珑�����ݐ�A�h���X���Z�o
				int p = (int)(mBmsData[ch[n]][index].llTime - line*BMSDATA_RESOLUTION);
				DEBUG( "%d:",p );
				p = (int)((float)p / (BMSDATA_RESOLUTION/64)) * 2;
				DEBUG( "%d\n",p );

				// ���ɂP�O�̏�񂪂��邩
				if( s[p]!='0' || s[p+1]!='0' ) {
					// ���ɂ���ꍇ�͌��݂܂ł̃f�[�^����������ł���
					if( LineCompact( s,dst ) )
						fprintf( fp,"#%03d%02d:%s\n",line,ch[n],dst );
					strcpy( s,"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
				}

				// �f�[�^���Z�b�g
				NumToStr36( mBmsData[ch[n]][index].lData,buf,2 );
//				DEBUG( "%s\n",buf );
				memcpy( &s[p],buf,2 );

				index++;
			}

			// �c��������o��
			if( strcmp(s,"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000")!=0 ) {
				if( LineCompact( s,dst ) ) {
					DEBUG( "�����o�� [%s]\n",dst );
					fprintf( fp,"#%03d%02d:%s\n",line,ch[n],dst );
				}
			}


			line++;
		}

		fprintf( fp,"\n" );

		n++;
	}

	fclose( fp );

	*/
	return TRUE;
}

BOOL CBmsPro::Restart( void )
{
	int i,j;
	for( j=0;j<BMSMAXBUFFER;j++ ) {
		for( i=0;i<iBmsData[j];i++ ) {
			mBmsData[j][i].bFlag = TRUE;
		}
	}
	return TRUE;
}

LONGLONG CBmsPro::GetCountFromTime( double sec )
{
	LONGLONG cnt = 0;			// BMS�J�E���g
	double t = 0;				// BMS��̎���
	double bpm = 130;
	
	auto channel = static_cast<int>(CHANNEL::TEMPO);
	if (iBmsData[channel] >0) {
		bpm = mBmsData[channel][0].fData;		// ����BPM
	}

	if( sec<0 )
		return 0;

//	DEBUG( "���w�莞�� (%.2f�b)\n",sec );

	// �w�莞�Ԃ��z����܂Ń^�C�������Z
	int i;
	auto tempo = static_cast<int>(CHANNEL::TEMPO);
	for( i=0;i<iBmsData[tempo];i++ ) {

		// �P�O�̎��ԂƐV�������ԂƂ̌o�ߎ��Ԃ���b���Z�o
		double add = (double)( mBmsData[tempo][i].llTime - cnt ) / ( bpm / 60 ) / (BMSDATA_RESOLUTION / 4);
//		DEBUG2( "  [%d] �o�ߎ��� %f�b\n",i,t+add );

		// ���݂̃e���|�l�Ŏ��Ԃ��߂����甲����
		if( t+add>sec ) {
//			DEBUG( "                  �����Ԃ��߂���\n","" );
			break;
		}

		t += add;										// �o�ߎ��Ԃ����Z
		bpm = (double)mBmsData[tempo][i].fData;		// ����BPM���Z�b�g
		cnt = mBmsData[tempo][i].llTime;			// �v�Z�ς݂̃J�E���g���Z�b�g
	}

//	DEBUG( "  BPM %f\n",bpm );
//	DEBUG( "  CNT %I64d\n",cnt );

	// �w�莞�Ԃ�1�O�܂ł̎��Ԃ̍���
	double sub = sec - t;
//	DEBUG( "  ���� %f�b\n",sub );

	// ��������BMS�J�E���g�����Z�o
	LONGLONG cnt2 = (LONGLONG)(sub * (BMSDATA_RESOLUTION / 4) * ( bpm / 60 ));

	// BMS�J�E���g�l�ɉ��Z
	cnt += cnt2;
//	DEBUG( "  ���ʂ̃J�E���g�l %I64d\n",cnt );

	return cnt;
}
