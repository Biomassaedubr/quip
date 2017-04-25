#include "catch.hpp"

#include "Document.hpp"
#include "Selection.hpp"
#include "SelectionSet.hpp"

using namespace quip;

TEST_CASE("Documents can be default-constructed.", "[DocumentTests]") {
  Document document;
  
  REQUIRE(document.rows() == 0);
  REQUIRE(document.isMissingTrailingNewline());
}

TEST_CASE("Documents can be constructed from a single line of text.", "[DocumentTests]") {
  Document document("ABCD");
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.contents() == "ABCD");
  REQUIRE(document.isMissingTrailingNewline());
}

TEST_CASE("Documents can be constructed from multiple lines of text.", "[DocumentTests]") {
  Document document("ABCD\nEFGH\nIJKL");
  
  REQUIRE(document.rows() == 3);
  REQUIRE(document.row(0) == "ABCD\n");
  REQUIRE(document.row(1) == "EFGH\n");
  REQUIRE(document.row(2) == "IJKL");
}

TEST_CASE("Documents can be constructed with a trailing newline.", "[DocumentTests]") {
  Document document("ABCD\n");
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.contents() == "ABCD\n");
  REQUIRE_FALSE(document.isMissingTrailingNewline());
}

TEST_CASE("Documents can tested for emptiness.", "[DocumentTests]") {
  Document empty;
  REQUIRE(empty.isEmpty());
  
  Document filled("ABCD");
  REQUIRE_FALSE(filled.isEmpty());
}

TEST_CASE("Documents can query for distance between the same location.", "[DocumentTests]") {
  Document document("ABCD");
  std::int64_t result = document.distance(Location(0, 0), Location(0, 0));
  
  REQUIRE(result == 0);
}

TEST_CASE("Documents can query for distance between increasing locations on the same row.", "[DocumentTests]") {
  Document document("ABCD");
  std::int64_t result = document.distance(Location(1, 0), Location(3, 0));
  
  REQUIRE(result == 2);
}

TEST_CASE("Documents can query for distance between decreasing locations on the same row.", "[DocumentTests]") {
  Document document("ABCD");
  std::int64_t result = document.distance(Location(3, 0), Location(1, 0));
  
  REQUIRE(result == -2);
}

TEST_CASE("Documents can query for distance between increasing locations on different rows.", "[DocumentTests]") {
  Document document("ABCD\nEFGH\nIJKL\n");
  std::int64_t result = document.distance(Location(1, 0), Location(2, 2));
  
  REQUIRE(result == 11);
}

TEST_CASE("Documents can query for distance between decreasing locations on different rows.", "[DocumentTests]") {
  Document document("ABCD\nEFGH\nIJKL\n");
  std::int64_t result = document.distance(Location(2, 2), Location(1, 0));
  
  REQUIRE(result == -11);
}

TEST_CASE("Documents can return their entire contents.", "[DocumentTests]") {
  Document document("ABCD\nEFGH\n");
  std::string result = document.contents();
  
  REQUIRE(result == "ABCD\nEFGH\n");
}

TEST_CASE("Documents can return their entire contents when empty.", "[DocumentTests]") {
  Document document;
  std::string result = document.contents();
  
  REQUIRE(result == "");
}

TEST_CASE("Documents can return the contents of a selection.", "[DocumentTests]") {
  Document document("ABCD\nEFGH\n");
  std::string result = document.contents(Selection(Location(2, 0), Location(1, 1)));
  
  REQUIRE(result == "CD\nEF");
}

TEST_CASE("Documents can return the contents of a selection when empty.", "[DocumentTests]") {
  Document document;
  std::string result = document.contents(Selection(Location(0, 0)));
  
  REQUIRE(result == "");
}

TEST_CASE("Documents can return the contents of a selection set.", "[DocumentTests]") {
  Document document("ABCD\nEFGH\nIJKL\n");
  SelectionSet selections({
    Selection(Location(3, 0), Location(0, 1)),
    Selection(Location(3, 1), Location(0, 2))
  });
  std::vector<std::string> result = document.contents(selections);
  
  REQUIRE(result.size() == 2);
  REQUIRE(result[0] == "D\nE");
  REQUIRE(result[1] == "H\nI");
}

