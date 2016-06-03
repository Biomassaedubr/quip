#pragma once

#include "CursorFlags.hpp"
#include "CursorStyle.hpp"
#include "Key.hpp"
#include "KeyStroke.hpp"

#include <functional>
#include <map>

namespace quip {
  struct EditContext;
  
  struct Mode {
    typedef std::function<void (EditContext &)> MapCallback;
    
    virtual ~Mode ();
    
    virtual CursorStyle cursorStyle () const;
    virtual CursorFlags cursorFlags () const;

    virtual std::string status () const;
    
    bool processKey (const KeyStroke & keyStroke, EditContext & context);
    
  protected:
    template<typename ModeType>
    void addMapping (Key key, void (ModeType::* callback)(EditContext &)) {
      MapCallback bound = std::bind(callback, static_cast<ModeType *>(this), std::placeholders::_1);
      m_mappings.insert(std::make_pair(key, bound));
    }
    
    virtual bool onUnmappedKey (const KeyStroke & keyStroke, EditContext & context);
    
  private:
    std::map<Key, MapCallback> m_mappings;
  };
}