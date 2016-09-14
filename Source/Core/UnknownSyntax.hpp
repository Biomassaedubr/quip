#pragma once

namespace quip {
  struct Syntax;
  
  // The "unknown" syntax is used as a fallback when no other grammar is appropriate for
  // a document. It is essentially a no-op parse, although for the time being it's also
  // used for testing.
  namespace UnknownSyntax {
    Syntax* get ();
  }
}