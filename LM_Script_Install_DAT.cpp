#include "LM_Script.h"
#include "LM_Functions.h"


#pragma warning (disable:6031)
#pragma warning (disable:4996)


//*** install.dat 스크립트 분석 및 적용 함수들을 서술하는 파일


//---------------------------- 스크립트 클래스 관련 함수 ----------------------------//
bool INSTALL_DAT_SCRIPT::Script_Analysis(wchar_t* DatFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Analysis: Lpb File: %s\n"), DatFile);

	HANDLE hDat = CreateFileW(DatFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDat == INVALID_HANDLE_VALUE) {
		wprintf(TEXT("%s: There's not DAT File\n"), DatFile);
		_setmode(_fileno(stdout), _O_TEXT);
		return false;
	}
	unsigned int dat_size = GetFileSize(hDat, NULL);
	unsigned char* dat_buffer = (unsigned char*)malloc(dat_size);
	ReadFile(hDat, dat_buffer, dat_size, NULL, NULL);
	unsigned char* dat_buffer_pnt = dat_buffer;
	CloseHandle(hDat);
	//*dat 파일 읽어들이기 (파일 경로 언어코드도 고려할 필요 있음)

	unsigned int* _i;
	unsigned char* _b;
	char* _s;

	_s = (char*)_get_data(&dat_buffer_pnt, 2);
	_Signature = GetUniStr(_s, code);
	free(_s);
	//*시그니처 문자열 읽어들이기

	_s = (char*)_get_data(&dat_buffer_pnt, 3);
	_Version = GetUniStr(_s, code);
	_Version_Int = strtol(_s, NULL, 10);
	free(_s);
	//*버전 읽어들인 후 숫자 변환

	_i = _get_int(&dat_buffer_pnt);
	_unk_1 = *_i; free(_i);
	//*모르는 값 1

	_s = _get_str(&dat_buffer_pnt);
	_Game_Title = GetUniStr(_s, code); free(_s);
	_s = _get_str(&dat_buffer_pnt);
	_unk_2 = GetUniStr(_s, code); free(_s);
	_s = _get_str(&dat_buffer_pnt);
	_Compile_Dir = GetUniStr(_s, code); free(_s);
	_s = _get_str(&dat_buffer_pnt);
	_unk_3 = GetUniStr(_s, code); free(_s);
	//*문자열 4개 순서대로

	_b = _get_byte(&dat_buffer_pnt);
	_unk_4 = *_b; free(_b);
	_s = _get_str(&dat_buffer_pnt);
	_unk_5 = GetUniStr(_s, code); free(_s);
	_b = _get_byte(&dat_buffer_pnt);
	_unk_6 = *_b; free(_b);
	//*모르는 값 4 / 5 / 6

	_i = _get_int(&dat_buffer_pnt);
	_Attr_Count = *_i; free(_i);
	//*속성(?)의 수

	_s = _get_str(&dat_buffer_pnt);
	_Execute_File_Name = GetUniStr(_s, code); free(_s);
	//*실행파일명

	_i = _get_int(&dat_buffer_pnt);
	_unk_7 = *_i; free(_i);
	_b = _get_byte(&dat_buffer_pnt);
	_unk_8 = *_b; free(_b);
	_b = _get_byte(&dat_buffer_pnt);
	_unk_9 = *_b; free(_b);
	//*모르는 값 7 / 8 / 9

	_s = _get_str(&dat_buffer_pnt);
	_Drive_Type = GetUniStr(_s, code); free(_s);
	//*드라이브 타입(?)

	_s = _get_str(&dat_buffer_pnt);
	_unk_10 = GetUniStr(_s, code); free(_s);
	//*모르는 값 10

	if (_Version_Int >= 0x64) {
		_Attr_list_1 = (DAT_ATTR_ELEM_64*)malloc(sizeof(DAT_ATTR_ELEM_64) * (_Attr_Count + 1));
		memset(_Attr_list_1, 0, (sizeof(DAT_ATTR_ELEM_64) * (_Attr_Count + 1)));
		for (unsigned int i = 0; i < (_Attr_Count + 1);i++) {
			for (unsigned int j = 0; j < 8; j++) {
				_i = _get_int(&dat_buffer_pnt);
				_Attr_list_1[i]._int_list[j] = *_i; free(_i);
			}
		}
	}
	else {
		_Attr_list_2 = (DAT_ATTR_ELEM_NOT_64*)malloc(sizeof(DAT_ATTR_ELEM_NOT_64) * (_Attr_Count + 1));
		memset(_Attr_list_2, 0, (sizeof(DAT_ATTR_ELEM_NOT_64) * (_Attr_Count + 1)));
		for (unsigned int i = 0; i < (_Attr_Count + 1); i++) {
			for (unsigned int j = 0; j < 4; j++) {
				_i = _get_int(&dat_buffer_pnt);
				_Attr_list_2[i]._int_list[j] = *_i; free(_i);
			}
		}
	}
	//*속성(?) 수만큼 할당하고 읽어들이기, 버전을 구분해야 함
	//*사실 +1만큼 더 해서 읽어들여야 하니까 할당은 반드시 해야 한다

	_b = _get_byte(&dat_buffer_pnt);
	_unk_11 = *_b; free(_b);
	//*모르는 값 11

	if (_Version_Int >= 0x65) {
		_b = _get_byte(&dat_buffer_pnt);
		_unk_12 = *_b; free(_b);
	}
	//*버전에 따라 모르는 값 12

	if (_Version_Int >= 0x66) {
		_i = _get_int(&dat_buffer_pnt);
		_unk_13_count = *_i; free(_i);
		if (_unk_13_count != 0) {
			_unk_13_list = (DAT_ATTR_ELEM_66*)malloc (sizeof(DAT_ATTR_ELEM_66) * _unk_13_count);
			memset(_unk_13_list, 0, (sizeof(DAT_ATTR_ELEM_66) * _unk_13_count));
			for (unsigned int i = 0; i < _unk_13_count;i++) {
				char* _ss = _get_str(&dat_buffer_pnt);
				_unk_13_list[i]._str = GetUniStr(_ss, code);
				free(_ss);
			}
		}
	}
	//*버전에 따라 모르는 값 13(리스트)
	//*앞쪽 값에 따라 읽어들일 수도 있고 아닐 수도 있다

	if (_Version_Int >= 0x66) {
		_b = _get_byte(&dat_buffer_pnt);
		_unk_14 = *_b; free(_b);
	}
	//*버전에 따라 모르는 값 14

	if (_Version_Int >= 0x68) {
		_b = _get_byte(&dat_buffer_pnt);
		_unk_15 = *_b; free(_b);
	}
	//*버전에 따라 모르는 값 15

	if (_Version_Int >= 0x69) {
		_b = _get_byte(&dat_buffer_pnt);
		_unk_16 = *_b; free(_b);
	}
	//*버전에 따라 모르는 값 16

	if (_Version_Int >= 0x6A) {
		_i = _get_int(&dat_buffer_pnt);
		_unk_17_count = *_i; free(_i);
		if (_unk_17_count != 0) {
			_unk_17_list = (wchar_t**)malloc(sizeof(wchar_t*) * _unk_17_count);
			memset(_unk_17_list, 0, (sizeof(wchar_t*) * _unk_17_count));
			for (unsigned int i = 0; i < _unk_17_count; i++) {
				_s = _get_str(&dat_buffer_pnt);
				_unk_17_list[i] = GetUniStr(_s, code); free(_s);
			}
		}
	}
	//*버전에 따라 모르는 값 17(리스트)

	if (_Version_Int >= 0x6B) {
		_s = _get_str(&dat_buffer_pnt);
		_unk_18 = GetUniStr(_s, code); free(_s);
	}
	//*버전에 따라 모르는 값 18

	if (_Version_Int >= 0x6C) {
		for (unsigned int i = 0; i < 2; i++) {
			_s = _get_str(&dat_buffer_pnt);
			_unk_19[i] = GetUniStr(_s, code); free(_s);
		}
	}
	//*버전에 따라 모르는 값 19(리스트)

	wprintf(TEXT("Script Analysis Complete!\n"));
	free(dat_buffer);
	_setmode(_fileno(stdout), _O_TEXT);
	return true;
}
//*install.dat 파일을 불러와 분석하는 함수


