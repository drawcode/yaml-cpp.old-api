#include <algorithm>
#include <memory>
#include <sstream>

#include "yaml-cpp/yaml.h"

#include "gtest/gtest.h"

#define EXPECT_THROW_EXCEPTION(statement, message) \
  ASSERT_THROW(statement, Exception);              \
  try {                                            \
    statement;                                     \
  }                                                \
  catch (const Exception& e) {                     \
    EXPECT_EQ(e.msg, message);                     \
  }

namespace YAML {
namespace {

class LegacyParserTest : public ::testing::Test {
 protected:
  void Parse(const std::string& input) {
    stream.reset(new std::stringstream(input));
    parser.reset(new Parser(*stream));
    parser->GetNextDocument(doc);
  }

  bool HasNext() { return *parser; }

  void ParseNext() { parser->GetNextDocument(doc); }

  Node doc;

 private:
  std::auto_ptr<std::stringstream> stream;
  std::auto_ptr<Parser> parser;
};

TEST_F(LegacyParserTest, SimpleSeq) {
  std::string input =
      "- eggs\n"
      "- bread\n"
      "- milk";
  Parse(input);

  ASSERT_EQ(3, doc.size());
  EXPECT_EQ("eggs", doc[0].to<std::string>());
  EXPECT_EQ("bread", doc[1].to<std::string>());
  EXPECT_EQ("milk", doc[2].to<std::string>());
}

TEST_F(LegacyParserTest, SimpleMap) {
  std::string input =
      "name: Prince Fielder\n"
      "position: 1B\n"
      "bats: L";
  Parse(input);

  EXPECT_EQ("Prince Fielder", doc["name"].to<std::string>());
  EXPECT_EQ("1B", doc["position"].to<std::string>());
  EXPECT_EQ("L", doc["bats"].to<std::string>());
}

TEST_F(LegacyParserTest, FlowSeq) {
  std::string input = "[ 2 , 3, 5  ,  7,   11]";
  Parse(input);

  EXPECT_EQ(2, doc[0].to<int>());
  EXPECT_EQ(3, doc[1].to<int>());
  EXPECT_EQ(5, doc[2].to<int>());
  EXPECT_EQ(7, doc[3].to<int>());
  EXPECT_EQ(11, doc[4].to<int>());
}

TEST_F(LegacyParserTest, FlowMap) {
  std::string input = "{hr: 65, avg: 0.278}";
  Parse(input);

  EXPECT_EQ("65", doc["hr"].to<std::string>());
  EXPECT_EQ("0.278", doc["avg"].to<std::string>());
}

TEST_F(LegacyParserTest, FlowMapWithOmittedKey) {
  std::string input = "{: omitted key}";
  Parse(input);

  EXPECT_EQ("omitted key", doc[YAML::Null].to<std::string>());
}

TEST_F(LegacyParserTest, FlowMapWithOmittedValue) {
  std::string input = "{a: b, c:, d:}";
  Parse(input);

  EXPECT_EQ("b", doc["a"].to<std::string>());
  EXPECT_TRUE(IsNull(doc["c"]));
  EXPECT_TRUE(IsNull(doc["d"]));
}

TEST_F(LegacyParserTest, FlowMapWithSoloEntry) {
  std::string input = "{a: b, c, d: e}";
  Parse(input);

  EXPECT_EQ("b", doc["a"].to<std::string>());
  EXPECT_TRUE(IsNull(doc["c"]));
  EXPECT_EQ("e", doc["d"].to<std::string>());
}

TEST_F(LegacyParserTest, FlowMapEndingWithSoloEntry) {
  std::string input = "{a: b, c}";
  Parse(input);

  EXPECT_EQ("b", doc["a"].to<std::string>());
  EXPECT_TRUE(IsNull(doc["c"]));
}

TEST_F(LegacyParserTest, QuotedSimpleKeys) {
  std::string KeyValue[3] = {"\"double\": double\n", "'single': single\n",
                             "plain: plain\n"};

  int perm[3] = {0, 1, 2};
  do {
    std::string input =
        KeyValue[perm[0]] + KeyValue[perm[1]] + KeyValue[perm[2]];
    Parse(input);

    EXPECT_EQ("double", doc["double"].to<std::string>());
    EXPECT_EQ("single", doc["single"].to<std::string>());
    EXPECT_EQ("plain", doc["plain"].to<std::string>());
  } while (std::next_permutation(perm, perm + 3));
}

TEST_F(LegacyParserTest, CompressedMapAndSeq) {
  std::string input = "key:\n- one\n- two";
  Parse(input);

  const YAML::Node& seq = doc["key"];
  ASSERT_EQ(2, seq.size());

  EXPECT_EQ("one", seq[0].to<std::string>());
  EXPECT_EQ("two", seq[1].to<std::string>());
}

TEST_F(LegacyParserTest, NullBlockSeqEntry) {
  std::string input = "- hello\n-\n- world";
  Parse(input);

  EXPECT_EQ("hello", doc[0].to<std::string>());
  EXPECT_TRUE(IsNull(doc[1]));
  EXPECT_EQ("world", doc[2].to<std::string>());
}

TEST_F(LegacyParserTest, NullBlockMapKey) {
  std::string input = ": empty key";
  Parse(input);

  EXPECT_EQ("empty key", doc[YAML::Null].to<std::string>());
}

TEST_F(LegacyParserTest, NullBlockMapValue) {
  std::string input = "empty value:";
  Parse(input);

  EXPECT_TRUE(IsNull(doc["empty value"]));
}

TEST_F(LegacyParserTest, SimpleAlias) {
  std::string input = "- &alias test\n- *alias";
  Parse(input);

  ASSERT_EQ(2, doc.size());
  EXPECT_EQ("test", doc[0].to<std::string>());
  EXPECT_EQ("test", doc[1].to<std::string>());
}

TEST_F(LegacyParserTest, AliasWithNull) {
  std::string input = "- &alias\n- *alias";
  Parse(input);

  ASSERT_EQ(2, doc.size());
  EXPECT_TRUE(IsNull(doc[0]));

  EXPECT_TRUE(IsNull(doc[1]));
}

TEST_F(LegacyParserTest, AnchorInSimpleKey) {
  std::string input = "- &a b: c\n- *a";
  Parse(input);

  ASSERT_EQ(2, doc.size());
  EXPECT_EQ("c", doc[0]["b"].to<std::string>());
  EXPECT_EQ("b", doc[1].to<std::string>());
}

TEST_F(LegacyParserTest, AliasAsSimpleKey) {
  std::string input = "- &a b\n- *a : c";
  Parse(input);

  ASSERT_EQ(2, doc.size());
  EXPECT_EQ("b", doc[0].to<std::string>());
  EXPECT_EQ("c", doc[1]["b"].to<std::string>());
}

TEST_F(LegacyParserTest, ExplicitDoc) {
  std::string input = "---\n- one\n- two";
  Parse(input);

  ASSERT_EQ(2, doc.size());
  EXPECT_EQ("one", doc[0].to<std::string>());
  EXPECT_EQ("two", doc[1].to<std::string>());
}

TEST_F(LegacyParserTest, MultipleDocs) {
  std::string input = "---\nname: doc1\n---\nname: doc2";
  Parse(input);

  EXPECT_EQ("doc1", doc["name"].to<std::string>());

  ASSERT_TRUE(HasNext());

  ParseNext();
  EXPECT_EQ("doc2", doc["name"].to<std::string>());
}

TEST_F(LegacyParserTest, ExplicitEndDoc) {
  std::string input = "- one\n- two\n...\n...";
  Parse(input);

  ASSERT_EQ(2, doc.size());

  EXPECT_EQ("one", doc[0].to<std::string>());
  EXPECT_EQ("two", doc[1].to<std::string>());
}

TEST_F(LegacyParserTest, MultipleDocsWithSomeExplicitIndicators) {
  std::string input =
      "- one\n- two\n...\n"
      "---\nkey: value\n...\n...\n"
      "- three\n- four\n"
      "---\nkey: value";
  Parse(input);

  ASSERT_EQ(2, doc.size());
  EXPECT_EQ("one", doc[0].to<std::string>());
  EXPECT_EQ("two", doc[1].to<std::string>());

  ParseNext();
  EXPECT_EQ("value", doc["key"].to<std::string>());

  ParseNext();
  ASSERT_EQ(2, doc.size());
  EXPECT_EQ("three", doc[0].to<std::string>());
  EXPECT_EQ("four", doc[1].to<std::string>());

  ParseNext();
  EXPECT_EQ("value", doc["key"].to<std::string>());
}

TEST_F(LegacyParserTest, BlockKeyWithNullValue) {
  std::string input =
      "key:\n"
      "just a key: value";
  Parse(input);

  ASSERT_EQ(2, doc.size());
  EXPECT_TRUE(IsNull(doc["key"]));
  EXPECT_EQ("value", doc["just a key"].to<std::string>());
}

TEST_F(LegacyParserTest, Bases) {
  std::string input =
      "- 15\n"
      "- 0x10\n"
      "- 030\n"
      "- 0xffffffff\n";
  Parse(input);

  ASSERT_EQ(4, doc.size());
  EXPECT_EQ(15, doc[0].to<int>());
  EXPECT_EQ(0x10, doc[1].to<int>());
  EXPECT_EQ(030, doc[2].to<int>());
  EXPECT_EQ(0xffffffff, doc[3].to<unsigned>());
}

TEST_F(LegacyParserTest, KeyNotFound) {
  std::string input = "key: value";
  Parse(input);

  EXPECT_THROW_EXCEPTION(
      doc["bad key"], std::string(YAML::ErrorMsg::KEY_NOT_FOUND) + ": bad key");

  EXPECT_THROW_EXCEPTION(doc[5],
                         std::string(YAML::ErrorMsg::KEY_NOT_FOUND) + ": 5");

  EXPECT_THROW_EXCEPTION(doc[2.5],
                         std::string(YAML::ErrorMsg::KEY_NOT_FOUND) + ": 2.5");
}

TEST_F(LegacyParserTest, DuplicateKey) {
  std::string input = "{a: 1, b: 2, c: 3, a: 4}";
  Parse(input);

  EXPECT_EQ(4, doc["a"].to<int>());
  EXPECT_EQ(2, doc["b"].to<int>());
  EXPECT_EQ(3, doc["c"].to<int>());
}

TEST_F(LegacyParserTest, DefaultPlainScalarTag) {
  Parse("--- 12");
  EXPECT_EQ("?", doc.Tag());
}

TEST_F(LegacyParserTest, DefaultSingleQuotedScalarTag) {
  Parse("--- '12'");
  EXPECT_EQ("!", doc.Tag());
}

TEST_F(LegacyParserTest, ExplicitNonSpecificPlainScalarTag) {
  Parse("--- ! 12");
  EXPECT_EQ("!", doc.Tag());
}

TEST_F(LegacyParserTest, BasicLocalTag) {
  Parse("--- !foo 12");
  EXPECT_EQ("!foo", doc.Tag());
}

TEST_F(LegacyParserTest, VerbatimLocalTag) {
  Parse("--- !<!foo> 12");
  EXPECT_EQ("!foo", doc.Tag());
}

TEST_F(LegacyParserTest, StandardShortcutTag) {
  Parse("--- !!int 12");
  EXPECT_EQ("tag:yaml.org,2002:int", doc.Tag());
}

TEST_F(LegacyParserTest, VerbatimURITag) {
  Parse("--- !<tag:yaml.org,2002:int> 12");
  EXPECT_EQ("tag:yaml.org,2002:int", doc.Tag());
}

TEST_F(LegacyParserTest, DefaultSequenceTag) {
  Parse("--- [12]");
  EXPECT_EQ("?", doc.Tag());
}

TEST_F(LegacyParserTest, ExplicitNonSpecificSequenceTag) {
  Parse("--- ! [12]");
  EXPECT_EQ("!", doc.Tag());
}

TEST_F(LegacyParserTest, Infinity) {
  std::string input =
      "- .inf\n"
      "- .Inf\n"
      "- .INF\n"
      "- +.inf\n"
      "- +.Inf\n"
      "- +.INF\n"
      "- -.inf\n"
      "- -.Inf\n"
      "- -.INF\n";
  Parse(input);

  for (unsigned i = 0; i < doc.size(); i++) {
    EXPECT_EQ((i < 6 ? +1 : -1) * std::numeric_limits<double>::infinity(),
              doc[i].to<double>());
  }

  for (unsigned i = 0; i < doc.size(); i++) {
    EXPECT_EQ((i < 6 ? +1 : -1) * std::numeric_limits<long double>::infinity(),
              doc[i].to<long double>());
  }

  for (unsigned i = 0; i < doc.size(); i++) {
    EXPECT_EQ((i < 6 ? +1 : -1) * std::numeric_limits<float>::infinity(),
              doc[i].to<float>());
  }
}

TEST_F(LegacyParserTest, NaN) {
  std::string input =
      "- .nan\n"
      "- .NaN\n"
      "- .NAN\n";
  Parse(input);

  for (unsigned i = 0; i < doc.size(); i++) {
    double d;
    doc[i] >> d;
    EXPECT_FALSE(d == d);
  }
  for (unsigned i = 0; i < doc.size(); i++) {
    long double d;
    doc[i] >> d;
    EXPECT_FALSE(d == d);
  }
  for (unsigned i = 0; i < doc.size(); i++) {
    float d;
    doc[i] >> d;
    EXPECT_FALSE(d == d);
  }
}

TEST_F(LegacyParserTest, NonConstKey) {
  std::string input = "{a: 1}";
  Parse(input);

  std::vector<char> key(2);
  key[0] = 'a';
  key[1] = '\0';
  EXPECT_EQ(1, doc[&key[0]].to<int>());
}

TEST_F(LegacyParserTest, SingleChar) {
  std::string input = "5";
  Parse(input);

  EXPECT_EQ(5, doc.to<int>());
}

TEST_F(LegacyParserTest, QuotedNewline) {
  std::string input = "foo: \"\\n\"";
  Parse(input);

  EXPECT_EQ("\n", doc["foo"].to<std::string>());
}

TEST_F(LegacyParserTest, DoubleAsInt) {
  std::string input = "1.5";
  Parse(input);

  EXPECT_THROW(doc.to<int>(), InvalidScalar);
}

TEST_F(LegacyParserTest, Binary) {
  std::string input =
      "[!!binary \"SGVsbG8sIFdvcmxkIQ==\", !!binary "
      "\"TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieS"
      "B0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIG"
      "x1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbi"
      "B0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZG"
      "dlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS"
      "4K\"]";
  Parse(input);

  Binary binary0(reinterpret_cast<const unsigned char*>("Hello, World!"), 13);
  Binary binary1(
      reinterpret_cast<const unsigned char*>(
          "Man is distinguished, not only by his reason, but by this "
          "singular passion from other animals, which is a lust of the "
          "mind, that by a perseverance of delight in the continued and "
          "indefatigable generation of knowledge, exceeds the short "
          "vehemence of any carnal pleasure.\n"),
      270);

  ASSERT_EQ(2, doc.size());
  EXPECT_EQ(binary0, doc[0].to<Binary>());
  EXPECT_EQ(binary1, doc[1].to<Binary>());
}

TEST_F(LegacyParserTest, DereferenceScalar) {
  std::string input = "key";
  Parse(input);

  EXPECT_THROW(*doc.begin(), DereferenceScalarError);
}

TEST_F(LegacyParserTest, DereferenceKeyScalar) {
  std::string input = "key";
  Parse(input);

  EXPECT_THROW(doc.begin().first(), DereferenceKeyScalarError);
}

TEST_F(LegacyParserTest, DereferenceValueScalar) {
  std::string input = "key";
  Parse(input);

  EXPECT_THROW(doc.begin().second(), DereferenceValueScalarError);
}

TEST_F(LegacyParserTest, DereferenceKeySeq) {
  std::string input = "[key]";
  Parse(input);

  EXPECT_THROW(doc.begin().first(), DereferenceKeySeqError);
}

TEST_F(LegacyParserTest, DereferenceValueSeq) {
  std::string input = "[key]";
  Parse(input);

  EXPECT_THROW(doc.begin().second(), DereferenceValueSeqError);
}

TEST_F(LegacyParserTest, DereferenceMap) {
  std::string input = "{key: value}";
  Parse(input);

  EXPECT_THROW(*doc.begin(), DereferenceMapError);
}
}
}
