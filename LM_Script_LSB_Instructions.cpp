#include "LM_Script_LSB.h"
#include "LM_Functions.h"


#pragma warning (disable:6031)
#pragma warning (disable:4996)

//*** lsb 스크립트의 명령어들만을 따와서 서술하는 파일


//---------------------------- if 명령어 ----------------------------//
LSB_INST_IF::LSB_INST_IF (LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER *_header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 (TEXT("Calc"))
}
//*생성자

void LSB_INST_IF::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Calc");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Calc"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_IF::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_IF::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	LSB_PARAMETERS* _t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	unsigned int accum_count = 0;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- if 명령어 ----------------------------//



//---------------------------- elif 명령어 ----------------------------//
LSB_INST_ELIF::LSB_INST_ELIF(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 (TEXT("Calc"))
}
//*생성자

void LSB_INST_ELIF::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Calc");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Calc"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_ELIF::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_ELIF::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	LSB_PARAMETERS* _t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	unsigned int accum_count = 0;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- elif 명령어 ----------------------------//



//---------------------------- else 명령어 ----------------------------//
LSB_INST_ELSE::LSB_INST_ELSE(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	//*필요한 패러미터 리스트 수 : 0개
}
//*생성자

void LSB_INST_ELSE::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_ELSE::_extract_text_each_inst(HANDLE hTxt)
{
	//*별도 작업 필요 없음
}
//*텍스트를 추출하는 함수

void LSB_INST_ELSE::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*텍스트를 대치하는 함수
//---------------------------- else 명령어 ----------------------------//



//---------------------------- label 명령어 ----------------------------//
LSB_INST_LABEL::LSB_INST_LABEL(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 (TEXT("Label"))
}
//*생성자

void LSB_INST_LABEL::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	char* _s;

	_data_chunks[0]._str = TEXT("Label");
	_s = _get_str(&lsb_buffer_pnt);
	_data_chunks[0]._data = GetUniStr (_s, code);
	free(_s);
	//* 데이터 읽어들이기 (TEXT("Label"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_LABEL::_extract_text_each_inst(HANDLE hTxt)
{
	wchar_t* _print_str;
	
	_print_str = (wchar_t*)_data_chunks[0]._data;
	if (!is_Pure_Ascii_String(_print_str) && !is_File_Path_String(_print_str)) {
		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[%s]%s%s%s%s"),
			_indent_, text_data_prefix, text_prefix, _print_str, text_prefix, _enter_raw_);
	}
	//*번역할 필요가 있을 때만 출력
}
//*텍스트를 추출하는 함수

void LSB_INST_LABEL::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	wchar_t* cp_txt, *n_txt;

	if (!is_Pure_Ascii_String((wchar_t*)_data_chunks[0]._data)
		&& !is_File_Path_String((wchar_t*)_data_chunks[0]._data)) {
		if (_data_chunks[0]._data != NULL) { free(_data_chunks[0]._data); }
		//*이미 데이터가 있으면 해제함
		cp_txt = txt_list_elem->_txt_list_per_inst[0];
		n_txt = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(cp_txt) + 1));
		n_txt[wcslen(cp_txt)] = 0;
		memcpy(n_txt, cp_txt, (sizeof(wchar_t) * wcslen(cp_txt)));
		_data_chunks[0]._data = n_txt;
		//*메모리 복사 후 대치
	}
	//*번역할 필요가 있을 때만 대치한다
}
//*텍스트를 대치하는 함수
//---------------------------- label 명령어 ----------------------------//



//---------------------------- jump 명령어 ----------------------------//
LSB_INST_JUMP::LSB_INST_JUMP(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 3;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 3개 (TEXT("Script_Page"), TEXT("Target_Label_Offset"), TEXT("Calc"))
}
//*생성자

void LSB_INST_JUMP::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;
	char* _s;

	_data_chunks[0]._str = TEXT("Script_Page");
	_s = _get_str(&lsb_buffer_pnt);
	_data_chunks[0]._data = GetUniStr(_s, code);
	free(_s);
	//* 데이터 읽어들이기 (TEXT("Script_Page"))

	_data_chunks[1]._str = TEXT("Target_Label_Offset");
	_data_chunks[1]._data = _get_int(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("Target_Label_Offset"))

	_data_chunks[2]._str = TEXT("Calc");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[2]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Calc"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_JUMP::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt = (LSB_PARAMETERS*)_data_chunks[2]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_JUMP::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	LSB_PARAMETERS* _t = (LSB_PARAMETERS*)_data_chunks[2]._data;
	unsigned int accum_count = 0;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- jump 명령어 ----------------------------//



//---------------------------- call 명령어 ----------------------------//
LSB_INST_CALL::LSB_INST_CALL(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 5;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임
	//*TEXT("Script_Page"), TEXT("Target_Label_Offset"), TEXT("Result"), TEXT("Calc"), TEXT("Parameter_Count") 이후 인자에 따라 달라짐
	//*먼저 5개만 할당한 후 다시 옮기는 식으로 해야 할 듯 하다
}
//*생성자

void LSB_INST_CALL::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;
	char* _s;

	_data_chunks[0]._str = TEXT("Script_Page");
	_s = _get_str(&lsb_buffer_pnt);
	_data_chunks[0]._data = GetUniStr(_s, code);
	free(_s);
	//* 데이터 읽어들이기 (TEXT("Script_Page"))

	_data_chunks[1]._str = TEXT("Target_Label_Offset");
	_data_chunks[1]._data = _get_int(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("Target_Label_Offset"))

	_data_chunks[2]._str = TEXT("Result");
	_s = _get_str(&lsb_buffer_pnt);
	_data_chunks[2]._data = GetUniStr(_s, code);
	free(_s);
	//* 데이터 읽어들이기 (TEXT("Result"))

	_data_chunks[3]._str = TEXT("Calc");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[3]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Calc"))

	_data_chunks[4]._str = TEXT("Parameter_Count");
	_data_chunks[4]._data = _get_int(&lsb_buffer_pnt);;
	//* 데이터 읽어들이기 (TEXT("Parameter_Count"))
	
	unsigned int _param_count = *(unsigned int*)_data_chunks[4]._data;
	LSB_MAP_DATA *_n_map = _get_initialized_data_chunks (_data_chunks_count + _param_count);
	memcpy(_n_map, _data_chunks, (sizeof(LSB_MAP_DATA) * _data_chunks_count));
	for (unsigned int i = 0; i < _param_count;i++) {
		_n_map[_data_chunks_count + i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_n_map[_data_chunks_count + i]._data = _pnt;
	}
	//* 제대로 된 개수대로 할당 후 복사 및 마저 읽어들이기

	_data_chunks_count += _param_count;
	free(_data_chunks);
	_data_chunks = _n_map;
	//*다시 갱신

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_CALL::_extract_text_each_inst(HANDLE hTxt)
{
	unsigned int _param_cnt = *(unsigned int*)_data_chunks[4]._data;
	//*인자 수 파악
	for (unsigned int i = 0; i < _param_cnt; i++) {
		LSB_PARAMETERS* _pnt = (LSB_PARAMETERS*)_data_chunks[5 + i]._data;
		_pnt->_extract_text(hTxt);
		//*패러미터 리스트에서 추출
	}
	//*인자 목록에서 문자열인 인자만 골라 추출
}
//*텍스트를 추출하는 함수

void LSB_INST_CALL::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int _param_cnt = *(unsigned int*)_data_chunks[4]._data;
	//*인자 수 파악
	unsigned int accum_count = 0;
	for (unsigned int i = 0; i < _param_cnt; i++) {
		LSB_PARAMETERS* _pnt = (LSB_PARAMETERS*)_data_chunks[5 + i]._data;
		_pnt->_replace_text(txt_list_elem, &accum_count);
		//*패러미터 리스트에서 대치
	}
}
//*텍스트를 대치하는 함수
//---------------------------- call 명령어 ----------------------------//



