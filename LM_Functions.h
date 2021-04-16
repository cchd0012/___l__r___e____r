#pragma once
#include <locale.h>
#include <fcntl.h>
#include "LM_Base.h"


//*** 호출 함수들의 기반이 되는 함수를 선언하는 헤더


#define DEFAULT_BLOCK_COUNT			100
//*스크램블 시 기준이 되는 블럭 수

#define LM_CP_JAP				0x0
#define LM_CP_KOR				0x1


static const wchar_t* separation_line = TEXT("---------------------------------------\r\n");
static const wchar_t* _enter_raw_ = TEXT("\r\n");
static const wchar_t* _enter_plained_ = TEXT("\\r\\n");
static const wchar_t* jap_prefix = TEXT("jap");
static const wchar_t* kor_prefix = TEXT("kor");
static const wchar_t* event_op_type_prefix = TEXT("EV_OP");
static const wchar_t* property_array_prefix = TEXT("_OpCode_Property_");
static const wchar_t* text_prefix = TEXT("[◆]");
static const wchar_t* attr_apply_prefix = TEXT("(＃)");
static const wchar_t* var_idx_prefix = TEXT("VAR_IDX");
static const wchar_t* line_prefix = TEXT("__LINE__");
static const wchar_t* offset_prefix = TEXT("__OFFSET__");
static const wchar_t* inst_type_prefix = TEXT("Inst_Type");
static const wchar_t* arg_name_prefix = TEXT("Arg_Name");
static const wchar_t hex_1_raw[] = { 0x01 };
static const wchar_t* hex_1_plained = TEXT("[\\x01]");
static const wchar_t* event_attr_prefix = TEXT("_EVENT_ATTR_");
static const wchar_t* event_header_prefix = TEXT("_EVENT_HEADER_");
static const wchar_t* event_inst_prefix = TEXT("_EVENT_INST_");
static const wchar_t* event_head_elem_prefix = TEXT("_HEAD_ELEM_");
static const wchar_t* refer_attr_prefix = TEXT("_ATTR_ELEM_");
static const wchar_t* color_prefix = TEXT("_COLOR_");
static const wchar_t* font_prefix = TEXT("_FONT_");
static const wchar_t* ruby_txt_prefix = TEXT("_RUBY_TXT_");
static const wchar_t* ev_attr_idx_prefix = TEXT("_IDX_");
static const wchar_t* ev_attr_def_idx_prefix = TEXT("_DEF_IDX_");
static const char* sharp_encoding_prefix = "[#]";
static const wchar_t* _indent_ = TEXT("  ");
static const wchar_t* text_header_prefix = TEXT("Extracted Text");
static const wchar_t* text_data_prefix = TEXT("__text__");
static const wchar_t* proj_name_prefix = TEXT("Project_Name");
static const wchar_t* disk_insert_prefix = TEXT("Disk_Insert_Prompt");
static const wchar_t* exit_prefix = TEXT("Exit_Prompt");
static const wchar_t* system_setting_prefix = TEXT("System_Setting");
//*텍스트 관련 프리픽스들

static const wchar_t* _inst_type_list[] =
{
	TEXT("_IF"), TEXT("_ELIF"), TEXT("_ELSE"), TEXT("_LABEL"),
	TEXT("_JUMP"), TEXT("_CALL"), TEXT("_EXIT"), TEXT("_WAIT"),
	TEXT("_BOX_NEW"), TEXT("_IMAGE_NEW"), TEXT("_MESSAGE_NEW"), TEXT("_TIMER"),
	TEXT("_MOVIE"), TEXT("_FLIP"), TEXT("_CALC_FUNC"), TEXT("_VAR_NEW"),
	TEXT("_VAR_DELETE"), TEXT("_GET_PROPERTY"), TEXT("_SET_PROPERTY"), TEXT("_OBJECT_DELETE"),
	TEXT("_TEXT_INST"), TEXT("_MOVIE_STOP"), TEXT("_CLEAR_HISTORY"), TEXT("_CINEMA"),
	TEXT("_CAPTION"), TEXT("_MENU"), TEXT("_MENU_CLOSE"), TEXT("_COMMENT"),
	TEXT("_TEXT_CLEAR"), TEXT("_CALL_HISTORY"), TEXT("_BUTTON"), TEXT("_WHILE"),
	TEXT("_WHILE_INIT"), TEXT("_WHILE_LOOP"), TEXT("_BREAK"), TEXT("_CONTINUE"),
	TEXT("_PARTICLE_NEW"), TEXT("_FIRE_NEW"), TEXT("_GAME_SAVE"), TEXT("_GAME_LOAD"),
	TEXT("_PC_RESET"), TEXT("_RESET"), TEXT("_SOUND"), TEXT("_EDIT_NEW"),
	TEXT("_MEMO_NEW"), TEXT("_TERMINATE"), TEXT("_DO_EVENT"), TEXT("_CLEAR_LOAD"),
	TEXT("_MAP_IMAGE_NEW"), TEXT("_WAVE_NEW"), TEXT("_TILE_NEW"), TEXT("_SLIDER_NEW"),
	TEXT("_SCROLL_BAR_NEW"), TEXT("_GAUGE_NEW"), TEXT("_CG_CAPTION"), TEXT("_MEDIA_PLAY"),
	TEXT("_PREV_MENU_NEW"), TEXT("_PROPERTY_MOTION"), TEXT("_FORMAT_HISTORY"), TEXT("_SAVE_CABINET"),
	TEXT("_LOAD_CABINET"), TEXT("_MACRO_IFDEF"), TEXT("_MACRO_IFNDEF"), TEXT("_MACRO_ENDIF")
};
//*** 명령어(inst) 타입 문자열 프리픽스 배열

