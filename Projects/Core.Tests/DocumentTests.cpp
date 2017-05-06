#include "catch.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"
#include "Selection.hpp"
#include "SelectionSet.hpp"

using namespace quip;

TEST_CASE("Default-construct a document.", "Document") {
  Document document;
  
  REQUIRE(document.rows() == 0);
  REQUIRE(document.isMissingTrailingNewline());
}

TEST_CASE("Construct a document from a single line of text.", "Document") {
  Document document("ABCD");
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.contents() == "ABCD");
  REQUIRE(document.isMissingTrailingNewline());
}

TEST_CASE("Construct a document from multiple lines of text.", "Document") {
  Document document("ABCD\nEFGH\nIJKL");
  
  REQUIRE(document.rows() == 3);
  REQUIRE(document.row(0) == "ABCD\n");
  REQUIRE(document.row(1) == "EFGH\n");
  REQUIRE(document.row(2) == "IJKL");
}

TEST_CASE("Construct a document with a trailing newline.", "Document") {
  Document document("ABCD\n");
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.contents() == "ABCD\n");
  REQUIRE_FALSE(document.isMissingTrailingNewline());
}

TEST_CASE("Test if a document is empty.", "Document") {
  Document empty;
  REQUIRE(empty.isEmpty());
  
  Document filled("ABCD");
  REQUIRE_FALSE(filled.isEmpty());
}

TEST_CASE("Get the distance between the same location.", "Document") {
  Document document("ABCD");
  std::int64_t result = document.distance(Location(0, 0), Location(0, 0));
  
  REQUIRE(result == 0);
}

TEST_CASE("Get the distance between increasing locations on the same row.", "Document") {
  Document document("ABCD");
  std::int64_t result = document.distance(Location(1, 0), Location(3, 0));
  
  REQUIRE(result == 2);
}

TEST_CASE("Get the distance between decreasing locations on the same row.", "Document") {
  Document document("ABCD");
  std::int64_t result = document.distance(Location(3, 0), Location(1, 0));
  
  REQUIRE(result == -2);
}

TEST_CASE("Get the distance between increasing locations on different rows.", "Document") {
  Document document("ABCD\nEFGH\nIJKL\n");
  std::int64_t result = document.distance(Location(1, 0), Location(2, 2));
  
  REQUIRE(result == 11);
}

TEST_CASE("Get the distance between decreasing locations on different rows.", "Document") {
  Document document("ABCD\nEFGH\nIJKL\n");
  std::int64_t result = document.distance(Location(2, 2), Location(1, 0));
  
  REQUIRE(result == -11);
}

TEST_CASE("Get the document content.", "Document") {
  Document document("ABCD\nEFGH\n");
  std::string result = document.contents();
  
  REQUIRE(result == "ABCD\nEFGH\n");
}

TEST_CASE("Get the document content when empty.", "Document") {
  Document document;
  std::string result = document.contents();
  
  REQUIRE(result == "");
}

TEST_CASE("Get a selection's content.", "Document") {
  Document document("ABCD\nEFGH\n");
  std::string result = document.contents(Selection(Location(2, 0), Location(1, 1)));
  
  REQUIRE(result == "CD\nEF");
}

TEST_CASE("Get a selection's content when empty.", "Document") {
  Document document;
  std::string result = document.contents(Selection(Location(0, 0)));
  
  REQUIRE(result == "");
}