//---------------------------- exit 명령어 ----------------------------//
LSB_INST_EXIT::LSB_INST_EXIT(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 (TEXT("Calc"))
}
//*생성자

void LSB_INST_EXIT::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Calc");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Calc"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_EXIT::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_EXIT::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	LSB_PARAMETERS* _t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	unsigned int accum_count = 0;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- exit 명령어 ----------------------------//



//---------------------------- wait 명령어 ----------------------------//
LSB_INST_WAIT::LSB_INST_WAIT(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 2;
	if (_script_ver >= 0x6B) { _data_chunks_count++; }
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 2개 혹은 3개 (TEXT("Calc"), TEXT("Time"), 버전에 따라 TEXT("StopEvent"))
}
//*생성자

void LSB_INST_WAIT::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Calc");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Calc"))

	_data_chunks[1]._str = TEXT("Time");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[1]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Time"))

	if (_script_ver >= 0x6B) {
		_data_chunks[2]._str = TEXT("StopEvent");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[2]._data = _pnt;
	}
	//*버전에 따라서 추가로 더 읽음

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_WAIT::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	_pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)

	_pnt = (LSB_PARAMETERS*)_data_chunks[1]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)

	if (_script_ver >= 0x6B) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[2]._data;
		_pnt->_extract_text(hTxt);
	}
	//*(버전 구분) 인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_WAIT::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	_t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치

	_t = (LSB_PARAMETERS*)_data_chunks[1]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치

	if (_script_ver >= 0x6B) {
		_t = (LSB_PARAMETERS*)_data_chunks[2]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*(버전 구분)패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- wait 명령어 ----------------------------//



//---------------------------- timer 명령어 ----------------------------//
LSB_INST_TIMER::LSB_INST_TIMER(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8);i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_TIMER::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_TIMER::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_TIMER::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- timer 명령어 ----------------------------//



//---------------------------- caculation function(특수함수) 명령어 ----------------------------//
LSB_INST_CALC_FUNC::LSB_INST_CALC_FUNC(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 (TEXT("Calc"))
}
//*생성자

void LSB_INST_CALC_FUNC::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Calc");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Calc"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_CALC_FUNC::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_CALC_FUNC::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	LSB_PARAMETERS* _t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	unsigned int accum_count = 0;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- caculation function(특수함수) 명령어 ----------------------------//



//---------------------------- box_new 명령어 ----------------------------//
LSB_INST_BOX_NEW::LSB_INST_BOX_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_BOX_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_BOX_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_BOX_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- box_new 명령어 ----------------------------//



//---------------------------- image_new 명령어 ----------------------------//
LSB_INST_IMAGE_NEW::LSB_INST_IMAGE_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_IMAGE_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_IMAGE_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_IMAGE_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- image_new 명령어 ----------------------------//



//---------------------------- message_new 명령어 ----------------------------//
LSB_INST_MESSAGE_NEW::LSB_INST_MESSAGE_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_MESSAGE_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_MESSAGE_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_MESSAGE_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- message_new 명령어 ----------------------------//



//---------------------------- particle_new 명령어 ----------------------------//
LSB_INST_PARTICLE_NEW::LSB_INST_PARTICLE_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_PARTICLE_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_PARTICLE_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_PARTICLE_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- particle_new 명령어 ----------------------------//



//---------------------------- fire_new 명령어 ----------------------------//
LSB_INST_FIRE_NEW::LSB_INST_FIRE_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_FIRE_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_FIRE_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_FIRE_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- fire_new 명령어 ----------------------------//



//---------------------------- edit_new 명령어 ----------------------------//
LSB_INST_EDIT_NEW::LSB_INST_EDIT_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_EDIT_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_EDIT_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_EDIT_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- edit_new 명령어 ----------------------------//



//---------------------------- memo_new 명령어 ----------------------------//
LSB_INST_MEMO_NEW::LSB_INST_MEMO_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_MEMO_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_MEMO_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_MEMO_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- memo_new 명령어 ----------------------------//



//---------------------------- map_image_new 명령어 ----------------------------//
LSB_INST_MAP_IMAGE_NEW::LSB_INST_MAP_IMAGE_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_MAP_IMAGE_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_MAP_IMAGE_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_MAP_IMAGE_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- map_image_new 명령어 ----------------------------//



//---------------------------- wave_new 명령어 ----------------------------//
LSB_INST_WAVE_NEW::LSB_INST_WAVE_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_WAVE_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_WAVE_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_WAVE_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- wave_new 명령어 ----------------------------//



//---------------------------- tile_new 명령어 ----------------------------//
LSB_INST_TILE_NEW::LSB_INST_TILE_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_TILE_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_TILE_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_TILE_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- tile_new 명령어 ----------------------------//



//---------------------------- slider_new 명령어 ----------------------------//
LSB_INST_SLIDER_NEW::LSB_INST_SLIDER_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_SLIDER_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_SLIDER_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_SLIDER_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- slider_new 명령어 ----------------------------//



//---------------------------- scroll_bar_new 명령어 ----------------------------//
LSB_INST_SCROLL_BAR_NEW::LSB_INST_SCROLL_BAR_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_SCROLL_BAR_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_SCROLL_BAR_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_SCROLL_BAR_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- scroll_bar_new 명령어 ----------------------------//



//---------------------------- gauge_new 명령어 ----------------------------//
LSB_INST_GAUGE_NEW::LSB_INST_GAUGE_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_GAUGE_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_GAUGE_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_GAUGE_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- gauge_new 명령어 ----------------------------//



//---------------------------- prev_menu_new 명령어 ----------------------------//
LSB_INST_PREV_MENU_NEW::LSB_INST_PREV_MENU_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_PREV_MENU_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_PREV_MENU_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_PREV_MENU_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- prev_menu_new 명령어 ----------------------------//



//---------------------------- var_new 명령어 ----------------------------//
LSB_INST_VAR_NEW::LSB_INST_VAR_NEW(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 4;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 4개 (TEXT("Var_Name"), TEXT("Var_Type"), TEXT("Var_Initlal_Value"), TEXT("Var_Scope_Byte"))
}
//*생성자

void LSB_INST_VAR_NEW::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;
	char* _s;

	_data_chunks[0]._str = TEXT("Var_Name");
	_s = _get_str(&lsb_buffer_pnt);
	_data_chunks[0]._data = GetUniStr (_s, code);
	free(_s);
	//* 데이터 읽어들이기 (TEXT("Var_Name"))

	_data_chunks[1]._str = TEXT("Var_Type");
	_data_chunks[1]._data = _get_byte(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("Var_Type"))

	_data_chunks[2]._str = TEXT("Var_Initlal_Value");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[2]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Var_Initlal_Value"))

	_data_chunks[3]._str = TEXT("Var_Scope_Byte");
	_data_chunks[3]._data = _get_byte(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("Var_Scope_Byte"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_VAR_NEW::_extract_text_each_inst(HANDLE hTxt)
{
	wchar_t* _print_str;
	LSB_PARAMETERS* _pnt;

	_print_str = (wchar_t*)_data_chunks[0]._data;
	if (!is_Pure_Ascii_String(_print_str) && !is_File_Path_String(_print_str)) {
		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[%s]%s%s%s%s"),
			_indent_, text_data_prefix, text_prefix, _print_str, text_prefix, _enter_raw_);
	}
	//*번역할 필요가 있을 때만 출력

	_pnt = (LSB_PARAMETERS*)_data_chunks[2]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_VAR_NEW::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;
	wchar_t* cp_txt, * n_txt;

	if (!is_Pure_Ascii_String((wchar_t*)_data_chunks[0]._data)
		&& !is_File_Path_String((wchar_t*)_data_chunks[0]._data)) {
		if (_data_chunks[0]._data != NULL) { free(_data_chunks[0]._data); }
		//*이미 데이터가 있으면 해제함
		cp_txt = txt_list_elem->_txt_list_per_inst[0];
		n_txt = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(cp_txt) + 1));
		n_txt[wcslen(cp_txt)] = 0;
		memcpy(n_txt, cp_txt, (sizeof(wchar_t) * wcslen(cp_txt)));
		_data_chunks[0]._data = n_txt;
		//*메모리 복사 후 대치
		accum_count++;
	}
	//*번역할 필요가 있을 때만 대치한다

	_t = (LSB_PARAMETERS*)_data_chunks[2]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- var_new 명령어 ----------------------------//



//---------------------------- var_delete 명령어 ----------------------------//
LSB_INST_VAR_DELETE::LSB_INST_VAR_DELETE(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 (TEXT("Var_Name"))
}
//*생성자

void LSB_INST_VAR_DELETE::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
//	LSB_PARAMETERS* _pnt;
	char* _s;

	_data_chunks[0]._str = TEXT("Var_Name");
	_s = _get_str(&lsb_buffer_pnt);
	_data_chunks[0]._data = GetUniStr(_s, code);
	free(_s);
	//* 데이터 읽어들이기 (TEXT("Var_Name"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_VAR_DELETE::_extract_text_each_inst(HANDLE hTxt)
{
	wchar_t* _print_str;

	_print_str = (wchar_t*)_data_chunks[0]._data;
	if (!is_Pure_Ascii_String(_print_str) && !is_File_Path_String(_print_str)) {
		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[%s]%s%s%s%s"),
			_indent_, text_data_prefix, text_prefix, _print_str, text_prefix, _enter_raw_);
	}
	//*번역할 필요가 있을 때만 출력
}
//*텍스트를 추출하는 함수

