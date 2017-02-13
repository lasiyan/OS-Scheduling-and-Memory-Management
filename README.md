# OS-Scheduling-and-Memory-Management

Create Threads and implement operating system process scheduling and also treating memory management using Win32 console in VisualStudio 2015

본 프로젝트는 랜덤한 우선순위를 가지는 thread를 사용하여 Operating System의 Scheduling 처리 과정을 구현한 것으로서 3개의 각각 다른 우선순위를 가진 Queue를 두고 해당 우선순위에 맞는 thread를 저장하여 출력한다. 또한 각각 다른 용량(Size)를 가진 thread 5개를 생성하여 임의로 생성한 메모리(RAM)에 할당하여 Memory Management를 구현한다.

## 개발 환경

* OS : Windows 10 Pro 64bit
* CPU : Intel(R) Core(TM) i5-5200U CPU @ 2.20GHz 2.20 GHz
* RAM : 4.00GB
* IDE : Microsoft Visual Studio Community 2015

## 사용 기술

C++, Window API, STL list, Multimedia timer, MultiThreading

## 부가 설명

 먼저 프로그램이 해당 프로세스의 정보를 출력하고 다음화면이 실행되면 MAX_Thread 개수만큼(본 프로젝트에서는 15개로 설정) thread를 생성한다. 이 때 각각의 thread는 랜덤하게 부여된 우선순위(-15 ~ 15)를 가지고 대기상태로 생성되며 STL list를 사용하여 해당 우선순위 범위를 3등분 한 Low, Middle, High Queue를 생성한다.

 이 후 각각의 우선순위 범위에 따라 알맞은 Queue에 저장하고 해당 thread의 생성번호, 핸들값, 아이디값, 우선순위 정보, 큐 정보, 생성시간, 작동시간, 종료시간을 출력한다. 이 때 높은 우선순위(ex. High)를 가진 thread가 낮은 우선순위(ex. Low)를 가진 thread보다 비교적 자주 출력되도록 각각의 Queue를 MultiThreading 기법으로 구성하고 해당 Queue에 따른 출력 빈도수를 Sleep 함수를 통해 제어한다.

 이렇게 scheduling에 대한 실행이 끝나면 5개의 다른 thread를 생성하고 각각 20~24Mb까지의 크기(Size)와 주소를 할당한다. 이 후 임의로 생성한 Logical to Physical Table에 따라 해당 논리주소에 대한 실제 물리주소를 임의의 배열 MyMemory를 생성하여 Mutex방식으로 직접 구현한 MyMalloc 함수를 통해 해당 배열의 주소에 위치시킨다. 이렇게 메모리 할당 전과 후, 그리고 마지막으로 메모리 반환 후 실제 OS 메모리 상태를 출력하고 프로그램을 종료한다.

## 실행 과정

1. OS-Scheduling-and-Memory-Management-master.zip 파일을 다운로드 후 압축 해제
2. 새 프로젝트 - 프로젝트 생성 옵션에서 SDL 체크 헤제
3. .cpp파일과 .h파일 추가 후 실행

## 실행 결과

[![Demo Doccou alpha](https://j.gifs.com/Bg28wk.gif)](https://youtu.be/pbiWALPjNU0)
