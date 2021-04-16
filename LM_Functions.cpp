#include "LM_Functions.h"
#include <random>


#pragma warning (disable:6031)
#pragma warning (disable:4996)

using namespace std;
//*** 호출 함수들의 기반이 되는 함수를 서술하는 파일


wchar_t *_txt_buf = NULL;
//*** 텍스트 버퍼

void GetRandomValue(unsigned int seed, unsigned int* RandomKeyArray);
//*랜덤 키 획득 함수

void Mul_64Bit(unsigned int a, unsigned int b,
	unsigned int* res, unsigned int* over, unsigned int* carry);
void Add_64Bit(unsigned int a, unsigned int b,
	unsigned int* res, unsigned int* carry);
void Add_With_Carry_64Bit(unsigned int a, unsigned int b, unsigned int carry,
	unsigned int* res, unsigned int* res_carry);
void Permutation_Round(unsigned int* KeyArray);
//*랜덤 키 획득 보조 함수


void Initialize_Tool()
{
	Load_Hanja();
	_txt_buf = (wchar_t*)malloc(sizeof(wchar_t) * TXT_BUFF_LEN);
	memset(_txt_buf, 0, (sizeof(wchar_t) * TXT_BUFF_LEN));
	//*한자파일 로드 및 텍스트 출력용 버퍼 공간 할당
}
//*툴 초기화 함수

void Terminate_Tool()
{
	free(_txt_buf);
	Release_Hanja();
	//*한자파일 언로드 및 텍스트 출력용 버퍼 해제
}
//*툴 마치는 함수



void CreateDir (wchar_t* Path)
{
    wchar_t DirName[MAX_PATH];	//생성할 디렉토리 이름
    wchar_t* p = Path;			//인자로 받은 디렉토리
    wchar_t* q = DirName;  
 
    while(*p)
    {
        if (('\\' == *p) || ('/' == *p))   //루트디렉토리 혹은 Sub디렉토리
        {
            if (':' != *(p-1))
            {
                CreateDirectoryW(DirName, NULL);
            }
        }
        *q++ = *p++;
        *q = '\0';
    }
}
//*경로에 있는 모든 디렉토리를 생성하는 함수

