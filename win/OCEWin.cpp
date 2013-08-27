// OCEWin.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <windows.h>
#include <string>
#include <iostream>

#include <lm.h>

extern int system_main(int argc, char *argv[]);

#ifdef _MSC_VER

int _tmain(int argc, _TCHAR* argv[])
{
    int my_argc = 2;
    char *my_argv[2] = {"app","engine.path=."};
    char *s = NULL;

   if (argc > 1)
    {
           std::basic_string<_TCHAR> mystring = argv[1];

           int len = mystring.length() + 1;

           s = new char[len+10];

           memset(s, 0, len+10);

           size_t cnt;

           //wcstombs_s(&cnt, s, len, mystring.c_str(), _TRUNCATE);
           
#ifdef UNICODE
           int ret = WideCharToMultiByte(CP_ACP, 0 ,mystring.c_str(),len, s, len+10, NULL, NULL  );
		   my_argv[1] = s;
#else
		   my_argv[1] = argv[1];
#endif

           
    }
    //_CrtSetBreakAlloc(49194); // for debugging memory leaks


	// ���μ��� �켱���� 
   SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	// QueueTimer���� ����� �ּ��ֱⰪ ���� (1msec)
   if(timeBeginPeriod(1) == TIMERR_NOCANDO)
		return -1;

 
   system_main(my_argc, (char **)my_argv);


	// QueueTimer���� ����� �ּ��ֱⰪ ����(1msec)
	timeEndPeriod(1);


    if (s != NULL) delete s;

    return 0;
}

#else

int main(int argc, char* argv[])
{

	system_main(argc, argv);

	return 0;
}

#endif
