#include "yaml-cpp/yaml.h"

#include "gtest/gtest.h"

namespace YAML {
namespace {

class LegacyScalarParserTest : public ::testing::Test {
 protected:
  void ExpectScalar(const std::string& input,
                    const std::string& expectedOutput) {
    std::stringstream stream(input);
    Parser parser(stream);
    Node doc;
    parser.GetNextDocument(doc);

    std::string output;
    doc >> output;
    EXPECT_EQ(expectedOutput, output);
  }
};

TEST_F(LegacyScalarParserTest, SimpleScalar) {
  ExpectScalar("Hello, World!", "Hello, World!");
}

TEST_F(LegacyScalarParserTest, MultiLineScalar) {
  ExpectScalar(
      "normal scalar, but\n"
      "over several lines",
      "normal scalar, but over several lines");
}

TEST_F(LegacyScalarParserTest, LiteralScalar) {
  ExpectScalar(
      "|\n"
      " literal scalar - so we can draw ASCII:\n"
      " \n"
      "          -   -\n"
      "         |  -  |\n"
      "          -----\n",
      "literal scalar - so we can draw ASCII:\n"
      "\n"
      "         -   -\n"
      "        |  -  |\n"
      "         -----\n");
}

TEST_F(LegacyScalarParserTest, FoldedScalar) {
  ExpectScalar(
      ">\n"
      " and a folded scalar... so we\n"
      " can just keep writing various\n"
      " things. And if we want to keep indentation:\n"
      " \n"
      "    we just indent a little\n"
      "    see, this stays indented",
      "and a folded scalar... so we"
      " can just keep writing various"
      " things. And if we want to keep indentation:\n"
      "\n"
      "   we just indent a little\n"
      "   see, this stays indented");
}

TEST_F(LegacyScalarParserTest, ChompedFoldedScalar) {
  ExpectScalar(
      ">-\n"
      "  Here's a folded scalar\n"
      "  that gets chomped.",
      "Here's a folded scalar"
      " that gets chomped.");
}

TEST_F(LegacyScalarParserTest, ChompedLiteralScalar) {
  ExpectScalar(
      "|-\n"
      "  Here's a literal scalar\n"
      "  that gets chomped.",
      "Here's a literal scalar\n"
      "that gets chomped.");
}

TEST_F(LegacyScalarParserTest, FoldedScalarWithIndent) {
  ExpectScalar(
      ">2\n"
      "       Here's a folded scalar\n"
      "  that starts with some indentation.",
      "     Here's a folded scalar\n"
      "that starts with some indentation.");
}

TEST_F(LegacyScalarParserTest, ColonScalar) {
  ExpectScalar("::vector", "::vector");
}

TEST_F(LegacyScalarParserTest, QuotedScalar) {
  ExpectScalar("\": - ()\"", ": - ()");
}

TEST_F(LegacyScalarParserTest, CommaScalar) {
  ExpectScalar("Up, up, and away!", "Up, up, and away!");
}

TEST_F(LegacyScalarParserTest, DashScalar) { ExpectScalar("-123", "-123"); }

TEST_F(LegacyScalarParserTest, URLScalar) {
  ExpectScalar("http://example.com/foo#bar", "http://example.com/foo#bar");
}
}
}
