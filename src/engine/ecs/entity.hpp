#pragma once

#include "util/assert.hpp"
#include <limits>

namespace fge::ecs
{

/**
 * @brief Represents a entity.
 *
 * @tparam TId Type for the entity id.
 * @tparam TVersion Type for the entity version.
 */
template <typename TId, typename TVersion> class basic_entity
{
public:
  using id_type = TId;
  using version_type = TVersion;

  /**
   * @brief Create a entity with version and id set to zero.
   *
   * @param id Id of the entity.
   */
  basic_entity() : entity_version(0), entity_id(0) {}

  /**
   * @brief Create a entity with version set to zero.
   *
   * @param id Id of the entity.
   */
  basic_entity(TId id) : entity_version(0), entity_id(id) {}

  /**
   * @brief Create a entity.
   *
   * @param version Version of the entity.
   * @param id Id of the entity.
   */
  basic_entity(TVersion version, TId id)
      : entity_version(version),
        entity_id(id)
  {
  }

  basic_entity(const basic_entity<TId, TVersion> &other)
      : entity_version(other.entity_version),
        entity_id(other.entity_id)
  {
  }

  basic_entity(basic_entity<TId, TVersion> &&other)
      : entity_version(other.entity_version),
        entity_id(other.entity_id)
  {
  }

  void operator=(const basic_entity<TId, TVersion> &other)
  {
    entity_version = other.entity_version;
    entity_id      = other.entity_id;
  }

  /**
   * @brief Get the id of the entity.
   *
   * @return Id of the entity.
   */
  TId id() const { return entity_id; }

  /**
   * @brief Get the version of the entity.
   *
   * @return Version of the entity.
   */
  TVersion version() const { return entity_version; }

  /**
   * @brief Increment the version of the entity by one.
   */
  void increment_version()
  {
    FGE_ASSERT(entity_version + 1 < max_version());
    ++entity_version;
  }

  /**
   * @brief Get the maximum number that can represented by the TId type.
   *
   * The maximum number represents a invalid id.
   *
   * @return Maximum representable number by TId
   */
  static constexpr TId max_id() { return std::numeric_limits<TId>::max(); }

  /**
   * @brief Get the maximum number that can represented by the TVersion type.
   *
   * The maximum number represents a invalid version.
   *
   * @return Maximum representable number by TVersion.
   */
  static constexpr TId max_version()
  {
    return std::numeric_limits<TVersion>::max();
  }

private:
  TVersion entity_version = 0;
  TId      entity_id      = 0;
};

/**
 * @brief Null object for all entity identifiers.
 */
struct null_t
{
  /**
   * @brief Converts the null object to identifiers of any type.
   *
   * @tparam TEntity Type of entity identifier.
   * @return The null representation for the given identifier.
   */
  template <typename TEntity> [[nodiscard]] constexpr operator TEntity() const
  {
    return TEntity(TEntity::max_id());
  }

  /**
   * @brief Compares a null object and an entity identifier of any type.
   *
   * @tparam TEntity Type of entity identifier.
   * @param entity Entity identifier with which to compare.
   *
   * @return False if the two elements differ, true otherwise.
   */
  template <typename TEntity>
  [[nodiscard]] constexpr bool operator==(const TEntity &entity) const
  {
    return entity.id() == TEntity::max_id();
  }
};

/**
 * @brief Compares a null object and an entity identifier of any type.
 * @tparam Entity Type of entity identifier.
 * @param entity Entity identifier with which to compare.
 * @param other A null object yet to be converted.
 * @return False if the two elements differ, true otherwise.
 */
template <typename TEntity>
[[nodiscard]] constexpr bool operator==(const TEntity &entity,
                                        const null_t & other)
{
  return other.operator==(entity);
}

/**
 * @brief Null entity
 *
 * Use this type if you want to mark that a entity does not exist.
 */
inline constexpr null_t null{};

}; // namespace fge::ecs
