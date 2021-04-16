#pragma once
#include "LM_Base.h"

//*** LSB 스크립트 분석 및 적용 함수들을 선언하는 헤더

//*** lcm : LiveCinema108 뒤에 (80 02) / LiveCinema112 뒤에 (20 03)으로 나뉨
//*** LiveCinema108 일때는 0x8b - 텍스트 - 0x8b - 텍스트 ... 이런 식이고
//*** LiveCinema112 일때는 0xa3 - 텍스트 - 0xa3 - 텍스트 ... 이런 식임
//*** 맨 앞이 0일때는 0xa3이 간격이지만 2일때는 0x90이 간격인 것으로 보이며,
//*** 텍스트 정보도 없는 듯 보인다
//*** 그러니 이때는 손대지 않는 게 나을 듯 싶음
//*** lmt : LiveMotion111 뒤에 (20 03)
//*** LiveMotion111 일때는 0xa3 - 텍스트 - 0xa3 - 텍스트 ... 이런 식임
//*** lcm이든 lmt든	(+0x26) 위치에 포함된 파일명 텍스트가 있음


enum LSB_INST_TYPES
{
	TYPE_INST_IF =						0x00,
	TYPE_INST_ELIF =					0x01,
	TYPE_INST_ELSE =					0x02,

	TYPE_INST_LABEL =					0x03,
	TYPE_INST_JUMP =					0x04,

	TYPE_INST_CALL =					0x05,

	TYPE_INST_EXIT =					0x06,

	TYPE_INST_WAIT =					0x07,
	TYPE_INST_TIMER =					0x0B,

	TYPE_INST_CALC_FUNC =				0x0E,

	TYPE_INST_BOX_NEW =					0x08,
	TYPE_INST_IMAGE_NEW =				0x09,
	TYPE_INST_MESSAGE_NEW =				0x0A,
	TYPE_INST_PARTICLE_NEW =			0x24,
	TYPE_INST_FIRE_NEW =				0x25,
	TYPE_INST_EDIT_NEW =				0x2B,
	TYPE_INST_MEMO_NEW =				0x2C,
	TYPE_INST_MAP_IMAGE_NEW =			0x30,
	TYPE_INST_WAVE_NEW =				0x31,
	TYPE_INST_TILE_NEW =				0x32,
	TYPE_INST_SLIDER_NEW =				0x33,
	TYPE_INST_SCROLL_BAR_NEW =			0x34,
	TYPE_INST_GAUGE_NEW =				0x35,
	TYPE_INST_PREV_MENU_NEW =			0x38,

	TYPE_INST_VAR_NEW =					0x0F,
	TYPE_INST_VAR_DELETE =				0x10,

	TYPE_INST_MOVIE =					0x0C,
	TYPE_INST_FLIP =					0x0D,
	TYPE_INST_MOVIE_STOP =				0x15,

	TYPE_INST_CINEMA =					0x17,

	TYPE_INST_GET_PROPERTY =			0x11,
	TYPE_INST_SET_PROPERTY =			0x12,

	TYPE_INST_OBJECT_DELETE =			0x13,

	TYPE_INST_TEXT_INST =				0x14,
	TYPE_INST_TEXT_CLEAR =				0x1C,

	TYPE_INST_CLEAR_HISTORY =			0x16,
	TYPE_INST_CALL_HISTORY =			0x1D,
	TYPE_INST_FORMAT_HISTORY =			0x3A,

	TYPE_INST_CAPTION =					0x18,
	TYPE_INST_CG_CAPTION =				0x36,

	TYPE_INST_MENU =					0x19,
	TYPE_INST_MENU_CLOSE =				0x1A,
	TYPE_INST_BUTTON =					0x1E,

	TYPE_INST_COMMENT =					0x1B,

	TYPE_INST_WHILE =					0x1F,
	TYPE_INST_WHILE_INIT =				0x20,
	TYPE_INST_WHILE_LOOP =				0x21,
	TYPE_INST_BREAK =					0x22,
	TYPE_INST_CONTINUE =				0x23,

	TYPE_INST_GAME_SAVE =				0x26,
	TYPE_INST_GAME_LOAD =				0x27,
	TYPE_INST_CLEAR_LOAD =				0x2F,

	TYPE_INST_PC_RESET =				0x28,
	TYPE_INST_RESET =					0x29,

	TYPE_INST_SOUND =					0x2A,
	TYPE_INST_MEDIA_PLAY =				0x37,

	TYPE_INST_TERMINATE =				0x2D,

	TYPE_INST_DO_EVENT =				0x2E,

	TYPE_INST_PROPERTY_MOTION =			0x39,
	
	TYPE_INST_SAVE_CABINET =			0x3B,
	TYPE_INST_LOAD_CABINET =			0x3C,

	TYPE_INST_MACRO_IFDEF =				0x3D,
	TYPE_INST_MACRO_IFNDEF =			0x3E,
	TYPE_INST_MACRO_ENDIF =				0x3F,

	TYPE_INST_UNKNOWN =					0xFFFFFFFF
};
//*** 명령어(instruction) 타입 열거형


enum LSB_OPER_TYPES
{
	TYPE_OP_ASSIGN =					0x01,		//*값 할당하기(=)
	TYPE_OP_PLUS,									//*더하기(+, 수치 계열 / 문자열에 쓰임)
	TYPE_OP_MINUS,									//*빼기(-)
	TYPE_OP_MULTIPLY,								//*곱하기(*)
	TYPE_OP_DIVIDE,									//*나누기(/)
	TYPE_OP_MODULUS,								//*나머지(%)
	TYPE_OP_OR,										//*or(|, 비트연산과 논리연산 다 쓰임)
	TYPE_OP_AND,									//*and(&, 비트연산과 논리연산 다 쓰임)
	TYPE_OP_XOR,									//*xor(^)
	TYPE_OP_ARRAY_ACCESS,							//*배열 접근 연산자([])
	TYPE_OP_CALC_FUNC,								//*함수 호출자(())
	TYPE_OP_EQUALS,									//*동치(==)
	TYPE_OP_GREATER,								//*이 값보다 더 큰지(>)
	TYPE_OP_LESS,									//*이 값보다 더 작은지(<)
	TYPE_OP_GREATER_EQUAL,							//*이 값보다 크거나 같은지(>=)
	TYPE_OP_LESS_EQUAL,								//*이 값보다 작거나 같은지(<=)
	TYPE_OP_SHIFT_LEFT,								//*왼쪽 시프트 연산(<<)
	TYPE_OP_SHIFT_RIGHT,							//*오른쪽 시프트 연산(<<)
	TYPE_OP_COMBO_STRING,							//*문자열 합연산(++)
	TYPE_OP_NOT_EQUALS,								//*동치가 아닌지(!=)
	TYPE_OP_UNKNOWN =					0xFFFFFFFF
};
//*** 연산자(operator) 타입 열거형


