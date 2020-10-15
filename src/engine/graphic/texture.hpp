#pragma once

#include "std.hpp"

namespace Fge
{

enum class ImageFormat
{
  Red,
  Rgb,
  Rgba
};

enum class WrapMode
{
  Repeat
};

enum class FilterMode
{
  Linear,
  LinearMipmapLinear
};

enum class PixelDataType
{
  UnsignedByte
};

struct Texture2DConfig
{
  std::string   name{};
  uint32_t      width{};
  uint32_t      height{};
  void *        data{};
  ImageFormat   format = ImageFormat::Rgb;

  WrapMode      wrap_s          = WrapMode::Repeat;
  WrapMode      wrap_t          = WrapMode::Repeat;
  FilterMode    filter_min      = FilterMode::Linear;
  FilterMode    filter_max      = FilterMode::Linear;
  PixelDataType pixel_data_type = PixelDataType::UnsignedByte;

  bool     generate_mipmap = false;
  uint32_t samples         = 0;
};

class Texture2D
{
public:
  Texture2D() = default;

  virtual ~Texture2D() = default;

  virtual void bind(uint32_t slot) = 0;

  virtual void unbind() = 0;

  virtual uint32_t get_target() = 0;

  virtual uint32_t get_id() = 0;

private:
  Texture2D(const Texture2D &) = delete;

  void operator=(const Texture2D &) = delete;
};

} // namespace Fge
