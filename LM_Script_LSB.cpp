#include "LM_Script.h"
#include "LM_Functions.h"


#pragma warning (disable:6031)
#pragma warning (disable:4996)

//*** lsb 스크립트 분석 및 적용 함수들을 서술하는 파일

LSB_INST_BASE* getInstClass(LSB_INST_TYPES _type, unsigned int _script_ver, LSB_HEADER* _header);
//*** 경우별로 타입에 맞춰 할당 후 반환하는 함수




//---------------------------- 스크립트 클래스 관련 함수 ----------------------------//
bool LSB_SCRIPT::Script_Analysis(wchar_t* LsbFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Analysis: Lsb File: %s\n"), LsbFile);

	HANDLE hLsb = CreateFileW(LsbFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hLsb == INVALID_HANDLE_VALUE) {
		wprintf(TEXT("%s: There's not LSB File\n"), LsbFile);
		_setmode(_fileno(stdout), _O_TEXT);
		return false;
	}
	unsigned int lsb_size = GetFileSize(hLsb, NULL);
	unsigned char* lsb_buffer = (unsigned char*)malloc(lsb_size);
	ReadFile(hLsb, lsb_buffer, lsb_size, NULL, NULL);
	CloseHandle(hLsb);
	//*lsb 파일 읽어들이기 (파일 경로 언어코드도 고려할 필요 있음)

	unsigned char* lsb_buffer_pnt = lsb_buffer;
	//*필요한 변수들

	if (!LSB_Header_Analysis(LsbFile, lsb_buffer, &lsb_buffer_pnt)) {
		_setmode(_fileno(stdout), _O_U16TEXT);
		wprintf(TEXT("%s: Error occured while analysing header\n"), LsbFile);
		free(lsb_buffer);
		_setmode(_fileno(stdout), _O_TEXT);
		return false;
	}
	//*헤더 분석

	if (!LSB_Body_Analysis(LsbFile, lsb_buffer, &lsb_buffer_pnt, code)) {
		_setmode(_fileno(stdout), _O_U16TEXT);
		wprintf(TEXT("%s: Error occured while analysing body\n"), LsbFile);
		free(lsb_buffer);
		_setmode(_fileno(stdout), _O_TEXT);
		return false;
	}
	//*몸통 분석

	wprintf(TEXT("Script Analysis Complete!\n"));
	free(lsb_buffer);
	_setmode(_fileno(stdout), _O_TEXT);
	return true;
	//*정리
}
//*** lsb 스크립트 파일 분석 함수


bool LSB_SCRIPT::LSB_Header_Analysis(wchar_t* LsbFile, unsigned char* _org_pnt, unsigned char** _lsb_buffer_pnt)
{
	_setmode(_fileno(stdout), _O_TEXT);
	if (_lsb_buffer_pnt == NULL) { return false; }
	//*필수조건

	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	lm_version = *(unsigned int*)(lsb_buffer_pnt);
	lsb_buffer_pnt += sizeof(int);
	if (lm_version != LM_VERSION_1) { printf("Lm script version : 1\n"); }
	else if (lm_version != LM_VERSION_2) { printf("Lm script version : 2\n"); }
	else if (lm_version != LM_VERSION_3) { printf("Lm script version : 3\n"); }
	else {
		_setmode(_fileno(stdout), _O_U16TEXT);
		wprintf(TEXT("%s: Not supported LSB script file\n"), LsbFile);
		_setmode(_fileno(stdout), _O_TEXT);
		return false;
	}
	//*버전 세팅 후 매직 바이트 분석

	flag = (*lsb_buffer_pnt); lsb_buffer_pnt++;
	//*첫 1바이트

	_header = new LSB_HEADER(lm_version);
	_header->Header_Read(_org_pnt, &lsb_buffer_pnt);
	//*헤더 데이터 읽어들이기

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
	return true;
	//*위치 변경 반영
}
//*** 헤더 분석 함수


bool LSB_SCRIPT::LSB_Body_Analysis(wchar_t* LsbFile, unsigned char* _org_pnt, unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	_setmode(_fileno(stdout), _O_TEXT);
	if (_lsb_buffer_pnt == NULL) { return false; }
	//*필수조건

	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);

	_body = new LSB_BODY(lm_version, _header);
	_body->Body_Read(_org_pnt, &lsb_buffer_pnt, code);
	//*헤더 세팅 후 몸체 데이터 읽어들이기

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
	return true;
	//*위치 변경 반영
}
//*** 몸통 분석 함수


void LSB_SCRIPT::Decompile_To_Code(wchar_t* TxtFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Decompiling to Code: Lsb File: To %s\n"), TxtFile);

	HANDLE hWrite = CreateFileW(TxtFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//*핸들 생성

	UINT16 BOM = 0xFEFF;
	WriteFile(hWrite, &BOM, 2, NULL, NULL);
	//*유니코드 형식 추출 준비 (BOM)
	
	memset(_txt_buf, 0, sizeof(wchar_t) * TXT_BUFF_LEN);
	//*맨 처음은 반드시 0으로 초기화함

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("Livemaker Script Version_0x%02X%s%s"), lm_version, _enter_raw_, separation_line);
	//*버전 기록

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[HEADER]%s"), _enter_raw_);
	if (_header != NULL) { _header->_decompile_code(hWrite); }
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), separation_line);
	WriteFile(hWrite, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//*헤더 클래스에 추출 명령

	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[BODY]%s"), _enter_raw_);
	WriteFile(hWrite, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);

	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	if (_body != NULL) { _body->_decompile_code (hWrite, code); }

	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), separation_line);
	WriteFile(hWrite, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//*몸통 클래스에 추출 명령

	CloseHandle(hWrite);
	//*핸들 해제

	wprintf(TEXT("Decompiling Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
}
//*스크립트 클래스 : 디컴파일 함수


void LSB_SCRIPT::Make_To_Script(wchar_t* newLsbFile, unsigned int code)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Writing: Lsb File: %s\n"), newLsbFile);

	HANDLE hNewLsb = CreateFileW(newLsbFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//*핸들 생성

	WriteFile(hNewLsb, &lm_version, sizeof(int), NULL, NULL);
	WriteFile(hNewLsb, &flag, sizeof(char), NULL, NULL);
	//* 버전 / 플래그 기록

	if (_header != NULL) { _header->_write_as_lsb(hNewLsb); }
	//* 헤더 기록

	if (_body != NULL) { _body->_write_as_lsb(hNewLsb, code); }
	//* 몸통 기록

	CloseHandle(hNewLsb);
	//*핸들 해제

	wprintf(TEXT("Writing Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	//*정리
}
//*스크립트 클래스 : lsb 파일로 기록하는 함수 (내부 언어코드 및 문자열 변환은 다른 함수에서)


void LSB_SCRIPT::Extract_Text_Data(wchar_t* DstTxtFile)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Extracting: Text File: %s\n"), DstTxtFile);

	HANDLE hTxt = CreateFileW(DstTxtFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//*핸들 생성

	UINT16 BOM = 0xFEFF;
	WriteFile(hTxt, &BOM, 2, NULL, NULL);
	//*유니코드 형식 추출 준비 (BOM)

	_body->_extract_text(hTxt);
	//*텍스트 추출 함수 호출

	CloseHandle(hTxt);
	//*핸들 해제

	wprintf(TEXT("Extracting Text Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	//*정리
}
//*텍스트 추출 함수

void LSB_SCRIPT::Replace_Text_Data(wchar_t* SrcTxtFile, unsigned int code)
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

	unsigned int line_count = Get_Txt_Count (txt_buffer, line_prefix, total_txt_count);
	//*텍스트가 있는 라인 수 확인

	LSB_TEXTS_PER_INST* _txt_list = (LSB_TEXTS_PER_INST*)malloc(sizeof(LSB_TEXTS_PER_INST) * line_count);
	memset(_txt_list, 0, (sizeof(LSB_TEXTS_PER_INST) * line_count));
	//*라인당 소유 텍스트 구조체 할당

	wchar_t* _line_pnt = txt_buffer, * _line_pnt_bak = txt_buffer;
	wchar_t line_num[9]; line_num[8] = 0x00;
	for (unsigned int i = 0; i < line_count;i++) {

		_line_pnt = wcsstr(_line_pnt_bak, line_prefix);
		_line_pnt += (wcslen(line_prefix) + 1);
		memcpy(line_num, _line_pnt, (sizeof(wchar_t) * 8));
		_txt_list[i]._line = wcstol(line_num, NULL, 10);
		//*_line_pnt : 라인 문자열 시작 지점
		//*라인 문자열 + ':' 만큼 길이를 더한 후 길이를 얻어낸다

		if (i != (line_count - 1)) {
			_line_pnt_bak = wcsstr(_line_pnt, line_prefix);
		}
		else {
			_line_pnt_bak = (txt_buffer + total_txt_count);
		}
		//*_line_pnt_bak : 해당 라인이 끝나는 지점
		//*만일 마지막 카운트면 위치를 못 찾기 때문에 수동으로 마무리를 지정한다

		unsigned int _tp_buf_size = _line_pnt_bak - _line_pnt;
		wchar_t* _tp_buf = (wchar_t*)malloc(sizeof(wchar_t) * (_tp_buf_size + 1));
		_tp_buf[_tp_buf_size] = 0;
		memcpy(_tp_buf, _line_pnt, (sizeof(wchar_t) * _tp_buf_size));
		//*해당 영역만 따로 카피를 떠서 진행함

		_txt_list[i]._txt_count = Get_Txt_Count(_tp_buf, text_data_prefix, _tp_buf_size);
		//*해당 명령어 내 텍스트 수 파악

		if (_txt_list[i]._txt_count != 0) {

			_txt_list[i]._txt_list_per_inst = (wchar_t**)malloc(sizeof(wchar_t*) * _txt_list[i]._txt_count);
			memset(_txt_list[i]._txt_list_per_inst, 0, (sizeof(wchar_t*) * _txt_list[i]._txt_count));
			//*공간 할당

			wchar_t* _txt_pnt = _tp_buf, * _txt_pnt_bak = _tp_buf;
			for (unsigned int j = 0; j < _txt_list[i]._txt_count;j++) {

				_txt_pnt = wcsstr(_txt_pnt_bak, text_data_prefix);
				_txt_pnt += wcslen(text_data_prefix);
				_txt_pnt_bak = wcsstr(_txt_pnt, text_prefix);
				_txt_pnt_bak += wcslen(text_prefix);
				//*_txt_pnt_bak : 텍스트 시작 지점

				_txt_pnt = wcsstr(_txt_pnt_bak, text_prefix);
				//*_txt_pnt : 텍스트 끝나는 지점

				wchar_t* _tmp = (wchar_t*)malloc(sizeof(wchar_t) * (_txt_pnt - _txt_pnt_bak + 1));
				memcpy(_tmp, _txt_pnt_bak, (sizeof(wchar_t) * (_txt_pnt - _txt_pnt_bak)));
				_tmp[(_txt_pnt - _txt_pnt_bak)] = 0;
				_txt_list[i]._txt_list_per_inst[j] = _tmp;
				//*데이터 할당 / 카피 후 붙여넣기 / 리스트에 세팅

				_txt_pnt_bak = _txt_pnt + wcslen(text_prefix);
				//*_txt_pnt_bak은 이 다음 지점에 세팅한다

			}
			//*텍스트 수만큼 루프 돌림

		}
		//*텍스트 수가 0이 아닐 때만 진행

		else if (wcsstr (_tp_buf, _inst_type_list[TYPE_INST_TEXT_INST]) != NULL) {
			_txt_list[i]._txt_count = 1;
			_txt_list[i]._txt_list_per_inst = (wchar_t**)malloc(sizeof(wchar_t*));
			_txt_list[i]._txt_list_per_inst[0] = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(_tp_buf) + 1));
			memset(_txt_list[i]._txt_list_per_inst[0], 0, (sizeof(wchar_t) * (wcslen(_tp_buf) + 1)));
			memcpy(_txt_list[i]._txt_list_per_inst[0], _tp_buf, (sizeof(wchar_t) * wcslen(_tp_buf)));
		}
		//*텍스트 수가 0일 때는 _TEXT_INST인지 확인해보고 만일 그렇다면 그대로 복사해서 추가해 준다
		
		free(_tp_buf);
		//*다 진행했으면 임시 버퍼 해제

	}
	//*읽어들인 파일로 텍스트 세팅

	_body->_replace_text(_txt_list, line_count, code);
	//*텍스트 교체 함수 호출

	for (unsigned int i = 0; i < line_count; i++) { 
		for (unsigned int j = 0; j < _txt_list[i]._txt_count; j++) { free(_txt_list[i]._txt_list_per_inst[j]); }
		if (_txt_list[i]._txt_list_per_inst != NULL) { free(_txt_list[i]._txt_list_per_inst); }
	}
	free (txt_buffer);
	//*버퍼 해제

	wprintf(TEXT("Replacing Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
	//*정리
}
//*텍스트 교체 함수


void LSB_SCRIPT::Convert_Codepage_J2K()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(TEXT("Changing Text Language Code(Jap -> Kor)...\n"));

	_body->_change_code_J2K();

	wprintf(TEXT("Complete!\n"));
	_setmode(_fileno(stdout), _O_TEXT);
}
//*텍스트 언어코드를 일본어에서 한글로 바꾸는 함수
//---------------------------- 스크립트 클래스 관련 함수 ----------------------------//




//---------------------------- 헤더 클래스 관련 함수 ----------------------------//
bool LSB_HEADER::Header_Read(unsigned char* _org_pnt, unsigned char** _lsb_buffer_pnt)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);

	opcode_count = *(unsigned int*)(lsb_buffer_pnt);
	lsb_buffer_pnt += sizeof(int);
	opcode_property_count = *(unsigned int*)(lsb_buffer_pnt);
	lsb_buffer_pnt += sizeof(int);
	//*헤더 영역 : 오브젝트 수 / 오브젝트 크기 분석

	opcode_property_table = (unsigned char**)malloc(sizeof(unsigned char*) * opcode_count);
	for (unsigned int i = 0; i < opcode_count; i++) {
		opcode_property_table[i] = (unsigned char*)malloc(opcode_property_count);
		memcpy(opcode_property_table[i], lsb_buffer_pnt, opcode_property_count);
		lsb_buffer_pnt += opcode_property_count;
	}
	//*각 크기에 맞추어 할당 후 복제

	opcode_property_boolean_table = (bool**)malloc(sizeof(bool*) * opcode_count);
	for (unsigned int i = 0; i < opcode_count; i++) {
		opcode_property_boolean_table[i] = (bool*)malloc(opcode_property_count * 8);
		for (unsigned int j = 0; j < opcode_property_count; j++) {
			for (unsigned int k = 0; k < 8; k++) {
				opcode_property_boolean_table[i][(j * 8) + k] = (opcode_property_table[i][j] & (1 << k)) != 0;
			}
		}
	}
	//*각 크기에 맞추어 할당 (bool이므로 8배 늘리기)
	//*아마 각 스크립트 버전에 따라 ver2는 0xA5개, ver3은 0xAC개로 제한된 듯 한데
	//*크게 신경 쓸 필요는 없을 듯 하다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
	return true;
	//*위치 변경 반영
}
//*헤더 클래스 : 버퍼에서 데이터 받아오기


void LSB_HEADER::_decompile_code(HANDLE hWrite)
{
	if (opcode_property_table != NULL) {
		for (unsigned int i = 0; i < opcode_count; i++) {

			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_);
			//*들여쓰기 출력

			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s%02d]::"), property_array_prefix, i);
			//*속성 헤더 기록

			for (unsigned int j = 0; j < opcode_property_count; j++) {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[0x%02X]"), opcode_property_table[i][j]);
			}
			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _enter_raw_);
			//*모든 원소 기록
		}
	}
	//*opcode 배열 기록
}
//*헤더 클래스 : 디컴파일 함수


void LSB_HEADER::_write_as_lsb(HANDLE hNewLsb)
{
	WriteFile(hNewLsb, &opcode_count, sizeof(int), NULL, NULL);
	WriteFile(hNewLsb, &opcode_property_count, sizeof(int), NULL, NULL);
	//*명령어 수 / 각 명령어당 속성 수 기록

	for (unsigned int i = 0; i < opcode_count; i++) {
		WriteFile(hNewLsb, opcode_property_table[i], opcode_property_count, NULL, NULL);
	}
	//*속성 정보 배열 기록

	wprintf(TEXT("Header Writing Complete...\n"));
}
//*헤더 클래스 : lsb 파일로 기록하는 함수
//---------------------------- 헤더 클래스 관련 함수 ----------------------------//




//---------------------------- 몸체 클래스 관련 함수 ----------------------------//
bool LSB_BODY::Body_Read(unsigned char* _org_pnt, unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);

	inst_list_count = *(unsigned int*)(lsb_buffer_pnt);
	lsb_buffer_pnt += sizeof(int);
	inst_list_arr = (LSB_INST_BASE**)malloc(sizeof(LSB_INST_BASE*) * inst_list_count);
	memset(inst_list_arr, 0, sizeof(LSB_INST_BASE*) * inst_list_count);
	//*총 명령어 덩어리 수 파악 / 할당

	for (unsigned int i = 0; i < inst_list_count; i++) {
		unsigned char _opcode = *(lsb_buffer_pnt++);
		inst_list_arr[i] = getInstClass((LSB_INST_TYPES)_opcode, _script_ver, _header);
		if (inst_list_arr[i] == NULL) {
			printf("Illegal command : 0x%02X at [0x%08X]\n", _opcode, ((lsb_buffer_pnt - 1) - _org_pnt));
			return false;
		}
		//*opcode 읽기, ver2에서는 0x3C까지만 지원하는 듯 한데 그거까지 신경쓰고 싶진 않다
		//*사실상 read_function_prologue가 모든 명령어에 적용되는 듯 하다
		inst_list_arr[i]->_read_inst_data(&lsb_buffer_pnt, code);
		//*각 명령어에 걸맞게 데이터 읽어들이기
	}
	//*스크립트에 기술된 명령어대로 돌아가며 읽어들이기

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
	return true;
	//*위치 변경 반영
}
//*몸체 클래스 : 버퍼에서 데이터 받아오기


void LSB_BODY::_decompile_code(HANDLE hWrite, unsigned int code)
{
	if (inst_list_arr != NULL) {
		for (unsigned int i = 0; i < inst_list_count; i++) {
			inst_list_arr[i]->_decompile_code(hWrite, i, code, 1);
		}
	}
	//*들여쓰기 추가
}
//*몸체 클래스 : 디컴파일 함수


void LSB_BODY::_write_as_lsb(HANDLE hNewLsb, unsigned int code)
{
	WriteFile(hNewLsb, &inst_list_count, sizeof(int), NULL, NULL);
	//*총 명령어 수 기록

	for (unsigned int i = 0; i < inst_list_count; i++) {
		inst_list_arr[i]->_write_as_lsb(hNewLsb, code);
	}
	//*명령어만큼 돌아가며 기록

	wprintf(TEXT("Body Writing Complete...\n"));
}
//*몸통 클래스 : lsb 파일로 기록하는 함수


void LSB_BODY::_extract_text(HANDLE hTxt)
{
	for (unsigned int i = 0; i < inst_list_count; i++) {
		inst_list_arr[i]->_extract_text(hTxt);
	}
	//*명령어만큼 돌아가며 추출

	wprintf(TEXT("Text Extracting Complete...\n"));
}
//*몸통 클래스 : 텍스트 추출 함수