enum LSB_CALC_FUNC_TYPES
{
	TYPE_CF_IntToStr =				0x00,
	TYPE_CF_IntToHex,
	TYPE_CF_GetProp,
	TYPE_CF_SetProp,
	TYPE_CF_GetArraySize,
	TYPE_CF_Length,
	TYPE_CF_JLength,
	TYPE_CF_Copy,
	TYPE_CF_JCopy,
	TYPE_CF_Delete,
	TYPE_CF_JDelete,
	TYPE_CF_Insert,
	TYPE_CF_JInsert,
	TYPE_CF_CompareStr,
	TYPE_CF_CompareText,
	TYPE_CF_Pos,
	TYPE_CF_JPos,
	TYPE_CF_Trim,
	TYPE_CF_JTrim,
	TYPE_CF_Exists,
	TYPE_CF_Not,
	TYPE_CF_SetArray,
	TYPE_CF_FillMem,
	TYPE_CF_CopyMem,
	TYPE_CF_GetCheck,
	TYPE_CF_SetCheck,
	TYPE_CF_Random,
	TYPE_CF_GetSaveCaption,
	TYPE_CF_ArrayToString,
	TYPE_CF_StringToArray,
	TYPE_CF_IndexOfStr,
	TYPE_CF_SortStr,
	TYPE_CF_ListCompo,
	TYPE_CF_ToClientX,
	TYPE_CF_ToClientY,
	TYPE_CF_ToScreenX,
	TYPE_CF_ToScreenY,
	TYPE_CF_Int,
	TYPE_CF_Float,
	TYPE_CF_Sin,
	TYPE_CF_Cos,
	TYPE_CF_Tan,
	TYPE_CF_ArcSin,
	TYPE_CF_ArcCos,
	TYPE_CF_ArcTan_1,
	TYPE_CF_ArcTan_2,
	TYPE_CF_Hypot,
	TYPE_CF_IndexOfMenu,
	TYPE_CF_Abs,
	TYPE_CF_Fabs,
	TYPE_CF_VarExists,
	TYPE_CF_EncodeDate,
	TYPE_CF_EncodeTime,
	TYPE_CF_DecodeDate,
	TYPE_CF_DecodeTime,
	TYPE_CF_GetYear,
	TYPE_CF_GetMonth,
	TYPE_CF_GetDay,
	TYPE_CF_GetHour,
	TYPE_CF_GetMin,
	TYPE_CF_GetSec,
	TYPE_CF_GetWeek,
	TYPE_CF_GetWeekStr,
	TYPE_CF_GetWeekJStr,
	TYPE_CF_FixStr,
	TYPE_CF_GetDisplayMode,
	TYPE_CF_AddArray,
	TYPE_CF_InsertArray,
	TYPE_CF_DeleteArray,
	TYPE_CF_InPrimary,
	TYPE_CF_CopyArray,
	TYPE_CF_FileExists,
	TYPE_CF_LoadTextFile,
	TYPE_CF_LowerCase,
	TYPE_CF_UpperCase,
	TYPE_CF_ExtractFilePath,
	TYPE_CF_ExtractFileName,
	TYPE_CF_ExtractFileExt,
	TYPE_CF_IsPathDelimiter,
	TYPE_CF_AddBackSlash,
	TYPE_CF_ChangeFileExt,
	TYPE_CF_IsDelimiter,
	TYPE_CF_StringOfChar,
	TYPE_CF_StringReplace,
	TYPE_CF_AssignTemp,
	TYPE_CF_HanToZen,
	TYPE_CF_ZenToHan,
	TYPE_CF_DBCreateTable,
	TYPE_CF_DBSetActive,
	TYPE_CF_DBAddField,
	TYPE_CF_DBSetRecNo,
	TYPE_CF_DBInsert,
	TYPE_CF_DBDelete,
	TYPE_CF_DBGetInt,
	TYPE_CF_DBSetInt,
	TYPE_CF_DBGetFloat,
	TYPE_CF_DBSetFloat,
	TYPE_CF_DBGetBool,
	TYPE_CF_DBSetBool,
	TYPE_CF_DBGetStr,
	TYPE_CF_DBSetStr,
	TYPE_CF_DBRecordCount,
	TYPE_CF_DBFindFirst,
	TYPE_CF_DBFindLast,
	TYPE_CF_DBFindNext,
	TYPE_CF_DBFindPrior,
	TYPE_CF_DBLocate,
	TYPE_CF_DBLoadTsvFile,
	TYPE_CF_DBDirectGetInt,
	TYPE_CF_DBDirectSetInt,
	TYPE_CF_DBDirectGetFloat,
	TYPE_CF_DBDirectSetFloat,
	TYPE_CF_DBDirectGetBool,
	TYPE_CF_DBDirectSetBool,
	TYPE_CF_DBDirectGetStr,
	TYPE_CF_DBDirectSetStr,
	TYPE_CF_DBCopyTable,
	TYPE_CF_DBDeleteTable,
	TYPE_CF_DBInsertTable,
	TYPE_CF_DBCopy,
	TYPE_CF_DBClearTable,
	TYPE_CF_DBSort,
	TYPE_CF_DBGetActive,
	TYPE_CF_DBGetRecNo,
	TYPE_CF_DBClearRecord,
	TYPE_CF_SetWallPaper,
	TYPE_CF_Min,
	TYPE_CF_Max,
	TYPE_CF_Fmin,
	TYPE_CF_Fmax,
	TYPE_CF_GetVarType,
	TYPE_CF_GetEnabled,
	TYPE_CF_SetEnabled,
	TYPE_CF_AddDelimiter,
	TYPE_CF_ListSaveCaption,
	TYPE_CF_OpenUrl,
	TYPE_CF_Calc,
	TYPE_CF_SaveScreen,
	TYPE_CF_StrToIntDef,
	TYPE_CF_StrToFloatDef,
	TYPE_CF_GetVisible,
	TYPE_CF_SetVisible,
	TYPE_CF_GetHistoryCount,
	TYPE_CF_GetHistoryMaxCount,
	TYPE_CF_SetHistoryMaxCount,
	TYPE_CF_GetGroupIndex,
	TYPE_CF_GetSelected,
	TYPE_CF_SetSelected,
	TYPE_CF_SelectOpenFile,
	TYPE_CF_SelectSaveFile,
	TYPE_CF_SelectDirectory,
	TYPE_CF_ExtractFile,
	TYPE_CF_Chr,
	TYPE_CF_Ord,
	TYPE_CF_InCabinet,
	TYPE_CF_PushVar,
	TYPE_CF_PopVar,
	TYPE_CF_DeleteStack,
	TYPE_CF_CopyFile,
	TYPE_CF_DBGetTableCount,
	TYPE_CF_DBGetTable,
	TYPE_CF_CreateObject,
	TYPE_CF_DeleteObject,
	TYPE_CF_GetItem,
	TYPE_CF_UniqueArray,
	TYPE_CF_TrimArray,
	TYPE_CF_GetImeOpened,
	TYPE_CF_SetImeOpened,
	TYPE_CF_Alert,
	TYPE_CF_COUNT,					//*** calc_func의 총 수를 세기 위함

