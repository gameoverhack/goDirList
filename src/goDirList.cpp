/*
 *  goDirList.cpp
 *
 *  Created by gameover (Matthew Gingold) on 22/03/10.
 *
 */

#include "goDirList.h"

//------------------------------------------------
void goDirList::setVerbose(bool _verbose)
{
    verbose = _verbose;
}

//------------------------------------------------
int goDirList::size()
{
    return files.size();
}

int    goDirList::findFileByName(string needle){
	string haystack;
	for(int i = 0; i < files.size(); i++){
		haystack = getName(i);
		if(haystack == needle){
			return i;
		}
	}
	return -1;
}

//------------------------------------------------
string goDirList::getName(int pos)
{
    return files[pos].name;
}
string  goDirList::getNameWithoutExtension(int pos){
	string str = getName(pos);
	return str.substr(0, str.find_last_of("."));
	
}

//------------------------------------------------
string goDirList::getPath(int pos)
{
    return files[pos].path;
}

//------------------------------------------------
string goDirList::getCreated(int pos)
{
    return files[pos].created;
}

//------------------------------------------------
string goDirList::getModified(int pos)
{
    return files[pos].modified;
}

//------------------------------------------------
int goDirList::getSize(int pos)
{
    return files[pos].size;
}

//------------------------------------------------
int goDirList::getNumberOfDirectories()
{
    return directoriesWithFiles.size();
}

//------------------------------------------------
void goDirList::getFilesByDirectory(int pos, vector<string> * _files)
{
    multimap<string, file_structure>::iterator it;
    pair<multimap<string, file_structure>::iterator,multimap<string, file_structure>::iterator> ret;

    ret = filesByDirectory.equal_range(directoriesWithFiles.at(pos));
    for (it=ret.first; it!=ret.second; ++it)
    {
        file_structure file;
        file = (*it).second;
        _files->push_back(file.path);
        //cout << file.path << endl;
    }

}

//------------------------------------------------
void goDirList::getFilesByDirectory(string _directory, vector<string> * _files)
{
    multimap<string, file_structure>::iterator it;
    pair<multimap<string, file_structure>::iterator,multimap<string, file_structure>::iterator> ret;

    ret = filesByDirectory.equal_range(_directory);
    for (it=ret.first; it!=ret.second; ++it)
    {
        file_structure file;
        file = (*it).second;
        _files->push_back(file.path);
        //cout << file.path << endl;
    }

}

//------------------------------------------------
void goDirList::reset()
{

    files.clear();
    allowedExt.clear();
    filesByDirectory.clear();

    while (!directories.empty())
    {
        directories.pop();
    }

}

//------------------------------------------------
void goDirList::allowExt(string ext)
{

    ext = "." + ext;
    allowedExt.push_back(ext);

    // add upper/lower case conversion (will this iterate too much??)
    allowedExt.push_back(toLower(ext));
    allowedExt.push_back(toUpper(ext));
}

//------------------------------------------------
void goDirList::addDir(string _directory)
{
    directories.push(_directory);
}

//------------------------------------------------
int goDirList::listDir(bool _recurse = false)
{

    recurse = _recurse;
    return doListing();
}

//------------------------------------------------
int goDirList::listDir(string _directory)
{
    return listDir (_directory, false);
}

//------------------------------------------------
int goDirList::listDir(string _directory, bool _recurse = false)
{

    addDir(_directory);
    recurse = _recurse;

    return doListing();

}

//------------------------------------------------
int goDirList::doListing()
{

    while (directories.size() > 0)
    {

        checkNodes();
    }

    return files.size();
}

//------------------------------------------------
void goDirList::checkNodes()
{

    Path dir = directories.front().c_str();
    try
    {
        DirectoryIterator it(dir);
        DirectoryIterator end;
        while (it != end)
        {
            Path p(it->path());

            if(it->isDirectory() && p.getFileName().rfind(".") == string::npos && !it->isHidden() && !it->isLink())
            {

                if (verbose) std::cout << "Adding directory: " << it->path() << std::endl;

                if(recurse) directories.push(it->path());

            }
            else
            {

                int num_types = allowedExt.size();

                bool isAllowedType = false;

                if(num_types > 0)
                {

                    for (int i = 0; i < num_types; i++)
                    {

                        if (p.getFileName().rfind(allowedExt[i]) != string::npos) isAllowedType = true;
                    }

                }
                else isAllowedType = true;

                if(isAllowedType)
                {

                    file_structure file;

                    file.name = p.getFileName();
                    file.path = it->path();
                    file.size = it->getSize();
                    file.created = DateTimeFormatter::format(it->created(), DateTimeFormat::SORTABLE_FORMAT);
                    file.modified = DateTimeFormatter::format(it->getLastModified(), DateTimeFormat::SORTABLE_FORMAT);

                    files.push_back(file);

                    if (isNewDirectory(directories.front())) directoriesWithFiles.push_back(directories.front());
                    filesByDirectory.insert(pair<string, file_structure>(directories.front(), file));

                    if (verbose) std::cout	<< "Adding file: " << file.name
                                                << " size: " << file.size
                                                << " created: " << file.created
                                                << " modified: " << file.modified
                                                << std::endl;

                }
            }

            ++it;
        }
    }
    catch (Poco::Exception& exc)
    {
        std::cerr << exc.displayText() << std::endl;
        //return 1;
    }

    directories.pop();

}

bool goDirList::isNewDirectory(string _directory)
{
    bool isNew = true;
    for (int i = 0; i < directoriesWithFiles.size(); i++)
    {
        if (_directory == directoriesWithFiles.at(i))
        {
            isNew = false;
            break;
        }
    }
    return isNew;
}