void LSB_BODY::_replace_text(LSB_TEXTS_PER_INST* txt_list, unsigned int txt_list_count, unsigned int code)
{
	for (unsigned int i = 0; i < txt_list_count; i++) {

		LSB_INST_BASE* _pnt = NULL;
		for (unsigned int j = 0; j < inst_list_count; j++) {
			if (inst_list_arr[j]->_line == txt_list[i]._line) { 
				_pnt = inst_list_arr[j]; break; 
			}
		}
		//*라인으로 등록된 명령어 찾기

		if (_pnt != NULL) { 
			_pnt->_replace_text (&txt_list[i], code); 
		}
		//*null이면 그냥 안함, 찾아내면 수행함

	}
	//*라인이 등록되어 있는 명령어만큼 돌아가며 대치

	wprintf(TEXT("Text Replacing Complete...\n"));
}
//*몸통 클래스 : 텍스트 교체 함수


void LSB_BODY::_change_code_J2K()
{
	for (unsigned int i = 0; i < inst_list_count; i++) {
		inst_list_arr[i]->_change_code_J2K();
	}
	//*루프 돌면서 바꿔치기
}
//*몸통 클래스 : 텍스트 코드 변경 함수
//---------------------------- 몸체 클래스 관련 함수 ----------------------------//




//---------------------------- 기본 클래스 함수 ----------------------------//
void LSB_INST_BASE::_decompile_code(HANDLE hWrite, unsigned int offset, unsigned int code, unsigned int _indent)
{
	//----------- 텍스트 버퍼에 모으기 -----------//
	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	for (unsigned int i = 0; i < _indent; i++) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_); }
	WriteFile(hWrite, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//*들여쓰기 출력 (텍스트 파일로)
	//----------- 텍스트 버퍼에 모으기 -----------//

	//----------- 텍스트 버퍼에 모으기 -----------//
	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s:%08d][%s:%08d][%08X][%02X][%02X][%s:%s]%s"),
		line_prefix, _line, offset_prefix, offset, this->_indent, _mute, _not_update, inst_type_prefix, _inst_type_list[_op_type], _enter_raw_);
	//*명령어 종류 출력

	unsigned int _param_c = 0, _addi = 0;
	for (unsigned int i = 0; i < _data_chunks_count; i++) {

		if (_param_c != 0) { _addi = 1; _param_c--; }
		else { _addi = 0; }
		//*함수 인자 수만큼 들여쓰기 반영

		for (unsigned int j = 0; j < (_indent + _addi + 1); j++) {
			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_);
		}
		//*들여쓰기 출력, 함수 인자 출력시 한칸 더 붙여 출력해야 함

		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s%s"), text_prefix, (wchar_t*)_data_chunks[i]._str);
		//*타입명 일부분만 먼저 출력
		//* ([◆]타입명)

		if (wcscmp(_data_chunks[i]._str, TEXT("Target_Label_Offset")) == 0)
		{
			unsigned int _val = *(unsigned int*)_data_chunks[i]._data;
			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s%08d%s%s"), text_prefix, _val, text_prefix, _enter_raw_);
		}
		else if ((wcscmp(_data_chunks[i]._str, TEXT("Parameter_Count")) == 0)
			|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Type")) == 0)
			|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Scope")) == 0))
		{
			unsigned int _val = *(unsigned int*)_data_chunks[i]._data;
			if (wcscmp(_data_chunks[i]._str, TEXT("Parameter_Count")) == 0) { _param_c = _val; }
			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s0x%08X%s%s"), text_prefix, _val, text_prefix, _enter_raw_);
		}
		//*단순 정수, 함수 인자 수라면 값을 새로 갱신한다
		//* ([◆]0x%08X[◆]\r\n)

		else if ((wcscmp(_data_chunks[i]._str, TEXT("Var_Scope_Byte")) == 0)
			|| (wcscmp(_data_chunks[i]._str, TEXT("All_Clear_Byte")) == 0))
		{
			unsigned char _val = *(unsigned char*)_data_chunks[i]._data;
			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s0x%02X%s%s"), text_prefix, _val, text_prefix, _enter_raw_);
		}
		//*단순 바이트
		//* ([◆]0x%02X[◆]\r\n)

		else if ((wcscmp(_data_chunks[i]._str, TEXT("Result")) == 0)
			|| (wcscmp(_data_chunks[i]._str, TEXT("Script_Page")) == 0)
			|| (wcscmp(_data_chunks[i]._str, TEXT("Label")) == 0)
			|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Name")) == 0))
		{
			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s%s%s%s"), text_prefix, (wchar_t*)_data_chunks[i]._data, text_prefix, _enter_raw_);
		}
		//*단순 문자열 (코드값에 따라 적용)
		//* ([◆]문자열[◆]\r\n)

		else if ((wcscmp(_data_chunks[i]._str, TEXT("Event_Block")) == 0))
		{
			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s%s"), text_prefix, _enter_raw_);
			//*개행 처리

			LSB_EVENT* _evb = (LSB_EVENT*)_data_chunks[i]._data;
			_evb->_decompile_code(hWrite, code, (_indent + _addi + 2));
			//*데이터 출력
		}
		//*이벤트 블럭

		else {
			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s%s"), text_prefix, _enter_raw_);
			//*개행 처리

			LSB_PARAMETERS* _pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
			_pnt->_decompile_code(hWrite, code, (_indent + _addi + 2));
		}
		//*이외는 전부 패러미터 리스트
	}
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _enter_raw_);
	//----------- 텍스트 버퍼에 모으기 -----------//

	WriteFile(hWrite, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//*개행 한 번 더 하고 출력
}
//*** 전부 디컴파일해 출력하는 함수


void LSB_INST_BASE::_write_as_lsb(HANDLE hNewLsb, unsigned int code)
{
	WriteFile(hNewLsb, &_op_type, sizeof(char), NULL, NULL);
	//*맨 앞에 이 명령어의 속성(1바이트) 기록

	WriteFile(hNewLsb, &_indent, sizeof(int), NULL, NULL);
	WriteFile(hNewLsb, &_mute, sizeof(char), NULL, NULL);
	WriteFile(hNewLsb, &_not_update, sizeof(char), NULL, NULL);
	WriteFile(hNewLsb, &_line, sizeof(int), NULL, NULL);
	//*들여쓰기(indent) / mute / _not_update_(static?) / 라인 위치 기록

	for (unsigned int i = 0; i < _data_chunks_count; i++) {

		if ((wcscmp(_data_chunks[i]._str, TEXT("Target_Label_Offset")) == 0)
			|| (wcscmp(_data_chunks[i]._str, TEXT("Parameter_Count")) == 0)
			|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Scope")) == 0))
		{
			_write_int(hNewLsb, *(unsigned int*)_data_chunks[i]._data);
		}
		//*단순 정수

		else if ((wcscmp(_data_chunks[i]._str, TEXT("Var_Type")) == 0)
			|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Scope_Byte")) == 0)
			|| (wcscmp(_data_chunks[i]._str, TEXT("All_Clear_Byte")) == 0))
		{
			_write_byte(hNewLsb, *(unsigned char*)_data_chunks[i]._data);
		}
		//*단순 바이트

		else if ((wcscmp(_data_chunks[i]._str, TEXT("Result")) == 0)
			|| (wcscmp(_data_chunks[i]._str, TEXT("Script_Page")) == 0)
			|| (wcscmp(_data_chunks[i]._str, TEXT("Label")) == 0)
			|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Name")) == 0))
		{
			wchar_t* _w1 = Replace_Text((wchar_t*)_data_chunks[i]._data, hex_1_plained, hex_1_raw);
			free(_data_chunks[i]._data);
			_data_chunks[i]._data = Replace_Text(_w1, _enter_plained_, _enter_raw_);
			free(_w1);
			char* _ansi = GetAnsiStr((wchar_t*)_data_chunks[i]._data, code);
			_write_str(hNewLsb, _ansi);
			free(_ansi);
			//*대치 필요, ansi 문자열로 바꾸고 기록
		}
		//*단순 문자열

		else if ((wcscmp(_data_chunks[i]._str, TEXT("Event_Block")) == 0))
		{
			LSB_EVENT* _t = (LSB_EVENT*)_data_chunks[i]._data;
			_t->_write_as_lsb(hNewLsb, code);
		}
		//*이벤트 블럭

		else {
			LSB_PARAMETERS *_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
			_t->_write_as_lsb(hNewLsb, code);
		}
		//*패러미터 리스트

		//*각 속성에 따라 기록함 (정수 / 문자열 / 이벤트 블럭 / 패러미터 리스트)
	}
	//*데이터 덩어리 수만큼 돌아가며 기록, 데이터 덩어리 수는 기록하지 않음
	//*그냥 순서대로 기록하면 됨
}
//*기본 클래스 : lsb 파일로 기록하는 함수


void LSB_INST_BASE::_extract_text(HANDLE hTxt)
{
	//----------- 텍스트 버퍼에 모으기 -----------//
	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s:%08d][%s:%s]%s"),
		line_prefix, _line, inst_type_prefix, _inst_type_list[_op_type], _enter_raw_);
	WriteFile(hTxt, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//*라인 넘버 먼저 출력
	//----------- 텍스트 버퍼에 모으기 -----------//

	//----------- 텍스트 버퍼에 모으기 -----------//
	memset(_txt_buf, 0, sizeof(wchar_t) * wcslen(_txt_buf));
	
	_extract_text_each_inst(hTxt);
	//*각 명령어별로 추출 실시

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _enter_raw_);
	//*가독성이 안좋으니 개행 추가

	WriteFile(hTxt, _txt_buf, (sizeof(wchar_t) * wcslen(_txt_buf)), NULL, NULL);
	//----------- 텍스트 버퍼에 모으기 -----------//
}
//*기본 클래스 : 텍스트 추출 함수


void LSB_INST_BASE::_replace_text(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	_replace_text_each_inst(txt_list_elem, code);
	//* 각 명령어별로 교체 실시
}
//*기본 클래스 : 텍스트 교체 함수


void LSB_INST_BASE::_change_code_J2K()
{
	if (_data_chunks != NULL) {
		for (unsigned int i = 0; i < _data_chunks_count; i++) {
			if ((wcscmp(_data_chunks[i]._str, TEXT("Result")) == 0)
				|| (wcscmp(_data_chunks[i]._str, TEXT("Script_Page")) == 0)
				|| (wcscmp(_data_chunks[i]._str, TEXT("Label")) == 0)
				|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Name")) == 0))
			{
				JPHan_2_KRHan((wchar_t*)_data_chunks[i]._data);
			}
			//*문자열은 직접 바꿔치기

			else if (wcscmp(_data_chunks[i]._str, TEXT("Event_Block")) == 0)
			{
				LSB_EVENT* _t = (LSB_EVENT*)_data_chunks[i]._data;
				_t->_change_code_J2K();
			}
			//*이벤트 클래스는 다시 호출

			else if ((wcscmp(_data_chunks[i]._str, TEXT("Parameter_Count")) == 0)
				|| (wcscmp(_data_chunks[i]._str, TEXT("Target_Label_Offset")) == 0)
				|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Type")) == 0)
				|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Scope")) == 0)
				|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Scope_Byte")) == 0)
				|| (wcscmp(_data_chunks[i]._str, TEXT("All_Clear_Byte")) == 0))
			{;}
			//*나머지는 그냥 무시한다

			else {
				LSB_PARAMETERS* _t = (LSB_PARAMETERS*)_data_chunks[i]._data;
				_t->_change_code_J2K();
			}
			//*패러미터 클래스는 다시 호출
		}
	}
	//*루프돌면서 수행
}
//*기본 클래스 : 텍스트 코드 변경 함수


LSB_INST_BASE* getInstClass(LSB_INST_TYPES _type, unsigned int _script_ver, LSB_HEADER* _header)
{
	switch (_type)
	{
		case TYPE_INST_IF: { return new LSB_INST_IF(_type, _script_ver, _header); }
		case TYPE_INST_ELIF: { return new LSB_INST_ELIF(_type, _script_ver, _header); }
		case TYPE_INST_ELSE: { return new LSB_INST_ELSE(_type, _script_ver, _header); }

		case TYPE_INST_LABEL: { return new LSB_INST_LABEL(_type, _script_ver, _header); }
		case TYPE_INST_JUMP: { return new LSB_INST_JUMP(_type, _script_ver, _header); }

		case TYPE_INST_CALL: { return new LSB_INST_CALL(_type, _script_ver, _header); }

		case TYPE_INST_EXIT: { return new LSB_INST_EXIT(_type, _script_ver, _header); }

		case TYPE_INST_WAIT: { return new LSB_INST_WAIT(_type, _script_ver, _header); }
		case TYPE_INST_TIMER: { return new LSB_INST_TIMER(_type, _script_ver, _header); }

		case TYPE_INST_CALC_FUNC: { return new LSB_INST_CALC_FUNC(_type, _script_ver, _header); }

		case TYPE_INST_BOX_NEW: { return new LSB_INST_BOX_NEW(_type, _script_ver, _header); }
		case TYPE_INST_IMAGE_NEW: { return new LSB_INST_IMAGE_NEW(_type, _script_ver, _header); }
		case TYPE_INST_MESSAGE_NEW: { return new LSB_INST_MESSAGE_NEW(_type, _script_ver, _header); }
		case TYPE_INST_PARTICLE_NEW: { return new LSB_INST_PARTICLE_NEW(_type, _script_ver, _header); }
		case TYPE_INST_FIRE_NEW: { return new LSB_INST_FIRE_NEW(_type, _script_ver, _header); }
		case TYPE_INST_EDIT_NEW: { return new LSB_INST_EDIT_NEW(_type, _script_ver, _header); }
		case TYPE_INST_MEMO_NEW: { return new LSB_INST_MEMO_NEW(_type, _script_ver, _header); }
		case TYPE_INST_MAP_IMAGE_NEW: { return new LSB_INST_MAP_IMAGE_NEW(_type, _script_ver, _header); }
		case TYPE_INST_WAVE_NEW: { return new LSB_INST_WAVE_NEW(_type, _script_ver, _header); }
		case TYPE_INST_TILE_NEW: { return new LSB_INST_TILE_NEW(_type, _script_ver, _header); }
		case TYPE_INST_SLIDER_NEW: { return new LSB_INST_SLIDER_NEW(_type, _script_ver, _header); }
		case TYPE_INST_SCROLL_BAR_NEW: { return new LSB_INST_SCROLL_BAR_NEW(_type, _script_ver, _header); }
		case TYPE_INST_GAUGE_NEW: { return new LSB_INST_GAUGE_NEW(_type, _script_ver, _header); }
		case TYPE_INST_PREV_MENU_NEW: { return new LSB_INST_PREV_MENU_NEW(_type, _script_ver, _header); }

		case TYPE_INST_VAR_NEW: { return new LSB_INST_VAR_NEW(_type, _script_ver, _header); }
		case TYPE_INST_VAR_DELETE: { return new LSB_INST_VAR_DELETE(_type, _script_ver, _header); }

		case TYPE_INST_MOVIE: { return new LSB_INST_MOVIE(_type, _script_ver, _header); }
		case TYPE_INST_FLIP: { return new LSB_INST_FLIP(_type, _script_ver, _header); }
		case TYPE_INST_MOVIE_STOP: { return new LSB_INST_MOVIE_STOP(_type, _script_ver, _header); }

		case TYPE_INST_CINEMA: { return new LSB_INST_CINEMA(_type, _script_ver, _header); }

		case TYPE_INST_GET_PROPERTY: { return new LSB_INST_GET_PROPERTY(_type, _script_ver, _header); }
		case TYPE_INST_SET_PROPERTY: { return new LSB_INST_SET_PROPERTY(_type, _script_ver, _header); }

		case TYPE_INST_OBJECT_DELETE: { return new LSB_INST_OBJECT_DELETE(_type, _script_ver, _header); }

		case TYPE_INST_TEXT_INST: { return new LSB_INST_TEXT_INST(_type, _script_ver, _header); }
		case TYPE_INST_TEXT_CLEAR: { return new LSB_INST_TEXT_CLEAR(_type, _script_ver, _header); }

		case TYPE_INST_CLEAR_HISTORY: { return new LSB_INST_CLEAR_HISTORY(_type, _script_ver, _header); }
		case TYPE_INST_CALL_HISTORY: { return new LSB_INST_CALL_HISTORY(_type, _script_ver, _header); }
		case TYPE_INST_FORMAT_HISTORY: { return new LSB_INST_FORMAT_HISTORY(_type, _script_ver, _header); }

		case TYPE_INST_CAPTION: { return new LSB_INST_CAPTION(_type, _script_ver, _header); }
		case TYPE_INST_CG_CAPTION: { return new LSB_INST_CG_CAPTION(_type, _script_ver, _header); }

		case TYPE_INST_MENU: { return new LSB_INST_MENU(_type, _script_ver, _header); }
		case TYPE_INST_MENU_CLOSE: { return new LSB_INST_MENU_CLOSE(_type, _script_ver, _header); }
		case TYPE_INST_BUTTON: { return new LSB_INST_BUTTON(_type, _script_ver, _header); }

		case TYPE_INST_COMMENT: { return new LSB_INST_COMMENT(_type, _script_ver, _header); }

		case TYPE_INST_WHILE: { return new LSB_INST_WHILE(_type, _script_ver, _header); }
		case TYPE_INST_WHILE_INIT: { return new LSB_INST_WHILE_INIT(_type, _script_ver, _header); }
		case TYPE_INST_WHILE_LOOP: { return new LSB_INST_WHILE_LOOP(_type, _script_ver, _header); }
		case TYPE_INST_BREAK: { return new LSB_INST_BREAK(_type, _script_ver, _header); }
		case TYPE_INST_CONTINUE: { return new LSB_INST_CONTINUE(_type, _script_ver, _header); }

		case TYPE_INST_GAME_SAVE: { return new LSB_INST_GAME_SAVE(_type, _script_ver, _header); }
		case TYPE_INST_GAME_LOAD: { return new LSB_INST_GAME_LOAD(_type, _script_ver, _header); }
		case TYPE_INST_CLEAR_LOAD: { return new LSB_INST_CLEAR_LOAD(_type, _script_ver, _header); }

		case TYPE_INST_PC_RESET: { return new LSB_INST_PC_RESET(_type, _script_ver, _header); }
		case TYPE_INST_RESET: { return new LSB_INST_RESET(_type, _script_ver, _header); }

		case TYPE_INST_SOUND: { return new LSB_INST_SOUND(_type, _script_ver, _header); }
		case TYPE_INST_MEDIA_PLAY: { return new LSB_INST_MEDIA_PLAY(_type, _script_ver, _header); }

		case TYPE_INST_TERMINATE: { return new LSB_INST_TERMINATE(_type, _script_ver, _header); }

		case TYPE_INST_DO_EVENT: { return new LSB_INST_DO_EVENT(_type, _script_ver, _header); }

		case TYPE_INST_PROPERTY_MOTION: { return new LSB_INST_PROPERTY_MOTION(_type, _script_ver, _header); }

		case TYPE_INST_SAVE_CABINET: { return new LSB_INST_SAVE_CABINET(_type, _script_ver, _header); }
		case TYPE_INST_LOAD_CABINET: { return new LSB_INST_LOAD_CABINET(_type, _script_ver, _header); }

		case TYPE_INST_MACRO_IFDEF: { return new LSB_INST_MACRO_IFDEF(_type, _script_ver, _header); }
		case TYPE_INST_MACRO_IFNDEF: { return new LSB_INST_MACRO_IFNDEF(_type, _script_ver, _header); }
		case TYPE_INST_MACRO_ENDIF: { return new LSB_INST_MACRO_ENDIF(_type, _script_ver, _header); }

		default: { return NULL; }
	}
}
//*** 경우별로 타입에 맞춰 할당 후 반환하는 함수
//---------------------------- 기본 클래스 함수 ----------------------------//




//---------------------------- 패러미터 리스트 클래스 ----------------------------//
void LSB_PARAMETERS::_read_parameter_list(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);

	_count = *(unsigned int*)lsb_buffer_pnt;
	lsb_buffer_pnt += sizeof(int);
	if (_count == 0) {
		(*_lsb_buffer_pnt) = lsb_buffer_pnt;
		return;
	}

	_list = (LSB_PARAMETER*)malloc(sizeof(LSB_PARAMETER) * _count);
	memset(_list, 0, (sizeof(LSB_PARAMETER) * _count));
	//*패러미터 수 파악 후 그만큼 할당
	//*만일 패러미터 수가 0이면 그냥 리턴한다

	char* _s;
	for (unsigned int i = 0; i < _count; i++) {
		_list[i]._type = (LSB_OPER_TYPES) * (lsb_buffer_pnt++);	//*** 패러미터 타입
		_s = _get_str(&lsb_buffer_pnt);
		_list[i]._name = GetUniStr (_s, code);			//*** 패러미터 이름
		free(_s);
		_list[i].arg_count = *(unsigned int*)lsb_buffer_pnt;	//*** 패러미터가 가진 인자의 수
		lsb_buffer_pnt += sizeof(int);
		_list[i].arg_list = (LM_SCR_ARG*)malloc(sizeof(LM_SCR_ARG) * _list[i].arg_count);
		memset(_list[i].arg_list, 0, (sizeof(LM_SCR_ARG) * _list[i].arg_count));
		//*인자 수만큼 읽어들이고 할당

		if (_list[i]._type == TYPE_ARG_CALC_FUNC) {
			_list[i]._calc_func = (LSB_CALC_FUNC_TYPES)(*lsb_buffer_pnt++);
		}
		else {
			_list[i]._calc_func = TYPE_CF_UNKNOWN;
		}
		//*패러미터 타입이 calc_func이라면 따로 읽어들인다
		//*일단은 기호가 잘못되면 안되는 처리는 할 수 있는데 그냥 진행

		for (unsigned int j = 0; j < _list[i].arg_count; j++) {
			_list[i].arg_list[j]._type = (LM_SCR_TYPES) * (lsb_buffer_pnt++);
			switch (_list[i].arg_list[j]._type)
			{
				case TYPE_ARG_INT:
				{
					_list[i].arg_list[j]._data = _get_int(&lsb_buffer_pnt);
					break;
					//*** 정수 처리
				}
				case TYPE_ARG_FLOAT: 
				{
					_list[i].arg_list[j]._data = _get_data(&lsb_buffer_pnt, 0xA);
					break;
					//*** 소수 처리, 공간을 0xA 크기만큼 잡아먹는다
				}
				case TYPE_ARG_BOOL:
				{
					_list[i].arg_list[j]._data = _get_data(&lsb_buffer_pnt, 0x1);
					break;
					//*** 논리형 처리, 공간을 0x1 크기만큼 잡아먹는다
				}
				case TYPE_ARG_STR:
				{
					_s = _get_str(&lsb_buffer_pnt);
					_list[i].arg_list[j]._data = GetUniStr (_s, code);
					free(_s);
					break;
					//*** 문자열 처리
				}
				default:
				{
					_s = _get_str(&lsb_buffer_pnt);
					_list[i].arg_list[j]._data = GetUniStr(_s, code);
					free(_s);
					break;
					//*** 이외의 경우(VAR 등)는 그냥 문자열 처리한다
				}
			}
			//*타입에 따라 처리를 나눈다
		}
		//*인자 수만큼 수행하기
	}
	//*패러미터 수만큼 읽어들이기

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*패러미터를 읽어들이는 함수


void LSB_PARAMETERS::_decompile_code(HANDLE hWrite, unsigned int code, unsigned int _indent)
{
	for (unsigned int i = 0; i < _count; i++) {

		for (unsigned int j = 0; j < (_indent); j++) {
			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_);
		}
		//*들여쓰기 출력

		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s:%s][_TYPE_:%s]"), 
			arg_name_prefix, (wchar_t*)_list[i]._name, _op_list[_list[i]._type]);
		//*인자 이름 출력

		if (_list[i]._type == TYPE_ARG_CALC_FUNC) {
			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[_CF_:%s]"), (wchar_t*)_cf_list[_list[i]._calc_func]);
		}
		//*CALC_FUNC일 경우 추가 출력

		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _enter_raw_);
		//*개행 마무리

		for (unsigned int j = 0; j < _list[i].arg_count; j++) {
			for (unsigned int j = 0; j < (_indent + 1); j++) {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_);
			}
			//*들여쓰기 출력

			switch (_list[i].arg_list[j]._type)
			{
				case TYPE_ARG_INT:
				{
					swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[_INT_:0x%08X]%s"), *(unsigned int*)(_list[i].arg_list[j]._data), _enter_raw_);
					break;
					//*** 정수 처리
				}
				case TYPE_ARG_FLOAT:
				{
					swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[_FLOAT_:%lf]%s"), *(double*)(_list[i].arg_list[j]._data), _enter_raw_);
					break;
					//*** 소수 처리, 공간을 0xA 크기만큼 잡아먹는다
				}
				case TYPE_ARG_BOOL:
				{
					if (*(bool*)_list[i].arg_list[j]._data) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[_BOOL_:True]%s"), _enter_raw_); }
					else { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[_BOOL_:False]%s"), _enter_raw_); }
					break;
					//*** 논리형 처리, 공간을 0x1 크기만큼 잡아먹는다
				}
				case TYPE_ARG_STR:
				default:
				{
					wchar_t* _n_w = Replace_Text((wchar_t*)_list[i].arg_list[j]._data, _enter_raw_, _enter_plained_);
					wchar_t* _n_w_2 = Replace_Text(_n_w, hex_1_raw, hex_1_plained); free(_n_w);
					if (_list[i].arg_list[j]._type == TYPE_ARG_STR) {
						swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[_STR_:%s]%s"), _n_w_2, _enter_raw_);
					}
					else {
						swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[_VAR_:%s]%s"), _n_w_2, _enter_raw_);
					}
					free(_n_w_2);
					break;
					//*** 문자열 처리 / 기타일 때도 마찬가지 / 교체 필요
				}
			}
			//*타입에 따라 나눠 출력
		}
	}
}
//*패러미터 리스트 클래스 : 핸들로 출력하는 함수

