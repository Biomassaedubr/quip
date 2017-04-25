#pragma once

namespace quip {
  struct EditContext;
  
  struct Transaction {
    virtual ~Transaction ();
    
    virtual void perform (EditContext & context) = 0;
    virtual void rollback (EditContext & context) = 0;
  };
}