HANDLE LmCreateFile (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) 
{
	CreateDir ((wchar_t*)lpFileName);
	return CreateFileW( lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
		dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}
//CreateFile 개량함수. 모든 디렉토리 한번에 생성




wchar_t* J2U(char* string);
wchar_t* K2U(char* string);
char* U2K(wchar_t* string);
char* U2J(wchar_t* string);

wchar_t* GetUniStr(char *string, unsigned int code)
{
	if (code == LM_CP_JAP) { return J2U(string); }
	else if (code == LM_CP_KOR) { return K2U(string); }
	else { return J2U(string); }
}
//*언어코드에 따라 문자열을 바꿔 유니코드로 반환함

char* GetAnsiStr(wchar_t* string, unsigned int code)
{
	if (code == LM_CP_JAP) { return U2J(string); }
	else if (code == LM_CP_KOR) { return U2K(string); }
	else { return U2J(string); }
}
//*언어코드에 따라 문자열을 바꿔 ansi로 반환함

wchar_t* J2U (char *string)
{
	wchar_t *bstr;
	int nLen = MultiByteToWideChar(932, 0, string, -1, NULL, NULL);
	bstr = (wchar_t*)malloc(sizeof(wchar_t)*(nLen+1));
	bstr[nLen] = 0;
	MultiByteToWideChar(932, 0, string, -1, bstr, nLen);
	return bstr;
}

wchar_t* K2U (char *string)
{
	wchar_t *bstr;
	int nLen = MultiByteToWideChar(949, 0, string, -1, NULL, NULL);
	bstr = (wchar_t*)malloc(sizeof(wchar_t)*(nLen+1));
	bstr[nLen] = 0;
	MultiByteToWideChar(949, 0, string, -1, bstr, nLen);
	return bstr;
}

char* U2K (wchar_t *string)
{
	char *lstr;
	int nLen = WideCharToMultiByte(949, 0, string, -1, NULL, NULL, NULL, NULL);
	lstr = (char*)malloc(sizeof(char)*(nLen+1));
	lstr[nLen] = 0;
	WideCharToMultiByte(949, 0, string, -1, lstr, nLen, NULL, NULL);
	return lstr;
}

char* U2J (wchar_t *string)
{
	char *lstr;
	int nLen = WideCharToMultiByte(932, 0, string, -1, NULL, NULL, NULL, NULL);
	lstr = (char*)malloc(sizeof(char)*(nLen+1));
	lstr[nLen] = 0;
	WideCharToMultiByte(932, 0, string, -1, lstr, nLen, NULL, NULL);
	return lstr;
}


UINT32 Get_Txt_Count(wchar_t* src_txt_buffer, const wchar_t* find_txt, UINT32 src_txt_size)
{
	UINT32 total_count = 0;
	wchar_t* txt_buffer_pnt = src_txt_buffer;
	while (txt_buffer_pnt < (src_txt_buffer + src_txt_size)) {
		wchar_t* check_str = wcsstr(txt_buffer_pnt, find_txt);
		if (check_str != NULL) {
			total_count++;
			txt_buffer_pnt = check_str + wcslen(find_txt);
		}
		else { break; }
	}
	return total_count;
}
//*현재 텍스트 버퍼 내에 찾는 텍스트가 몇 개나 있는지 파악하는 함수




void Unscramble_Data(unsigned char** Buffer, unsigned int* BufferSize)
{
	unsigned int BlockSize = *(unsigned int*)(*Buffer);
	unsigned int Seed = *(unsigned int*)((*Buffer) + sizeof(int));
	Seed ^= 0xF8EA;
	unsigned int BlockCount = ((*BufferSize) - (sizeof(int) * 2)) / BlockSize;
	if ((((*BufferSize) - (sizeof(int) * 2)) % BlockSize) != 0) { BlockCount++; }
	//*블럭 단위크기, 블럭의 총 수와 랜덤시드 파악하기

	unsigned int RandomKeyArray[5];
	GetRandomValue(Seed, RandomKeyArray);
	//*랜덤시드를 기반으로 랜덤값 얻기 (5개 int)

	unsigned __int64 _tmp_64bit;
	vector<unsigned int> Origin_Block_Idx_List(BlockCount);
	for (unsigned int i = 0; i < BlockCount; i++) { Origin_Block_Idx_List[i] = i; }
	vector<unsigned int> New_Block_Idx_List(BlockCount, 0);
	for (int i = BlockCount; i > 0; i--) {
		Permutation_Round(RandomKeyArray);
		_tmp_64bit = (unsigned __int64)RandomKeyArray[0] * (unsigned __int64)(i - 1);
		unsigned int idx = (unsigned int)(_tmp_64bit / 0x100000000);
		New_Block_Idx_List[Origin_Block_Idx_List[idx]] = (BlockCount - i);
		Origin_Block_Idx_List.erase(Origin_Block_Idx_List.begin() + idx);
		Origin_Block_Idx_List.push_back((BlockCount - i));
		//*0부터 BlockCount-1 까지
	}
	//*새 블럭 인덱스로 갱신

	unsigned char* org_buff = (unsigned char*)malloc((*BufferSize));
	memset(org_buff, 0, (*BufferSize));
	for (unsigned int i = 0; i < BlockCount; i++) {
		unsigned int cp_len = (i != (BlockCount - 1)) ? BlockSize : ((*BufferSize) - (BlockSize * i) - (sizeof(int) * 2));
		memcpy(&org_buff[(BlockSize * i)], &(*Buffer)[(BlockSize * New_Block_Idx_List[i]) + (sizeof(int) * 2)], cp_len);
	}
	free(*Buffer);
	(*Buffer) = org_buff;
	(*BufferSize) -= 8;
	//*새 블럭 인덱스대로 차례차례 옮긴 후 버퍼 바꿔치기
}
//*** 스크램블된 데이터를 풀어주는 함수


bool Scramble_Data(unsigned char** Buffer, unsigned int* BufferSize)
{
	unsigned int BlockSize = (*BufferSize) / DEFAULT_BLOCK_COUNT;
	if (BlockSize == 0) { return false; }
	//*블럭 사이즈 계산
	//*만일 블럭 사이즈가 0이 나오면 그냥 안한다

	unsigned int BlockCount = (*BufferSize) / BlockSize;
	if (((*BufferSize) % BlockSize) != 0) { BlockCount++; }
	//*블럭 수 세기

	unsigned int res_buff_size = (*BufferSize) + (sizeof(int) * 2);
	unsigned char* res_buff = (unsigned char*)malloc(res_buff_size);
	memset(res_buff, 0, res_buff_size);
	*(unsigned int*)res_buff = BlockSize;
	//*새로 반환할 크기만큼 할당하고 블럭 크기 세팅

	random_device rd;
	mt19937 mersenne(rd());
	uniform_int_distribution<> die(0x1111, 0xFFFF);
	unsigned short Seed1 = (unsigned int)die(mersenne);
	unsigned short Seed2 = (unsigned int)die(mersenne);
	unsigned int Seed = ((unsigned int)Seed1 << 0x10) | Seed2;
	*(unsigned int*)(res_buff + sizeof(int)) = Seed;
	Seed ^= 0xF8EA;
	//*시드값 세팅 및 수행 준비

	unsigned int RandomKeyArray[5];
	GetRandomValue(Seed, RandomKeyArray);
	//*랜덤시드를 기반으로 랜덤값 얻기 (5개 int)

	unsigned __int64 _tmp_64bit;
	vector<unsigned int> Origin_Block_Idx_List(BlockCount);
	for (unsigned int i = 0; i < BlockCount; i++) { Origin_Block_Idx_List[i] = i; }
	vector<unsigned int> New_Block_Idx_List(BlockCount, 0);
	for (int i = BlockCount; i > 0; i--) {
		Permutation_Round(RandomKeyArray);
		_tmp_64bit = (unsigned __int64)RandomKeyArray[0] * (unsigned __int64)(i - 1);
		unsigned int idx = (unsigned int)(_tmp_64bit / 0x100000000);
		New_Block_Idx_List[Origin_Block_Idx_List[idx]] = (BlockCount - i);
		Origin_Block_Idx_List.erase(Origin_Block_Idx_List.begin() + idx);
		Origin_Block_Idx_List.push_back((BlockCount - i));
		//*0부터 BlockCount-1 까지
	}
	//*새 블럭 인덱스로 갱신

	vector<unsigned int> Reverse_Block_Idx_List(BlockCount, 0);
	for (unsigned int i = 0; i < BlockCount;i++) {
		Reverse_Block_Idx_List[New_Block_Idx_List[i]] = i;
	}
	//*New_Block_Idx_List과 Origin_Block_Idx_List의 관계를 역산하여
	//*새롭게 벡터를 하나 더 만들고 세팅한다

	unsigned int last_idx = Reverse_Block_Idx_List[BlockCount -1];

	for (unsigned int i = 0; i < BlockCount; i++) {
		unsigned int cp_len = (i != (BlockCount - 1)) ? BlockSize : ((*BufferSize) - (BlockSize * i));
		memcpy(&res_buff[(BlockSize * i) + (sizeof(int)*2)], &(*Buffer)[(BlockSize * Reverse_Block_Idx_List[i])], cp_len);
	}
	//*새롭게 만든 대로 잘라내어 세팅한다

	free(*Buffer);
	(*Buffer) = res_buff;
	(*BufferSize) = res_buff_size;
	return true;
	//*버퍼 재할당과 새로운 크기가 필요하다
}
//*** 데이터 스크램블링, 스크램블링을 성공했다는 신호를 보내야 하므로 bool 함수로 한다




void GetRandomValue(unsigned int seed, unsigned int* RandomKeyArray)
{
	unsigned int seed1, seed2;
	seed1 = seed;
	for (unsigned int i = 0; i < 5; i++) {
		seed2 = (seed1 << 0x0D) ^ seed1;
		seed1 = seed2 ^ (seed2 >> 0x11);
		seed1 = seed1 ^ (seed1 << 0x05);
		RandomKeyArray[i] = seed1;
	}
	//*5개 시드값 얻기
	for (unsigned int i = 0; i < 0x13; i++) { Permutation_Round(RandomKeyArray); }
	//*완전한 랜덤값이 되도록 19차례 라운드 진행
}
//*시드 기반 랜덤값 얻기 (5개 int)


void Permutation_Round(unsigned int* KeyArray)
{
	unsigned int m, carry, eax, ecx, edx, edi;
	Mul_64Bit(0x7DD4FFC7, KeyArray[3], &m, &edi, &carry);
	ecx = m;

	Mul_64Bit(0x000005D4, KeyArray[2], &m, &edx, &carry);
	Add_64Bit(ecx, m, &ecx, &carry);
	KeyArray[3] = KeyArray[2];
	Add_With_Carry_64Bit(edi, edx, carry, &edi, &carry);

	Mul_64Bit(0x000006F0, KeyArray[1], &m, &edx, &carry);
	Add_64Bit(ecx, m, &ecx, &carry);
	KeyArray[2] = KeyArray[1];
	Add_With_Carry_64Bit(edi, edx, carry, &edi, &carry);

	Mul_64Bit(0x000013FB, KeyArray[0], &m, &edx, &carry);
	Add_64Bit(0, m, &eax, &carry);
	KeyArray[1] = KeyArray[0];

	Add_64Bit(eax, ecx, &eax, &carry);
	Add_With_Carry_64Bit(edx, edi, carry, &edx, &carry);

	Add_64Bit(eax, KeyArray[4], &eax, &carry);
	Add_With_Carry_64Bit(edx, 0, carry, &edx, &carry);

	KeyArray[0] = eax;
	KeyArray[4] = edx;
}
//*랜덤값을 연산하는 과정


void Mul_64Bit(unsigned int a, unsigned int b,
	unsigned int* res, unsigned int* over, unsigned int* carry)
{
	unsigned __int64 _tmp_64bit = (unsigned __int64)a * (unsigned __int64)b;
	(*res) = (unsigned int)_tmp_64bit;
	(*over) = (_tmp_64bit >= 0x100000000) ? (unsigned int)(_tmp_64bit / 0x100000000) : 0;
	(*carry) = (_tmp_64bit >= 0x100000000) ? 1 : 0;
}


void Add_64Bit(unsigned int a, unsigned int b,
	unsigned int* res, unsigned int* carry)
{
	unsigned __int64 _tmp_64bit = (unsigned __int64)a + (unsigned __int64)b;
	(*res) = (unsigned int)_tmp_64bit;
	(*carry) = (_tmp_64bit >= 0x100000000) ? 1 : 0;
}


void Add_With_Carry_64Bit(unsigned int a, unsigned int b, unsigned int carry,
	unsigned int* res, unsigned int* res_carry)
{
	unsigned __int64 _tmp_64bit = (unsigned __int64)a + (unsigned __int64)b + (unsigned __int64)carry;
	(*res) = (unsigned int)_tmp_64bit;
	(*res_carry) = (_tmp_64bit >= 0x100000000) ? 1 : 0;
}




unsigned int Get_Files_Pass_List (wchar_t *DataFolder, FILE_DESC **f_desc_list_pnt)
{
	HANDLE hFile;
	unsigned int res_count = 0;
	WIN32_FIND_DATAW wfd_w;
	wchar_t f_format[MAX_PATH];
	memset(f_format, 0, sizeof(wchar_t) * MAX_PATH);
	swprintf(f_format, TEXT("%s\\*"), DataFolder);
	hFile = FindFirstFileW(f_format, &wfd_w);
	if (hFile != INVALID_HANDLE_VALUE) {
		do
		{
			wchar_t f_n_format[MAX_PATH];
			memset(f_n_format, 0, sizeof(wchar_t) * MAX_PATH);
			swprintf(f_n_format, TEXT("%s\\%s"), DataFolder, wfd_w.cFileName);
			if (wfd_w.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ((wcscmp(wfd_w.cFileName, TEXT(".")) != 0) && (wcscmp(wfd_w.cFileName, TEXT("..")) != 0)) {
					res_count += Get_Files_Pass_List(f_n_format, f_desc_list_pnt);
				}
			}
			//*디렉토리 속성일 때 : "."나 ".."이 아니면 재귀로 파고들기
			else { 
				res_count++;
				if (f_desc_list_pnt != NULL) {
					wchar_t* tp_str = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(f_n_format) + 1));
					tp_str[wcslen(f_n_format)] = 0;
					memcpy(tp_str, f_n_format, (sizeof(wchar_t)* wcslen(f_n_format)));
					(*f_desc_list_pnt)->FileName_uni = tp_str;
					(*f_desc_list_pnt)++;
				}
			}
			//*파일 속성일 때 : 그냥 더하기, f_desc_list 값이 null이 아니면 붙여주기
		} while (FindNextFileW (hFile, &wfd_w));
		FindClose(hFile);
	}
	//*포맷 확정 후 찾아보기

	return res_count;
	//*합친 값 반환
}
//*파일경로를 매칭시켜 주는 함수 / 뒤쪽이 null이면 그냥 수만 세서 리턴




