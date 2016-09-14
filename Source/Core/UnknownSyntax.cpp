#include "UnknownSyntax.hpp"

#include "Document.hpp"
#include "Syntax.hpp"

#include "pegtl.hh"

namespace quip {
  namespace UnknownSyntax {
    using namespace pegtl;
    
    struct KeywordQuip : pegtl_string_t("Quip") {};
    struct Anything : sor<KeywordQuip, any> {};
    struct File : until<eof, Anything> {};
    
    template<typename Rule>
    struct Action : nothing<Rule> {};
    
    template<>
    struct Action<KeywordQuip> {
      template<typename Input>
      static void apply (const Input & input, std::vector<AttributeRange> & results) {
        AttributeRange range(Keyword, input.byte_in_line(), input.size());
        results.push_back(range);
      }
    };
    
    struct Implementation : Syntax {
      virtual std::vector<AttributeRange> parse (const std::string & row, const std::string & context) override {
        std::vector<AttributeRange> results;
        parse_string<File, Action>(row, context, results);
        
        return results;
      }
    };
    
    Syntax * get () {
      static Implementation result;
      return &result;
    }
  }
}