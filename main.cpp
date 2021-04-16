#include "LM_Extract.h"
#include "LM_Pack.h"
#include "LM_Text.h"
#include "LM_Others.h"
#include "LM_Functions.h"
#include "LM_Script.h"


int main (int argc, char *argv[])
{
	Initialize_Tool();

	switch (argc)
	{
		case 7:
		{
			if (strcmp(argv[1], "-p") == 0) { 
				char *ExeFile = NULL; 
				int CodePage = LM_CP_JAP;
				//*디폴트 옵션 세팅
				for (int cdpg_idx = 2;cdpg_idx < 6;cdpg_idx += 2) {
					if (strcmp(argv[cdpg_idx], "--code") == 0) {
						if ((strcmp(argv[cdpg_idx+1], "-jap") == 0)) {
							CodePage = LM_CP_JAP;
							break;
							//*(--code 옵션 지정 : 일본어)
						}
						else if ((strcmp(argv[cdpg_idx+1], "-kor") == 0)) {
							CodePage = LM_CP_KOR;
							break;
							//*(--code 옵션 지정 : 한국어)
						}
						else { goto DEF_OPER; }
						//*올바른 옵션이 없으면 디폴트 동작 실행
					}
				}
				//*--onto 옵션 확인 (인덱스 위치를 판단하기 위한 반복 루프)
				for (int cdpg_idx = 2;cdpg_idx < 6;cdpg_idx += 2) {
					if (strcmp(argv[cdpg_idx], "--onto") == 0) {
						ExeFile = argv[cdpg_idx+1];
						break;
						//*(--onto 옵션 지정)
					}
				}
				if (ExeFile == NULL) { goto DEF_OPER; }
				//*--code 옵션 확인 (인덱스 위치를 판단하기 위한 반복 루프)
				LM_Pack (argv[6], ExeFile, CodePage);
				break; 
			}
			//*데이터 병합 함수 (--onto, --code 옵션 지정)
			//*exe 파일 위에 병합

			else { goto DEF_OPER; }
			//*올바른 옵션이 없으면 디폴트 동작 실행
		}

		case 6:
		{
			if (strcmp(argv[1], "-x") == 0) { 
				char *DataFile = argv[2]; 
				int CodePage = LM_CP_JAP;
				//*디폴트 옵션 세팅
				for (int cdpg_idx = 2;cdpg_idx < 4;cdpg_idx++) {
					if (strcmp(argv[cdpg_idx], "--code") == 0) {
						if ((strcmp(argv[cdpg_idx+1], "-jap") == 0)) {
							CodePage = LM_CP_JAP;
							break;
							//*(--code 옵션 지정 : 일본어)
						}
						else if ((strcmp(argv[cdpg_idx+1], "-kor") == 0)) {
							CodePage = LM_CP_KOR;
							break;
							//*(--code 옵션 지정 : 한국어)
						}
						else { goto DEF_OPER; }
						//*올바른 옵션이 없으면 디폴트 동작 실행
					}
				}
				//*--code 옵션 확인 (인덱스 위치를 판단하기 위한 반복 루프)
				LM_Extract (DataFile, argv[5], CodePage); 
				break; 
			}
			//*데이터 분해 함수 (--code 옵션 지정)

			else { goto DEF_OPER; }
			//*올바른 옵션이 없으면 디폴트 동작 실행
		}


		case 5:
		{
			if (strcmp(argv[1], "-p") == 0) { 
				char *ExeFile = NULL; 
				int CodePage = LM_CP_KOR;
				//*디폴트 옵션 세팅
				if (strcmp(argv[2], "--onto") == 0) {
					ExeFile = argv[3];
					//*(--onto 옵션 지정)
				}
				//*--onto 옵션 확인
				else if (strcmp(argv[2], "--code") == 0) {
					if ((strcmp(argv[3], "-jap") == 0)) {
						CodePage = LM_CP_JAP;
						//*(--code 옵션 지정 : 일본어)
					}
					else if ((strcmp(argv[3], "-kor") == 0)) {
						CodePage = LM_CP_KOR;
						//*(--code 옵션 지정 : 한국어)
					}
					else { goto DEF_OPER; }
					//*올바른 옵션이 없으면 디폴트 동작 실행
				}
				//*--code 옵션 확인
				else { goto DEF_OPER; }
				//*올바른 옵션이 없으면 디폴트 동작 실행
				LM_Pack (argv[4], ExeFile, CodePage);
				break;
			}
			//*데이터 병합 함수 (--onto 혹은 --code 옵션 지정)
			//*--code가 있으면 ~ext와 ~dat으로 나눠 병합
			//*--onto가 있으면 exe 파일 위에 병합

			else if (strcmp(argv[1], "-extt") == 0) {
				int CodePage = LM_CP_JAP;
				//*디폴트 옵션 세팅

				if (strcmp(argv[2], "--code") == 0) {
					if ((strcmp(argv[3], "-jap") == 0)) {
						CodePage = LM_CP_JAP;
						//*(--code 옵션 지정 : 일본어)
					}
					else if ((strcmp(argv[3], "-kor") == 0)) {
						CodePage = LM_CP_KOR;
						//*(--code 옵션 지정 : 한국어)
					}
					else { goto DEF_OPER; }
					//*올바른 옵션이 없으면 디폴트 동작 실행
				}
				//*--code 옵션 확인
				else { goto DEF_OPER; }
				//*올바른 옵션이 없으면 디폴트 동작 실행

				LM_Extract_All_Script_Text(argv[4], CodePage);
				break;
			}
			//*** 폴더 내 lsb 파일 일괄 추출 함수

			else if (strcmp(argv[1], "-rept") == 0) {
				int CodePage = LM_CP_KOR;
				//*디폴트 옵션 세팅

				if (strcmp(argv[2], "--code") == 0) {
					if ((strcmp(argv[3], "-jap") == 0)) {
						CodePage = LM_CP_JAP;
						//*(--code 옵션 지정 : 일본어)
					}
					else if ((strcmp(argv[3], "-kor") == 0)) {
						CodePage = LM_CP_KOR;
						//*(--code 옵션 지정 : 한국어)
					}
					else { goto DEF_OPER; }
					//*올바른 옵션이 없으면 디폴트 동작 실행
				}
				//*--code 옵션 확인
				else { goto DEF_OPER; }
				//*올바른 옵션이 없으면 디폴트 동작 실행

				LM_Replace_All_Script_Text(argv[4], CodePage);
				break;
			}
			//*** 폴더 내 lsb 파일 일괄 대치 함수

			else if (strcmp(argv[1], "-decm") == 0) {
				int CodePage = LM_CP_KOR;
				//*디폴트 옵션 세팅

				if (strcmp(argv[2], "--code") == 0) {
					if ((strcmp(argv[3], "-jap") == 0)) {
						CodePage = LM_CP_JAP;
						//*(--code 옵션 지정 : 일본어)
					}
					else if ((strcmp(argv[3], "-kor") == 0)) {
						CodePage = LM_CP_KOR;
						//*(--code 옵션 지정 : 한국어)
					}
					else { goto DEF_OPER; }
					//*올바른 옵션이 없으면 디폴트 동작 실행
				}
				//*--code 옵션 확인
				else { goto DEF_OPER; }
				//*올바른 옵션이 없으면 디폴트 동작 실행

				LM_Decompile_All_Script(argv[4], CodePage);
				break;
			}
			//*** 폴더 내 스크립트 일괄 디컴파일 함수

			else { goto DEF_OPER; }
			//*올바른 옵션이 없으면 디폴트 동작 실행
		}


		case 4:
		{
			if (strcmp(argv[1], "-x") == 0) { 
				LM_Extract (argv[2], argv[3], LM_CP_JAP); 
				break; 
			}
			//*데이터 분해 함수 (--code 옵션 미지정)

			else { goto DEF_OPER; }
			//*올바른 옵션이 없으면 디폴트 동작 실행
		}


		case 3:
		{
			if (strcmp(argv[1], "-p") == 0) { 
				LM_Pack (argv[2], NULL, LM_CP_KOR); 
				break; 
			}
			//*데이터 병합 함수 (--onto, --code 옵션 미지정)
			//*~ext와 ~dat으로 나눠 병합

			else if (strcmp(argv[1], "-cvt") == 0) {
				LM_Convert_Codepage_J2K(argv[2]);
				break; 
			}
			//*** 폴더 내 파일 코드페이지 일괄 변경 함수 (--code 옵션 미지정)

			else if (strcmp(argv[1], "-cvte") == 0) {
				LM_Convert_Exe_Codepage_J2K(argv[2]);
				break;
			}
			//*** exe 내 문자열 코드페이지 일괄 변경 함수 (--code 옵션 미지정)

			else if (strcmp(argv[1], "-extt") == 0) {
				LM_Extract_All_Script_Text(argv[2], LM_CP_JAP);
				break;
			}
			//*** 폴더 내 lsb 파일 일괄 추출 함수 (--code 옵션 미지정)

			else if (strcmp(argv[1], "-rept") == 0) {
				LM_Replace_All_Script_Text(argv[2], LM_CP_KOR);
				break;
			}
			//*** 폴더 내 lsb 파일 일괄 대치 함수 (--code 옵션 미지정)

			else if (strcmp(argv[1], "-decm") == 0) {
				LM_Decompile_All_Script(argv[2], LM_CP_JAP);
				break;
			}
			//*** 폴더 내 스크립트 일괄 디컴파일 함수 (--code 옵션 미지정)

			else { goto DEF_OPER; }
			//*올바른 옵션이 없으면 디폴트 동작 실행
		}


		case 2:
		{
			if (strcmp(argv[1], "-h") == 0) { LM_Help(); break; }
			//*사용법 출력 함수

			else { goto DEF_OPER; }
			//*올바른 옵션이 없으면 디폴트 동작 실행
		}


		case 1:
		default:
		{ 
DEF_OPER:
			LM_Help(); break; 
		}
		//*사용법 출력 함수
	}

//	LSB_SCRIPT *_scr = new LSB_SCRIPT();
//	_scr->Script_Analysis(TEXT("00000001.lsb"), LM_CP_KOR);
//	_scr->Convert_Codepage_J2K();
//	_scr->Decompile_To_Code(TEXT("00000101.txt"), LM_CP_JAP);
//	_scr->Extract_Text_Data(TEXT("00000101.ext.txt"));
//	_scr->Replace_Text_Data(TEXT("00000001.ext.txt"), LM_CP_KOR);
//	_scr->Convert_Codepage_J2K();
//	_scr->Make_To_Script(TEXT("00000101_n.lsb"), LM_CP_KOR);
//	delete _scr; 

//	LM_Convert_Codepage_J2K("Output");
//	LM_Convert_Exe_Codepage_J2K("koifem_pure.exe");

//	LM_Extract("game.exe", "Output", LM_CP_JAP);
//	LM_Pack("Output_mbsn3", NULL, LM_CP_KOR);

//	LM_Decompile_All_Script("Output", LM_CP_KOR);

	Terminate_Tool();
	return 0;
}