bool Replace_Data(TEX_REPLACE* _rep_util, UINT32 org_data_offset, UINT32 org_data_length, UINT8* new_data, UINT32 new_data_length)
{
	if (_rep_util == NULL) { return false; }
	if ((_rep_util->n_b == NULL) || (_rep_util->n_b_sav == NULL)) { return false; }
	if ((_rep_util->n_b->_buffer == NULL) || (_rep_util->n_b_sav->_buffer == NULL)) { return false; }
	//*조건 체크

	if (org_data_offset > (_rep_util->n_b->_size)) { return false; }
	if ((org_data_offset + org_data_length) > (_rep_util->n_b->_size)) { return false; }
	//*추가 조건 체크 (데이터의 오프셋과 길이가 원본 크기를 벗어나면 안됨)

	if (org_data_length == new_data_length) {
		memcpy((_rep_util->n_b->_buffer + org_data_offset), new_data, org_data_length);
		return true;
	}
	//*만약에 원래 데이터 길이와 새 데이터 길이가 같다면 그냥 복사하고 나가면 됨

	UINT32 rev1_length = org_data_offset;
	UINT32 rev2_length = (_rep_util->n_b->_size - (org_data_offset + org_data_length));
	memcpy(_rep_util->rev1, _rep_util->n_b->_buffer, rev1_length);
	memcpy(_rep_util->rev2, (_rep_util->n_b->_buffer + org_data_offset + org_data_length), rev2_length);
	//*각 부분을 조각내서 rev1과 rev2에 복사

	memcpy(_rep_util->n_b_sav->_buffer, _rep_util->rev1, rev1_length);
	memcpy((_rep_util->n_b_sav->_buffer) + rev1_length, new_data, new_data_length);
	memcpy((_rep_util->n_b_sav->_buffer) + (rev1_length + new_data_length), _rep_util->rev2, rev2_length);
	_rep_util->n_b_sav->_size = rev1_length + new_data_length + rev2_length;
	//*그리고 새롭게 옯겨넣은 후 길이 갱신

	_BUFFER_INFO* tbp = _rep_util->n_b;
	_rep_util->n_b = _rep_util->n_b_sav;
	_rep_util->n_b_sav = tbp;
	//*포인터 바꿔치기

	return true;
}
//*데이터 교체 함수


