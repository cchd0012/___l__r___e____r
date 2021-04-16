#include "LM_Pack.h"
#include "LM_Functions.h"
#include "zlib-1.2.11/zlib.h"

#pragma warning (disable:6031)
#pragma warning (disable:4996)


//*** 병합 함수의 기능을 서술하는 파일


void LM_Pack (char *DataFolder, char *ExeFile, int Codepage)
{
	printf ("Pack test:\ntest:DataFolder:%s, ExeFile:%s, Codepage:%d\n"
		, DataFolder, ExeFile, Codepage);
	//*** dat 파일로 만들게 되면 (ExeFile이 null이면) 'output.dat으로 만든다'

	bool isIncludedExeFile = false;
	if (ExeFile != NULL) { isIncludedExeFile = true; }
	//*ExeFile이 NULL이면 ~.ext 파일과 ~.dat 파일로 나눠 병합된다
	//*만일 ~.dat 파일이 1GB가 넘어간다 싶으면 쪼개서 묶어야 한다
	//*이때 출력 파일명은 'game.ext'와 'game.dat/.001/.002...' 이런 식으로 정해진다

	HANDLE hArchive;						//*새로 기록할 아카이브 파일 핸들
	UINT32 pure_exe_size = 0;				//*아카이브의 디폴트 오프셋
	unsigned char* arc_buff = NULL;			//*아카이브를 임시로 저장할 버퍼
	char new_archive_file_name[MAX_PATH];	//*새로운 아카이브의 파일명
	memset(new_archive_file_name, 0, MAX_PATH);
	if (isIncludedExeFile) {
		hArchive = CreateFileA(ExeFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hArchive == INVALID_HANDLE_VALUE) {
			printf("%s: There's not File\n", ExeFile); return;
		}
		pure_exe_size = GetFileSize(hArchive, NULL);
		arc_buff = (unsigned char*)malloc(pure_exe_size);
		ReadFile(hArchive, arc_buff, pure_exe_size, NULL, NULL);
		CloseHandle(hArchive);
		//*미리 읽어들여 두기
		char* tp_pnt = ExeFile;
		for (unsigned int i = 0; i < (strlen(ExeFile) + strlen("_new")); i++) {
			new_archive_file_name[i] = *(tp_pnt++);
			if ((*tp_pnt == '.')) {
				sprintf(&new_archive_file_name[i + 1], "_new");
				i += strlen("_new");
			}
		}
		//*새로운 exe 파일명 지정
		hArchive = CreateFileA(new_archive_file_name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		//*새 exe 파일 생성하기
	}
	//*exe 파일이 있을 경우 exe 파일을 읽어들인다
	else {
		sprintf(new_archive_file_name, "game.ext");
		hArchive = CreateFileA(new_archive_file_name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		//*game.ext 파일 핸들 생성하기
	}
	//*exe 파일을 지정하지 않았으면 고정적으로 'game.ext' 파일로 만든다

	wchar_t* tp_dir = GetUniStr (DataFolder, Codepage);
	//* 유니코드 기본 파일명

	unsigned int file_count = Get_Files_Pass_List(tp_dir, NULL);
	FILE_DESC* file_desc_list = (FILE_DESC*)malloc(sizeof(FILE_DESC) * file_count);
	FILE_DESC* file_desc_list_pnt = file_desc_list;
	memset(file_desc_list, 0, (sizeof(FILE_DESC) * file_count));
	Get_Files_Pass_List(tp_dir, &file_desc_list_pnt);
	printf("Total File Count:%d\n\n", file_count);
	//*해당 폴더 내 파일 수 파악 / 필요한 파일 정보(파일경로) 읽어들이기

	printf("File name header making...");
	for (unsigned int i = 0; i < file_count;i++) {
		file_desc_list[i].FileName = GetAnsiStr((file_desc_list[i].FileName_uni + wcslen(tp_dir) + 1), Codepage);
	}
	free(tp_dir);
	//*코드페이지에 따라 진짜 파일경로를 만들되 앞의 "Datafolder\\"를 제거한 값으로 추가해야 한다

	unsigned int file_name_header_size = 0;
	for (unsigned int i = 0; i < file_count;i++) {
		file_name_header_size += sizeof(int);
		file_name_header_size += strlen(file_desc_list[i].FileName);
	}
	unsigned char* file_name_header_buffer = (unsigned char*)malloc(file_name_header_size);
	unsigned char* tp_a_pnt = file_name_header_buffer;
	unsigned int i_key = 0x0;
	for (unsigned int i = 0; i < file_count; i++) {
		unsigned int File_Path_Len = strlen(file_desc_list[i].FileName);
		*(unsigned int*)tp_a_pnt = File_Path_Len; tp_a_pnt += sizeof(int);
		memcpy(tp_a_pnt, file_desc_list[i].FileName, File_Path_Len);
		for (unsigned int j = 0; j < File_Path_Len; j++) {
			i_key += (i_key << 2);
			i_key += 0x75D6EE39;
			(*tp_a_pnt) = (i_key ^ (*tp_a_pnt)) & 0xFF;
			tp_a_pnt++;
		}
	}
	printf("Finished!\n");
	//*파일 이름 헤더 버퍼 크기 측정 / 만들기

	unsigned int file_offset_header_size = sizeof(int) * 2 * (file_count + 1);
	unsigned int* file_offset_header_buffer = (unsigned int*)malloc (file_offset_header_size);
	memset(file_offset_header_buffer, 0, file_offset_header_size);
	//*파일 오프셋 헤더 만들기
	//*오프셋 값, 체커까지 포함해서 정수칸 2개씩 할당한다
	//*이 값들은 천천히 읽어들이면서 세팅해야 한다

	printf("File attribute header making...");
	unsigned char* file_attr_header_buffer = (unsigned char*)malloc(file_count);
	memset(file_attr_header_buffer, 0, file_count);
	for (unsigned int i = 0; i < file_count; i++) {
		if ((strncmp((file_desc_list[i].FileName + strlen(file_desc_list[i].FileName) - 4), ".ogg", 4) != 0)
			&& (strncmp((file_desc_list[i].FileName + strlen(file_desc_list[i].FileName) - 4), ".jpg", 4) != 0)
			&& (strncmp((file_desc_list[i].FileName + strlen(file_desc_list[i].FileName) - 4), ".gif", 4) != 0)
			&& (strncmp((file_desc_list[i].FileName + strlen(file_desc_list[i].FileName) - 4), ".png", 4) != 0)
			&& (strncmp((file_desc_list[i].FileName + strlen(file_desc_list[i].FileName) - 4), ".gal", 4) != 0)) {
			file_attr_header_buffer[i] = 0x3;
			//*확장자 중에 ogg, jpg, gif, png, gal 빼고 다 압축함
		}
		else {
			file_attr_header_buffer[i] = 0x2;
		}
	}
	printf("Finished!\n");
	//*파일 속성 헤더 만들기 (스크램블 속성 포함)
	//*0x0 : 압축된 데이터, 0x1 : 그냥 데이터
	//*0x2 : 스크램블된 데이터, 0x3 : 압축 후 스크램블된 데이터

	printf("File header writing...");
	unsigned short value_2byte;
	unsigned int value_4byte;
	if (isIncludedExeFile) {
		WriteFile(hArchive, arc_buff, pure_exe_size, NULL, NULL);
	}
	//*exe 파일일 경우에는 exe 데이터를 먼저 기록한다
	value_2byte = VF_MAGIC;
	WriteFile(hArchive, &value_2byte, sizeof(short), NULL, NULL);
	value_4byte = VF_VERSION;
	WriteFile(hArchive, &value_4byte, sizeof(int), NULL, NULL);
	value_4byte = file_count;
	WriteFile(hArchive, &value_4byte, sizeof(int), NULL, NULL);
	//*vf 헤더들 및 파일 수 기록

	WriteFile(hArchive, file_name_header_buffer, file_name_header_size, NULL, NULL);
	//*파일명 헤더 기록

	unsigned int offset_buffer_pos = SetFilePointer(hArchive, 0, NULL, FILE_CURRENT);
	SetFilePointer(hArchive, file_offset_header_size, NULL, FILE_CURRENT);
	unsigned int attr_buffer_pos = SetFilePointer(hArchive, 0, NULL, FILE_CURRENT);
	SetFilePointer(hArchive, file_count, NULL, FILE_CURRENT);
	printf("Finished!\n\n");
	//*파일 오프셋은 압축하는 등 바로 기록할 수 없으니 나중에 덩어리를 기록하면서 같이 기록한다
	//*파일 속성은 압축 혹은 스크램블이 불가할 경우 수정해야 할 수 있다

	if (!isIncludedExeFile) {
		CloseHandle(hArchive);
		hArchive = CreateFileA("game.dat", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	//*쪼개서 기록할 때는 이제 핸들을 바꿔준다

	i_key = 0x0;
	unsigned int checker;
	unsigned int accum_offset = SetFilePointer(hArchive, 0, NULL, FILE_CURRENT) - pure_exe_size;
	unsigned int* offset_buffer_pnt = file_offset_header_buffer;
	HANDLE hEachFile;
	unsigned int each_file_size;
	unsigned char *each_file_buffer;
	char _game_part_name[0x9] = "game.dat";
	unsigned int _accum_part_idx = 0x001;
	for (unsigned int i = 0; i < file_count; i++) {

		printf("%s\n", file_desc_list[i].FileName);
		printf("[*]");

		i_key += (i_key << 2);
		i_key += 0x75D6EE39;
		checker = (i_key < 0x80000000) ? 0x00000000 : 0xFFFFFFFF;
		(*offset_buffer_pnt++) = accum_offset ^ i_key;
		(*offset_buffer_pnt++) = checker;
		//*오프셋 버퍼 갱신
		hEachFile = CreateFileW(file_desc_list[i].FileName_uni, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		each_file_size = GetFileSize(hEachFile, NULL);
		each_file_buffer = (unsigned char*)malloc(each_file_size);
		ReadFile(hEachFile, each_file_buffer, each_file_size, NULL, NULL);
		CloseHandle(hEachFile);
		//*각 파일 열고 읽어들이기
		if ((file_attr_header_buffer[i] == 0x0) || (file_attr_header_buffer[i] == 0x3)) {
			if (each_file_size == 0x00) { 
				file_attr_header_buffer[i] = 0x1; 
			}
			else {
				printf("Compressing...");
				unsigned int comp_size = each_file_size * 0x3;
				unsigned char* comp_buff = (unsigned char*)malloc (comp_size);
				memset(comp_buff, 0, comp_size);
				compress(comp_buff, (unsigned long*)&comp_size, each_file_buffer, each_file_size);
				free(each_file_buffer);
				each_file_buffer = comp_buff;
				each_file_size = comp_size;
			}
		}
		//*경우에 따라 압축하기, 아예 안 되면 그냥 넘긴다
		if ((file_attr_header_buffer[i] == 0x2) || (file_attr_header_buffer[i] == 0x3)) {
			printf("Scrambling...");
			if (!Scramble_Data(&each_file_buffer, &each_file_size)) {
				if (file_attr_header_buffer[i] == 0x2) { file_attr_header_buffer[i] = 0x1; }
				if (file_attr_header_buffer[i] == 0x3) { file_attr_header_buffer[i] = 0x0; }
			}
		}
		//*경우에 따라 스크램블하기, 실패하면 그냥 파일 혹은 그냥 압축파일로 둔다

		if (!isIncludedExeFile) {
			unsigned int _pos = SetFilePointer(hArchive, 0, NULL, FILE_CURRENT);
			if ((_pos + each_file_size) > 0x40000000) {
				sprintf(_game_part_name, "game.%03d", _accum_part_idx);
				unsigned int _left = (_pos + each_file_size) - 0x40000000;
				unsigned int _write = each_file_size - _left;
				WriteFile(hArchive, each_file_buffer, _write, NULL, NULL);
				CloseHandle(hArchive);
				hArchive = CreateFileA(_game_part_name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				WriteFile(hArchive, each_file_buffer + _write, _left, NULL, NULL);
				_accum_part_idx++;
			}
			else {
				WriteFile(hArchive, each_file_buffer, each_file_size, NULL, NULL);
			}
		}
		//*쪼개서 기록할 때, 현재 핸들 크기가 1GB를 넘어갈 듯 하면 바꿔 기록한다

		else {
			WriteFile(hArchive, each_file_buffer, each_file_size, NULL, NULL);
		}
		//*그렇지 않으면 평범하게 기록하면 됨

		accum_offset += each_file_size;
		free(each_file_buffer);
		printf("File Data Write Complete!\n\n");
		//*파일 기록 / 오프셋 갱신 / 정리
	}
	printf("All File Data Writing Finished!\n\n");
	//*파일 덩어리 데이터는 모든 파일을 순차적으로 기록한다
	//*압축할 수 있으면 압축 후에 스크램블 실시

	i_key += (i_key << 2);
	i_key += 0x75D6EE39;
	checker = (i_key < 0x80000000) ? 0x00000000 : 0xFFFFFFFF;
	(*offset_buffer_pnt++) = accum_offset ^ i_key;
	(*offset_buffer_pnt++) = checker;
	//*마지막 값도 기록함

	printf("File offset / attribute header writing...");
	if (isIncludedExeFile) {
		value_4byte = pure_exe_size;
		WriteFile(hArchive, &value_4byte, sizeof(int), NULL, NULL);
		value_2byte = LV_MAGIC;
		WriteFile(hArchive, &value_2byte, sizeof(short), NULL, NULL);
	}
	//*exe 파일일 경우만 마지막 위치 및 헤더 기록

	if (!isIncludedExeFile) {
		CloseHandle(hArchive);
		hArchive = CreateFileA("game.ext", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	//*exe 파일이 아니라면 다시 game.ext 파일로 돌려놓는다

	SetFilePointer(hArchive, attr_buffer_pos, NULL, FILE_BEGIN);
	WriteFile(hArchive, file_attr_header_buffer, file_count, NULL, NULL);
	SetFilePointer(hArchive, offset_buffer_pos, NULL, FILE_BEGIN);
	WriteFile(hArchive, file_offset_header_buffer, file_offset_header_size, NULL, NULL);
	//*마지막으로 속성과 오프셋 데이터를 기록한다
	printf("Finished!\n\n");

	CloseHandle(hArchive);
	if (isIncludedExeFile) { free(arc_buff); }
	for (unsigned int i = 0; i < file_count;i++) { free(file_desc_list[i].FileName_uni); }
	free(file_desc_list);
	free(file_name_header_buffer);
	free(file_offset_header_buffer);
	free(file_attr_header_buffer);
	//*정리
}
//*** 데이터 병합 함수