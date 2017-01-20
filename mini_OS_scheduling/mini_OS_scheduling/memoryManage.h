#pragma once
#include "stdafx.h"

typedef struct node {
	char* Address;
	unsigned int Size;

	struct node* Nextnode;	// ���� ����� ���� ��� Next Node
	struct node* Prenode;	// ���� ����� ���� ��� Pre Node

	BOOL Free_status;
	HANDLE hthread;
}NODE;

/* �Լ� ���� */
int memory_Management_start();
DWORD WINAPI MemoryManagement(LPVOID);
void myfree(void*);
void* Mymalloc(size_t);
char* Transfer_LA_to_PA(UINT, int);
BOOL GetThreadStartAddress(ULONG, PVOID);
void Print_Memory_Status(const char*);