static const wchar_t* _op_list[] =
{
	TEXT("_none_"),
	TEXT("Assign('=')"), TEXT("Plus('+')"), TEXT("Minus('-')"), TEXT("Multiply('*')"), TEXT("Divide('/')"), TEXT("Modulus('%')"),
	TEXT("Or('|')"), TEXT("And('&')"), TEXT("Xor('^')"),
	TEXT("Array_Op('[]')"), TEXT("Function('()')"),
	TEXT("Equals('==')"), TEXT("Greater('>')"), TEXT("Less('<')"), TEXT("Greater_or_Equal('>=')"), TEXT("Less_or_Equal('<=')"),
	TEXT("Left_Shift('<<')"), TEXT("Right_Shift('>>')"),
	TEXT("Combo_String('++')"), TEXT("Not_Equal('!=')"),
};
//*** 연산자(operation) 문자열 배열

static const wchar_t* _cf_list[] =
{ 
	TEXT("_IntToStr"), TEXT("_IntToHex"), TEXT("_GetProp"), TEXT("_SetProp"),
	TEXT("_GetArraySize"), TEXT("_Length"), TEXT("_JLength"), TEXT("_Copy"),
	TEXT("_JCopy"), TEXT("_Delete"), TEXT("_JDelete"), TEXT("_Insert"),
	TEXT("_JInsert"), TEXT("_CompareStr"), TEXT("_CompareText"), TEXT("_Pos"),
	TEXT("_JPos"), TEXT("_Trim"), TEXT("_JTrim"), TEXT("_Exists"),
	TEXT("_Not"), TEXT("_SetArray"), TEXT("_FillMem"), TEXT("_CopyMem"),
	TEXT("_GetCheck"), TEXT("_SetCheck"), TEXT("_Random"), TEXT("_GetSaveCaption"),
	TEXT("_ArrayToString"), TEXT("_StringToArray"), TEXT("_IndexOfStr"), TEXT("_SortStr"),
	TEXT("_ListCompo"), TEXT("_ToClientX"), TEXT("_ToClientY"), TEXT("_ToScreenX"),
	TEXT("_ToScreenY"), TEXT("_Int"), TEXT("_Float"), TEXT("_Sin"), TEXT("_Cos"),
	TEXT("_Tan"), TEXT("_ArcSin"), TEXT("_ArcCos"), TEXT("_ArcTan_1"), TEXT("_ArcTan_2"),
	TEXT("_Hypot"), TEXT("_IndexOfMenu"), TEXT("_Abs"), TEXT("_Fabs"), TEXT("_VarExists"),
	TEXT("_EncodeDate"), TEXT("_EncodeTime"), TEXT("_DecodeDate"), TEXT("_DecodeTime"),
	TEXT("_GetYear"), TEXT("_GetMonth"), TEXT("_GetDay"), TEXT("_GetHour"), TEXT("_GetMin"),
	TEXT("_GetSec"), TEXT("_GetWeek"), TEXT("_GetWeekStr"), TEXT("_GetWeekJStr"), TEXT("_FixStr"),
	TEXT("_GetDisplayMode"), TEXT("_AddArray"), TEXT("_InsertArray"), TEXT("_DeleteArray"),
	TEXT("_InPrimary"), TEXT("_CopyArray"), TEXT("_FileExists"), TEXT("_LoadTextFile"), TEXT("_LowerCase"),
	TEXT("_UpperCase"), TEXT("_ExtractFilePath"), TEXT("_ExtractFileName"), TEXT("_ExtractFileExt"),TEXT("_IsPathDelimiter"),
	TEXT("_AddBackSlash"), TEXT("_ChangeFileExt"), TEXT("_IsDelimiter"), TEXT("_StringOfChar"), TEXT("_StringReplace"),
	TEXT("_AssignTemp"), TEXT("_HanToZen"), TEXT("_ZenToHan"), TEXT("_DBCreateTable"), TEXT("_DBSetActive"),
	TEXT("_DBAddField"), TEXT("_DBSetRecNo"), TEXT("_DBInsert"), TEXT("_DBDelete"), TEXT("_DBGetInt"),
	TEXT("_DBSetInt"), TEXT("_DBGetFloat"), TEXT("_DBSetFloat"), TEXT("_DBGetBool"), TEXT("_DBSetBool"),
	TEXT("_DBGetStr"), TEXT("_DBSetStr"), TEXT("_DBRecordCount"), TEXT("_DBFindFirst"), TEXT("_DBFindLast"),
	TEXT("_DBFindNext"), TEXT("_DBFindPrior"), TEXT("_DBLocate"), TEXT("_DBLoadTsvFile"), TEXT("_DBDirectGetInt"),
	TEXT("_DBDirectSetInt"), TEXT("_DBDirectGetFloat"), TEXT("_DBDirectSetFloat"), TEXT("_DBDirectGetBool"),
	TEXT("_DBDirectSetBool"), TEXT("_DBDirectGetStr"), TEXT("_DBDirectSetStr"), TEXT("_DBCopyTable"),
	TEXT("_DBDeleteTable"), TEXT("_DBInsertTable"), TEXT("_DBCopy"), TEXT("_DBClearTable"), TEXT("_DBSort"),
	TEXT("_DBGetActive"), TEXT("_DBGetRecNo"), TEXT("_DBClearRecord"), TEXT("_SetWallPaper"), TEXT("_Min"),
	TEXT("_Max"), TEXT("_Fmin"), TEXT("_Fmax"), TEXT("_GetVarType"), TEXT("_GetEnabled"), TEXT("_SetEnabled"),
	TEXT("_AddDelimiter"), TEXT("_ListSaveCaption"), TEXT("_OpenUrl"), TEXT("_Calc"), TEXT("_SaveScreen"),
	TEXT("_StrToIntDef"), TEXT("_StrToFloatDef"), TEXT("_GetVisible"), TEXT("_SetVisible"), TEXT("_GetHistoryCount"),
	TEXT("_GetHistoryMaxCount"), TEXT("_SetHistoryMaxCount"), TEXT("_GetGroupIndex"), TEXT("_GetSelected"),
	TEXT("_SetSelected"), TEXT("_SelectOpenFile"), TEXT("_SelectSaveFile"), TEXT("_SelectDirectory"), TEXT("_ExtractFile"),
	TEXT("_Chr"), TEXT("_Ord"), TEXT("_InCabinet"), TEXT("_PushVar"), TEXT("_PopVar"), TEXT("_DeleteStack"),
	TEXT("_CopyFile"), TEXT("_DBGetTableCount"), TEXT("_DBGetTable"), TEXT("_CreateObject"), TEXT("_DeleteObject"),
	TEXT("_GetItem"), TEXT("_UniqueArray"), TEXT("_TrimArray"), TEXT("_GetImeOpened"), TEXT("_SetImeOpened"),
	TEXT("_Alert")
};
//*** calc_func 문자열 배열

