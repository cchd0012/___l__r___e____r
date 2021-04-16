#pragma once
#include "LM_Base.h"

//*** LPM 스크립트 분석 및 적용 함수들을 선언하는 헤더


typedef struct _Lpm_Item_Arg
{
	wchar_t* _str[5];
	unsigned int _int;
}LPM_ITEM_ARG;
//*아이템 클래스의 변수 같은 느낌인듯?


class LPM_ITEM
{
public:
	unsigned int _Version;								//*버튼 클래스에서 받을 버전값
	unsigned int _Left;									//*바이너리 시 "width"에 대응
	unsigned int _Top;									//*바이너리 시 "height"에 대응
	wchar_t* _Src;										//*바이너리 시 "src"에 대응
	unsigned char _unk_2;								//*바이너리 시 "unk2"에 대응
	wchar_t* _Name;										//*바이너리 시 "name"에 대응
	wchar_t* _Src_Selected;								//*바이너리 시 "src_selected"에 대응
	wchar_t* _unk_3;									//*나머지는 전부 바이너리에서 따옴
	wchar_t* _unk_4;
	wchar_t* _unk_5;
	wchar_t* _unk_6[2];
	wchar_t* _unk_7;
	wchar_t* _unk_8;
	wchar_t* _unk_9;
	wchar_t* _unk_10[2];
	unsigned int _unk_11;
	unsigned int _unk_12;
	wchar_t* _unk_13;
	LPM_ITEM_ARG _unk_14;
	wchar_t* _unk_15;
	wchar_t* _unk_16;

	LPM_ITEM (unsigned int Version)
	{
		_Version = Version;
		_Left = _Top = 0;
		_unk_2 = 0;
		_Name = _Src_Selected = NULL;
		_unk_3 = _unk_4 = _unk_5 = NULL;
		memset(_unk_6, 0, (sizeof(wchar_t*) * 2));
		_unk_7 = _unk_8 = _unk_9 = NULL;
		memset(_unk_10, 0, (sizeof(wchar_t*) * 2));
		_unk_11 = _unk_12 = 0;
		_unk_13 = NULL;
		memset(&_unk_14, 0, sizeof(LPM_ITEM_ARG));
		_unk_15 = _unk_16 = NULL;
	}
	//*생성자

	~LPM_ITEM()
	{
		if (_Name != NULL) { free(_Name); }
		if (_Src_Selected != NULL) { free(_Src_Selected); }
		if (_unk_3 != NULL) { free(_unk_3); } if (_unk_4 != NULL) { free(_unk_4); } if (_unk_5 != NULL) { free(_unk_5); }
		for (unsigned int i = 0; i < 2;i++) { 
			if (_unk_6[i] != NULL) { free(_unk_6[i]); }
		}
		if (_unk_7 != NULL) { free(_unk_7); } if (_unk_8 != NULL) { free(_unk_8); } if (_unk_9 != NULL) { free(_unk_9); }
		for (unsigned int i = 0; i < 2; i++) {
			if (_unk_10[i] != NULL) { free(_unk_10[i]); }
		}
		if (_unk_13 != NULL) { free(_unk_13); }
		for (unsigned int i = 0; i < 5; i++) {
			if (_unk_14._str[i] != NULL) { free(_unk_14._str[i]); }
		}
		if (_unk_15 != NULL) { free(_unk_15); } if (_unk_16 != NULL) { free(_unk_16); }
	}
	//*소멸자

	void _read_from_buffer(unsigned char** _lpm_buffer_pnt, unsigned int code);
	//*버퍼에서 읽어들이는 함수

	void _decompile_code(HANDLE hWrite, unsigned int code);
	//*디컴파일 함수

	void _write_as_lpm(HANDLE hNewLpm, unsigned int code);
	//*lpm 파일로 기록하는 함수

	void _change_code_J2K();
	//*텍스트 코드 변경 함수
};
//*lpm 스크립트에서 Item에 대응되는 클래스
//*xml과 바이너리에 동시 대응되도록 짜는게 너무 어려움


class LPM_BUTTON
{
public:
	unsigned int _Version;
	unsigned int _item_count;
	LPM_ITEM** _item_list;

