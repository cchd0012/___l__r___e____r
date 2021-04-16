#pragma once
#include "LM_Base.h"


//*** 텍스트 관련 함수를 선언하는 헤더


void LM_Convert_Codepage_J2K(char* DataFolder);
//*** 폴더 내 파일 코드페이지 일괄 변경 함수

void LM_Convert_Exe_Codepage_J2K(char* ExeFile);
//*exe파일 코드페이지 변경용 함수

void LM_Extract_All_Script_Text(char *DataFolder, unsigned int code);
//*해당 디렉토리 내 모든 lsb/lpb/dat 파일 텍스트를 추출하는 함수

void LM_Replace_All_Script_Text(char* DataFolder, unsigned int code);
//*해당 디렉토리 내 모든 lsb/lpb/dat 파일을 대치하는 함수

void LM_Decompile_All_Script(char* DataFolder, unsigned int code);
//*해당 디렉토리 내 모든 스크립트 파일을 디컴파일하는 함수