wchar_t* Replace_Text(wchar_t* org_txt, const wchar_t* org_part, const wchar_t* change_part)
{
	wchar_t* ret_txt;
	if (wcsstr(org_txt, org_part) == NULL) {
		ret_txt = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(org_txt) + 1));
		memcpy(ret_txt, org_txt, (sizeof(wchar_t) * wcslen(org_txt)));
		ret_txt[wcslen(org_txt)] = 0;
		return ret_txt;
	}
	//*바꿀 문자열을 찾을 수 없으면 그냥 복사하고 반환함
	UINT32 new_txt_len = wcslen(org_txt);
	UINT32 cnt = Get_Txt_Count(org_txt, org_part, wcslen(org_txt));
	for (UINT32 i = 0; i < cnt; i++) {
		new_txt_len -= wcslen(org_part);
		new_txt_len += wcslen(change_part);
	}
	//*바뀐 길이를 미리 체크함
	ret_txt = (wchar_t*)malloc(sizeof(wchar_t) * (new_txt_len + 1));
	memset(ret_txt, 0, sizeof(wchar_t) * (new_txt_len + 1));
	wchar_t* tmp_str_pnt = org_txt;
	wchar_t* tmp_str_2_pnt = ret_txt;
	while (tmp_str_pnt < (org_txt + wcslen(org_txt))) {
		if (wcsncmp(tmp_str_pnt, org_part, wcslen(org_part)) == 0) {
			memcpy(tmp_str_2_pnt, change_part, wcslen(change_part) * sizeof(wchar_t));
			tmp_str_pnt += wcslen(org_part);
			tmp_str_2_pnt += wcslen(change_part);
		}
		else {
			*(tmp_str_2_pnt) = *(tmp_str_pnt);
			tmp_str_pnt++; tmp_str_2_pnt++;
		}
	}
	return ret_txt;
	//*문자열 전부 바꿔치고 반환
}
//*문자열 교체 함수 (결과값으로 바뀐 문자열 반환)




wchar_t* unicode_table_J2K = NULL;
//wchar_t* _Inv_Inicode_table_J2K = NULL;
UINT32 HanjaCnt = 0;

