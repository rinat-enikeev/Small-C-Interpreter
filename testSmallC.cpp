#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#ifndef EXTERN_VAR_DECL_H
#include "externVars.h"
#endif

#ifndef COMMON_ENUMS_DECL_H
#include "commonEnums.h"
#endif

#ifndef RESTRICTIONS_DECL_H
#include "restrictions.h"
#endif

// {{* added array functionality
extern struct array_type {
  char arr_name[ID_LEN];
  int arr_type;
  void *value;
} global_arrays[NUM_GLOBAL_ARRAYS];
// added array functionality *}}

extern int garr_index;

void testcase_arrays(void);
int load_program(char *p, char *fname);

int get_token(void);
void prescan(void);

int main(void)
{
	/* ��������� ������ ��� ��������� */
	if((p_buf = (char *) malloc(PROG_SIZE))==NULL) {
		printf("Can not allocate memory");
		exit(1);
	}

	/* �������� �������� ��������� � �������� ��� ���������� */
	if(!load_program(p_buf, "arrays.test")) exit(1);
	prog = p_buf;
	testcase_arrays(); 

	return 0;
}

/* �������� ���������. */
int load_program(char *p, char *fname)
{
	FILE *fp;
	int i=0;

	if((fp=fopen(fname, "rb"))==NULL) return 0;

	i = 0;
	do {
		*p = getc(fp);
		p++; i++;
	} while(!feof(fp) && i<PROG_SIZE);

	if(*(p-2) == 0x1a) *(p-2) = '\0'; /* ��������� ���������
									������� �������� */
	else *(p-1) = '\0';
	fclose(fp);
	return 1;
}

/* ���� ��������������� ������� ����: 

	int [] array;

*/
void testcase_arrays(void)
{
	cout << "Test input source is: \n" << endl;
	cout << "________________________________________ \n" << endl;
	cout << p_buf << endl;
	cout << "________________________________________ \n" << endl;
	
	cout << "entering prescan..." << endl;
	prescan(); // int [] array; 
	cout << "prescan ended." << endl;

	/* cout << garr_index << endl;

	assert(garr_index == 1);
	assert(global_arrays[0].arr_type == INT);
	assert(strcmp(global_arrays[0].arr_name, "array")); */
}