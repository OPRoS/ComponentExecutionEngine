/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSFileSystem.h
 *
 */

#ifndef _OS_FILE_SYSTEM_H_
#define _OS_FILE_SYSTEM_H_

#include <cstdio>

#include "OSFile.h"

/**
	파일 시스템 관련 함수 래핑 클래스.
*/
class OSFileSystem {
protected:
	std::string  m_dir;
	bool m_opened;

private:

	bool isAbsolute(const std::string  &dir);
	bool containCharInString(char c, const std::string  &str);
	bool checkIfValidAndAbsolutePattern(const std::string  &pattern);
	bool checkIfValidAndAbsoluteDirname(const std::string  &dirname);
	bool validateDirname(const std::string  &dirname, const std::string  &validChar);
	bool validateFirstCharIsNotHyphen(const std::string  &dirname);
	bool openFileWithFlag(OSFile &file, const std::string  &filenane, const std::string  &flag);
	bool addFilenameToDir(std::string  &absoluteDir, const std::string  &dir);

	void setup();

public:
	OSFileSystem();
	OSFileSystem(const std::string  &dir);
	virtual ~OSFileSystem();

	bool open();
	void close();
	bool isOpen() { return m_opened; };

	void setDir(const std::string  &dir);
	std::string  getDir();
	std::string  toDir(const std::string  &dir);

	virtual bool createFile(OSFile &file, const std::string  &fileName);
	virtual bool openFile(OSFile &file, const std::string  &fileName, const  std::string  &flag);

	virtual bool renameFile(const std::string  &oldname, const std::string  &newname);
	virtual bool removeFile(const std::string  &filename);
	virtual bool copyFile(const std::string  &sourceFileName, const std::string  &destinationFileName);
	virtual bool existsFile(const std::string  &filename);

	virtual bool listFiles(std::vector<std::string> &listArray, const std::string  &pattern, bool includeDir);
	virtual bool listFiles(std::vector<std::string> &listArray, const std::string  &dir, const std::string  &pattern, bool includeDir);

	virtual bool findFiles(std::vector<std::string> &listArray, const std::string  &pattern,  bool recursive = false);
	virtual bool findFiles(std::vector<std::string> &listArray, const std::string  &dir, const std::string  &pattern, bool recursive = false);

	virtual bool mkdir(const std::string  &directoryName);
	virtual bool rmdir(const std::string  &directoryName);
	virtual bool rmdir_recusive(const std::string &directoryName);
	virtual bool delete_dir(const std::string &directoryName);
};

#endif 