void Load_Hanja()
{
	wchar_t *JapHanja = NULL, *KorHanja = NULL;
	HanjaCnt = 0;
	//*미리 초기화하기

	HANDLE hHanja = CreateFileW(L"Hanja.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hHanja == INVALID_HANDLE_VALUE) {
		MessageBoxW(NULL, L"Hanja.txt 파일이 없습니다.\n이 파일이 없으면 파일 이름을 수동으로 지정해야 합니다.", L"Alert", MB_OK);
		return;
	}

	unsigned int HanjaListSize = (GetFileSize(hHanja, NULL) / sizeof(wchar_t)) - 1;
	wchar_t* buff = (wchar_t*)malloc((HanjaListSize + 1) * sizeof(wchar_t));
	memset(buff, 0, ((HanjaListSize + 1) * sizeof(wchar_t)));
	SetFilePointer(hHanja, sizeof(wchar_t), NULL, FILE_BEGIN);
	ReadFile(hHanja, buff, (HanjaListSize * sizeof(wchar_t)), NULL, NULL);
	CloseHandle(hHanja);
	//*파일 읽어들이기

	for (unsigned int i = 0; i < HanjaListSize - 3; i++) {
		if ((buff[i] == '<') && (buff[i + 1] == '-') && (buff[i + 2] == '>')) { HanjaCnt++; }
	}
	//*로드하기

	KorHanja = (wchar_t*)malloc(sizeof(wchar_t) * HanjaCnt);
	memset(KorHanja, 0, (sizeof(wchar_t) * HanjaCnt));
	JapHanja = (wchar_t*)malloc(sizeof(wchar_t) * HanjaCnt);
	memset(JapHanja, 0, (sizeof(wchar_t) * HanjaCnt));
	//*할당 및 초기화

	unsigned int buffpntidx = 0;
	for (unsigned int i = 0; i < HanjaCnt; i++) {
		while (!((buff[buffpntidx] == '<') && (buff[buffpntidx + 1] == '-') && (buff[buffpntidx + 2] == '>'))) { buffpntidx++; }
		JapHanja[i] = buff[buffpntidx - 2]; KorHanja[i] = buff[buffpntidx + 4];
		buffpntidx++;
	}
	//*버퍼에서 얻어와 뿌리기

	unicode_table_J2K = (wchar_t*)malloc(sizeof(wchar_t*) * 0x10000);
	memset(unicode_table_J2K, 0, (sizeof(wchar_t*) * 0x10000));
	for (unsigned int i = 0; i < 0x10000;i++) { unicode_table_J2K[i] = i; }
	//*유니코드 테이블에 값 옮겨넣기(처음 초기화는 그대로)

//	_Inv_Inicode_table_J2K = (wchar_t*)malloc(sizeof(wchar_t) * 0x10000);
//	memset(_Inv_Inicode_table_J2K, 0, (sizeof(wchar_t) * 0x10000));
//	//*역참조 테이블도 활성화

	for (unsigned int i = 0; i < HanjaCnt; i++) { 
		unicode_table_J2K[JapHanja[i]] = KorHanja[i];
//		_Inv_Inicode_table_J2K[KorHanja[i]] = JapHanja[i];
	}
	//*일본어 한자를 인덱스로 삼고 한국어 한자를 값으로 삼도록 세팅함
	//*변경되지 않은 값은 그냥 있음

	free(JapHanja);
	free(KorHanja);
	free(buff);
	//*버퍼 해제
}
//*"Hanja.txt" 파일 로드 함수


void Release_Hanja()
{
	if (unicode_table_J2K != NULL) { free(unicode_table_J2K); }
//	if (_Inv_Inicode_table_J2K != NULL) { free(_Inv_Inicode_table_J2K); }
}
//*"Hanja.txt" 파일 해제 함수


void JPHan_2_KRHan(wchar_t* str)
{
	if (str != NULL) {
		UINT32 str_length = wcslen(str);
		for (unsigned int j = 0; j < str_length; j++) {
			if (str[j] == 0x3005) {
				if (j > 0) { str[j] = str[j - 1]; }
				else { str[j] = 0x2015; }
			}
			//*중복 문자
			else {
				str[j] = unicode_table_J2K[str[j]];
				//*매칭되는 한자를 찾아서 바꿔치기
			}
		}
	}
}
//*정해진 문자열에서 바꿀 수 있는 한자는 모두 한국한자로 바꾸는 함수


