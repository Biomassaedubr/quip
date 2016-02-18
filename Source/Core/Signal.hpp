#pragma once

#include <functional>

namespace quip {
  template<typename> struct Signal;
  
  template<typename ReturnType, typename ... ArgumentTypes>
  struct Signal<ReturnType (ArgumentTypes...)> {
    typedef std::function<ReturnType (ArgumentTypes...)> HandlerType;
    
    Signal () noexcept
    : m_head(nullptr)
    , m_tail(nullptr) {
    }
    
    ~Signal () {
      HandlerNode * cursor = m_head;
      while (cursor != nullptr) {
        HandlerNode * previous = cursor;
        cursor = cursor->next;
        
        delete previous;
      }
    }
    
    void connect (const HandlerType & handler) {
      if (m_head == nullptr) {
        m_head = new HandlerNode(handler);
        m_tail = m_head;
      } else {
        m_tail->next = new HandlerNode(handler);
        m_tail = m_tail->next;
      }
    }
    
    ReturnType transmit (ArgumentTypes ... arguments) {
      HandlerNode * cursor = m_head;
      while (cursor != nullptr) {
        if (cursor == m_tail) {
          return cursor->handler(std::forward<ArgumentTypes>(arguments)...);
        }
        
        cursor->handler(std::forward<ArgumentTypes>(arguments)...);
        cursor = cursor->next;
      }
      
      return ReturnType();
    }
    
  private:
    struct HandlerNode {
      HandlerNode (const HandlerType & handler) noexcept
      : handler(handler)
      , next(nullptr) {
      }
      
      HandlerType handler;
      HandlerNode * next;
    };
    
    HandlerNode * m_head;
    HandlerNode * m_tail;
  };
}