TEST_CASE("Documents can return the contents of a selection set when empty.", "[DocumentTests]") {
  Document document;
  SelectionSet selections(Selection(Location(0, 0)));
  std::vector<std::string> result = document.contents(selections);
  
  REQUIRE(result.size() == 0);
}

TEST_CASE("Documents can have their path set.", "[DocumentTests]") {
  Document document;
  REQUIRE(document.path() == "");
  
  document.setPath("~/path.txt");
  REQUIRE(document.path() == "~/path.txt");
}

TEST_CASE("Documents can have text inserted when empty.", "[DocumentTests]") {
  Document document;
  SelectionSet result = document.insert(Selection(Location(0, 0)), "ABCD\nEFGH\n");
  
  REQUIRE(document.rows() == 2);
  REQUIRE(document.row(0) == "ABCD\n");
  REQUIRE(document.row(1) == "EFGH\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(5, 1));
}

TEST_CASE("Documents can have text inserted via a single selection.", "[DocumentTests]") {
  Document document("ABD\n");
  SelectionSet result = document.insert(Selection(Location(2, 0)), "C");
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "ABCD\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(3, 0));
}

TEST_CASE("Documents can have text inserted via multiple selections.", "[DocumentTests]") {
  Document document("ADF\n");
  std::vector<std::string> text({
    "BC",
    "E",
    "GH"
  });
  SelectionSet selections({
    Selection(Location(1, 0)),
    Selection(Location(2, 0)),
    Selection(Location(3, 0)),
    
  });
  SelectionSet result = document.insert(selections, text);
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "ABCDEFGH\n");
  REQUIRE(result.count() == 3);
  REQUIRE(result[0].origin() == Location(3, 0));
  REQUIRE(result[1].origin() == Location(5, 0));
  REQUIRE(result[2].origin() == Location(8, 0));
}

TEST_CASE("Documents can have text with leading newlines inserted via a single selection.", "[DocumentTests]") {
  Document document("ABCD\n");
  document.insert(Selection(Location(4, 0)), "\nEFGH");
  
  REQUIRE(document.rows() == 2);
  REQUIRE(document.row(0) == "ABCD\n");
  REQUIRE(document.row(1) == "EFGH\n");
}

TEST_CASE("Documents can have text with trailing newlines inserted via a single selection.", "[DocumentTests]") {
  Document document("ABCD\n");
  SelectionSet result = document.insert(Selection(Location(4, 0)), "EFGH\n");
  
  REQUIRE(document.rows() == 2);
  REQUIRE(document.row(0) == "ABCDEFGH\n");
  REQUIRE(document.row(1) == "\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(0, 1));
}

TEST_CASE("Documents can have text with internal newlines inserted via a single selection.", "[DocumentTests]") {
  Document document("AB\nKL\n");
  SelectionSet result = document.insert(Selection(Location(2, 0)), "CD\nEFGH\nIJ");
  
  REQUIRE(document.rows() == 4);
  REQUIRE(document.row(0) == "ABCD\n");
  REQUIRE(document.row(1) == "EFGH\n");
  REQUIRE(document.row(2) == "IJ\n");
  REQUIRE(document.row(3) == "KL\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(2, 2));
}

TEST_CASE("Documents can have text erased when empty.", "[DocumentTests]") {
  Document document;
  Selection selection(Location(0, 0));
  SelectionSet result = document.erase(selection);
  
  REQUIRE(document.rows() == 0);
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(0, 0));
}

TEST_CASE("Documents can have text erased via empty selections.", "[DocumentTests]") {
  Document document("ABCD\n");
  SelectionSet empty;
  document.erase(empty);
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "ABCD\n");
}

TEST_CASE("Documents can have a single character erased via a single selection.", "[DocumentTests]") {
  Document document("ABCD\n");
  SelectionSet result = document.erase(Selection(Location(1, 0)));
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "ACD\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(1, 0));
}

TEST_CASE("Documents can have a multiple characters erased via a single selection.", "[DocumentTests]") {
  Document document("ABCD\n");
  SelectionSet result = document.erase(Selection(Location(1, 0), Location(2, 0)));
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "AD\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(1, 0));
}

TEST_CASE("Documents can have a single line erased via a single selection.", "[DocumentTests]") {
  Document document("ABCD\nEFGH\n");
  SelectionSet result = document.erase(Selection(Location(0, 0), Location(4, 0)));
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "EFGH\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(0, 0));
}