/*
void Make_J2K_Table()
{
	Load_Hanja();
	//*테이블 불러오기

	char* jap_table = (char*)malloc(0x30000);
	memset(jap_table, 0, 0x30000);
	
	for (unsigned int i = 0x20; i <= 0x7E;i++) { jap_table[3 * i] = i; }
	//*ascii
	
	for (unsigned int i = 0xA1; i <= 0xDF; i++) { jap_table[3 * i] = i; }
	//*반각

	for (unsigned int i = 0x81; i <= 0x9F;i++) {
		for (unsigned int j = 0x40; j <= 0xFF;j++) {
			wchar_t _val = ((wchar_t)i * 0x100) + j;
			jap_table[3 * _val] = i;
			jap_table[(3 * _val) + 1] = j;
			wchar_t* _wt = J2U(&jap_table[3 * _val]);
			if (_wt[0] == '?') {
				jap_table[3 * _val] = jap_table[(3 * _val) + 1] = 0;
			}
			free(_wt);
			//*일일이 확인하려면 머리에 쥐가 날 것 같으니 유니코드로 바꿔보는게 더 빠를 듯 하다
		}
	}
	//*전각 (첫번째 영역)

	for (unsigned int i = 0xE0; i <= 0xEE; i++) {
		for (unsigned int j = 0x40; j <= 0xFF; j++) {
			wchar_t _val = ((wchar_t)i * 0x100) + j;
			jap_table[3 * _val] = i;
			jap_table[(3 * _val) + 1] = j;
			wchar_t* _wt = J2U(&jap_table[3 * _val]);
			if (_wt[0] == '?') {
				jap_table[3 * _val] = jap_table[(3 * _val) + 1] = 0;
			}
			free(_wt);
			//*일일이 확인하려면 머리에 쥐가 날 것 같으니 유니코드로 바꿔보는게 더 빠를 듯 하다
		}
	}
	//*전각 (두번째 영역)

	for (unsigned int i = 0xFA; i <= 0xFC; i++) {
		for (unsigned int j = 0x40; j <= 0xFF; j++) {
			wchar_t _val = ((wchar_t)i * 0x100) + j;
			jap_table[3 * _val] = i;
			jap_table[(3 * _val) + 1] = j;
			wchar_t* _wt = J2U(&jap_table[3 * _val]);
			if (_wt[0] == '?') {
				jap_table[3 * _val] = jap_table[(3 * _val) + 1] = 0;
			}
			free(_wt);
			//*일일이 확인하려면 머리에 쥐가 날 것 같으니 유니코드로 바꿔보는게 더 빠를 듯 하다
		}
	}
	//*전각 (세번째 영역)

	wchar_t** _jap_table_uni = (wchar_t**)malloc(sizeof(wchar_t*) * 0x10000);
	memset(_jap_table_uni, 0, (sizeof(wchar_t*) * 0x10000));
	for (unsigned int i = 0; i < 0x10000;i++) {
		if (jap_table[3 * i] != 0) { _jap_table_uni[i] = J2U(&jap_table[3 * i]); }
	}
	//*일본어 영역에서 쓰는 문자들만 싸그리 긁어서 유니코드 테이블로 하나 만들기

	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stdout), _O_U16TEXT);
	//*입력/출력을 unicode로 준비

	for (unsigned int i = 0x20; i <= 0x7E; i++) {
		_Inv_Inicode_table_J2K[i] = i;
	}
	//*ascii 코드는 그대로 세팅

	for (unsigned int i = 0xA1; i <= 0xDF; i++) { 
		if ((i >= 0xA1) && (i <= 0xBA)) { 
			unicode_table_J2K[i] = (i - 0xA1) + 'A';
			_Inv_Inicode_table_J2K[(i - 0xA1) + 'A'] = i;
		}
		//*대문자
		else if ((i >= 0xBB) && (i <= 0xD4)) {
			unicode_table_J2K[i] = (i - 0xBB) + 'a';
			_Inv_Inicode_table_J2K[(i - 0xBB) + 'a'] = i;
		}
		//*소문자
		else {
			if ((i - 0xD5) <= 9) {
				unicode_table_J2K[i] = (i - 0xD5) + '0';
				_Inv_Inicode_table_J2K[(i - 0xD5) + '0'] = i;
			}
			else {
				unicode_table_J2K[i] = '@';
				_Inv_Inicode_table_J2K['@'] = i;
			}
		}
		//*숫자
	}
	//*반각은 알파벳 영역에 맞추어 처리한다

	//*** _jap_table_uni : 인덱스는 ansi, 값은 unicode

	char* _kor_c;
	wchar_t _tp_char[2];
	_tp_char[0] = _tp_char[1] = 0;
	//*임시로 값을 받아올 변수

	random_device rd;
	mt19937 engine(rd());
	uniform_int<> distribution_1(0xB0, 0xFE);
	uniform_int<> distribution_2(0xA1, 0xFF);
	//*랜덤 돌릴 준비, 한글을 최소화해야 보기 좀 편할듯하다

	uniform_int<> distribution_3(0x81, 0xC7);
	uniform_int<> distribution_4(0x41, 0xFF);
	//*공간부족할 때 랜덤 추가용

	for (unsigned int i = 0x8140; i < 0x10000;i++) {
		if (_jap_table_uni[i] != NULL) {

			wchar_t _get_uni_char = _jap_table_uni[i][0];
			//*유니코드 문자 얻기

			if (_get_uni_char == 0x3005) { continue; }
			//*중복문자가 아닐 때만 수행한다

			_kor_c = U2K(_jap_table_uni[i]);
			//*한국어 언어코드로 바꿨을 때 문자 얻기

			if (_kor_c[0] == '?') {
				free(_kor_c);
				//*기존 값은 쓸모없으니 해제

				_tp_char[0] = unicode_table_J2K[_get_uni_char];
				_kor_c = U2K(_tp_char);
				//*기존에 등록된 테이블에 있는 값을 확인해 본다

				if (_kor_c[0] == '?') {

					setlocale(LC_ALL, ".932");
					wprintf(TEXT("'%s'[%04X]::"), _jap_table_uni[i], _jap_table_uni[i][0]);
					setlocale(LC_ALL, ".949");
					//*일본한자 출력 후 되돌리기

					if (_jap_table_uni[i][0] == 0x86EC) {
						bool _ss = true;
					}

					wprintf(TEXT("한국어 코드 글자가 아니고 테이블에도 없습니다.\n"));
					wprintf(TEXT("임의로 매칭합니다...\n"));
					while (1)
					{
//						if (_jap_table_uni[i][0] >= 0xE700) {
							_tp_char[0] = (distribution_1(engine) << 8) | distribution_2(engine);
//						}
//						else {
//							_tp_char[0] = (distribution_3(engine) << 8) | distribution_4(engine);
//						}
						_tp_char[1] = 0;
						//*입력받기...는 너무 힘들다
						//*그냥 한글 언어코드 중에 랜덤으로 바꾸는 게 나을 듯 하다
						//*euc-kr 기준 0xCAA0 ~ 0xFDFE까지 랜덤매칭?, 앞뒤 하나씩 빼고

						_kor_c = U2K(_tp_char);
						if (_kor_c[0] == '?') {
//							wprintf(TEXT("해당 글자는 한국어 코드 글자가 아닙니다.\n"));
							free(_kor_c);
							continue;
						}
						//*해당 문자가 한국어 언어코드 글자인지 확인

						if (_Inv_Inicode_table_J2K[_tp_char[0]] != 0) {
//							wprintf(TEXT("해당 글자는 이미 등록된 상태입니다.\n"));
							free(_kor_c);
							continue;
						}
						//*한국어 언어코드가 맞다면 중복등록된 글자인지 확인

						_Inv_Inicode_table_J2K[_tp_char[0]] = _get_uni_char;
						unicode_table_J2K[_get_uni_char] = _tp_char[0];
						free(_kor_c);
						setlocale(LC_ALL, ".932");
						wprintf(TEXT("'%s' <-> "), _jap_table_uni[i]);
						setlocale(LC_ALL, ".949");
						wprintf(TEXT("'%s'\n\n"), _tp_char);
						break;
						//*중복등록된 글자가 아니라면 그제서야 등록하고 빠져나온다
						//*그리고 원본도 바꿔친다
					}
					//*입력루프
				}
				//*여기서도 등록된 값이 없으면 직접 입력받아야 한다

				else {
					_Inv_Inicode_table_J2K[_tp_char[0]] = _get_uni_char;
					free(_kor_c);
				}
				//*등록된 값이 있으면 그걸로 하면 된다
			}
			//*한국어 언어코드에 없는 문자일 때

			else {
				_Inv_Inicode_table_J2K[_get_uni_char] = _get_uni_char;
				free(_kor_c);
				//*유니코드를 그대로 등록해주면 된다
			}
			//*한국어 언어코드에 있는 문자일 때

		}
	}
	//*전각 처리
	//*일본어에서 한국어로 바꿀 때 깨지는 문자가 있는지 확인하고 바꿀 문자 입력받기
	//*바꿀 문자를 받았을 때 중복 검사
	//*ascii는 건들지 않음

	HANDLE hWrite = CreateFile(TEXT("Output_Hanja.txt"), GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	wchar_t _str[0x10];
	wchar_t tps_1[2], tps_2[2];
	wchar_t BOM = 0xFEFF;
	WriteFile(hWrite, &BOM, sizeof(wchar_t), NULL, NULL);
	tps_1[0] = tps_1[1] = tps_2[0] = tps_2[1] = 0;
	for (unsigned int i = 0; i < 0x10000;i++) {
		if ((_Inv_Inicode_table_J2K[i] != 0x00)) {
			tps_1[0] = _Inv_Inicode_table_J2K[i];
			tps_2[0] = i;
			if (tps_1[0] != tps_2[0]) {
				wsprintf(_str, TEXT("%s <-> %s\r\n"), tps_1, tps_2);
				WriteFile(hWrite, _str, (sizeof(wchar_t)* wcslen(_str)), NULL, NULL);
			}
		}
	}
	CloseHandle(hWrite);
	//*최종적으로 완성된 테이블을 텍스트파일로 출력하기
	//*서로 같으면 그냥 기록하지 않는다
	//_Inv_Inicode_table_J2K[KorHanja[i]] = JapHanja[i];

	free(jap_table);
	for (unsigned int i = 0; i < 0x10000; i++) {
		if (_jap_table_uni[i] != NULL) { free(_jap_table_uni[i]); }
	}
	free(_jap_table_uni);
	//*정리

	_setmode(_fileno(stdin), _O_TEXT);
	_setmode(_fileno(stdout), _O_TEXT);
	//*입력/출력을 원래대로 되돌림
}
*/


