#include "MapTrie.hpp"

#include "KeySequence.hpp"

namespace quip {
  MapTrie::MapTrie ()
  : m_root(std::make_unique<MapTrieNode>()) {
  }
  
  void MapTrie::insert (const KeySequence & sequence, MapHandler handler) {
    const Key * key = sequence.begin();
    const Key * end = sequence.end();
    MapTrieNode * node = m_root.get();
    
    while(key != end) {
      MapTrieNode * child = node->findChild(*key);
      if (child == nullptr) {
        // The remainder of the sequence has no structure in the trie, add it.
        while(key != end) {
          node = node->findOrAddChild(*key);
          ++key;
        }
      } else {
        node = child;
        ++key;
      }
    }
    
    node->setHandler(handler);
  }
  
  const MapTrieNode * MapTrie::find (const KeySequence & sequence) const {
    const Key * key = sequence.begin();
    const Key * end = sequence.end();
    MapTrieNode * node = m_root.get();
    
    while (key != end) {
      node = node->findChild(*key++);
      if (node == nullptr) {
        break;
      }
    }
    
    return node;
  }
}