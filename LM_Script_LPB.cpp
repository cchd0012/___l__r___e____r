#include "LM_Script.h"
#include "LM_Functions.h"


#pragma warning (disable:6031)
#pragma warning (disable:4996)


bool LPB_SCRIPT::Script_Analysis(wchar_t* LpbFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Analysis: Lpb File: %s\n"), LpbFile);

	HANDLE hLpb = CreateFileW(LpbFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hLpb == INVALID_HANDLE_VALUE) {
		wprintf(TEXT("%s: There's not LPB File\n"), LpbFile);
		_setmode(_fileno(stdout), _O_TEXT);
		return false;
	}
	unsigned int lpb_size = GetFileSize(hLpb, NULL);
	unsigned char* lpb_buffer = (unsigned char*)malloc(lpb_size);
	ReadFile(hLpb, lpb_buffer, lpb_size, NULL, NULL);
	unsigned char* lpb_buffer_pnt = lpb_buffer;
	CloseHandle(hLpb);
	//*lpb 파일 읽어들이기 (파일 경로 언어코드도 고려할 필요 있음)

	unsigned int* _int;
	unsigned char* _c;
	char* _str;

	_int = _get_int(&lpb_buffer_pnt);
	lm_version = *_int; free(_int);
	//*스크립트 버전

	_str = _get_str(&lpb_buffer_pnt);
	_proj_name = GetUniStr(_str, code); free(_str);
	//*프로젝트 이름

	_int = _get_int(&lpb_buffer_pnt);
	_resolution1_width = *_int; free(_int);
	_int = _get_int(&lpb_buffer_pnt);
	_resolution1_height = *_int; free(_int);
	_int = _get_int(&lpb_buffer_pnt);
	_resolution2_width = *_int; free(_int);
	_int = _get_int(&lpb_buffer_pnt);
	_resolution2_height = *_int; free(_int);
	//*해상도값(?)

	_str = _get_str(&lpb_buffer_pnt);
	_init_lsb = GetUniStr(_str, code); free(_str);
	_str = _get_str(&lpb_buffer_pnt);
	_exit_lsb = GetUniStr(_str, code); free(_str);
	//*시작 / 종료 lsb 파일

	_str = _get_str(&lpb_buffer_pnt);
	_proj_dir = GetUniStr(_str, code); free(_str);
	//*프로젝트 디렉토리

	_int = _get_int(&lpb_buffer_pnt);
	_unk_1 = *_int; free(_int);
	//*모르는 값

	_c = _get_byte(&lpb_buffer_pnt);
	_flag_1 = *_c; free(_c);
	if (lm_version >= 0x6A) {
		_c = _get_byte(&lpb_buffer_pnt);
		_flag_2 = *_c; free(_c);
	}
	//*플래그 1 / 2 (버전 구분)

	if (lm_version >= 0x6D) {
		_str = _get_str(&lpb_buffer_pnt);
		_support_audio_format = GetUniStr(_str, code); free(_str);
	}
	//*지원하는 오디오 형식 (버전 구분)

	if (lm_version >= 0x71) {
		_c = _get_byte(&lpb_buffer_pnt);
		_flag_3 = *_c; free(_c);
	}
	if (lm_version >= 0x72) {
		_c = _get_byte(&lpb_buffer_pnt);
		_flag_4 = *_c; free(_c);
	}
	if (lm_version >= 0x74) {
		_c = _get_byte(&lpb_buffer_pnt);
		_flag_5 = *_c; free(_c);
	}
	//*플래그 3 / 4 / 5 (버전 구분)

	_str = _get_str(&lpb_buffer_pnt);
	_disk_insert_prompt = GetUniStr(_str, code); free(_str);
	_str = _get_str(&lpb_buffer_pnt);
	_exit_prompt = GetUniStr(_str, code); free(_str);
	//*디스크 미삽입 시 문구 / 종료 문구

	_int = _get_int(&lpb_buffer_pnt);
	_sys_setting_count = *_int; free(_int);
	//*시스템 세팅 수

	_sys_setting_list = (LPB_SYS_DATA*)malloc(sizeof(LPB_SYS_DATA) * _sys_setting_count);
	memset(_sys_setting_list, 0, (sizeof(LPB_SYS_DATA) * _sys_setting_count));
	for (unsigned int i = 0; i < _sys_setting_count;i++) {
		_c = _get_byte(&lpb_buffer_pnt);
		_sys_setting_list[i]._arg_attr._type = (LM_SCR_TYPES)*_c; free(_c);
		//*타입
		_str = _get_str(&lpb_buffer_pnt);
		_sys_setting_list[i]._arg_name = GetUniStr(_str, code); free(_str);
		//*이름
		switch (_sys_setting_list[i]._arg_attr._type)
		{
			case TYPE_ARG_INT:
			{
				_sys_setting_list[i]._arg_attr._data = _get_int(&lpb_buffer_pnt);
				break;
			}
			case TYPE_ARG_FLOAT:
			{
				_sys_setting_list[i]._arg_attr._data = _get_data(&lpb_buffer_pnt, 0xA);
				break;
			}
			case TYPE_ARG_BOOL:
			{
				_sys_setting_list[i]._arg_attr._data = _get_byte(&lpb_buffer_pnt);
				break;
			}
			case TYPE_ARG_STR:
			{
				_str = _get_str(&lpb_buffer_pnt);
				_sys_setting_list[i]._arg_attr._data = GetUniStr(_str, code);
				free(_str);
				break;
			}
			default:
			{
				_str = _get_str(&lpb_buffer_pnt);
				_sys_setting_list[i]._arg_attr._data = GetUniStr(_str, code);
				free(_str);
				break;
			}
		}
		//*데이터
	}
	//*시스템 세팅 리스트

	_int = _get_int(&lpb_buffer_pnt);
	_dir_count = *_int; free(_int);
	//*디렉토리 수
	
	_dir_list = (wchar_t**)malloc(sizeof(wchar_t*) * _dir_count);
	memset(_dir_list, 0, (sizeof(wchar_t*) * _dir_count));
	for (unsigned int i = 0; i < _dir_count; i++) {
		_str = _get_str(&lpb_buffer_pnt);
		_dir_list[i] = GetUniStr(_str, code); free(_str);
		lpb_buffer_pnt += sizeof(int);
		//*마지막에 4바이트 빈칸이 남으므로 더해줘야 한다
	}
	//*디렉토리 리스트

	wprintf(TEXT("Script Analysis Complete!\n"));
	free(lpb_buffer);
	_setmode(_fileno(stdout), _O_TEXT);
	return true;
	//*정리
}
//*lpb 파일을 불러와 분석하는 함수


