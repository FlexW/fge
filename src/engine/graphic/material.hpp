#pragma once

#include "shader.hpp"
#include "std.hpp"
#include "texture.hpp"

namespace Fge
{

class UniformBase
{
public:
  virtual ~UniformBase() = default;

  virtual uint32_t set(Shader &shader, uint32_t texture_bind_point = 0) = 0;

  virtual std::shared_ptr<UniformBase> clone() = 0;
};

template <typename T> class Uniform : public UniformBase
{
public:
  Uniform(const std::string &name, const T &value) : name(name), value(value) {}

  void update(const T &value) { this->value = value; }

  virtual uint32_t set(Shader &shader, uint32_t texture_bind_point = 0) override
  {
    shader.set_uniform(name, value);
    return texture_bind_point;
  }

  const std::string &get_name() const { return name; }

  std::shared_ptr<UniformBase> clone() override
  {
    auto new_uniform = std::make_shared<Uniform<T>>(name, value);

    return new_uniform;
  }

private:
  std::string name;
  T           value;
};

template <> class Uniform<std::shared_ptr<Texture2D>> : public UniformBase
{
public:
  Uniform(const std::string &name, const std::shared_ptr<Texture2D> &value)
      : name(name),
        value(value)
  {
  }

  void update(const std::shared_ptr<Texture2D> &value) { this->value = value; }

  virtual uint32_t set(Shader &shader, uint32_t texture_bind_point = 0) override
  {
    value->bind(texture_bind_point);
    shader.set_uniform(name, static_cast<int32_t>(texture_bind_point));
    ++texture_bind_point;
    return texture_bind_point;
  }

  const std::string &get_name() const { return name; }

  std::shared_ptr<UniformBase> clone() override
  {
    auto new_uniform =
        std::make_shared<Uniform<std::shared_ptr<Texture2D>>>(name, value);

    return new_uniform;
  }

private:
  std::string                name;
  std::shared_ptr<Texture2D> value;
};

class Material
{
public:
  Material(const std::string &name);

  virtual ~Material() = default;

  std::string get_name() const { return name; }

  virtual void unbind() const;

  virtual void bind(uint32_t texture_bind_point = 0) = 0;

  template <typename T>
  void set_uniform(const std::string &name, const T &value)
  {
    auto uniform = find_instance_uniform<T>(name);
    if (uniform)
    {
      uniform->update(value);
      return;
    }

    uniform = std::make_shared<Uniform<T>>(name, value);
    uniforms.emplace_back(uniform);
  }

  virtual void reload();

  virtual std::shared_ptr<Material> clone() = 0;

protected:
  std::string name;

  virtual uint32_t bind_uniforms(std::shared_ptr<Shader>,
                                 uint32_t texture_bind_point = 0) const;

protected:
  std::vector<std::shared_ptr<UniformBase>> uniforms;

  template <typename T>
  std::shared_ptr<Uniform<T>> find_instance_uniform(const std::string &name)
  {
    for (auto uniform : uniforms)
    {
      auto casted_uniform = std::static_pointer_cast<Uniform<T>>(uniform);
      if (name == casted_uniform->get_name())
      {
        return casted_uniform;
      }
    }

    return nullptr;
  }
};

} // namespace Fge
