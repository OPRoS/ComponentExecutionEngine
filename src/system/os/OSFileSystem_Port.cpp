/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSFileSystem.cpp
 *
 */

#include "OSFileSystem.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <malloc.h>
#include <string.h>

#ifdef OS_WIN32

#include <direct.h>
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>

#else // OS_LINUX

#include <dirent.h>
#include <glob.h>
#include <unistd.h>
#define _access access
#ifndef _S_IFDIR
#define _S_IFDIR S_IFDIR
#endif
#define _rmdir ::rmdir

#endif

#include "OSFunctions.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

OSFileSystem::OSFileSystem()
{
	setup();

	m_opened = false;
}

OSFileSystem::OSFileSystem(const string &dir)
{
	setup();

	m_opened = false;
	setDir(dir);
}

OSFileSystem::~OSFileSystem()
{
}

void OSFileSystem::setup() 
{
}

void OSFileSystem::setDir(const string &dir)
{
	if (dir.size() > 0) m_dir = OS::toDir(dir);
}

string OSFileSystem::getDir()
{
	return m_dir;
}

bool OSFileSystem::open()
{
	m_opened = true;

	return true;
}

void OSFileSystem::close()
{
}

bool OSFileSystem::renameFile(const string &oldname, const string &newname)
{
	int result= ::rename(oldname.c_str() , newname.c_str());
	if ( result == 0 ) return true;
	else return false;
}

bool OSFileSystem::removeFile(const string &filename)
{
	trace("Removing " << filename);

	if(filename == "") return true;

	string fileDir;
	if (!addFilenameToDir(fileDir, filename)) return false;

	trace("File dir is " << fileDir);
	if(::remove(fileDir.c_str()) < 0) return false;

	return true;
}

bool OSFileSystem::copyFile(const string &sourceFileName, const string &destinationFileName)
{
	trace("Copying file from " << sourceFileName << " to " << destinationFileName);

	//add absolute dir
	string source;
	if (!addFilenameToDir(source, sourceFileName)) return false;

	string destination;
	if (!addFilenameToDir(destination, destinationFileName)) return false;

	FILE *fold, *fnew;
	int c;

	// open source file with binary mode
	if( ( fold = fopen( source.c_str(), "rb") ) == NULL ) {
		log_error("Couldn't open source file " << sourceFileName);
		return false;
	}

	// open destination file with binary mode
	if( ( fnew = fopen( destination.c_str(), "wb") ) == NULL ) {
		fclose( fold );

		log_error("Couldn't open  destination file " << destinationFileName);
		return false;
	}

	// read one byte from source file at once.
	// until meet the end of file
	while(1) {
		c = fgetc( fold );
		if( !feof( fold ) ) fputc( c, fnew );
		else break;
	}

	fclose( fnew );
	fclose( fold );

	return true;
}

bool OSFileSystem::existsFile(const string &filename)
{
	trace("Checking for existence of " << filename);

	if( filename == "" ) {
		log_error("Invalid file name.");
		return false;
	}

	//add absolute dir
	string fileDir;
	if (!addFilenameToDir(fileDir, filename)) return false;

	//test for existence of source file
	// 00 : Existence only, 02 : Write-only, 04 : Read-only, 06 : Read and write
	if( _access( fileDir.c_str() , 0 ) < 0 ) return false;
	else return true;
}

bool OSFileSystem::listFiles(vector<string> &listArray, const string &pattern, bool includeDir)
{
	trace("Listing files which match the pattern " << pattern);

	string wildcardPattern;

	//Does the pattern include a wildcard?
	string fileDir = "";
	bool patternWithWildcards = (pattern.find( '*' ) != pattern.npos)
						|| (pattern.find( '?' ) != pattern.npos);
	if( !patternWithWildcards ) {
		trace("no wild cards!!");
		//add absolute dir
		if (!addFilenameToDir(fileDir, pattern)) return false;

		//is it directory?
		struct stat statbuf;
		if( stat( fileDir.c_str(), &statbuf ) < 0 ) {
			string msg = "lstat failed for " + fileDir;
			log_error(msg);
			return false;
		}
		if(statbuf.st_mode == _S_IFDIR) {
			trace("directory!!");
			//add absolute dir
			fileDir = OS::toDir(fileDir);
			wildcardPattern = "*";
		}
	}
	else {
		fileDir = getDir();
		wildcardPattern = pattern;
	}

	return listFiles(listArray, fileDir, pattern, includeDir);
}