void LSB_INST_VAR_DELETE::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	wchar_t* cp_txt, * n_txt;

	if (!is_Pure_Ascii_String((wchar_t*)_data_chunks[0]._data)
		&& !is_File_Path_String((wchar_t*)_data_chunks[0]._data)) {
		if (_data_chunks[0]._data != NULL) { free(_data_chunks[0]._data); }
		//*이미 데이터가 있으면 해제함
		cp_txt = txt_list_elem->_txt_list_per_inst[0];
		n_txt = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(cp_txt) + 1));
		n_txt[wcslen(cp_txt)] = 0;
		memcpy(n_txt, cp_txt, (sizeof(wchar_t) * wcslen(cp_txt)));
		_data_chunks[0]._data = n_txt;
		//*메모리 복사 후 대치
		accum_count++;
	}
	//*번역할 필요가 있을 때만 대치한다
}
//*텍스트를 대치하는 함수
//---------------------------- var_delete 명령어 ----------------------------//



//---------------------------- movie 명령어 ----------------------------//
LSB_INST_MOVIE::LSB_INST_MOVIE(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_MOVIE::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_MOVIE::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_MOVIE::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- movie 명령어 ----------------------------//



//---------------------------- flip 명령어 ----------------------------//
LSB_INST_FLIP::LSB_INST_FLIP(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 8;
	if (_script_ver >= 0x65) { _data_chunks_count++; }
	if (_script_ver >= 0x6B) { _data_chunks_count++; }
	if (_script_ver >= LM_VERSION_3) { _data_chunks_count++; }
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (TEXT("Wipe"), TEXT("Time") 등 최소 8개)
	//(TEXT("Wipe"), TEXT("Time"), TEXT("Reverse"), TEXT("Act"), TEXT("Parameter_Count"), [패러미터리스트 수만큼], TEXT("Delete"), TEXT("Item_1"), TEXT("Item_2"),
	// 0x65 이상이면 TEXT("Source") 추가, 0x6B 이상이면 TEXT("StopEvent") 추가, 0x75 이상이면 TEXT("DifferenceOnly") 추가)
}
//*생성자

void LSB_INST_FLIP::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Wipe");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Wipe"))

	_data_chunks[1]._str = TEXT("Time");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[1]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Time"))

	_data_chunks[2]._str = TEXT("Reverse");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[2]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Reverse"))

	_data_chunks[3]._str = TEXT("Act");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[3]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Act"))

	_data_chunks[4]._str = TEXT("Parameter_Count");
	_data_chunks[4]._data = _get_int(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("Parameter_Count"))
	
	unsigned int _param_count = *(unsigned int*)_data_chunks[4]._data;
	LSB_MAP_DATA* _n_map = _get_initialized_data_chunks(_data_chunks_count + _param_count);
	memcpy(_n_map, _data_chunks, (sizeof(LSB_MAP_DATA) * 5));
	for (unsigned int i = 0; i < _param_count; i++) {
		_n_map[5 + i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_n_map[5 + i]._data = _pnt;
	}
	//* 제대로 된 개수대로 할당 후 복사 및 마저 읽어들이기

	_data_chunks_count += _param_count;
	free(_data_chunks);
	_data_chunks = _n_map;
	//*다시 갱신

	_data_chunks[(5 + _param_count)]._str = TEXT("Delete");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[(5 + _param_count)]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Delete"))

	_data_chunks[(5 + _param_count) + 1]._str = TEXT("Item_1");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[(5 + _param_count) + 1]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Item_1"))

	_data_chunks[(5 + _param_count) + 2]._str = TEXT("Item_2");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[(5 + _param_count) + 2]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Item_2"))

	if (_script_ver >= 0x65) {
		_data_chunks[(5 + _param_count) + 3]._str = TEXT("Source");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[(5 + _param_count) + 3]._data = _pnt;
	}
	//* 0x65 이상이면 데이터 읽어들이기 (TEXT("Source"))

	if (_script_ver >= 0x6B) {
		_data_chunks[(5 + _param_count) + 4]._str = TEXT("StopEvent");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[(5 + _param_count) + 4]._data = _pnt;
	}
	//* 0x6B 이상이면 데이터 읽어들이기 (TEXT("StopEvent"))

	if (_script_ver >= LM_VERSION_3) {
		_data_chunks[(5 + _param_count) + 5]._str = TEXT("DifferenceOnly");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[(5 + _param_count) + 5]._data = _pnt;
	}
	//* 0x75 이상이면 데이터 읽어들이기 (TEXT("DifferenceOnly"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_FLIP::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < 4; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출

	unsigned int _param_count = *(unsigned int*)_data_chunks[4]._data;
	for (unsigned int i = 0; i < _param_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[5 + i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*개수 계산 후 인자 목록에서 문자열인 인자만 골라 추출

	for (unsigned int i = (5 + _param_count); i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*개수 계산 후 인자 목록에서 문자열인 인자만 골라 추출
}
//*텍스트를 추출하는 함수

void LSB_INST_FLIP::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < 4; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치

	unsigned int _param_count = *(unsigned int*)_data_chunks[4]._data;
	for (unsigned int i = 0; i < _param_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[5 + i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*개수 계산 후 패러미터 리스트에서 대치

	for (unsigned int i = (5 + _param_count); i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*버전에 따라 패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- flip 명령어 ----------------------------//



//---------------------------- movie_stop 명령어 ----------------------------//
LSB_INST_MOVIE_STOP::LSB_INST_MOVIE_STOP(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 3;
	if (_script_ver >= 0x6B) { _data_chunks_count++; }
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 3개 혹은 4개 (TEXT("Target"), TEXT("Time"), TEXT("Wait"), 0x6B 이상이면 TEXT("DifferentVer")(?) 추가)
}
//*생성자

void LSB_INST_MOVIE_STOP::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Target");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Target"))

	_data_chunks[1]._str = TEXT("Time");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[1]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Time"))

	_data_chunks[2]._str = TEXT("Wait");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[2]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Wait"))

	if (_script_ver >= 0x6B) {
		_data_chunks[3]._str = TEXT("DifferentVer");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[3]._data = _pnt;
	}
	//* 0x6B 이상이면 데이터 읽어들이기 (TEXT("DifferentVer")(?))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_MOVIE_STOP::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < 3; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출

	if (_script_ver >= 0x6B) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[3]._data;
		_pnt->_extract_text(hTxt);
	}
	//*버전에 따라 인자 목록에서 문자열인 인자만 골라 추출
}
//*텍스트를 추출하는 함수

void LSB_INST_MOVIE_STOP::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < 3; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치

	if (_script_ver >= 0x6B) {
		_t = (LSB_PARAMETERS*)_data_chunks[3]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*버전에 따라 패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- movie_stop 명령어 ----------------------------//



//---------------------------- cinema 명령어 ----------------------------//
LSB_INST_CINEMA::LSB_INST_CINEMA(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_CINEMA::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_CINEMA::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_CINEMA::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- cinema 명령어 ----------------------------//



//---------------------------- get_property 명령어 ----------------------------//
LSB_INST_GET_PROPERTY::LSB_INST_GET_PROPERTY(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 3;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 3개 (TEXT("ObjName"), TEXT("ObjProp"), TEXT("Var_Name"))
}
//*생성자

void LSB_INST_GET_PROPERTY::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;
	char* _s;

	_data_chunks[0]._str = TEXT("ObjName");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("ObjName"))

	_data_chunks[1]._str = TEXT("ObjProp");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[1]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("ObjProp"))

	_data_chunks[2]._str = TEXT("Var_Name");
	_s = _get_str(&lsb_buffer_pnt);
	_data_chunks[2]._data = GetUniStr (_s, code);
	free(_s);
	//* 데이터 읽어들이기 (TEXT("Var_Name"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_GET_PROPERTY::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;
	wchar_t* _print_str;

	for (unsigned int i = 0; i < 2; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)

	_print_str = (wchar_t*)_data_chunks[2]._data;
	if (!is_Pure_Ascii_String(_print_str) && !is_File_Path_String(_print_str)) {
		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[%s]%s%s%s%s"),
			_indent_, text_data_prefix, text_prefix, _print_str, text_prefix, _enter_raw_);
	}
	//*번역할 필요가 있을 때만 출력
}
//*텍스트를 추출하는 함수

