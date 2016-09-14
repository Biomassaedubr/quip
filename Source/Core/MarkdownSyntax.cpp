#include "MarkdownSyntax.hpp"

#include "Document.hpp"
#include "Syntax.hpp"

#include "pegtl.hh"

namespace quip {
  namespace MarkdownSyntax {
    using namespace pegtl;
    
    struct Header : seq<plus<one<'#'>>, star<any>> {};
    
    struct File : until<eof, sor<
      Header,
      any
    >> {};
    
    template<typename Rule>
    struct Action : nothing<Rule> {};
    
    template<>
    struct Action<Header> {
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