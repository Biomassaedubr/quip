#pragma once

#include <functional>

namespace quip {
  template<typename> struct Signal;
  
  // A basic multicast delegate. A signal can be connected to listeners of the appropriate
  // signature. When the signal transmits, all connected listeners are called in unspecified
  // order. Connected listeners are assigned a token when connected which can be used to
  // disconnect the listener at a later date. Tokens are unsigned 32-bit integers beginning
  // at one (zero can be used to indicate an invalid or unassigned token).
  template<typename ReturnType, typename... ArgumentTypes>
  struct Signal<ReturnType (ArgumentTypes...)> {
    typedef std::function<ReturnType (ArgumentTypes...)> HandlerType;
    
    Signal() noexcept
    : m_head(nullptr)
    , m_tail(nullptr)
    , m_nextToken(1) {
    }
    
    ~Signal() {
      HandlerNode* cursor = m_head;
      while(cursor != nullptr) {
        HandlerNode* previous = cursor;
        cursor = cursor->next;
        
        delete previous;
      }
    }
    
    std::uint32_t connect(const HandlerType& handler) {
      if(m_head == nullptr) {
        m_head = new HandlerNode(handler, m_nextToken);
        m_tail = m_head;
      } else {
        m_tail->next = new HandlerNode(handler, m_nextToken);
        m_tail = m_tail->next;
      }
      
      ++m_nextToken;
      return m_tail->token;
    }
    
    void disconnect(std::uint32_t token) {
      HandlerNode* prior = nullptr;
      HandlerNode* cursor = m_head;
      while(cursor != nullptr) {
        if(cursor->token == token) {
          if(prior != nullptr) {
            prior->next = cursor->next;
          } else {
            m_head = cursor->next;
          }
          
          if(cursor == m_tail) {
            m_tail = prior;
          }
          
          delete cursor;
          return;
        } else {
          prior = cursor;
          cursor = cursor->next;
        }
      }
    }
    
    ReturnType transmit(ArgumentTypes... arguments) {
      HandlerNode* cursor = m_head;
      while(cursor != nullptr) {
        if(cursor == m_tail) {
          return cursor->handler(std::forward<ArgumentTypes>(arguments)...);
        }
        
        cursor->handler(std::forward<ArgumentTypes>(arguments)...);
        cursor = cursor->next;
      }
      
      return ReturnType();
    }
    
  private:
    struct HandlerNode {
      HandlerNode(const HandlerType& handler, std::uint32_t token) noexcept
      : handler(handler)
      , token(token)
      , next(nullptr) {
      }
      
      HandlerType handler;
      std::uint32_t token;
      HandlerNode* next;
    };
    
    HandlerNode* m_head;
    HandlerNode* m_tail;
    std::uint32_t m_nextToken;
  };
}