void LSB_INST_GET_PROPERTY::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;
	wchar_t* cp_txt, * n_txt;

	for (unsigned int i = 0; i < 2; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치

	if (!is_Pure_Ascii_String((wchar_t*)_data_chunks[2]._data)
		&& !is_File_Path_String((wchar_t*)_data_chunks[2]._data)) {
		if (_data_chunks[2]._data != NULL) { free(_data_chunks[2]._data); }
		//*이미 데이터가 있으면 해제함
		cp_txt = txt_list_elem->_txt_list_per_inst[accum_count];
		n_txt = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(cp_txt) + 1));
		n_txt[wcslen(cp_txt)] = 0;
		memcpy(n_txt, cp_txt, (sizeof(wchar_t) * wcslen(cp_txt)));
		_data_chunks[2]._data = n_txt;
		//*메모리 복사 후 대치
		accum_count++;
	}
	//*번역할 필요가 있을 때만 대치한다
}
//*텍스트를 대치하는 함수
//---------------------------- get_property 명령어 ----------------------------//



//---------------------------- set_property 명령어 ----------------------------//
LSB_INST_SET_PROPERTY::LSB_INST_SET_PROPERTY(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 3;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 3개 (TEXT("ObjName"), TEXT("ObjProp"), TEXT("Set_Value"))
}
//*생성자

void LSB_INST_SET_PROPERTY::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("ObjName");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("ObjName"))

	_data_chunks[1]._str = TEXT("ObjProp");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[1]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("ObjProp"))

	_data_chunks[2]._str = TEXT("Set_Value");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[2]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Set_Value"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_SET_PROPERTY::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < 3; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_SET_PROPERTY::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < 3; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- set_property 명령어 ----------------------------//



//---------------------------- object_delete 명령어 ----------------------------//
LSB_INST_OBJECT_DELETE::LSB_INST_OBJECT_DELETE(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 (TEXT("ObjName"))
}
//*생성자

void LSB_INST_OBJECT_DELETE::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("ObjName");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("ObjName"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_OBJECT_DELETE::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	_pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_OBJECT_DELETE::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	_t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- object_delete 명령어 ----------------------------//



//---------------------------- text_inst 명령어 ----------------------------//
LSB_INST_TEXT_INST::LSB_INST_TEXT_INST(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 4;
	if (_script_ver >= 0x6B) { _data_chunks_count++; }
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 4개 혹은 5개 (TEXT("Event_Block"), TEXT("Unknown_1"), TEXT("Unknown_2"), TEXT("Unknown_3"), 버전에 따라 TEXT("Unknown_4"))
}
//*생성자

void LSB_INST_TEXT_INST::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;
	LSB_EVENT* _pnt_e;

	_data_chunks[0]._str = TEXT("Event_Block");
	_pnt_e = new LSB_EVENT ();
	_pnt_e->_read_event_script(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt_e;
	//* 데이터 읽어들이기 (TEXT("Event_Block"))

	_data_chunks[1]._str = TEXT("Unknown_1");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[1]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Unknown_1"))

	_data_chunks[2]._str = TEXT("Unknown_2");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[2]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Unknown_2"))

	_data_chunks[3]._str = TEXT("Unknown_3");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[3]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Unknown_3"))

	if (_script_ver >= 0x6B) {
		_data_chunks[4]._str = TEXT("Unknown_4");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[4]._data = _pnt;
	}
	//* 0x6B 이상이면 데이터 읽어들이기 (TEXT("Unknown_4"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_TEXT_INST::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_EVENT* _t = (LSB_EVENT*)_data_chunks[0]._data;
	_t->_extract_text(hTxt);
	//*텍스트 명령어 시 이벤트 블럭에서만 추출
}
//*텍스트를 추출하는 함수

void LSB_INST_TEXT_INST::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	LSB_EVENT* _n_ev = new LSB_EVENT();
	LSB_EVENT* _t = (LSB_EVENT*)_data_chunks[0]._data;
	//*미리 준비

	wchar_t* _tp = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(_t->_tpword) + 1));
	memset(_tp, 0, (sizeof(wchar_t) * (wcslen(_t->_tpword) + 1)));
	memcpy(_tp, _t->_tpword, (sizeof(wchar_t) * wcslen(_t->_tpword)));
	_n_ev->_tpword = _tp;
	_tp = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(_t->_ver) + 1));
	memset(_tp, 0, (sizeof(wchar_t) * (wcslen(_t->_ver) + 1)));
	memcpy(_tp, _t->_ver, (sizeof(wchar_t) * wcslen(_t->_ver)));
	_n_ev->_ver = _tp;
	_n_ev->_tpword_ver = wcstol(_tp, NULL, 10);
	//*필요한 값들만 deep copy로 복사해옴

	_n_ev->_recompile_from_txt(txt_list_elem->_txt_list_per_inst[0], code);
	//*여기 들어온 텍스트는 뭉탱이로 들어온 텍스트 하나뿐이니 잘게 쪼개서 다시 만들어줘야 함

	delete _t;
	_data_chunks[0]._data = _n_ev;
	//*그리고 현재 이벤트 블럭 해제 후 이것으로 세팅
}
//*텍스트를 대치하는 함수
//---------------------------- text_inst 명령어 ----------------------------//



