#pragma once
#include "stdafx.h"
#include "thread.h"

void scheduling_start();
DWORD WINAPI HighQue_func(LPVOID);		// High_Que function
DWORD WINAPI MiddleQue_func(LPVOID);	// Middle_Que function
DWORD WINAPI LowQue_func(LPVOID);		// Low_Que function