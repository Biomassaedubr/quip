#pragma once

#include "CursorFlags.hpp"
#include "CursorStyle.hpp"
#include "Key.hpp"
#include "KeySequence.hpp"
#include "KeyStroke.hpp"
#include "MapTrie.hpp"

#include <functional>
#include <map>

namespace quip {
  struct EditContext;
  
  // An operational state.
  struct Mode {
    virtual ~Mode ();
    
    virtual CursorStyle cursorStyle () const;
    virtual CursorFlags cursorFlags () const;

    virtual std::string status () const;
    
    bool processKey (const KeyStroke & keyStroke, EditContext & context);
    
    void enter (EditContext & context);
    void exit (EditContext & context);
    
  protected:
    template<typename ModeType>
    void addMapping (Key key, void (ModeType::* callback)(EditContext &)) {
      addMapping(KeySequence(key), callback);
    }
    
    template<typename ModeType>
    void addMapping (KeySequence sequence, void (ModeType::* callback)(EditContext &)) {
      MapHandler bound = std::bind(callback, static_cast<ModeType *>(this), std::placeholders::_1);
      m_mappings.insert(sequence, bound);
    }
    
    virtual void onEnter (EditContext & context);
    virtual void onExit (EditContext & context);
    
    virtual bool onUnmappedKey (const KeyStroke & keyStroke, EditContext & context);
    
  private:
    KeySequence m_sequence;
    std::uint32_t m_count;
    
    MapTrie m_mappings;
  };
}