void LSB_PARAMETERS::_write_as_lsb(HANDLE hNewLsb, unsigned int code)
{
	_write_int(hNewLsb, _count);
	//*패러미터 수 기록

	char* _s;
	for (unsigned int i = 0; i < _count;i++) {

		_write_byte(hNewLsb, _list[i]._type);
		_s = GetAnsiStr(_list[i]._name, code);
		_write_str(hNewLsb, _s);
		free(_s);
		_write_int(hNewLsb, _list[i].arg_count);
		//*타입 / 이름 / 패러미터의 인자 수 기록

		if (_list[i]._type == TYPE_ARG_CALC_FUNC) {
			_write_byte(hNewLsb, _list[i]._calc_func);
		}
		//*패러미터 타입이 calc_func이라면 이 값도 추가로 기록한다

		for (unsigned int j = 0; j < _list[i].arg_count; j++) {
			_write_byte(hNewLsb, _list[i].arg_list[j]._type);
			//*인자의 타입 기록
			switch (_list[i].arg_list[j]._type)
			{
				case TYPE_ARG_INT:
				{
					_write_int(hNewLsb, *(unsigned int*)_list[i].arg_list[j]._data);
					break;
					//*** 정수 처리
				}
				case TYPE_ARG_FLOAT:
				{
					_write_data(hNewLsb, (unsigned char*)_list[i].arg_list[j]._data, 0xA);
					break;
					//*** 소수 처리, 공간을 0xA 크기만큼 잡아먹는다
				}
				case TYPE_ARG_BOOL:
				{
					_write_byte(hNewLsb, *(unsigned char*)_list[i].arg_list[j]._data);
					break;
					//*** 논리형 처리, 공간을 0x1 크기만큼 잡아먹는다
				}
				case TYPE_ARG_STR:
				default:
				{
					_s = GetAnsiStr((wchar_t*)_list[i].arg_list[j]._data, code);
					_write_str(hNewLsb, _s);
					free(_s);
					break;
					//*** 문자열 혹은 이외의 경우(VAR 등)는 그냥 문자열 처리한다
				}
			}
			//*타입에 따라 처리를 나눈다
		}
		//*인자 수만큼 기록하기

	}
	//*패러미터들을 돌아가며 기록
}
//*패러미터 리스트 : lsb 파일로 기록하는 함수


void LSB_PARAMETERS::_extract_text(HANDLE hTxt)
{
	for (unsigned int i = 0; i < _count; i++) {
		for (unsigned int j = 0; j < _list[i].arg_count; j++) {
			if ((_list[i].arg_list[j]._type != TYPE_ARG_INT)
				&& (_list[i].arg_list[j]._type != TYPE_ARG_FLOAT)
				&& (_list[i].arg_list[j]._type != TYPE_ARG_BOOL)) {

				wchar_t* _print_str = (wchar_t*)_list[i].arg_list[j]._data;
				if (is_Pure_Ascii_String(_print_str) || is_File_Path_String(_print_str)) { continue; }
				//*굳이 번역할 필요 없으면 출력하지 않음

				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[%s]%s%s%s%s"), 
					_indent_, text_data_prefix, text_prefix, (wchar_t*)_list[i].arg_list[j]._data, text_prefix, _enter_raw_);
				//*문자열 출력
				//*여기서 문자열 대치는 굳이 할 필요가 없는 듯 하다

			}
			//*인자 종류가 문자열일 때만 뽑아내도록 한다
			//*이름은 출력되는 게 아니므로 여기서는 건들지 않는다
		}
		//*각 패러미터의 인자 리스트 루프
	}
	//*각 패러미터 루프
}
//*패러미터 리스트 클래스 : 텍스트 추출 함수


void LSB_PARAMETERS::_replace_text(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int *accum_count)
{
	//*accum_count는 누적 인덱스가 담긴 값의 주소다

	for (unsigned int i = 0; i < _count; i++) {
		for (unsigned int j = 0; j < _list[i].arg_count; j++) {
			if ((_list[i].arg_list[j]._type != TYPE_ARG_INT)
				&& (_list[i].arg_list[j]._type != TYPE_ARG_FLOAT)
				&& (_list[i].arg_list[j]._type != TYPE_ARG_BOOL)) {

				if (is_Pure_Ascii_String((wchar_t*)_list[i].arg_list[j]._data)
					|| is_File_Path_String((wchar_t*)_list[i].arg_list[j]._data)) { continue; }

				if (_list[i].arg_list[j]._data != NULL) { free(_list[i].arg_list[j]._data); }
				//*이미 데이터가 있으면 해제함

				wchar_t* cp_txt = txt_list_elem->_txt_list_per_inst[(*accum_count)++];
				wchar_t* n_txt = (wchar_t*)malloc (sizeof(wchar_t) * (wcslen(cp_txt) + 1));
				n_txt[wcslen(cp_txt)] = 0;
				memcpy (n_txt, cp_txt, (sizeof(wchar_t) * wcslen(cp_txt)));
				_list[i].arg_list[j]._data = n_txt;
				//*메모리 복사 후 대치

			}
			//*인자 종류가 문자열일 때만 대치하도록 한다
			//*이름은 출력되는 텍스트가 아니므로 여기서는 건들지 않는다
		}
		//*각 패러미터의 인자 리스트 루프
	}
	//*각 패러미터 루프
}
//*패러미터 리스트 클래스 : 텍스트 교체 함수


const wchar_t* _part_j_font = TEXT("ＭＳ");
const wchar_t* _part_j_font_2 = TEXT("07やさしさゴシック");
const wchar_t* _n_font = TEXT("HY태고딕");

void LSB_PARAMETERS::_change_code_J2K()
{
	for (unsigned int i = 0; i < _count; i++) {

		JPHan_2_KRHan (_list[i]._name);
		//*이름의 언어코드 바꾸기

		for (unsigned int j = 0; j < _list[i].arg_count; j++) {
			if ((_list[i].arg_list[j]._type != TYPE_ARG_INT)
				&& (_list[i].arg_list[j]._type != TYPE_ARG_FLOAT)
				&& (_list[i].arg_list[j]._type != TYPE_ARG_BOOL)) {
				JPHan_2_KRHan((wchar_t*)_list[i].arg_list[j]._data);
				wchar_t* _chk = (wchar_t*)_list[i].arg_list[j]._data;
				if ((wcsstr(_chk, _part_j_font) != NULL)
					|| (wcsstr(_chk, _part_j_font_2) != NULL)) {
					_chk = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(_n_font) + 1));
					memset(_chk, 0, (sizeof(wchar_t) * (wcslen(_n_font) + 1)));
					memcpy(_chk, _n_font, (sizeof(wchar_t) * wcslen(_n_font)));
					free(_list[i].arg_list[j]._data);
					_list[i].arg_list[j]._data = _chk;
				}
			}
			//*인자 종류가 문자열일 때만 코드를 바꾸도록 한다
		}
		//*각 패러미터의 인자 리스트 루프
	}
	//*각 패러미터 루프
}
//*패러미터 리스트 클래스 : 텍스트 코드 변경 함수


void LSB_PARAMETERS::_release()
{
	if (_list != NULL) {
		for (unsigned int i = 0; i < _count; i++) {
			for (unsigned int j = 0; j < _list[i].arg_count; j++) {
				if (_list[i].arg_list[j]._data != NULL) {
					free(_list[i].arg_list[j]._data);
				}
			}
			free(_list[i].arg_list);
		}
		free(_list); _list = NULL;
	}
}
//*패러미터를 해제하는 함수
//---------------------------- 패러미터 리스트 클래스 ----------------------------//




//---------------------------- 이벤트 관련 클래스 ----------------------------//
void LSB_EVENT::_read_event_script(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);

	unsigned int* _int;
	unsigned char* _c;
	char* __s;

	_int = _get_int(&lsb_buffer_pnt);
	unsigned int _ev_scr_size = *_int; free(_int);
	unsigned char* ev_scr_end_pnt = lsb_buffer_pnt + _ev_scr_size;
	//* 전체 길이 파악 / 끝 위치 세팅

	__s = (char*)_get_data(&lsb_buffer_pnt, 6);
	_tpword = GetUniStr (__s, code);
	free(__s);
	__s = (char*)_get_data(&lsb_buffer_pnt, 3);
	_ver = GetUniStr(__s, code);
	free(__s);
	_tpword_ver = wcstol(_ver, NULL, 10);
	//* 매직 바이트 및 버전 읽어들이기

	_int = _get_int(&lsb_buffer_pnt);
	_attribute_count = *_int; free(_int);
	//*변수의 개수 파악

	if (_attribute_count > 0) {
		_attribute_list = (LSB_EV_TXT_ATTRIBUTE**)malloc(sizeof(LSB_EV_TXT_ATTRIBUTE*) * _attribute_count);
		memset(_attribute_list, 0, (sizeof(LSB_EV_TXT_ATTRIBUTE*) * _attribute_count));
		for (unsigned int i = 0; i < _attribute_count; i++) {
			_attribute_list[i] = new LSB_EV_TXT_ATTRIBUTE (_tpword_ver);
			_attribute_list[i]->_read_var_data(&lsb_buffer_pnt, code);
		}
	}
	//*변수 개수만큼 할당하고 읽어들이기

	if (_tpword_ver >= 0x69) {
		_ev_header = new LSB_EVENT_HEADER();
		_ev_header->_read_event_header(&lsb_buffer_pnt, code);
	}
	//*버전에 따라 이벤트 헤더 블럭 읽기

	_int = _get_int(&lsb_buffer_pnt);
	_total_inst_count = *_int; free(_int);
	_several_inst_count = _total_inst_count;
	//*총 명령어 수 파악

	unsigned int* _chk_count_arr = NULL;
	if (_attribute_count > 0) {
		_chk_count_arr = (unsigned int*)malloc(sizeof(int) * _attribute_count);
		memset(_chk_count_arr, 0, (sizeof(int) * _attribute_count));
	}
	//*체크용

	_main_inst_list = new LSB_EVENT_ARG_LIST(_attribute_list, _attribute_count, _tpword_ver);
	//*버전 때문에 여기서 할당해야 할듯함

	LSB_EVENT_OP_INST* arg_elem = NULL;
	LSB_EV_OPCODE_TYPES _ev_op_type;
	unsigned int _txt_accum_idx = 0;
	unsigned int _several_count_chk = 0;
	unsigned int _char_op_count_chk = 0;
	for (unsigned int i = 0; i < _total_inst_count; i++) {

		_c = _get_byte(&lsb_buffer_pnt);
		_ev_op_type = (LSB_EV_OPCODE_TYPES)*_c;
		free(_c);
		//*명령어 타입 읽어들이기

		if ((_ev_op_type != TYPE_EV_OP_IMG) && (_ev_op_type != TYPE_EV_OP_ALIGN)) {
			_several_count_chk++;
		}
		//*무슨 조건인지는 잘 모르겠지만 아무튼 이 두 조건을 제외한 개수를 카운팅한다

		if (_ev_op_type == TYPE_EV_OP_CHAR) {
			if (arg_elem == NULL) { 
				arg_elem = new LSB_EVENT_OP_INST(_ev_op_type, _attribute_list, _attribute_count, _tpword_ver); 
			}
			//*arg_elem이 NULL이면 새로 할당해주기
			arg_elem->_read_ev_data(&lsb_buffer_pnt, _txt_accum_idx, code);
			//*데이터 읽어들이기
			if (_chk_count_arr != NULL) { _chk_count_arr[arg_elem->_last_idx]++; }
			//*체크용 반영
			_txt_accum_idx++;
			//*텍스트 인덱스 누적

			if (_tpword_ver >= 0x69) { _char_op_count_chk++; }
			//*버전에 따른 값 추가
		}
		//*명령어 타입이 char라면 계속 덧붙여주기

		else {
			_txt_accum_idx = 0;
			//*누적된 텍스트 카운트 초기화
			if (arg_elem != NULL) {
				_main_inst_list->Append(arg_elem);
			}
			//*만일 arg_elem이 NULL이 아니라면 그 값을 리스트에 추가해 준다
			arg_elem = new LSB_EVENT_OP_INST(_ev_op_type, _attribute_list, _attribute_count, _tpword_ver);
			arg_elem->_read_ev_data(&lsb_buffer_pnt, _txt_accum_idx, code);
			if ((_ev_op_type == TYPE_EV_OP_VAR) || (_ev_op_type == TYPE_EV_OP_HISTORY_CHAR)) {
				if (_chk_count_arr != NULL) { _chk_count_arr[arg_elem->_last_idx]++; }
				if (_tpword_ver >= 0x69) { _char_op_count_chk++; }
				//*버전에 따른 값 추가
			}
			//*체크용 반영
			_main_inst_list->Append(arg_elem);
			arg_elem = NULL;
			//*그리고 다시 할당한 후 읽어들이고 추가하고 NULL로 바꾸면 됨
		}
		//*다른 경우에는 일반적으로 수행하듯 수행
	}
	//*명령어 개수만큼 읽어들이기
	//*명령어 타입이 char일 때는 재할당이 아니라 계속 돌아가면서 읽어들인다

	if (arg_elem != NULL) { _main_inst_list->Append(arg_elem); }
	//*마지막으로 남았으면 그것도 추가함

	for (unsigned int i = 0; i < _attribute_count; i++) {
		if (_attribute_list[i]->_ev_op_ref_count != _chk_count_arr[i]) {
			wprintf(TEXT("인덱스 값의 매칭이 일치하지 않습니다.\n나중에 반영할 때 에러가 있을 수 있습니다.\n"));
		}
	}
	if (_tpword_ver >= 0x69) {
		if ((_several_count_chk != _ev_header->_several_op_count) 
			&& (_total_inst_count != _ev_header->_several_op_count)) {
			wprintf(TEXT("인덱스의 수가 일치하지 않습니다.\n나중에 반영할 때 에러가 있을 수 있습니다.\n"));
		}
		if (_ev_header->_elem_count != 0) {
			if (_char_op_count_chk != _ev_header->_elem_list[0]._ref_count) {
				wprintf(TEXT("인덱스의 수가 일치하지 않습니다.(0x%X, 0x%X)\n나중에 반영할 때 에러가 있을 수 있습니다.\n"),
					_char_op_count_chk, _ev_header->_elem_list[0]._ref_count);
			}
		}
	}
	//*기록한 배열값과 값이 어긋나면 메시지 띄우기

	if (_chk_count_arr != NULL) { free(_chk_count_arr); }
	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
	//*위치 변경 반영
}
//*이벤트 블럭을 읽어들이는 함수


