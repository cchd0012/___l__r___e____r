#include "LM_Text.h"
#include "LM_Script.h"
#include "LM_Functions.h"


#pragma warning (disable:6031)
#pragma warning (disable:4996)


//*** 텍스트 관련 함수들의 기능을 서술하는 파일


const char* _exe_xml_shift_jis_str = "<?xml version=\"1.0\" encoding=\"Shift_JIS\" ?>\r\n";
const char* _exe_xml_euc_kr_str = "<?xml version=\"1.0\" encoding=\"EUC-KR\" ?>\r\n";
const char* _exe_shift_jis_str_1 = "shift_jis";
const char* _exe_shift_jis_str_2 = "shift-jis";
const char* _exe_euc_kr_str_1 = "euc_kr";
const char* _exe_euc_kr_str_2 = "euc-kr";
//*J2K 실행시 exe 내부에서 바꿔야 할 문자열

const unsigned char _exe_search_bs_1[6] = { 0x8A, 0x40, 0x1A, 0x88, 0x45, 0xD7 };
const unsigned char _exe_change_bs_1[6] = { 0xC6, 0x45, 0xD7, 0x81, 0x90, 0x90 };
//*로직을 변경하기 위한 명령어 1

const unsigned char _exe_search_bs_2[8] = { 0x83, 0xC0, 0xE0, 0x3D, 0xBF, 0x00, 0x00, 0x00 };
const unsigned char _exe_change_bs_2[8] = { 0x83, 0xC0, 0xE0, 0x3D, 0x60, 0x00, 0x00, 0x00 };
//*로직을 변경하기 위한 명령어 2

const unsigned char _exe_search_bs_3_1[6] = { 0x01, 0x00, 0x00, 0x00, 0xA4, 0xFD };
const unsigned char _exe_change_bs_3_1[6] = { 0x01, 0x00, 0x00, 0x00, 0xA3, 0x00 };
const unsigned char _exe_search_bs_3_2[6] = { 0x83, 0xEA, 0x30, 0x83, 0xC2, 0x4F };
const unsigned char _exe_change_bs_3_2[6] = { 0x81, 0xC2, 0x80, 0x00, 0x00, 0x00 };
const unsigned char _exe_search_bs_3_3[6] = { 0x83, 0xEA, 0x41, 0x83, 0xC2, 0x81 };
const unsigned char _exe_change_bs_3_3[6] = { 0x81, 0xC2, 0x80, 0x00, 0x00, 0x00 };
//*로직을 변경하기 위한 명령어 3_1, 3_2, 3_3

const unsigned char _exe_search_bs_4[11] = { 0x80, 0x38, 0x81, 0x75, 0x25, 0x8B, 0x03, 0x80, 0x78, 0x01, 0x40 };
const unsigned char _exe_change_bs_4[11] = { 0x80, 0x38, 0xA1, 0x75, 0x25, 0x8B, 0x03, 0x80, 0x78, 0x01, 0xA1 };
const unsigned char _exe_search_bs_5[14] = { 0x80, 0x7C, 0x30, 0xFE, 0x81, 0x75, 0x49, 0x8B, 0x03, 0x80, 0x7C, 0x30, 0xFF, 0x40 };
const unsigned char _exe_change_bs_5[14] = { 0x80, 0x7C, 0x30, 0xFE, 0xA1, 0x75, 0x49, 0x8B, 0x03, 0x80, 0x7C, 0x30, 0xFF, 0xA1 };
//*로직을 변경하기 위한 명령어 4, 5

const char _exe_font_jap_part_name_ms[5] = "ＭＳ";
const char _exe_font_jap_name_07_soft_gothic[19] = "07やさしさゴシック";
const char _exe_font_kor_name_hy_taegothic[9] = "HY태고딕";
//*J2K 실행시 exe 내부에서 바꿔야 할 폰트 문자열 1

const unsigned char _exe_font_jap_part_name_ms_2[16] = 
	{ 0x0F, 0x82, 0x6C, 0x82, 0x72, 0x20, 0x82, 0x6F, 0x83, 0x53, 0x83, 0x56, 0x83, 0x62, 0x83, 0x4E };
const unsigned char _exe_font_kor_name_08_seoul_namsan_eb[16] = 
	{ 0x0F, 0x30, 0x38, 0xBC, 0xAD, 0xBF, 0xEF, 0xB3, 0xB2, 0xBB, 0xEA, 0xC3, 0xBC, 0x20, 0x45, 0x42 };
//*J2K 실행시 exe 내부에서 바꿔야 할 폰트 문자열 2 (08서울남산체 EB)


void Change_Name_Codepage_J2K (wchar_t *DataFolder);
//*이름 코드페이지 변경용 재귀함수 선언
void Change_Data_Codepage_J2K(wchar_t* DataFolder);
//*데이터 코드페이지 변경용 재귀함수 선언

void Extract_Script_Text(wchar_t* DataFolder, unsigned int code);
//*lsb 텍스트 추출용 재귀함수 선언
void Replace_Script_Text(wchar_t* DataFolder, unsigned int code);
//*lsb 텍스트 추출용 재귀함수 선언
void Decompile_Script(wchar_t* DataFolder, unsigned int code);
//*디컴파일용 추출용 재귀함수 선언


