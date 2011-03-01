/*
 *  goDirList.h
 *
 *  Created by gameover (Matthew Gingold) on 22/03/10.
 *
 */

#ifndef __GO_DIR_LIST_H
#define __GO_DIR_LIST_H

#include "ofMain.h"
#include "goUtils.h"

#include <map>
#include <queue>

#include "Poco/DirectoryIterator.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"

using Poco::DirectoryIterator;
using Poco::File;
using Poco::Path;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;

typedef struct
{
    string	name;
    string	path;
    string	modified;
    string	created;
    int		size;
} file_structure;

class goDirList
{

public:

    goDirList()
    {
        verbose		= false;
    };
    ~goDirList() {};

    void	setVerbose(bool _verbose);
    string	getName(int pos);
    string	getPath(int pos);
    string	getCreated(int pos);									// returns date created
    string	getModified(int pos);									// returns date modified
    int		getSize(int pos);										// returns size in bytes
    int     getNumberOfDirectories();
    void    getFilesByDirectory(int pos, vector<string> * _files);
    void    getFilesByDirectory(string _directory, vector<string> * _files);
    void	reset();												// resets extension list
    void	allowExt(string ext);									// add an extension
    void	addDir(string _directory);								// add directories to iterate through
    int		listDir(bool _recurse);									// recurse (using multiple added directories)
    int		listDir(string _directory);								// returns number of files found
    int		listDir(string _directory, bool _recurse);				// recurse folders
    int		size();

    multimap<string, file_structure>        filesByDirectory;
    vector<file_structure>		            files;
    vector<string>                          directoriesWithFiles;

private:

    bool	verbose;
    bool	recurse;

    queue<string>				directories;
    vector<string>				allowedExt;

    void	checkNodes();
    int		doListing();

    bool    isNewDirectory(string _directory);

};

#endif