	TYPE_CF_UNKNOWN = 0xFFFFFFFF
};
//*** calc_func(특수함수) 열거형


enum LSB_EV_OPCODE_TYPES
{
	TYPE_EV_OP_CHAR =				0x01,
	TYPE_EV_OP_ALIGN =				0x02,
	TYPE_EV_OP_RETURN =				0x03,
	TYPE_EV_OP_INDENT =				0x04,
	TYPE_EV_OP_UNDENT =				0x05,
	TYPE_EV_OP_EVENT =				0x06,
	TYPE_EV_OP_VAR =				0x07,
	TYPE_EV_OP_IMG =				0x09,
	TYPE_EV_OP_HISTORY_CHAR =		0x0A,
	TYPE_EV_OP_STR =				0xFFFFFFFF
};
//*** 이벤트 명령어(opcode) 타입 열거형



//*** 클래스 선언 ***//
class LSB_PARAMETERS;
class LSB_EV_TXT_ATTRIBUTE;
class LSB_EVENT_OP_INST;
class LSB_EVENT_ARG_LIST;
class LSB_EVENT;
class LSB_INST_BASE;
class LSB_HEADER;
class LSB_BODY;
class LSB_SCRIPT;
//*** 클래스 선언 ***//



//*Label/SetLabel/Event 오브젝트와 Argument 오브젝트는 같은 클래스에서 파생된 것으로 쳐야 할 듯 하다
//*ParseEventBlock은 TYPE_INST_TEXT_INST 일 때만 불러들이는 듯 하다
//*이것도 Argument로 치면 될듯?
//*차라리 인자 수 1인 패러미터로 가정해 넣는 게 어떨까?


typedef struct _lsb_txt_list
{
	unsigned int _line;
	unsigned int _txt_count;
	wchar_t** _txt_list_per_inst;
}LSB_TEXTS_PER_INST;
//*각 명령어 당 텍스트 리스트에 해당되는 구조체

typedef struct _lsb_parameter_struct
{
	LSB_OPER_TYPES _type;							//*** 패러미터의 연산자 타입
	wchar_t* _name;									//*** 패러미터의 이름
	unsigned int arg_count;							//*** 인자의 수
	LM_SCR_ARG* arg_list;							//*** 인자 리스트
	LSB_CALC_FUNC_TYPES _calc_func;					//*** 특수 함수
}LSB_PARAMETER;
//*** 패러미터 구조체

class LSB_PARAMETERS
{
public:
	LSB_PARAMETER* _list;							//*** 패러미터 리스트
	unsigned int _count;							//*** 패러미터의 수

	LSB_PARAMETERS()
	{
		_list = NULL;
		_count = 0;
	}
	//*생성자

	~LSB_PARAMETERS()
	{
		_release();
	}
	//*파괴자

	void _read_parameter_list(unsigned char** _lsb_buffer_pnt, unsigned int code);
	//*패러미터를 읽어들이는 함수

	void _decompile_code(HANDLE hWrite, unsigned int code, unsigned int _indent);
	//*디컴파일 함수

	void _write_as_lsb(HANDLE hNewLsb, unsigned int code);
	//*lsb 파일로 기록하는 함수

	void _extract_text(HANDLE hTxt);
	//*텍스트 추출 함수

	void _replace_text(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int* accum_count);
	//*텍스트 교체 함수

	void _change_code_J2K();
	//*텍스트 코드 변경 함수

	void _release();
	//*패러미터를 해제하는 함수
};
//*** 패러미터 리스트 클래스


class LSB_EV_TXT_ATTRIBUTE
{
public:
	unsigned int _tpword_ver;						//*** 현재 이벤트 블럭의 버전
	unsigned int _ev_op_ref_count;					//*** 각 이벤트 명령어에서 참조한 횟수
	unsigned int _txt_color;						//*** 텍스트의 색깔
	unsigned int _unk_2;							//*** 모르는 변수
	unsigned int _unk_3;							//*** 모르는 변수
	unsigned char _unk_4;							//*** 모르는 변수
	unsigned char _unk_5;							//*** 모르는 변수
	unsigned char _unk_6;							//*** 모르는 변수
	unsigned int _unk_7;							//*** 모르는 변수
	wchar_t* _font_name;							//*** 폰트명, 없으면 미리 지정된 폰트가 적용되는 듯 함
	wchar_t* _ruby_txt;								//*** 루비 문자가 적용될 시 쓰이는 텍스트
	unsigned int _unk_10;							//*** 모르는 변수
	unsigned int _unk_11;							//*** 모르는 변수

