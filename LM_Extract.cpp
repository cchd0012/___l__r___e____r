#include "LM_Extract.h"
#include "LM_Functions.h"
#include "zlib-1.2.11/zlib.h"

#pragma warning (disable:6031)
#pragma warning (disable:4996)


//*** 분해 함수의 기능을 서술하는 파일


void LM_Extract_For_Ext_Dat(char* DataFile, char* OutputFolder, int Codepage);
//*.ext, *.dat용 데이터 분해 함수


void LM_Extract (char *DataFile, char *OutputFolder, int Codepage)
{
	if ((stristr(DataFile, ".ext") != NULL) || (stristr(DataFile, ".dat") != NULL)) {
		LM_Extract_For_Ext_Dat(DataFile, OutputFolder, Codepage);
		return;
	}
	//*확장자가 .ext 혹은 .dat인 파일이 들어오면 밑의 함수로 보냄

	printf ("Extract:\nDataFile:%s, OutputFolder:%s, CodePage:%d\n"
		, DataFile, OutputFolder, Codepage);

	HANDLE hData = CreateFileA (DataFile, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hData == INVALID_HANDLE_VALUE) {
		printf ("%s: There's not File\n", DataFile); return;
	}
	//*핸들 열기

	unsigned int val, Arc_Start_Pos;
	bool IsExeFile = false;
	ReadFile (hData, &val, sizeof(unsigned int), NULL, NULL);
	if (val == MZ_MAGIC) { IsExeFile = true; }
	//*exe 혹은 dat 파일인지 구분하기

	HANDLE hWrite;
	Arc_Start_Pos = 0;
	//*상태 통일을 위한 세팅
	
	if (!IsExeFile) {
		printf("This function is for .exe package\n");
		CloseHandle(hData);
		return;
	}
	//*여기서는 exe만 다루도록 한다

	SetFilePointer (hData, -2, NULL, FILE_END);
	val = 0;
	ReadFile (hData, &val, sizeof(unsigned short), NULL, NULL);
	if (val != LV_MAGIC) { 
		printf ("%s: This isn't valid VF exe file\n", DataFile); 
		CloseHandle (hData); return;
	}
	//*맨 끝의 문자열이 "lv"인지 확인
		
	SetFilePointer (hData, -6, NULL, FILE_END);
	val = 0;
	ReadFile (hData, &val, sizeof(unsigned int), NULL, NULL);
	//*데이터 덩어리가 시작되는 위치 파악 (=exe 파일의 한계점)

	char ExeFile[MAX_PATH], *tp_pnt = DataFile;
	unsigned char *ExeBuff;
	memset (ExeFile, 0, MAX_PATH);
	for (tp_pnt = DataFile + (strlen(DataFile) - 1); tp_pnt >= DataFile; tp_pnt--) {
		if ((*tp_pnt == '.')) { break; }
	}
	memcpy(ExeFile, DataFile, sizeof(char) * (tp_pnt - DataFile));
	sprintf(ExeFile + strlen(ExeFile), "_pure.exe");
	//*exe 파일명 지정

	ExeBuff = (unsigned char*)malloc(sizeof(char)*val);
	SetFilePointer (hData, 0, NULL, FILE_BEGIN);
	ReadFile (hData, ExeBuff, val, NULL, NULL);
	Arc_Start_Pos = val;
	hWrite = CreateFileA (ExeFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile (hWrite, ExeBuff, val, NULL, NULL);
	CloseHandle (hWrite); hWrite = INVALID_HANDLE_VALUE;
	free (ExeBuff);
	//*exe 파일 부분만 따로 추출하여 (~_pure.exe) 기록 후 전부 파기
	//*이 상황이면 hData는 데이터 아카이브 시작점에 위치하게 된다
	
	SetFilePointer (hData, Arc_Start_Pos, NULL, FILE_BEGIN);
	val = 0;
	ReadFile (hData, &val, sizeof(unsigned short), NULL, NULL);
	if (val != VF_MAGIC) { 
		printf ("%s: This isn't valid VF archive file\n", DataFile); 
		CloseHandle (hData); return;
	}
	//*아카이브 시작 문자열이 "vf"인지 확인

	SetFilePointer (hData, sizeof(int), NULL, FILE_CURRENT);
	//*맨 앞에 있는 0x66(int)은 그냥 시그니처인듯?

	unsigned int FileCount;
	ReadFile (hData, &FileCount, sizeof(unsigned int), NULL, NULL);
	printf ("Total File Count:%d\n\n", FileCount);
	//*파일 갯수 파악

	vector<FILE_DESC> fd_list (FileCount + 1);
	unsigned int i_key = 0x0;
	memset (&fd_list[FileCount], 0, sizeof(FILE_DESC));
	for (unsigned int i = 0;i < FileCount;i++) {
		unsigned int File_Path_Len;
		char *tmp_n_buff = (char*)malloc (MAX_PATH);
		memset (tmp_n_buff, 0, MAX_PATH);
		memset (&fd_list[i], 0, sizeof(FILE_DESC));
		ReadFile (hData, &File_Path_Len, sizeof(unsigned int), NULL, NULL);
		ReadFile (hData, tmp_n_buff, File_Path_Len, NULL, NULL);
		//*파일경로 데이터 읽어들이기
		for (unsigned int j = 0;j < File_Path_Len;j++) {
			i_key += (i_key << 2);
			i_key += 0x75D6EE39;
			tmp_n_buff[j] = (i_key ^ tmp_n_buff[j]) & 0xFF;
		}
		fd_list[i].FileName = (char*)tmp_n_buff;
		//*파일경로 데이터 복호화 후 디스크립션에 추가하기
	}
	//*각 파일별로 파일 헤더 세팅 (파일명/파일버퍼/파일버퍼크기/압축여부/스크램블 여부)
	//*먼저 파일 이름부터 세팅한다 (일단 예시 게임파일 중 파일이름부분 크기는 0x217F7)
	
	i_key = 0x0;
	for (unsigned int j = 0;j < (FileCount + 1);j++) {
		i_key += (i_key << 2);
		i_key += 0x75D6EE39;
		ReadFile (hData, &fd_list[j].OffsetInVF, sizeof(unsigned int), NULL, NULL);
		ReadFile (hData, &val, sizeof(unsigned int), NULL, NULL);
		unsigned int checker = (i_key < 0x80000000) ? 0x00000000 : 0xFFFFFFFF;
		if (val != checker) {
			printf ("%s: Error: File Header Cruched\n", DataFile); 
			fd_list.clear(); CloseHandle (hData);
			return;
			//*여기 값은 이 단계에서 i_key 키값이 0x80000000보다 크거나 같으면 -1, 작으면 0이다
			//*그런고로 이 규칙을 지키지 않으면 에러처리
		}
		fd_list[j].OffsetInVF ^= i_key;
		fd_list[j].OffsetInVF += Arc_Start_Pos;
		if (j >= 1) { fd_list[j-1].BufferSize = fd_list[j].OffsetInVF - fd_list[j-1].OffsetInVF; }
	}
	//*파일 오프셋 세팅. 이 부분 크기는 파일수보다 하나 더 많다

	for (unsigned int j = 0;j < FileCount;j++) {
		unsigned char DataType;
		ReadFile (hData, &DataType, sizeof(unsigned char), NULL, NULL);
		fd_list[j].IsScrambled = ((DataType & 0x02) != 0);
		fd_list[j].IsCompressed = 
			((DataType & 0x01) != 0) ? fd_list[j].IsScrambled : !fd_list[j].IsScrambled;
		//*0x0 : 압축된 데이터, 0x1 : 그냥 데이터
		//*0x2 : 스크램블된 데이터, 0x3 : 압축 후 스크램블된 데이터
	}
	fd_list[FileCount].IsCompressed = true; fd_list[FileCount].IsScrambled = false;
	//*파일 읽는 방식 세팅

	unsigned int tmp_size;
	unsigned char* tmp_buff;
	unsigned long org_size;
	for (unsigned int j = 0;j < FileCount;j++) {
		fd_list[j].FileBuffer = (unsigned char*)malloc (fd_list[j].BufferSize);
		memset (fd_list[j].FileBuffer, 0, fd_list[j].BufferSize);
		SetFilePointer (hData, fd_list[j].OffsetInVF, NULL, SEEK_SET);
		ReadFile (hData, fd_list[j].FileBuffer, fd_list[j].BufferSize, NULL, NULL);

		printf("%s\n", fd_list[j].FileName);
		wchar_t *uni_path;
		string complete_path = OutputFolder; 
		complete_path += "\\";
		complete_path += fd_list[j].FileName;
		uni_path = GetUniStr((char*)complete_path.c_str(), Codepage);
		//*경로 확정

		printf("[*]");

		if (fd_list[j].IsScrambled) { 
			printf("Unscrambling...");
			Unscramble_Data (&fd_list[j].FileBuffer, &fd_list[j].BufferSize);
		}
		if (fd_list[j].IsCompressed) {
			printf("Decompressing...");
			tmp_size = fd_list[j].BufferSize * 30;
			tmp_buff = (unsigned char*)malloc(tmp_size);
			memset(tmp_buff, 0, tmp_size);
			//*30배 되는 크기로 미리 할당 후 초기화
			org_size = tmp_size;
			uncompress(tmp_buff, &org_size, fd_list[j].FileBuffer, fd_list[j].BufferSize);
			fd_list[j].BufferSize = org_size;
			free(fd_list[j].FileBuffer); fd_list[j].FileBuffer = tmp_buff;
			//*압축풀어서 원래 크기 확정 후 다시 세팅
		}
		//*스크램블/압축풀기

		hWrite = LmCreateFile (uni_path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, 
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile (hWrite, fd_list[j].FileBuffer, fd_list[j].BufferSize, NULL, NULL);
		CloseHandle (hWrite); free (fd_list[j].FileBuffer); fd_list[j].FileBuffer = NULL;
		free (uni_path);
		free(fd_list[j].FileName);
		printf("File Write Complete!\n\n");
		//*기록할 경로에 기록 후 메모리 정리
	}
	//*스크램블, 압축풀기 순서대로 수행

	fd_list.clear();
	CloseHandle (hData);
	//*작업 마치고 깨끗이 지운 후 핸들 닫기
}
//*** 데이터 분해 함수


void LM_Extract_For_Ext_Dat(char* DataFile, char* OutputFolder, int Codepage)
{
	char *ExtFile, *DatFile;

	bool isExtFileStr = false;
	bool isDatFileStr = false;

	if (stristr(DataFile, ".ext") != NULL) { isExtFileStr = true; }
	else if (stristr(DataFile, ".dat") != NULL) { isDatFileStr = true; }
	else {
		printf("%s:Can't Extract File\n", DataFile);
		return;
	}

	ExtFile = (char*)malloc(strlen(DataFile) + 1); memset(ExtFile, 0, (strlen(DataFile) + 1));
	DatFile = (char*)malloc(strlen(DataFile) + 1); memset(DatFile, 0, (strlen(DataFile) + 1));

	if (isExtFileStr) { 
		memcpy(ExtFile, DataFile, strlen(DataFile));
		memcpy(DatFile, DataFile, strlen(DataFile) - 3);
		sprintf(DatFile + strlen(DatFile), "dat");
	}
	else if (isDatFileStr) {
		memcpy(DatFile, DataFile, strlen(DataFile));
		memcpy(ExtFile, DataFile, strlen(DataFile) - 3);
		sprintf(ExtFile + strlen(ExtFile), "ext");
	}
	//*ext 파일 문자열과 dat 파일 문자열 만들기
	printf("Extract:\nDataFile:%s / %s, OutputFolder:%s, CodePage:%d\n"
		, ExtFile, DatFile, OutputFolder, Codepage);

	unsigned int _ext_size = 0, _dat_size = 0;
	HANDLE hExt, hDat;
	unsigned int val, Arc_Start_Pos;
	hExt = CreateFileA(ExtFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hExt == INVALID_HANDLE_VALUE) {
		printf("%s: There's not File\n", ExtFile); return;
	}
	hDat = CreateFileA(DatFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDat == INVALID_HANDLE_VALUE) {
		CloseHandle(hExt);
		printf("%s: There's not File\n", DatFile); return;
	}
	//*파일 분해 준비

	Arc_Start_Pos = 0;
	SetFilePointer(hExt, Arc_Start_Pos, NULL, FILE_BEGIN);
	val = 0;
	ReadFile(hExt, &val, sizeof(unsigned short), NULL, NULL);
	if (val != VF_MAGIC) {
		printf("%s: This isn't valid VF extention list file\n", ExtFile);
		CloseHandle(hExt); CloseHandle(hDat); return;
	}
	//*아카이브 시작 문자열이 "vf"인지 확인 (ext 파일로)

	SetFilePointer(hExt, sizeof(int), NULL, FILE_CURRENT);
	//*맨 앞에 있는 0x66(int)은 그냥 시그니처인듯?

	unsigned int FileCount;
	ReadFile(hExt, &FileCount, sizeof(unsigned int), NULL, NULL);
	printf("Total File Count:%d\n\n", FileCount);
	//*파일 갯수 파악 (ext 파일로)

	vector<FILE_DESC> fd_list(FileCount + 1);
	unsigned int i_key = 0x0;
	memset(&fd_list[FileCount], 0, sizeof(FILE_DESC));
	for (unsigned int i = 0; i < FileCount; i++) {
		unsigned int File_Path_Len;
		char* tmp_n_buff = (char*)malloc(MAX_PATH);
		memset(tmp_n_buff, 0, MAX_PATH);
		memset(&fd_list[i], 0, sizeof(FILE_DESC));
		ReadFile(hExt, &File_Path_Len, sizeof(unsigned int), NULL, NULL);
		ReadFile(hExt, tmp_n_buff, File_Path_Len, NULL, NULL);
		//*파일경로 데이터 읽어들이기
		for (unsigned int j = 0; j < File_Path_Len; j++) {
			i_key += (i_key << 2);
			i_key += 0x75D6EE39;
			tmp_n_buff[j] = (i_key ^ tmp_n_buff[j]) & 0xFF;
		}
		fd_list[i].FileName = (char*)tmp_n_buff;
		//*파일경로 데이터 복호화 후 디스크립션에 추가하기
	}
	//*각 파일별로 파일 헤더 세팅 (파일명/파일버퍼/파일버퍼크기/압축여부/스크램블 여부)
	//*먼저 파일 이름부터 세팅한다 (일단 예시 게임파일 중 파일이름부분 크기는 0x217F7)
	//* (ext 파일로)
	
	i_key = 0x0;
	for (unsigned int j = 0; j < (FileCount + 1); j++) {
		i_key += (i_key << 2);
		i_key += 0x75D6EE39;
		ReadFile(hExt, &fd_list[j].OffsetInVF, sizeof(unsigned int), NULL, NULL);
		ReadFile(hExt, &val, sizeof(unsigned int), NULL, NULL);
		unsigned int checker = (i_key < 0x80000000) ? 0x00000000 : 0xFFFFFFFF;
		if (val != checker) {
			printf("%s: Error: File Header Cruched\n", ExtFile);
			fd_list.clear(); 
			CloseHandle(hExt); CloseHandle(hDat); return;
			//*여기 값은 이 단계에서 i_key 키값이 0x80000000보다 크거나 같으면 -1, 작으면 0이다
			//*그런고로 이 규칙을 지키지 않으면 에러처리
		}
		fd_list[j].OffsetInVF ^= i_key;
		fd_list[j].OffsetInVF += Arc_Start_Pos;
		if (j >= 1) { fd_list[j - 1].BufferSize = fd_list[j].OffsetInVF - fd_list[j - 1].OffsetInVF; }
	}
	//*파일 오프셋 세팅. 이 부분 크기는 파일수보다 하나 더 많다 (ext 파일로)

	for (unsigned int j = 0; j < FileCount; j++) {
		unsigned char DataType;
		ReadFile(hExt, &DataType, sizeof(unsigned char), NULL, NULL);
		fd_list[j].IsScrambled = ((DataType & 0x02) != 0);
		fd_list[j].IsCompressed =
			((DataType & 0x01) != 0) ? fd_list[j].IsScrambled : !fd_list[j].IsScrambled;
		//*0x0 : 압축된 데이터, 0x1 : 그냥 데이터
		//*0x2 : 스크램블된 데이터, 0x3 : 압축 후 스크램블된 데이터
	}
	fd_list[FileCount].IsCompressed = true; fd_list[FileCount].IsScrambled = false;
	CloseHandle(hExt);
	//*파일 읽는 방식 세팅 (ext 파일로)
	//*여기까지가 ext 파일의 역할

	unsigned int tmp_size;
	unsigned char* tmp_buff;
	unsigned long org_size;
	for (unsigned int j = 0; j < FileCount; j++) {

		fd_list[j].FileBuffer = (unsigned char*)malloc(fd_list[j].BufferSize);
		memset(fd_list[j].FileBuffer, 0, fd_list[j].BufferSize);
		SetFilePointer(hDat, fd_list[j].OffsetInVF, NULL, SEEK_SET);
		ReadFile(hDat, fd_list[j].FileBuffer, fd_list[j].BufferSize, NULL, NULL);

		printf("%s\n", fd_list[j].FileName);
		wchar_t* uni_path;
		string complete_path = OutputFolder;
		complete_path += "\\";
		complete_path += fd_list[j].FileName;
		uni_path = GetUniStr((char*)complete_path.c_str(), Codepage);
		//*경로 확정

		printf("[*]");

		if (fd_list[j].IsScrambled) {
			printf("Unscrambling...");
			Unscramble_Data(&fd_list[j].FileBuffer, &fd_list[j].BufferSize);
		}
		if (fd_list[j].IsCompressed) {
			printf("Decompressing...");
			tmp_size = fd_list[j].BufferSize * 30;
			tmp_buff = (unsigned char*)malloc(tmp_size);
			memset(tmp_buff, 0, tmp_size);
			//*30배 되는 크기로 미리 할당 후 초기화
			org_size = tmp_size;
			uncompress(tmp_buff, &org_size, fd_list[j].FileBuffer, fd_list[j].BufferSize);
			fd_list[j].BufferSize = org_size;
			free(fd_list[j].FileBuffer); fd_list[j].FileBuffer = tmp_buff;
			//*압축풀어서 원래 크기 확정 후 다시 세팅
		}
		//*스크램블/압축풀기

		HANDLE hWrite = LmCreateFile(uni_path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(hWrite, fd_list[j].FileBuffer, fd_list[j].BufferSize, NULL, NULL);
		CloseHandle(hWrite); free(fd_list[j].FileBuffer); fd_list[j].FileBuffer = NULL;
		free(uni_path);
		free(fd_list[j].FileName);
		printf("File Write Complete!\n\n");
		//*기록할 경로에 기록 후 메모리 정리
	}
	//*스크램블, 압축풀기 순서대로 수행(dat 파일로)

	free(ExtFile); free(DatFile);
	fd_list.clear();
	CloseHandle(hDat);
	//*작업 마치고 깨끗이 지운 후 핸들 닫기
}
//*.ext, *.dat용 데이터 분해 함수