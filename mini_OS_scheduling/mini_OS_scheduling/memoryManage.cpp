#include "stdafx.h"
#include "memoryManage.h"

/* MemoryManagement and Mutex - Virtual Alloc and Mutex */
SYSTEM_INFO sysInfo;	//SYSTEM_INFO ����ü
HANDLE ghMutex;			// Mutex�ڵ� ����
char* MyMemory[MEMORY_ALLOC_SET] = { 0, };	// My Memroy 
int PageTable[MEMORY_ALLOC_SET][2] = { 0 };	// Virtual PageTable Creating
NODE Mthread[MEMORY_ALLOC_SET];				// Creating threads using in Memory Management
NODE* node_head = 0;
char* MyBuffer;			// My virtual Memory

int memory_Management_start() {
	printf("\n## �޸� �Ҵ��� ������ ���� ##\n");
	GetSystemInfo(&sysInfo);

	ghMutex = CreateMutex(NULL, FALSE, NULL);
	if (ghMutex == NULL)
		printf("���ؽ� ���� ����\n");

	for (int j = 0; j < MEMORY_ALLOC_SET; j++) {
		Mthread[j].hthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MemoryManagement,
			(LPVOID)j, CREATE_SUSPENDED, NULL);
		Sleep(700);
		if (Mthread[j].hthread == NULL)	{
			printf("�޸� �Ҵ�� ������ ���� ����!!\n");
			return 0;
		}
	}
	printf("## �޸� �Ҵ��� ������ ���� �Ϸ� ##\n");
	Print_Memory_Status("�ʱ� �޸� ���� : "); Sleep(1000);

	printf("\n## ������ �޸� �Ҵ� ���� ##\n");
	printf("MyMemory : 256Mbyte, PageSize : 4kbyte, offset : 0x040\n");
	printf("\n   �̸�		ũ��		My���ּ�		My�����ּ�\n");
	printf("------------------------------------------------------------------\n");
	for (int j = 0; j < MEMORY_ALLOC_SET; j++) {
		ResumeThread(Mthread[j].hthread);
		Sleep(700);
	}
	printf("## ������ �޸� �Ҵ� �Ϸ� ##\n");
	Print_Memory_Status("�Ҵ� �� �޸� ���� : ");

	printf("## ������ �޸� ��ȯ ���� ##\n");
	for (int j = 0; j < MEMORY_ALLOC_SET; j++) {
		VirtualFree(Mthread[j].hthread, 0, MEM_RELEASE);
		myfree(MyMemory[j]);
		Sleep(700);
	}
	free(MyBuffer);
	Print_Memory_Status("��ȯ �� �޸� ���� : ");
	printf("## �޸� ��ȯ ���� ##\n");

	WaitForMultipleObjects(MEMORY_ALLOC_SET, (HANDLE*)Mthread, TRUE, INFINITE);
	
	for (int j = 0; j < MEMORY_ALLOC_SET; j++)
		CloseHandle(Mthread[j].hthread);	// �޸� �Ҵ�� ������ �ڵ� ����
	CloseHandle(ghMutex);	// Mutex ��� �ڵ� ����
	Sleep(1000);
	printf("\n## ���α׷� ���� ##\n");
}

DWORD WINAPI MemoryManagement(LPVOID arg)
{
	DWORD dwWaitResult;	//���ؽ� ���� ������ ����
	int j = (int)arg;
	while (1) {
		dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);

		switch (dwWaitResult) {	// WaitForSingleObject���� ���ϵ� ������ �Ǵ�
		case WAIT_OBJECT_0:
			__try {
				Mthread[j].Size = (20 + j) * MB;		// ������ ���� ���� / 20, 21, 22 ...MB
				
				/* ����޸� �Ҵ� - �̱��� */
				/*
				Mthread[j].hthread = VirtualAlloc(NULL, Mthread[j].Size, MEM_RESERVE, PAGE_READWRITE);
					// Mthread[j]�� ����޸� �Ҵ� (����)
				Mthread[j].hthread = VirtualAlloc(Mthread[j].hthread, Mthread[j].Size,
				MEM_COMMIT, PAGE_READWRITE);	// ����޸𸮿� �Ҵ� Ȯ��
				memset(Mthread[j].hthread, '\0', Mthread[j].Size); // RAM�� ������ size��ŭ �Ҵ�
				printf("Mthread[%d]	%-3dMB	0X%08X", (int)arg, (Mthread[j].Size / (1024 * 1024)), (Mthread + j));
					//VirtualFree(Mthread[j].hthread, 0, MEM_RELEASE);
				*/

				MyMemory[j] = (char*)Mymalloc(sizeof(char) * Mthread[j].Size);
				printf("Mthread[%d]	%-3dMB		0x%08p", j, Mthread[j].Size / (1024 * 1024), MyMemory[j]);

				/* Paging */
				PageTable[j][1] = (10 + 3 * j) * 1234;	// �����ּ��� �������ѹ� ���� ����
				printf("		0x%08p\n",
					Transfer_LA_to_PA((int)MyMemory[j], j));
			}
			__finally {
				if (!ReleaseMutex(ghMutex)) //ghMutex �ڵ� ���
					printf("Wait for Create Thread..\n");
			}
			break;
		case WAIT_ABANDONED:	// �̹� ���ؽ� ������̸�
			return FALSE;
		}
		return TRUE;
	}

}
void Print_Memory_Status(const char *pszMsg)
{
	HANDLE hMe = NULL;
	PROCESS_MEMORY_COUNTERS mem;

	hMe = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());

	if (hMe && GetProcessMemoryInfo(hMe, &mem, sizeof(PROCESS_MEMORY_COUNTERS)))
	{
		printf("%s", pszMsg);
		printf("Working : %7ld bytes, PageFileUsage : %10ld bytes\n", mem.WorkingSetSize, mem.PagefileUsage);
	}
}

