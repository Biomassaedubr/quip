#include "MapTrieNode.hpp"

namespace quip {
  bool MapTrieNode::hasChildren () const {
    return m_children.size() > 0;
  }
  
  bool MapTrieNode::hasHandler () const {
    return m_handler != nullptr;
  }
  
  MapTrieNode * MapTrieNode::findChild (Key key) {
    std::map<Key, std::unique_ptr<MapTrieNode>>::iterator cursor = m_children.find(key);
    if (cursor == std::end(m_children)) {
      return nullptr;
    }
    
    return cursor->second.get();
  }
  
  MapTrieNode * MapTrieNode::findOrAddChild (Key key) {
    MapTrieNode * child = findChild(key);
    if (child == nullptr) {
      std::pair<std::map<Key, std::unique_ptr<MapTrieNode>>::iterator, bool> result = m_children.emplace(key, std::make_unique<MapTrieNode>());
      child = result.first->second.get();
    }
    
    return child;
  }
  
  MapHandler MapTrieNode::handler () const {
    return m_handler;
  }
  
  void MapTrieNode::setHandler (MapHandler handler) {
    m_handler = handler;
  }
}