bool is_Pure_Ascii_String(wchar_t *_str)
{
	for (unsigned int i = 0; i < wcslen(_str);i++) {
		if (_str[i] >= 0x80) { return false; }
		else if (_str[i] == '\\') { 
			return false; 
		}
	}
	return true;
}
//*해당 문자열이 순수 ascii 문자열인지 판별하는 함수


bool is_File_Path_String(wchar_t* _str)
{
	unsigned int i = 0;
	while (i < wcslen(_str)) {
		if (_str[i] == '\\') { 
			if (i < (wcslen(_str) - 1)) {
				if ((_str[i + 1] == 'r') || (_str[i + 1] == 'n')) { i += 2; }
				else { return true; }
			}
			else { return true; }
		}
		else { i++; }
	}
	return false;
}
//*해당 문자열이 경로 문자열인지 판별하는 함수
//"\r"이나 "\n"은 경로 문자열로 치지 않는다




char* _get_str(unsigned char** _buffer_pnt)
{
	unsigned char* buffer_pnt = (*_buffer_pnt);

	unsigned int str_len = *(unsigned int*)buffer_pnt;
	buffer_pnt += sizeof(int);
	//*문자열 길이 얻기

	char* str_buffer = (char*)malloc(str_len + 1);
	str_buffer[str_len] = 0;
	memcpy(str_buffer, buffer_pnt, str_len);
	buffer_pnt += str_len;
	//*길이만큼 할당 후 문자열 복사 

	(*_buffer_pnt) = buffer_pnt;
	return str_buffer;
}
//*문자열을 읽어들이는 함수

