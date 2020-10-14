#pragma once

#include "std.hpp"

namespace Fge
{

class ShaderPreprocessor
{
public:
  ShaderPreprocessor(
      const std::vector<std::string> &absolute_include_base_paths);

  void set_define(const std::string &define);

  std::string preprocess(const std::string &shader_path,
                         const std::string &shader_code);

private:
  std::vector<std::string> absolute_include_base_paths;
  std::vector<std::string> defines;

  std::string resolve_includes(const std::string &shader_path,
                               const std::string &shader_code);

  std::string insert_include_content(const std::string &shader_code,
                                     size_t             include_begin,
                                     size_t             include_end,
                                     const std::string &include_content);

  std::string add_defines(const std::string &shader_code);
};

} // namespace Fge