void LPB_SCRIPT::Decompile_To_Code(wchar_t* TxtFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Decompiling to Code: Lpb File: To %s\n"), TxtFile);

	HANDLE hWrite = CreateFileW(TxtFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//*핸들 생성

	UINT16 BOM = 0xFEFF;
	WriteFile(hWrite, &BOM, 2, NULL, NULL);
	//*유니코드 형식 추출 준비 (BOM)

	memset(_txt_buf, 0, sizeof(wchar_t) * TXT_BUFF_LEN);
	//*맨 처음은 반드시 0으로 초기화함

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("Livemaker Script Version_0x%02X%s%s"), lm_version, _enter_raw_, separation_line);
	//*버전 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s]::%s%s"), proj_name_prefix, _proj_name, _enter_raw_);
	//*프로젝트 이름 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Resolution]::(%d * %d) / (%d * %d)%s"), 
		_resolution1_width, _resolution1_height, _resolution2_width, _resolution2_height, _enter_raw_);
	//*해상도 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Init LSB]::%s%s"), _init_lsb, _enter_raw_);
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Exit LSB]::%s%s"), _exit_lsb, _enter_raw_);
	//*시작 / 종료 lsb 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Project Directory]::%s_0x%08X%s"), _proj_dir, _unk_1, _enter_raw_);
	//*프로젝트 디렉토리 / 모르는 숫자 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Flags]::[%02X][%02X][%02X][%02X][%02X]%s"), 
		_flag_1, _flag_2, _flag_3, _flag_4, _flag_5, _enter_raw_);
	//*플래그 5개 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Support Audio Format]::%s%s"), _support_audio_format, _enter_raw_);
	//*지원하는 오디오 형식 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s]::%s%s"), disk_insert_prefix, _disk_insert_prompt, _enter_raw_);
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s]::%s%s"), exit_prefix, _exit_prompt, _enter_raw_);
	WriteFile(hWrite, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//*디스크 미삽입 시 문구 / 종료 문구 기록
	//*1차 기록

	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[%s]%s"), _enter_raw_, system_setting_prefix, _enter_raw_);
	for (unsigned int i = 0; i < _sys_setting_count;i++) {
		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("  [Name]::%s%s    [Data]::"), 
			_sys_setting_list[i]._arg_name, _enter_raw_);
		switch (_sys_setting_list[i]._arg_attr._type)
		{
			case TYPE_ARG_INT:
			{
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("0x%08X%s"), 
					*(unsigned int*)_sys_setting_list[i]._arg_attr._data, _enter_raw_);
				break;
			}
			case TYPE_ARG_FLOAT:
			{
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%lf%s"),
					*(double*)_sys_setting_list[i]._arg_attr._data, _enter_raw_);
				break;
			}
			case TYPE_ARG_BOOL:
			{
				if (*(bool*)_sys_setting_list[i]._arg_attr._data) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[True]%s"), _enter_raw_); }
				else { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[False]%s"), _enter_raw_); }
				break;
			}
			case TYPE_ARG_STR:
			{
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s%s%s%s"),
					text_prefix, (wchar_t*)_sys_setting_list[i]._arg_attr._data, text_prefix, _enter_raw_);
				break;
			}
			default:
			{
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s%s%s%s"),
					text_prefix, (wchar_t*)_sys_setting_list[i]._arg_attr._data, text_prefix, _enter_raw_);
				break;
			}
		}
	}
	WriteFile(hWrite, _txt_buf, (sizeof(wchar_t)* wcslen(_txt_buf)), NULL, NULL);
	//* 시스템 세팅 리스트
	//*2차 기록

	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[Directories]%s"), _enter_raw_, _enter_raw_);
	for (unsigned int i = 0; i < _dir_count; i++) {
		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("  %s%s%s%s"),
			text_prefix, _dir_list[i], text_prefix, _enter_raw_);
	}
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), separation_line);
	WriteFile(hWrite, _txt_buf, (sizeof(wchar_t)* wcslen(_txt_buf)), NULL, NULL);
	//* 디렉토리 리스트
	//* 3차 기록

	CloseHandle(hWrite);
	//*핸들 해제

	wprintf(TEXT("Decompiling Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	//*정리

}
//*디컴파일해서 기록하는 함수


void LPB_SCRIPT::Make_To_Script(wchar_t* newLpbFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Writing: Lpb File: %s\n"), newLpbFile);

	HANDLE hNewLpb = CreateFileW(newLpbFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//*핸들 생성

	char* _s;
	//*기록용 변수

	_write_int(hNewLpb, lm_version);
	//*버전 기록

	_s = GetAnsiStr(_proj_name, code);
	_write_str(hNewLpb, _s);
	free(_s);
	//*프로젝트명 기록

	_write_int(hNewLpb, _resolution1_width);
	_write_int(hNewLpb, _resolution1_height);
	_write_int(hNewLpb, _resolution2_width);
	_write_int(hNewLpb, _resolution2_height);
	//*해상도 기록

	_s = GetAnsiStr(_init_lsb, code);
	_write_str(hNewLpb, _s);
	free(_s);
	//*시작 lsb 파일명 기록

	_s = GetAnsiStr(_exit_lsb, code);
	_write_str(hNewLpb, _s);
	free(_s);
	//*종료 lsb 파일명 기록

	_s = GetAnsiStr(_proj_dir, code);
	_write_str(hNewLpb, _s);
	free(_s);
	//*프로젝트 디렉토리 기록

	_write_int(hNewLpb, _unk_1);
	//*모르는 값 기록

	_write_byte(hNewLpb, _flag_1);
	if (lm_version >= 0x6A) { _write_byte(hNewLpb, _flag_2); }
	//*플래그 1 / 2 기록 (버전 구분)

	if (lm_version >= 0x6D) {
		_s = GetAnsiStr(_support_audio_format, code);
		_write_str(hNewLpb, _s);
		free(_s);
	}
	//*지원하는 오디오 형식 기록 (버전 구분)

	if (lm_version >= 0x71) { _write_byte(hNewLpb, _flag_3); }
	if (lm_version >= 0x72) { _write_byte(hNewLpb, _flag_4); }
	if (lm_version >= 0x74) { _write_byte(hNewLpb, _flag_5); }
	//*플래그 3 / 4 / 5 기록 (버전 구분)

	_s = GetAnsiStr(_disk_insert_prompt, code);
	_write_str(hNewLpb, _s);
	free(_s);
	//*디스크 미삽입 시 문구 기록

	_s = GetAnsiStr(_exit_prompt, code);
	_write_str(hNewLpb, _s);
	free(_s);
	//*종료 문구 기록

	_write_int(hNewLpb, _sys_setting_count);
	//*시스템 세팅 수 기록

	for (unsigned int i = 0; i < _sys_setting_count; i++) {

		_write_byte(hNewLpb, _sys_setting_list[i]._arg_attr._type);
		//*타입 기록

		_s = GetAnsiStr(_sys_setting_list[i]._arg_name, code);
		_write_str(hNewLpb, _s);
		free(_s);
		//*이름 기록

		switch (_sys_setting_list[i]._arg_attr._type)
		{
			case TYPE_ARG_INT:
			{
				_write_int(hNewLpb, *(unsigned int*)_sys_setting_list[i]._arg_attr._data);
				break;
			}
			case TYPE_ARG_FLOAT:
			{
				_write_data(hNewLpb, (unsigned char*)_sys_setting_list[i]._arg_attr._data, 0xA);
				break;
			}
			case TYPE_ARG_BOOL:
			{
				_write_byte(hNewLpb, *(unsigned char*)_sys_setting_list[i]._arg_attr._data);
				break;
			}
			case TYPE_ARG_STR:
			{
				_s = GetAnsiStr((wchar_t*)_sys_setting_list[i]._arg_attr._data, code);
				_write_str(hNewLpb, _s);
				free(_s);
				break;
			}
			default:
			{
				_s = GetAnsiStr((wchar_t*)_sys_setting_list[i]._arg_attr._data, code);
				_write_str(hNewLpb, _s);
				free(_s);
				break;
			}
		}
		//*데이터 기록
	}
	//*시스템 세팅 리스트 기록

	_write_int(hNewLpb, _dir_count);
	//*디렉토리 수 기록

	char* _dummy = "";
	for (unsigned int i = 0; i < _dir_count; i++) {
		_s = GetAnsiStr(_dir_list[i], code);
		_write_str(hNewLpb, _s);
		_write_str(hNewLpb, _dummy);
		free(_s);
		//*마지막에 4바이트 더미를 추가로 기록함
	}
	//*디렉토리 리스트 기록

	CloseHandle(hNewLpb);
	//*핸들 해제

	wprintf(TEXT("Writing Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	//*정리

}
//*lpb 파일로 기록하는 함수


void LPB_SCRIPT::Extract_Text_Data(wchar_t* DstTxtFile)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Extracting: Text File: %s\n"), DstTxtFile);

	HANDLE hTxt = CreateFileW(DstTxtFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//*핸들 생성

	UINT16 BOM = 0xFEFF;
	WriteFile(hTxt, &BOM, 2, NULL, NULL);
	//*유니코드 형식 추출 준비 (BOM)

	memset(_txt_buf, 0, sizeof(wchar_t) * TXT_BUFF_LEN);
	//*맨 처음은 반드시 0으로 초기화함

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s]::[%s]::%s%s%s%s"), 
		proj_name_prefix, text_data_prefix, text_prefix, _proj_name, text_prefix, _enter_raw_);
	//*프로젝트 이름 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s]::[%s]::%s%s%s%s"), 
		disk_insert_prefix, text_data_prefix, text_prefix, _disk_insert_prompt, text_prefix, _enter_raw_);
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s]::[%s]::%s%s%s%s"),
		exit_prefix, text_data_prefix, text_prefix, _exit_prompt, text_prefix, _enter_raw_);
	WriteFile(hTxt, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//*디스크 미삽입 시 문구 / 종료 문구 기록
	//*1차 기록

	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[%s]%s"), _enter_raw_, system_setting_prefix, _enter_raw_);
	for (unsigned int i = 0; i < _sys_setting_count; i++) {
		if (_sys_setting_list[i]._arg_attr._type == TYPE_ARG_STR) {
			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("  [%s]::%s%s%s::[%s]::%s%s%s%s"),
				arg_name_prefix, 
				text_prefix, _sys_setting_list[i]._arg_name, text_prefix,
				text_data_prefix,
				text_prefix, (wchar_t*)_sys_setting_list[i]._arg_attr._data, text_prefix,
				_enter_raw_
			);
		}
	}
	WriteFile(hTxt, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//* 시스템 세팅 리스트
	//*2차 기록

	CloseHandle(hTxt);
	//*핸들 해제

	wprintf(TEXT("Extracting Text Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	//*정리
}
//*텍스트 추출 함수
//*추출 대상 : 프로젝트명, 디스크 미삽입시 문구 / 종료 문구, 시스템 세팅 중 문자열로 된 값들 전부


void LPB_SCRIPT::Replace_Text_Data(wchar_t* SrcTxtFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Replacing: Text File: %s\n"), SrcTxtFile);

	HANDLE hTxt = CreateFileW(SrcTxtFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hTxt == INVALID_HANDLE_VALUE) {
		wprintf(TEXT("%s: Can't Open File\n"), SrcTxtFile);
		_setmode(_fileno(stdout), _O_TEXT);
		return;
	}
	//*핸들 생성

	UINT16 _chk = 0;
	ReadFile(hTxt, &_chk, 2, NULL, NULL);
	if (_chk != 0xFEFF) {
		wprintf(TEXT("%s: This File Isn't Unicode File\n"), SrcTxtFile);
		_setmode(_fileno(stdout), _O_TEXT);
		CloseHandle(hTxt);
		return;
	}
	//*유니코드 텍스트인지 체크

	unsigned int txt_size = GetFileSize(hTxt, NULL);
	unsigned int total_txt_count = (txt_size / sizeof(wchar_t)) - 1;
	wchar_t* txt_buffer = (wchar_t*)malloc((txt_size));
	ReadFile(hTxt, txt_buffer, (txt_size - sizeof(wchar_t)), NULL, NULL);
	txt_buffer[total_txt_count] = 0;
	CloseHandle(hTxt);
	//*텍스트 파일 읽어들이기 / 핸들 해제

	unsigned int text_count = Get_Txt_Count(txt_buffer, text_data_prefix, total_txt_count);
	//*텍스트 수 확인

	wchar_t** text_list = (wchar_t**)malloc(sizeof(wchar_t*) * text_count);
	memset(text_list, 0, (sizeof(wchar_t*) * text_count));
	wchar_t* _txt_pnt = txt_buffer, * _txt_pnt_bak = txt_buffer;

	for (unsigned int i = 0; i < text_count;i++) {
		_txt_pnt = wcsstr(_txt_pnt_bak, text_data_prefix) + wcslen(text_data_prefix);
		_txt_pnt_bak = wcsstr(_txt_pnt, text_prefix) + wcslen(text_prefix);
		//*먼저 텍스트 시작 위치 파악
		//*_txt_pnt_bak : 텍스트 시작 위치

		_txt_pnt = wcsstr(_txt_pnt_bak, text_prefix);
		//*_txt_pnt : 텍스트 끝 위치

		unsigned int cp_len = _txt_pnt - _txt_pnt_bak;
		text_list[i] = (wchar_t*)malloc(sizeof(wchar_t) * (cp_len + 1));
		text_list[i][cp_len] = 0;
		memcpy(text_list[i], _txt_pnt_bak, (sizeof(wchar_t) * cp_len));
		//*텍스트 데이터 할당 및 복사

		_txt_pnt_bak = _txt_pnt + wcslen(text_prefix);
		//*_txt_pnt_bak 위치 갱신
	}
	//*텍스트 배치

	wchar_t* _tmp;

	_tmp = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(text_list[0]) + 1));
	memcpy(_tmp, text_list[0], (sizeof(wchar_t) * wcslen(text_list[0])));
	_tmp[wcslen(text_list[0])] = 0;
	if (_proj_name != NULL) { free(_proj_name); }
	_proj_name = _tmp;
	//*프로젝트명 세팅

	_tmp = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(text_list[1]) + 1));
	memcpy(_tmp, text_list[1], (sizeof(wchar_t) * wcslen(text_list[1])));
	_tmp[wcslen(text_list[1])] = 0;
	if (_disk_insert_prompt != NULL) { free(_disk_insert_prompt); }
	_disk_insert_prompt = _tmp;
	//*디스크 미삽입시 문구 세팅

	_tmp = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(text_list[2]) + 1));
	memcpy(_tmp, text_list[2], (sizeof(wchar_t) * wcslen(text_list[2])));
	_tmp[wcslen(text_list[2])] = 0;
	if (_exit_prompt != NULL) { free(_exit_prompt); }
	_exit_prompt = _tmp;
	//*종료시 문구 세팅

	unsigned int accum_idx = 3;
	for (unsigned int i = 0; i < _sys_setting_count; i++) {
		if (_sys_setting_list[i]._arg_attr._type == TYPE_ARG_STR) {
			_tmp = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(text_list[accum_idx]) + 1));
			memcpy(_tmp, text_list[accum_idx], (sizeof(wchar_t) * wcslen(text_list[accum_idx])));
			_tmp[wcslen(text_list[accum_idx])] = 0;
			accum_idx++;
			if (_sys_setting_list[i]._arg_attr._data != NULL) { free(_sys_setting_list[i]._arg_attr._data); }
			_sys_setting_list[i]._arg_attr._data = _tmp;
		}
	}
	//*루프 돌아가면서 텍스트 교체

	for (unsigned int i = 0; i < text_count; i++) {
		if (text_list[i] != NULL) { free(text_list[i]); }
	}
	free(text_list);
	free(txt_buffer);
	//*버퍼 해제

	wprintf(TEXT("Replacing Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	//*정리
}
//*텍스트 교체 함수


void LPB_SCRIPT::Convert_Codepage_J2K()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Changing Text Language Code(Jap -> Kor)...\n"));

	JPHan_2_KRHan(_proj_name);
	JPHan_2_KRHan(_init_lsb);
	JPHan_2_KRHan(_exit_lsb);
	JPHan_2_KRHan(_proj_dir);
	JPHan_2_KRHan(_support_audio_format);
	JPHan_2_KRHan(_disk_insert_prompt);
	JPHan_2_KRHan(_exit_prompt);
	//*먼저 wchar_t*로 된 것들을 모두 바꾼다

	for (unsigned int i = 0; i < _sys_setting_count; i++) {
		JPHan_2_KRHan (_sys_setting_list[i]._arg_name);
		//*이름은 기본으로 바꾼다
		if (_sys_setting_list[i]._arg_attr._type == TYPE_ARG_STR) {
			JPHan_2_KRHan((wchar_t*)_sys_setting_list[i]._arg_attr._data);
		}
		//*데이터는 문자열 타입일 때만 바꾼다
	}
	//*시스템 세팅 데이터의 언어코드를 바꾼다

	for (unsigned int i = 0; i < _dir_count; i++) {
		JPHan_2_KRHan(_dir_list[i]);
	}
	//*디렉토리 리스트의 언어코드를 바꾼다

	wprintf(TEXT("Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
}
//*텍스트 언어코드를 일본어에서 한글로 바꾸는 함수