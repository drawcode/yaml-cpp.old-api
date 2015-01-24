#include <sstream>

#include "specexamples.h"   // IWYU pragma: keep
#include "yaml-cpp/yaml.h"  // IWYU pragma: keep

#include "gtest/gtest.h"

#define EXPECT_THROW_PARSER_EXCEPTION(statement, message) \
  ASSERT_THROW(statement, ParserException);               \
  try {                                                   \
    statement;                                            \
  }                                                       \
  catch (const ParserException& e) {                      \
    EXPECT_EQ(e.msg, message);                            \
  }

namespace YAML {
namespace {

class LegacyNodeSpecTest : public ::testing::Test {
 protected:
  void Parse(const std::string& input) {
    stream.reset(new std::stringstream(input));
    parser.reset(new Parser(*stream));
    parser->GetNextDocument(doc);
  }

  void ParseNext() { parser->GetNextDocument(doc); }

  Node doc;

 private:
  std::auto_ptr<std::stringstream> stream;
  std::auto_ptr<Parser> parser;
};

TEST_F(LegacyNodeSpecTest, Ex2_1_SeqScalars) {
  Parse(ex2_1);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("Mark McGwire", doc[0].to<std::string>());
  EXPECT_EQ("Sammy Sosa", doc[1].to<std::string>());
  EXPECT_EQ("Ken Griffey", doc[2].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_2_MappingScalarsToScalars) {
  Parse(ex2_2);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("65", doc["hr"].to<std::string>());
  EXPECT_EQ("0.278", doc["avg"].to<std::string>());
  EXPECT_EQ("147", doc["rbi"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_3_MappingScalarsToSequences) {
  Parse(ex2_3);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ(3, doc["american"].size());
  EXPECT_EQ("Boston Red Sox", doc["american"][0].to<std::string>());
  EXPECT_EQ("Detroit Tigers", doc["american"][1].to<std::string>());
  EXPECT_EQ("New York Yankees", doc["american"][2].to<std::string>());
  EXPECT_EQ(3, doc["national"].size());
  EXPECT_EQ("New York Mets", doc["national"][0].to<std::string>());
  EXPECT_EQ("Chicago Cubs", doc["national"][1].to<std::string>());
  EXPECT_EQ("Atlanta Braves", doc["national"][2].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_4_SequenceOfMappings) {
  Parse(ex2_4);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ(3, doc[0].size());
  EXPECT_EQ("Mark McGwire", doc[0]["name"].to<std::string>());
  EXPECT_EQ("65", doc[0]["hr"].to<std::string>());
  EXPECT_EQ("0.278", doc[0]["avg"].to<std::string>());
  EXPECT_EQ(3, doc[1].size());
  EXPECT_EQ("Sammy Sosa", doc[1]["name"].to<std::string>());
  EXPECT_EQ("63", doc[1]["hr"].to<std::string>());
  EXPECT_EQ("0.288", doc[1]["avg"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_5_SequenceOfSequences) {
  Parse(ex2_5);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ(3, doc[0].size());
  EXPECT_EQ("name", doc[0][0].to<std::string>());
  EXPECT_EQ("hr", doc[0][1].to<std::string>());
  EXPECT_EQ("avg", doc[0][2].to<std::string>());
  EXPECT_EQ(3, doc[1].size());
  EXPECT_EQ("Mark McGwire", doc[1][0].to<std::string>());
  EXPECT_EQ("65", doc[1][1].to<std::string>());
  EXPECT_EQ("0.278", doc[1][2].to<std::string>());
  EXPECT_EQ(3, doc[2].size());
  EXPECT_EQ("Sammy Sosa", doc[2][0].to<std::string>());
  EXPECT_EQ("63", doc[2][1].to<std::string>());
  EXPECT_EQ("0.288", doc[2][2].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_6_MappingOfMappings) {
  Parse(ex2_6);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ(2, doc["Mark McGwire"].size());
  EXPECT_EQ("65", doc["Mark McGwire"]["hr"].to<std::string>());
  EXPECT_EQ("0.278", doc["Mark McGwire"]["avg"].to<std::string>());
  EXPECT_EQ(2, doc["Sammy Sosa"].size());
  EXPECT_EQ("63", doc["Sammy Sosa"]["hr"].to<std::string>());
  EXPECT_EQ("0.288", doc["Sammy Sosa"]["avg"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_7_TwoDocumentsInAStream) {
  Parse(ex2_7);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("Mark McGwire", doc[0].to<std::string>());
  EXPECT_EQ("Sammy Sosa", doc[1].to<std::string>());
  EXPECT_EQ("Ken Griffey", doc[2].to<std::string>());

  ParseNext();
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ("Chicago Cubs", doc[0].to<std::string>());
  EXPECT_EQ("St Louis Cardinals", doc[1].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_8_PlayByPlayFeed) {
  Parse(ex2_8);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("20:03:20", doc["time"].to<std::string>());
  EXPECT_EQ("Sammy Sosa", doc["player"].to<std::string>());
  EXPECT_EQ("strike (miss)", doc["action"].to<std::string>());

  ParseNext();
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("20:03:47", doc["time"].to<std::string>());
  EXPECT_EQ("Sammy Sosa", doc["player"].to<std::string>());
  EXPECT_EQ("grand slam", doc["action"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_9_SingleDocumentWithTwoComments) {
  Parse(ex2_9);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ(2, doc["hr"].size());
  EXPECT_EQ("Mark McGwire", doc["hr"][0].to<std::string>());
  EXPECT_EQ("Sammy Sosa", doc["hr"][1].to<std::string>());
  EXPECT_EQ(2, doc["rbi"].size());
  EXPECT_EQ("Sammy Sosa", doc["rbi"][0].to<std::string>());
  EXPECT_EQ("Ken Griffey", doc["rbi"][1].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_10_SimpleAnchor) {
  Parse(ex2_10);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ(2, doc["hr"].size());
  EXPECT_EQ("Mark McGwire", doc["hr"][0].to<std::string>());
  EXPECT_EQ("Sammy Sosa", doc["hr"][1].to<std::string>());
  EXPECT_EQ(2, doc["rbi"].size());
  EXPECT_EQ("Sammy Sosa", doc["rbi"][0].to<std::string>());
  EXPECT_EQ("Ken Griffey", doc["rbi"][1].to<std::string>());
}

struct Pair {
  Pair() {}
  Pair(const std::string& f, const std::string& s) : first(f), second(s) {}
  std::string first, second;
};

bool operator==(const Pair& p, const Pair& q) {
  return p.first == q.first && p.second == q.second;
}

void operator>>(const Node& node, Pair& p) {
  node[0] >> p.first;
  node[1] >> p.second;
}

TEST_F(LegacyNodeSpecTest, Ex2_11_MappingBetweenSequences) {
  Parse(ex2_11);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ(1, doc[Pair("Detroit Tigers", "Chicago cubs")].size());
  EXPECT_EQ("2001-07-23",
            doc[Pair("Detroit Tigers", "Chicago cubs")][0].to<std::string>());
  EXPECT_EQ(3, doc[Pair("New York Yankees", "Atlanta Braves")].size());
  EXPECT_EQ(
      "2001-07-02",
      doc[Pair("New York Yankees", "Atlanta Braves")][0].to<std::string>());
  EXPECT_EQ(
      "2001-08-12",
      doc[Pair("New York Yankees", "Atlanta Braves")][1].to<std::string>());
  EXPECT_EQ(
      "2001-08-14",
      doc[Pair("New York Yankees", "Atlanta Braves")][2].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_12_CompactNestedMapping) {
  Parse(ex2_12);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ(2, doc[0].size());
  EXPECT_EQ("Super Hoop", doc[0]["item"].to<std::string>());
  EXPECT_EQ(1, doc[0]["quantity"].to<int>());
  EXPECT_EQ(2, doc[1].size());
  EXPECT_EQ("Basketball", doc[1]["item"].to<std::string>());
  EXPECT_EQ(4, doc[1]["quantity"].to<int>());
  EXPECT_EQ(2, doc[2].size());
  EXPECT_EQ("Big Shoes", doc[2]["item"].to<std::string>());
  EXPECT_EQ(1, doc[2]["quantity"].to<int>());
}

TEST_F(LegacyNodeSpecTest, Ex2_13_InLiteralsNewlinesArePreserved) {
  Parse(ex2_13);
  EXPECT_EQ(
      "\\//||\\/||\n"
      "// ||  ||__",
      doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_14_InFoldedScalarsNewlinesBecomeSpaces) {
  Parse(ex2_14);
  EXPECT_EQ("Mark McGwire's year was crippled by a knee injury.",
            doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest,
       Ex2_15_FoldedNewlinesArePreservedForMoreIndentedAndBlankLines) {
  Parse(ex2_15);
  EXPECT_EQ(
      "Sammy Sosa completed another fine season with great stats.\n\n"
      "  63 Home Runs\n"
      "  0.288 Batting Average\n\n"
      "What a year!",
      doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_16_IndentationDeterminesScope) {
  Parse(ex2_16);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("Mark McGwire", doc["name"].to<std::string>());
  EXPECT_EQ("Mark set a major league home run record in 1998.\n",
            doc["accomplishment"].to<std::string>());
  EXPECT_EQ("65 Home Runs\n0.278 Batting Average\n",
            doc["stats"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_17_QuotedScalars) {
  Parse(ex2_17);
  EXPECT_EQ(6, doc.size());
  EXPECT_EQ("Sosa did fine.\xe2\x98\xba", doc["unicode"].to<std::string>());
  EXPECT_EQ("\b1998\t1999\t2000\n", doc["control"].to<std::string>());
  EXPECT_EQ("\x0d\x0a is \r\n", doc["hex esc"].to<std::string>());
  EXPECT_EQ("\"Howdy!\" he cried.", doc["single"].to<std::string>());
  EXPECT_EQ(" # Not a 'comment'.", doc["quoted"].to<std::string>());
  EXPECT_EQ("|\\-*-/|", doc["tie-fighter"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_18_MultiLineFlowScalars) {
  Parse(ex2_18);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ("This unquoted scalar spans many lines.",
            doc["plain"].to<std::string>());
  EXPECT_EQ("So does this quoted scalar.\n", doc["quoted"].to<std::string>());
}

// TODO: 2.19 - 2.22 schema tags

TEST_F(LegacyNodeSpecTest, Ex2_23_VariousExplicitTags) {
  Parse(ex2_23);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("tag:yaml.org,2002:str", doc["not-date"].Tag());
  EXPECT_EQ("2002-04-28", doc["not-date"].to<std::string>());
  EXPECT_EQ("tag:yaml.org,2002:binary", doc["picture"].Tag());
  EXPECT_EQ(
      "R0lGODlhDAAMAIQAAP//9/X\n"
      "17unp5WZmZgAAAOfn515eXv\n"
      "Pz7Y6OjuDg4J+fn5OTk6enp\n"
      "56enmleECcgggoBADs=\n",
      doc["picture"].to<std::string>());
  EXPECT_EQ("!something", doc["application specific tag"].Tag());
  EXPECT_EQ(
      "The semantics of the tag\n"
      "above may be different for\n"
      "different documents.",
      doc["application specific tag"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_24_GlobalTags) {
  Parse(ex2_24);
  EXPECT_EQ("tag:clarkevans.com,2002:shape", doc.Tag());
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("tag:clarkevans.com,2002:circle", doc[0].Tag());
  EXPECT_EQ(2, doc[0].size());
  EXPECT_EQ(2, doc[0]["center"].size());
  EXPECT_EQ(73, doc[0]["center"]["x"].to<int>());
  EXPECT_EQ(129, doc[0]["center"]["y"].to<int>());
  EXPECT_EQ(7, doc[0]["radius"].to<int>());
  EXPECT_EQ("tag:clarkevans.com,2002:line", doc[1].Tag());
  EXPECT_EQ(2, doc[1].size());
  EXPECT_EQ(2, doc[1]["start"].size());
  EXPECT_EQ(73, doc[1]["start"]["x"].to<int>());
  EXPECT_EQ(129, doc[1]["start"]["y"].to<int>());
  EXPECT_EQ(2, doc[1]["finish"].size());
  EXPECT_EQ(89, doc[1]["finish"]["x"].to<int>());
  EXPECT_EQ(102, doc[1]["finish"]["y"].to<int>());
  EXPECT_EQ("tag:clarkevans.com,2002:label", doc[2].Tag());
  EXPECT_EQ(3, doc[2].size());
  EXPECT_EQ(2, doc[2]["start"].size());
  EXPECT_EQ(73, doc[2]["start"]["x"].to<int>());
  EXPECT_EQ(129, doc[2]["start"]["y"].to<int>());
  EXPECT_EQ("0xFFEEBB", doc[2]["color"].to<std::string>());
  EXPECT_EQ("Pretty vector drawing.", doc[2]["text"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_25_UnorderedSets) {
  Parse(ex2_25);
  EXPECT_EQ("tag:yaml.org,2002:set", doc.Tag());
  EXPECT_EQ(3, doc.size());
  EXPECT_TRUE(IsNull(doc["Mark McGwire"]));
  EXPECT_TRUE(IsNull(doc["Sammy Sosa"]));
  EXPECT_TRUE(IsNull(doc["Ken Griffey"]));
}

TEST_F(LegacyNodeSpecTest, Ex2_26_OrderedMappings) {
  Parse(ex2_26);
  EXPECT_EQ("tag:yaml.org,2002:omap", doc.Tag());
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ(1, doc[0].size());
  EXPECT_EQ(65, doc[0]["Mark McGwire"].to<int>());
  EXPECT_EQ(1, doc[1].size());
  EXPECT_EQ(63, doc[1]["Sammy Sosa"].to<int>());
  EXPECT_EQ(1, doc[2].size());
  EXPECT_EQ(58, doc[2]["Ken Griffey"].to<int>());
}

TEST_F(LegacyNodeSpecTest, Ex2_27_Invoice) {
  Parse(ex2_27);
  EXPECT_EQ("tag:clarkevans.com,2002:invoice", doc.Tag());
  EXPECT_EQ(8, doc.size());
  EXPECT_EQ(34843, doc["invoice"].to<int>());
  EXPECT_EQ("2001-01-23", doc["date"].to<std::string>());
  EXPECT_EQ(3, doc["bill-to"].size());
  EXPECT_EQ("Chris", doc["bill-to"]["given"].to<std::string>());
  EXPECT_EQ("Dumars", doc["bill-to"]["family"].to<std::string>());
  EXPECT_EQ(4, doc["bill-to"]["address"].size());
  EXPECT_EQ("458 Walkman Dr.\nSuite #292\n",
            doc["bill-to"]["address"]["lines"].to<std::string>());
  EXPECT_EQ("Royal Oak", doc["bill-to"]["address"]["city"].to<std::string>());
  EXPECT_EQ("MI", doc["bill-to"]["address"]["state"].to<std::string>());
  EXPECT_EQ("48046", doc["bill-to"]["address"]["postal"].to<std::string>());
  EXPECT_EQ(3, doc["ship-to"].size());
  EXPECT_EQ("Chris", doc["ship-to"]["given"].to<std::string>());
  EXPECT_EQ("Dumars", doc["ship-to"]["family"].to<std::string>());
  EXPECT_EQ(4, doc["ship-to"]["address"].size());
  EXPECT_EQ("458 Walkman Dr.\nSuite #292\n",
            doc["ship-to"]["address"]["lines"].to<std::string>());
  EXPECT_EQ("Royal Oak", doc["ship-to"]["address"]["city"].to<std::string>());
  EXPECT_EQ("MI", doc["ship-to"]["address"]["state"].to<std::string>());
  EXPECT_EQ("48046", doc["ship-to"]["address"]["postal"].to<std::string>());
  EXPECT_EQ(2, doc["product"].size());
  EXPECT_EQ(4, doc["product"][0].size());
  EXPECT_EQ("BL394D", doc["product"][0]["sku"].to<std::string>());
  EXPECT_EQ(4, doc["product"][0]["quantity"].to<int>());
  EXPECT_EQ("Basketball", doc["product"][0]["description"].to<std::string>());
  EXPECT_EQ("450.00", doc["product"][0]["price"].to<std::string>());
  EXPECT_EQ(4, doc["product"][1].size());
  EXPECT_EQ("BL4438H", doc["product"][1]["sku"].to<std::string>());
  EXPECT_EQ(1, doc["product"][1]["quantity"].to<int>());
  EXPECT_EQ("Super Hoop", doc["product"][1]["description"].to<std::string>());
  EXPECT_EQ("2392.00", doc["product"][1]["price"].to<std::string>());
  EXPECT_EQ("251.42", doc["tax"].to<std::string>());
  EXPECT_EQ("4443.52", doc["total"].to<std::string>());
  EXPECT_EQ(
      "Late afternoon is best. Backup contact is Nancy Billsmer @ 338-4338.",
      doc["comments"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex2_28_LogFile) {
  Parse(ex2_28);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("2001-11-23 15:01:42 -5", doc["Time"].to<std::string>());
  EXPECT_EQ("ed", doc["User"].to<std::string>());
  EXPECT_EQ("This is an error message for the log file",
            doc["Warning"].to<std::string>());

  ParseNext();
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("2001-11-23 15:02:31 -5", doc["Time"].to<std::string>());
  EXPECT_EQ("ed", doc["User"].to<std::string>());
  EXPECT_EQ("A slightly different error message.",
            doc["Warning"].to<std::string>());

  ParseNext();
  EXPECT_EQ(4, doc.size());
  EXPECT_EQ("2001-11-23 15:03:17 -5", doc["Date"].to<std::string>());
  EXPECT_EQ("ed", doc["User"].to<std::string>());
  EXPECT_EQ("Unknown variable \"bar\"", doc["Fatal"].to<std::string>());
  EXPECT_EQ(2, doc["Stack"].size());
  EXPECT_EQ(3, doc["Stack"][0].size());
  EXPECT_EQ("TopClass.py", doc["Stack"][0]["file"].to<std::string>());
  EXPECT_EQ("23", doc["Stack"][0]["line"].to<std::string>());
  EXPECT_EQ("x = MoreObject(\"345\\n\")\n",
            doc["Stack"][0]["code"].to<std::string>());
  EXPECT_EQ(3, doc["Stack"][1].size());
  EXPECT_EQ("MoreClass.py", doc["Stack"][1]["file"].to<std::string>());
  EXPECT_EQ("58", doc["Stack"][1]["line"].to<std::string>());
  EXPECT_EQ("foo = bar", doc["Stack"][1]["code"].to<std::string>());
}

// TODO: 5.1 - 5.2 BOM

TEST_F(LegacyNodeSpecTest, Ex5_3_BlockStructureIndicators) {
  Parse(ex5_3);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ(2, doc["sequence"].size());
  EXPECT_EQ("one", doc["sequence"][0].to<std::string>());
  EXPECT_EQ("two", doc["sequence"][1].to<std::string>());
  EXPECT_EQ(2, doc["mapping"].size());
  EXPECT_EQ("blue", doc["mapping"]["sky"].to<std::string>());
  EXPECT_EQ("green", doc["mapping"]["sea"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex5_4_FlowStructureIndicators) {
  Parse(ex5_4);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ(2, doc["sequence"].size());
  EXPECT_EQ("one", doc["sequence"][0].to<std::string>());
  EXPECT_EQ("two", doc["sequence"][1].to<std::string>());
  EXPECT_EQ(2, doc["mapping"].size());
  EXPECT_EQ("blue", doc["mapping"]["sky"].to<std::string>());
  EXPECT_EQ("green", doc["mapping"]["sea"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex5_5_CommentIndicator) {
  Parse(ex5_5);
  EXPECT_EQ(0, doc.size());
}

TEST_F(LegacyNodeSpecTest, Ex5_6_NodePropertyIndicators) {
  Parse(ex5_6);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ("value", doc["anchored"].to<std::string>());  // TODO: assert tag
  EXPECT_EQ("value", doc["alias"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex5_7_BlockScalarIndicators) {
  Parse(ex5_7);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ("some\ntext\n", doc["literal"].to<std::string>());
  EXPECT_EQ("some text\n", doc["folded"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex5_8_QuotedScalarIndicators) {
  Parse(ex5_8);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ("text", doc["single"].to<std::string>());
  EXPECT_EQ("text", doc["double"].to<std::string>());
}

// TODO: 5.9 directive
// TODO: 5.10 reserved indicator

TEST_F(LegacyNodeSpecTest, Ex5_11_LineBreakCharacters) {
  Parse(ex5_11);
  EXPECT_EQ("Line break (no glyph)\nLine break (glyphed)\n",
            doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex5_12_TabsAndSpaces) {
  Parse(ex5_12);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ("Quoted\t", doc["quoted"].to<std::string>());
  EXPECT_EQ(
      "void main() {\n"
      "\tprintf(\"Hello, world!\\n\");\n"
      "}",
      doc["block"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex5_13_EscapedCharacters) {
  Parse(ex5_13);
  EXPECT_EQ("Fun with \x5C \x22 \x07 \x08 \x1B \x0C \x0A \x0D \x09 \x0B " +
                std::string("\x00", 1) +
                " \x20 \xA0 \x85 \xe2\x80\xa8 \xe2\x80\xa9 A A A",
            doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex5_14_InvalidEscapedCharacters) {
  EXPECT_THROW_PARSER_EXCEPTION(Parse(ex5_14),
                                std::string(ErrorMsg::INVALID_ESCAPE) + "c");
}

TEST_F(LegacyNodeSpecTest, Ex6_1_IndentationSpaces) {
  Parse(ex6_1);
  EXPECT_EQ(1, doc.size());
  EXPECT_EQ(2, doc["Not indented"].size());
  EXPECT_EQ("By four\n  spaces\n",
            doc["Not indented"]["By one space"].to<std::string>());
  EXPECT_EQ(3, doc["Not indented"]["Flow style"].size());
  EXPECT_EQ("By two", doc["Not indented"]["Flow style"][0].to<std::string>());
  EXPECT_EQ("Also by two",
            doc["Not indented"]["Flow style"][1].to<std::string>());
  EXPECT_EQ("Still by two",
            doc["Not indented"]["Flow style"][2].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_2_IndentationIndicators) {
  Parse(ex6_2);
  EXPECT_EQ(1, doc.size());
  EXPECT_EQ(2, doc["a"].size());
  EXPECT_EQ("b", doc["a"][0].to<std::string>());
  EXPECT_EQ(2, doc["a"][1].size());
  EXPECT_EQ("c", doc["a"][1][0].to<std::string>());
  EXPECT_EQ("d", doc["a"][1][1].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_3_SeparationSpaces) {
  Parse(ex6_3);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ(1, doc[0].size());
  EXPECT_EQ("bar", doc[0]["foo"].to<std::string>());
  EXPECT_EQ(2, doc[1].size());
  EXPECT_EQ("baz", doc[1][0].to<std::string>());
  EXPECT_EQ("baz", doc[1][1].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_4_LinePrefixes) {
  Parse(ex6_4);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("text lines", doc["plain"].to<std::string>());
  EXPECT_EQ("text lines", doc["quoted"].to<std::string>());
  EXPECT_EQ("text\n \tlines\n", doc["block"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_5_EmptyLines) {
  Parse(ex6_5);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ("Empty line\nas a line feed", doc["Folding"].to<std::string>());
  EXPECT_EQ("Clipped empty lines\n", doc["Chomping"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_6_LineFolding) {
  Parse(ex6_6);
  EXPECT_EQ("trimmed\n\n\nas space", doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_7_BlockFolding) {
  Parse(ex6_7);
  EXPECT_EQ("foo \n\n\t bar\n\nbaz\n", doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_8_FlowFolding) {
  Parse(ex6_8);
  EXPECT_EQ(" foo\nbar\nbaz ", doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_9_SeparatedComment) {
  Parse(ex6_9);
  EXPECT_EQ(1, doc.size());
  EXPECT_EQ("value", doc["key"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_10_CommentLines) {
  Parse(ex6_10);
  EXPECT_EQ(0, doc.size());
}

TEST_F(LegacyNodeSpecTest, Ex6_11_MultiLineComments) {
  Parse(ex6_11);
  EXPECT_EQ(1, doc.size());
  EXPECT_EQ("value", doc["key"].to<std::string>());
}

struct StringMap {
  typedef std::map<std::string, std::string> Map;
  Map _;
};

bool operator==(const StringMap& m, const StringMap& n) { return m._ == n._; }

void operator>>(const Node& node, StringMap& m) {
  m._.clear();
  for (Iterator it = node.begin(); it != node.end(); ++it) {
    std::string key = it.first().to<std::string>();
    std::string value = it.second().to<std::string>();
    m._[key] = value;
  }
}

TEST_F(LegacyNodeSpecTest, Ex6_12_SeparationSpacesII) {
  Parse(ex6_12);
  std::map<std::string, std::string> key;
  key["first"] = "Sammy";
  key["last"] = "Sosa";
  EXPECT_EQ(1, doc.size());
  EXPECT_EQ(2, doc[key].size());
  EXPECT_EQ(65, doc[key]["hr"].to<int>());
  EXPECT_EQ("0.278", doc[key]["avg"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_13_ReservedDirectives) { Parse(ex6_13); }

TEST_F(LegacyNodeSpecTest, Ex6_14_YAMLDirective) { Parse(ex6_14); }

TEST_F(LegacyNodeSpecTest, Ex6_15_InvalidRepeatedYAMLDirective) {
  EXPECT_THROW_PARSER_EXCEPTION(Parse(ex6_15),
                                ErrorMsg::REPEATED_YAML_DIRECTIVE);
}

TEST_F(LegacyNodeSpecTest, Ex6_16_TagDirective) {
  Parse(ex6_16);
  EXPECT_EQ("tag:yaml.org,2002:str", doc.Tag());
  EXPECT_EQ("foo", doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_17_InvalidRepeatedTagDirective) {
  EXPECT_THROW_PARSER_EXCEPTION(Parse(ex6_17),
                                ErrorMsg::REPEATED_TAG_DIRECTIVE);
}

TEST_F(LegacyNodeSpecTest, Ex6_18_PrimaryTagHandle) {
  Parse(ex6_18);
  EXPECT_EQ("!foo", doc.Tag());
  EXPECT_EQ("bar", doc.to<std::string>());

  ParseNext();
  EXPECT_EQ("tag:example.com,2000:app/foo", doc.Tag());
  EXPECT_EQ("bar", doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_19_SecondaryTagHandle) {
  Parse(ex6_19);
  EXPECT_EQ("tag:example.com,2000:app/int", doc.Tag());
  EXPECT_EQ("1 - 3", doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_20_TagHandles) {
  Parse(ex6_20);
  EXPECT_EQ("tag:example.com,2000:app/foo", doc.Tag());
  EXPECT_EQ("bar", doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_21_LocalTagPrefix) {
  Parse(ex6_21);
  EXPECT_EQ("!my-light", doc.Tag());
  EXPECT_EQ("fluorescent", doc.to<std::string>());

  ParseNext();
  EXPECT_EQ("!my-light", doc.Tag());
  EXPECT_EQ("green", doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_22_GlobalTagPrefix) {
  Parse(ex6_22);
  EXPECT_EQ(1, doc.size());
  EXPECT_EQ("tag:example.com,2000:app/foo", doc[0].Tag());
  EXPECT_EQ("bar", doc[0].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_23_NodeProperties) {
  Parse(ex6_23);
  EXPECT_EQ(2, doc.size());
  for (Iterator it = doc.begin(); it != doc.end(); ++it) {
    if (it.first().to<std::string>() == "foo") {
      EXPECT_EQ("tag:yaml.org,2002:str", it.first().Tag());
      EXPECT_EQ("tag:yaml.org,2002:str", it.second().Tag());
      EXPECT_EQ("bar", it.second().to<std::string>());
    } else if (it.first().to<std::string>() == "baz") {
      EXPECT_EQ("foo", it.second().to<std::string>());
    } else
      FAIL() << "unknown key";
  }
}

TEST_F(LegacyNodeSpecTest, Ex6_24_VerbatimTags) {
  Parse(ex6_24);
  EXPECT_EQ(1, doc.size());
  for (Iterator it = doc.begin(); it != doc.end(); ++it) {
    EXPECT_EQ("tag:yaml.org,2002:str", it.first().Tag());
    EXPECT_EQ("foo", it.first().to<std::string>());
    EXPECT_EQ("!bar", it.second().Tag());
    EXPECT_EQ("baz", it.second().to<std::string>());
  }
}

// TODO: check tags (but we probably will say these are valid, I think)
TEST_F(LegacyNodeSpecTest, DISABLED_Ex6_25_InvalidVerbatimTags) {
  Parse(ex6_25);
  FAIL() << "not implemented yet";
}

TEST_F(LegacyNodeSpecTest, Ex6_26_TagShorthands) {
  Parse(ex6_26);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("!local", doc[0].Tag());
  EXPECT_EQ("foo", doc[0].to<std::string>());
  EXPECT_EQ("tag:yaml.org,2002:str", doc[1].Tag());
  EXPECT_EQ("bar", doc[1].to<std::string>());
  EXPECT_EQ("tag:example.com,2000:app/tag%21", doc[2].Tag());
  EXPECT_EQ("baz", doc[2].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_27A_InvalidTagShorthands) {
  EXPECT_THROW_PARSER_EXCEPTION(Parse(ex6_27a), ErrorMsg::TAG_WITH_NO_SUFFIX);
}

// TODO: should we reject this one (since !h! is not declared)?
TEST_F(LegacyNodeSpecTest, DISABLED_Ex6_27B_InvalidTagShorthands) {
  Parse(ex6_27b);
  FAIL() << "not implemented yet";
}

TEST_F(LegacyNodeSpecTest, Ex6_28_NonSpecificTags) {
  Parse(ex6_28);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("12", doc[0].to<std::string>());  // TODO: check tags. How?
  EXPECT_EQ(12, doc[1].to<int>());
  EXPECT_EQ("12", doc[2].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex6_29_NodeAnchors) {
  Parse(ex6_29);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ("Value", doc["First occurrence"].to<std::string>());
  EXPECT_EQ("Value", doc["Second occurrence"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_1_AliasNodes) {
  Parse(ex7_1);
  EXPECT_EQ(4, doc.size());
  EXPECT_EQ("Foo", doc["First occurrence"].to<std::string>());
  EXPECT_EQ("Foo", doc["Second occurrence"].to<std::string>());
  EXPECT_EQ("Bar", doc["Override anchor"].to<std::string>());
  EXPECT_EQ("Bar", doc["Reuse anchor"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_2_EmptyNodes) {
  Parse(ex7_2);
  EXPECT_EQ(2, doc.size());
  for (Iterator it = doc.begin(); it != doc.end(); ++it) {
    if (it.first().to<std::string>() == "foo") {
      EXPECT_EQ("tag:yaml.org,2002:str", it.second().Tag());
      EXPECT_EQ("", it.second().to<std::string>());
    } else if (it.first().to<std::string>() == "") {
      EXPECT_EQ("tag:yaml.org,2002:str", it.first().Tag());
      EXPECT_EQ("bar", it.second().to<std::string>());
    } else
      FAIL() << "unexpected key";
  }
}

TEST_F(LegacyNodeSpecTest, Ex7_3_CompletelyEmptyNodes) {
  Parse(ex7_3);
  EXPECT_EQ(2, doc.size());
  EXPECT_TRUE(IsNull(doc["foo"]));
  EXPECT_EQ("bar", doc[Null].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_4_DoubleQuotedImplicitKeys) {
  Parse(ex7_4);
  EXPECT_EQ(1, doc.size());
  EXPECT_EQ(1, doc["implicit block key"].size());
  EXPECT_EQ(1, doc["implicit block key"][0].size());
  EXPECT_EQ(
      "value",
      doc["implicit block key"][0]["implicit flow key"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_5_DoubleQuotedLineBreaks) {
  Parse(ex7_5);
  EXPECT_EQ("folded to a space,\nto a line feed, or \t \tnon-content",
            doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_6_DoubleQuotedLines) {
  Parse(ex7_6);
  EXPECT_EQ(" 1st non-empty\n2nd non-empty 3rd non-empty ",
            doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_7_SingleQuotedCharacters) {
  Parse(ex7_7);
  EXPECT_EQ("here's to \"quotes\"", doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_8_SingleQuotedImplicitKeys) {
  Parse(ex7_8);
  EXPECT_EQ(1, doc.size());
  EXPECT_EQ(1, doc["implicit block key"].size());
  EXPECT_EQ(1, doc["implicit block key"][0].size());
  EXPECT_EQ(
      "value",
      doc["implicit block key"][0]["implicit flow key"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_9_SingleQuotedLines) {
  Parse(ex7_9);
  EXPECT_EQ(" 1st non-empty\n2nd non-empty 3rd non-empty ",
            doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_10_PlainCharacters) {
  Parse(ex7_10);
  EXPECT_EQ(6, doc.size());
  EXPECT_EQ("::vector", doc[0].to<std::string>());
  EXPECT_EQ(": - ()", doc[1].to<std::string>());
  EXPECT_EQ("Up, up, and away!", doc[2].to<std::string>());
  EXPECT_EQ(-123, doc[3].to<int>());
  EXPECT_EQ("http://example.com/foo#bar", doc[4].to<std::string>());
  EXPECT_EQ(5, doc[5].size());
  EXPECT_EQ("::vector", doc[5][0].to<std::string>());
  EXPECT_EQ(": - ()", doc[5][1].to<std::string>());
  EXPECT_EQ("Up, up, and away!", doc[5][2].to<std::string>());
  EXPECT_EQ(-123, doc[5][3].to<int>());
  EXPECT_EQ("http://example.com/foo#bar", doc[5][4].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_11_PlainImplicitKeys) {
  Parse(ex7_11);
  EXPECT_EQ(1, doc.size());
  EXPECT_EQ(1, doc["implicit block key"].size());
  EXPECT_EQ(1, doc["implicit block key"][0].size());
  EXPECT_EQ(
      "value",
      doc["implicit block key"][0]["implicit flow key"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_12_PlainLines) {
  Parse(ex7_12);
  EXPECT_EQ("1st non-empty\n2nd non-empty 3rd non-empty",
            doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_13_FlowSequence) {
  Parse(ex7_13);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ(2, doc[0].size());
  EXPECT_EQ("one", doc[0][0].to<std::string>());
  EXPECT_EQ("two", doc[0][1].to<std::string>());
  EXPECT_EQ(2, doc[1].size());
  EXPECT_EQ("three", doc[1][0].to<std::string>());
  EXPECT_EQ("four", doc[1][1].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_14_FlowSequenceEntries) {
  Parse(ex7_14);
  EXPECT_EQ(5, doc.size());
  EXPECT_EQ("double quoted", doc[0].to<std::string>());
  EXPECT_EQ("single quoted", doc[1].to<std::string>());
  EXPECT_EQ("plain text", doc[2].to<std::string>());
  EXPECT_EQ(1, doc[3].size());
  EXPECT_EQ("nested", doc[3][0].to<std::string>());
  EXPECT_EQ(1, doc[4].size());
  EXPECT_EQ("pair", doc[4]["single"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_15_FlowMappings) {
  Parse(ex7_15);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ(2, doc[0].size());
  EXPECT_EQ("two", doc[0]["one"].to<std::string>());
  EXPECT_EQ("four", doc[0]["three"].to<std::string>());
  EXPECT_EQ(2, doc[1].size());
  EXPECT_EQ("six", doc[1]["five"].to<std::string>());
  EXPECT_EQ("eight", doc[1]["seven"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_16_FlowMappingEntries) {
  Parse(ex7_16);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("entry", doc["explicit"].to<std::string>());
  EXPECT_EQ("entry", doc["implicit"].to<std::string>());
  EXPECT_TRUE(IsNull(doc[Null]));
}

TEST_F(LegacyNodeSpecTest, Ex7_17_FlowMappingSeparateValues) {
  Parse(ex7_17);
  EXPECT_EQ(4, doc.size());
  EXPECT_EQ("separate", doc["unquoted"].to<std::string>());
  EXPECT_TRUE(IsNull(doc["http://foo.com"]));
  EXPECT_TRUE(IsNull(doc["omitted value"]));
  EXPECT_EQ("omitted key", doc[Null].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_18_FlowMappingAdjacentValues) {
  Parse(ex7_18);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("value", doc["adjacent"].to<std::string>());
  EXPECT_EQ("value", doc["readable"].to<std::string>());
  EXPECT_TRUE(IsNull(doc["empty"]));
}

TEST_F(LegacyNodeSpecTest, Ex7_19_SinglePairFlowMappings) {
  Parse(ex7_19);
  EXPECT_EQ(1, doc.size());
  EXPECT_EQ(1, doc[0].size());
  EXPECT_EQ("bar", doc[0]["foo"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_20_SinglePairExplicitEntry) {
  Parse(ex7_20);
  EXPECT_EQ(1, doc.size());
  EXPECT_EQ(1, doc[0].size());
  EXPECT_EQ("baz", doc[0]["foo bar"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_21_SinglePairImplicitEntries) {
  Parse(ex7_21);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ(1, doc[0].size());
  EXPECT_EQ(1, doc[0][0].size());
  EXPECT_EQ("separate", doc[0][0]["YAML"].to<std::string>());
  EXPECT_EQ(1, doc[1].size());
  EXPECT_EQ(1, doc[1][0].size());
  EXPECT_EQ("empty key entry", doc[1][0][Null].to<std::string>());
  EXPECT_EQ(1, doc[2].size());
  EXPECT_EQ(1, doc[2][0].size());
  StringMap key;
  key._["JSON"] = "like";
  EXPECT_EQ("adjacent", doc[2][0][key].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_22_InvalidImplicitKeys) {
  EXPECT_THROW_PARSER_EXCEPTION(Parse(ex7_22), ErrorMsg::END_OF_SEQ_FLOW);
}

TEST_F(LegacyNodeSpecTest, Ex7_23_FlowContent) {
  Parse(ex7_23);
  EXPECT_EQ(5, doc.size());
  EXPECT_EQ(2, doc[0].size());
  EXPECT_EQ("a", doc[0][0].to<std::string>());
  EXPECT_EQ("b", doc[0][1].to<std::string>());
  EXPECT_EQ(1, doc[1].size());
  EXPECT_EQ("b", doc[1]["a"].to<std::string>());
  EXPECT_EQ("a", doc[2].to<std::string>());
  EXPECT_EQ('b', doc[3].to<char>());
  EXPECT_EQ("c", doc[4].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex7_24_FlowNodes) {
  Parse(ex7_24);
  EXPECT_EQ(5, doc.size());
  EXPECT_EQ("tag:yaml.org,2002:str", doc[0].Tag());
  EXPECT_EQ("a", doc[0].to<std::string>());
  EXPECT_EQ('b', doc[1].to<char>());
  EXPECT_EQ("c", doc[2].to<std::string>());
  EXPECT_EQ("c", doc[3].to<std::string>());
  EXPECT_EQ("tag:yaml.org,2002:str", doc[4].Tag());
  EXPECT_EQ("", doc[4].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_1_BlockScalarHeader) {
  Parse(ex8_1);
  EXPECT_EQ(4, doc.size());
  EXPECT_EQ("literal\n", doc[0].to<std::string>());
  EXPECT_EQ(" folded\n", doc[1].to<std::string>());
  EXPECT_EQ("keep\n\n", doc[2].to<std::string>());
  EXPECT_EQ(" strip", doc[3].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_2_BlockIndentationHeader) {
  Parse(ex8_2);
  EXPECT_EQ(4, doc.size());
  EXPECT_EQ("detected\n", doc[0].to<std::string>());
  EXPECT_EQ("\n\n# detected\n", doc[1].to<std::string>());
  EXPECT_EQ(" explicit\n", doc[2].to<std::string>());
  EXPECT_EQ("\t\ndetected\n", doc[3].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_3a_InvalidBlockScalarIndentationIndicators) {
  EXPECT_THROW_PARSER_EXCEPTION(Parse(ex8_3a), ErrorMsg::END_OF_SEQ);
}

TEST_F(LegacyNodeSpecTest, Ex8_3b_InvalidBlockScalarIndentationIndicators) {
  EXPECT_THROW_PARSER_EXCEPTION(Parse(ex8_3b), ErrorMsg::END_OF_SEQ);
}

TEST_F(LegacyNodeSpecTest, Ex8_3c_InvalidBlockScalarIndentationIndicators) {
  EXPECT_THROW_PARSER_EXCEPTION(Parse(ex8_3c), ErrorMsg::END_OF_SEQ);
}

TEST_F(LegacyNodeSpecTest, Ex8_4_ChompingFinalLineBreak) {
  Parse(ex8_4);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("text", doc["strip"].to<std::string>());
  EXPECT_EQ("text\n", doc["clip"].to<std::string>());
  EXPECT_EQ("text\n", doc["keep"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, DISABLED_Ex8_5_ChompingTrailingLines) {
  Parse(ex8_5);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("# text", doc["strip"].to<std::string>());
  EXPECT_EQ("# text\n", doc["clip"].to<std::string>());
  // NOTE: I believe this is a bug in the YAML spec -
  // it should be "# text\n\n"
  EXPECT_EQ("# text\n", doc["keep"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_6_EmptyScalarChomping) {
  Parse(ex8_6);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("", doc["strip"].to<std::string>());
  EXPECT_EQ("", doc["clip"].to<std::string>());
  EXPECT_EQ("\n", doc["keep"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_7_LiteralScalar) {
  Parse(ex8_7);
  EXPECT_EQ("literal\n\ttext\n", doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_8_LiteralContent) {
  Parse(ex8_8);
  EXPECT_EQ("\n\nliteral\n \n\ntext\n", doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_9_FoldedScalar) {
  Parse(ex8_9);
  EXPECT_EQ("folded text\n", doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_10_FoldedLines) {
  Parse(ex8_10);
  EXPECT_EQ(
      "\nfolded line\nnext line\n  * bullet\n\n  * list\n  * "
      "lines\n\nlast line\n",
      doc.to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_11_MoreIndentedLines) {
  // same as 8.10
}

TEST_F(LegacyNodeSpecTest, Ex8_12_EmptySeparationLines) {
  // same as 8.10
}

TEST_F(LegacyNodeSpecTest, Ex8_13_FinalEmptyLines) {
  // same as 8.10
}

TEST_F(LegacyNodeSpecTest, Ex8_14_BlockSequence) {
  Parse(ex8_14);
  EXPECT_EQ(1, doc.size());
  EXPECT_EQ(2, doc["block sequence"].size());
  EXPECT_EQ("one", doc["block sequence"][0].to<std::string>());
  EXPECT_EQ(1, doc["block sequence"][1].size());
  EXPECT_EQ("three", doc["block sequence"][1]["two"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_15_BlockSequenceEntryTypes) {
  Parse(ex8_15);
  EXPECT_EQ(4, doc.size());
  EXPECT_TRUE(IsNull(doc[0]));
  EXPECT_EQ("block node\n", doc[1].to<std::string>());
  EXPECT_EQ(2, doc[2].size());
  EXPECT_EQ("one", doc[2][0].to<std::string>());
  EXPECT_EQ("two", doc[2][1].to<std::string>());
  EXPECT_EQ(1, doc[3].size());
  EXPECT_EQ("two", doc[3]["one"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_16_BlockMappings) {
  Parse(ex8_16);
  EXPECT_EQ(1, doc.size());
  EXPECT_EQ(1, doc["block mapping"].size());
  EXPECT_EQ("value", doc["block mapping"]["key"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_17_ExplicitBlockMappingEntries) {
  Parse(ex8_17);
  EXPECT_EQ(2, doc.size());
  EXPECT_TRUE(IsNull(doc["explicit key"]));
  EXPECT_EQ(2, doc["block key\n"].size());
  EXPECT_EQ("one", doc["block key\n"][0].to<std::string>());
  EXPECT_EQ("two", doc["block key\n"][1].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_18_ImplicitBlockMappingEntries) {
  Parse(ex8_18);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("in-line value", doc["plain key"].to<std::string>());
  EXPECT_TRUE(IsNull(doc[Null]));
  EXPECT_EQ(1, doc["quoted key"].size());
  EXPECT_EQ("entry", doc["quoted key"][0].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_19_CompactBlockMappings) {
  Parse(ex8_19);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ(1, doc[0].size());
  EXPECT_EQ("yellow", doc[0]["sun"].to<std::string>());
  EXPECT_EQ(1, doc[1].size());
  std::map<std::string, std::string> key;
  key["earth"] = "blue";
  EXPECT_EQ(1, doc[1][key].size());
  EXPECT_EQ("white", doc[1][key]["moon"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, Ex8_20_BlockNodeTypes) {
  Parse(ex8_20);
  EXPECT_EQ(3, doc.size());
  EXPECT_EQ("flow in block", doc[0].to<std::string>());
  EXPECT_EQ("Block scalar\n", doc[1].to<std::string>());
  EXPECT_EQ(1, doc[2].size());
  EXPECT_EQ("bar", doc[2]["foo"].to<std::string>());
}

TEST_F(LegacyNodeSpecTest, DISABLED_Ex8_21_BlockScalarNodes) {
  Parse(ex8_21);
  EXPECT_EQ(2, doc.size());
  // NOTE: I believe this is a bug in the YAML spec -
  // it should be "value\n"
  EXPECT_EQ("value", doc["literal"].to<std::string>());
  EXPECT_EQ("value", doc["folded"].to<std::string>());
  EXPECT_EQ("!foo", doc["folded"].Tag());
}

TEST_F(LegacyNodeSpecTest, Ex8_22_BlockCollectionNodes) {
  Parse(ex8_22);
  EXPECT_EQ(2, doc.size());
  EXPECT_EQ(2, doc["sequence"].size());
  EXPECT_EQ("entry", doc["sequence"][0].to<std::string>());
  EXPECT_EQ(1, doc["sequence"][1].size());
  EXPECT_EQ("nested", doc["sequence"][1][0].to<std::string>());
  EXPECT_EQ(1, doc["mapping"].size());
  EXPECT_EQ("bar", doc["mapping"]["foo"].to<std::string>());
}
}
}