TEST_CASE("Documents can have multiple lines erased via a single selection.", "[DocumentTests]") {
  Document document("ABCD\nEFGH\nHIJK\n");
  SelectionSet result = document.erase(Selection(Location(0, 0), Location(4, 1)));
  
  REQUIRE(document.row(0) == "HIJK\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(0, 0));
}

TEST_CASE("Documents can have multiple characters erased via multiple selections.", "[DocumentTests]") {
  Document document("ABCDEFGH\n");
  SelectionSet selections({
    Selection(Location(2, 0)),
    Selection(Location(4, 0), Location(5, 0)),
    Selection(Location(7, 0))
  });
  SelectionSet result = document.erase(selections);
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "ABDG\n");
  REQUIRE(result.count() == 3);
  REQUIRE(result[0].origin() == Location(2, 0));
  REQUIRE(result[1].origin() == Location(3, 0));
  REQUIRE(result[2].origin() == Location(4, 0));
}

TEST_CASE("Documents can have multiple characters erased across multiple lines via multiple selections.", "[DocumentTests]") {
  Document document("ABCDEFGH\nIJKLMNOP\n");
  SelectionSet selections({
    Selection(Location(1, 0), Location(2, 0)),
    Selection(Location(6, 0), Location(1, 1)),
    Selection(Location(5, 1), Location(6, 1))
  });
  SelectionSet result = document.erase(selections);
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "ADEFKLMP\n");
  REQUIRE(result.count() == 3);
  REQUIRE(result[0].origin() == Location(1, 0));
  REQUIRE(result[1].origin() == Location(4, 0));
  REQUIRE(result[2].origin() == Location(7, 0));
}

TEST_CASE("Documents can have multiple characters erased across multiple disjoint lines via multiple selections.", "[DocumentTests]") {
  Document document("ABCDEFGH\nIJKLMNOP\nQRSTUVWX\n");
  SelectionSet selections({
    Selection(Location(0, 0)),
    Selection(Location(2, 0), Location(4, 0)),
    Selection(Location(1, 2)),
    Selection(Location(3, 2), Location(5, 2))
  });
  SelectionSet result = document.erase(selections);
  
  REQUIRE(document.rows() == 3);
  REQUIRE(document.row(0) == "BFGH\n");
  REQUIRE(document.row(1) == "IJKLMNOP\n");
  REQUIRE(document.row(2) == "QSWX\n");
  REQUIRE(result.count() == 4);
  REQUIRE(result[0].origin() == Location(0, 0));
  REQUIRE(result[1].origin() == Location(1, 0));
  REQUIRE(result[2].origin() == Location(1, 2));
  REQUIRE(result[3].origin() == Location(2, 2));
}

TEST_CASE("Documents can have a single newline erased.", "[DocumentTests]") {
  Document document("ABCD\nEFGH\n");
  SelectionSet result = document.erase(Selection(Location(4, 0)));
  
  REQUIRE(document.row(0) == "ABCDEFGH\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(4, 0));
}

TEST_CASE("Documents can have multiple newlines erased.", "[DocumentTests]") {
  Document document("AB\nCD\nEF\nGH\n");
  SelectionSet selections({
    Selection(Location(2, 0)),
    Selection(Location(2, 2)),
    Selection(Location(2, 3))
  });
  SelectionSet result = document.erase(selections);
  
  REQUIRE(document.rows() == 2);
  REQUIRE(document.row(0) == "ABCD\n");
  REQUIRE(document.row(1) == "EFGH");
  REQUIRE(result.count() == 3);
  REQUIRE(result[0].origin() == Location(2, 0));
  REQUIRE(result[1].origin() == Location(2, 1));
  REQUIRE(result[2].origin() == Location(3, 1));
}

TEST_CASE("Documents can have the last character in the document erased.", "[DocumentTests]") {
  Document document("ABCD");
  SelectionSet result = document.erase(Selection(Location(3, 0)));
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "ABC");
  REQUIRE(result.primary().origin() == Location(2, 0));
}

TEST_CASE("Documents can have the last character of the document erased.", "[DocumentTests]") {
  Document document("A");
  SelectionSet result = document.erase(Selection(Location(0, 0)));
  
  REQUIRE(document.rows() == 0);
  REQUIRE(result.primary().origin() == Location(0, 0));
}
