#include "FileTypeDatabase.hpp"

#include "ScriptHost.hpp"

namespace quip {
  FileTypeDatabase::FileTypeDatabase(ScriptHost& scriptHost)
  : m_scriptHost(scriptHost) {
    m_unknownFileType.name = "?";
    m_unknownFileType.syntax = m_scriptHost.getScript(m_scriptHost.scriptRootPath() + "/syntax/text.lua");
  }
  
  void FileTypeDatabase::registerFileType(const std::string& displayName, const std::string& canonicalName, const std::vector<std::string>& extensions) {
    m_knownTypes.emplace_back(std::make_unique<FileType>());
    
    FileType* type = m_knownTypes.back().get();
    type->name = displayName;
    type->syntax = m_scriptHost.getScript(m_scriptHost.scriptRootPath() + "/syntax/" + canonicalName + ".lua");
    
    for (const std::string & extension : extensions) {
      m_knownExtensions.emplace(extension, type);
    }
  }
  
  const FileType* FileTypeDatabase::lookupByExtension(const std::string& extension) const {
    std::map<std::string, FileType*>::const_iterator cursor = m_knownExtensions.find(extension);
    if (cursor != std::end(m_knownExtensions)) {
      return cursor->second;
    }
    
    return &m_unknownFileType;
  }
}