//---------------------------- text_clear 명령어 ----------------------------//
LSB_INST_TEXT_CLEAR::LSB_INST_TEXT_CLEAR(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 (TEXT("Target"))
}
//*생성자

void LSB_INST_TEXT_CLEAR::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Target");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Target"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_TEXT_CLEAR::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	_pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_TEXT_CLEAR::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	_t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- text_clear 명령어 ----------------------------//



//---------------------------- clear_history 명령어 ----------------------------//
LSB_INST_CLEAR_HISTORY::LSB_INST_CLEAR_HISTORY(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	//*필요한 패러미터 리스트 수 : 0개
}
//*생성자

void LSB_INST_CLEAR_HISTORY::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_CLEAR_HISTORY::_extract_text_each_inst(HANDLE hTxt)
{
	//*별도 작업 필요 없음
}
//*텍스트를 추출하는 함수

void LSB_INST_CLEAR_HISTORY::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*텍스트를 대치하는 함수
//---------------------------- clear_history 명령어 ----------------------------//



//---------------------------- call_history 명령어 ----------------------------//
LSB_INST_CALL_HISTORY::LSB_INST_CALL_HISTORY(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 4;
	if (_script_ver >= 0x6F) { _data_chunks_count++; }
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 4개 혹은 5개 (TEXT("Target"), TEXT("Index"), TEXT("Count"), TEXT("CutBreak"), 버전에 따라 TEXT("FormatName"))
}
//*생성자

void LSB_INST_CALL_HISTORY::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Target");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Target"))

	_data_chunks[1]._str = TEXT("Index");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[1]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Index"))

	_data_chunks[2]._str = TEXT("Count");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[2]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Count"))

	_data_chunks[3]._str = TEXT("CutBreak");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[3]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("CutBreak"))

	if (_script_ver >= 0x6F) {
		_data_chunks[4]._str = TEXT("FormatName");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[4]._data = _pnt;
	}
	//* 0x6F 이상이면 데이터 읽어들이기 (TEXT("FormatName"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_CALL_HISTORY::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < 4; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)

	if (_script_ver >= 0x6F) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[4]._data;
		_pnt->_extract_text(hTxt);
	}
	//*버전에 따라 인자 목록에서 문자열인 인자만 골라 추출
}
//*텍스트를 추출하는 함수

void LSB_INST_CALL_HISTORY::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < 4; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치

	if (_script_ver >= 0x6F) {
		_t = (LSB_PARAMETERS*)_data_chunks[4]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*버전에 따라 패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- call_history 명령어 ----------------------------//



//---------------------------- format_history 명령어 ----------------------------//
LSB_INST_FORMAT_HISTORY::LSB_INST_FORMAT_HISTORY(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	if (_script_ver >= 0x6F) { _data_chunks_count++; }
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 혹은 2개 (TEXT("Name"), 버전에 따라 TEXT("Unknown"))
}
//*생성자

void LSB_INST_FORMAT_HISTORY::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Name");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Name"))

	if (_script_ver >= 0x6F) {
		_data_chunks[1]._str = TEXT("Unknown");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[1]._data = _pnt;
	}
	//* 0x6F 이상이면 데이터 읽어들이기 (TEXT("Unknown"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_FORMAT_HISTORY::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	_pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)

	if (_script_ver >= 0x6F) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[1]._data;
		_pnt->_extract_text(hTxt);
	}
	//*버전에 따라 인자 목록에서 문자열인 인자만 골라 추출
}
//*텍스트를 추출하는 함수

