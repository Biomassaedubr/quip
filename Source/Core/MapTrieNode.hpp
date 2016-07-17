#pragma once

#include "Key.hpp"

#include <functional>
#include <map>
#include <memory>

namespace quip {
  struct EditContext;
  
  // A callback for handling mapped commands.
  typedef std::function<void (EditContext &)> MapHandler;
  
  // A node in a MapTrie.
  //
  // A trie node contains children and a map handler. Both are optional, and it
  // is valid for a node to both at the same time.
  struct MapTrieNode {
    bool hasChildren () const;
    bool hasHandler () const;

    MapTrieNode * findChild (Key key);
    MapTrieNode * findOrAddChild (Key key);
    
    MapHandler handler () const;
    void setHandler (MapHandler handler);
    
  private:
    std::map<Key, std::unique_ptr<MapTrieNode>> m_children;
    MapHandler m_handler;
  };
}