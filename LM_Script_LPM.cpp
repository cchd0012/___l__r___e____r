#include "LM_Script.h"
#include "LM_Functions.h"


#pragma warning (disable:6031)
#pragma warning (disable:4996)

//*** lpm 스크립트 분석 및 적용 함수들을 서술하는 파일


const wchar_t* _shift_jis_str = TEXT("Shift_JIS");
const wchar_t* _euc_kr_str = TEXT("EUC-KR");
//*J2K 실행시 xml에서 바꿔야 할 문자열



//---------------------------- 스크립트 클래스 관련 함수 ----------------------------//
bool LPM_SCRIPT::Script_Analysis(wchar_t* LpmFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Analysis: LPM File: %s\n"), LpmFile);

	HANDLE hLpm = CreateFileW(LpmFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hLpm == INVALID_HANDLE_VALUE) {
		wprintf(TEXT("%s: There's not LPM File\n"), LpmFile);
		_setmode(_fileno(stdout), _O_TEXT);
		return false;
	}
	char _tmp[13]; _tmp[12] = 0;
	ReadFile(hLpm, _tmp, 12, NULL, NULL);
	CloseHandle(hLpm);
	//*먼저 12바이트 읽어들이기 (파일 경로 언어코드도 고려할 필요 있음)

	if (strcmp(_tmp, "LivePrevMenu") == 0) { return _analysis_as_binary(LpmFile, code); }
	else { return _analysis_as_xml(LpmFile, code); }
	//*맨 앞 12바이트를 읽어들여 그 값이 "LivePrevMenu"면 이진파일, 아니면 xml로 분석한다
}
//*lpm 파일을 불러와 분석하는 함수