bool OSFileSystem::listFiles(vector<string> &listArray, const string &dir, const string &pattern, bool includeDir)
{
	string fileDir = dir + pattern;

	trace("list pattern : dir=" << dir << ";pattern=" << pattern << ";fileDir=" << fileDir);

#ifdef OS_WIN32
	WIN32_FIND_DATAA wfd = {0};

	HANDLE hFind = FindFirstFileA(fileDir.c_str(), &wfd);
	if(INVALID_HANDLE_VALUE == hFind) {
		return false;
	}

    BOOL bFound =  TRUE;
	while(bFound) {
		if (strcmp((const char *)wfd.cFileName, ".") == 0 || strcmp((const char *)wfd.cFileName, "..") == 0) {
		    bFound =  FindNextFileA(hFind, &wfd);
			continue;
		}

		if (wfd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {
			string filename((const char *)wfd.cFileName);
			listArray.push_back(filename);
			trace("adding files : " << wfd.cFileName);
         }
		else if (includeDir) {
			string filename((const char *)wfd.cFileName);
			string dir = OS::toDir(filename);
			listArray.push_back(dir);		
			trace("adding dir : " << dir);
		}

	    bFound =  FindNextFileA(hFind, &wfd);
	}
	FindClose(hFind);
	
#else // OS_LINUX

	int ret;
	int num;

	glob_t globbuf;
	globbuf.gl_pathc = 0;
	globbuf.gl_pathv = NULL;
	globbuf.gl_offs = 0;
	ret = glob( fileDir.c_str(), GLOB_DOOFFS | GLOB_MARK, NULL, &globbuf );

	if( ret != 0 && ret != GLOB_NOMATCH) {
		string msg = "glob failed for " + fileDir;
		log_error(msg);
		return false;
	}

	if(ret == GLOB_NOMATCH) num = 0;
	else num = globbuf.gl_pathc;

	for( int i = 0; i < num; i++ ) {
		string fileName;
		string temp = (char*)((globbuf.gl_pathv)[i]);

		size_t lpos = temp.find_last_of(OS::fileSeparator());
		size_t fpos = temp.find_first_of(OS::fileSeparator());
		if (lpos != string::npos) {
			if (temp.at(temp.size()-1) == OS::fileSeparator()) {
				if (lpos == fpos) { // "dir/"
					listArray.push_back(temp);
				}
				else { // "/abc/def/"
					int j = temp.size()-2;
					for (; j >= 0; --j) {
						if (temp.at(j) == OS::fileSeparator()) break;
					}
					if (j >= 0) {
						fileName = temp.substr(j+1);
						listArray.push_back(fileName);
					}
				}
			}
			else {
				fileName = temp.substr(lpos+1);
				listArray.push_back(fileName);
			}
			trace("<listFiles> " << (i+1) << " : " << fileName);
		}
		else {
			listArray.push_back(temp);
			trace("<listFiles> " << (i+1) << " : " << temp);
		}

	}

	//frees the dynamically allocated storage for glob
	globfree( &globbuf );

#endif

	return true;
}

bool OSFileSystem::findFiles(vector<string> &listArray, const string &pattern, bool recursive)
{
	trace("isting files which match the pattern " << pattern);

	string wildcardPattern;

	//Does the pattern include a wildcard?
	string fileDir = "";
	bool patternWithWildcards = (pattern.find( '*' ) != pattern.npos)
						|| (pattern.find( '?' ) != pattern.npos);
	if( !patternWithWildcards ) {
		trace("no wild cards!!");
		//add absolute dir
		if (!addFilenameToDir(fileDir, pattern)) return false;

		//is it directory?
		struct stat statbuf;
		if( stat( fileDir.c_str(), &statbuf ) < 0 ) {
			string msg = "lstat failed for " + fileDir;
			log_error(msg);
			return false;
		}
		if(statbuf.st_mode == _S_IFDIR) {
			trace("directory!!");
			//add absolute dir
			fileDir = OS::toDir(fileDir);
			wildcardPattern = "*";
		}
	}
	else {
		fileDir = getDir();
		wildcardPattern = pattern;
	}

	return findFiles(listArray, fileDir, pattern, recursive);
}

bool OSFileSystem::findFiles(vector<string> &listArray, const string &dir, const string &pattern, bool recursive)
{
	string fileDir = dir + pattern;

	trace("list pattern : " << fileDir);

#ifdef OS_WIN32
	WIN32_FIND_DATA wfd = {0};
#ifdef _MSC_VER
	#ifdef UNICODE
	HANDLE hFind = FindFirstFile((LPCWSTR) fileDir.c_str(), &wfd);
	#else
	HANDLE hFind = FindFirstFile((LPCSTR) fileDir.c_str(), &wfd);
	#endif
#else
	HANDLE hFind = FindFirstFile(fileDir.c_str(), &wfd);
#endif


	if(INVALID_HANDLE_VALUE == hFind) return false;

    BOOL bFound =  TRUE;
	while(bFound) {
		if (strcmp((const char *)wfd.cFileName, ".") == 0 || strcmp((const char *)wfd.cFileName, "..") == 0) {
		    bFound =  FindNextFile(hFind, &wfd);
			continue;
		}

		if (wfd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
			trace("finding directory : " << " : " << wfd.cFileName);

			if (recursive) {
				string filename((const char *)wfd.cFileName);
				string fullDir = OS::toDir(dir + filename);
				findFiles(listArray, fullDir, pattern, recursive);
			}
         }

		// wfd.cFilename
		// listArray full dir
		string filename((const char *)wfd.cFileName);
		string tmp = dir + filename;
		listArray.push_back(tmp);
		trace("adding files : " << tmp);

	    bFound =  FindNextFile(hFind, &wfd);
	}
	FindClose(hFind);

#else // OS_LINUX

	int ret;
	int num;

	glob_t globbuf;
	globbuf.gl_pathc = 0;
	globbuf.gl_pathv = NULL;
	globbuf.gl_offs = 0;
	ret = glob( fileDir.c_str(), GLOB_DOOFFS | GLOB_MARK, NULL, &globbuf );

	if( ret != 0 && ret != GLOB_NOMATCH) {
		log_error("glob failed for " + fileDir);
		return false;
	}

	if(ret == GLOB_NOMATCH) num = 0;
	else num = globbuf.gl_pathc;

	for( int i = 0; i < num; i++ ) {
		string tmp = dir + (char*)((globbuf.gl_pathv)[i]);
		listArray.push_back(tmp);
		trace(" " << (i+1) << " : " << tmp);
	}

	//frees the dynamically allocated storage for glob
	globfree( &globbuf );

#endif

	return true;
}

bool OSFileSystem::createFile(OSFile &file, const string &filename)
{
	trace("Creating file  " << filename);

	if( existsFile( filename ) ) {
		string msg = "File " + filename + " already exist.";
		log_error(msg);
		return false;
	}

	return openFileWithFlag( file, filename , "wb+");
}

bool OSFileSystem::openFile(OSFile &file, const string &filename, const string &flag)
{
	trace("Opening file " << filename);

	if( !existsFile( filename ) ) {
		string msg = "File " + filename + " doesn't exist.";
		log_error(msg);
		return false;
	}

	return openFileWithFlag(file, filename, flag);
}

bool OSFileSystem::openFileWithFlag(OSFile &file, const string &filename, const string &flag)
{
	try {
		//add absolute dir
		string fileDir;
		if (!addFilenameToDir(fileDir, filename)) return false;

		trace("Opening file " << fileDir);
		file.setFileName(filename);
		file.setDir(fileDir);
		file.setFlag(flag);

		FILE *fh = ::fopen(fileDir.c_str(), flag.c_str());
		file.setHandle(fh);
		if (fh == NULL) {
			return false;
		}
		return true;
	}
	catch(...) {
		string msg = "Exception occured.";
		log_error(msg);
		return false;
	}

	return false;
}

bool OSFileSystem::mkdir(const string &directoryName)
{
	trace_enter_msg("Creating directory " << directoryName);

	if( directoryName == "" ) {
		string msg = "Invalid directory name " + directoryName;
		log_error(msg);
		return false;
	}

	if(existsFile(directoryName)) {
		string msg = "Directory " +  directoryName + " already exists";
		log_error(msg);
		return false;
	}

	//add absolute dir
	string dirName;
	if (!addFilenameToDir(dirName, directoryName)) return false;

	//make directory
	//read/write/search permissions for owner and group, and with read/search permissions for others.
	int status = 0;

#ifdef OS_WIN32
	status = ::_mkdir(dirName.c_str());
#else
	status = ::mkdir( dirName.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
#endif

	if( status < 0 ) {
		string msg = "mkdir for " + directoryName + " failed";
		log_error(msg);
		return false;
	}

	return true;
}

bool OSFileSystem::rmdir(const string &directoryName)
{
	trace_enter_msg("Removing directory " << directoryName);

	if( directoryName == "" ) {
		string msg = "Invalid directory name " + directoryName;
		log_error(msg);
		return false;
	}

	 //add absolute dir
	string dirName;
	if (!addFilenameToDir(dirName, directoryName)) return false;

	struct stat statbuf;
	if( stat(dirName.c_str() , &statbuf) < 0 ) {
		string msg = "lstat failed for " + directoryName;
		log_error(msg);
		return false;
	}
	if(statbuf.st_mode == _S_IFDIR) {
		string msg = directoryName + " is not a directory";
		log_error(msg);
		return false;
	}

	//remove directory
	int status = _rmdir(dirName.c_str()) ;

	if( status < 0 ) {
		string msg = "rmdir for " + directoryName + " failed. ";
		msg += "Native filename was " + dirName;
		log_error(msg);
		return false;
	}

	return true;
}

bool OSFileSystem::rmdir_recusive(const string &dirName)
{
#ifdef OS_WIN32
	//relative dir
	return delete_dir(dirName);
#else
	//add absolute dir
	string absDirName = OS::toDir(getDir() + dirName);
	return delete_dir(absDirName);
#endif
}

#ifdef OS_WIN32

bool OSFileSystem::delete_dir(const string &dirName)
{
	trace_enter_msg("Removing directory " << dirName);

	if( dirName == "" ) {
		log_error("Invalid directory name : dir.name=" << dirName);
		return false;
	}

	 //add absolute dir
	string absDirName = OS::toDir(getDir() + dirName);

	vector<string> list;
	if (listFiles(list, absDirName, "*", true) == false) return false;

	for (int i = 0; i < list.size(); ++i) {
		string fn = OS::toDir(dirName) + list[i];
		if (list[i].at(list[i].size()-1) == OS::fileSeparator()) {
			delete_dir(fn);
		}
		else {
			removeFile(fn);
		}
	}

	/*
	struct stat statbuf;
	if( stat(dirName.c_str() , &statbuf) < 0 ) {
		log_error("lstat failed for " << directoryName);
		return false;
	}
	if(statbuf.st_mode == _S_IFDIR) {
		log_error(directoryName << " is not a directory");
		return false;
	}
	*/

	int status = _rmdir(absDirName.c_str()) ;
	if( status < 0 ) {
		log_error("err_status=" << status << ",dir=" << dirName << ",abs_dir=" << absDirName);
		return false;
	}

	return true;
}

#else // OS_LINUX

bool OSFileSystem::delete_dir(const string &absDirName)
{
	const char *path = absDirName.c_str();
	DIR *d = opendir(path);
	size_t path_len = strlen(path);
	int r = -1;
	if (d) {
		struct dirent *p;
		r = 0;
		while (!r && (p=readdir(d))) {
			int r2 = -1;
			char *buf;
			size_t len;

			/* Skip the names "." and ".." as we don't want to recurse on them. */
			if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
				continue;
			}

			len = path_len + strlen(p->d_name) + 2;
			buf = (char *) malloc(len);
			if (buf)           {
				struct stat statbuf;
				snprintf(buf, len, "%s/%s", path, p->d_name);
				if (!stat(buf, &statbuf)) {
					if (S_ISDIR(statbuf.st_mode)) {
						r2 = delete_dir(buf);
					}
					else {
						r2 = unlink(buf);
					}
				}
				free(buf);
			}
			r = r2;
		}
		closedir(d);
	}

	if (!r) {
		r = _rmdir(path);
		if (r !=0) {
			return false;
		}
	}
	else {
		return false;
	}

	return true;
}

#endif

bool OSFileSystem::addFilenameToDir(string &absoluteDir, const string &dir)
{
	absoluteDir = getDir() + dir;

	return true;
}