	LSB_EV_TXT_ATTRIBUTE (unsigned int tpword_ver)
	{
		_tpword_ver = tpword_ver;
		_ev_op_ref_count = _txt_color = _unk_2 = _unk_3 = _unk_7 = _unk_10 = _unk_11 = 0;
		_unk_4 = _unk_5 = _unk_6 = 0;
		_font_name = _ruby_txt = NULL;
	}
	//*생성자

	~LSB_EV_TXT_ATTRIBUTE()
	{
		_release();
	}
	//*파괴자

	void _read_var_data(unsigned char** _lsb_buffer_pnt, unsigned int code);
	//*이벤트 내부 변수 읽기

	void _decompile_code(HANDLE hWrite, unsigned int code, unsigned int _indent);
	//*디컴파일 함수

	void _recompile_from_txt(wchar_t* _code_txt_buffer);
	//*리컴파일 함수 (인자 : 해당 이벤트 블럭의 데이터가 들어있는 텍스트 데이터 일부)

	void _write_as_lsb(HANDLE hNewLsb, unsigned int code);
	//*lsb 파일로 기록하는 함수

	void _extract_text(HANDLE hTxt);
	//*텍스트 추출 함수

	void _change_code_J2K();
	//*텍스트 코드 변경 함수

	void _release()
	{
		if (_font_name != NULL) { free(_font_name); }
		if (_ruby_txt != NULL) { free(_ruby_txt); }
	}
	//*해제 함수
};
//*** 이벤트 내부 속성 클래스

typedef struct _lsb_ev_attr_info
{
	unsigned int _attr_idx;							//*해당 텍스트 영역에 적용될 속성의 인덱스
	unsigned int _start_txt_pos;					//*적용될 텍스트 영역의 시작 지점
	unsigned int _end_txt_pos;						//*적용될 텍스트 영역의 끝 지점
}LSB_EV_ATTR_INFO;
//*** 이벤트 내부 속성 적용 시 쓰이는 구조체

typedef struct _lsb_ev_type2_type3
{
	unsigned char _unk_1;
	unsigned int _unk_2;
	unsigned int _unk_3;
}LSB_EV_TYPE2_TYPE3;
//*** _unk_1이 0이 아니고 type이 2나 3일 때 쓰는 구조체

class LSB_EVENT_OP_INST
{
public:
	unsigned int _tpword_ver;						//*** tpword의 버전
	unsigned int _unk_1;							//*** 뭔지 모르지만 105버전 들어 읽어들임

	wchar_t* _s;									//*_s, _extra_data, _i는 0x01 일 동안에는 전부 같고
	unsigned int _extra_data;						//*_str만 달라지는 듯 싶다
	wchar_t* _str;									//*그러니 그에 특화되게 짜야 할 듯 하다
	unsigned int _default_attr_idx;					//*이 이벤트 명령어가 참조하는 텍스트 속성 인덱스
													//*단 TYPE_EV_OP_CHAR일 경우에는 이거 하나 가지고는 안된다
	unsigned int _i_2;
	unsigned int _i_3;
	unsigned char _b;								//*모든 ev_op에 대응하여 읽을 수 있게 세팅해 놓는다	

	LSB_EV_TYPE2_TYPE3 _unk_t2_t3;					//*type이 2나 3일 때 쓰이는 배열
	unsigned int _unk_i_list[6];					//*type이 9일 때 쓰이는 배열

	//*** TYPE_EV_OP_CHAR 전용 변수 ***//
	unsigned int _ev_attr_info_cnt;					//*이벤트가 다르게 적용되는 속성 개수
	unsigned int* _ev_attr_idx_ref_list;			//*각 이벤트 내 속성별 참조횟수 저장하는 변수
	unsigned int* _idx_list;						//*배열 정보를 토대로 한 문자별 인덱스 세팅
	LSB_EV_TXT_ATTRIBUTE **_ev_attr_list_pnt;		//*이벤트 내 속성 배열을 가리키는 포인터, 텍스트를 반영할 때 필요
	LSB_EV_ATTR_INFO* _ev_attr_info_list;			//*어디에 어떤 속성을 적용해야 하는지 정보를 포함한 배열
	unsigned int _last_idx;							//*마지막으로 받은 텍스트의 참조 속성 인덱스
	//*서로 다른 묶음이 얼마나 있는지 판별하기 위한 변수
	//*** TYPE_EV_OP_CHAR 전용 변수 ***//

	LSB_EV_OPCODE_TYPES _op_type;					//*** 해당 인자의 명령어 타입
	LSB_EVENT_OP_INST* _prev;						//*** 이전 인자(리스트 이용)
	LSB_EVENT_OP_INST* _next;						//*** 이전 인자(리스트 이용)

	LSB_EVENT_OP_INST (LSB_EV_OPCODE_TYPES op_type, LSB_EV_TXT_ATTRIBUTE** ev_attr_list, unsigned int ev_attr_cnt, unsigned int tpword_ver)
	{
		_tpword_ver = tpword_ver;
		_unk_1 = 0;
		_op_type = op_type;
		_prev = _next = NULL;
		_ev_attr_list_pnt = ev_attr_list;
		_ev_attr_info_cnt = 0;
		_ev_attr_idx_ref_list = (unsigned int*)malloc(sizeof(int) * ev_attr_cnt);
		memset(_ev_attr_idx_ref_list, 0, (sizeof(int) * ev_attr_cnt));
		_ev_attr_info_list = NULL;
		_idx_list = NULL;
		_last_idx = 0;

		_s = _str = NULL;
		_extra_data = _i_2 = _i_3 = 0;
		_default_attr_idx = 0xFFFFFFFF;
		_b = 0;
		memset(&_unk_t2_t3, 0, sizeof(LSB_EV_TYPE2_TYPE3));
		memset(_unk_i_list, 0, (sizeof(int) * 6));
	}
	//*생성자

