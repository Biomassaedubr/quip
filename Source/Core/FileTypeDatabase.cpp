#include "FileTypeDatabase.hpp"

namespace quip {
  FileTypeDatabase::FileTypeDatabase() {
    m_unknownFileType.name = "?";
  }
  
  void FileTypeDatabase::registerFileType (const std::string & name, Syntax * syntax, const std::vector<std::string> & extensions) {
    m_knownTypes.emplace_back(std::make_unique<FileType>());
    
    FileType * type = m_knownTypes.back().get();
    type->name = name;
    type->syntax = syntax;
    
    for (const std::string & extension : extensions) {
      m_knownExtensions.emplace(extension, type);
    }
  }
  
  const FileType* FileTypeDatabase::lookupByExtension (const std::string & extension) const {
    std::map<std::string, FileType*>::const_iterator cursor = m_knownExtensions.find(extension);
    if (cursor != std::end(m_knownExtensions)) {
      return cursor->second;
    }
    
    return &m_unknownFileType;
  }
}