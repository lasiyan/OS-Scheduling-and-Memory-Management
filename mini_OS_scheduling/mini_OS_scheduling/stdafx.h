// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//
#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <list> 
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>			
#include <time.h>
#include <mmsystem.h>
#include <Psapi.h>

#define _CRT_SECURE_NO_WARNINGS
#define MAX_Thread 15
#define MEMORY_ALLOC_SET 5
#define MB 1024 * 1024

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "psapi.lib")
#pragma warning(disable: 4996)

using namespace std; //제공되는 list클래스를 가지고 오기위해 사용 

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