void LM_Convert_Codepage_J2K(char* DataFolder)
{
	wprintf(TEXT("Changing Text Language Code(Jap -> Kor)...\n"));

	wchar_t* uni_dir = GetUniStr(DataFolder, LM_CP_KOR);
	//*디렉토리 유니코드로 바꿔놓기

	Change_Name_Codepage_J2K (uni_dir);
	Change_Data_Codepage_J2K(uni_dir);
	//*해당 디렉토리에 있는 모든 파일에 대해 검사
	//*파일명/디렉토리명 변경도 겸해 검사한다
	//*대상 확장자 : .lsb, .lpb, .lcm/.lmt, .txt, .lpm, .dat
	//*내가 만들어둔 stristr 혹은 wcristr로 검사해서 처리한다

	wprintf(TEXT("Convert Complete!\n"));
	free(uni_dir);
}
//*** 폴더 내 파일 코드페이지 일괄 변경 함수
//*** 그 과정에서 코드페이지에 없는 한자는 고쳐야 하며,
//*** 이를 위해서 목록에 없는 한자는 임의로 변경해야 할 가능성도 있다


void Change_Name_Codepage_J2K(wchar_t* DataFolder)
{
	HANDLE hFile;
	WIN32_FIND_DATAW wfd_w;
	wchar_t f_format[MAX_PATH];
	memset(f_format, 0, sizeof(wchar_t) * MAX_PATH);
	swprintf(f_format, TEXT("%s\\*"), DataFolder);
	//*찾을 준비 마치기

	bool is_there_next_file_or_dir = true;
	wchar_t f_d_n[MAX_PATH];
	wchar_t *f_d_n_j2k;
	DWORD _attr = 0;
	wchar_t f_n_org[MAX_PATH];
	wchar_t f_n_cvt[MAX_PATH];

	hFile = FindFirstFileW(f_format, &wfd_w);
	if (hFile != INVALID_HANDLE_VALUE) {

		do
		{
			_attr = wfd_w.dwFileAttributes;
			memset(f_d_n, 0, sizeof(wchar_t) * MAX_PATH);
			f_d_n_j2k = (wchar_t*)malloc(sizeof(wchar_t) * MAX_PATH);
			memset(f_d_n_j2k, 0, sizeof(wchar_t) * MAX_PATH);
			memcpy(f_d_n, wfd_w.cFileName, (sizeof(wchar_t) * wcslen(wfd_w.cFileName)));
			memcpy(f_d_n_j2k, wfd_w.cFileName, (sizeof(wchar_t) * wcslen(wfd_w.cFileName)));
			is_there_next_file_or_dir = FindNextFileW(hFile, &wfd_w);
			//*혹시 모르니 파일 / 폴더명을 바꾸기 전에 미리 속성을 저장 / 파일명 복사 후
			//*다음 결과를 확인한다
			 
			JPHan_2_KRHan(f_d_n_j2k);
			//*언어코드 바꿔치기

			memset(f_n_org, 0, sizeof(wchar_t) * MAX_PATH);
			swprintf(f_n_org, TEXT("%s\\%s"), DataFolder, f_d_n);
			//*원본 파일/디렉토리 이름

			memset(f_n_cvt, 0, sizeof(wchar_t) * MAX_PATH);
			swprintf(f_n_cvt, TEXT("%s\\%s"), DataFolder, f_d_n_j2k);
			//*바뀐 파일/디렉토리 이름

			if ((wcscmp(f_d_n_j2k, TEXT(".")) != 0) && (wcscmp(f_d_n_j2k, TEXT("..")) != 0)) {
				if (wcscmp(f_n_org, f_n_cvt) != 0) {
					_wrename(f_n_org, f_n_cvt);
				}
			}
			//*.이나 ..이 아닐 때만 이름 바꾸기

			if (_attr & FILE_ATTRIBUTE_DIRECTORY) {
				if ((wcscmp(f_d_n_j2k, TEXT(".")) != 0) && (wcscmp(f_d_n_j2k, TEXT("..")) != 0)) {
					Change_Name_Codepage_J2K(f_n_cvt);
					//*바뀐 디렉토리명을 넣어서 재귀로 호출
				}
			}
			//*디렉토리 속성일 때 : "."나 ".."이 아니면 재귀로 파고들기

			free(f_d_n_j2k);

		} while (is_there_next_file_or_dir);
		FindClose(hFile);
	}
	//*파일 / 디렉토리명 바꾸기
}
//*이름 코드페이지 변경용 재귀함수


