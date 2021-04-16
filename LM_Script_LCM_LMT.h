#pragma once
#include "LM_Base.h"

//*** LCM/LMT 스크립트 분석 및 적용 함수들을 선언하는 헤더


typedef struct _lcm_lmt_type_type_info_1
{
	unsigned int _int_list[6];
	unsigned char _char_list[2];
}LCM_LMT_TILE_INFO_1;
//*lcm/lmt 전용 인자 구조체 1(TILE)

typedef struct _lcm_lmt_type_0_info_2
{
	double _double_list[2];
	unsigned char _char_1;
	unsigned int _int_list[2];
	unsigned char _char_2;
}LCM_LMT_TILE_INFO_2;
//*lcm/lmt 전용 인자 구조체 2(TILE)

typedef struct _lcm_lmt_type_2_info
{
	unsigned int _int_list_1[4];
	unsigned char _char_list[2];
	unsigned int _int_list_2[12];
}LCM_LMT_BOX_INFO;
//*lcm/lmt 전용 인자 구조체 3(BOX)
//*여기에 _int_list_2[0]~[5]번째에 x, y 위치/너비,높이/출력시 너비,높이가 있는 듯 하다

typedef struct _lcm_lmt_type_4_info
{
	unsigned int _int_list[4];
	unsigned char _char_list[2];
}LCM_LMT_SOUND_INFO;
//*lcm/lmt 전용 인자 구조체 4(SOUND)

typedef struct _lcm_lmt_unk_65
{
	unsigned int _int_list[8];
	unsigned char _char_list[2];
}LCM_LMT_UNK_65;
//*lcm/lmt 전용 인자 구조체 3 (ver >= 0x65)

typedef struct _lcm_lmt_unk_69
{
	double _double_list[2];
	unsigned char _char;
}LCM_LMT_UNK_69;
//*lcm/lmt 전용 인자 구조체 3 (ver >= 0x69)

typedef struct _lcm_lmt_unk_6d
{
	unsigned int _int_list[4];
}LCM_LMT_UNK_6D;
//*lcm/lmt 전용 인자 구조체 4 (ver >= 0x6D)

typedef struct _lcm_lmt_unk_6e
{
	double _double_list[2];
}LCM_LMT_UNK_6E;
//*lcm/lmt 전용 인자 구조체 5 (ver >= 0x6E)

typedef struct _lcm_lmt_unk_not_6e
{
	unsigned int _int_list[2];
}LCM_LMT_UNK_NOT_6E;
//*lcm/lmt 전용 인자 구조체 6 (ver < 0x6E, type 0에서만)


class LCM_LMT_SCRIPT_ELEM
{
public:
	unsigned int _Version;						//*** lcm/lmt 스크립트의 버전
	unsigned char _elem_type;					//*** 공통
	unsigned int _unk_2[6];						//*** 공통
	unsigned char _unk_3[3];					//*** 공통

	wchar_t *_unk_3_1;							//*** 버전 0x71 이상
	
	LCM_LMT_TILE_INFO_1 *_elem_info_0_1;		//*** _elem_type이 0일 때, 1번
	LCM_LMT_UNK_6E* _unk_4_1;					//*** 버전 0x6E 이상, _elem_type이 0일 때
	LCM_LMT_UNK_NOT_6E* _unk_4_2;				//*** 버전 0x6E 미만, _elem_type이 0일 때
	LCM_LMT_TILE_INFO_2* _elem_info_0_2;		//*** _elem_type이 0일 때, 2번
	LCM_LMT_BOX_INFO* _elem_info_2;			//*** _elem_type이 2일 때
	LCM_LMT_SOUND_INFO* _elem_info_4;			//*** _elem_type이 4일 때
	LCM_LMT_UNK_65 *_unk_5;						//*** 버전 0x65 이상
	LCM_LMT_UNK_69* _unk_6;						//*** 버전 0x69 이상
	LCM_LMT_UNK_6D* _unk_7;						//*** 버전 0x6D 이상
	wchar_t* _SrcFile;							//*** 소스가 되는 파일

