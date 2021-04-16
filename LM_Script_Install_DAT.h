#pragma once
#include "LM_Base.h"

//*** INSTALL.DAT 스크립트 분석 및 적용 함수들을 선언하는 헤더


typedef struct _dat_attr_elem_66
{
	wchar_t* _str;
}DAT_ATTR_ELEM_66;
//*여기서 읽지 않으면 0이 되도록 함

typedef struct _dat_attr_elem_64
{
	unsigned int _int_list[8];
}DAT_ATTR_ELEM_64;

typedef struct _dat_attr_elem_not_64
{
	unsigned int _int_list[4];
}DAT_ATTR_ELEM_NOT_64;
//*(아마도) dat 파일이 담고있는 속성
//*버전에 따라 나뉨


class INSTALL_DAT_SCRIPT : public LM_BASE_SCRIPT
{
public:
	wchar_t* _Signature;
	wchar_t* _Version;
	unsigned int _Version_Int;
	unsigned int _unk_1;
	//*_Version_Int는 기록용이 아니고 편하게 다루기 위함이다

	wchar_t* _Game_Title;
	wchar_t* _unk_2;
	wchar_t* _Compile_Dir;
	wchar_t* _unk_3;

	unsigned char _unk_4;
	wchar_t* _unk_5;
	unsigned char _unk_6;
	unsigned int _Attr_Count;

	wchar_t* _Execute_File_Name;
	unsigned int _unk_7;
	unsigned char _unk_8;
	unsigned char _unk_9;

	wchar_t* _Drive_Type;
	wchar_t* _unk_10;
	//*일단 짐작 가능한 대로 이름을 정해둔다
	//*사실 (_Attr_Count + 1)만큼 읽어들여야 한다

	DAT_ATTR_ELEM_64* _Attr_list_1;
	DAT_ATTR_ELEM_NOT_64* _Attr_list_2;
	//*버전에 따라 둘 중 하나를 읽어들인다

	unsigned char _unk_11;
	unsigned char _unk_12;
	//*_unk_12는 버전에 따라 갈림

	unsigned int _unk_13_count;
	DAT_ATTR_ELEM_66* _unk_13_list;
	//*_unk_13은 버전에 따라 갈림

	unsigned char _unk_14;
	unsigned char _unk_15;
	unsigned char _unk_16;
	//*_unk_14, 15, 16은 버전에 따라 갈림

	unsigned int _unk_17_count;
	wchar_t** _unk_17_list;
	//*_unk_17은 버전에 따라 갈림

	wchar_t* _unk_18;
	//*_unk_18은 버전에 따라 갈림

	wchar_t* _unk_19[2];
	//*_unk_19는 버전에 따라 갈림

	INSTALL_DAT_SCRIPT()
	{
		_Signature = _Version = NULL;
		_Version_Int = 0;
		_unk_1 = 0;
		_Game_Title = _unk_2 = _Compile_Dir = _unk_3 = NULL;
		_unk_4 = 0;
		_unk_5 = NULL;
		_unk_6 = 0;
		_Attr_Count = 0;
		_Execute_File_Name = NULL;
		_unk_7 = 0;
		_unk_8 = _unk_9 = 0;
		_Drive_Type = _unk_10 = 0;
		_Attr_list_1 = NULL;
		_Attr_list_2 = NULL;
		_unk_11 = _unk_12 = 0;
		_unk_13_count = 0;
		_unk_13_list = NULL;
		_unk_14 = _unk_15 = _unk_16 = 0;
		_unk_17_count = 0;
		_unk_17_list = NULL;
		_unk_18 = NULL;
		for (unsigned int i = 0; i < 2; i++) { _unk_19[i] = NULL; }
	}
	//*생성자

	~INSTALL_DAT_SCRIPT()
	{
		if (_Signature != NULL) { free(_Signature); }
		if (_Version != NULL) { free(_Version); }
		if (_Game_Title != NULL) { free(_Game_Title); }
		if (_unk_2 != NULL) { free(_unk_2); }
		if (_Compile_Dir != NULL) { free(_Compile_Dir); }
		if (_unk_3 != NULL) { free(_unk_3); }
		if (_unk_5 != NULL) { free(_unk_5); }
		if (_Attr_list_1 != NULL) { free(_Attr_list_1); }
		if (_Attr_list_2 != NULL) { free(_Attr_list_2); }
		if (_unk_13_list != NULL) { free(_unk_13_list); }
		if (_unk_17_list != NULL) { 
			for (unsigned int i = 0; i < _unk_17_count;i++) {
				if (_unk_17_list[i] != NULL) { free(_unk_17_list[i]); }
			}
			free(_unk_17_list);
		}
		if (_unk_18 != NULL) { free(_unk_18); }
		for (unsigned int i = 0; i < 2; i++) {
			if (_unk_19[i] != NULL) { free(_unk_19[i]); }
		}
	}
	//*소멸자

	bool Script_Analysis(wchar_t* DatFile, unsigned int code);
	//*install.dat 파일을 불러와 분석하는 함수

	void Decompile_To_Code(wchar_t* TxtFile, unsigned int code);
	//*디컴파일해서 기록하는 함수

	void Make_To_Script(wchar_t* newDatFile, unsigned int code);
	//*dat 파일로 기록하는 함수

	void Extract_Text_Data(wchar_t* DstTxtFile);
	//*텍스트 추출 함수

	void Replace_Text_Data(wchar_t* SrcTxtFile, unsigned int code);
	//*텍스트 교체 함수

	void Convert_Codepage_J2K();
	//*텍스트 언어코드를 일본어에서 한글로 바꾸는 함수
};
//INSTALL.DAT 스크립트 클래스