void LSB_EVENT::_decompile_code(HANDLE hWrite, unsigned int code, unsigned int _indent)
{
	for (unsigned int i = 0; i < _indent; i++) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_); }
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s]%s"), event_attr_prefix, _enter_raw_);
	//*이벤트 변수 헤더 출력

	for (unsigned int i = 0; i < _attribute_count; i++) {
		_attribute_list[i]->_decompile_code(hWrite, code, _indent + 1);
	}
	//*이벤트 내부 변수들 돌아가면서 출력

	if (_tpword_ver>= 0x69) {
		_ev_header->_decompile_code(hWrite, code, _indent);
	}
	//*이벤트 헤더 블럭 출력

	for (unsigned int i = 0; i < _indent; i++) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_); }
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s]%s"), event_inst_prefix, _enter_raw_);
	//*이벤트 명령어 헤더 헤더 출력

	LSB_EVENT_OP_INST* _eapt = _main_inst_list->_dummy_start->_next;
	for (unsigned int i = 0; i < _main_inst_list->_count; i++) {
		_eapt->_decompile_code(hWrite, code, _indent + 1);
		_eapt = _eapt->_next;
	}
	//*명령어들 돌아가면서 출력
}
//*이벤트 클래스 : 핸들로 출력하는 함수


void LSB_EVENT::_recompile_from_txt(wchar_t* _code_txt_buffer, unsigned int code)
{
	//*** _tpword, _ver는 이전 버전에서 가져왔다 가정함

	wchar_t* _code_txt_pnt = _code_txt_buffer, * _code_txt_pnt_2 = _code_txt_buffer;
	wchar_t* _tm_cpy = NULL;
	//*텍스트를 얻기 위한 변수

	_attribute_count = Get_Txt_Count(_code_txt_buffer, refer_attr_prefix, wcslen(_code_txt_buffer));
	wchar_t* _attr_end_pnt = NULL;
	if (_tpword_ver >= 0x69) {
		_attr_end_pnt = wcsstr(_code_txt_buffer, event_header_prefix);
	}
	else {
		_attr_end_pnt = wcsstr(_code_txt_buffer, event_inst_prefix);
	}
	//*경우에 따라 끝지점을 다르게 세팅한다

	if (_attribute_count != 0) {
		_attribute_list = (LSB_EV_TXT_ATTRIBUTE**)malloc(sizeof(LSB_EV_TXT_ATTRIBUTE*) * _attribute_count);
		memset(_attribute_list, 0, (sizeof(LSB_EV_TXT_ATTRIBUTE*) * _attribute_count));
		for (unsigned int i = 0; i < _attribute_count;i++) {
			_attribute_list[i] = new LSB_EV_TXT_ATTRIBUTE(_tpword_ver);
			//*초기화
			_code_txt_pnt_2 = wcsstr(_code_txt_pnt, refer_attr_prefix) + wcslen(refer_attr_prefix);
			_code_txt_pnt = wcsstr(_code_txt_pnt_2, refer_attr_prefix);
			if (_code_txt_pnt == NULL) { _code_txt_pnt = _attr_end_pnt; }
			_tm_cpy = (wchar_t*)malloc(sizeof(wchar_t) * (_code_txt_pnt - _code_txt_pnt_2 + 1));
			memset(_tm_cpy, 0, (sizeof(wchar_t) * (_code_txt_pnt - _code_txt_pnt_2 + 1)));
			memcpy(_tm_cpy, _code_txt_pnt_2, (sizeof(wchar_t) * (_code_txt_pnt - _code_txt_pnt_2)));
			//*필요한 텍스트만 복사해오기
			_attribute_list[i]->_recompile_from_txt(_tm_cpy);
			//*속성값 읽어들이기
			free(_tm_cpy);
			//*다음을 위한 준비
		}
	}
	//*이벤트 내부 속성 수 파악 / 할당 / 초기화(0이 아닐 때만)
	//*이벤트 내부 속성 읽어들이기(내부 명령어 할당 시 필요함)

	if (_tpword_ver >= 0x69) {
		_code_txt_pnt = wcsstr(_code_txt_buffer, event_header_prefix);
		_ev_header = new LSB_EVENT_HEADER();
		_ev_header->_recompile_from_txt(_code_txt_pnt, code);
		if (_ev_header->_elem_count != 0) {
			_ev_header->_elem_list[0]._ref_count = 0;
		}
	}
	//*만일 원래 버전이 105라면 헤더블럭 할당을 하고 읽어와야 함
	//*그리고 새로 컴파일을 하는 것이므로 일반 문자에 대한 참조값은 다시 계산해야 한다

	_main_inst_list = new LSB_EVENT_ARG_LIST(_attribute_list, _attribute_count, _tpword_ver);
	_code_txt_pnt = wcsstr(_code_txt_buffer, event_inst_prefix);
	_total_inst_count = Get_Txt_Count(_code_txt_pnt, event_op_type_prefix, wcslen(_code_txt_pnt));
	//*명령어 돌입 전 초기화 및 위치 세팅

	wchar_t ss_num[3]; memset(ss_num, 0, sizeof(wchar_t) * 3);
	if (_total_inst_count != 0) {
		for (unsigned int i = 0; i < _total_inst_count; i++) {
			_code_txt_pnt_2 = wcsstr(_code_txt_pnt, event_op_type_prefix) + wcslen(event_op_type_prefix);
			memcpy(ss_num, &_code_txt_pnt_2[1], sizeof(wchar_t) * 2);
			LSB_EV_OPCODE_TYPES _ev_op_tp = (LSB_EV_OPCODE_TYPES) wcstol (ss_num, NULL, 10);
			//*이벤트 명령어 타입 읽어들이기
			LSB_EVENT_OP_INST* _ttp = new LSB_EVENT_OP_INST(_ev_op_tp, _attribute_list, _attribute_count, _tpword_ver);
			//*초기화
			
			_code_txt_pnt = wcsstr(_code_txt_pnt_2, event_op_type_prefix);
			if (_code_txt_pnt == NULL) { _code_txt_pnt = _code_txt_buffer + wcslen(_code_txt_buffer); }
			_tm_cpy = (wchar_t*)malloc(sizeof(wchar_t) * (_code_txt_pnt - _code_txt_pnt_2 + 1));
			memset(_tm_cpy, 0, (sizeof(wchar_t) * (_code_txt_pnt - _code_txt_pnt_2 + 1)));
			memcpy(_tm_cpy, _code_txt_pnt_2, (sizeof(wchar_t) * (_code_txt_pnt - _code_txt_pnt_2)));
			//*필요한 텍스트만 복사해오기
			_ttp->_recompile_from_txt(_tm_cpy, code);
			//*각 명령어에 해당하는 정보 읽어들이기
			_main_inst_list->Append(_ttp);
			//*리스트에 추가
			free(_tm_cpy);
			//*다음을 위한 준비

			if (_tpword_ver >= 0x69) {
				if (_ev_op_tp == TYPE_EV_OP_CHAR)
				{
					_several_inst_count += wcslen(_ttp->_str);
					_ev_header->_several_op_count += wcslen(_ttp->_str);
					if (_ev_header->_elem_count != 0) { _ev_header->_elem_list[0]._ref_count += wcslen(_ttp->_str); }
					//*참조값을 추가할 때 이것도 신경써야 함
				}
				else if ((_ev_op_tp == TYPE_EV_OP_VAR) || (_ev_op_tp == TYPE_EV_OP_HISTORY_CHAR))
				{
					_several_inst_count++;
					_ev_header->_several_op_count++;
				}
				else { 
					_several_inst_count++; 
					if ((_ev_op_tp != TYPE_EV_OP_IMG) && (_ev_op_tp != TYPE_EV_OP_ALIGN)) {
						_ev_header->_several_op_count++;
					}
					//*무슨 조건인지는 모르겠지만 이래야 맞는듯 하다
				}
			}
			//*버전에 따른 값 추가
		}
		//*딱 필요한 텍스트 부분만 복사해서 넘겨주기
		//*각 개수만큼 원소 할당해서 읽어들인 후 추가해주면 된다
	}
	//*이벤트 내부 명령어 수 파악 / 할당 / 초기화(0이 아닐 때만)
	//*이벤트 내부 명령어 읽어들이기
}
//*이벤트 클래스 : 리컴파일 함수 (인자 : 해당 이벤트 블럭의 데이터가 들어있는 텍스트 데이터 일부)


void LSB_EVENT::_write_as_lsb(HANDLE hNewLsb, unsigned int code)
{
	char* __s;

	unsigned int _total_ev_block_size_ofs = SetFilePointer(hNewLsb, 0, NULL, FILE_CURRENT);
	SetFilePointer(hNewLsb, sizeof(int), NULL, FILE_CURRENT);
	//*일단 전체 길이는 나중에 알 수 있으니 위치만 기억하고 넘어가기

	__s = GetAnsiStr(_tpword, code);
	_write_data(hNewLsb, (unsigned char*)__s, strlen(__s));
	free(__s);
	__s = GetAnsiStr(_ver, code);
	_write_data(hNewLsb, (unsigned char*)__s, strlen(__s));
	free(__s);
	//*매직 바이트 / 버전 기록

	_write_int(hNewLsb, _attribute_count);
	//*변수 개수 기록

	for (unsigned int i = 0; i < _attribute_count; i++) {
		_attribute_list[i]->_write_as_lsb(hNewLsb, code);
	}
	//*변수 데이터 기록

	if (_tpword_ver >= 0x69) {
		_ev_header->_write_as_lsb(hNewLsb, code);
	}
	//***버전에 따라 헤더블럭도 기록

	unsigned int _total_inst_count_ofs = SetFilePointer(hNewLsb, 0, NULL, FILE_CURRENT);
	SetFilePointer(hNewLsb, sizeof(int), NULL, FILE_CURRENT);
	//*총 명령어 수도 다시 계산해야 하므로 위치만 기억하고 넘어가기

	unsigned int _total_inst_count = 0;
	LSB_EVENT_OP_INST* _pnt = _main_inst_list->_dummy_start->_next;
	for (unsigned int i = 0; i < _main_inst_list->_count; i++) {
		_pnt->_write_as_lsb(hNewLsb, code, &_total_inst_count);
		_pnt = _pnt->_next;
	}
	//*리스트의 크기만큼 돌아가면서 총 명령어 수 계산
	
	unsigned int _now_ofs = SetFilePointer(hNewLsb, 0, NULL, FILE_CURRENT);
	unsigned int _total_ev_block_size = _now_ofs - (_total_ev_block_size_ofs + sizeof(int));
	//*처리가 완료됐으면 전체 크기 계산

	SetFilePointer(hNewLsb, _total_inst_count_ofs, NULL, FILE_BEGIN);
	_write_int(hNewLsb, _total_inst_count);
	SetFilePointer(hNewLsb, _total_ev_block_size_ofs, NULL, FILE_BEGIN);
	_write_int(hNewLsb, _total_ev_block_size);
	SetFilePointer(hNewLsb, _now_ofs, NULL, FILE_BEGIN);
	//*각 위치에 기록하고 핸들 위치 옮김
}
//*이벤트 클래스 : lsb 파일로 기록하는 함수


void LSB_EVENT::_extract_text(HANDLE hTxt)
{
	for (unsigned int i = 0; i < 1; i++) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_); }
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s]%s"), event_attr_prefix, _enter_raw_);
	//*이벤트 변수 헤더 출력, 들여쓰기 1번

	for (unsigned int i = 0; i < _attribute_count; i++) {
		_attribute_list[i]->_extract_text(hTxt);
	}
	//*각 변수도 추출해 준다

	if (_tpword_ver >= 0x69) {
		_ev_header->_extract_text(hTxt);
	}
	//*버전에 따라 헤더 블럭을 추출한다

	for (unsigned int i = 0; i < 1; i++) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_); }
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s]%s"), event_inst_prefix, _enter_raw_);
	//*이벤트 명령어 헤더 헤더 출력, 들여쓰기 1번

	LSB_EVENT_OP_INST* _elem_pnt = _main_inst_list->_dummy_start->_next;
	for (unsigned int i = 0; i < _main_inst_list->_count; i++) {
		_elem_pnt->_extract_text(hTxt);
		//*텍스트 추출
		_elem_pnt = _elem_pnt->_next;
		//*리스트 포인터 이동
	}
	//*현재 리스트에 등록된 수만큼 추출
}
//*이벤트 클래스 : 텍스트 추출 함수


void LSB_EVENT::_replace_text(LSB_TEXTS_PER_INST* txt_list_elem)
{
	//*** 이벤트 블럭만큼은 아예 새로 만들어야 할 수준인 것 같음 ***//
	//*** 그래야 스크립트를 자유롭게 지우고 추가할 수 있음 ***//
	//*** 다행히도 text_data_prefix는 전혀 들어가지 않으니 새로 만들기 수월할 듯 하다 ***//
	//*** 즉 이 함수는 안쓰인다 ***//
}
//*이벤트 클래스 : 텍스트 교체 함수


void LSB_EVENT::_change_code_J2K()
{
	for (unsigned int i = 0; i < _attribute_count; i++) {
		_attribute_list[i]->_change_code_J2K();
	}
	//*이벤트 변수 : 현재 리스트에 등록된 수만큼 바꾸기

	if (_tpword_ver >= 0x69) {
		_ev_header->_change_code_J2K();
	}
	//*이벤트 헤더블럭은 버전에 따라 바꾸기

	LSB_EVENT_OP_INST* _elem_pnt = _main_inst_list->_dummy_start->_next;
	for (unsigned int i = 0; i < _main_inst_list->_count; i++) {
		_elem_pnt->_change_code_J2K();
		_elem_pnt = _elem_pnt->_next;
		//*리스트 포인터 바꿔가며 바꾸기
	}
	//*이벤트 명령어 : 현재 리스트에 등록된 수만큼 바꾸기
}
//*이벤트 클래스 : 텍스트 코드 변경 함수


#define VARIABLES_DECLARE(_code_txt_buffer_) \
	wchar_t* _code_txt_pnt = (_code_txt_buffer), * _code_txt_pnt_2 = (_code_txt_buffer), * _swp;\
	wchar_t _buf_for_int[11];\
	wchar_t _buf_for_byte[5];\
	memset(_buf_for_int, 0, (sizeof(wchar_t) * 11));\
	memset(_buf_for_byte, 0, (sizeof(wchar_t) * 5));\
	wchar_t * _w_str_2, * _ww_str_2;
//*변수 선언 매크로

#define GET_INT_FROM_TEXT(_int_) \
	_code_txt_pnt_2 = wcsstr(_code_txt_pnt, TEXT("[")) + wcslen(TEXT("["));\
	_w_str_2 = (wchar_t*)malloc(sizeof(wchar_t) * 11);\
	memset(_w_str_2, 0, (sizeof(wchar_t) * 11));\
	memcpy(_w_str_2, _code_txt_pnt_2, (sizeof(wchar_t) * 10));\
	(_int_) = wcstoul(_w_str_2, NULL, 16); free(_w_str_2);\
	_swp = _code_txt_pnt; _code_txt_pnt = _code_txt_pnt_2; _code_txt_pnt_2 = _swp;
//*정수 얻는 매크로

#define GET_DEF_IDX_FROM_TEXT(_def_idx_) \
	_code_txt_pnt_2 = wcsstr(_code_txt_pnt, ev_attr_def_idx_prefix) + wcslen(ev_attr_def_idx_prefix) + wcslen(TEXT(":"));\
	_w_str_2 = (wchar_t*)malloc(sizeof(wchar_t) * 11);\
	memset(_w_str_2, 0, (sizeof(wchar_t) * 11));\
	memcpy(_w_str_2, _code_txt_pnt_2, (sizeof(wchar_t) * 10));\
	(_def_idx_) = wcstoul(_w_str_2, NULL, 16); free(_w_str_2);
//*디폴트 인덱스 얻는 매크로

#define GET_BYTE_FROM_TEXT(_byte_) \
	_code_txt_pnt_2 = wcsstr(_code_txt_pnt, TEXT("[")) + wcslen(TEXT("["));\
	_w_str_2 = (wchar_t*)malloc(sizeof(wchar_t) * 5);\
	memcpy(_w_str_2, _code_txt_pnt_2, (sizeof(wchar_t) * 4));\
	(_byte_) = (unsigned char)wcstoul(_w_str_2, NULL, 16); free(_w_str_2);\
	_swp = _code_txt_pnt; _code_txt_pnt = _code_txt_pnt_2; _code_txt_pnt_2 = _swp;
//*바이트 얻는 매크로

#define GET_STR_FROM_TEXT(_str_) \
	_code_txt_pnt_2 = wcsstr(_code_txt_pnt, text_prefix) + wcslen(text_prefix);\
	_code_txt_pnt = wcsstr(_code_txt_pnt_2, text_prefix);\
	(_str_) = (wchar_t*)malloc(sizeof(wchar_t) * (_code_txt_pnt - _code_txt_pnt_2 + 1));\
	memset((_str_), 0, (sizeof(wchar_t)* (_code_txt_pnt - _code_txt_pnt_2 + 1)));\
	memcpy((_str_), _code_txt_pnt_2, (sizeof(wchar_t)* (_code_txt_pnt - _code_txt_pnt_2)));\
	_code_txt_pnt += wcslen(text_prefix);