	~LSB_EVENT_OP_INST()
	{
		_release();
	}
	//*소멸자

	void _read_ev_data(unsigned char** _lsb_buffer_pnt, unsigned int txt_accum_idx, unsigned int code);
	//*모든 명령어 타입에 대해서 읽어들이는 명령어

	void _read_char_data(unsigned char** _lsb_buffer_pnt, unsigned int txt_accum_idx, unsigned int code);
	//*이벤트 내부 char 변수 읽기

	void _decode_sharp_encoding(char **_str_addr);
	//*이벤트 명령문에서 # + 숫자문자열로 인코딩된 값을 원래대로 돌려놓기

	void _decompile_code(HANDLE hWrite, unsigned int code, unsigned int _indent);
	//*디컴파일 함수

	void _recompile_from_txt(wchar_t* _code_txt_buffer, unsigned int code);
	//*리컴파일 함수 (인자 : 해당 이벤트 블럭의 데이터가 들어있는 텍스트 데이터 일부)

	void _set_char_data();
	//*이벤트 내부 _str 변수를 분해해 정보 세팅하기

	void _encode_sharp_prefix(char** _str_addr);
	//*이벤트 명령문에서 [#] ~ [#] 사이에 있는 문자열을 # + 숫자문자열로 인코딩하기

	void _write_as_lsb(HANDLE hNewLsb, unsigned int code, unsigned int *_total_inst_count);
	//*lsb 파일로 기록하는 함수, 전체 카운트도 기억해야 함

	void _extract_text(HANDLE hTxt);
	//*텍스트 추출 함수

	void _replace_text(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int *accum_count);
	//*텍스트 교체 함수

	void _change_code_J2K();
	//*텍스트 코드 변경 함수

	void _release()
	{
		if (_s != NULL) { free(_s); }
		if (_str != NULL) { free(_str); }
		if (_ev_attr_idx_ref_list != NULL) { free(_ev_attr_idx_ref_list); }
		if (_ev_attr_info_list != NULL) { free(_ev_attr_info_list); }
		if (_idx_list != NULL) { free(_idx_list); }
	}
	//*해제 함수
};
//*** 이벤트 내부 인자 클래스

class LSB_EVENT_ARG_LIST
{
public:
	unsigned int _tpword_ver;						//*** tpword의 버전
	unsigned int _count;							//*** 리스트가 가진 인자의 총 수
	LSB_EVENT_OP_INST* _dummy_start;				//*** 시작 더미 포인터
	LSB_EVENT_OP_INST* _dummy_end;					//*** 끝 더미 포인터

	LSB_EVENT_ARG_LIST (LSB_EV_TXT_ATTRIBUTE** ev_attr_list, unsigned int ev_attr_cnt, unsigned int tpword_ver)
	{
		_count = 0;
		_tpword_ver = tpword_ver;
		_dummy_start = new LSB_EVENT_OP_INST(TYPE_EV_OP_STR, ev_attr_list, ev_attr_cnt, _tpword_ver);
		_dummy_end = new LSB_EVENT_OP_INST(TYPE_EV_OP_STR, ev_attr_list, ev_attr_cnt, _tpword_ver);
		_dummy_start->_next = _dummy_end;
		_dummy_end->_prev = _dummy_start;
	}
	//*생성자

	~LSB_EVENT_ARG_LIST()
	{
		_release();
	}
	//*소멸자

	void Append (LSB_EVENT_OP_INST *_n)
	{
		if (_n == NULL) { return; }
		LSB_EVENT_OP_INST* _last = _dummy_end->_prev;
		_last->_next = _n;
		_n->_next = _dummy_end;
		_dummy_end->_prev = _n;
		_n->_prev = _last;
		_count++;
	}
	//*순서대로 인자 추가 함수 (null이 아닌 것만)
	
	void _release()
	{
		for (unsigned int i = 0; i < _count;i++) {
			LSB_EVENT_OP_INST* _d = _dummy_start->_next;
			LSB_EVENT_OP_INST* _d_n = _dummy_start->_next->_next;
			_dummy_start->_next = _d_n;
			if (_d_n != NULL) { _d_n->_prev = _dummy_start; }
			delete _d;
		}
		//*앞에서부터 차례대로 모든 리스트 해제
		delete _dummy_start;
		delete _dummy_end;
		//*시작 / 끝 더미리스트 해제
	}
	//*해제 함수
};
//*** 이벤트 내부 인자 리스트 클래스

typedef struct _lsb_evh_elem
{
	unsigned int _ref_count;						//*** 이 속성 호출 횟수
	wchar_t* _str;									//*** 문자열, 아마 명령어 역할을 할 것으로 추측, 개행 변환 필요
	unsigned int _unk_2;							//*** 모르는 값 2

	void _recompile_from_txt(wchar_t* _code_txt_buffer, unsigned int code);
	//*리컴파일 함수 (인자 : 해당 이벤트 블럭의 데이터가 들어있는 텍스트 데이터 일부)
}LSB_EVH_ELEM;
//*** 이벤트 클래스 헤더 구조체의 원소

class LSB_EVENT_HEADER
{
public:
	unsigned int _type;								//*** 이벤트 헤더의 타입
	unsigned int _several_op_count;					//*** 화면에 출력되는 문자열과 관련된 카운트
	unsigned int _unk_1;							//*** 모르는 값 1
	unsigned int _unk_2;							//*** 모르는 값 2 (타입 2에서만 활성화)
	wchar_t* _str;									//*** 뭔지 모를 문자열, 함수 기능 관련? (타입 2에서만 활성화)
	unsigned int _elem_count;						//*** 하위 원소들 개수
	LSB_EVH_ELEM* _elem_list;						//*** 하위 원소들 리스트

	LSB_EVENT_HEADER()
	{
		_type = 0;
		_several_op_count = 0;
		_unk_1 = _unk_2 = 0;
		_str = NULL;
		_elem_count = 0;
		_elem_list = NULL;
	}
	//*생성자
	
	~LSB_EVENT_HEADER()
	{
		_release();
	}
	//*소멸자

