#include "args_parser.hpp"

namespace Fge
{

ArgsParserException::ArgsParserException(const std::string &message,
                                         ArgsParserError    error)
    : std::runtime_error(message),
      error(error)
{
}

void ArgsParser::add_option(const std::string &             name,
                            const std::string &             description,
                            OptionImportance                importance,
                            const std::vector<std::string> &mutually_exclusive)
{
  Option option{};
  option.name               = name;
  option.description        = description;
  option.type               = OptionType::NonValue;
  option.importance         = importance;
  option.mutually_exclusive = mutually_exclusive;

  add_option(option);
}

void ArgsParser::add_option_with_value(
    const std::string &             name,
    const std::string &             description,
    OptionImportance                importance,
    const std::vector<std::string> &mutually_exclusive)
{
  Option option{};
  option.name               = name;
  option.description        = description;
  option.type               = OptionType::Value;
  option.importance         = importance;
  option.mutually_exclusive = mutually_exclusive;

  add_option(option);
}

void ArgsParser::add_option(const Option &option)
{
  const auto option_name_with_prefix = argument_prefix + option.name;

  if (allowed_options.find(option_name_with_prefix) != allowed_options.end())
  {
    throw ArgsParserException("Option " + option.name + " already added!",
                              ArgsParserError::OptionAlreadyAdded);
  }

  allowed_options[option_name_with_prefix] = option;
}

void ArgsParser::parse_args(int argc, const char *argv[])
{
  for (int i = 1; i < argc; ++i)
  {
    const auto argument = argv[i];

    const auto allowed_options_iter = allowed_options.find(argument);
    if (allowed_options_iter != allowed_options.end())
    {
      check_if_mutually_exclusive_option_set(argument);

      const auto allowed_option = allowed_options_iter->second;

      SetOption set_option;
      set_option.type = OptionType::NonValue;
      set_option.name = allowed_option.name;

      // Check if option requires a value
      if (does_option_require_value(allowed_option))
      {
        set_option.type = OptionType::Value;

        if (argc <= i + 1)
        {
          throw ArgsParserException(
              std::string("Can not read value for option ") + argument,
              ArgsParserError::OptionNoValue);
        }

        const auto value = argv[i + 1];
        set_option.value = value;

        ++i;
      }

      set_options[set_option.name] = set_option;

      continue;
    }

    throw ArgsParserException(std::string("Unknown option ") + argument,
                              ArgsParserError::OptionUnknown);
  }

  check_required_options_have_been_set();
}

bool ArgsParser::is_option_set(const std::string &name) const
{
  return set_options.find(name) != set_options.end();
}

std::string
ArgsParser::get_value_as_string(const std::string &option_name) const
{
  const auto set_options_iter = set_options.find(option_name);

  if (set_options_iter == set_options.end())
  {
    throw ArgsParserException("Option is not set",
                              ArgsParserError::OptionMissing);
  }

  return set_options_iter->second.value;
}

std::string ArgsParser::format_help() const
{
  std::stringstream help;
  for (const auto &p : allowed_options)
  {
    const auto option = p.second;
    help << argument_prefix << option.name << "\n\t" << option.description
         << "\n";

    if (option.type == OptionType::Value)
    {

      help << "\tNeeds a value to be set\n";
    }

    help << "\t";
    if (option.importance == OptionImportance::Required)
    {
      help << "Required";
    }
    else
    {
      help << "Optional";
    }
    help << "\n";

    if (option.mutually_exclusive.size() > 0)
    {
      help << "\tMutually exclusive with:\n\t\t";
      uint32_t i = 0;
      for (const auto &mutually_exclusive_option : option.mutually_exclusive)
      {
        if (i != 0)
        {
          help << ", ";
        }
        help << mutually_exclusive_option;
      }
      help << "\n";
    }
  }

  return help.str();
}

void ArgsParser::check_if_mutually_exclusive_option_set(
    const std::string &argument)
{
  for (const auto &allowed_option : allowed_options)
  {
    for (const auto &mutually_exclusive_option_name :
         allowed_option.second.mutually_exclusive)
    {
      if (argument_prefix + mutually_exclusive_option_name == argument &&
          is_option_set(allowed_option.second.name))
      {
        // Error argument is mutually exclusive
        throw ArgsParserException(std::string("Option ") + argument +
                                      " is mutually exclusive with option " +
                                      argument_prefix +
                                      allowed_option.second.name,
                                  ArgsParserError::OptionMutuallyExclusive);
      }
    }
  }
}

bool ArgsParser::does_option_require_value(const Option &allowed_option)
{
  return allowed_option.type == OptionType::Value;
}

void ArgsParser::check_required_options_have_been_set()
{
  for (const auto &allowed_option : allowed_options)
  {
    if (allowed_option.second.importance == OptionImportance::Required)
    {
      if (set_options.find(allowed_option.second.name) == set_options.end())
      {
        // Error required Option has not been set
        throw ArgsParserException("Required option " + argument_prefix +
                                      allowed_option.second.name +
                                      " has not been set",
                                  ArgsParserError::OptionMissing);
      }
    }
  }
}

} // namespace Fge