//*문자열 얻는 매크로

#define GET_IDX_FROM_TEXT(_idx_) \
	_code_txt_pnt_2 = wcsstr(_code_txt_pnt, ev_attr_idx_prefix) + wcslen(ev_attr_idx_prefix) + wcslen(TEXT(":"));\
	_w_str_2 = (wchar_t*)malloc(sizeof(wchar_t) * 11);\
	memset(_w_str_2, 0, (sizeof(wchar_t) * 11));\
	memcpy(_w_str_2, _code_txt_pnt_2, (sizeof(wchar_t) * 10));\
	(_idx_) = wcstoul(_w_str_2, NULL, 16); free(_w_str_2);
//*인덱스 얻는 매크로

void LSB_EVENT_HEADER::_read_event_header(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);

	char* _s;

	_type = *(unsigned int*)lsb_buffer_pnt;
	lsb_buffer_pnt += sizeof(int);
	//*타입 먼저 읽기

	_several_op_count = *(unsigned int*)lsb_buffer_pnt;
	lsb_buffer_pnt += sizeof(int);
	_unk_1 = *(unsigned int*)lsb_buffer_pnt;
	lsb_buffer_pnt += sizeof(int);
	//*공통적인 값들 읽기

	if (_type > 1) {
		_unk_2 = *(unsigned int*)lsb_buffer_pnt;
		lsb_buffer_pnt += sizeof(int);
		_s = _get_str(&lsb_buffer_pnt);
		_str = GetUniStr(_s, code); free(_s);
	}
	//*타입이 2일때 활성화되는 값 읽기

	_elem_count = *(unsigned int*)lsb_buffer_pnt;
	lsb_buffer_pnt += sizeof(int);
	//*원소 수 읽기

	if (_elem_count != 0) {
		_elem_list = (LSB_EVH_ELEM*)malloc(sizeof(LSB_EVH_ELEM) * _elem_count);
		memset(_elem_list, 0, (sizeof(LSB_EVH_ELEM) * _elem_count));
		for (unsigned int i = 0; i < _elem_count;i++) {
			_elem_list[i]._ref_count = *(unsigned int*)lsb_buffer_pnt;
			lsb_buffer_pnt += sizeof(int);
			_s = _get_str(&lsb_buffer_pnt);
			_elem_list[i]._str = GetUniStr(_s, code); free(_s);
			_elem_list[i]._unk_2 = *(unsigned int*)lsb_buffer_pnt;
			lsb_buffer_pnt += sizeof(int);
		}
	}
	//*원소 수에 따라 할당하고 읽어들이기

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*이벤트 헤더 클래스 : 읽어들이는 함수


void LSB_EVENT_HEADER::_decompile_code(HANDLE hWrite, unsigned int code, unsigned int _indent)
{
	for (unsigned int i = 0; i < _indent; i++) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_); }
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s][0x%08X][0x%08X]"), event_header_prefix, _type, _unk_1);
	if (_type > 1) {
		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[0x%08X]%s%s%s"), _unk_2, text_prefix, _str, text_prefix);
	}
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _enter_raw_);
	//*이벤트 헤더 프리픽스 출력, 버전이 어쨌든 상관없이 일단 출력한다
	//*다만 여기서 개수들 만큼은 제외함, _char_op_count는 나중에 합칠때 달라지며 _elem_count는 특정 키워드 수를 세기만 하면 됨
	//*type에 따라 _unk_2와 _str은 구분해서 기록한다

	wchar_t* _tp_uni;
	for (unsigned int i = 0; i < _elem_count;i++) {
		for (unsigned int j = 0; j < (_indent + 1); j++) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_); }
		//*들여쓰기
		_tp_uni = Replace_Text(_elem_list[i]._str, _enter_raw_, _enter_plained_);
		//*문자열 대치 필요
		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s]::[0x%08X][0x%08X]%s%s%s%s"),
			event_head_elem_prefix,
			_elem_list[i]._ref_count, _elem_list[i]._unk_2, text_prefix, _tp_uni, text_prefix,
			_enter_raw_);
		free(_tp_uni);
		//*각 요소들 출력
	}
	//*개수에 따라 이벤트 다 돌려가면서 출력
}
//*이벤트 헤더 클래스 : 디컴파일 함수


void LSB_EVENT_HEADER::_recompile_from_txt(wchar_t* _code_txt_buffer, unsigned int code)
{
	VARIABLES_DECLARE(_code_txt_buffer_);

	_elem_count = Get_Txt_Count(_code_txt_buffer, event_head_elem_prefix, wcslen(_code_txt_buffer));
	//*원소 수 세기

	GET_INT_FROM_TEXT(_type);
	GET_INT_FROM_TEXT(_unk_1);
	if (_type > 1) {
		GET_INT_FROM_TEXT(_unk_2);
		GET_STR_FROM_TEXT(_str);
	}
	//*필요한 값들 얻어 세팅하기, _type에 따라 추가로 얻을 값 구분하기

	wchar_t* _inst_start_pnt = wcsstr(_code_txt_buffer, event_inst_prefix);
	wchar_t* _tm_cpy = NULL;
	wchar_t* _tmp_uni;
	//*헤더의 끝지점이자 명령어 시작지점 확인

	if (_elem_count != 0) {
		_elem_list = (LSB_EVH_ELEM*)malloc(sizeof(LSB_EVH_ELEM) * _elem_count);
		memset(_elem_list, 0, (sizeof(LSB_EVH_ELEM) * _elem_count));
		for (unsigned int i = 0; i < _elem_count; i++) {
			_code_txt_pnt_2 = wcsstr(_code_txt_pnt, event_head_elem_prefix) + wcslen(event_head_elem_prefix);
			_code_txt_pnt = wcsstr(_code_txt_pnt_2, event_head_elem_prefix);
			if (_code_txt_pnt == NULL) { _code_txt_pnt = _inst_start_pnt; }
			_tm_cpy = (wchar_t*)malloc(sizeof(wchar_t) * (_code_txt_pnt - _code_txt_pnt_2 + 1));
			memset(_tm_cpy, 0, (sizeof(wchar_t) * (_code_txt_pnt - _code_txt_pnt_2 + 1)));
			memcpy(_tm_cpy, _code_txt_pnt_2, (sizeof(wchar_t) * (_code_txt_pnt - _code_txt_pnt_2)));
			//*필요한 텍스트만 복사해오기

			_elem_list[i]._recompile_from_txt(_tm_cpy, code);
			_tmp_uni = Replace_Text(_elem_list[i]._str, _enter_plained_, _enter_raw_);
			free(_elem_list[i]._str);
			_elem_list[i]._str = _tmp_uni;
			//*필요한 값들 읽어들이기, 문자열은 대치 필요

			free(_tm_cpy);
			//*다음을 위한 준비
		}
	}
	//*이벤트 내부 속성 수 파악 / 할당 / 초기화(0이 아닐 때만)
	//*이벤트 내부 속성 읽어들이기(내부 명령어 할당 시 필요함)
}
//*이벤트 헤더 클래스 : 리컴파일 함수


void LSB_EVENT_HEADER::_write_as_lsb(HANDLE hNewLsb, unsigned int code)
{
	char* _s;

	_write_int(hNewLsb, _type);
	//*타입 기록

	_write_int(hNewLsb, _several_op_count);
	_write_int(hNewLsb, _unk_1);
	//*공통적인 값들 기록

	if (_type > 1) {
		_write_int(hNewLsb, _unk_2);
		_s = GetAnsiStr(_str, code);
		_write_str(hNewLsb, _s); free(_s);
	}
	//*타입에 따라 나머지 값 기록

	_write_int(hNewLsb, _elem_count);
	//*마지막 공통값(원소 수) 기록

	for (unsigned int i = 0; i < _elem_count;i++) {
		_write_int(hNewLsb, _elem_list[i]._ref_count);
		_s = GetAnsiStr(_elem_list[i]._str, code);
		_write_str(hNewLsb, _s); free(_s);
		_write_int(hNewLsb, _elem_list[i]._unk_2);
	}
	//*원소 수에 따라 추가 기록
}
//*이벤트 헤더 클래스 : lsb 기록 함수


void LSB_EVENT_HEADER::_extract_text(HANDLE hTxt)
{
	for (unsigned int i = 0; i < 1; i++) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_); }
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s][0x%08X][0x%08X]"), event_header_prefix, _type, _unk_1);
	if (_type > 1) {
		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[0x%08X]%s%s%s"), _unk_2, text_prefix, _str, text_prefix);
	}
	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _enter_raw_);
	//*이벤트 헤더 프리픽스 출력, 버전이 어쨌든 상관없이 일단 출력한다
	//*다만 여기서 개수들 만큼은 제외함, _char_op_count는 나중에 합칠때 달라지며 _elem_count는 특정 키워드 수를 세기만 하면 됨
	//*type에 따라 _unk_2와 _str은 구분해서 기록한다

	wchar_t* _tp_uni;
	for (unsigned int i = 0; i < _elem_count; i++) {
		for (unsigned int j = 0; j < 2; j++) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_); }
		//*들여쓰기
		_tp_uni = Replace_Text(_elem_list[i]._str, _enter_raw_, _enter_plained_);
		//*문자열 대치 필요
		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s]::[0x%08X][0x%08X]%s%s%s%s"),
			event_head_elem_prefix,
			_elem_list[i]._ref_count, _elem_list[i]._unk_2, text_prefix, _tp_uni, text_prefix,
			_enter_raw_);
		free(_tp_uni);
		//*각 요소들 출력
	}
	//*개수에 따라 이벤트 다 돌려가면서 출력
}
//*이벤트 헤더 클래스 : 텍스트 추출 함수


void LSB_EVENT_HEADER::_change_code_J2K()
{
	JPHan_2_KRHan(_str);
	//*문자열 변환

	for (unsigned int i = 0; i < _elem_count; i++) {
		JPHan_2_KRHan(_elem_list[i]._str);
	}
	//*각 원소의 문자열 변환
}
//*이벤트 헤더 클래스 : 텍스트 코드 변경 함수


void LSB_EVH_ELEM::_recompile_from_txt (wchar_t* _code_txt_buffer, unsigned int code)
{
	VARIABLES_DECLARE(_code_txt_buffer_);

	GET_INT_FROM_TEXT(_ref_count);
	GET_STR_FROM_TEXT(_str);
	GET_INT_FROM_TEXT(_unk_2);
	//*값들을 얻어주기만 하면 된다
}
//*이벤트 헤더 클래스의 원소 구조체 : 텍스트 코드 변경 함수


void LSB_EVENT_OP_INST::_read_ev_data(unsigned char** _lsb_buffer_pnt, unsigned int txt_accum_idx, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);

	if (_tpword_ver >= 0x69) {
		_unk_1 = *(unsigned int*)lsb_buffer_pnt;
		lsb_buffer_pnt += sizeof(int);
	}
	//***tpword 버전이 오르면 정수값인지 뭔지 하나 더 읽어야함
	//*이게 아마 개수일 가능성이 있음, 이게 있냐 없냐에 따라 뭔가를 더 읽는 듯한 느낌이 남

	char* __s;
	switch (_op_type)
	{
		case TYPE_EV_OP_CHAR:
		{
			_read_char_data(&lsb_buffer_pnt, txt_accum_idx, code);
			break;
			//* TYPE_EV_OP_CHAR일 때 : _read_char_data 호출 (_i_3, _extra_data, _str, _i) 이용
		}
		case TYPE_EV_OP_ALIGN:
		case TYPE_EV_OP_RETURN:
		{
			if (_unk_1 != 0) {
				_unk_t2_t3._unk_1 = *(unsigned char*)lsb_buffer_pnt;
				lsb_buffer_pnt++;
				_unk_t2_t3._unk_2 = *(unsigned int*)lsb_buffer_pnt;
				lsb_buffer_pnt += sizeof(int);
				_unk_t2_t3._unk_3 = *(unsigned int*)lsb_buffer_pnt;
				lsb_buffer_pnt += sizeof(int);
			}
			//*일단 뭐가 뭔지 모르겠으니까 읽어들여 둔다
			unsigned char* _n_b = _get_byte(&lsb_buffer_pnt);
			_b = *_n_b; free(_n_b);
			break;
			//* TYPE_EV_OP_ALIGN / TYPE_EV_OP_RETURN일 때 : _unk_t2_t3, _b 이용
		}
		case TYPE_EV_OP_INDENT:
		case TYPE_EV_OP_UNDENT:
		{
			break;
			//* TYPE_EV_OP_INDENT / TYPE_EV_OP_UNDENT일 때 : 이용하지 않음
		}
		case TYPE_EV_OP_EVENT:
		{
			if (_str != NULL) { free(_str); }
			__s = _get_str(&lsb_buffer_pnt);
			//*문자열 얻기

			_decode_sharp_encoding (&__s);
			_str = GetUniStr(__s, code);
			free(__s);
			//*유니코드화하기 전에 # + 숫자 3개로 된 문자값을 싹 모아 ansi 문자열로 바꿔 대치해줘야 됨

			//*** 나중에 기록할 때 문자열 대치해야 함 _str
			break;
			//* TYPE_EV_OP_EVENT일 때 : _str 이용 (대치 필요)
		}
		case TYPE_EV_OP_VAR:
		{
			if (_tpword_ver < 0x65) {
				if (_str != NULL) { free(_str); }
				__s = _get_str(&lsb_buffer_pnt);
				_str = GetUniStr (__s, code);
				free(__s);
				unsigned int* _n_i = _get_int(&lsb_buffer_pnt);
				_last_idx = _default_attr_idx = *_n_i; free(_n_i);
			}
			else {
				unsigned int* _n_i = _get_int(&lsb_buffer_pnt);
				_last_idx = _default_attr_idx = *_n_i; free(_n_i);
				_n_i = _get_int(&lsb_buffer_pnt);
				_i_2 = *_n_i; free(_n_i);
				if (_s != NULL) { free(_s); }
				__s = _get_str(&lsb_buffer_pnt);
				_s = GetUniStr(__s, code);
				free(__s);
				if (_tpword_ver >= 0x66) {
					if (_str != NULL) { free(_str); }
					__s = _get_str(&lsb_buffer_pnt);
					_str = GetUniStr(__s, code);
					free(__s);
				}
			}
			_ev_attr_idx_ref_list[_default_attr_idx]++;
			//*참조 횟수 반영
			break;
			//* TYPE_EV_OP_VAR일 때 : ver < 0x65[_str, _i 이용], 0x65 <= ver[_i, _i_2, _s 이용], 0x66 <= ver[_i, _i_2, _s, _str 이용]
		}
		case TYPE_EV_OP_IMG:
		{
			unsigned int* _n_i = _get_int(&lsb_buffer_pnt);
			_i_2 = *_n_i; free(_n_i);
			_n_i = _get_int(&lsb_buffer_pnt);
			_extra_data = *_n_i; free(_n_i);
			if (_str != NULL) { free(_str); }
			__s = _get_str(&lsb_buffer_pnt);
			_str = GetUniStr(__s, code);
			free(__s);
			unsigned char* _n_b = _get_byte(&lsb_buffer_pnt);
			_b = *_n_b; free(_n_b);
			//*참조 횟수 반영
			if (_tpword_ver >= 0x69) {
				for (unsigned int i = 0; i < 6;i++) {
					_unk_i_list[i] = *(unsigned int*)lsb_buffer_pnt;
					lsb_buffer_pnt += sizeof(int);
				}
			}
			//***버전에 따라 0x18바이트 더 더해야하는듯?
			break;
			//* TYPE_EV_OP_IMG일 때 : _i_2, _extra_data, _str, _b 이용
		}
		case TYPE_EV_OP_HISTORY_CHAR:
		{
			unsigned int* _n_i = _get_int(&lsb_buffer_pnt);
			_i_3 = *_n_i; free(_n_i);
			_n_i = _get_int(&lsb_buffer_pnt);
			_i_2 = *_n_i; free(_n_i);
			_n_i = _get_int(&lsb_buffer_pnt);
			_last_idx = _default_attr_idx = *_n_i; free(_n_i);
			if (_str != NULL) { free(_str); }
			__s = _get_str(&lsb_buffer_pnt);
			_str = GetUniStr(__s, code);
			free(__s);
			_ev_attr_idx_ref_list[_default_attr_idx]++;
			//*참조 횟수 반영
			break;
			//* TYPE_EV_OP_HISTORY_CHAR일 때 : _s...가 아니라 _i_3, _i, _i_2, _str 이용
		}
		default:
		{
			if (_str != NULL) { free(_str); }
				__s = _get_str(&lsb_buffer_pnt);
				_str = GetUniStr (__s, code);
				free(__s);
			//*** replace _str
			break;
			//* 이외의 경우 : _str 이용
		}
	}

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//* 이벤트 내부 인자 : 모든 명령어 타입에 대해서 읽어들이는 명령어


