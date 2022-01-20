
#include "util.h"
#include <vector>
#include <string>
#include <set>
#include <unordered_map>

class FileSystem{
    enum class fileType {
        file,
        dir
    };
    class  File {
      public:
        File(const std::string name) : _name{name},  _type{fileType::file} {}

        using iterator = std::set<File*>::iterator;
        iterator begin() { return _files.begin();}
        iterator end() {return _files.end();}
        using const_iterator = std::set<File*>::const_iterator;
        const_iterator cbegin() const { return _files.cbegin();}
        const_iterator cend() const {return _files.cend();}

        bool operator < (const File &rhs) {
           return _name < rhs._name;
        }

        std::string   _name;
        fileType      _type;
        std::set<File *> _files;
        std::unordered_map<std::string, File *> _namesMap;
    };


    public:
    FileSystem() {
      root = new File("/");
    }


    std::vector<std::string> lsUtil(const File *file, 
                                    int index, const std::vector<std::string> &path) {
        if (index == path.size() - 1) {
            std::string currPath;
            for (const auto &name : path) {
                currPath += "/" + name;    
            }

            if (file->_type == fileType::file) {
                return {currPath};
            } else {
                std::vector<std::string> result;
                for (File::const_iterator it = file->cbegin(); it != file->cend(); ++it) {
                    result.push_back(currPath + "/" + (*it)->_name);
                }
            }
        }

        const std::string &name = path[index];
        auto it =  file->_namesMap.find(name);
        if (it != file->_namesMap.end()) {
            return lsUtil(it->second, index + 1, path);
        }
    }


    std::vector<std::string> ls(const std::string &pathStr) {
       std::vector<std::string> path = split(pathStr);
       return lsUtil(root, 0, path); 
    }

    void mkdir(const std::string &path) {
       std::vector<std::string> path = split(pathStr);
       File *currFile = root;
       for (auto & name : path) {
         auto it = _namesMap.find(name);
         if (it == _namesMap.end()) {
           File *newFile = new File(name, fileType::dir);
           _file->_files.insert(newFile);
           _namesMap[name] = newFile;
           currFile = newFile;
         } else {
          currFile = *it; 
         }
       }
    }


    private:
    File  *root;
};
