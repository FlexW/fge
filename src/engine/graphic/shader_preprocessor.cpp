#include "shader_preprocessor.hpp"
#include "shader.hpp"

namespace Fge
{

std::string
get_sub_string(const std::string &str, const size_t start_index, size_t count)
{
  return str.substr(start_index, count);
}

std::string strip_file_from_path(const std::string &path_with_file)
{
  std::string path{};

  size_t path_with_file_end = path_with_file.length();

  size_t i = 0;
  while (path_with_file_end - i > 0 &&
         path_with_file[path_with_file_end - i] != '/')
  {
    ++i;
  }

  return get_sub_string(path_with_file, 0, path_with_file_end - i + 1);
}

ShaderPreprocessor::ShaderPreprocessor(
    const std::vector<std::string> &absolute_include_base_paths)
    : absolute_include_base_paths(absolute_include_base_paths)
{
  for (auto &include : absolute_include_base_paths)
  {
    if (!(include[include.length() - 1] == '/'))
    {
      std::cout << "Absolute include must end with /\n";
      std::exit(EXIT_FAILURE);
    }
  }
}

void ShaderPreprocessor::set_define(const std::string &define)
{
  defines.push_back(define);
}

bool match(const std::string  str,
           const size_t       start_index,
           const std::string &word_match)
{
  assert(start_index < str.length());
  assert(word_match.length() > 0);

  size_t i = 0;
  while (start_index + i < str.length() &&
         str[start_index + i] == word_match[i])
  {
    ++i;
  }

  if (i == word_match.length())
  {
    return true;
  }

  return false;
}

enum class IncludeType
{
  Absolute,
  Relative
};

struct Include
{
  IncludeType include_type{};
  std::string path{};
  size_t      include_begin{};
  size_t      include_end{};
  size_t      length{};
};

std::string
ShaderPreprocessor::insert_include_content(const std::string &shader_code,
                                           size_t             include_begin,
                                           size_t             include_end,
                                           const std::string &include_content)
{
  auto before_include = get_sub_string(shader_code, 0, include_begin);

  // assert(shader_code[include_end + 1] == '\n');

  if (include_end + 2 < shader_code.length())
  {
    auto after_include =
        get_sub_string(shader_code, include_end + 2, shader_code.length());
    // return before_include + "\n" + include_content + "\n" + after_include;
    return before_include + include_content + after_include;
  }

  // return before_include + "\n" + include_content + "\n";
  return before_include + include_content;
}

std::optional<std::string> get_file_content(const std::string &filepath)
{
  std::ifstream ifs(filepath);
  if (!ifs.is_open())
  {
    return {};
  }
  std::string str((std::istreambuf_iterator<char>(ifs)),
                  std::istreambuf_iterator<char>());
  return str;
}

std::string ShaderPreprocessor::resolve_includes(const std::string &shader_path,
                                                 const std::string &shader_code)
{
  std::vector<Include> includes;
  size_t               current_line = 0;

  assert(shader_path[shader_path.length() - 1] == '/');

  for (size_t i = 0; i < shader_code.length(); ++i)
  {
    if (shader_code[i] == '\n')
    {
      ++current_line;
    }

    if (i == 0 || shader_code[i - 1] == '\n')
    {
      if (match(shader_code, i, "#include"))
      {
        size_t length = 8;

        Include include{};
        include.include_begin = i;

        // Get path
        size_t j = i + 8;
        while (j < shader_code.length() &&
               (shader_code[j] == ' ' || shader_code[j] == '\t'))
        {
          ++j;
          ++length;
        }

        char include_end_sign;
        if (shader_code[j] == '"')
        {
          include.include_type = IncludeType::Relative;
          include_end_sign     = '"';
        }
        else if (shader_code[j] == '<')
        {
          include.include_type = IncludeType::Absolute;
          include_end_sign     = '>';
        }
        else
        {
          throw std::runtime_error("Misformed include on line " +
                                   std::to_string(current_line));
        }

        size_t       k          = 0;
        const size_t path_begin = j + 1;
        while (path_begin + k < shader_code.length() &&
               shader_code[path_begin + k] != include_end_sign)
        {
          ++k;
          ++length;
        }
        if (!(path_begin + k < shader_code.length() &&
              shader_code[path_begin + k] == include_end_sign))
        {
          throw std::runtime_error("Misformed include on line " +
                                   std::to_string(current_line));
        }

        include.include_end = path_begin + k;

        include.path = get_sub_string(shader_code, path_begin, k);

        include.length = length;

        includes.push_back(include);
      }
    }
  }

  size_t      offset          = 0;
  std::string new_shader_code = shader_code;
  for (auto &include : includes)
  {
    std::string include_content;
    switch (include.include_type)
    {
    case IncludeType::Absolute:
    {
      bool found = false;
      for (auto &absolute_include_path : absolute_include_base_paths)
      {
        auto content = get_file_content(absolute_include_path + include.path);
        if (content.has_value())
        {
          auto content_to_include = content.value();

          include_content = resolve_includes(
              strip_file_from_path(absolute_include_path + include.path),
              content_to_include);
          found = true;
          break;
        }
      }
      if (!found)
      {
        throw std::runtime_error("Absolute included file " + include.path +
                                 " not found");
      }
    }
    break;

    case IncludeType::Relative:
    {
      auto content = get_file_content(shader_path + include.path);
      if (content.has_value())
      {
        auto content_to_include = content.value();

        include_content =
            resolve_includes(strip_file_from_path(shader_path + include.path),
                             content_to_include);
        break;
      }
      throw std::runtime_error("Relative included file " + include.path +
                               " not found");
    }
    break;

    default:
      assert(0 && "No such include type");
    }

    auto old_shader_code = new_shader_code;
    new_shader_code      = insert_include_content(new_shader_code,
                                             include.include_begin + offset,
                                             include.include_end + offset,
                                             include_content);
    offset += include_content.length() - include.length - 3;
  }

  return new_shader_code;
}

std::string ShaderPreprocessor::add_defines(const std::string &shader_code)
{
  uint32_t i = 0;
  while (shader_code[i] != '\n')
  {
    ++i;
  }
  if (shader_code.length() == i)
  {
    assert(0 && "Just one line in shader code");
  }

  std::string define_string;
  for (auto &define : defines)
  {
    define_string += "#define " + define + "\n";
  }

  auto version_string = get_sub_string(shader_code, 0, i + 2);
  auto after_version = get_sub_string(shader_code, i + 1, shader_code.length());

  return version_string + define_string + after_version;
}

std::string ShaderPreprocessor::preprocess(const std::string &shader_path,
                                           const std::string &shader_code)
{
  auto new_shader_code = resolve_includes(shader_path, shader_code);
  new_shader_code      = add_defines(new_shader_code);

  return new_shader_code;
}

} // namespace Fge