void LSB_INST_FORMAT_HISTORY::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	_t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치

	if (_script_ver >= 0x6F) {
		_t = (LSB_PARAMETERS*)_data_chunks[1]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*버전에 따라 패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- format_history 명령어 ----------------------------//



//---------------------------- caption 명령어 ----------------------------//
LSB_INST_CAPTION::LSB_INST_CAPTION(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_CAPTION::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_CAPTION::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_CAPTION::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- caption 명령어 ----------------------------//



//---------------------------- cg_caption 명령어 ----------------------------//
LSB_INST_CG_CAPTION::LSB_INST_CG_CAPTION(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_CG_CAPTION::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_CG_CAPTION::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_CG_CAPTION::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- cg_caption 명령어 ----------------------------//



//---------------------------- menu 명령어 ----------------------------//
LSB_INST_MENU::LSB_INST_MENU(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_MENU::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_MENU::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_MENU::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- menu 명령어 ----------------------------//



//---------------------------- menu_close 명령어 ----------------------------//
LSB_INST_MENU_CLOSE::LSB_INST_MENU_CLOSE(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 (TEXT("Target"))
}
//*생성자

void LSB_INST_MENU_CLOSE::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Target");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Target"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_MENU_CLOSE::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	_pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_MENU_CLOSE::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	_t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- menu_close 명령어 ----------------------------//



//---------------------------- button 명령어 ----------------------------//
LSB_INST_BUTTON::LSB_INST_BUTTON(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_BUTTON::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_BUTTON::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_BUTTON::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- button 명령어 ----------------------------//



//---------------------------- comment 명령어 ----------------------------//
LSB_INST_COMMENT::LSB_INST_COMMENT(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 (TEXT("Label"))
}
//*생성자

void LSB_INST_COMMENT::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	char* _s;

	_data_chunks[0]._str = TEXT("Label");
	_s = _get_str(&lsb_buffer_pnt);
	_data_chunks[0]._data = GetUniStr(_s, code);
	free(_s);
	//* 데이터 읽어들이기 (TEXT("Label"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_COMMENT::_extract_text_each_inst(HANDLE hTxt)
{
	wchar_t* _print_str;

	_print_str = (wchar_t*)_data_chunks[0]._data;
	if (!is_Pure_Ascii_String(_print_str) && !is_File_Path_String(_print_str)) {
		swprintf(_txt_buf + wcslen(_txt_buf), TEXT("%s[%s]%s%s%s%s"),
			_indent_, text_data_prefix, text_prefix, _print_str, text_prefix, _enter_raw_);
	}
	//*번역할 필요가 있을 때만 출력
}
//*텍스트를 추출하는 함수

void LSB_INST_COMMENT::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	wchar_t* cp_txt, * n_txt;

	if (!is_Pure_Ascii_String((wchar_t*)_data_chunks[0]._data)
		&& !is_File_Path_String((wchar_t*)_data_chunks[0]._data)) {
		if (_data_chunks[0]._data != NULL) { free(_data_chunks[0]._data); }
		//*이미 데이터가 있으면 해제함
		cp_txt = txt_list_elem->_txt_list_per_inst[accum_count];
		n_txt = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(cp_txt) + 1));
		n_txt[wcslen(cp_txt)] = 0;
		memcpy(n_txt, cp_txt, (sizeof(wchar_t) * wcslen(cp_txt)));
		_data_chunks[0]._data = n_txt;
		//*메모리 복사 후 대치
		accum_count++;
	}
	//*번역할 필요가 있을 때만 대치한다
}
//*텍스트를 대치하는 함수
//---------------------------- comment 명령어 ----------------------------//



//---------------------------- while 명령어 ----------------------------//
LSB_INST_WHILE::LSB_INST_WHILE(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 2;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 2개 (TEXT("_UNK_1"), TEXT("Var_Scope"))
}
//*생성자

void LSB_INST_WHILE::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("_UNK_1");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("_UNK_1"))

	_data_chunks[1]._str = TEXT("Var_Scope");
	_data_chunks[1]._data = _get_int(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("Var_Scope"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_WHILE::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	_pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_WHILE::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	_t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- while 명령어 ----------------------------//



//---------------------------- while_init 명령어 ----------------------------//
LSB_INST_WHILE_INIT::LSB_INST_WHILE_INIT(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 (TEXT("__UNK__"))
}
//*생성자

void LSB_INST_WHILE_INIT::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("__UNK__");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("__UNK__"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_WHILE_INIT::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	_pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_WHILE_INIT::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	_t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- while_init 명령어 ----------------------------//



//---------------------------- while_loop 명령어 ----------------------------//
LSB_INST_WHILE_LOOP::LSB_INST_WHILE_LOOP(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 2;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 2개 (TEXT("_UNK_1"), TEXT("Var_Scope"))
}
//*생성자

void LSB_INST_WHILE_LOOP::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("_UNK_1");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("_UNK_1"))

	_data_chunks[1]._str = TEXT("Var_Scope");
	_data_chunks[1]._data = _get_int(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("Var_Scope"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_WHILE_LOOP::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	_pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_WHILE_LOOP::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	_t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- while_loop 명령어 ----------------------------//



//---------------------------- break 명령어 ----------------------------//
LSB_INST_BREAK::LSB_INST_BREAK(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 2;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 2개 (TEXT("Calc"), TEXT("Var_Scope"))
}
//*생성자

void LSB_INST_BREAK::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Calc");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Calc"))

	_data_chunks[1]._str = TEXT("Var_Scope");
	_data_chunks[1]._data = _get_int(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("Var_Scope"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_BREAK::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	_pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_BREAK::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	_t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- break 명령어 ----------------------------//



//---------------------------- continue 명령어 ----------------------------//
LSB_INST_CONTINUE::LSB_INST_CONTINUE(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 2;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 2개 (TEXT("Calc"), TEXT("Var_Scope"))
}
//*생성자

void LSB_INST_CONTINUE::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Calc");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Calc"))

	_data_chunks[1]._str = TEXT("Var_Scope");
	_data_chunks[1]._data = _get_int(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("Var_Scope"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_CONTINUE::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	_pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_CONTINUE::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	_t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- continue 명령어 ----------------------------//



//---------------------------- game_save 명령어 ----------------------------//
LSB_INST_GAME_SAVE::LSB_INST_GAME_SAVE(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 3;
	if (_script_ver >= 0x69) { _data_chunks_count++; }
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 3개 혹은 4개
	//*(TEXT("No"), TEXT("Script_Page"), 버전에 따라 TEXT("Target_Label_Offset") 추가, TEXT("Caption"))
}
//*생성자

void LSB_INST_GAME_SAVE::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;
	char* _s;

	_data_chunks[0]._str = TEXT("No");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("No"))

	_data_chunks[1]._str = TEXT("Script_Page");
	_s = _get_str(&lsb_buffer_pnt);
	_data_chunks[1]._data = GetUniStr(_s, code);
	free(_s);
	//* 데이터 읽어들이기 (TEXT("Script_Page"))

	if (_script_ver >= 0x69) {
		_data_chunks[2]._str = TEXT("Target_Label_Offset");
		_data_chunks[2]._data = _get_int(&lsb_buffer_pnt);
		//* 데이터 읽어들이기 (TEXT("Target_Label_Offset"))

		_data_chunks[3]._str = TEXT("Caption");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[3]._data = _pnt;
		//* 데이터 읽어들이기 (TEXT("Caption"))
	}
	//* 0x69 이상이면 데이터 읽어들이기 (TEXT("Target_Label_Offset"), TEXT("Caption"))

	else {
		_data_chunks[2]._str = TEXT("Caption");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[2]._data = _pnt;
		//* 데이터 읽어들이기 (TEXT("Caption"))
	}
	//* 0x69 밑이면 TEXT("Caption")만 읽어들이기

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_GAME_SAVE::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	_pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)

	if (_script_ver >= 0x69) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[3]._data;
		_pnt->_extract_text(hTxt);
	}
	else {
		_pnt = (LSB_PARAMETERS*)_data_chunks[2]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_GAME_SAVE::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	_t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치

	if (_script_ver >= 0x69) {
		_t = (LSB_PARAMETERS*)_data_chunks[3]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	else {
		_t = (LSB_PARAMETERS*)_data_chunks[2]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*버전에 따라 패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- game_save 명령어 ----------------------------//



//---------------------------- game_load 명령어 ----------------------------//
LSB_INST_GAME_LOAD::LSB_INST_GAME_LOAD(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 (TEXT("No"))
}
//*생성자

void LSB_INST_GAME_LOAD::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("No");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("No"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_GAME_LOAD::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	_pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_GAME_LOAD::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	_t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- game_load 명령어 ----------------------------//



//---------------------------- clear_load 명령어 ----------------------------//
LSB_INST_CLEAR_LOAD::LSB_INST_CLEAR_LOAD(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	//*필요한 패러미터 리스트 수 : 0개
}
//*생성자

void LSB_INST_CLEAR_LOAD::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_CLEAR_LOAD::_extract_text_each_inst(HANDLE hTxt)
{
	//*별도 작업 필요 없음
}
//*텍스트를 추출하는 함수

void LSB_INST_CLEAR_LOAD::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*텍스트를 대치하는 함수
//---------------------------- clear_load 명령어 ----------------------------//



//---------------------------- pc_reset 명령어 ----------------------------//
LSB_INST_PC_RESET::LSB_INST_PC_RESET(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 3;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 2개 (TEXT("Script_Page"), TEXT("Target_Label_Offset"), TEXT("All_Clear_Byte"))
}
//*생성자

void LSB_INST_PC_RESET::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	char* _s;

	_data_chunks[0]._str = TEXT("Script_Page");
	_s = _get_str(&lsb_buffer_pnt);
	_data_chunks[0]._data = GetUniStr(_s, code);
	free(_s);
	//* 데이터 읽어들이기 (TEXT("Script_Page"))

	_data_chunks[1]._str = TEXT("Target_Label_Offset");
	_data_chunks[1]._data = _get_int(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("Target_Label_Offset"))

	_data_chunks[2]._str = TEXT("All_Clear_Byte");
	_data_chunks[2]._data = _get_byte(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("All_Clear_Byte"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_PC_RESET::_extract_text_each_inst(HANDLE hTxt)
{
	//*별도 작업 필요 없음
}
//*텍스트를 추출하는 함수

void LSB_INST_PC_RESET::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*텍스트를 대치하는 함수
//---------------------------- pc_reset 명령어 ----------------------------//



//---------------------------- reset 명령어 ----------------------------//
LSB_INST_RESET::LSB_INST_RESET(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 3;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 2개 (TEXT("Script_Page"), TEXT("Target_Label_Offset"), TEXT("All_Clear_Byte"))
}
//*생성자

void LSB_INST_RESET::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	char* _s;

	_data_chunks[0]._str = TEXT("Script_Page");
	_s = _get_str(&lsb_buffer_pnt);
	_data_chunks[0]._data = GetUniStr(_s, code);
	free(_s);
	//* 데이터 읽어들이기 (TEXT("Script_Page"))

	_data_chunks[1]._str = TEXT("Target_Label_Offset");
	_data_chunks[1]._data = _get_int(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("Target_Label_Offset"))

	_data_chunks[2]._str = TEXT("All_Clear_Byte");
	_data_chunks[2]._data = _get_byte(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("All_Clear_Byte"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_RESET::_extract_text_each_inst(HANDLE hTxt)
{
	//*별도 작업 필요 없음
}
//*텍스트를 추출하는 함수

void LSB_INST_RESET::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*텍스트를 대치하는 함수
//---------------------------- reset 명령어 ----------------------------//



//---------------------------- sound 명령어 ----------------------------//
LSB_INST_SOUND::LSB_INST_SOUND(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
}
//*생성자

void LSB_INST_SOUND::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_SOUND::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_SOUND::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- sound 명령어 ----------------------------//



//---------------------------- media_play 명령어 ----------------------------//
LSB_INST_MEDIA_PLAY::LSB_INST_MEDIA_PLAY(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 1;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 1개 (TEXT("Target"))
}
//*생성자

void LSB_INST_MEDIA_PLAY::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Target");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Target"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_MEDIA_PLAY::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	_pnt = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_MEDIA_PLAY::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	_t = (LSB_PARAMETERS*)_data_chunks[0]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- media_play 명령어 ----------------------------//



//---------------------------- terminate 명령어 ----------------------------//
LSB_INST_TERMINATE::LSB_INST_TERMINATE(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	//*필요한 패러미터 리스트 수 : 0개
}
//*생성자

void LSB_INST_TERMINATE::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_TERMINATE::_extract_text_each_inst(HANDLE hTxt)
{
	//*별도 작업 필요 없음
}
//*텍스트를 추출하는 함수

void LSB_INST_TERMINATE::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*텍스트를 대치하는 함수
//---------------------------- terminate 명령어 ----------------------------//



//---------------------------- do_event 명령어 ----------------------------//
LSB_INST_DO_EVENT::LSB_INST_DO_EVENT(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	//*필요한 패러미터 리스트 수 : 0개
}
//*생성자

void LSB_INST_DO_EVENT::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_DO_EVENT::_extract_text_each_inst(HANDLE hTxt)
{
	//*별도 작업 필요 없음
}
//*텍스트를 추출하는 함수

void LSB_INST_DO_EVENT::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*텍스트를 대치하는 함수
//---------------------------- do_event 명령어 ----------------------------//



//---------------------------- property_motion 명령어 ----------------------------//
LSB_INST_PROPERTY_MOTION::LSB_INST_PROPERTY_MOTION(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	_data_chunks_count = 6;
	if (_script_ver >= 0x6C) { _data_chunks_count++; }
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 6개 혹은 7개
	//*(TEXT("Name"), TEXT("ObjName"), , TEXT("ObjProp"), TEXT("Value"), TEXT("Time"), TEXT("MoveType"), 버전에 따라 TEXT("Paused"))
}
//*생성자

void LSB_INST_PROPERTY_MOTION::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	_data_chunks[0]._str = TEXT("Name");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[0]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Name"))

	_data_chunks[1]._str = TEXT("ObjName");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[1]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("ObjName"))

	_data_chunks[2]._str = TEXT("ObjProp");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[2]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("ObjProp"))

	_data_chunks[3]._str = TEXT("Value");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[3]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Value"))

	_data_chunks[4]._str = TEXT("Time");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[4]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Time"))

	_data_chunks[5]._str = TEXT("MoveType");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[5]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("MoveType"))

	if (_script_ver >= 0x6C) {
		_data_chunks[6]._str = TEXT("Paused");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[6]._data = _pnt;
	}
	//* 0x6C이상이면 데이터 읽어들이기 (TEXT("Paused"))

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_PROPERTY_MOTION::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_PROPERTY_MOTION::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;

	for (unsigned int i = 0; i < _data_chunks_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치
}
//*텍스트를 대치하는 함수
//---------------------------- property_motion 명령어 ----------------------------//



//---------------------------- save_cabinet 명령어 ----------------------------//
LSB_INST_SAVE_CABINET::LSB_INST_SAVE_CABINET(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks_count += 2;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐, TEXT("Name"), TEXT("Parameter_Count") 추가)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
	//*거기다 TEXT("Parameter_Count") 값까지 반영해야 하므로 당장 정확히 정할 수 없다
}
//*생성자

void LSB_INST_SAVE_CABINET::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < (_data_chunks_count - 2); i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	_data_chunks[(_data_chunks_count - 2)]._str = TEXT("Name");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[(_data_chunks_count - 2)]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Name"))

	_data_chunks[(_data_chunks_count - 1)]._str = TEXT("Parameter_Count");
	_data_chunks[(_data_chunks_count - 1)]._data = _get_int(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("Parameter_Count"))
	
	unsigned int _param_count = *(unsigned int*)_data_chunks[(_data_chunks_count - 1)]._data;
	LSB_MAP_DATA* _n_map = _get_initialized_data_chunks(_data_chunks_count + _param_count);
	memcpy(_n_map, _data_chunks, (sizeof(LSB_MAP_DATA) * _data_chunks_count));
	for (unsigned int i = 0; i < _param_count; i++) {
		_n_map[_data_chunks_count + i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_n_map[_data_chunks_count + i]._data = _pnt;
	}
	//* 제대로 된 개수대로 할당 후 복사 및 마저 읽어들이기

	_data_chunks_count += _param_count;
	free(_data_chunks);
	_data_chunks = _n_map;
	//*다시 갱신

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_SAVE_CABINET::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;
	unsigned int _tp_count = 0;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _tp_count++; }
	}
	//*임시로 값을 얻어와야 한다

	for (unsigned int i = 0; i < _tp_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)

	_pnt = (LSB_PARAMETERS*)_data_chunks[_tp_count]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)

	unsigned int _param_count = *(unsigned int*)_data_chunks[(_tp_count + 1)]._data;
	for (unsigned int i = 0; i < _param_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[(_tp_count + 2) + i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*개수 파악 후 인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_SAVE_CABINET::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;
	unsigned int _tp_count = 0;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _tp_count++; }
	}
	//*임시로 값을 얻어와야 한다

	for (unsigned int i = 0; i < _tp_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치

	_t = (LSB_PARAMETERS*)_data_chunks[_tp_count]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치

	unsigned int _param_count = *(unsigned int*)_data_chunks[(_tp_count + 1)]._data;
	for (unsigned int i = 0; i < _param_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[(_tp_count + 2) + i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
}
//*텍스트를 대치하는 함수
//---------------------------- save_cabinet 명령어 ----------------------------//



//---------------------------- load_cabinet 명령어 ----------------------------//
LSB_INST_LOAD_CABINET::LSB_INST_LOAD_CABINET(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	_data_chunks_count = 0;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _data_chunks_count++; }
	}
	_data_chunks_count += 2;
	_data_chunks = _get_initialized_data_chunks(_data_chunks_count);
	//*필요한 패러미터 리스트 수 : 유동적임 (opcode값에 따라 달라짐, TEXT("Name"), TEXT("Parameter_Count") 추가)
	//*원래 길이는 버전에 따라 나뉘는데 그냥 진행해도 될 것 같다
	//*거기다 TEXT("Parameter_Count") 값까지 반영해야 하므로 당장 정확히 정할 수 없다
}
//*생성자

void LSB_INST_LOAD_CABINET::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);
	LSB_PARAMETERS* _pnt;

	for (unsigned int i = 0; i < (_data_chunks_count - 2); i++) {
		_data_chunks[i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_data_chunks[i]._data = _pnt;
	}
	//*지정된 수만큼 전부 읽어들인다

	_data_chunks[(_data_chunks_count - 2)]._str = TEXT("Name");
	_pnt = new LSB_PARAMETERS();
	_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
	_data_chunks[(_data_chunks_count - 2)]._data = _pnt;
	//* 데이터 읽어들이기 (TEXT("Name"))

	_data_chunks[(_data_chunks_count - 1)]._str = TEXT("Parameter_Count");
	_data_chunks[(_data_chunks_count - 1)]._data = _get_int(&lsb_buffer_pnt);
	//* 데이터 읽어들이기 (TEXT("Parameter_Count"))

	unsigned int _param_count = *(unsigned int*)_data_chunks[(_data_chunks_count - 1)]._data;
	LSB_MAP_DATA* _n_map = _get_initialized_data_chunks(_data_chunks_count + _param_count);
	memcpy(_n_map, _data_chunks, (sizeof(LSB_MAP_DATA) * _data_chunks_count));
	for (unsigned int i = 0; i < _param_count; i++) {
		_n_map[_data_chunks_count + i]._str = TEXT("__ARG__");
		_pnt = new LSB_PARAMETERS();
		_pnt->_read_parameter_list(&lsb_buffer_pnt, code);
		_n_map[_data_chunks_count + i]._data = _pnt;
	}
	//* 제대로 된 개수대로 할당 후 복사 및 마저 읽어들이기

	_data_chunks_count += _param_count;
	free(_data_chunks);
	_data_chunks = _n_map;
	//*다시 갱신

	(*_lsb_buffer_pnt) = lsb_buffer_pnt;
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_LOAD_CABINET::_extract_text_each_inst(HANDLE hTxt)
{
	LSB_PARAMETERS* _pnt;
	unsigned int _tp_count = 0;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _tp_count++; }
	}
	//*임시로 값을 얻어와야 한다

	for (unsigned int i = 0; i < _tp_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)

	_pnt = (LSB_PARAMETERS*)_data_chunks[_tp_count]._data;
	_pnt->_extract_text(hTxt);
	//*인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)

	unsigned int _param_count = *(unsigned int*)_data_chunks[(_tp_count + 1)]._data;
	for (unsigned int i = 0; i < _param_count; i++) {
		_pnt = (LSB_PARAMETERS*)_data_chunks[(_tp_count + 2) + i]._data;
		_pnt->_extract_text(hTxt);
	}
	//*개수 파악 후 인자 목록에서 문자열인 인자만 골라 추출 (순수 ascii 코드가 아니면 추출한다)
}
//*텍스트를 추출하는 함수

void LSB_INST_LOAD_CABINET::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	unsigned int accum_count = 0;
	LSB_PARAMETERS* _t;
	unsigned int _tp_count = 0;
	bool** property_boolean_table = _header->opcode_property_boolean_table;
	for (unsigned int i = 0; i < (_header->opcode_property_count * 0x8); i++) {
		if (property_boolean_table[_op_type][i]) { _tp_count++; }
	}
	//*임시로 값을 얻어와야 한다

	for (unsigned int i = 0; i < _tp_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
	//*패러미터 리스트에서 대치

	_t = (LSB_PARAMETERS*)_data_chunks[_tp_count]._data;
	_t->_replace_text(txt_list_elem, &accum_count);
	//*패러미터 리스트에서 대치

	unsigned int _param_count = *(unsigned int*)_data_chunks[(_tp_count + 1)]._data;
	for (unsigned int i = 0; i < _param_count; i++) {
		_t = (LSB_PARAMETERS*)_data_chunks[(_tp_count + 2) + i]._data;
		_t->_replace_text(txt_list_elem, &accum_count);
	}
}
//*텍스트를 대치하는 함수
//---------------------------- load_cabinet 명령어 ----------------------------//



//---------------------------- IFDEF 전처리 명령어 ----------------------------//
LSB_INST_MACRO_IFDEF::LSB_INST_MACRO_IFDEF(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	//*필요한 패러미터 리스트 수 : 0개
}
//*생성자

void LSB_INST_MACRO_IFDEF::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_MACRO_IFDEF::_extract_text_each_inst(HANDLE hTxt)
{
	//*별도 작업 필요 없음
}
//*텍스트를 추출하는 함수

void LSB_INST_MACRO_IFDEF::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*텍스트를 대치하는 함수
//---------------------------- IFDEF 전처리 명령어 ----------------------------//



//---------------------------- IFNDEF 전처리 명령어 ----------------------------//
LSB_INST_MACRO_IFNDEF::LSB_INST_MACRO_IFNDEF(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	//*필요한 패러미터 리스트 수 : 0개
}
//*생성자

void LSB_INST_MACRO_IFNDEF::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_MACRO_IFNDEF::_extract_text_each_inst(HANDLE hTxt)
{
	//*별도 작업 필요 없음
}
//*텍스트를 추출하는 함수

void LSB_INST_MACRO_IFNDEF::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*텍스트를 대치하는 함수
//---------------------------- IFNDEF 전처리 명령어 ----------------------------//



//---------------------------- ENDIF 전처리 명령어 ----------------------------//
LSB_INST_MACRO_ENDIF::LSB_INST_MACRO_ENDIF(LSB_INST_TYPES _op_type, unsigned int _script_ver, LSB_HEADER* _header)
	: LSB_INST_BASE(_script_ver, _header)
{
	this->_op_type = _op_type;
	//*필요한 패러미터 리스트 수 : 0개
}
//*생성자

void LSB_INST_MACRO_ENDIF::_read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*버퍼에서 읽어들이는 함수

void LSB_INST_MACRO_ENDIF::_extract_text_each_inst(HANDLE hTxt)
{
	//*별도 작업 필요 없음
}
//*텍스트를 추출하는 함수

void LSB_INST_MACRO_ENDIF::_replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code)
{
	//*별도 작업 필요 없음
}
//*텍스트를 대치하는 함수
//---------------------------- ENDIF 전처리 명령어 ----------------------------//

//*********************** 각 클래스들 함수 ***********************//