#pragma once

#include "CursorFlags.hpp"
#include "CursorStyle.hpp"
#include "Key.hpp"
#include "KeySequence.hpp"
#include "MapTrie.hpp"

#include <functional>
#include <map>

namespace quip {
  struct EditContext;
  
  // An operational state.
  struct Mode {
    Mode ();
    virtual ~Mode ();
    
    virtual CursorStyle cursorStyle () const;
    virtual CursorFlags cursorFlags () const;

    virtual std::string status () const;
    
    bool processKeyEvent (Key key, EditContext & context);
    bool processKeyEvent (Key key, const std::string & text, EditContext & context);
    
    void enter (EditContext & context, std::uint64_t how);
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
    
    virtual bool allowsRepeats () const;
    virtual bool allowsCounts () const;
    
    virtual void onEnter (EditContext & context, std::uint64_t how);
    virtual void onExit (EditContext & context);
    
    virtual bool onUnmappedKey (Key key, const std::string & text, EditContext & context);
    
  private:
    KeySequence m_sequence;
    KeySequence m_previousSequence;
    std::uint32_t m_count;
    
    MapTrie m_mappings;
  };
}