	void _read_event_header(unsigned char** _lsb_buffer_pnt, unsigned int code);
	//*이벤트 헤더를 읽어들이는 함수

	void _release()
	{
		if (_str != NULL) { free(_str); }
		if (_elem_list != NULL) { free(_elem_list); }
	}
	//*해제 함수

	void _decompile_code(HANDLE hWrite, unsigned int code, unsigned int _indent);
	//*디컴파일 함수

	void _recompile_from_txt(wchar_t* _code_txt_buffer, unsigned int code);
	//*리컴파일 함수 (인자 : 해당 이벤트 블럭의 데이터가 들어있는 텍스트 데이터 일부)

	void _write_as_lsb(HANDLE hNewLsb, unsigned int code);
	//*lsb 파일로 기록하는 함수

	void _extract_text(HANDLE hTxt);
	//*텍스트 추출 함수

	void _replace_text(LSB_TEXTS_PER_INST* txt_list_elem) { ; }
	//*텍스트 교체 함수 (사용 안함)

	void _change_code_J2K();
	//*텍스트 코드 변경 함수
};
//*** 이벤트 클래스 전용 헤더? 클래스, 이 값들은 각각 type 1과 6에 관계되는 것 같다

class LSB_EVENT
{
public:
	wchar_t* _tpword;								//*** 매직 문자열(TpWord)
	wchar_t *_ver;									//*** 이벤트 블럭(TpWord)의 버전
	unsigned int _tpword_ver;						//*** 위 값을 정수화한 것

	LSB_EVENT_HEADER* _ev_header;					//*** 이벤트 헤더 (버전이 105 이상일 때 적용)

	unsigned int _attribute_count;					//*** 이벤트 내 변수들의 수
	LSB_EV_TXT_ATTRIBUTE** _attribute_list;			//*** 이벤트 내 변수들 리스트

	unsigned int _total_inst_count;					//*** 이벤트 내 명령어들의 수
	LSB_EVENT_ARG_LIST *_main_inst_list;			//*** 주요 이벤트 명령어 리스트

	unsigned int _several_inst_count;				//*** 각 문자별로 쪼갰을 때 순수한 개수

	LSB_EVENT ()
	{
		_tpword = _ver = NULL;
		_tpword_ver = 0;
		_ev_header = NULL;
		_attribute_count = 0;
		_attribute_list = NULL;
		_total_inst_count = 0;
		_main_inst_list = NULL;
		_several_inst_count = 0;
	}
	//*생성자

	~LSB_EVENT()
	{
		_release();
	}
	//*소멸자

	void _read_event_script(unsigned char** _lsb_buffer_pnt, unsigned int code);
	//*이벤트 블럭을 읽어들이는 함수

	void _release()
	{
		if (_tpword != NULL) { free(_tpword); }
		if (_ver != NULL) { free(_ver); }
		if (_ev_header != NULL) { delete _ev_header; }
		if (_attribute_list != NULL) {
			for (unsigned int i = 0; i < _attribute_count; i++) { delete _attribute_list[i]; }
			free(_attribute_list);
		}
		delete _main_inst_list;
	}
	//*해제 함수

	void _decompile_code(HANDLE hWrite, unsigned int code, unsigned int _indent);
	//*디컴파일 함수

	void _recompile_from_txt (wchar_t *_code_txt_buffer, unsigned int code);
	//*리컴파일 함수 (인자 : 해당 이벤트 블럭의 데이터가 들어있는 텍스트 데이터 일부)

	void _write_as_lsb(HANDLE hNewLsb, unsigned int code);
	//*lsb 파일로 기록하는 함수

	void _extract_text(HANDLE hTxt);
	//*텍스트 추출 함수

	void _replace_text(LSB_TEXTS_PER_INST* txt_list_elem);
	//*텍스트 교체 함수(안 쓰임)

	void _change_code_J2K();
	//*텍스트 코드 변경 함수
};
//*** 이벤트 블럭 클래스


typedef struct _map_data
{
	const wchar_t* _str;
	void* _data;
}LSB_MAP_DATA;
//*** 문자열과 데이터를 매핑해주는 구조체

class LSB_INST_BASE
{
public:
	//---- 수동으로 조정할 값들 ----//
	LSB_MAP_DATA* _data_chunks;						//*** 문자열과 각 패러미터 리스트 등 데이터를 매핑시킨 변수
	unsigned int _data_chunks_count;				//*** 패러미터 리스트의 수
	//---- 수동으로 조정할 값들 ----//
	unsigned int _script_ver;						//*** lsb 스크립트의 버전
	LSB_HEADER* _header;							//*** lsb 헤더 포인터
	unsigned int _indent;							//*** 들여쓰기 칸수?
	unsigned char _mute;							//*** 무시 여부?
	unsigned char _not_update;						//*** static 여부?
	unsigned int _line;								//*** 해당 명령어가 위치한 라인
	LSB_INST_TYPES _op_type;						//*** 명령어 타입
	//*공통 변수

	LSB_INST_BASE (unsigned int script_ver, LSB_HEADER* header)
	{
		_script_ver = script_ver;
		_header = header;
		_op_type = TYPE_INST_UNKNOWN;
		_data_chunks = NULL;
		_data_chunks_count = 0;
		_indent = 0;
		_mute = 0;
		_not_update = 0;
		_line = 0;
	}
	//*생성자
	
