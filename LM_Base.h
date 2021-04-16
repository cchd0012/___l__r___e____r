#pragma once
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <Windows.h>
#include <direct.h>
#include <vector>
#include <string>
#include <tchar.h>


//*** 기본적으로 포함하는 헤더 파일


using namespace std;

#define MZ_MAGIC				0x00505A4D
#define LV_MAGIC				0x0000766C
#define VF_MAGIC				0x00006676
#define VF_VERSION				0x00000066
//*** 아카이브 관련 값

#define LM_VERSION_1			0x00000067
#define LM_VERSION_2			0x00000074
#define LM_VERSION_3			0x00000075
//*** 스크립트 버전 값


#define TXT_BUFF_LEN			0x400000
extern wchar_t* _txt_buf;
//*_txt_buf는 텍스트 출력용 버퍼다


enum LM_SCR_TYPES
{
	TYPE_ARG_VAR = 0x00,
	TYPE_ARG_INT = 0x01,
	TYPE_ARG_FLOAT = 0x02,
	TYPE_ARG_BOOL = 0x03,
	TYPE_ARG_STR = 0x04,
	TYPE_ARG_CALC_FUNC = 0x0B,

	TYPE_ARG_UNKNOWN = 0xFFFFFFFF
};
//*** 인자 (argument) 타입 열거형

typedef struct _lsb_arg_struct
{
	LM_SCR_TYPES _type;								//*** 인자의 타입
	void* _data;									//*** 인자가 가지는 데이터
}LM_SCR_ARG;
//*** 인자 구조체


typedef struct F_DESC
{
	char *FileName;
	wchar_t* FileName_uni;
	unsigned char *FileBuffer;
	unsigned int BufferSize;
	unsigned int OffsetInVF;
	bool IsCompressed;
	bool IsScrambled;
}FILE_DESC;
//*파일 구조


class LM_BASE_SCRIPT
{
public:
	virtual bool Script_Analysis(wchar_t* SrciptFile, unsigned int code) = 0;
	//*스크립트 파일을 불러와 분석하는 함수
	virtual void Decompile_To_Code(wchar_t* TxtFile, unsigned int code) = 0;
	//*디컴파일해서 기록하는 함수
	virtual void Make_To_Script(wchar_t* newLpmFile, unsigned int code) = 0;
	//*스크립트 파일로 기록하는 함수
	virtual void Extract_Text_Data(wchar_t* DstTxtFile) = 0;
	//*텍스트 추출 함수
	virtual void Replace_Text_Data(wchar_t* SrcTxtFile, unsigned int code) = 0;
	//*텍스트 교체 함수
	virtual void Convert_Codepage_J2K() = 0;
	//*텍스트 언어코드를 일본어에서 한글로 바꾸는 함수
};
//***베이스 스크립트 클래스