TEST_CASE("Get a selection set's content.", "Document") {
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

TEST_CASE("Get a selection set's content when empty.", "Document") {
  Document document;
  SelectionSet selections(Selection(Location(0, 0)));
  std::vector<std::string> result = document.contents(selections);
  
  REQUIRE(result.size() == 0);
}

TEST_CASE("Get an iterator to the start of the document", "Document") {
  Document document("Hello, world!");
  DocumentIterator iterator = document.begin();
  
  REQUIRE(iterator != document.end());
  REQUIRE(iterator.location() == Location(0, 0));
}

TEST_CASE("Get an iterator to the end of the document", "Document") {
  Document document("Hello, world!");
  DocumentIterator iterator = document.end();
  
  REQUIRE(iterator != document.begin());
  REQUIRE(iterator.location() == Location(13, 0));
}

TEST_CASE("Get an iterator to a specific location in the document.", "Document") {
  Document document("Hello, world!");
  DocumentIterator iterator = document.from(Location(5, 0));
  
  REQUIRE(iterator != document.end());
  REQUIRE(iterator.location() == Location(5, 0));
  REQUIRE(*iterator == ',');
}

TEST_CASE("Get an iterator to a specific column and row in the document.", "Document") {
  Document document("Hello, world!");
  DocumentIterator iterator = document.from(5, 0);
  
  REQUIRE(iterator != document.end());
  REQUIRE(iterator.location() == Location(5, 0));
  REQUIRE(*iterator == ',');
}

TEST_CASE("Iterate a document in reverse.", "Document") {
  Document document("43210");
  std::string expected = "01234";
  std::stringstream result;
  Document::ReverseIterator cursor = document.rbegin();
  Document::ReverseIterator end = document.rend();
  while(cursor != end) {
    result << *cursor;
    ++cursor;
  }
  
  REQUIRE(result.str() == expected);
}

TEST_CASE("Set the path.", "Document") {
  Document document;
  REQUIRE(document.path() == "");
  
  document.setPath("~/path.txt");
  REQUIRE(document.path() == "~/path.txt");
}

TEST_CASE("Insert text when empty.", "Document") {
  Document document;
  SelectionSet result = document.insert(Selection(Location(0, 0)), "ABCD\nEFGH\n");
  
  REQUIRE(document.rows() == 2);
  REQUIRE(document.row(0) == "ABCD\n");
  REQUIRE(document.row(1) == "EFGH\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(5, 1));
}

TEST_CASE("Insert text via a single selection.", "Document") {
  Document document("ABD\n");
  SelectionSet result = document.insert(Selection(Location(2, 0)), "C");
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "ABCD\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(3, 0));
}

TEST_CASE("Insert text via multiple selections.", "Document") {
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

TEST_CASE("Insert text with leading newlines via a single selection.", "Document") {
  Document document("ABCD\n");
  document.insert(Selection(Location(4, 0)), "\nEFGH");
  
  REQUIRE(document.rows() == 2);
  REQUIRE(document.row(0) == "ABCD\n");
  REQUIRE(document.row(1) == "EFGH\n");
}

TEST_CASE("Insert text with trailing newlines via a single selection.", "Document") {
  Document document("ABCD\n");
  SelectionSet result = document.insert(Selection(Location(4, 0)), "EFGH\n");
  
  REQUIRE(document.rows() == 2);
  REQUIRE(document.row(0) == "ABCDEFGH\n");
  REQUIRE(document.row(1) == "\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(0, 1));
}

TEST_CASE("Insert text with internal newlines via a single selection.", "Document") {
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

TEST_CASE("Erase text when empty.", "Document") {
  Document document;
  Selection selection(Location(0, 0));
  SelectionSet result = document.erase(selection);
  
  REQUIRE(document.rows() == 0);
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(0, 0));
}

TEST_CASE("Erase text via empty selections.", "Document") {
  Document document("ABCD\n");
  SelectionSet empty;
  document.erase(empty);
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "ABCD\n");
}

TEST_CASE("Erase a single character erased via a single selection.", "Document") {
  Document document("ABCD\n");
  SelectionSet result = document.erase(Selection(Location(1, 0)));
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "ACD\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(1, 0));
}

TEST_CASE("Erase multiple characters via a single selection.", "Document") {
  Document document("ABCD\n");
  SelectionSet result = document.erase(Selection(Location(1, 0), Location(2, 0)));
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "AD\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(1, 0));
}

TEST_CASE("Erase a single line via a single selection.", "Document") {
  Document document("ABCD\nEFGH\n");
  SelectionSet result = document.erase(Selection(Location(0, 0), Location(4, 0)));
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "EFGH\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(0, 0));
}

TEST_CASE("Erase multiple lines via a single selection.", "Document") {
  Document document("ABCD\nEFGH\nHIJK\n");
  SelectionSet result = document.erase(Selection(Location(0, 0), Location(4, 1)));
  
  REQUIRE(document.row(0) == "HIJK\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(0, 0));
}

TEST_CASE("Erase multiple characters via multiple selections.", "Document") {
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

TEST_CASE("Erase multiple characters across multiple lines via multiple selections.", "Document") {
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

TEST_CASE("Erase multiple characters across multiple disjoint lines via multiple selections.", "Document") {
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

TEST_CASE("Erase a single newline.", "Document") {
  Document document("ABCD\nEFGH\n");
  SelectionSet result = document.erase(Selection(Location(4, 0)));
  
  REQUIRE(document.row(0) == "ABCDEFGH\n");
  REQUIRE(result.count() == 1);
  REQUIRE(result.primary().origin() == Location(4, 0));
}

TEST_CASE("Erase multiple newlines.", "Document") {
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

TEST_CASE("Erase the last character in the document.", "Document") {
  // This tests that the last character in document order can be erased (the document will still be non-empty).
  Document document("ABCD");
  SelectionSet result = document.erase(Selection(Location(3, 0)));
  
  REQUIRE(document.rows() == 1);
  REQUIRE(document.row(0) == "ABC");
  REQUIRE(result.primary().origin() == Location(2, 0));
}

TEST_CASE("Erase the last character of the document.", "Document") {
  // This tests that the final character of a document can be erased (thus leaving the document empty).
  Document document("A");
  SelectionSet result = document.erase(Selection(Location(0, 0)));

  REQUIRE(document.isEmpty());
  REQUIRE(document.rows() == 0);
  REQUIRE(result.primary().origin() == Location(0, 0));
}
