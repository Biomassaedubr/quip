#pragma once

#include "Script.hpp"

#include <map>
#include <string>
#include <vector>

namespace quip {
  struct ScriptHost;
  
  struct FileType {
    std::string name;
    Script syntax;
  };
  
  struct FileTypeDatabase {
    FileTypeDatabase(ScriptHost& scriptHost);
    
    void registerFileType(const std::string& displayName, const std::string& canonicalName, const std::vector<std::string>& extensions);
    
    const FileType* lookupByExtension(const std::string& extension) const;
    
  private:
    ScriptHost& m_scriptHost;
    
    FileType m_unknownFileType;
    std::vector<std::unique_ptr<FileType>> m_knownTypes;
    std::map<std::string, FileType*> m_knownExtensions;
  };
}
