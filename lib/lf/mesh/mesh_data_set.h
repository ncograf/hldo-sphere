#ifndef __f6f00b1842024f36a0ad87792d103c89
#define __f6f00b1842024f36a0ad87792d103c89

#include "entity.h"
#include "mesh_interface.h"

namespace lf::mesh {

/**
 * @brief Interface that specifies how data is stored with an
 * entity.
 * @tparam T The type of data that is stored with the entity.
 *
 * A MeshDataSet is a class that allows us to store data with entities of a
 * mesh. Typical types of data that could be stored in a MeshDataSet:
 * - Flags that mark boundary entities.
 * - Material parameters for mesh elements (codim=0)
 *
 * @attention A MeshDataSet may store information only with a subset of
 * entities. See DefinedOn() .
 *
 * #### Implementations
 * There are a number of classes that implement the MeshDataSet interface
 * that differ mostly by the subset of entities to which they can attach data:
 * - CodimMeshDataSet attaches data to all entities of a given codimension and
 *   is undefined on other entities.
 *
 */
template <class T>
class MeshDataSet {
 protected:
  MeshDataSet() = default;
  MeshDataSet(const MeshDataSet&) = default;
  MeshDataSet(MeshDataSet&&) noexcept = default;
  MeshDataSet& operator=(const MeshDataSet&) = default;
  MeshDataSet& operator=(MeshDataSet&&) noexcept = default;

 public:
  /**
   * @brief Get a (modifiable) reference to the data stored with entity e.
   * @param e The entity whose data should be retrieved/modified
   * @return  A reference to the stored data.
   *
   * @note The behavior of this method is undefined if `DefinedOn(e) == false`!
   */
  virtual T& data(const Entity& e) = 0;

  /**
   * @brief Get a const reference to the data stored with entity e.
   * @param e The entity whose data should be retrieved.
   * @return A constant reference to the data stored for this entity.
   *
   * @note The behavior of this method is undefined if `DefinedOn(e) == false`!
   */
  virtual const T& data(const Entity& e) const = 0;

  /**
   * @brief Does the dataset store information with this entity?
   * @param e The entity that should be tested.
   * @return true if this dataset associates information with this entity.
   *              Otherwise false.
   */
  virtual bool DefinedOn(const Entity& e) const = 0;

  /// Virtual destructor
  virtual ~MeshDataSet() = default;
};

}  // namespace lf::mesh

#endif  // __f6f00b1842024f36a0ad87792d103c89