void LSB_EVENT_OP_INST::_read_char_data(unsigned char** _lsb_buffer_pnt, unsigned int txt_accum_idx, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);

	unsigned int* _p = _get_int(&lsb_buffer_pnt);
	_i_3 = *_p; free(_p);
	//*_i_3 읽어들이기

	_p = _get_int(&lsb_buffer_pnt);
	_extra_data = *_p; free(_p);
	//*_extra_data 읽어들이기, 이게 50인 경우 뿐만 아니라 300이 되는 경우가 나왔음;;
	//*이걸 고칠지 어떨지는 실행결과 보고 판단

	char *__s = (char*)malloc(3);
	__s[0] = *(char*)(lsb_buffer_pnt + 1);
	__s[1] = *(char*)(lsb_buffer_pnt);
	__s[2] = 0;
	//*문자 얻어오기

	if (__s[0] == 0) { __s[0] = __s[1]; __s[1] = 0; }
	//*1바이트 문자는 이대로 하면 그냥 0이 되서 다시 수정해야 한다

	lsb_buffer_pnt += 2;
	wchar_t* __w = GetUniStr(__s, code);
	free(__s);
	//*3칸 미리 할당해서 읽어들인 후 유니코드로 변환

	if (_str == NULL) {
		_str = __w;
		//*처음이라면 할당한 값 그대로 붙여넣는다
	}
	else {
		wchar_t* _n_str = (wchar_t*)malloc(sizeof(wchar_t)*(wcslen(_str) + 1 + 1));
		memcpy(_n_str, _str, (sizeof(wchar_t)*wcslen(_str)));
		_n_str[wcslen(_str)] = __w[0];
		_n_str[wcslen(_str) + 1] = 0;
		free(__w); free(_str); 
		_str = _n_str;
		//*그렇지 않다면 공간 재할당 후 바꿔친다
	}
	//*_str 갱신하기

	_p = _get_int(&lsb_buffer_pnt);
	unsigned int _attr_idx = *_p;
	free(_p);
	//*먼저 속성 인덱스 값 확보

	_ev_attr_idx_ref_list[_attr_idx]++;
	_last_idx = _attr_idx;
	//*참조 횟수 반영

	if (_default_attr_idx == 0xFFFFFFFF) { _default_attr_idx = _attr_idx; }
	//*기준이 될 속성 인덱스 지정

	if (_attr_idx != _default_attr_idx) {
		if (_ev_attr_info_list == NULL) {
			_ev_attr_info_list = (LSB_EV_ATTR_INFO*)malloc(sizeof(LSB_EV_ATTR_INFO));
			memset(_ev_attr_info_list, 0, sizeof(LSB_EV_ATTR_INFO));
			_ev_attr_info_list[0]._attr_idx = _attr_idx;
			_ev_attr_info_list[0]._start_txt_pos = txt_accum_idx;
			_ev_attr_info_list[0]._end_txt_pos = txt_accum_idx + 1;
			_ev_attr_info_cnt++;
		}
		//*아예 없으면 새로 할당하고 개수 늘림
		else {
			bool found_txt_chunk = false;
			for (unsigned int i = 0; i < _ev_attr_info_cnt;i++) {
				if ((_attr_idx == _ev_attr_info_list[i]._attr_idx)
					&& (txt_accum_idx == (_ev_attr_info_list[i]._end_txt_pos)))
				{
					_ev_attr_info_list[i]._end_txt_pos++; 
					found_txt_chunk = true; break;
				}
				//*적용될 속성이 같고 현재 텍스트 인덱스 끝자락에 이어 붙일 수 있을 때는 숫자만 늘려주면 된다
				//*그렇지 않으면 계속 찾아봄
			}
			if (!found_txt_chunk) {
				LSB_EV_ATTR_INFO* _n_il = (LSB_EV_ATTR_INFO*)malloc(sizeof(LSB_EV_ATTR_INFO) * (_ev_attr_info_cnt+1));
				memset(_n_il, 0, (sizeof(LSB_EV_ATTR_INFO) * (_ev_attr_info_cnt + 1)));
				memcpy(_n_il, _ev_attr_info_list, (sizeof(LSB_EV_ATTR_INFO) * _ev_attr_info_cnt));
				_n_il[_ev_attr_info_cnt]._attr_idx = _attr_idx;
				_n_il[_ev_attr_info_cnt]._start_txt_pos = txt_accum_idx;
				_n_il[_ev_attr_info_cnt]._end_txt_pos = txt_accum_idx + 1;
				free(_ev_attr_info_list); _ev_attr_info_list = _n_il;
				_ev_attr_info_cnt++;
			}
			//*못찾았다면 그냥 새로 할당 후 복사 및 추가하는 수 밖에 없음, 그리고 교체 및 수 늘리기
		}
		//*있으면 새로이 크기를 할당하고 옮긴 후 추가할지, 아니면 더 찾아보고 숫자를 늘릴지 파악
	}
	//*새로 들어온 값이 기본 속성 인덱스 값이 아닐 경우라면
	//*해당 인덱스 바로 다음 숫자가 있나 보고,
	//*아니면 새로 붙여넣는다
	//*똑같으면 굳이 신경쓸 필요가 없다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//* 이벤트 내부 인자 : 이벤트 내부 char 변수 읽기
//*_word_str 데이터만 지속적으로 추가하고, 나머지는 있으면 읽어들이지 않는다


void LSB_EVENT_OP_INST::_decode_sharp_encoding(char** _str_addr)
{
	unsigned int _len = 0;
	unsigned int _new_len = 0;
	unsigned int _char_count = 0;
	char* _start_pos = NULL;
	char* _end_pos = NULL;
	char _tp_arr[4];				//*임시 숫자 받기용

	if (_str_addr == NULL) { return; }
	char* _s = (*_str_addr);
	if (_s == NULL) { return; }
	//*조건 검사

	_len = strlen(_s);
	//*문자열 길이 확인

	for (unsigned int i = 0; i < _len;i++) {
		if (_s[i] == '#') {
			if (i >= (_len - 4)) { break; }
			if (((_s[i+1] >= '0') && (_s[i+1] <= '9')) && ((_s[i+2] >= '0') && (_s[i+2] <= '9')) 
				&& ((_s[i+3] >= '0') && (_s[i+3] <= '9'))) 
			{ _start_pos = &_s[i]; break; }
		}
	}
	//*# + 숫자 3자리로 된 값들이 시작되는 위치 확인

	if (_start_pos == NULL) { return; }
	//*없으면 그냥 리턴한다

	for (unsigned int i = 0; i < _len; i++) {
		if (_s[i] == '#') {
			if (i >= (_len - 4)) { break; }
			if (((_s[i + 1] >= '0') && (_s[i + 1] <= '9')) && ((_s[i + 2] >= '0') && (_s[i + 2] <= '9'))
				&& ((_s[i + 3] >= '0') && (_s[i + 3] <= '9')))
			{ _end_pos = &_s[i + 4]; i += 3; }
		}
	}
	//*# + 숫자 3자리로 된 값들이 끝나는 위치 확인

	_char_count = (_end_pos - _start_pos) / 4;
	char* _n_tp = (char*)malloc(_char_count + (strlen(sharp_encoding_prefix) * 2) + 1);
	memset(_n_tp, 0, (_char_count + (strlen(sharp_encoding_prefix) * 2) + 1));
	memcpy(_n_tp, sharp_encoding_prefix, strlen(sharp_encoding_prefix));
	for (unsigned int i = 0; i < _char_count;i++) {
		memset(_tp_arr, 0, 4);
		memcpy(_tp_arr, (_start_pos + (i * 4) + 1), 3);
		*(unsigned char*)(&_n_tp[strlen(_n_tp)]) = (unsigned char)strtol(_tp_arr, NULL, 10);
	}
	memcpy(&_n_tp[strlen(_n_tp)], sharp_encoding_prefix, strlen(sharp_encoding_prefix));
	//*위 값을 싹 읽어들여서 바이트 변환해 붙여넣어 ansi 문자열 만듦

	_new_len = _len - (_end_pos - _start_pos) + strlen(_n_tp);
	char* _new_s = (char*)malloc(_new_len + 1);
	memset(_new_s, 0, (_new_len + 1));
	memcpy(_new_s, _s, (_start_pos - _s));
	memcpy(_new_s + strlen(_new_s), _n_tp, strlen(_n_tp));
	memcpy(_new_s + strlen(_new_s), _end_pos, (_s + strlen(_s) - _end_pos));
	free(_s); free(_n_tp);
	(*_str_addr) = _new_s;
	//*이렇게 변환한 문자열을 다시 자르고 조각내서 새로이 만들어 붙여넣고 반환
}
//*이벤트 내부 인자 : 이벤트 명령문에서 # + 숫자문자열로 인코딩된 값을 원래대로 돌려놓기
//*일단은 한 번만 해도 되는 것 같으니 한 번만 한다


void LSB_EVENT_OP_INST::_decompile_code(HANDLE hWrite, unsigned int code, unsigned int _indent)
{
	for (unsigned int j = 0; j < (_indent); j++) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_); }
	//*들여쓰기 출력

	wchar_t* _w_str, * _w_str_2, * _ww_str_2;

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s:%02d][0x%08X]"), event_op_type_prefix, (unsigned char)_op_type, _unk_1);
	switch (_op_type)
	{
		case TYPE_EV_OP_CHAR:
		{
			unsigned int _n_wcslen = 0;
			unsigned int _accmul_txt_pos = 0;

			if (_ev_attr_info_cnt == 0) {
				_w_str = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(_str) + 1));
				memset(_w_str, 0, (sizeof(wchar_t) * (wcslen(_str) + 1)));
				memcpy(_w_str, _str, (sizeof(wchar_t) * wcslen(_str)));
			}
			//*만일 바꿀 게 없으면 그대로 복사함

			else {
				_w_str_2 = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(_str) + 1));
				memset(_w_str_2, 0, (sizeof(wchar_t) * (wcslen(_str) + 1)));
				memcpy(_w_str_2, _str, (sizeof(wchar_t) * wcslen(_str)));
				//*미리 복사해둠

				for (unsigned int i = 0; i < _ev_attr_info_cnt; i++) {
					_n_wcslen = wcslen(_w_str_2) + (wcslen(attr_apply_prefix) * 2) + wcslen(ev_attr_idx_prefix) + 13;
					//*바뀌게 될 경우 문자열 길이 확립 ("(＃)" + "[_IDX_:0x(8자리)]"(길이:13 + "_IDX_") + 그대로 + "(＃)")
					_w_str = (wchar_t*)malloc(sizeof(wchar_t) * (_n_wcslen + 1));
					memset(_w_str, 0, (sizeof(wchar_t) * (_n_wcslen + 1)));
					//*할당 후 세팅
					memcpy(_w_str, _w_str_2, (sizeof(wchar_t) * (_ev_attr_info_list[i]._start_txt_pos + _accmul_txt_pos)));
					swprintf(_w_str + wcslen(_w_str), TEXT("%s[%s:0x%08X]"),
						attr_apply_prefix, ev_attr_idx_prefix, _ev_attr_info_list[i]._attr_idx);
					memcpy(_w_str + wcslen(_w_str),
						_w_str_2 + (_ev_attr_info_list[i]._start_txt_pos + _accmul_txt_pos),
						(sizeof(wchar_t) * (_ev_attr_info_list[i]._end_txt_pos - _ev_attr_info_list[i]._start_txt_pos)));
					swprintf(_w_str + wcslen(_w_str), TEXT("%s"), attr_apply_prefix);
					memcpy(_w_str + wcslen(_w_str),
						_w_str_2 + ((_ev_attr_info_list[i]._end_txt_pos) + _accmul_txt_pos),
						(sizeof(wchar_t) * (wcslen(_w_str_2) - ((_ev_attr_info_list[i]._end_txt_pos) + _accmul_txt_pos))));
					//*조각내가면서 추가함, 누적값도 고려해가면서 세팅해야 함
					_accmul_txt_pos += ((wcslen(attr_apply_prefix) * 2) + wcslen(ev_attr_idx_prefix) + 13);
					//*누적값 올려줌
					free(_w_str_2); _w_str_2 = _w_str;
					//*이전 값은 해제하고 바꿔치기
				}
				//*_str에 확립한 속성 정보를 이용해서 중간중간 문자열을 삽입해줌
			}

			swprintf(_txt_buf + wcslen(_txt_buf),
				TEXT("[0x%08X][0x%08X]%s%s%s[%s:0x%08X]%s"),
				_i_3, _extra_data, text_prefix, _w_str, text_prefix, ev_attr_def_idx_prefix, _default_attr_idx, _enter_raw_);
			free(_w_str);
			break;
			//* TYPE_EV_OP_CHAR일 때 : _i_3, _extra_data, _str, _i 이용
			//* 그동안 봤는데 여기서는 코드 대치가 필요없음
		}
		case TYPE_EV_OP_ALIGN:
		case TYPE_EV_OP_RETURN:
		{
			if (_unk_1 != 0) {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[0x%02X][0x%08X][0x%08X][0x%02X]%s"), 
					_unk_t2_t3._unk_1, _unk_t2_t3._unk_2, _unk_t2_t3._unk_3, _b, _enter_raw_);
			}
			else {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[0x%02X]%s"), _b, _enter_raw_);
			}
			break;
			//* TYPE_EV_OP_ALIGN / TYPE_EV_OP_RETURN일 때 : _unk_t2_t3, _b 이용
		}
		case TYPE_EV_OP_INDENT:
		case TYPE_EV_OP_UNDENT:
		{
			break;
			//* TYPE_EV_OP_INDENT / TYPE_EV_OP_UNDENT일 때 : 이용하지 않음
		}
		case TYPE_EV_OP_EVENT:
		{
			_w_str_2 = _str;
			_ww_str_2 = Replace_Text(_w_str_2, hex_1_raw, hex_1_plained);
			_w_str_2 = Replace_Text(_ww_str_2, _enter_raw_, _enter_plained_); free(_ww_str_2);
			//*코드 변환 후 대치

			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s%s%s%s"), text_prefix, _w_str_2, text_prefix, _enter_raw_);
			free(_w_str_2);
			break;
			//* TYPE_EV_OP_EVENT일 때 : _str 이용 (대치 필요)
		}
		case TYPE_EV_OP_VAR:
		{
			if (_tpword_ver < 0x65) {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s%s%s[%s:0x%08X]%s"), text_prefix, _str, text_prefix, ev_attr_def_idx_prefix, _default_attr_idx, _enter_raw_);
			}
			else if (_tpword_ver >= 0x66) {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[0x%08X]%s%s%s%s%s%s[%s:0x%08X]%s"), 
					_i_2, text_prefix, _s, text_prefix, text_prefix, _str, text_prefix, ev_attr_def_idx_prefix, _default_attr_idx, _enter_raw_);
			}
			else {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[0x%08X]%s%s%s[%s:0x%08X]%s"), 
					_i_2, text_prefix, _s, text_prefix, ev_attr_def_idx_prefix, _default_attr_idx, _enter_raw_);
			}
			break;
			//* TYPE_EV_OP_VAR일 때 : ver < 0x65[_str, _i 이용], 0x65 <= ver[_i, _i_2, _s 이용], 0x66 <= ver[_i, _i_2, _s, _str 이용]
			//* 아무래도 변환을 할 필요가 없을 것 같음
		}
		case TYPE_EV_OP_IMG:
		{
			if (_tpword_ver < 0x69) {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[0x%08X][0x%08X]%s%s%s[0x%02X]%s"),
					_i_2, _extra_data, text_prefix, _str, text_prefix, _b, _enter_raw_);
			}
			else {
				swprintf(_txt_buf + wcslen(_txt_buf),
					TEXT("[0x%08X][0x%08X]%s%s%s::[0x%08X][0x%08X][0x%08X][0x%08X][0x%08X][0x%08X]::[0x%02X]%s"),
					_i_2, _extra_data, text_prefix, _str, text_prefix, 
					_unk_i_list[0], _unk_i_list[1], _unk_i_list[2], _unk_i_list[3], _unk_i_list[4], _unk_i_list[5],
					_b, _enter_raw_);
			}
			break;
			//* TYPE_EV_OP_IMG일 때 : _i_2, _extra_data, _str, _b 이용
			//* 아무래도 변환을 할 필요가 없을 것 같음
			//* 버전이 105 이상이면 정수 6개 추가
		}
		case TYPE_EV_OP_HISTORY_CHAR:
		{
			swprintf(_txt_buf + wcslen(_txt_buf),
				TEXT("[0x%08X][0x%08X]%s%s%s[%s:0x%08X]%s"),
				_i_3, _i_2, text_prefix, _str, text_prefix, ev_attr_def_idx_prefix, _default_attr_idx, _enter_raw_);
			break;
			//* TYPE_EV_OP_HISTORY_CHAR일 때 : _s...가 아니라 _i_3, _i, _i_2, _str 이용
			//* 함수 호출용으로나 쓰이니 변환할 필요가 없을 것 같음
		}
		default:
		{
			_w_str_2 = _str;
			_ww_str_2 = Replace_Text(_w_str_2, hex_1_raw, hex_1_plained);
			_w_str_2 = Replace_Text(_ww_str_2, _enter_raw_, _enter_plained_); free(_ww_str_2);
			//*코드 변환 후 대치

			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s%s%s%s"), text_prefix, _w_str_2, text_prefix, _enter_raw_);
			free(_w_str_2);
			break;
			//* 이외의 경우 : _str 이용
		}
	}
}
//*이벤트 인자 클래스 : 핸들로 출력하는 함수


void LSB_EVENT_OP_INST::_recompile_from_txt(wchar_t* _code_txt_buffer, unsigned int code)
{
	VARIABLES_DECLARE(_code_txt_buffer_);

	GET_INT_FROM_TEXT(_unk_1);
	switch (_op_type)
	{
		case TYPE_EV_OP_CHAR:
		{
			GET_INT_FROM_TEXT(_i_3);
			GET_INT_FROM_TEXT(_extra_data);
			GET_STR_FROM_TEXT(_str);
			GET_DEF_IDX_FROM_TEXT(_default_attr_idx);
			//*_i_3 / _extra_data / _str(루비가 포함된 값) / _default_attr_idx 분리

			_set_char_data();
			//*_str에서 추가적으로 정보를 세팅해 준다

			break;
			//* TYPE_EV_OP_CHAR일 때 : _i_3, _extra_data, _str, _i 이용
			//* 그동안 봤는데 여기서는 코드 대치가 필요없음
		}
		case TYPE_EV_OP_ALIGN:
		case TYPE_EV_OP_RETURN:
		{
			if (_unk_1 != 0) {
				GET_BYTE_FROM_TEXT(_unk_t2_t3._unk_1);
				GET_INT_FROM_TEXT(_unk_t2_t3._unk_2);
				GET_INT_FROM_TEXT(_unk_t2_t3._unk_3);
			}

			GET_BYTE_FROM_TEXT(_b);
			//*_b 분리

			break;
			//* TYPE_EV_OP_ALIGN / TYPE_EV_OP_RETURN일 때 : _unk_t2_t3, _b 이용
		}
		case TYPE_EV_OP_INDENT:
		case TYPE_EV_OP_UNDENT:
		{
			break;
			//* TYPE_EV_OP_INDENT / TYPE_EV_OP_UNDENT일 때 : 이용하지 않음
		}
		case TYPE_EV_OP_EVENT:
		{
			GET_STR_FROM_TEXT(_w_str_2);
			//*_str(풀어진값) 분리

			_ww_str_2 = Replace_Text(_w_str_2, hex_1_plained, hex_1_raw); free(_w_str_2);
			_str = Replace_Text(_ww_str_2, _enter_plained_, _enter_raw_); free(_ww_str_2);
			//*문자열 대치

			break;
			//* TYPE_EV_OP_EVENT일 때 : _str 이용 (대치 필요)
		}
		case TYPE_EV_OP_VAR:
		{
			if (_tpword_ver < 0x65) {
				GET_STR_FROM_TEXT(_str);
				GET_DEF_IDX_FROM_TEXT(_default_attr_idx);
				//*_str / _default_attr_idx 분리
			}
			else if (_tpword_ver >= 0x66) {
				GET_INT_FROM_TEXT(_i_2);
				GET_STR_FROM_TEXT(_s);
				GET_STR_FROM_TEXT(_str);
				GET_DEF_IDX_FROM_TEXT(_default_attr_idx);
				//*_i_2 / _s / _str / _default_attr_idx 분리
			}
			else {
				GET_INT_FROM_TEXT(_i_2);
				GET_STR_FROM_TEXT(_s);
				GET_DEF_IDX_FROM_TEXT(_default_attr_idx);
				//*_i_2 / _s / _default_attr_idx 분리
			}

			_ev_attr_list_pnt[_default_attr_idx]->_ev_op_ref_count++;
			//*참조 횟수 반영
			
			break;
			//* TYPE_EV_OP_VAR일 때 : ver < 0x65[_str, _i 이용], 0x65 <= ver[_i, _i_2, _s 이용], 0x66 <= ver[_i, _i_2, _s, _str 이용]
			//* 아무래도 변환을 할 필요가 없을 것 같음
		}
		case TYPE_EV_OP_IMG:
		{
			GET_INT_FROM_TEXT(_i_2);
			GET_INT_FROM_TEXT(_extra_data);
			GET_STR_FROM_TEXT(_str);
			if (_tpword_ver >= 0x69) {
				for (unsigned int i = 0; i < 6;i++) { GET_INT_FROM_TEXT(_unk_i_list[i]); }
			}
			GET_BYTE_FROM_TEXT(_b);
			//*_i_2 / _str / _b / _extra_data 분리
			//*버전에 따라서는 정수 6개 추가

			break;
			//* TYPE_EV_OP_IMG일 때 : _i_w, _extra_data, _str, _b 이용
			//* 아무래도 변환을 할 필요가 없을 것 같음
		}
		case TYPE_EV_OP_HISTORY_CHAR:
		{
			GET_INT_FROM_TEXT(_i_3);
			GET_INT_FROM_TEXT(_i_2);
			GET_STR_FROM_TEXT(_str);
			GET_DEF_IDX_FROM_TEXT(_default_attr_idx);
			//*_s...가 아니라 _i_3 / _i_2 / _str / _default_attr_idx 분리

			_ev_attr_list_pnt[_default_attr_idx]->_ev_op_ref_count++;
			//*참조 횟수 반영

			break;
			//* TYPE_EV_OP_HISTORY_CHAR일 때 : _s...가 아니라 _i_3, _i, _i_2, _str 이용
			//* 함수 호출용으로나 쓰이니 변환할 필요가 없을 것 같음
		}
		default:
		{
			break;
			//* 이외의 경우 : 그냥 넘김
		}
	}
	//*각 경우에 맞춰 변환함
}
//*이벤트 인자 클래스 : 리컴파일 함수 (인자 : 해당 이벤트 블럭의 데이터가 들어있는 텍스트 데이터 일부)


