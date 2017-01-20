#pragma once
#include "stdafx.h"

typedef struct node {
	char* Address;
	unsigned int Size;

	struct node* Nextnode;	// 현재 노드의 다음 노드 Next Node
	struct node* Prenode;	// 현재 노드의 이전 노드 Pre Node

	BOOL Free_status;
	HANDLE hthread;
}NODE;

/* 함수 원형 */
int memory_Management_start();
DWORD WINAPI MemoryManagement(LPVOID);
void myfree(void*);
void* Mymalloc(size_t);
char* Transfer_LA_to_PA(UINT, int);
BOOL GetThreadStartAddress(ULONG, PVOID);
void Print_Memory_Status(const char*);