typedef struct _buffer_info
{
	UINT32 _size;
	UINT8* _buffer;
}_BUFFER_INFO;

typedef struct _txt_replace
{
	_BUFFER_INFO* n_b;
	_BUFFER_INFO* n_b_sav;
	UINT8* rev1;
	UINT8* rev2;

	UINT8* getBuffer() { if (n_b != NULL) { return n_b->_buffer; } else { return NULL; } }
	UINT32 getBufferSize() { if (n_b != NULL) { return n_b->_size; } else { return -1; } }
	//*간단히 현재 버퍼 / 크기 얻는 함수

	void init_copy(UINT8* _buffer, UINT32 _buffer_size)
	{
		n_b = (_BUFFER_INFO*)malloc(sizeof(_BUFFER_INFO));
		n_b->_size = _buffer_size;
		n_b->_buffer = (UINT8*)malloc((_buffer_size * 2));
		memset(n_b->_buffer, 0, (_buffer_size * 2));
		memcpy(n_b->_buffer, _buffer, _buffer_size);

		n_b_sav = (_BUFFER_INFO*)malloc(sizeof(_BUFFER_INFO));
		n_b_sav->_size = 0;
		n_b_sav->_buffer = (UINT8*)malloc((_buffer_size * 2));
		memset(n_b_sav->_buffer, 0, (_buffer_size * 2));

		rev1 = (UINT8*)malloc((_buffer_size * 2));
		rev2 = (UINT8*)malloc((_buffer_size * 2));
		memset(rev1, 0, (_buffer_size * 2));
		memset(rev2, 0, (_buffer_size * 2));
	}
	//*간단한 초기화 함수, 처음에는 미리 복사해 둔다
	//*공간에 충분한 여유를 두고 생성해야 한다

	void destroy()
	{
		free(n_b->_buffer); free(n_b_sav->_buffer);
		free(n_b); free(n_b_sav);
		free(rev1); free(rev2);
	}
	//*간단한 파기 함수
}TEX_REPLACE;
//*텍스트 교체 시 필요한 구조체