bool LPM_SCRIPT::_analysis_as_xml(wchar_t* LpmFile, unsigned int code)
{
	wprintf(TEXT("Analysis As XML...\n"));
	_is_xml_script = true;
	//*xml이면 분석을 할 필요가 없음
	//*나중에 바꿀 때를 대비해서 버퍼를 유니코드로 바꿔 들고 있으면 될 듯 하다

	HANDLE hLpm = CreateFileW(LpmFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	unsigned int lpm_size = GetFileSize(hLpm, NULL);
	unsigned char* lpm_buffer = (unsigned char*)malloc(lpm_size + 1);
	ReadFile(hLpm, lpm_buffer, lpm_size, NULL, NULL);
	lpm_buffer[lpm_size] = 0;
	unsigned char* lpm_buffer_pnt = lpm_buffer;
	CloseHandle(hLpm);
	//*lpm 파일 읽어들이기 (파일 경로 언어코드도 고려할 필요 있음)

	UINT16 BOM = 0xFEFF;
	if ((*(wchar_t*)lpm_buffer_pnt) == BOM) {
		_Lpm_Buffer = (wchar_t*)malloc(lpm_size);
		memset(_Lpm_Buffer, 0, lpm_size);
		memcpy(_Lpm_Buffer, lpm_buffer + sizeof(wchar_t), (lpm_size - sizeof(wchar_t)));
		//*맨 앞이 유니코드면 앞부분만 떼고 붙여주기
	}
	else {
		_Lpm_Buffer = GetUniStr((char*)lpm_buffer_pnt, code);
		//*그렇지 않다면 유니코드로 바꾸고 붙여주기
	}
	free(lpm_buffer);
	//*유니코드화된 xml(텍스트 파일)을 소유하도록 하기

	wprintf(TEXT("Script Analysis Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	return true;
}
//*xml로 삼아 분석하는 함수


bool LPM_SCRIPT::_analysis_as_binary(wchar_t* LpmFile, unsigned int code)
{
	wprintf(TEXT("Analysis As Binary...\n"));
	_is_xml_script = false;

	HANDLE hLpm = CreateFileW(LpmFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	unsigned int lpm_size = GetFileSize(hLpm, NULL);
	unsigned char* lpm_buffer = (unsigned char*)malloc(lpm_size);
	ReadFile(hLpm, lpm_buffer, lpm_size, NULL, NULL);
	unsigned char* lpm_buffer_pnt = lpm_buffer;
	CloseHandle(hLpm);
	//*lpm 파일 읽어들이기 (파일 경로 언어코드도 고려할 필요 있음)

	_PrevMenu = new LPM_PREVMENU();
	_PrevMenu->_read_from_buffer(&lpm_buffer_pnt, code);
	//*버퍼로부터 분석

	wprintf(TEXT("Script Analysis Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	return true;
}
//*이진파일로 삼아 분석하는 함수


void LPM_SCRIPT::Decompile_To_Code(wchar_t* TxtFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Decompiling to Code: Lpm File: To %s\n"), TxtFile);

	HANDLE hWrite = CreateFileW(TxtFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//*핸들 생성

	UINT16 BOM = 0xFEFF;
	WriteFile(hWrite, &BOM, 2, NULL, NULL);
	//*유니코드 형식 추출 준비 (BOM)

	if (_is_xml_script) {
		WriteFile(hWrite, _Lpm_Buffer, sizeof(wchar_t) * wcslen(_Lpm_Buffer), NULL, NULL);
	}
	//*xml 파일일 경우 그냥 파일을 확장자만 바꿔 기록한다

	else {
		_PrevMenu->_decompile_code(hWrite, code);
		//*디컴파일 함수 호출

		CloseHandle(hWrite);
		//*핸들 해제
	}
	//*그렇지 않다면 전부 처리함

	wprintf(TEXT("Decompiling Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
}
//*디컴파일해서 기록하는 함수


void LPM_SCRIPT::Make_To_Script(wchar_t* newLpmFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Writing: Lpm File: %s\n"), newLpmFile);

	HANDLE hNewLpm = CreateFileW(newLpmFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//*핸들 생성

	wchar_t* _pt;
	char* _write_buffer = NULL;
	if (_is_xml_script)
	{
		if (code == LM_CP_JAP) {
_DEFAULT:
			if ((_pt = wcsistr(_Lpm_Buffer, _euc_kr_str)) != NULL) {
				wchar_t* _cpy = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(_euc_kr_str) + 1));
				memcpy(_cpy, _pt, (sizeof(wchar_t) * wcslen(_euc_kr_str)));
				_cpy[wcslen(_euc_kr_str)] = 0;
				wchar_t* _n_Lpm_Buffer = Replace_Text(_Lpm_Buffer, _cpy, _shift_jis_str);
				_write_buffer = GetAnsiStr(_n_Lpm_Buffer, code);
				free(_n_Lpm_Buffer);
			}
			else {
				_write_buffer = GetAnsiStr(_Lpm_Buffer, code);
			}
			//일본어 언어코드 : "EUC-KR" -> "Shift_JIS", 없으면 그냥 기록
		}
		else if (code == LM_CP_KOR) {
			if ((_pt = wcsistr(_Lpm_Buffer, _shift_jis_str)) != NULL) {
				wchar_t* _cpy = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(_shift_jis_str) + 1));
				memcpy(_cpy, _pt, (sizeof(wchar_t) * wcslen(_shift_jis_str)));
				_cpy[wcslen(_shift_jis_str)] = 0;
				wchar_t* _n_Lpm_Buffer = Replace_Text(_Lpm_Buffer, _cpy, _euc_kr_str);
				_write_buffer = GetAnsiStr(_n_Lpm_Buffer, code);
				free(_n_Lpm_Buffer);
			}
			else {
				_write_buffer = GetAnsiStr(_Lpm_Buffer, code);
			}
			//한국어 언어코드 : "Shift_JIS" -> "EUC-KR", 없으면 그냥 기록
		}
		else { goto _DEFAULT; }
		//*기록시에는 코드에 따라 "Shift_JIS" / "cp932" 혹은 "cp949"로 바꿔야 한다 (대소문자 구별 없음)

		_write_data(hNewLpm, (unsigned char*)_write_buffer, strlen(_write_buffer));
		free(_write_buffer);
		//*마지막으로 바꾼 텍스트를 기록해줌
	}
	//*xml 파일(텍스트)일 시

	else {
		_PrevMenu->_write_as_lpm(hNewLpm, code);
	}
	//*이진파일일 시 : PrevMenu 클래스에서 수행

	CloseHandle(hNewLpm);
	//*핸들 해제

	wprintf(TEXT("Writing Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	//*정리
}
//*lpm 파일로 기록하는 함수


void LPM_SCRIPT::Extract_Text_Data(wchar_t* DstTxtFile)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Not Need To Extract...\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	//*굳이 텍스트를 추출할 필요가 없음
}
//*텍스트 추출 함수


void LPM_SCRIPT::Replace_Text_Data(wchar_t* SrcTxtFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Not Need To Replace...\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	//*굳이 텍스트를 대치할 필요가 없음
}
//*텍스트 교체 함수


void LPM_SCRIPT::Convert_Codepage_J2K()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Changing Text Language Code(Jap -> Kor)...\n"));

	if (_is_xml_script) { 
		JPHan_2_KRHan(_Lpm_Buffer); 
	}
	//*xml 파일일 시 파일버퍼만 바꿔주면 됨

	else { _PrevMenu->_change_code_J2K(); }
	//*이진파일이면 PrevMenu를 싹 돌면서 바꿔준다

	wprintf(TEXT("Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
}
//*텍스트 언어코드를 일본어에서 한글로 바꾸는 함수
//---------------------------- 스크립트 클래스 관련 함수 ----------------------------//




//---------------------------- PrevMenu 클래스 관련 함수 ----------------------------//
void LPM_PREVMENU::_read_from_buffer(unsigned char** _lpm_buffer_pnt, unsigned int code)
{
	unsigned char* lpm_buffer_pnt = (*_lpm_buffer_pnt);
	char* _s;
	unsigned int* _int;

	_s = (char*)_get_data(&lpm_buffer_pnt, 12);
	_Signature = GetUniStr(_s, code);
	free(_s);
	//*시그니처 문자열 읽어들이기

	_s = (char*)_get_data(&lpm_buffer_pnt, 3);
	_Version = GetUniStr(_s, code);
	_Button->_Version = strtol(_s, NULL, 10);
	free(_s);
	//*버전 읽어들인 후 숫자 변환 -> 버튼 클래스에 넣어주기

	_int = _get_int(&lpm_buffer_pnt);
	_Width = *_int; free(_int);
	_int = _get_int(&lpm_buffer_pnt);
	_Height = *_int; free(_int);
	//*너비 / 높이 읽어들이기

	_Button->_read_from_buffer(&lpm_buffer_pnt, code);
	//*나머지는 버튼 클래스에서 읽어들이기

	(*_lpm_buffer_pnt) = lpm_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수


void LPM_PREVMENU::_decompile_code(HANDLE hWrite, unsigned int code)
{
	memset(_txt_buf, 0, sizeof(wchar_t) * TXT_BUFF_LEN);
	//*맨 처음은 반드시 0으로 초기화함

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s_%s%s%s"), _Signature, _Version, _enter_raw_, separation_line);
	//*시그니처 및 버전 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Resolution]::[%03d*%03d]%s%s"), _Width, _Height, _enter_raw_, separation_line);
	WriteFile(hWrite, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//*너비 및 높이 기록
	//*1차 기록

	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	_Button->_decompile_code(hWrite, code);
	//*버튼에서 추가 기록

	WriteFile(hWrite, separation_line, (sizeof(wchar_t) * wcslen(separation_line)), NULL, NULL);
	//*개행선 추가
}
//*디컴파일 함수


void LPM_PREVMENU::_write_as_lpm(HANDLE hNewLpm, unsigned int code)
{
	char* _s;

	_s = GetAnsiStr(_Signature, code);
	_write_data(hNewLpm, (unsigned char*)_s, strlen(_s));
	free(_s);
	//*시그니처 기록
	
	_s = GetAnsiStr(_Version, code);
	_write_data(hNewLpm, (unsigned char*)_s, strlen(_s));
	free(_s);
	//*버전 기록

	_write_int(hNewLpm, _Width);
	_write_int(hNewLpm, _Height);
	//*너비 / 높이 기록

	_Button->_write_as_lpm(hNewLpm, code);
	//*Button 클래스에서 마저 기록
}
//*lpm 파일로 기록하는 함수


void LPM_PREVMENU::_change_code_J2K()
{
	_Button->_change_code_J2K();
	//*버튼에서 다 바꾸게 한다
}
//*텍스트 코드 변경 함수
//---------------------------- PrevMenu 클래스 관련 함수 ----------------------------//




//---------------------------- Button 클래스 관련 함수 ----------------------------//
void LPM_BUTTON::_read_from_buffer(unsigned char** _lpm_buffer_pnt, unsigned int code)
{
	unsigned char* lpm_buffer_pnt = (*_lpm_buffer_pnt);
	unsigned int* _int;

	_int = _get_int(&lpm_buffer_pnt);
	_item_count = *_int; free(_int);
	_item_list = (LPM_ITEM**)malloc(sizeof(LPM_ITEM*)*_item_count);
	memset(_item_list, 0, (sizeof(LPM_ITEM*) * _item_count));
	//*아이템 수 읽어들이고 할당하기

	for (unsigned int i = 0; i < _item_count;i++) {
		_item_list[i] = new LPM_ITEM(_Version);
		_item_list[i]->_read_from_buffer(&lpm_buffer_pnt, code);
	}
	//*아이템들 읽어들이기

	(*_lpm_buffer_pnt) = lpm_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수


void LPM_BUTTON::_decompile_code(HANDLE hWrite, unsigned int code)
{
	for (unsigned int i = 0; i < _item_count; i++) {

		memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[Item_%02d]%s"), i, _enter_raw_);
		//*헤더 정보 기록

		_item_list[i]->_decompile_code(hWrite, code);
		WriteFile(hWrite, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
		//*아이템으로 기록 후 파일로 출력하기

	}
}
//*디컴파일 함수

void LPM_BUTTON::_write_as_lpm(HANDLE hNewLpm, unsigned int code)
{
	_write_int(hNewLpm, _item_count);
	//*총 아이템 수 기록

	for (unsigned int i = 0; i < _item_count;i++) {
		_item_list[i]->_write_as_lpm(hNewLpm, code);
	}
	//*리스트를 돌아가며 아이템 기록
}
//*lpm 파일로 기록하는 함수


void LPM_BUTTON::_change_code_J2K()
{
	for (unsigned int i = 0; i < _item_count; i++) {
		_item_list[i]->_change_code_J2K();
	}
	//*아이템 요소에서 바꾸게 한다
}
//*텍스트 코드 변경 함수
//---------------------------- Button 클래스 관련 함수 ----------------------------//




//---------------------------- item 클래스 관련 함수 ----------------------------//
void LPM_ITEM::_read_from_buffer(unsigned char** _lpm_buffer_pnt, unsigned int code)
{
	unsigned char* lpm_buffer_pnt = (*_lpm_buffer_pnt);
	char* _s;
	unsigned char* _b;
	unsigned int* _int;

	_int = _get_int(&lpm_buffer_pnt);
	_Left = *_int; free(_int);
	_int = _get_int(&lpm_buffer_pnt);
	_Top = *_int; free(_int);
	//*x좌표 / y좌표 읽어들이기

	_s = _get_str(&lpm_buffer_pnt);
	_Src = GetUniStr(_s, code); free(_s);
	//*src 읽어들이기

	_b = _get_byte(&lpm_buffer_pnt);
	_unk_2 = *_b; free(_b);
	//*unk_2 읽어들이기

	_s = _get_str(&lpm_buffer_pnt);
	_Name = GetUniStr(_s, code); free(_s);
	//*name 읽어들이기

	_s = _get_str(&lpm_buffer_pnt);
	_Src_Selected = GetUniStr(_s, code); free(_s);
	//*src_selected 읽어들이기

	_s = _get_str(&lpm_buffer_pnt);
	_unk_3 = GetUniStr(_s, code); free(_s);
	_s = _get_str(&lpm_buffer_pnt);
	_unk_4 = GetUniStr(_s, code); free(_s);
	//*unk_3 ~ 4 읽어들이기

	if (_Version > 100) {
		_s = _get_str(&lpm_buffer_pnt);
		_unk_5 = GetUniStr(_s, code); free(_s);
	}
	//*버전에 따라 unk_5 읽어들이기

	if (_Version > 102) {
		for (unsigned int i = 0; i < 2;i++) {
			_s = _get_str(&lpm_buffer_pnt);
			_unk_6[i] = GetUniStr(_s, code); free(_s);
		}
	}
	//*버전에 따라 unk_6 읽어들이기

	_s = _get_str(&lpm_buffer_pnt);
	_unk_7 = GetUniStr(_s, code); free(_s);
	_s = _get_str(&lpm_buffer_pnt);
	_unk_8 = GetUniStr(_s, code); free(_s);
	_s = _get_str(&lpm_buffer_pnt);
	_unk_9 = GetUniStr(_s, code); free(_s);
	//*unk 7, 8, 9 읽어들이기

	if (_Version > 101) {
		for (unsigned int i = 0; i < 2; i++) {
			_s = _get_str(&lpm_buffer_pnt);
			_unk_10[i] = GetUniStr(_s, code); free(_s);
		}
	}
	//*버전에 따라 unk_10 읽어들이기

	_int = _get_int(&lpm_buffer_pnt);
	_unk_11 = *_int; free(_int);
	_int = _get_int(&lpm_buffer_pnt);
	_unk_12 = *_int; free(_int);
	//*unk_11, 12 읽어들이기

	_s = _get_str(&lpm_buffer_pnt);
	_unk_13 = GetUniStr(_s, code); free(_s);
	//*unk_13 읽어들이기

	if (_Version > 103) {
		for (unsigned int i = 0; i < 5; i++) {
			_s = _get_str(&lpm_buffer_pnt);
			_unk_14._str[i] = GetUniStr(_s, code); free(_s);
		}
		_int = _get_int(&lpm_buffer_pnt);
		_unk_14._int = *_int; free(_int);
	}
	//*버전에 따라 unk_14 읽어들이기

	if (_Version > 104) {
		_s = _get_str(&lpm_buffer_pnt);
		_unk_15 = GetUniStr(_s, code); free(_s);
	}
	//*버전에 따라 unk_15 읽어들이기

	if (_Version > 105) {
		_s = _get_str(&lpm_buffer_pnt);
		_unk_16 = GetUniStr(_s, code); free(_s);
	}
	//*버전에 따라 unk_16 읽어들이기

	(*_lpm_buffer_pnt) = lpm_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수


void LPM_ITEM::_decompile_code(HANDLE hWrite, unsigned int code)
{
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[(__LEFT__, __TOP__)]::(%d, %d)%s"), _indent_, _Left, _Top, _enter_raw_);
	//*너비 / 높이

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[__SOURCE_0x%02X__]::%s%s%s%s"), 
		_indent_, _unk_2, text_prefix, _Src, text_prefix, _enter_raw_);
	//*src, unk_2

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[__NAME__]::%s%s%s%s"),
		_indent_, text_prefix, _Name, text_prefix, _enter_raw_);
	//*name

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[__SRC_SELECTED__]::%s%s%s%s"),
		_indent_, text_prefix, _Src_Selected, text_prefix, _enter_raw_);
	//*src_selected

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[__UNK_TYPE_1__]::[%s][%s][%s][[%s][%s]]%s"),
		_indent_, _unk_3, _unk_4, _unk_5, _unk_6[0], _unk_6[1], _enter_raw_);
	//*unk_3, 4, 5, 6

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[__UNK_TYPE_2__]::[%s][%s][%s][[%s][%s]]%s"),
		_indent_, _unk_7, _unk_8, _unk_9, _unk_10[0], _unk_10[1], _enter_raw_);
	//*unk_7, 8, 9, 10

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[__UNK_VALUE__]::[0x%08X][0x%08X]%s"),
		_indent_, _unk_11, _unk_12, _enter_raw_);
	//*unk_11, 12

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[__UNK_STR__]::%s%s%s%s"),
		_indent_, text_prefix, _unk_13, text_prefix, _enter_raw_);
	//*unk_13

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[__UNK_TYPE_3_0x%08X__]::[[%s][%s][%s][%s][%s]]%s"),
		_indent_, _unk_14._int, _unk_14._str[0], _unk_14._str[1], _unk_14._str[2], _unk_14._str[3], _unk_14._str[4], _enter_raw_);
	//*unk_14

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[__UNK_STR__]::%s%s%s%s"),
		_indent_, text_prefix, _unk_15, text_prefix, _enter_raw_);
	//*unk_15

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[__UNK_STR__]::%s%s%s%s"),
		_indent_, text_prefix, _unk_16, text_prefix, _enter_raw_);
	//*unk_16

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _enter_raw_);
	//*개행 추가
}
//*디컴파일 함수

void LPM_ITEM::_write_as_lpm(HANDLE hNewLpm, unsigned int code)
{
	char* _s;

	_write_int(hNewLpm, _Left);
	_write_int(hNewLpm, _Top);
	//*left, top 기록

	_s = GetAnsiStr(_Src, code);
	_write_str(hNewLpm, _s); free(_s);
	//*src 기록

	_write_byte(hNewLpm, _unk_2);
	//*unk_2 기록

	_s = GetAnsiStr(_Name, code);
	_write_str(hNewLpm, _s); free(_s);
	_s = GetAnsiStr(_Src_Selected, code);
	_write_str(hNewLpm, _s); free(_s);
	//*name, src_selected 기록

	_s = GetAnsiStr(_unk_3, code);
	_write_str(hNewLpm, _s); free(_s);
	_s = GetAnsiStr(_unk_4, code);
	_write_str(hNewLpm, _s); free(_s);
	//*unk_3, 4 기록

	if (_Version > 100) {
		_s = GetAnsiStr(_unk_5, code);
		_write_str(hNewLpm, _s); free(_s);
	}
	//*버전에 따라 unk_5 기록

	if (_Version > 102) {
		for (unsigned int i = 0; i < 2;i++) {
			_s = GetAnsiStr(_unk_6[i], code);
			_write_str(hNewLpm, _s); free(_s);
		}
	}
	//*버전에 따라 unk_6 기록

	_s = GetAnsiStr(_unk_7, code);
	_write_str(hNewLpm, _s); free(_s);
	_s = GetAnsiStr(_unk_8, code);
	_write_str(hNewLpm, _s); free(_s);
	_s = GetAnsiStr(_unk_9, code);
	_write_str(hNewLpm, _s); free(_s);
	//*unk_7, 8, 9 기록

	if (_Version > 101) {
		for (unsigned int i = 0; i < 2; i++) {
			_s = GetAnsiStr(_unk_10[i], code);
			_write_str(hNewLpm, _s); free(_s);
		}
	}
	//*버전에 따라 unk_10 기록

	_write_int(hNewLpm, _unk_11);
	_write_int(hNewLpm, _unk_12);
	//*unk_11, 12 기록

	_s = GetAnsiStr(_unk_13, code);
	_write_str(hNewLpm, _s); free(_s);
	//*unk_13 기록

	if (_Version > 103) {
		for (unsigned int i = 0; i < 5; i++) {
			_s = GetAnsiStr(_unk_14._str[i], code);
			_write_str(hNewLpm, _s); free(_s);
		}
		_write_int(hNewLpm, _unk_14._int);
	}
	//*버전에 따라 unk_14 기록

	_s = GetAnsiStr(_unk_15, code);
	_write_str(hNewLpm, _s); free(_s);
	_s = GetAnsiStr(_unk_16, code);
	_write_str(hNewLpm, _s); free(_s);
	//*unk_15, 16 기록
}
//*lpm 파일로 기록하는 함수


void LPM_ITEM::_change_code_J2K()
{
	JPHan_2_KRHan(_Src);
	JPHan_2_KRHan(_Name);
	JPHan_2_KRHan(_Src_Selected);
	JPHan_2_KRHan(_unk_3);
	JPHan_2_KRHan(_unk_4);
	JPHan_2_KRHan(_unk_5);
	for (unsigned int i = 0; i < 2;i++) { JPHan_2_KRHan(_unk_6[i]); }
	JPHan_2_KRHan(_unk_7);
	JPHan_2_KRHan(_unk_8);
	JPHan_2_KRHan(_unk_9);
	for (unsigned int i = 0; i < 2; i++) { JPHan_2_KRHan(_unk_10[i]); }
	JPHan_2_KRHan(_unk_13);
	for (unsigned int i = 0; i < 5; i++) { JPHan_2_KRHan(_unk_14._str[i]); }
	JPHan_2_KRHan(_unk_15);
	JPHan_2_KRHan(_unk_16);
	//* wchar_t* 형식인 모든 데이터를 바꿔친다
}
//*텍스트 코드 변경 함수
//---------------------------- item 클래스 관련 함수 ----------------------------//