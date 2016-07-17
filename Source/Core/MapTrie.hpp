#pragma once

#include "MapTrieNode.hpp"

namespace quip {
  struct KeySequence;
  
  // A trie (or prefix tree) used to associate key sequences with mapped commands.
  struct MapTrie {
    MapTrie ();
    
    void insert (const KeySequence & sequence, MapHandler handler);
    
    const MapTrieNode * find (const KeySequence & sequence) const;

  private:
    std::unique_ptr<MapTrieNode> m_root;
  };
}