void Change_Data_Codepage_J2K(wchar_t* DataFolder)
{
	HANDLE hFile;
	WIN32_FIND_DATAW wfd_w;
	wchar_t f_format[MAX_PATH];
	memset(f_format, 0, sizeof(wchar_t) * MAX_PATH);
	swprintf(f_format, TEXT("%s\\*"), DataFolder);
	//*찾을 준비 마치기

	bool is_there_next_file_or_dir = true;
	wchar_t f_d_n[MAX_PATH];
	DWORD _attr = 0;
	wchar_t f_n_org[MAX_PATH];

	hFile = FindFirstFileW(f_format, &wfd_w);
	if (hFile != INVALID_HANDLE_VALUE) {

		do
		{
			_attr = wfd_w.dwFileAttributes;
			memset(f_d_n, 0, sizeof(wchar_t) * MAX_PATH);
			memcpy(f_d_n, wfd_w.cFileName, (sizeof(wchar_t) * wcslen(wfd_w.cFileName)));
			is_there_next_file_or_dir = FindNextFileW(hFile, &wfd_w);
			//*혹시 모르니 파일 / 폴더명을 바꾸기 전에 미리 속성을 저장 / 파일명 복사 후
			//*다음 결과를 확인한다

			memset(f_n_org, 0, sizeof(wchar_t) * MAX_PATH);
			swprintf(f_n_org, TEXT("%s\\%s"), DataFolder, f_d_n);
			//*파일/디렉토리 이름

			if (_attr & FILE_ATTRIBUTE_DIRECTORY) {
				if ((wcscmp(f_d_n, TEXT(".")) != 0) && (wcscmp(f_d_n, TEXT("..")) != 0)) {
					Change_Data_Codepage_J2K(f_n_org);
					//*바뀐 디렉토리명을 넣어서 재귀로 호출
				}
			}
			//*디렉토리 속성일 때 : "."나 ".."이 아니면 재귀로 파고들기

			else {
				if ((wcsistr(f_d_n, TEXT(".lsb")) != 0) || (wcsistr(f_d_n, TEXT(".lpb")) != 0)
					|| (wcsistr(f_d_n, TEXT(".lpm")) != 0) || (wcsistr(f_d_n, TEXT(".lcm")) != 0)
					|| (wcsistr(f_d_n, TEXT(".lmt")) != 0) || (wcsistr(f_d_n, TEXT("install.dat")) != 0))
				{
					LM_BASE_SCRIPT* _scr;
					if (wcsistr(f_d_n, TEXT(".lsb")) != 0) { _scr = new LSB_SCRIPT(); }
					else if (wcsistr(f_d_n, TEXT(".lpb")) != 0) { _scr = new LPB_SCRIPT(); }
					else if (wcsistr(f_d_n, TEXT(".lpm")) != 0) { _scr = new LPM_SCRIPT(); }
					else if ((wcsistr(f_d_n, TEXT(".lcm")) != 0) || (wcsistr(f_d_n, TEXT(".lmt")) != 0))
					{
						_scr = new LCM_LMT_SCRIPT();
					}
					else { _scr = new INSTALL_DAT_SCRIPT(); }
					_scr->Script_Analysis(f_n_org, LM_CP_JAP);
					_scr->Convert_Codepage_J2K();
					_scr->Make_To_Script(f_n_org, LM_CP_KOR);
					delete _scr;
					//*스크립트로 편집함, 확장자별로 나눠서 진행
				}
				else if (wcsistr(f_d_n, TEXT(".lft")) != 0)
				{
					HANDLE hLft = CreateFileW(f_n_org, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hLft == INVALID_HANDLE_VALUE) {
						wprintf(TEXT("%s: Can't Open File\n"), f_n_org);
						_setmode(_fileno(stdout), _O_TEXT);
						if (!is_there_next_file_or_dir) { FindClose(hFile); return; }
						else { continue; }
					}
					//*핸들 생성

					unsigned int lft_size = GetFileSize(hLft, NULL);
					char* lft_buffer = (char*)malloc(lft_size);
					ReadFile(hLft, lft_buffer, lft_size, NULL, NULL);
					CloseHandle(hLft);
					//*파일 읽어들이기

					memset((lft_buffer + 0x1A), 0, 0x40);
					memcpy (lft_buffer + 0x1A, _exe_font_kor_name_hy_taegothic, strlen(_exe_font_kor_name_hy_taegothic));
					//*폰트명 새로 기록

					hLft = CreateFileW(f_n_org, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					WriteFile(hLft, lft_buffer, lft_size, NULL, NULL);
					free(lft_buffer);
					CloseHandle(hLft);
					//*다시 기록 후 정리

					//*폰트파일일 때는 0x1A 위치에 기록하면 된다
				}
				else if ((wcsistr(f_d_n, TEXT(".txt")) != 0) || (wcsistr(f_d_n, TEXT(".tsv")) != 0))
				{
					HANDLE hTxt = CreateFileW(f_n_org, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hTxt == INVALID_HANDLE_VALUE) {
						wprintf(TEXT("%s: Can't Open File\n"), f_n_org);
						_setmode(_fileno(stdout), _O_TEXT);
						if (!is_there_next_file_or_dir) { FindClose(hFile); return; }
						else { continue; }
					}
					//*핸들 생성

					unsigned int txt_size = GetFileSize(hTxt, NULL);
					if (txt_size == 0) {
						wprintf(TEXT("%s: File Size Is 0\n"), f_n_org);
						_setmode(_fileno(stdout), _O_TEXT);
						CloseHandle(hTxt);
						if (!is_there_next_file_or_dir) { FindClose(hFile); return; }
						else { continue; }
					}
					//*크기가 0이면 진행하지 않음

					UINT16 _chk = 0;
					ReadFile(hTxt, &_chk, 2, NULL, NULL);
					if (_chk == 0xFEFF) {
						wprintf(TEXT("%s: This File Isn't Unicode File\n"), f_n_org);
						_setmode(_fileno(stdout), _O_TEXT);
						CloseHandle(hTxt);
						if (!is_there_next_file_or_dir) { FindClose(hFile); return; }
						else { continue; }
					}
					//*유니코드가 아닐 때 진행, 생각해보면 txt 파일은 전부 유니코드가 아니었음

					SetFilePointer(hTxt, 0, NULL, FILE_BEGIN);
					char* txt_buffer = (char*)malloc(txt_size + 1);
					ReadFile(hTxt, txt_buffer, txt_size, NULL, NULL);
					txt_buffer[txt_size] = 0;
					CloseHandle(hTxt);
					//*파일 읽어들이기

					wchar_t* _txt_uni = GetUniStr(txt_buffer, LM_CP_JAP);
					free(txt_buffer);
					JPHan_2_KRHan(_txt_uni);
					char* _txt_j2k = GetAnsiStr(_txt_uni, LM_CP_KOR);
					free(_txt_uni);
					//*유니코드 -> 언어코드 변경 -> 다시 ansi코드

					hTxt = CreateFileW(f_n_org, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					WriteFile(hTxt, _txt_j2k, strlen(_txt_j2k), NULL, NULL);
					free(_txt_j2k);
					CloseHandle(hTxt);
					//*다시 기록 후 정리
				}
			}
			//*파일 속성일 때 : 포맷에 부합하면 파일 내용도 한국어 언어코드로 변경

		} while (is_there_next_file_or_dir);
		FindClose(hFile);
	}
	//*파일들 내용을 바꾸기
}
//*데이터 코드페이지 변경용 재귀함수




void CHANGE_DATA_IN_BUFFER (unsigned char* org_buff, unsigned int org_size, 
	const unsigned char* org_data, const unsigned char *chg_data, unsigned int data_len, bool is_once)
{
	unsigned char *_exe_buff_pnt = org_buff;
	bool Found_Bs;
	while (_exe_buff_pnt < (org_buff + org_size - data_len)) {
		while ((*(_exe_buff_pnt) != (org_data)[0]) && (_exe_buff_pnt < (org_buff + org_size - data_len)))
		{ _exe_buff_pnt++; }
		Found_Bs = true;
		for (unsigned int i = 0; i < data_len; i++) {
			if (_exe_buff_pnt[i] != (org_data)[i]) { Found_Bs = false; break; }
		}
		if (Found_Bs) { 
			memcpy(_exe_buff_pnt, chg_data, data_len); 
			if (is_once) { break; }
		}
		else { _exe_buff_pnt++; }
	}
	return;
}
//*같은 길이일 때 데이터 바꾸기

void LM_Convert_Exe_Codepage_J2K(char* ExeFile)
{
	wchar_t* uni_exe = GetUniStr(ExeFile, LM_CP_KOR);

	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Converting Exe Codepage(Jap -> Kor): Exe File: To %s\n"), uni_exe);

	HANDLE hExe = CreateFileW(uni_exe, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	free(uni_exe);
	if (hExe == INVALID_HANDLE_VALUE) {
		wprintf(TEXT("Failed!\n"));
		_setmode(_fileno(stdout), _O_TEXT);
		return; 
	}
	//*핸들 생성

	unsigned int ExeSize = GetFileSize(hExe, NULL);
	unsigned char* ExeBuffer = (unsigned char*)malloc(ExeSize);
	ReadFile(hExe, ExeBuffer, ExeSize, NULL, NULL);
	CloseHandle(hExe);
	//*파일 읽어들이기 / 핸들 정리

	unsigned char* _exe_buff_pnt;

	CHANGE_DATA_IN_BUFFER (ExeBuffer, ExeSize, _exe_search_bs_1, _exe_change_bs_1, 6, true);
	//*첫째 명령어 바꾸기 (LOGFONT 구조체에 0x81 강제 세팅)

	CHANGE_DATA_IN_BUFFER (ExeBuffer, ExeSize, _exe_search_bs_2, _exe_change_bs_2, 8, true);
	//*둘째 명령어 바꾸기 (반각을 전각으로 바꾸는 함수가 있는데 한글 완성형 밑으로만 바꾸게 하기)

	CHANGE_DATA_IN_BUFFER (ExeBuffer, ExeSize, _exe_search_bs_4, _exe_change_bs_4, 11, true);
	CHANGE_DATA_IN_BUFFER (ExeBuffer, ExeSize, _exe_search_bs_5, _exe_change_bs_5, 14, true);
	//*넷째, 다섯째 명령어 바꾸기 (81 40 매칭을 A1 A1 매칭으로 바꾸기)

	CHANGE_DATA_IN_BUFFER (ExeBuffer, ExeSize, _exe_font_jap_part_name_ms_2, _exe_font_kor_name_08_seoul_namsan_eb, 16, false);
	//*일부 명령어 및 폰트 바꾸기 (밑의 필터로 걸러낼 수 없으므로 먼저 손을 쓴다)
	//*모든 폰트를 바꿔야 하니 이처럼 해야 한다

	unsigned int data_section_start_pos = *(unsigned int*)(ExeBuffer + 0x220 + (sizeof(int) * 5));
	unsigned int data_section_size = *(unsigned int*)(ExeBuffer + 0x220 + (sizeof(int) * 4));
	unsigned int* pnt = (unsigned int*)(ExeBuffer + data_section_start_pos);
	for (unsigned int i = 0; i < (data_section_size / 4);i++) {
		if (pnt[i] == 0x3A4) { pnt[i] = 0x3B5; }
	}
	//*추가로 3A4 -> 3B5가 몇 군데 있음
	//*code 말고 data 영역만 뒤지면 딱 두 군데가 나올텐데 거기만 바꿔주면 된다
	//*어차피 4배수로 떨어지니 그냥 그렇게 판단하면 되는 것 같다

	_exe_buff_pnt = ExeBuffer;
	unsigned char* _last_pnt = NULL;
	while (_exe_buff_pnt < (ExeBuffer + ExeSize - 5)) {
		if (((*_exe_buff_pnt) == 0) && (*(unsigned int*)(_exe_buff_pnt + 1) == 0x3A4)) { _last_pnt = _exe_buff_pnt; }
		_exe_buff_pnt++;
	}
	if (_last_pnt != NULL) { *(unsigned int*)(_last_pnt + 1) = 0x3B5; }
	//*나머지 하나는 마지막 다 가서야 바꿔야 함
	//*없으면 바꾸지 말고.

	unsigned char* _exe_pnt = ExeBuffer;
	unsigned char* _exe_pnt_2 = ExeBuffer;
	unsigned char* _exe_pnt_3 = ExeBuffer;
	unsigned char* _exe_pnt_4 = ExeBuffer;
	while (_exe_pnt < (ExeBuffer + ExeSize)) {
		if ((*(unsigned int*)_exe_pnt) != 0xFFFFFFFF) { _exe_pnt++; }
		else {
			_exe_pnt_2 = (_exe_pnt + sizeof(int));
			unsigned int _str_len = *(unsigned int*)_exe_pnt_2;
			if ((_str_len == 0) || (_str_len >= 0x400)) { _exe_pnt++; }
			else {
				_exe_pnt_3 = (_exe_pnt_2 + sizeof(int));
				char* _str = (char*)malloc(_str_len + 1);
				memset(_str, 0, (_str_len + 1));
				bool _is_real_str = true;
				for (unsigned int i = 0; i < _str_len;i++) {
					if (_exe_pnt_3[i] == 0) { _is_real_str = false; }
					_str[i] = (char)_exe_pnt_3[i];
				}
				if (!_is_real_str) { free(_str); _exe_pnt++; }
				else {
					_exe_pnt_4 = (_exe_pnt_3 + _str_len);
					if ((*_exe_pnt_4) != 0x00) { free(_str); _exe_pnt++; }
					else {
						//*_exe_pnt : 0xFFFFFFFF
						//*_exe_pnt_2 : 문자열 길이 위치
						//*_exe_pnt_3 : 문자열 데이터
						//*_exe_pnt_4 : 문자열 끝부분

						wchar_t* _t_uni = GetUniStr(_str, LM_CP_JAP);
						free(_str);
						JPHan_2_KRHan(_t_uni);
						char* _t_cvt = GetAnsiStr(_t_uni, LM_CP_KOR);
						free(_t_uni);
						//*먼저 값을 뽑아놓기

						if (stricmp(_exe_xml_shift_jis_str, _t_cvt) == 0) {
							for (unsigned char* _ppt = _exe_pnt_2; _ppt < _exe_pnt_4; _ppt++) { *_ppt = 0; }
							*(unsigned int*)_exe_pnt_2 = strlen(_exe_xml_euc_kr_str);
							memcpy((_exe_pnt_2 + sizeof(int)), _exe_xml_euc_kr_str, strlen(_exe_xml_euc_kr_str));
						}
						else if (stricmp(_exe_shift_jis_str_1, _t_cvt) == 0) {
							for (unsigned char* _ppt = _exe_pnt_2; _ppt < _exe_pnt_4; _ppt++) { *_ppt = 0; }
							*(unsigned int*)_exe_pnt_2 = strlen(_exe_euc_kr_str_1);
							memcpy((_exe_pnt_2 + sizeof(int)), _exe_euc_kr_str_1, strlen(_exe_euc_kr_str_1));
						}
						else if (stricmp(_exe_shift_jis_str_2, _t_cvt) == 0) {
							for (unsigned char* _ppt = _exe_pnt_2; _ppt < _exe_pnt_4; _ppt++) { *_ppt = 0; }
							*(unsigned int*)_exe_pnt_2 = strlen(_exe_euc_kr_str_2);
							memcpy((_exe_pnt_2 + sizeof(int)), _exe_euc_kr_str_2, strlen(_exe_euc_kr_str_2));
						}
						else if ((stristr(_t_cvt, _exe_font_jap_part_name_ms) != NULL)
							|| (stristr(_t_cvt, _exe_font_jap_name_07_soft_gothic) != NULL)) {
							for (unsigned char* _ppt = _exe_pnt_2; _ppt < _exe_pnt_4; _ppt++) { *_ppt = 0; }
							*(unsigned int*)_exe_pnt_2 = strlen(_exe_font_kor_name_hy_taegothic);
							memcpy((_exe_pnt_2 + sizeof(int)), 
								_exe_font_kor_name_hy_taegothic, strlen(_exe_font_kor_name_hy_taegothic));
						}
						//*특수한 경우일 때는 원본 버퍼를 깡그리 비우고 원하는 내용으로 채워넣는다
						//*폰트는 예외가 좀 있을수도 있긴 한데 아마 "ＭＳ"만 검색해도 거의 다 걸릴거고
						//*안된다 싶으면 예외를 세팅하면 된다

						else {

							if (strcmp(_t_cvt, "終了") == 0) { memcpy(_exe_pnt_3, "종료", strlen("종료")); }
							else if (strcmp(_t_cvt, "畵面切替") == 0) { memcpy(_exe_pnt_3, "화면전환", strlen("화면전환")); }
							else { memcpy(_exe_pnt_3, _t_cvt, strlen(_t_cvt)); }
							free(_t_cvt);
							//*문자열 언어코드 바꿔치고 _exe_pnt_3에 기록하면 됨
							//*특수한 문자는 직접 기록
						}

						_exe_pnt = (_exe_pnt_4 + 1);
					}
					//*여기가 진짜이므로 위치를 통째로 갱신한다
				}
			}
		}
	}
	//*포인터로 돌아가면서 위치를 찾아 체크하기
	//[FF FF FF FF][글자 길이][글자 바이트][00 00 00 00] 이런 식으로 되어있는 부분을 싹 바꾼다

	CHANGE_DATA_IN_BUFFER(ExeBuffer, ExeSize, _exe_search_bs_3_1, _exe_change_bs_3_1, 6, false);
	CHANGE_DATA_IN_BUFFER(ExeBuffer, ExeSize, _exe_search_bs_3_2, _exe_change_bs_3_2, 6, true);
	CHANGE_DATA_IN_BUFFER(ExeBuffer, ExeSize, _exe_search_bs_3_3, _exe_change_bs_3_3, 6, true);
	//*마지막으로 셋째 명령어 바꾸기 (반각->전각 전환시 알파벳 / 숫자를 정상적으로 인식하게 하기)

	char _n_ExeFile[MAX_PATH], *tp_pnt = ExeFile;
	memset(_n_ExeFile, 0, MAX_PATH);
	for (tp_pnt = ExeFile + (strlen(ExeFile) - 1); tp_pnt >= ExeFile; tp_pnt--) {
		if ((*tp_pnt == '.')) { break; }
	}
	memcpy(_n_ExeFile, ExeFile, sizeof(char) * (tp_pnt - ExeFile));
	sprintf(_n_ExeFile + strlen(_n_ExeFile), "_h.exe");
	//*exe 파일명 지정

	wchar_t* _n_exe_uni = GetUniStr(_n_ExeFile, LM_CP_KOR);
	hExe = CreateFileW(_n_exe_uni, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	free(_n_exe_uni);
	WriteFile(hExe, ExeBuffer, ExeSize, NULL, NULL);
	CloseHandle(hExe);
	free(ExeBuffer);
	wprintf(TEXT("Finished!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
//	CloseHandle(hTxt);
	//*기록 및 정리
}
//*exe파일 코드페이지 변경용 함수




void LM_Extract_All_Script_Text(char* DataFolder, unsigned int code)
{
	wchar_t* uni_dir = GetUniStr(DataFolder, LM_CP_KOR);

	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Extracting Lsb Text: In Directory: %s\n"), uni_dir);

	Extract_Script_Text(uni_dir, code);

	wprintf(TEXT("Extract Complete!\n"));
	free(uni_dir);
}
//*해당 디렉토리 내 모든 lsb 파일 텍스트를 추출하는 함수


void Extract_Script_Text(wchar_t* DataFolder, unsigned int code)
{
	HANDLE hFile;
	WIN32_FIND_DATAW wfd_w;
	wchar_t f_format[MAX_PATH];
	memset(f_format, 0, sizeof(wchar_t) * MAX_PATH);
	swprintf(f_format, TEXT("%s\\*"), DataFolder);
	//*찾을 준비 마치기

	wchar_t f_d_n[MAX_PATH];
	DWORD _attr = 0;
	wchar_t f_n_org[MAX_PATH];
	//*기타 변수

	hFile = FindFirstFileW(f_format, &wfd_w);
	if (hFile != INVALID_HANDLE_VALUE) {

		do
		{
			_attr = wfd_w.dwFileAttributes;
			memset(f_d_n, 0, sizeof(wchar_t) * MAX_PATH);
			memcpy(f_d_n, wfd_w.cFileName, (sizeof(wchar_t) * wcslen(wfd_w.cFileName)));

			memset(f_n_org, 0, sizeof(wchar_t) * MAX_PATH);
			swprintf(f_n_org, TEXT("%s\\%s"), DataFolder, f_d_n);
			//*파일/디렉토리 이름

			if (_attr & FILE_ATTRIBUTE_DIRECTORY) {
				if ((wcscmp(f_d_n, TEXT(".")) != 0) && (wcscmp(f_d_n, TEXT("..")) != 0)) {
					Extract_Script_Text(f_n_org, code);
					//*바뀐 디렉토리명을 넣어서 재귀로 호출
				}
			}
			//*디렉토리 속성일 때 : "."나 ".."이 아니면 재귀로 파고들기

			else {

				if ((wcsistr(f_d_n, TEXT(".lsb")) != 0) 
					|| (wcsistr(f_d_n, TEXT(".lpb")) != 0)
					|| (wcsistr(f_d_n, TEXT("install.dat")) != 0))
				{
					LM_BASE_SCRIPT* _scr;
					if (wcsistr(f_d_n, TEXT(".lsb")) != 0) { _scr = new LSB_SCRIPT(); }
					else if (wcsistr(f_d_n, TEXT(".lpb")) != 0) { _scr = new LPB_SCRIPT(); }
					else { _scr = new INSTALL_DAT_SCRIPT(); }

					wchar_t* _tp_name = (wchar_t*)malloc(sizeof(wchar_t) * MAX_PATH);
					memset(_tp_name, 0, (sizeof(wchar_t) * MAX_PATH));
					memcpy(_tp_name, f_n_org, (sizeof(wchar_t) * (wcslen(f_n_org) - 4)));
					swprintf(_tp_name + wcslen(_tp_name), TEXT(".ext.txt"));
					//*파일명 만들기

					_scr->Script_Analysis(f_n_org, code);
					_scr->Extract_Text_Data(_tp_name);
					delete _scr;
					free(_tp_name);
					//* 출력명 : '~.lsb' -> '~.ext.txt'
				}
			}
			//*파일 속성일 때 : 포맷에 부합하면( = .lsb 혹은 lpb 혹은 dat 파일) 텍스트 추출

		} while (FindNextFileW(hFile, &wfd_w));

	}
	//*재귀적 호출
}
//*lsb 텍스트 추출용 재귀함수 선언




void LM_Replace_All_Script_Text(char* DataFolder, unsigned int code)
{
	wchar_t* uni_dir = GetUniStr(DataFolder, LM_CP_KOR);

	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Replacing Lsb Text: In Directory: %s\n"), uni_dir);

	Replace_Script_Text(uni_dir, code);

	wprintf(TEXT("Replace Complete!\n"));
	free(uni_dir);
}
//*해당 디렉토리 내 모든 lsb 파일을 대치하는 함수


void Replace_Script_Text(wchar_t* DataFolder, unsigned int code)
{
	HANDLE hFile;
	WIN32_FIND_DATAW wfd_w;
	wchar_t f_format[MAX_PATH];
	memset(f_format, 0, sizeof(wchar_t) * MAX_PATH);
	swprintf(f_format, TEXT("%s\\*"), DataFolder);
	//*찾을 준비 마치기

	wchar_t f_d_n[MAX_PATH];
	DWORD _attr = 0;
	wchar_t f_n_org[MAX_PATH];
	//*기타 변수

	hFile = FindFirstFileW(f_format, &wfd_w);
	if (hFile != INVALID_HANDLE_VALUE) {

		do
		{
			_attr = wfd_w.dwFileAttributes;
			memset(f_d_n, 0, sizeof(wchar_t) * MAX_PATH);
			memcpy(f_d_n, wfd_w.cFileName, (sizeof(wchar_t) * wcslen(wfd_w.cFileName)));

			memset(f_n_org, 0, sizeof(wchar_t) * MAX_PATH);
			swprintf(f_n_org, TEXT("%s\\%s"), DataFolder, f_d_n);
			//*파일/디렉토리 이름

			if (_attr & FILE_ATTRIBUTE_DIRECTORY) {
				if ((wcscmp(f_d_n, TEXT(".")) != 0) && (wcscmp(f_d_n, TEXT("..")) != 0)) {
					Replace_Script_Text(f_n_org, code);
					//*바뀐 디렉토리명을 넣어서 재귀로 호출
				}
			}
			//*디렉토리 속성일 때 : "."나 ".."이 아니면 재귀로 파고들기

			else {

				if ((wcsistr(f_d_n, TEXT(".lsb")) != 0)
					|| (wcsistr(f_d_n, TEXT(".lpb")) != 0)
					|| (wcsistr(f_d_n, TEXT("install.dat")) != 0))
				{
					LM_BASE_SCRIPT* _scr;
					if (wcsistr(f_d_n, TEXT(".lsb")) != 0) { _scr = new LSB_SCRIPT(); }
					else if (wcsistr(f_d_n, TEXT(".lpb")) != 0) { _scr = new LPB_SCRIPT(); }
					else { _scr = new INSTALL_DAT_SCRIPT(); }

					wchar_t* _tp_txt_name = (wchar_t*)malloc(sizeof(wchar_t) * MAX_PATH);
					memset(_tp_txt_name, 0, (sizeof(wchar_t) * MAX_PATH));
					memcpy(_tp_txt_name, f_n_org, (sizeof(wchar_t) * (wcslen(f_n_org) - 4)));
					swprintf(_tp_txt_name + wcslen(_tp_txt_name), TEXT(".ext.txt"));
					//*추출되었던 파일명 만들기

					HANDLE hT = CreateFileW(_tp_txt_name, GENERIC_READ, FILE_SHARE_READ,
						NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hT == INVALID_HANDLE_VALUE) {
						wprintf(TEXT("%s: There's no Extracted Txt File\n"), _tp_txt_name);
						free(_tp_txt_name); continue;
					}
					CloseHandle(hT);
					//*이 파일명 없으면 그냥 continue

					_scr->Script_Analysis(f_n_org, code);
					_scr->Replace_Text_Data(_tp_txt_name, code);
					_wremove(f_n_org);
					_scr->Make_To_Script(f_n_org, code);
					delete _scr;
					free(_tp_txt_name);
					//* 출력명 : '~.lsb' -> '~.ext.txt'
					//* 원본 파일은 지워야 한다
				}
			}
			//*파일 속성일 때 : 포맷에 부합하면( = .lsb 혹은 lpb 혹은 dat 파일) 텍스트 추출

		} while (FindNextFileW(hFile, &wfd_w));

	}
	//*재귀적 호출
}
//*lsb 텍스트 대치용 재귀함수 선언




void LM_Decompile_All_Script(char* DataFolder, unsigned int code)
{
	wchar_t* uni_dir = GetUniStr(DataFolder, LM_CP_KOR);

	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Decompiling All Script: In Directory: %s\n"), uni_dir);

	Decompile_Script(uni_dir, code);

	wprintf(TEXT("Decompile Complete!\n"));
	free(uni_dir);
}
//*해당 디렉토리 내 모든 스크립트 파일을 디컴파일하는 함수


void Decompile_Script(wchar_t* DataFolder, unsigned int code)
{
	HANDLE hFile;
	WIN32_FIND_DATAW wfd_w;
	wchar_t f_format[MAX_PATH];
	memset(f_format, 0, sizeof(wchar_t) * MAX_PATH);
	swprintf(f_format, TEXT("%s\\*"), DataFolder);
	//*찾을 준비 마치기

	wchar_t f_d_n[MAX_PATH];
	DWORD _attr = 0;
	wchar_t f_n_org[MAX_PATH];
	//*기타 변수

	hFile = FindFirstFileW(f_format, &wfd_w);
	if (hFile != INVALID_HANDLE_VALUE) {

		do
		{
			_attr = wfd_w.dwFileAttributes;
			memset(f_d_n, 0, sizeof(wchar_t) * MAX_PATH);
			memcpy(f_d_n, wfd_w.cFileName, (sizeof(wchar_t) * wcslen(wfd_w.cFileName)));

			memset(f_n_org, 0, sizeof(wchar_t) * MAX_PATH);
			swprintf(f_n_org, TEXT("%s\\%s"), DataFolder, f_d_n);
			//*파일/디렉토리 이름

			if (_attr & FILE_ATTRIBUTE_DIRECTORY) {
				if ((wcscmp(f_d_n, TEXT(".")) != 0) && (wcscmp(f_d_n, TEXT("..")) != 0)) {
					Decompile_Script(f_n_org, code);
					//*바뀐 디렉토리명을 넣어서 재귀로 호출
				}
			}
			//*디렉토리 속성일 때 : "."나 ".."이 아니면 재귀로 파고들기

			else {


				if ((wcsistr(f_d_n, TEXT(".lsb")) != 0) || (wcsistr(f_d_n, TEXT(".lpb")) != 0)
					|| (wcsistr(f_d_n, TEXT(".lpm")) != 0) || (wcsistr(f_d_n, TEXT(".lcm")) != 0)
					|| (wcsistr(f_d_n, TEXT(".lmt")) != 0) || (wcsistr(f_d_n, TEXT("install.dat")) != 0))
				{
					LM_BASE_SCRIPT* _scr;
					if (wcsistr(f_d_n, TEXT(".lsb")) != 0) { _scr = new LSB_SCRIPT(); }
					else if (wcsistr(f_d_n, TEXT(".lpb")) != 0) { _scr = new LPB_SCRIPT(); }
					else if (wcsistr(f_d_n, TEXT(".lpm")) != 0) { _scr = new LPM_SCRIPT(); }
					else if ((wcsistr(f_d_n, TEXT(".lcm")) != 0) || (wcsistr(f_d_n, TEXT(".lmt")) != 0))
					{
						_scr = new LCM_LMT_SCRIPT();
					}
					else { _scr = new INSTALL_DAT_SCRIPT(); }

					wchar_t* _tp_txt_name = (wchar_t*)malloc(sizeof(wchar_t) * MAX_PATH);
					memset(_tp_txt_name, 0, (sizeof(wchar_t) * MAX_PATH));
					memcpy(_tp_txt_name, f_n_org, (sizeof(wchar_t) * (wcslen(f_n_org) - 4)));
					swprintf(_tp_txt_name + wcslen(_tp_txt_name), TEXT(".txt"));
					//*파일명 만들기

					_scr->Script_Analysis(f_n_org, code);
					_scr->Decompile_To_Code(_tp_txt_name, code);
					free(_tp_txt_name);
					delete _scr;
					//*스크립트로 편집함, 확장자별로 나눠서 진행
				}
			}
			//*파일 속성일 때 : 포맷에 부합하면( = .lsb 혹은 lpb 혹은 dat 파일) 텍스트 추출

		} while (FindNextFileW(hFile, &wfd_w));

	}
	//*재귀적 호출
}
//*디컴파일용 재귀함수 선언