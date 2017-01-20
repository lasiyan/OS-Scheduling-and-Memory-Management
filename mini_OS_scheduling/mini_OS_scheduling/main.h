#pragma once

#include"stdafx.h"

#ifndef __MAIN_H__
#define __MAIN_H__

/* time */
extern TIMECAPS resolution;			// 타이머 정밀도 정보를 담을 구조체
extern float Program_Begin_Time;	// 프로그램이 시작된 시간을 담을 변수(기준) // 경과시간 = 특정시간 - (기준)
extern struct tm *t;				// tm형 구조체포인터 t
extern time_t timer;				// 시간 측정(시스템 현재 시간 구조체)

void Process_Info(void);			// 프로세스 정보 출력 함수

#endif // !__MAIN_H__