void _write_str(HANDLE hWrite, char* _str)
{
	unsigned int _len = strlen(_str);
	WriteFile(hWrite, &_len, sizeof(int), NULL, NULL);
	WriteFile(hWrite, _str, _len, NULL, NULL);
}
//*문자열을 기록하는 함수


double* _get_double(unsigned char** _buffer_pnt)
{
	unsigned char* buffer_pnt = (*_buffer_pnt);

	double* double_buffer = (double*)malloc(sizeof(double));
	*double_buffer = *(double*)buffer_pnt;
	buffer_pnt += sizeof(double);
	//*정수 할당 후 복사 

	(*_buffer_pnt) = buffer_pnt;
	return double_buffer;
}
//*double을 읽어들이는 함수

void _write_double(HANDLE hWrite, double _do)
{
	WriteFile(hWrite, &_do, sizeof(double), NULL, NULL);
}
//*double을 기록하는 함수


unsigned int* _get_int(unsigned char** _buffer_pnt)
{
	unsigned char* buffer_pnt = (*_buffer_pnt);

	unsigned int* int_buffer = (unsigned int*)malloc(sizeof(int));
	*int_buffer = *(unsigned int*)buffer_pnt;
	buffer_pnt += sizeof(int);
	//*정수 할당 후 복사 

	(*_buffer_pnt) = buffer_pnt;
	return int_buffer;
}
//*정수를 읽어들이는 함수

void _write_int(HANDLE hWrite, unsigned int _int)
{
	WriteFile(hWrite, &_int, sizeof(int), NULL, NULL);
}
//*정수를 기록하는 함수


unsigned short* _get_word(unsigned char** _buffer_pnt)
{
	unsigned char* buffer_pnt = (*_buffer_pnt);

	unsigned short* word_buffer = (unsigned short*)malloc(sizeof(short));
	*word_buffer = *(unsigned short*)buffer_pnt;
	buffer_pnt += sizeof(short);
	//*워드 할당 후 복사 

	(*_buffer_pnt) = buffer_pnt;
	return word_buffer;
}
//*워드를 읽어들이는 함수

void _write_word(HANDLE hWrite, unsigned short _s)
{
	WriteFile(hWrite, &_s, sizeof(short), NULL, NULL);
}
//*워드를 기록하는 함수


unsigned char* _get_byte(unsigned char** _buffer_pnt)
{
	unsigned char* buffer_pnt = (*_buffer_pnt);

	unsigned char* byte_buffer = (unsigned char*)malloc(sizeof(char));
	*byte_buffer = *buffer_pnt;
	buffer_pnt++;
	//*정수 할당 후 복사 

	(*_buffer_pnt) = buffer_pnt;
	return byte_buffer;
}
//*바이트를 읽어들이는 함수

void _write_byte(HANDLE hWrite, unsigned char _c)
{
	WriteFile(hWrite, &_c, sizeof(char), NULL, NULL);
}
//*바이트를 기록하는 함수


unsigned char* _get_data(unsigned char** _buffer_pnt, unsigned int _len)
{
	unsigned char* buffer_pnt = (*_buffer_pnt);

	unsigned char* data_buffer = (unsigned char*)malloc(_len + 1);
	memcpy(data_buffer, buffer_pnt, _len);
	data_buffer[_len] = 0;
	buffer_pnt += _len;
	//*길이만큼 할당 후 데이터 복사 
	//*문자열을 얻을 수도 있으니 끝부분에 마감처리를 한다

	(*_buffer_pnt) = buffer_pnt;
	return data_buffer;
}
//*지정 길이만큼 읽어들이는 함수

void _write_data(HANDLE hWrite, unsigned char* _buff, unsigned int _len)
{
	WriteFile(hWrite, _buff, _len, NULL, NULL);
}
//*지정 길이만큼 기록하는 함수


char* stristr(const char* str1, const char* str2)
{
	const char* p1 = str1;
	const char* p2 = str2;
	const char* r = *p2 == 0 ? str1 : 0;

	while (*p1 != 0 && *p2 != 0)
	{
		if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2))
		{
			if (r == 0) { r = p1; }
			p2++;
		}
		else
		{
			p2 = str2;
			if (r != 0) { p1 = r + 1; }
			if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2)) { r = p1; p2++; }
			else { r = 0; }
		}
		p1++;
	}

	return *p2 == 0 ? (char*)r : 0;
}
//*대소문자 구분 없는 strstr

wchar_t* wcsistr(const wchar_t* str1, const wchar_t* str2)
{
	const wchar_t* p1 = str1;
	const wchar_t* p2 = str2;
	const wchar_t* r = *p2 == 0 ? str1 : 0;

	while (*p1 != 0 && *p2 != 0)
	{
		if (towlower((unsigned short)*p1) == towlower((unsigned short)*p2))
		{
			if (r == 0) { r = p1; }
			p2++;
		}
		else
		{
			p2 = str2;
			if (r != 0) { p1 = r + 1; }
			if (towlower((unsigned short)*p1) == towlower((unsigned short)*p2)) { r = p1; p2++; }
			else { r = 0; }
		}
		p1++;
	}

	return *p2 == 0 ? (wchar_t*)r : 0;
}
//*대소문자 구분 없는 wcsstr