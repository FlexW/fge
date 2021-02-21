#include <gtest/gtest.h>

#include "tests_common.hpp"
#include "util/args_parser.hpp"

using namespace Fge;

TEST(ArgsParserTest, addOption_OptionAlreadyAdded_ThrowException)
{
  ArgsParser args_parser;

  args_parser.add_option("sampleoption",
                         "Description",
                         Fge::ArgsParser::OptionImportance::Optional);

  Tests::assert_exception<ArgsParserException>(
      [&] {
        args_parser.add_option("sampleoption",
                               "Description",
                               Fge::ArgsParser::OptionImportance::Optional);
      },
      "No ArgsParserException thrown!",
      [](const ArgsParserException &exception) {
        EXPECT_EQ(exception.error, ArgsParserError::OptionAlreadyAdded);
      });

  Tests::assert_exception<ArgsParserException>(
      [&] {
        args_parser.add_option_with_value(
            "sampleoption",
            "Description",
            Fge::ArgsParser::OptionImportance::Optional);
      },
      "No ArgsParserException thrown!",
      [](const ArgsParserException &exception) {
        EXPECT_EQ(exception.error, ArgsParserError::OptionAlreadyAdded);
      });

  Tests::assert_exception<ArgsParserException>(
      [&] {
        ArgsParser::Option option;
        option.name = "sampleoption";
        args_parser.add_option(option);
      },
      "No ArgsParserException thrown!",
      [](const ArgsParserException &exception) {
        EXPECT_EQ(exception.error, ArgsParserError::OptionAlreadyAdded);
      });
}

TEST(ArgsParserTest, ParseArgs_OptionalOptionSet_DetectOption)
{
  ArgsParser args_parser;

  args_parser.add_option("sampleoption",
                         "Description",
                         Fge::ArgsParser::OptionImportance::Optional);

  const char *argv[] = {"app_name", "-sampleoption"};
  args_parser.parse_args(2, argv);

  EXPECT_TRUE(args_parser.is_option_set("sampleoption"));
}

TEST(ArgsParserTest, ParseArgs_UnkownOptionSet_ThrowException)
{
  ArgsParser args_parser;

  args_parser.add_option("sampleoption",
                         "Description",
                         Fge::ArgsParser::OptionImportance::Optional);

  const char *argv[] = {"app_name", "-sampleoption", "-anotheroption"};

  Tests::assert_exception<ArgsParserException>(
      [&] { args_parser.parse_args(3, argv); },
      "No ArgsParserException thrown!",
      [](const ArgsParserException &exception) {
        EXPECT_EQ(exception.error, ArgsParserError::OptionUnknown);
      });
}

TEST(ArgsParserTest, ParseArgs_RequiredOptionNotSet_ThrowException)
{
  ArgsParser args_parser;

  args_parser.add_option("sampleoption",
                         "Description",
                         Fge::ArgsParser::OptionImportance::Required);

  args_parser.add_option("anotheroption",
                         "Description",
                         Fge::ArgsParser::OptionImportance::Required);

  const char *argv[] = {"app_name", "-anotheroption"};

  Tests::assert_exception<ArgsParserException>(
      [&] { args_parser.parse_args(1, argv); },
      "No ArgsParserException thrown!",
      [](const ArgsParserException &exception) {
        EXPECT_EQ(exception.error, ArgsParserError::OptionMissing);
      });
}

TEST(ArgsParserTest, ParseArgs_MutuallyExclusiveOptionsSet_ThrowException)
{
  ArgsParser args_parser;

  args_parser.add_option("sampleoption",
                         "Description",
                         Fge::ArgsParser::OptionImportance::Optional,
                         {"anotheroption"});

  args_parser.add_option("anotheroption",
                         "Description",
                         Fge::ArgsParser::OptionImportance::Optional,
                         {"sampleoption"});

  const char *argv[] = {"app_name", "-anotheroption", "-sampleoption"};

  Tests::assert_exception<ArgsParserException>(
      [&] { args_parser.parse_args(3, argv); },
      "No ArgsParserException thrown!",
      [](const ArgsParserException &exception) {
        EXPECT_EQ(exception.error, ArgsParserError::OptionMutuallyExclusive);
      });
}

TEST(ArgsParserTest, ParseArgs_NoValueForOptionSet_ThrowException)
{
  ArgsParser args_parser;

  args_parser.add_option_with_value(
      "sampleoption",
      "Description",
      Fge::ArgsParser::OptionImportance::Required);

  const char *argv[] = {"app_name", "-sampleoption"};

  Tests::assert_exception<ArgsParserException>(
      [&] { args_parser.parse_args(2, argv); },
      "No ArgsParserException thrown!",
      [](const ArgsParserException &exception) {
        EXPECT_EQ(exception.error, ArgsParserError::OptionNoValue);
      });
}

TEST(ArgsParserTest, GetValueAsString_OptionWithValueSet_GetValue)
{
  ArgsParser args_parser;

  args_parser.add_option_with_value(
      "sampleoption",
      "Description",
      Fge::ArgsParser::OptionImportance::Required);

  const char *argv[] = {
      "app_name",
      "-sampleoption",
      "value",
  };

  args_parser.parse_args(3, argv);

  EXPECT_TRUE(args_parser.is_option_set("sampleoption"));
  EXPECT_EQ(args_parser.get_value_as_string("sampleoption"), "value");
}

TEST(ArgsParserTest, GetValueAsString_OptionNotSet_ThrowException)
{
  ArgsParser args_parser;

  args_parser.add_option_with_value(
      "sampleoption",
      "Description",
      Fge::ArgsParser::OptionImportance::Required);

  const char *argv[] = {
      "app_name",
      "-sampleoption",
      "value",
  };

  args_parser.parse_args(3, argv);

  Tests::assert_exception<ArgsParserException>(
      [&] { args_parser.get_value_as_string("anotheroption"); });
}

TEST(ArgsParserTest, ParseArgs_OptionsCorrectSet_DontThrowException)
{
  ArgsParser args_parser;

  args_parser.add_option_with_value(
      "sampleoption",
      "Description",
      Fge::ArgsParser::OptionImportance::Required);

  args_parser.add_option("anotheroption",
                         "Description",
                         Fge::ArgsParser::OptionImportance::Required);

  args_parser.add_option("optionaloption",
                         "Description",
                         Fge::ArgsParser::OptionImportance::Optional);

  args_parser.add_option("exclusive1option",
                         "Description",
                         Fge::ArgsParser::OptionImportance::Optional,
                         {"exclusive2option"});

  args_parser.add_option("exclusive2option",
                         "Description",
                         Fge::ArgsParser::OptionImportance::Optional,
                         {"exclusive1option"});

  const char *argv[] = {
      "app_name",
      "-anotheroption",
      "-sampleoption",
      "value",
      "-exclusive2option",
  };

  args_parser.parse_args(5, argv);

  EXPECT_TRUE(args_parser.is_option_set("sampleoption"));
  EXPECT_TRUE(args_parser.is_option_set("anotheroption"));
  EXPECT_TRUE(args_parser.is_option_set("exclusive2option"));
  EXPECT_EQ(args_parser.get_value_as_string("sampleoption"), "value");
}