	~LSB_INST_BASE()
	{ 
		if (_data_chunks != NULL) {
			for (unsigned int i = 0; i < _data_chunks_count; i++) {
				if ((wcscmp(_data_chunks[i]._str, TEXT("Result")) == 0)
					|| (wcscmp(_data_chunks[i]._str, TEXT("Script_Page")) == 0)
					|| (wcscmp(_data_chunks[i]._str, TEXT("Label")) == 0)
					|| (wcscmp(_data_chunks[i]._str, TEXT("Target_Label_Offset")) == 0)
					|| (wcscmp(_data_chunks[i]._str, TEXT("Parameter_Count")) == 0)
					|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Name")) == 0)
					|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Type")) == 0)
					|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Scope")) == 0)
					|| (wcscmp(_data_chunks[i]._str, TEXT("Var_Scope_Byte")) == 0)
					|| (wcscmp(_data_chunks[i]._str, TEXT("All_Clear_Byte")) == 0))
				{ 
					free(_data_chunks[i]._data); 
				}
				else if (wcscmp(_data_chunks[i]._str, TEXT("Event_Block")) == 0)
				{
					LSB_EVENT* _t = (LSB_EVENT*)_data_chunks[i]._data;
					delete _t;
				}
				else { 
					LSB_PARAMETERS* _t = (LSB_PARAMETERS*)_data_chunks[i]._data;
					delete _t;
				}
				//*매칭된 문자열 값이 위 8개와 같다면 data에는 패러미터 리스트가 아닌 단순 데이터이므로 free
				//*혹은 "Event_Block"과 같다면 이벤트 블럭 데이터,
				//*이외 경우는 패러미터 리스트로 간주하여 delete를 수행한다
			}
			free(_data_chunks);
		}
	}
	//*파괴자 (인수들도 신경써서 해제함)

	LSB_MAP_DATA * _get_initialized_data_chunks (unsigned int __data_chunks_count)
	{
		LSB_MAP_DATA *__data_chunks = (LSB_MAP_DATA*)malloc(sizeof(LSB_MAP_DATA) * __data_chunks_count);
		memset(__data_chunks, 0, (sizeof(LSB_MAP_DATA) * __data_chunks_count));
		return __data_chunks;
	}
	//*패러미터 리스트 초기화된 값 반환

	void _read_inst_data(unsigned char** _lsb_buffer_pnt, unsigned int code) {
		unsigned char* lsb_buffer_pnt = (*_lsb_buffer_pnt);

		_indent = *(unsigned int*)(lsb_buffer_pnt); lsb_buffer_pnt += sizeof(int);
		_mute = *(lsb_buffer_pnt); lsb_buffer_pnt += sizeof(char);
		_not_update = *(lsb_buffer_pnt); lsb_buffer_pnt += sizeof(char);
		_line = *(unsigned int*)(lsb_buffer_pnt); lsb_buffer_pnt += sizeof(int);
		//*기초 데이터 읽어들이기

		this->_read_from_buffer(&lsb_buffer_pnt, code);
		(*_lsb_buffer_pnt) = lsb_buffer_pnt;
	}
	//*실제로 읽어들일 때 쓰일 함수

	virtual void _read_from_buffer (unsigned char** _lsb_buffer_pnt, unsigned int code) = 0;
	//*버퍼에서 읽어들이는 함수

	void _decompile_code(HANDLE hWrite, unsigned int offset, unsigned int code, unsigned int _indent);
	//*디컴파일 함수

	void _write_as_lsb(HANDLE hNewLsb, unsigned int code);
	//*lsb 파일로 기록하는 함수

	void _extract_text(HANDLE hTxt);
	virtual void _extract_text_each_inst(HANDLE hTxt) = 0;
	//*텍스트 추출 함수 및 각 명령어마다 추출하는 함수

	void _replace_text(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code);
	virtual void _replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code) = 0;
	//*텍스트 교체 함수 및 각 명령어마다 교체하는 함수

	void _change_code_J2K();
	//*텍스트 코드 변경 함수
};
//*기본 클래스

#define DEFINE_SCRIPT_CLASS(script_class) \
class script_class : public LSB_INST_BASE \
{ \
public: \
	script_class (LSB_INST_TYPES _op_type, unsigned int script_ver, LSB_HEADER* header); \
	void _read_from_buffer(unsigned char** _lsb_buffer_pnt, unsigned int code); \
	void _extract_text_each_inst(HANDLE hTxt); \
	void _replace_text_each_inst(LSB_TEXTS_PER_INST* txt_list_elem, unsigned int code); \
}
//*스크립트 함수들 원형 정의

DEFINE_SCRIPT_CLASS(LSB_INST_IF);							//*if 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_ELIF);							//*elif 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_ELSE);							//*else 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_LABEL);						//*label 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_JUMP);							//*jump 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_CALL);							//*call 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_EXIT);							//*exit 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_WAIT);							//*wait 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_TIMER);						//*timer 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_CALC_FUNC);					//*caculation function(특수함수) 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_BOX_NEW);						//*box_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_IMAGE_NEW);					//*image_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_MESSAGE_NEW);					//*message_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_PARTICLE_NEW);					//*particle_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_FIRE_NEW);						//*fire_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_EDIT_NEW);						//*edit_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_MEMO_NEW);						//*memo_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_MAP_IMAGE_NEW);				//*map_image_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_WAVE_NEW);						//*wave_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_TILE_NEW);						//*tile_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_SLIDER_NEW);					//*slider_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_SCROLL_BAR_NEW);				//*scroll_bar_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_GAUGE_NEW);					//*gauge_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_PREV_MENU_NEW);				//*prev_menu_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_VAR_NEW);						//*var_new 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_VAR_DELETE);					//*var_delete 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_MOVIE);						//*movie 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_FLIP);							//*flip 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_MOVIE_STOP);					//*movie_stop 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_CINEMA);						//*cinema 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_GET_PROPERTY);					//*get_property 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_SET_PROPERTY);					//*set_property 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_OBJECT_DELETE);				//*object_delete 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_TEXT_INST);					//*text_inst 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_TEXT_CLEAR);					//*text_clear 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_CLEAR_HISTORY);				//*clear_history 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_CALL_HISTORY);					//*call_history 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_FORMAT_HISTORY);				//*format_history 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_CAPTION);						//*caption 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_CG_CAPTION);					//*cg_caption 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_MENU);							//*menu 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_MENU_CLOSE);					//*menu_close 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_BUTTON);						//*button 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_COMMENT);						//*comment 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_WHILE);						//*while 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_WHILE_INIT);					//*while_init 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_WHILE_LOOP);					//*while_loop 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_BREAK);						//*break 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_CONTINUE);						//*continue 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_GAME_SAVE);					//*game_save 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_GAME_LOAD);					//*game_load 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_CLEAR_LOAD);					//*clear_load 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_PC_RESET);						//*pc_reset 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_RESET);						//*reset 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_SOUND);						//*sound 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_MEDIA_PLAY);					//*media_play 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_TERMINATE);					//*terminate 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_DO_EVENT);						//*do_event 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_PROPERTY_MOTION);				//*property_motion 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_SAVE_CABINET);					//*save_cabinet 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_LOAD_CABINET);					//*load_cabinet 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_MACRO_IFDEF);					//*IFDEF 전처리 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_MACRO_IFNDEF);					//*IFNDEF 전처리 명령어
DEFINE_SCRIPT_CLASS(LSB_INST_MACRO_ENDIF);					//*ENDIF 전처리 명령어