	LPM_BUTTON()
	{
		_Version = 0;
		_item_count = 0;
		_item_list = NULL;
	}
	//*생성자

	~LPM_BUTTON()
	{
		if (_item_list != NULL) {
			for (unsigned int i = 0; i < _item_count;i++) {
				if (_item_list[i] != NULL) { delete _item_list[i]; }
			}
			free (_item_list);
		}
	}
	//*소멸자

	void _read_from_buffer(unsigned char** _lpm_buffer_pnt, unsigned int code);
	//*버퍼에서 읽어들이는 함수

	void _decompile_code(HANDLE hWrite, unsigned int code);
	//*디컴파일 함수

	void _write_as_lpm(HANDLE hNewLpm, unsigned int code);
	//*lpm 파일로 기록하는 함수

	void _change_code_J2K();
	//*텍스트 코드 변경 함수
};
//*lpm 스크립트에서 Button에 대응되는 클래스


class LPM_PREVMENU
{
public:
	wchar_t* _Signature;
	wchar_t* _Version;
	unsigned int _Width;
	unsigned int _Height;
	LPM_BUTTON* _Button;

	LPM_PREVMENU()
	{
		_Signature = _Version = NULL;
		_Width = _Height = 0;
		_Button = new LPM_BUTTON();
	}
	//*생성자

	~LPM_PREVMENU()
	{
		if (_Signature != NULL) { free(_Signature); }
		if (_Version != NULL) { free(_Version); }
		if (_Button != NULL) { delete _Button; }
	}
	//*소멸자

	void _read_from_buffer(unsigned char** _lpm_buffer_pnt, unsigned int code);
	//*버퍼에서 읽어들이는 함수

	void _decompile_code(HANDLE hWrite, unsigned int code);
	//*디컴파일 함수

	void _write_as_lpm(HANDLE hNewLpm, unsigned int code);
	//*lpm 파일로 기록하는 함수

	void _change_code_J2K();
	//*텍스트 코드 변경 함수
};
//*lpm 스크립트에서 PrevMenu에 대응되는 클래스
//* 근데 lpm이 xml파일인 경우도 있고 바이너리도 되어있는 경우도 있어서 경우에 따라 나눠야 할 듯 하다
//* xml파일일 경우는 아예 디컴파일이 되어있으니 건들 필요가 없음


class LPM_SCRIPT : public LM_BASE_SCRIPT
{
public:
	bool _is_xml_script;						//*** xml 파일인지 판별하는 변수
	wchar_t* _Lpm_Buffer;						//*** xml 파일이라면 그냥 전부 읽어들여 소지함
	LPM_PREVMENU *_PrevMenu;					//*** lpm 스크립트에 선언된 PrevMenu 클래스

	LPM_SCRIPT()
	{
		_is_xml_script = false;
		_Lpm_Buffer = NULL;
		_PrevMenu = NULL;
	}
	//*생성자

	~LPM_SCRIPT()
	{
		if (_Lpm_Buffer != NULL) { free (_Lpm_Buffer); }
		if (_PrevMenu != NULL) { delete _PrevMenu; }
	}
	//*소멸자

	bool Script_Analysis(wchar_t* LpmFile, unsigned int code);
	//*lpm 파일을 불러와 분석하는 함수

	void Decompile_To_Code(wchar_t* TxtFile, unsigned int code);
	//*디컴파일해서 기록하는 함수

	void Make_To_Script(wchar_t* newLpmFile, unsigned int code);
	//*lpm 파일로 기록하는 함수

	void Extract_Text_Data(wchar_t* DstTxtFile);
	//*텍스트 추출 함수

	void Replace_Text_Data(wchar_t* SrcTxtFile, unsigned int code);
	//*텍스트 교체 함수

	void Convert_Codepage_J2K();
	//*텍스트 언어코드를 일본어에서 한글로 바꾸는 함수
	
private:
	bool _analysis_as_xml(wchar_t* LpmFile, unsigned int code);
	//*xml로 삼아 분석하는 함수
	
	bool _analysis_as_binary(wchar_t* LpmFile, unsigned int code);
	//*이진파일로 삼아 분석하는 함수
};
//*.lpm 스크립트 클래스
//*xml로 되어 있으면 아예 분석을 할 필요가 없다