char* Transfer_LA_to_PA(UINT LAddr, int j)		// �����ּҿ��� ���ּ��� ��ȯ
{
	/* Transfer LAddr to PAddr logic
	offset = LAddr % PageSize
	pg num = LAddr / PageSize
	Frame  = [j] == PageTable[j]
	PAddr  = Frame * PageSize + offset;
	*/

	int PageSize = 4096;	// 4K
	int offset = LAddr % PageSize;
	int PageNum = LAddr / PageSize;
	int Frame = PageTable[j][1];

	LAddr = Frame * PageSize + offset;

	return Mthread[j].Address = (char*)LAddr;
}

/* Double-Linked List using in MyMemory */
node* search_free_node(size_t size)
{
	node* n = node_head;

	while (1)
	{
		/* ����ִ� ����� ������� ū ��带 Ž�� */
		if (n->Free_status == TRUE && n->Size >= size)
			return n;
		else
		{
			n = n->Nextnode;

			/* ������ ���� ��带 ã�� ���ϸ� ���α׷� ���� */
			if (n == node_head)
			{
				printf("������ �����ϴ� �� ��尡 �����ϴ�.\n");
				return 0;
			}
		}
	}
}

node* dll_create()	// ó�� �迭�� �� ������ ���
{
	int FullSize = 256 * MB;
	node* n = (node*)malloc(sizeof(node));
	MyBuffer = (char*)malloc(sizeof(char) * FullSize);
	// MyMemory = 256MB �Ҵ�

	n->Address = MyBuffer;
	n->Size = FullSize;
	n->Nextnode = n;
	n->Prenode = n;
	n->Free_status = TRUE;

	return n;
}
void* Mymalloc(size_t size)
{
	node* n = 0;
	node* temp = 0;

	if (node_head == 0) { // For first use
		node_head = dll_create();
	}
	temp = search_free_node(size);	// temp = �� ���
	if (temp == 0)
		printf("Malloc Fail\n");
	else
	{
		n = (node*)malloc(sizeof(node));	// node�� ������ �����Ҵ�
		n->Address = temp->Address;			// temp�� �ּ� ����			
		n->Size = temp->Size;	// temp�� �Ҵ��� ũ�� ����
		n->Nextnode = temp;		// ����� �������� temp
		n->Prenode = temp->Prenode;	// ����� ���� ���� temp�� ����Ű�� ����� �������
		n->Free_status = FALSE;		// ��尡 ��������� ǥ��

		temp->Address = temp->Address + size;	// �� ����� �ּҸ� size��ŭ ����
		temp->Size = temp->Size - size;			// �� ����� ũ��� size��ŭ ����
		temp->Prenode->Nextnode = n;
		temp->Prenode = n;
		// �� ������ size��ŭ�� ��� �߰� �Ϸ�
	}
	return n->Address;
}

node* search_node(void* addr)
{
	node* n = node_head;

	while (1)
	{
		if (n->Address == (char*)addr)
			return n;
		else
		{
			n = n->Nextnode;
			if (n == node_head) {
				printf("��尡 �����ϴ�.\n");
				return 0;
			}
		}
	}
}

BOOL check_sequence(node* n1, node* n2)	// ��尡 ������ִ��� Ȯ��
{
	if (n1->Address + n1->Size == n2->Address)
		return TRUE;
	else
		return FALSE;
}
void mysize_free(void* addr)
{
	node* n = 0;
	if (!(n = search_node(addr))) {
		printf("[Free] free is failed: no node which has the address\n");
		return;
	}
	n = search_node(addr);
	n->Free_status = TRUE;
}
void myfree(void* addr) {	// Free() �Լ� ����
	node* n = 0;
	node* temp = 0;

	if (!(n = search_node(addr))) { // There is no node which has the address
		printf("[Free] free is failed: no node which has the address\n");
		return;
	}
	if ((n->Prenode->Free_status == TRUE) && (n->Nextnode->Free_status == TRUE)) {
		if ((n->Prenode->Free_status == TRUE) && (check_sequence(n->Prenode, n) == TRUE)) {
			n->Prenode->Size += n->Size;
			n->Prenode->Nextnode = n->Nextnode;
			n->Prenode->Nextnode = n->Prenode;
			if ((n->Nextnode->Free_status == TRUE) && (n->Nextnode != n->Prenode && (check_sequence(n, n->Nextnode) == TRUE)))
			{
				n->Prenode->Size += n->Nextnode->Size;
				n->Prenode->Nextnode = n->Nextnode->Nextnode;
				n->Nextnode->Nextnode->Prenode = n->Prenode;
				free(n->Nextnode);
			}
			free(n);

		}
		else if (check_sequence(n, n->Nextnode) == TRUE) {
			temp = n->Nextnode;

			n->Nextnode->Address -= n->Size;
			n->Nextnode->Size += n->Size;
			n->Prenode->Nextnode = n->Nextnode;
			n->Nextnode->Prenode = n->Prenode;

			free(temp);
		}
	}
	else {
		n->Free_status = TRUE;
	}
}