	LCM_LMT_SCRIPT_ELEM(unsigned int Version)
	{
		_Version = Version;
		_elem_type = 0;
		memset(_unk_2, 0, (sizeof(int) * 6));
		memset(_unk_3, 0, (sizeof(char) * 3));
		_unk_3_1 = NULL;
		_elem_info_0_1 = NULL;
		_unk_4_1 = NULL;
		_unk_4_2 = NULL;
		_elem_info_0_2 = NULL;
		_elem_info_2 = NULL;
		_elem_info_4 = NULL;
		_unk_5 = NULL;
		_unk_6 = NULL;
		_unk_7 = NULL;
		_SrcFile = NULL;
	}
	//*생성자

	~LCM_LMT_SCRIPT_ELEM()
	{
		if (_elem_info_0_1 != NULL) { free(_elem_info_0_1); }
		if (_unk_3_1 != NULL) { free(_unk_3_1); }
		if (_unk_4_1 != NULL) { free(_unk_4_1); }
		if (_unk_4_2 != NULL) { free(_unk_4_2); }
		if (_elem_info_0_2 != NULL) { free(_elem_info_0_2); }
		if (_elem_info_2 != NULL) { free(_elem_info_2); }
		if (_elem_info_4 != NULL) { free(_elem_info_4); }
		if (_unk_5 != NULL) { free(_unk_5); }
		if (_unk_6 != NULL) { free(_unk_6); }
		if (_unk_7 != NULL) { free(_unk_7); }
		if (_SrcFile != NULL) { free(_SrcFile); }
	}
	//*소멸자

	void _read_from_buffer(unsigned char** _lcm_lmt_buffer_pnt, unsigned int code);
	//*버퍼에서 읽어들이는 함수

	void _decompile_code(HANDLE hWrite, unsigned int code);
	//*디컴파일 함수

	void _write_as_lcm_or_lmt(HANDLE hNew_Lcm_Lmt, unsigned int code);
	//*lcm/lmt 파일로 기록하는 함수
};
//*이 스크립트에서 원소로 쓰이는 클래스


class LCM_LMT_SCRIPT : public LM_BASE_SCRIPT
{
public:
	wchar_t* _Signature;
	wchar_t* _Version;
	unsigned int _Width;
	unsigned int _Height;
	unsigned int _unk_1[4];
	unsigned char _unk_2;
	unsigned int _elem_count;
	LCM_LMT_SCRIPT_ELEM** _elem_list;

	LCM_LMT_SCRIPT()
	{
		_Signature = NULL;
		_Version = NULL;
		_Width = _Height = 0;
		memset(_unk_1, 0, (sizeof(int) * 4));
		_unk_2 = 0;
		_elem_count = 0;
		_elem_list = NULL;
	}
	//*생성자

	~LCM_LMT_SCRIPT()
	{
		if (_Signature != NULL) { free(_Signature); }
		if (_Version != NULL) { free(_Version); }
		if (_elem_count != 0) {
			for (unsigned int i = 0; i < _elem_count;i++) {
				if (_elem_list[i] != NULL) { delete _elem_list[i]; }
			}
			free(_elem_list);
		}
	}
	//*소멸자

	bool Script_Analysis(wchar_t* Lcm_Lmt_File, unsigned int code);
	//*lcm/lmt 파일을 불러와 분석하는 함수

	void Decompile_To_Code(wchar_t* TxtFile, unsigned int code);
	//*디컴파일해서 기록하는 함수

	void Make_To_Script(wchar_t* new_Lcm_Lmt_File, unsigned int code);
	//*lsb 파일로 기록하는 함수

	void Extract_Text_Data(wchar_t* DstTxtFile);
	//*텍스트 추출 함수

	void Replace_Text_Data(wchar_t* SrcTxtFile, unsigned int code);
	//*텍스트 교체 함수

	void Convert_Codepage_J2K();
	//*텍스트 언어코드를 일본어에서 한글로 바꾸는 함수
};
//*.lcm/*.lmt 스크립트 클래스