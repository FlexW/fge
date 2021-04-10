#include <fstream>
#include <sstream>
#include <stdexcept>

#include <stb_image.h>

#include "assert.hpp"
#include "filesystem.hpp"
#include "resource_root.hpp"

namespace fge::fs
{

image::~image()
{
  if (data)
  {
    stbi_image_free(data);
  }
}

image::image(image &&other)
{
  data           = other.data;
  height         = other.height;
  width          = other.width;
  channels_count = other.channels_count;
  other.data     = nullptr;
}

void image::operator=(image &&other)
{
  data           = other.data;
  height         = other.height;
  width          = other.width;
  channels_count = other.channels_count;
  other.data     = nullptr;
}

std::string load_text_file(const std::string &file_path)
{
  std::ifstream ifs((logl_root + file_path).c_str());
  if (!ifs.is_open())
  {
    throw std::runtime_error("Could not open file: " + (logl_root + file_path));
  }
  std::string content(
      static_cast<std::stringstream const &>(std::stringstream() << ifs.rdbuf())
          .str());
  ifs.close();

  return content;
}

image load_image(const std::string &file_path)
{
  image img;

  stbi_set_flip_vertically_on_load(true);
  img.data = stbi_load((logl_root + file_path).c_str(),
                       &img.width,
                       &img.height,
                       &img.channels_count,
                       0);

  if (!img.data)
  {
    throw std::runtime_error("Could not load image: " +
                             (logl_root + file_path));
  }

  return img;
}

} // namespace fge::fs