void Initialize_Tool();
void Terminate_Tool();
//*툴 초기화 함수 / 마치는 함수


HANDLE LmCreateFile (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
//*** CreateFile 개량함수. 모든 디렉토리 한번에 생성

wchar_t* GetUniStr(char* string, unsigned int code);
char* GetAnsiStr(wchar_t* string, unsigned int code);
//*** ansi <-> 유니코드간 변환 함수

void Unscramble_Data(unsigned char** Buffer, unsigned int* BufferSize);
//*** 스크램블된 데이터를 풀어주는 함수

bool Scramble_Data(unsigned char** Buffer, unsigned int* BufferSize);
//*** 데이터 스크램블링, 스크램블링을 성공했다는 신호를 보내야 하므로 bool 함수로 한다

unsigned int Get_Files_Pass_List(wchar_t* DataFolder, FILE_DESC **f_desc_list_pnt);
//*파일경로를 매칭시켜 주는 함수 / 뒤쪽이 null이면 그냥 수만 세서 리턴

bool Replace_Data(TEX_REPLACE* rep_util, UINT32 org_data_offset, UINT32 org_data_length, UINT8* new_data, UINT32 new_data_length);
//*데이터 교체 함수

UINT32 Get_Txt_Count(wchar_t* src_txt_buffer, const wchar_t* find_txt, UINT32 src_txt_size);
//*현재 텍스트 버퍼 내에 찾는 텍스트가 몇 개나 있는지 파악하는 함수

wchar_t* Replace_Text(wchar_t* org_txt, const wchar_t* org_part, const wchar_t* change_part);
//*문자열 교체 함수 (결과값으로 바뀐 문자열 반환)

void Load_Hanja();
//*"Hanja.txt" 파일 로드 함수

void Release_Hanja();
//*"Hanja.txt" 파일 해제 함수

void JPHan_2_KRHan(wchar_t* str);
//*정해진 문자열에서 바꿀 수 있는 한자는 모두 한국한자로 바꾸는 함수

bool is_Pure_Ascii_String(wchar_t* _str);
//*해당 문자열이 순수 ascii 문자열인지 판별하는 함수

bool is_File_Path_String(wchar_t* _str);
//*해당 문자열이 경로 문자열인지 판별하는 함수


char* _get_str(unsigned char** _buffer_pnt);
void _write_str(HANDLE hWrite, char* _str);
//*문자열을 읽어들이는 / 기록하는 함수

double* _get_double(unsigned char** _buffer_pnt);
void _write_double(HANDLE hWrite, double _do);
//*double을 읽어들이는 / 기록하는 함수

unsigned int* _get_int(unsigned char** _buffer_pnt);
void _write_int(HANDLE hWrite, unsigned int _int);
//*정수를 읽어들이는 / 기록하는 함수

unsigned short* _get_word(unsigned char** _buffer_pnt);
void _write_word(HANDLE hWrite, unsigned short _s);
//*워드를 읽어들이는 / 기록하는 함수

unsigned char* _get_byte(unsigned char** _buffer_pnt);
void _write_byte(HANDLE hWrite, unsigned char _c);
//*바이트를 읽어들이는 / 기록하는 함수

unsigned char* _get_data(unsigned char** _buffer_pnt, unsigned int _len);
void _write_data(HANDLE hWrite, unsigned char* _buff, unsigned int _len);
//*지정 길이만큼 읽어들이는 / 기록하는 함수

char* stristr(const char* str1, const char* str2);
wchar_t* wcsistr(const wchar_t* str1, const wchar_t* str2);
//*대소문자 구분 없는 strstr / wcsstr


//void Make_J2K_Table();