//*** 기본 인스트럭션 클래스 및 파생 클래스들




class LSB_HEADER
{
public:
	unsigned int _script_ver;					//*** lsb 스크립트의 버전
	unsigned int opcode_count;					//*** 명령어 오브젝트의 수? (대체로 0x40이 될듯)
	unsigned int opcode_property_count;			//*** 해당 명령어가 가질 수 있는 속성 수? (대체로 0x16인듯?)
	unsigned char** opcode_property_table;		//*** 각 명령어 코드에 대한 추가 정보를 담은 테이블
	bool** opcode_property_boolean_table;		//*** 위 테이블의 boolean 버전
	//*이것들은 아마 해당 명령어를 

	LSB_HEADER(unsigned int script_ver)
	{
		_script_ver = script_ver;
		opcode_count = opcode_property_count = 0;
		opcode_property_table = NULL;
		opcode_property_boolean_table = NULL;
	}
	//*생성자

	~LSB_HEADER()
	{
		if (opcode_property_table != NULL) {
			for (unsigned int i = 0; i < opcode_count; i++) { free(opcode_property_table[i]); }
			free(opcode_property_table); opcode_property_table = NULL;
		}
		if (opcode_property_boolean_table != NULL) {
			for (unsigned int i = 0; i < opcode_count; i++) { free(opcode_property_boolean_table[i]); }
			free(opcode_property_boolean_table); opcode_property_boolean_table = NULL;
		}
	}
	//*파괴자

	bool Header_Read(unsigned char* _org_pnt, unsigned char** _lsb_buffer_pnt);
	//*버퍼에서 데이터 받아오기

	void _decompile_code (HANDLE hWrite);
	//*디컴파일 함수

	void _write_as_lsb(HANDLE hNewLsb);
	//*lsb 파일로 기록하는 함수
};
//*** 헤더 구조체


class LSB_BODY
{
public:
	unsigned int _script_ver;					//*** lsb 스크립트의 버전
	LSB_HEADER* _header;						//*** lsb 스크립트의 헤더
	unsigned int inst_list_count;				//*** 명령어 묶음의 총 수, 사실상 함수와 같은 역할
	LSB_INST_BASE** inst_list_arr;				//*** 명령어 묶음 데이터

	LSB_BODY(unsigned int script_ver, LSB_HEADER *header)
	{
		_script_ver = script_ver;
		_header = header;
		inst_list_count = 0;
		inst_list_arr = NULL;
	}
	//*생성자

	~LSB_BODY()
	{
		if (inst_list_arr != NULL) {
			for (unsigned int i = 0; i < inst_list_count; i++) { 
				if (inst_list_arr[i] != NULL) { delete inst_list_arr[i]; }
			}
			delete inst_list_arr; inst_list_arr = NULL;
			//*전부 해제하고 없앰
		}
	}
	//*파괴자

	bool Body_Read(unsigned char* _org_pnt, unsigned char** _lsb_buffer_pnt, unsigned int code);
	//*버퍼에서 데이터 받아오기

	void _decompile_code (HANDLE hWrite, unsigned int code);
	//*디컴파일 함수

	void _write_as_lsb(HANDLE hNewLsb, unsigned int code);
	//*lsb 파일로 기록하는 함수

	void _extract_text(HANDLE hTxt);
	//*텍스트 추출 함수

	void _replace_text(LSB_TEXTS_PER_INST *txt_list, unsigned int txt_list_count, unsigned int code);
	//*텍스트 교체 함수

	void _change_code_J2K();
	//*텍스트 코드 변경 함수
};
//*** 몸통 구조체


class LSB_SCRIPT : public LM_BASE_SCRIPT
{
public:
	unsigned int lm_version;					//*** 스크립트 버전
	bool flag;									//*** 스크립트 플래그?
	LSB_HEADER* _header;						//*** 헤더 데이터
	LSB_BODY* _body;							//*** 몸체 데이터

	LSB_SCRIPT()
	{
		lm_version = 0x00000000;
		flag = false;
		_header = NULL;
		_body = NULL;
	}
	//*생성자

	~LSB_SCRIPT()
	{
		if (_header != NULL) { delete _header; _header = NULL; }
		if (_body != NULL) { delete _body; _body = NULL; }
	}
	//*소멸자

	bool Script_Analysis(wchar_t* LsbFile, unsigned int code);
	//*lsb 파일을 불러와 분석하는 함수

	void Decompile_To_Code(wchar_t* TxtFile, unsigned int code);
	//*디컴파일해서 기록하는 함수

	void Make_To_Script(wchar_t *newLsbFile, unsigned int code);
	//*lsb 파일로 기록하는 함수

	void Extract_Text_Data(wchar_t* DstTxtFile);
	//*텍스트 추출 함수

	void Replace_Text_Data(wchar_t* SrcTxtFile, unsigned int code);
	//*텍스트 교체 함수

	void Convert_Codepage_J2K();
	//*텍스트 언어코드를 일본어에서 한글로 바꾸는 함수

private:
	bool LSB_Header_Analysis (wchar_t* LsbFile, unsigned char* _org_pnt, unsigned char** _lsb_buffer_pnt);
	//*헤더 분석 함수

	bool LSB_Body_Analysis(wchar_t* LsbFile, unsigned char* _org_pnt, unsigned char** _lsb_buffer_pnt, unsigned int code);
	//*몸통 분석 함수
};
//*** 스크립트 전체를 포괄하는 구조체