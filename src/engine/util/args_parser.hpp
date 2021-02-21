#include <cstring>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace Fge
{

enum class ArgsParserError
{
  OptionMutuallyExclusive,
  OptionNoValue,
  OptionUnknown,
  OptionMissing,
  OptionAlreadyAdded
};

class ArgsParserException : public std::runtime_error
{
public:
  ArgsParserException(const std::string &message, ArgsParserError error);

  const ArgsParserError error;
};

class ArgsParser
{
public:
  enum class ValueType
  {
    String
  };

  enum class OptionType
  {
    Value,
    NonValue
  };

  enum class OptionImportance
  {
    Optional,
    Required
  };

  struct Option
  {
    OptionType               type        = OptionType::NonValue;
    std::string              name        = "";
    std::string              description = "";
    OptionImportance         importance  = OptionImportance::Optional;
    std::vector<std::string> mutually_exclusive;
  };

  void add_option(const std::string &name,
                  const std::string &description,
                  OptionImportance   importance = OptionImportance::Optional,
                  const std::vector<std::string> &mutually_exclusive = {});

  void add_option_with_value(
      const std::string &             name,
      const std::string &             description,
      OptionImportance                importance = OptionImportance::Optional,
      const std::vector<std::string> &mutually_exclusive = {});

  void add_option(const Option &option);

  void parse_args(int argc, char *argv[]);

  bool is_option_set(const std::string &name) const;

  std::string get_value_as_string(const std::string &option_name) const;

  std::string format_help() const;

private:
  const std::string argument_prefix = "-";

  struct SetOption
  {
    std::string name;
    OptionType  type;
    std::string value;
  };

  std::unordered_map<std::string, Option>    allowed_options;
  std::unordered_map<std::string, SetOption> set_options;
};

} // namespace Fge
