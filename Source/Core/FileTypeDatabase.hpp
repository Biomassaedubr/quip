#pragma once

#include <map>
#include <string>
#include <vector>

namespace quip {
  struct FileType {
    std::string name;
  };
  
  struct FileTypeDatabase {
    FileTypeDatabase ();
    
    void registerFileType (const std::string & name, const std::vector<std::string> & extensions);
    
    const FileType* lookupByExtension (const std::string & extension) const;
    
  private:
    FileType m_unknownFileType;
    std::vector<std::unique_ptr<FileType>> m_knownTypes;
    std::map<std::string, FileType*> m_knownExtensions;
  };
}