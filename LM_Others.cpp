#include "LM_Others.h"
#include "LM_Functions.h"


//*** 기타 함수들 기능을 서술하는 파일


void LM_Help()
{
	printf ("\nToolForLiveMaker v%1.2f\n", __VERSION);
	printf ("사용법 : ToolForLiveMaker [option] [param]\n");
	printf ("(option과 param이 없다면 'ToolForLiveMaker -h' 실행)\n\n");
	printf ("options and param:\n");
	printf ("\n\t-x ['~.exe' or '~.dat'] [--code [codepage]] [folder]\n");
	printf ("\t   해당 exe나 dat 파일에서 지정한 폴더로 데이터 추출\n");
	printf ("\t   (exe 파일일 때 데이터를 제외한 '~_pure.exe' 파일을 새로 만듦)\n");
	printf ("\t   (지정한 폴더가 없을 시 자동 생성)\n");
	printf ("\t   (--code 옵션 지정 시 -jap/-kor 옵션이 필요하며)\n");
	printf ("\t   (--code 옵션 미지정 시 -jap을 적용함)\n");
	printf ("\t   (대상 데이터 파일은 반드시 처음에 지정)\n");
	printf ("\t   (폴더명은 반드시 마지막에 지정)\n");
	printf ("\n\t-p [--onto ['~.exe']] [--code [codepage]] [folder]\n");
	printf ("\t   지정한 폴더에 있는 모든 파일을 병합하여 'output.dat' 파일로 만듦\n");
	printf ("\t   (--onto 옵션 지정 시 exe 파일명 인자가 필요하며,)\n");
	printf ("\t   (dat 파일을 만들지 않고 해당 exe 파일에 병합함)\n");
	printf ("\t   (옵션 미지정 시에는 '~.exe' 파일과 '~.dat/~.001/... 등으로 나누어 병합')\n");
	printf ("\t   (--code 옵션 지정 시 -jap/-kor 옵션이 필요하며)\n");
	printf ("\t   (--code 옵션 미지정 시 -kor를 적용함)\n");
	printf ("\t   (폴더명은 반드시 마지막에 지정)\n");
	printf ("\n\t-cvt [folder]\n");
	printf ("\t   지정한 폴더에 있는 모든 파일의 코드페이지를 [일본어 -> 한국어] 코드페이지로 변경\n");
	printf ("\n\t-cvte ['~.exe']\n");
	printf ("\t   해당 exe 파일에서 호출되는 문자열의 코드페이지를 [일본어 -> 한국어] 코드페이지로 변경\n");
	printf ("\n\t-extt [--code [codepage]] [folder]\n");
	printf ("\t   지정한 폴더에 있는 모든 lsb 파일을 추출하는 함수\n");
	printf ("\t   원래 이름이 '~.lsb'였다면 '~.ext.txt'로 출력된다\n");
	printf ("\t   언어코드 옵션을 입력하지 않으면 일본어로 지정된다\n");
	printf ("\n\t-rept [--code [codepage]] [folder]\n");
	printf ("\t   지정한 폴더에 있는 모든 텍스트 파일을 병합해 원래 파일과 대치시키는 함수\n");
	printf ("\t   '~.ext.txt'인 파일만 병합 대상이 된다\n");
	printf ("\t   언어코드 옵션을 입력하지 않으면 한국어로 지정된다\n");
	printf ("\n\t-decm [--code [codepage]] [folder]\n");
	printf ("\t   지정한 폴더에 있는 모든 스크립트 파일을 디컴파일하는 함수\n");
	printf ("\n\t-h : 사용법 출력 함수\n");
	printf ("\n예시\n");
	printf ("\n\tToolForLiveMaker -x Game.exe --code -jap Output\n");
	printf ("\tToolForLiveMaker -s Output\n");
	printf ("\tToolForLiveMaker -p --onto Game_pure.exe --code -kor Output\n");
}
//*** 사용법 출력 함수