void LSB_EVENT_OP_INST::_set_char_data()
{
	wchar_t* _w_str = _str, * _w_str_2 = _str;
	//*진행되면서 갱신된 값을 저장할 문자열 포인터들

	//*** 디폴트 속성이 이미 적용되었다 가정함
	//*** 굳이 대치할 필요는 없다

	_ev_attr_info_cnt = Get_Txt_Count(_str, ev_attr_idx_prefix, wcslen(_str));
	if (_ev_attr_info_cnt == 0) { 
		_ev_attr_list_pnt[_default_attr_idx]->_ev_op_ref_count += (wcslen(_str));
		return; 
	}
	//*다른 속성이 적용된 횟수 구하기, 없으면 횟수만 반영하고 나간다

	_ev_attr_info_list = (LSB_EV_ATTR_INFO*)malloc(sizeof(LSB_EV_ATTR_INFO) * _ev_attr_info_cnt);
	memset(_ev_attr_info_list, 0, (sizeof(LSB_EV_ATTR_INFO) * _ev_attr_info_cnt));
	//*속성 적용 인덱스 정보 배열 할당

	wchar_t* _code_txt_pnt = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(_str) + 1));
	memset(_code_txt_pnt, 0, (sizeof(wchar_t) * (wcslen(_str) + 1)));
	memcpy(_code_txt_pnt, _str, (sizeof(wchar_t) * wcslen(_str)));
	wchar_t* _code_txt_pnt_2 = _str, * _swp;
	unsigned int _sub_length = 0, _cp_pos_after = 0;
	//*_code_txt_pnt는 이례적으로 직접 복사해서 넣어야 한다

	for (unsigned int i = 0; i < _ev_attr_info_cnt;i++) {

		GET_IDX_FROM_TEXT(_ev_attr_info_list[i]._attr_idx);
		//*먼저 인덱스 얻기

		_sub_length = wcslen(attr_apply_prefix) + wcslen(TEXT("[")) + wcslen(ev_attr_idx_prefix) + wcslen(TEXT(":"));
		//*빼야 할 길이 확정 ("(＃)" + "[" + "_IDX_" + ":")

		_ev_attr_info_list[i]._start_txt_pos = (_code_txt_pnt_2 - _code_txt_pnt) - _sub_length;
		//*이때 _code_txt_pnt_2는 원래 인덱스보다 "(＃)" + "[" + "_IDX_" + ":" 정도 와 있을 것이므로
		//*시작값을 이 길이만큼 빼서 넣는다

		_cp_pos_after = _ev_attr_info_list[i]._start_txt_pos + _sub_length + 10 + wcslen(TEXT("]"));
		//*복사할 위치 확정 (시작 위치 + "(＃)" + "[" + "_IDX_" + 16진수 10자리(0x********) + ":" + "]")

		_swp = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(_code_txt_pnt) - _sub_length + 1));
		memset(_swp, 0, (sizeof(wchar_t) * (wcslen(_code_txt_pnt) - _sub_length + 1)));
		memcpy(_swp, _code_txt_pnt, (sizeof(wchar_t) * _ev_attr_info_list[i]._start_txt_pos));
		memcpy((_swp + _ev_attr_info_list[i]._start_txt_pos), (_code_txt_pnt + _cp_pos_after),
			(sizeof(wchar_t) * (wcslen(_code_txt_pnt) - _cp_pos_after)));
		free(_code_txt_pnt); _code_txt_pnt = _swp;
		//*위 길이만큼 문자열 길이 줄여서 _code_txt_pnt 갱신
		//*복사위치를 잘 지정해 가지고 온다

		_code_txt_pnt_2 = wcsstr(_code_txt_pnt, attr_apply_prefix);
		_ev_attr_info_list[i]._end_txt_pos = (_code_txt_pnt_2 - _code_txt_pnt);
		//*적용 텍스트가 끝나는 위치 찾기

		_sub_length = wcslen(attr_apply_prefix);
		_cp_pos_after = _ev_attr_info_list[i]._end_txt_pos + _sub_length;
		//*빼야 할 길이 / 복사할 위치 확정

		_swp = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(_code_txt_pnt) - _sub_length + 1));
		memset(_swp, 0, (sizeof(wchar_t) * (wcslen(_code_txt_pnt) - _sub_length + 1)));
		memcpy(_swp, _code_txt_pnt, (sizeof(wchar_t) * _ev_attr_info_list[i]._end_txt_pos));
		memcpy((_swp + _ev_attr_info_list[i]._end_txt_pos), (_code_txt_pnt + _cp_pos_after),
			(sizeof(wchar_t) * (wcslen(_code_txt_pnt) - _cp_pos_after)));
		free(_code_txt_pnt); _code_txt_pnt = _swp;
		//*다시금 표식 지우고 위치 갱신, 이번에 뺄 값은 ev_attr_idx_prefix다.
	}
	//*그 횟수만큼 돌아가면서 다시 원래 문자열로 돌리기 / 인덱스 보정 / 정보 배열에 세팅하기

	free(_str); _str = _code_txt_pnt;
	//*이렇게 나온 원본 문자열을 _str에 세팅해주기

	_idx_list = (unsigned int*)malloc(sizeof(int) * wcslen(_str));
	memset(_idx_list, 0, (sizeof(int) * wcslen(_str)));
	for (unsigned int i = 0; i < wcslen(_str); i++) { _idx_list[i] = _default_attr_idx; }
	if (_ev_attr_info_list != NULL) {
		for (unsigned int i = 0; i < _ev_attr_info_cnt; i++) {
			for (unsigned int j = _ev_attr_info_list[i]._start_txt_pos; j < _ev_attr_info_list[i]._end_txt_pos; j++) {
				_idx_list[j] = _ev_attr_info_list[i]._attr_idx;
			}
		}
	}
	//*배열 정보를 토대로 각 문자별 인덱스 세팅

	for (unsigned int i = 0; i < wcslen(_str);i++) { 
		_ev_attr_list_pnt[_idx_list[i]]->_ev_op_ref_count++; 
	}
	//*참조횟수 반영
}
//*이벤트 인자 클래스 : 이벤트 내부 _str 변수를 분해해 정보 세팅하기


void LSB_EVENT_OP_INST::_encode_sharp_prefix(char** _str_addr)
{
	unsigned int _len = 0;
	unsigned int _new_len = 0;
	unsigned int _char_count = 0;
	char* _start_pos = NULL;
	char* _end_pos = NULL;

	if (_str_addr == NULL) { return; }
	char* _s = (*_str_addr);
	if (_s == NULL) { return; }
	//*조건 검사

	_len = strlen(_s);
	//*문자열 길이 확인

	_start_pos = strstr(_s, sharp_encoding_prefix);
	//*[#]가 처음 발견되는 곳 확인

	if (_start_pos == NULL) { return; }
	//*없으면 그냥 리턴한다

	_end_pos = strstr((_start_pos + strlen(sharp_encoding_prefix)), sharp_encoding_prefix);
	//*[#]가 나중에 발견되는 곳 확인

	if (_end_pos == NULL) { return; }
	_end_pos += strlen(sharp_encoding_prefix);
	//*없으면 그냥 리턴하고, 있으면 [#]의 길이만큼 더해서 세팅한다

	_char_count = (_end_pos - _start_pos) - (strlen(sharp_encoding_prefix) * 2);
	char* _n_tp = (char*)malloc((_char_count * 4) + 1);
	memset(_n_tp, 0, ((_char_count * 4) + 1));
	for (unsigned int i = 0; i < _char_count; i++) {
		sprintf(_n_tp + strlen(_n_tp), "#%03d", *(unsigned char*)(_start_pos + strlen(sharp_encoding_prefix) + i));
	}
	//*위 값을 싹 읽어들여서 바이트로 조각낸 후 # + 숫자 3자리 형태로 만듦

	_new_len = _len - (_end_pos - _start_pos) + strlen(_n_tp);
	char* _new_s = (char*)malloc(_new_len + 1);
	memset(_new_s, 0, (_new_len + 1));
	memcpy(_new_s, _s, (_start_pos - _s));
	memcpy(_new_s + strlen(_new_s), _n_tp, strlen(_n_tp));
	memcpy(_new_s + strlen(_new_s), _end_pos, (_s + strlen(_s) - _end_pos));
	free(_s); free(_n_tp);
	(*_str_addr) = _new_s;
	//*이렇게 변환한 문자열을 다시 자르고 조각내서 새로이 만들어 붙여넣고 반환
}
//*이벤트 내부 인자 : 이벤트 명령문에서 [#] ~ [#] 사이에 있는 문자열을 # + 숫자문자열로 인코딩하기


void LSB_EVENT_OP_INST::_write_as_lsb(HANDLE hNewLsb, unsigned int code, unsigned int *_total_inst_count)
{
	char* __s;
	//*임시 문자열 / 부분 문자열

	switch (_op_type)
	{
		case TYPE_EV_OP_CHAR:
		{
			wchar_t __w[2];
			memset(__w, 0, sizeof(wchar_t) * 2);
			//*부분 문자열 준비

			if (_idx_list == NULL) {
				_idx_list = (unsigned int*)malloc(sizeof(int) * wcslen(_str));
				memset(_idx_list, 0, (sizeof(int) * wcslen(_str)));
				for (unsigned int i = 0; i < wcslen(_str); i++) { _idx_list[i] = _default_attr_idx; }
				if (_ev_attr_info_list != NULL) {
					for (unsigned int i = 0; i < _ev_attr_info_cnt; i++) {
						for (unsigned int j = _ev_attr_info_list[i]._start_txt_pos; j < _ev_attr_info_list[i]._end_txt_pos; j++) {
							_idx_list[j] = _ev_attr_info_list[i]._attr_idx;
						}
					}
				}
			}
			//*배열 정보를 토대로 각 문자별 인덱스 세팅

			for (unsigned int i = 0; i < wcslen(_str);i++) {

				_write_byte(hNewLsb, _op_type);
				//*명령어 코드 기록

				if (_tpword_ver >= 0x69) { _write_int(hNewLsb, _unk_1); }
				//*버전에 따라 중간값도 기록

				_write_int(hNewLsb, _i_3);
				//*_i_3 기록

				_write_int(hNewLsb, _extra_data);
				//*_extra_data 기록

				__w[0] = _str[i];
				__s = GetAnsiStr(__w, code);
				if (__s[1] != 0) {
					_write_byte(hNewLsb, __s[1]);
					_write_byte(hNewLsb, __s[0]);
				}
				else {
					_write_data(hNewLsb, (unsigned char*)__s, 2);
				}
				free(__s);
				//*해당 위치의 문자를 하나 떼서 변환 후 2바이트면 앞뒤를 바꿔 기록
				//*1바이트가 나오면 그냥 순서대로 기록한다

				_write_int(hNewLsb, _idx_list[i]);
				//*인덱스 기록

				(*_total_inst_count)++;
				//*개수 갱신
			}

			return;
			//* TYPE_EV_OP_CHAR일 때 : _read_char_data 호출 (_i_3, _extra_data, _str, _i) 이용
			//*문자를 하나씩 떼내어 가면서 전체적으로 반복해서 기록함
		}
		case TYPE_EV_OP_ALIGN:
		case TYPE_EV_OP_RETURN:
		{
			_write_byte(hNewLsb, _op_type);

			if (_tpword_ver >= 0x69) { _write_int(hNewLsb, _unk_1); }
			//*버전에 따라 중간값도 기록

			if (_unk_1 != 0) {
				_write_byte(hNewLsb, _unk_t2_t3._unk_1);
				_write_int(hNewLsb, _unk_t2_t3._unk_2);
				_write_int(hNewLsb, _unk_t2_t3._unk_3);
			}
			//*_unk_1에 따라 추가값 기록

			_write_byte(hNewLsb, _b);
			//*** 0이나 1이 아닐때 확인
			//*_b 기록

			break;
			//* TYPE_EV_OP_ALIGN / TYPE_EV_OP_RETURN일 때 : _unk_t2_t3, _b 기록
		}
		case TYPE_EV_OP_INDENT:
		case TYPE_EV_OP_UNDENT:
		{
			_write_byte(hNewLsb, _op_type);

			if (_tpword_ver >= 0x69) { _write_int(hNewLsb, _unk_1); }
			//*버전에 따라 중간값도 기록

			break;
			//* TYPE_EV_OP_INDENT / TYPE_EV_OP_UNDENT일 때 : _op_type만 기록
		}
		case TYPE_EV_OP_EVENT:
		{
			_write_byte(hNewLsb, _op_type);

			if (_tpword_ver >= 0x69) { _write_int(hNewLsb, _unk_1); }
			//*버전에 따라 중간값도 기록

			__s = GetAnsiStr(_str, code);
			_encode_sharp_prefix(&__s);
			_write_str(hNewLsb, __s);
			free(__s);
			//*_str 기록
			//*이 값을 기록할 때는 인코딩 한 번 하고 기록해줘야 함

			break;
			//* TYPE_EV_OP_EVENT일 때 : _str 기록
		}
		case TYPE_EV_OP_VAR:
		{
			_write_byte(hNewLsb, _op_type);

			if (_tpword_ver >= 0x69) { _write_int(hNewLsb, _unk_1); }
			//*버전에 따라 중간값도 기록

			if (_tpword_ver < 0x65) {
				__s = GetAnsiStr(_str, code);
				_write_str(hNewLsb, __s);
				free(__s);
				//*_str 기록
				_write_int(hNewLsb, _default_attr_idx);
				//*_i 기록
			}
			else {
				_write_int(hNewLsb, _default_attr_idx);
				//*_int 기록
				_write_int(hNewLsb, _i_2);
				//*_i_2 기록
				__s = GetAnsiStr(_s, code);
				_write_str(hNewLsb, __s);
				free(__s);
				//*_s 기록
				if (_tpword_ver >= 0x66) {
					__s = GetAnsiStr(_str, code);
					_write_str(hNewLsb, __s);
					free(__s);
					//*_str 기록
				}
			}

			break;
			//* TYPE_EV_OP_VAR일 때 : ver < 0x65[_str, _i 이용], 0x65 <= ver[_i, _i_2, _s 이용], 0x66 <= ver[_i, _i_2, _s, _str 이용]
		}
		case TYPE_EV_OP_IMG:
		{
			_write_byte(hNewLsb, _op_type);

			if (_tpword_ver >= 0x69) { _write_int(hNewLsb, _unk_1); }
			//*버전에 따라 중간값도 기록

			_write_int(hNewLsb, _i_2);
			//*_i_2 기록

			_write_int(hNewLsb, _extra_data);
			//*_extra_data 기록

			__s = GetAnsiStr(_str, code);
			_write_str(hNewLsb, __s);
			free(__s);
			//*_str 기록

			_write_byte(hNewLsb, _b);
			//*_b 기록

			if (_tpword_ver >= 0x69) {
				for (unsigned int i = 0; i < 6; i++) { _write_int(hNewLsb, _unk_i_list[i]); }
			}
			//*버전에 따라 unk_i_list 기록

			break;
			//* TYPE_EV_OP_IMG일 때 : _i_2, _extra_data, _str, _b 이용
		}
		case TYPE_EV_OP_HISTORY_CHAR:
		{
			_write_byte(hNewLsb, _op_type);

			if (_tpword_ver >= 0x69) { _write_int(hNewLsb, _unk_1); }
			//*버전에 따라 중간값도 기록

			_write_int(hNewLsb, _i_3);
			//*_s...가 아니라 _i_3 기록

			_write_int(hNewLsb, _i_2);
			//*_i_2 기록

			_write_int(hNewLsb, _default_attr_idx);
			//*_i 기록

			__s = GetAnsiStr(_str, code);
			_write_str(hNewLsb, __s);
			free(__s);
			//*_str 기록

			break;
			//* TYPE_EV_OP_HISTORY_CHAR일 때 : _s...가 아니라 _i_3, _i, _i_2, _str 이용
		}
		default:
		{
			_write_byte(hNewLsb, _op_type);

			__s = GetAnsiStr(_str, code);
			_write_str(hNewLsb, __s);
			free(__s);
			//*_str 기록

			break;
			//* 이외의 경우 : _str 이용 (원래 문자열로 대치 필요)
		}
	}

	(*_total_inst_count)++;
	//*TYPE_EV_OP_CHAR일 때만 제외하면 전부 이곳에 다다른다
}
//*이벤트 내부 인자 : lsb 파일로 기록하는 함수


