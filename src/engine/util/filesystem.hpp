#pragma once

#include <cstdint>
#include <string>

namespace fge::fs
{

struct image
{
  unsigned char *data;
  int            width;
  int            height;
  int            channels_count;

  image() = default;

  ~image();

  image(const image &) = delete;

  void operator=(const image &) = delete;

  image(image &&other);

  void operator=(image &&other);
};

/**
 * @brief Load text file from given path.
 */
std::string load_text_file(const std::string &file_path);

image load_image(const std::string &file_path);
} // namespace fs
