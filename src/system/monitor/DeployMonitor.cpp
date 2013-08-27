/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : DeployMonitor.cpp
 *
 */


#include "DeployMonitor.h"


#include "system/Registry.h"
#include "system/SystemTypes.h"
#include "system/ComponentEngine.h"

#include "system/config/ConfigLoaderFactory.h"
#include "system/util/Properties.h"
#include "system/util/StringUtil.h"
#include "system/util/BasicTypes.h"

#include "system/profile/ComponentProfile.h"
#include "system/cm/ComponentManager.h"
#include "system/cm/ComponentAdapter.h"
#include "system/cm/ComponentAdapterAtomic.h"
#include "system/deploy/PackageDeployer.h"

#include "Component.h"

#include "system/Log.h"
#include "system/Trace.h"

#include "system/os/OSSetting.h"

#define FILE_BUF_SIZE 1024

using namespace std;

DeployMonitor::DeployMonitor()
{
	m_fs.open();
}

DeployMonitor::~DeployMonitor()
{
}

bool DeployMonitor::doCommand(MonitorWorker *mw, Properties &props)
{
	try {
		string cmd = props.getProperty("cmd");
		if (cmd == "file.download") {
			return fileDownload(mw, props);
		}
		else if (cmd == "file.upload") {
			return fileUpload(mw, props);
		}
		else {
			mw->returnFail("deploy", props.getProperty("cmd", "unknown"),"Invalid command");
		}
	}
	catch (Exception &ex) {
		log_error(ex);
		mw->returnFail("deploy", props.getProperty("cmd", "unknown"),ex.what());
	}

	return true;
}

string DeployMonitor::createDeployPath(const string &filename)
{
	vector<string> dirList;
	string dir;

	string fn;
	char fs = OS::fileSeparator();
	for (size_t i = 0; i < filename.size(); i++) {
		char ch = (char) filename[i];
		if ('/' == ch || '\\' == ch) {
			fn += fs;
			dirList.push_back(dir);
			dir.clear();
		}
		else
		{
			fn += ch;
			dir += ch;
		}
	}

	string deployDir = Registry::getRegistry()->getDeployDir();
	string tmpDir = deployDir;
	for (size_t i = 0; i < dirList.size(); ++i) {
		tmpDir += dirList[i];
		if(!m_fs.existsFile(tmpDir)) 	m_fs.mkdir(tmpDir);
		tmpDir += fs;
	}

	return  deployDir + fn;
}

string DeployMonitor::getDeployPath(const string &filename)
{
	vector<string> dirList;
	string dir;

	string fn;
	char fs = OS::fileSeparator();
	for (size_t i = 0; i < filename.size(); i++) {
		char ch = (char) filename[i];
		if ('/' == ch || '\\' == ch) {
			fn += fs;
			dirList.push_back(dir);
			dir.clear();
		}
		else {
			fn += ch;
			dir += ch;
		}
	}

	string deployDir = Registry::getRegistry()->getDeployDir();
	return  deployDir + fn;
}

bool DeployMonitor::fileDownload(MonitorWorker *mw, Properties &props)
{
	trace_enter();

	string fileName = props.getProperty("file.name");
	int fileSize = props.getInt("payloadSize");
	int rSize;
	unsigned char buf[FILE_BUF_SIZE];

	string filePath = createDeployPath(fileName);
	string mode("wb");
	OSFile f;

	trace("[fileDownload] filepath=" << filePath << ", fileSize==" << fileSize);

	if (f.open(filePath, mode) == false) {
		trace("File open error");
		mw->returnFail("deploy", "file.download", "File open error");
		return false;
	}

	int wlen = 0;
	try {
		while (fileSize > 0) {
			rSize = fileSize > FILE_BUF_SIZE ? FILE_BUF_SIZE : fileSize;
			rSize = mw->read(buf, rSize);
			wlen += f.write(buf, rSize);

			fileSize -= rSize;
		}

		mw->returnOK("deploy", "file.download", "ok");
		f.close();

		trace("[fileDownload] End : fileSize=" << fileSize << ", writeSize=" << wlen);

		return true;
	}
	catch (exception &ex) {
		mw->returnFail("deploy", "file.download", ex.what());
	}

	f.close();
	return false;
}

bool DeployMonitor::fileUploadTwoWay(MonitorWorker *mw, Properties &props)
{
	trace_enter();

	string fileName = props.getProperty("file.name");
	string filePath = getDeployPath(fileName);
	trace("filepath=" << filePath);

	OSFile f;
	string mode("rb");
	if (f.open(filePath, mode) == false) {
		mw->returnFail("deploy", "file.download", "File open error");
		return false;
	}

	unsigned char buf[FILE_BUF_SIZE];
	int fileSize;
	int rSize;

	fileSize = f.sizeOf();

	trace("[fileUpload] filepath=" << filePath << ", fileSize=" << fileSize);

	int wlen = 0;
	try {
		while (fileSize > 0) {
			rSize = fileSize > FILE_BUF_SIZE ? FILE_BUF_SIZE : fileSize;

			rSize = f.read(buf, rSize);
			if (rSize <= 0) break;

			mw->writeString("ver=1.0;target=deploy;cmd=file.upload;success=ok;ret=ok;payloadSize=" + StringUtil::intToStr(rSize) + "\n");

			wlen += mw->write(buf, rSize);
			fileSize -= rSize;

			trace("[fileUpload] Write done : fileSize=" << fileSize << ", writeSize=" << wlen);

			mw->readLine((char *) buf, FILE_BUF_SIZE);
		}
		f.close();

		mw->writeString("ver=1.0;target=deploy;cmd=file.upload;success=ok;ret=ok;payloadSize=0\n");
		trace("[fileUpload] End : fileSize=" << fileSize << ", writeSize=" << wlen);

		return true;
	}
	catch (exception &ex) {
		log_error(ex.what());
	}

	f.close();
	return false;
}

bool DeployMonitor::fileUpload(MonitorWorker *mw, Properties &props)
{
	trace_enter();

	string fileName = props.getProperty("file.name");
	string filePath = getDeployPath(fileName);
	trace("filepath=" << filePath);

	OSFile f;
	string mode("rb");
	if (f.open(filePath, mode) == false) {
		mw->returnFail("deploy", "file.download", "File open error");
		return false;
	}

	unsigned char buf[FILE_BUF_SIZE];
	int fileSize;
	int rSize;

	fileSize = f.sizeOf();

	trace("[fileUpload] filepath=" << filePath << ", fileSize=" << fileSize);

	mw->writeString("ver=1.0;target=deploy;cmd=file.upload;success=ok;ret=ok;payloadSize=" + StringUtil::intToStr(fileSize) + "\n");

	int wlen = 0;
	try {
		while (fileSize > 0) {
			rSize = fileSize > FILE_BUF_SIZE ? FILE_BUF_SIZE : fileSize;

			rSize = f.read(buf, rSize);
			if (rSize <= 0) break;

			wlen += mw->write(buf, rSize);
			fileSize -= rSize;
		}
		f.close();

		trace("[fileUpload] End : fileSize=" << fileSize << ", writeSize=" << wlen);

		return true;
	}
	catch (exception &ex) {
		log_error(ex.what());
	}

	f.close();
	return false;
}