void LSB_EVENT_OP_INST::_extract_text(HANDLE hTxt)
{
	for (unsigned int j = 0; j < 2; j++) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_); }
	//*들여쓰기 2번 출력

	wchar_t* _w_str, * _w_str_2;
	wchar_t* _ww_str, * _ww_str_2;

	swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[%s:%02d][0x%08X]"), event_op_type_prefix, (unsigned char)_op_type, _unk_1);
	switch (_op_type)
	{
		case TYPE_EV_OP_CHAR:
		{
			unsigned int _n_wcslen = 0;
			unsigned int _accmul_txt_pos = 0;

			if (_ev_attr_info_cnt == 0) {
				_w_str = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(_str) + 1));
				memset(_w_str, 0, (sizeof(wchar_t) * (wcslen(_str) + 1)));
				memcpy(_w_str, _str, (sizeof(wchar_t) * wcslen(_str)));
			}
			//*만일 바꿀 게 없으면 그대로 복사함

			else {
				_w_str_2 = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(_str) + 1));
				memset(_w_str_2, 0, (sizeof(wchar_t) * (wcslen(_str) + 1)));
				memcpy(_w_str_2, _str, (sizeof(wchar_t) * wcslen(_str)));
				//*미리 복사해둠

				for (unsigned int i = 0; i < _ev_attr_info_cnt; i++) {
					_n_wcslen = wcslen(_w_str_2) + (wcslen(attr_apply_prefix) * 2) + wcslen(ev_attr_idx_prefix) + 13;
					//*바뀌게 될 경우 문자열 길이 확립 ("(＃)" + "[_IDX_:0x(8자리)]"(길이:13 + "_IDX_") + 그대로 + "(＃)")
					_w_str = (wchar_t*)malloc(sizeof(wchar_t) * (_n_wcslen + 1));
					memset(_w_str, 0, (sizeof(wchar_t) * (_n_wcslen + 1)));
					//*할당 후 세팅
					memcpy(_w_str, _w_str_2, (sizeof(wchar_t) * (_ev_attr_info_list[i]._start_txt_pos + _accmul_txt_pos)));
					swprintf(_w_str + wcslen(_w_str), TEXT("%s[%s:0x%08X]"),
						attr_apply_prefix, ev_attr_idx_prefix, _ev_attr_info_list[i]._attr_idx);
					memcpy(_w_str + wcslen(_w_str),
						_w_str_2 + (_ev_attr_info_list[i]._start_txt_pos + _accmul_txt_pos),
						(sizeof(wchar_t) * (_ev_attr_info_list[i]._end_txt_pos - _ev_attr_info_list[i]._start_txt_pos)));
					swprintf(_w_str + wcslen(_w_str), TEXT("%s"), attr_apply_prefix);
					memcpy(_w_str + wcslen(_w_str),
						_w_str_2 + ((_ev_attr_info_list[i]._end_txt_pos) + _accmul_txt_pos),
						(sizeof(wchar_t) * (wcslen(_w_str_2) - ((_ev_attr_info_list[i]._end_txt_pos) + _accmul_txt_pos))));
					//*조각내가면서 추가함, 누적값도 고려해가면서 세팅해야 함
					_accmul_txt_pos += ((wcslen(attr_apply_prefix) * 2) + wcslen(ev_attr_idx_prefix) + 13);
					//*누적값 올려줌
					free(_w_str_2); _w_str_2 = _w_str;
					//*이전 값은 해제하고 바꿔치기
				}
				//*_str에 확립한 속성 정보를 이용해서 중간중간 문자열을 삽입해줌
			}

			swprintf(_txt_buf + wcslen(_txt_buf),
				TEXT("[0x%08X][0x%08X]%s%s%s[%s:0x%08X]%s"),
				_i_3, _extra_data, text_prefix, _w_str, text_prefix, ev_attr_def_idx_prefix, _default_attr_idx, _enter_raw_);
			free(_w_str);
			break;
			//* TYPE_EV_OP_CHAR일 때 : _i_3, _extra_data, _str, _i 이용
			//* 그동안 봤는데 여기서는 코드 대치가 필요없음
		}
		case TYPE_EV_OP_ALIGN:
		case TYPE_EV_OP_RETURN:
		{
			if (_unk_1 != 0) {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[0x%02X][0x%08X][0x%08X][0x%02X]%s"),
					_unk_t2_t3._unk_1, _unk_t2_t3._unk_2, _unk_t2_t3._unk_3, _b, _enter_raw_);
			}
			else {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[0x%02X]%s"), _b, _enter_raw_);
			}
			break;
			//* TYPE_EV_OP_ALIGN / TYPE_EV_OP_RETURN일 때 : _unk_t2_t3, _b 이용
		}
		case TYPE_EV_OP_INDENT:
		case TYPE_EV_OP_UNDENT:
		{
			break;
			//* TYPE_EV_OP_INDENT / TYPE_EV_OP_UNDENT일 때 : 이용하지 않음
		}
		case TYPE_EV_OP_EVENT:
		{
			_w_str_2 = _str;
			_ww_str_2 = Replace_Text(_w_str_2, hex_1_raw, hex_1_plained);
			_w_str_2 = Replace_Text(_ww_str_2, _enter_raw_, _enter_plained_); free(_ww_str_2);
			//*코드 변환 후 대치

			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s%s%s%s"), text_prefix, _w_str_2, text_prefix, _enter_raw_);
			free(_w_str_2);
			break;
			//* TYPE_EV_OP_EVENT일 때 : _str 이용 (대치 필요)
		}
		case TYPE_EV_OP_VAR:
		{
			if (_tpword_ver < 0x65) {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s%s%s[%s:0x%08X]%s"), text_prefix, _str, text_prefix, ev_attr_def_idx_prefix, _default_attr_idx, _enter_raw_);
			}
			else if (_tpword_ver >= 0x66) {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[0x%08X]%s%s%s%s%s%s[%s:0x%08X]%s"),
					_i_2, text_prefix, _s, text_prefix, text_prefix, _str, text_prefix, ev_attr_def_idx_prefix, _default_attr_idx, _enter_raw_);
			}
			else {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[0x%08X]%s%s%s[%s:0x%08X]%s"),
					_i_2, text_prefix, _s, text_prefix, ev_attr_def_idx_prefix, _default_attr_idx, _enter_raw_);
			}
			break;
			//* TYPE_EV_OP_VAR일 때 : ver < 0x65[_str, _i 이용], 0x65 <= ver[_i, _i_2, _s 이용], 0x66 <= ver[_i, _i_2, _s, _str 이용]
			//* 아무래도 변환을 할 필요가 없을 것 같음
		}
		case TYPE_EV_OP_IMG:
		{
			if (_tpword_ver < 0x69) {
				swprintf(_txt_buf + wcslen(_txt_buf), TEXT("[0x%08X][0x%08X]%s%s%s[0x%02X]%s"),
					_i_2, _extra_data, text_prefix, _str, text_prefix, _b, _enter_raw_);
			}
			else {
				swprintf(_txt_buf + wcslen(_txt_buf),
					TEXT("[0x%08X][0x%08X]%s%s%s::[0x%08X][0x%08X][0x%08X][0x%08X][0x%08X][0x%08X]::[0x%02X]%s"),
					_i_2, _extra_data, text_prefix, _str, text_prefix,
					_unk_i_list[0], _unk_i_list[1], _unk_i_list[2], _unk_i_list[3], _unk_i_list[4], _unk_i_list[5],
					_b, _enter_raw_);
			}
			break;
			//* TYPE_EV_OP_IMG일 때 : _i_2, _extra_data, _str, _b 이용
			//* 아무래도 변환을 할 필요가 없을 것 같음
			//* 버전이 105 이상이면 정수 6개 추가
		}
		case TYPE_EV_OP_HISTORY_CHAR:
		{
			swprintf(_txt_buf + wcslen(_txt_buf),
				TEXT("[0x%08X][0x%08X]%s%s%s[%s:0x%08X]%s"),
				_i_3, _i_2, text_prefix, _str, text_prefix, ev_attr_def_idx_prefix, _default_attr_idx, _enter_raw_);
			break;
			//* TYPE_EV_OP_HISTORY_CHAR일 때 : _s...가 아니라 _i_3, _i, _i_2, _str 이용
			//* 함수 호출용으로나 쓰이니 변환할 필요가 없을 것 같음
		}
		default:
		{
			_w_str_2 = _str;
			_ww_str_2 = Replace_Text(_w_str_2, hex_1_raw, hex_1_plained);
			_w_str_2 = Replace_Text(_ww_str_2, _enter_raw_, _enter_plained_); free(_ww_str_2);
			//*코드 변환 후 대치

			swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s%s%s%s"), text_prefix, _w_str_2, text_prefix, _enter_raw_);
			free(_w_str_2);
			break;
			//* 이외의 경우 : _str 이용
		}
	}
}
//*이벤트 내부 인자 : 텍스트 추출 함수


void LSB_EVENT_OP_INST::_replace_text(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int* accum_count)
{
	if (_op_type == TYPE_EV_OP_CHAR) {

		if (is_Pure_Ascii_String(_str) || is_File_Path_String(_str)) { return; }
		//*굳이 대치할 필요 없으면 대치하지 않음

		wchar_t* cp_str = txt_list_elem->_txt_list_per_inst[(*accum_count)++];
		wchar_t* n_str = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(cp_str) + 1));
		n_str[wcslen(cp_str)] = 0;
		memcpy(n_str, cp_str, (sizeof(wchar_t) * wcslen(cp_str)));
		if (_str != NULL) { free(_str); }
		_str = n_str;
		//*메모리 복사 후 대치
		//*여기서 0x01, \r\n 문자열 대치는 굳이 할 필요가 없는 듯 하다

	}
	//*특성이 char일 때만
}
//*이벤트 내부 인자 : 텍스트 교체 함수


void LSB_EVENT_OP_INST::_change_code_J2K()
{
	JPHan_2_KRHan(_s);
	JPHan_2_KRHan(_str);
}
//*이벤트 내부 인자 : 텍스트 코드 변경 함수


void LSB_EV_TXT_ATTRIBUTE::_read_var_data(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);

	unsigned int* _int;
	unsigned char* _c;
	char* __s;

	_int = _get_int(&lsb_buffer_pnt);
	_ev_op_ref_count = *_int; free(_int);
	//*텍스트 커맨드? 읽어들이기

	_int = _get_int(&lsb_buffer_pnt);
	_txt_color = *_int; free(_int);
	_int = _get_int(&lsb_buffer_pnt);
	_unk_2 = *_int; free(_int);
	_int = _get_int(&lsb_buffer_pnt);
	_unk_3 = *_int; free(_int);
	//*모르는 값들 1, 2, 3

	_c = _get_byte(&lsb_buffer_pnt);
	_unk_4 = *_c; free(_c);
	_c = _get_byte(&lsb_buffer_pnt);
	_unk_5 = *_c; free(_c);
	//*모르는 값들 4, 5

	_unk_6 = 0;
	if (_tpword_ver < 0x64) {
		_c = _get_byte(&lsb_buffer_pnt);
		_unk_6 = *_c; free(_c);
	}
	//*버전에 따라 모르는 값 6

	_int = _get_int(&lsb_buffer_pnt);
	_unk_7 = *_int; free(_int);
	//*모르는 값 7

	__s = _get_str(&lsb_buffer_pnt);
	_font_name = GetUniStr (__s, code);
	free(__s);
	__s = _get_str(&lsb_buffer_pnt);
	_ruby_txt = GetUniStr(__s, code);
	free(__s);
	//*모르는 값 8, 9

	_unk_10 = _unk_11 = 0;
	if (_tpword_ver >= 0x64) {
		_int = _get_int(&lsb_buffer_pnt);
		_unk_10 = *_int; free(_int);
		_int = _get_int(&lsb_buffer_pnt);
		_unk_11 = *_int; free(_int);
	}
	//*버전에 따라 모르는 값 10, 11

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*이벤트 내부 속성 데이터 읽기


void LSB_EV_TXT_ATTRIBUTE::_decompile_code(HANDLE hWrite, unsigned int code, unsigned int _indent)
{
	for (unsigned int j = 0; j < (_indent); j++) { swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_); }
	//*들여쓰기 출력

	swprintf(_txt_buf + wcslen(_txt_buf),
		TEXT("[%s]::[%s:0x%08X][0x%08X][0x%08X][0x%02X][0x%02X][0x%02X][0x%08X][%s]%s%s%s[%s]%s%s%s[0x%08X][0x%08X]%s"),
		refer_attr_prefix, color_prefix, _txt_color, _unk_2, _unk_3, _unk_4, _unk_5, _unk_6, _unk_7,
		font_prefix, text_prefix, _font_name, text_prefix, ruby_txt_prefix, text_prefix, _ruby_txt, text_prefix,
		_unk_10, _unk_11, _enter_raw_);
	//*11개 값 순서대로 기록
}
//*이벤트 내부 속성 : 디컴파일 함수


void LSB_EV_TXT_ATTRIBUTE::_recompile_from_txt(wchar_t* _code_txt_buffer)
{
	wchar_t* _code_txt_pnt = _code_txt_buffer, *_code_txt_pnt_2 = _code_txt_buffer, *_swp;
	wchar_t _buf_for_int[11];
	wchar_t _buf_for_byte[5];
	memset(_buf_for_int, 0, (sizeof(wchar_t) * 11));
	memset(_buf_for_byte, 0, (sizeof(wchar_t) * 5));

	_code_txt_pnt_2 = wcsstr(_code_txt_pnt, color_prefix) + wcslen(color_prefix) + wcslen(TEXT(":"));
	memcpy(_buf_for_int, _code_txt_pnt_2, (sizeof(wchar_t) * 10));
	_txt_color = wcstoul(_buf_for_int, NULL, 16);
	//*color

	_code_txt_pnt = wcsstr(_code_txt_pnt_2, TEXT("[")) + wcslen(TEXT("["));
	memcpy(_buf_for_int, _code_txt_pnt, (sizeof(wchar_t) * 10));
	_unk_2 = wcstoul(_buf_for_int, NULL, 16);
	//*unk_2

	_code_txt_pnt_2 = wcsstr(_code_txt_pnt, TEXT("[")) + wcslen(TEXT("["));
	memcpy(_buf_for_int, _code_txt_pnt_2, (sizeof(wchar_t) * 10));
	_unk_3 = wcstoul(_buf_for_int, NULL, 16);
	//*unk_3

	_code_txt_pnt = wcsstr(_code_txt_pnt_2, TEXT("[")) + wcslen(TEXT("["));
	memcpy(_buf_for_byte, _code_txt_pnt, (sizeof(wchar_t) * 4));
	_unk_4 = (unsigned char)wcstoul(_buf_for_byte, NULL, 16);
	//*unk_4

	_code_txt_pnt_2 = wcsstr(_code_txt_pnt, TEXT("[")) + wcslen(TEXT("["));
	memcpy(_buf_for_byte, _code_txt_pnt_2, (sizeof(wchar_t) * 4));
	_unk_5 = (unsigned char)wcstoul(_buf_for_byte, NULL, 16);
	//*unk_5
	
	_code_txt_pnt = wcsstr(_code_txt_pnt_2, TEXT("[")) + wcslen(TEXT("["));
	memcpy(_buf_for_byte, _code_txt_pnt, (sizeof(wchar_t) * 4));
	if (_tpword_ver < 0x64) {
		_unk_6 = (unsigned char)wcstoul(_buf_for_byte, NULL, 16);
	}
	_swp = _code_txt_pnt;
	_code_txt_pnt = _code_txt_pnt_2;
	_code_txt_pnt_2 = _swp;
	//*unk_6 (_tpword_ver < 0x64)

	_code_txt_pnt = wcsstr(_code_txt_pnt_2, TEXT("[")) + wcslen(TEXT("["));
	memcpy(_buf_for_int, _code_txt_pnt, (sizeof(wchar_t) * 10));
	_unk_7 = wcstoul(_buf_for_int, NULL, 16);
	//*unk_7

	_code_txt_pnt_2 = wcsstr(_code_txt_pnt, font_prefix) + wcslen(font_prefix);
	_code_txt_pnt = wcsstr(_code_txt_pnt_2, text_prefix) + wcslen(text_prefix);
	_code_txt_pnt_2 = wcsstr(_code_txt_pnt, text_prefix);
	_font_name = (wchar_t*)malloc(sizeof(wchar_t) * (_code_txt_pnt_2 - _code_txt_pnt + 1));
	memset(_font_name, 0, (sizeof(wchar_t) * (_code_txt_pnt_2 - _code_txt_pnt + 1)));
	memcpy(_font_name, _code_txt_pnt, (sizeof(wchar_t) * (_code_txt_pnt_2 - _code_txt_pnt)));
	_code_txt_pnt_2 += wcslen(text_prefix);
	_swp = _code_txt_pnt;
	_code_txt_pnt = _code_txt_pnt_2;
	_code_txt_pnt_2 = _swp;
	//*font

	_code_txt_pnt_2 = wcsstr(_code_txt_pnt, ruby_txt_prefix) + wcslen(ruby_txt_prefix);
	_code_txt_pnt = wcsstr(_code_txt_pnt_2, text_prefix) + wcslen(text_prefix);
	_code_txt_pnt_2 = wcsstr(_code_txt_pnt, text_prefix);
	_ruby_txt = (wchar_t*)malloc(sizeof(wchar_t) * (_code_txt_pnt_2 - _code_txt_pnt + 1));
	memset(_ruby_txt, 0, (sizeof(wchar_t) * (_code_txt_pnt_2 - _code_txt_pnt + 1)));
	memcpy(_ruby_txt, _code_txt_pnt, (sizeof(wchar_t) * (_code_txt_pnt_2 - _code_txt_pnt)));
	_code_txt_pnt_2 += wcslen(text_prefix);
	//*ruby_txt
	
	_code_txt_pnt = wcsstr(_code_txt_pnt_2, TEXT("[")) + wcslen(TEXT("["));
	memcpy(_buf_for_int, _code_txt_pnt, (sizeof(wchar_t) * 10));
	if (_tpword_ver >= 0x64) {
		_unk_10 = wcstoul(_buf_for_int, NULL, 16);
	}
	//*unk_10 (_tpword_ver >= 0x64)

	_code_txt_pnt_2 = wcsstr(_code_txt_pnt, TEXT("[")) + wcslen(TEXT("["));
	memcpy(_buf_for_int, _code_txt_pnt_2, (sizeof(wchar_t) * 10));
	if (_tpword_ver >= 0x64) {
		_unk_11 = wcstoul(_buf_for_int, NULL, 16);
	}
	//*unk_11 (_tpword_ver >= 0x64)
	//*순서대로 읽어들이면 된다
}
//*이벤트 내부 속성 : 리컴파일 함수 (인자 : 해당 이벤트 블럭의 데이터가 들어있는 텍스트 데이터 일부)


void LSB_EV_TXT_ATTRIBUTE::_write_as_lsb(HANDLE hNewLsb, unsigned int code)
{
	char* __s;
	_write_int(hNewLsb, _ev_op_ref_count);
	_write_int(hNewLsb, _txt_color);
	_write_int(hNewLsb, _unk_2);
	_write_int(hNewLsb, _unk_3);
	_write_byte(hNewLsb, _unk_4);
	_write_byte(hNewLsb, _unk_5);

	if (_tpword_ver < 0x64) {
		_write_byte(hNewLsb, _unk_6);
	}

	_write_int(hNewLsb, _unk_7);

	__s = GetAnsiStr(_font_name, code);
	_write_str(hNewLsb, __s);
	free(__s);

	__s = GetAnsiStr(_ruby_txt, code);
	_write_str(hNewLsb, __s);
	free(__s);

	if (_tpword_ver >= 0x64) {
		_write_int(hNewLsb, _unk_10);
		_write_int(hNewLsb, _unk_11);
	}
	//*11개 값 순서대로 기록
}
//*이벤트 내부 속성 : lsb 파일로 기록하는 함수


void LSB_EV_TXT_ATTRIBUTE::_extract_text(HANDLE hTxt)
{
	for (unsigned int i = 0; i < 2;i++) {
		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s"), _indent_);
	}
	//*들여쓰기 2번

	swprintf(_txt_buf + wcslen(_txt_buf),
		TEXT("[%s]::[%s:0x%08X][0x%08X][0x%08X][0x%02X][0x%02X][0x%02X][0x%08X][%s]%s%s%s[%s]%s%s%s[0x%08X][0x%08X]%s"),
		refer_attr_prefix, color_prefix, _txt_color, _unk_2, _unk_3, _unk_4, _unk_5, _unk_6, _unk_7,
		font_prefix, text_prefix, _font_name, text_prefix, ruby_txt_prefix, text_prefix, _ruby_txt, text_prefix,
		_unk_10, _unk_11, _enter_raw_);
	//*출력해 주기
}
//*이벤트 내부 변수 : 텍스트 추출 함수


void LSB_EV_TXT_ATTRIBUTE::_change_code_J2K()
{
	JPHan_2_KRHan(_font_name);
	JPHan_2_KRHan(_ruby_txt);
}
//*이벤트 내부 변수 : 텍스트 코드 변경 함수
//---------------------------- 이벤트 관련 클래스 ----------------------------//