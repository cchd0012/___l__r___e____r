#pragma once
#include "LM_Base.h"

//*** LPB 스크립트 분석 및 적용 함수들을 선언하는 헤더


typedef struct _lpb_arg_struct
{
	wchar_t* _arg_name;							//*** 인자 이름
	LM_SCR_ARG _arg_attr;						//*** 인자의 속성
}LPB_SYS_DATA;
//*lpb 전용 시스템 세팅 구조체


class LPB_SCRIPT : public LM_BASE_SCRIPT
{
public:
	unsigned int lm_version;					//*** 스크립트 버전
	wchar_t* _proj_name;						//*** 이 게임(프로젝트)의 이름
	unsigned int _resolution1_width;			//*** 해상도 너비?
	unsigned int _resolution1_height;			//*** 해상도 높이?
	unsigned int _resolution2_width;			//*** 해상도 너비?
	unsigned int _resolution2_height;			//*** 해상도 높이?
	wchar_t* _init_lsb;							//*** 시작시 호출하는 lsb 파일
	wchar_t* _exit_lsb;							//*** 종료시 호출하는 lsb 파일
	wchar_t* _proj_dir;							//*** 프로젝트 디렉토리
	unsigned int _unk_1;						//*** 모르는 값
	unsigned char _flag_1;						//*** 플래그
	unsigned char _flag_2;						//*** 플래그
	wchar_t* _support_audio_format;				//*** 지원하는 오디오 형식
	unsigned char _flag_3;						//*** 플래그
	unsigned char _flag_4;						//*** 플래그
	unsigned char _flag_5;						//*** 플래그
	wchar_t* _disk_insert_prompt;				//*** 디스크 미삽입 시 문구
	wchar_t* _exit_prompt;						//*** 종료 문구
	unsigned int _sys_setting_count;			//*** 시스템 세팅 요소의 수
	LPB_SYS_DATA* _sys_setting_list;			//*** 시스템 세팅 리스트
	unsigned int _dir_count;					//*** 이 게임(프로젝트)에서 이용되는 디렉토리의 수
	wchar_t** _dir_list;						//*** 이 게임에서 이용되는 디렉토리 리스트

	LPB_SCRIPT()
	{
		lm_version = 0;
		_proj_name = NULL;
		_resolution1_width = _resolution1_height = _resolution2_width = _resolution2_height = 0;
		_init_lsb = _exit_lsb = NULL;
		_proj_dir = NULL;
		_unk_1 = 0;
		_flag_1 = _flag_2 = _flag_3 = _flag_4 = _flag_5 = 0;
		_support_audio_format = NULL;
		_disk_insert_prompt = _exit_prompt = NULL;
		_sys_setting_count = 0;
		_sys_setting_list = NULL;
		_dir_count = 0;
		_dir_list = NULL;
	}
	//*생성자

	~LPB_SCRIPT()
	{
		if (_proj_name != NULL) { free(_proj_name); }
		if (_init_lsb != NULL) { free(_init_lsb); }
		if (_exit_lsb != NULL) { free(_exit_lsb); }
		if (_proj_dir != NULL) { free(_proj_dir); }
		if (_support_audio_format != NULL) { free(_support_audio_format); }
		if (_disk_insert_prompt != NULL) { free(_disk_insert_prompt); }
		if (_exit_prompt != NULL) { free(_exit_prompt); }

		if (_sys_setting_list != NULL) {
			for (unsigned int i = 0; i < _sys_setting_count; i++) {
				if (_sys_setting_list[i]._arg_name != NULL) { free(_sys_setting_list[i]._arg_name); }
				if (_sys_setting_list[i]._arg_attr._data != NULL) { free(_sys_setting_list[i]._arg_attr._data); }
			}
			free(_sys_setting_list);
		}

		if (_dir_list != NULL) {
			for (unsigned int i = 0; i < _dir_count; i++) {
				if (_dir_list[i] != NULL) { free(_dir_list[i]); }
			}
			free(_dir_list);
		}
	}
	//*소멸자

	bool Script_Analysis(wchar_t* LpbFile, unsigned int code);
	//*lpb 파일을 불러와 분석하는 함수

	void Decompile_To_Code(wchar_t* TxtFile, unsigned int code);
	//*디컴파일해서 기록하는 함수

	void Make_To_Script(wchar_t* newLpbFile, unsigned int code);
	//*lpb 파일로 기록하는 함수

	void Extract_Text_Data(wchar_t* DstTxtFile);
	//*텍스트 추출 함수

	void Replace_Text_Data(wchar_t* SrcTxtFile, unsigned int code);
	//*텍스트 교체 함수

	void Convert_Codepage_J2K();
	//*텍스트 언어코드를 일본어에서 한글로 바꾸는 함수
};
//*.lpb 스크립트 클래스