#pragma once

#include"stdafx.h"

#ifndef __MAIN_H__
#define __MAIN_H__

/* time */
extern TIMECAPS resolution;			// Ÿ�̸� ���е� ������ ���� ����ü
extern float Program_Begin_Time;	// ���α׷��� ���۵� �ð��� ���� ����(����) // ����ð� = Ư���ð� - (����)
extern struct tm *t;				// tm�� ����ü������ t
extern time_t timer;				// �ð� ����(�ý��� ���� �ð� ����ü)

void Process_Info(void);			// ���μ��� ���� ��� �Լ�

#endif // !__MAIN_H__