void INSTALL_DAT_SCRIPT::Decompile_To_Code(wchar_t* TxtFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Decompiling to Code: Dat File: To %s\n"), TxtFile);

	HANDLE hWrite = CreateFileW(TxtFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//*핸들 생성

	UINT16 BOM = 0xFEFF;
	WriteFile(hWrite, &BOM, 2, NULL, NULL);
	//*유니코드 형식 추출 준비 (BOM)

	memset(_txt_buf, 0, sizeof(wchar_t) * TXT_BUFF_LEN);
	//*맨 처음은 반드시 0으로 초기화함

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s_%s[%08X]%s%s"), _Signature, _Version, _unk_1, _enter_raw_, separation_line);
	//*시그니처 및 버전 및 모르는 값 1 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Game_Title]::%s%s%s%s"), text_prefix, _Game_Title, text_prefix, _enter_raw_);
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[__UNK_2__]::%s%s%s%s"), text_prefix, _unk_2, text_prefix, _enter_raw_);
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Compiled_Dir]::%s%s%s%s"), text_prefix, _Compile_Dir, text_prefix, _enter_raw_);
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[__UNK_3__]::%s%s%s%s"), text_prefix, _unk_3, text_prefix, _enter_raw_);
	//*문자열 4개 순서대로 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[__UNK_5_%02X_%02X__]::%s%s%s%s"), 
		_unk_4, _unk_6, text_prefix, _unk_5, text_prefix, _enter_raw_);
	//*모르는 값 4 / 5 / 6 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Execute_File]::%s%s%s%s"), text_prefix, _Execute_File_Name, text_prefix, _enter_raw_);
	//*실행파일명 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[__UNK_5_%02X_%02X__]::[0x%08X]%s"),
		_unk_8, _unk_9, _unk_7, _enter_raw_);
	//*모르는 값 7 / 8 / 9 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Drive_Type]::%s%s%s%s%s%s"),
		text_prefix, _Drive_Type, text_prefix, _unk_10, text_prefix, _enter_raw_);
	WriteFile(hWrite, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//*드라이브 타입 / 모르는 값 10 기록
	//*1차 기록

	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Install_Attr:]%s"), _enter_raw_);
	for (unsigned int i = 0; i < (_Attr_Count + 1); i++) {
		if (_Version_Int >= 0x64) {
			swprintf(_txt_buf + wcslen(_txt_buf), 
				TEXT("  [Attr_Data]::[0x%08X, 0x%08X]:[0x%08X, 0x%08X]:[0x%08X, 0x%08X]:[0x%08X, 0x%08X]%s"),
				_Attr_list_1->_int_list[0], _Attr_list_1->_int_list[1], _Attr_list_1->_int_list[2], _Attr_list_1->_int_list[3],
				_Attr_list_1->_int_list[4], _Attr_list_1->_int_list[5], _Attr_list_1->_int_list[6], _Attr_list_1->_int_list[7],
				_enter_raw_);
		}
		else {
			swprintf(_txt_buf + wcslen(_txt_buf),
				TEXT("  [Attr_Data]::[0x%08X]:[0x%08X]:[0x%08X]:[0x%08X]%s"),
				_Attr_list_2->_int_list[0], _Attr_list_2->_int_list[1], _Attr_list_2->_int_list[2], _Attr_list_2->_int_list[3],
				_enter_raw_);
		}
	}
	//*루프 돌아가며 속성 기록 (버전 구분)

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[__UNK_VAL_1__]::[%02X_%02X]%s"), _unk_11, _unk_12, _enter_raw_);
	WriteFile(hWrite, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//*모르는 값 11, 12
	//*2차 기록

	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[__UNK_13__]%s"), _enter_raw_);
	for (unsigned int i = 0; i < _unk_13_count; i++) {
		swprintf(_txt_buf + wcslen(_txt_buf),
			TEXT("  [__Data]::%s%s%s%s"),
			text_prefix, _unk_13_list[i]._str, text_prefix,
			_enter_raw_);
	}
	//*모르는 값 13(리스트)

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[__UNK_VAL_2__]::[%02X_%02X_%02X]%s"), _unk_14, _unk_15, _unk_16, _enter_raw_);
	WriteFile(hWrite, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//*모르는 값 14 / 15 / 16
	//*3차 기록

	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[__UNK_17__]%s"), _enter_raw_);
	for (unsigned int i = 0; i < _unk_17_count; i++) {
		swprintf(_txt_buf + wcslen(_txt_buf),
			TEXT("  [__Data]::%s%s%s%s"), text_prefix, _unk_17_list[i], text_prefix, _enter_raw_);
	}
	//*모르는 값 17(리스트)

	swprintf(_txt_buf + wcslen(_txt_buf),
		TEXT("[__UNK_18__]::%s%s%s%s"), text_prefix, _unk_18, text_prefix, _enter_raw_);
	//*모르는 값 18

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[__UNK_19__]%s"), _enter_raw_);
	for (unsigned int i = 0; i < 2; i++) {
		swprintf(_txt_buf + wcslen(_txt_buf),
			TEXT("  [__Data]::%s%s%s%s"), text_prefix, _unk_19[i], text_prefix, _enter_raw_);
	}
	WriteFile(hWrite, _txt_buf, (sizeof(wchar_t)* wcslen(_txt_buf)), NULL, NULL);
	//*모르는 값 19(리스트)
	//*마지막 기록

	CloseHandle(hWrite);
	//*핸들 해제

	wprintf(TEXT("Decompiling Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	//*정리
}
//*디컴파일해서 기록하는 함수


void INSTALL_DAT_SCRIPT::Make_To_Script(wchar_t* newDatFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Writing: Dat File: %s\n"), newDatFile);

	HANDLE hNewDat = CreateFileW(newDatFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//*핸들 생성

	char* _s;
	//*기록용 변수

	_s = GetAnsiStr(_Signature, code);
	_write_data(hNewDat, (unsigned char*)_s, strlen(_s)); free(_s);
	//*시그니처 기록

	_s = GetAnsiStr(_Version, code);
	_write_data(hNewDat, (unsigned char*)_s, strlen(_s)); free(_s);
	//*버전 기록

	_write_int(hNewDat, _unk_1);
	//*_unk_1 기록

	_s = GetAnsiStr(_Game_Title, code);
	_write_str(hNewDat, _s); free(_s);
	_s = GetAnsiStr(_unk_2, code);
	_write_str(hNewDat, _s); free(_s);
	_s = GetAnsiStr(_Compile_Dir, code);
	_write_str(hNewDat, _s); free(_s);
	_s = GetAnsiStr(_unk_3, code);
	_write_str(hNewDat, _s); free(_s);
	//*문자열 4개 기록

	_write_byte(hNewDat, _unk_4);
	_s = GetAnsiStr(_unk_5, code);
	_write_str(hNewDat, _s); free(_s);
	_write_byte(hNewDat, _unk_6);
	//*_unk_4, 5, 6 기록

	_write_int(hNewDat, _Attr_Count);
	//*속성 개수 기록

	_s = GetAnsiStr(_Execute_File_Name, code);
	_write_str(hNewDat, _s); free(_s);
	//*실행파일명 기록

	_write_int(hNewDat, _unk_7);
	_write_byte(hNewDat, _unk_8);
	_write_byte(hNewDat, _unk_9);
	//*_unk_7, 8, 9 기록

	_s = GetAnsiStr(_Drive_Type, code);
	_write_str(hNewDat, _s); free(_s);
	//*드라이브 타입(?) 기록

	_s = GetAnsiStr(_unk_10, code);
	_write_str(hNewDat, _s); free(_s);
	//*_unk_10 기록

	for (unsigned int i = 0; i < (_Attr_Count + 1); i++) {
		if (_Version_Int >= 0x64) {
			for (unsigned int j = 0; j < 8;j++) { _write_int(hNewDat, _Attr_list_1[i]._int_list[j]); }
		}
		else {
			for (unsigned int j = 0; j < 4; j++) { _write_int(hNewDat, _Attr_list_2[i]._int_list[j]); }
		}
	}
	//*(속성 수 + 1)만큼 돌아가면서 기록, 버전 구분

	_write_byte(hNewDat, _unk_11);
	//*_unk_11 기록

	if (_Version_Int >= 0x65) { _write_byte(hNewDat, _unk_12); }
	//*버전에 따라 _unk_12 기록

	if (_Version_Int >= 0x66) {
		_write_int(hNewDat, _unk_13_count);
		for (unsigned int i = 0; i < _unk_13_count; i++) {
			char* _n_c = GetAnsiStr(_unk_13_list[i]._str, code);
			_write_str(hNewDat, _n_c);
			free(_n_c);
		}
	}
	//*버전에 따라 _unk_13 기록
	//*앞값을 기록하고 앞값이 0이 아니면 다음값을 기록

	if (_Version_Int >= 0x67) { _write_byte(hNewDat, _unk_14); }
	if (_Version_Int >= 0x68) { _write_byte(hNewDat, _unk_15); }
	if (_Version_Int >= 0x69) { _write_byte(hNewDat, _unk_16); }
	//*버전에 따라 _unk_14, 15, 16 기록

	if (_Version_Int >= 0x6A) {
		_write_int(hNewDat, _unk_17_count);
		for (unsigned int i = 0; i < _unk_17_count; i++) {
			_s = GetAnsiStr(_unk_17_list[i], code);
			_write_str(hNewDat, _s); free(_s);
		}
	}
	//*버전에 따라 _unk_17 기록

	if (_Version_Int >= 0x6B) {
		_s = GetAnsiStr(_unk_18, code);
		_write_str(hNewDat, _s); free(_s);
	}
	//*버전에 따라 _unk_18 기록

	if (_Version_Int >= 0x6C) {
		for (unsigned int i = 0; i < 2; i++) {
			_s = GetAnsiStr(_unk_19[i], code);
			_write_str(hNewDat, _s); free(_s);
		}
	}
	//*버전에 따라 _unk_19 기록

	wprintf(TEXT("Writing Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	//*정리
}
//*dat 파일로 기록하는 함수


void INSTALL_DAT_SCRIPT::Extract_Text_Data(wchar_t* DstTxtFile)
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

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Game_Title]::[%s]::%s%s%s%s"),
		text_data_prefix, text_prefix, _Game_Title, text_prefix, _enter_raw_);
	//*게임 타이틀 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Compile_Dir]::[%s]::%s%s%s%s"),
		text_data_prefix, text_prefix, _Compile_Dir, text_prefix, _enter_raw_);
	//*컴파일 디렉토리명 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Execute_File]::[%s]::%s%s%s%s"),
		text_data_prefix, text_prefix, _Execute_File_Name, text_prefix, _enter_raw_);
	WriteFile(hTxt, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//*실행파일명 기록 및 정리

	CloseHandle(hTxt);
	//*핸들 해제

	wprintf(TEXT("Extracting Text Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	//*정리
}
//*텍스트 추출 함수


void INSTALL_DAT_SCRIPT::Replace_Text_Data(wchar_t* SrcTxtFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Replacing: Text File: %s\n"), SrcTxtFile);

	HANDLE hTxt = CreateFileW(SrcTxtFile, GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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

	for (unsigned int i = 0; i < text_count; i++) {
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
	if (_Game_Title != NULL) { free(_Game_Title); }
	_Game_Title = _tmp;
	//*게임 타이틀 세팅

	_tmp = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(text_list[1]) + 1));
	memcpy(_tmp, text_list[1], (sizeof(wchar_t) * wcslen(text_list[1])));
	_tmp[wcslen(text_list[1])] = 0;
	if (_Compile_Dir != NULL) { free(_Compile_Dir); }
	_Compile_Dir = _tmp;
	//*컴파일 디렉토리명 세팅

	_tmp = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(text_list[2]) + 1));
	memcpy(_tmp, text_list[2], (sizeof(wchar_t) * wcslen(text_list[2])));
	_tmp[wcslen(text_list[2])] = 0;
	if (_Execute_File_Name != NULL) { free(_Execute_File_Name); }
	_Execute_File_Name = _tmp;
	//*실행파일명 세팅

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


void INSTALL_DAT_SCRIPT::Convert_Codepage_J2K()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Changing Text Language Code(Jap -> Kor)...\n"));

	JPHan_2_KRHan(_Signature);
	JPHan_2_KRHan(_Version);
	JPHan_2_KRHan(_Game_Title);
	JPHan_2_KRHan(_unk_2);
	JPHan_2_KRHan(_Compile_Dir);
	JPHan_2_KRHan(_unk_3);
	JPHan_2_KRHan(_unk_5);
	JPHan_2_KRHan(_Execute_File_Name);
	JPHan_2_KRHan(_Drive_Type);
	JPHan_2_KRHan(_unk_10);
	for (unsigned int i = 0; i < _unk_17_count; i++) { JPHan_2_KRHan(_unk_17_list[i]); }
	JPHan_2_KRHan(_unk_18);
	for (unsigned int i = 0; i < 2; i++) { JPHan_2_KRHan(_unk_19[i]); }
	//*wchar_t*로 된 것들을 모두 바꾼다

	wprintf(TEXT("Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
}
//*텍스트 언어코드를 일본어에서 한글로 바꾸는 함수
//---------------------------- 스크립트 클래스 관련 함수 ----------------------------//