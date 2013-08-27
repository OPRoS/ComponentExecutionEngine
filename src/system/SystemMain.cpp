/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 9
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : SystemMain.cpp
 *
 */

#include <iostream>
#include <string>
#ifdef SUPPORT_ROS
#include <ros/ros.h>	
#endif

#include "system/ComponentEngineLoader.h"

//#define METHOD_TEST 1
#ifdef METHOD_TEST
extern int test_main();
#endif

using namespace std;

#ifdef _USRDLL
extern "C"
{
__declspec(dllexport) int system_main(int argc, char *argv[]);
}
#endif

int system_main(int argc, char *argv[])
{
#ifdef METHOD_TEST
	if (METHOD_TEST) {
		test_main();
	}
	else { //
#endif
//		int my_argc = 2;
//		char *my_argv[100] = { "app", "engine.path=.." };

//		char *name = argv[0];

//		std::string str = name;


//		cout <<  "PATH:" << str << "\n";

#ifdef SUPPORT_ROS
		int targ = 0;
		ros::init(targ, 0, "opros");
#endif

		ComponentEngineLoader loader;
		loader.load(argc, argv);
#ifdef METHOD_TEST
	}
#endif

#ifdef SUPPORT_ROS
		ros::shutdown();
#endif

	cout << "\n\nbye..." << endl;

	return 0;
}

#ifndef _MSC_VER
int main(int argc, char *argv[])
{
	return system_main(argc,argv);
}
#endif
