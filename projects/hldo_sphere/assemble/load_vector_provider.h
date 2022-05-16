#ifndef THESIS_ASSEMBLE_LOAD_VECTOR_PROVIDER_H
#define THESIS_ASSEMBLE_LOAD_VECTOR_PROVIDER_H

/**
 * @file load_vector_provider.h
 */

#include <lf/mesh/entity.h>
#include <lf/mesh/utils/mesh_data_set.h>

#include <Eigen/Dense>
#include <functional>
#include <utility>

namespace projects::hldo_sphere {

namespace assemble {

/**
 * @brief An element vector provider for piecewise linear (barycentric) basis
 * functions in a 3 dimensional world with 2 dimensional triangular cells
 *
 * The locally evaluated linear form is
 * @f[
 *  \int\limits_{K}\ f(\mathbf{x}) \cdot v(\mathbf{x}) \, dx, \quad f, v \in
 * H^1(\Omega)
 * @f]
 *
 * Evaluates the function values on the cell.
 *
 *
 * @note This class complies with the type requirements for the template
 * argument ENTITY_VECTOR_PROVIDER of the function
 * lf::assemble::AssembleVectorLocally().
 *
 * @note Only triangular meshes are supported
 *
 */
class LoadVectorProvider {
 public:
  /**
   * @brief Constructor
   * @param f a realvalued function defined on the surface of the sphere
   */
  LoadVectorProvider(std::function<double(const Eigen::Vector3d &)> f);

  /**
   * @brief Compute the element vector for some cell on the mesh
   * @param entity the mesh cell to compute the element vector for
   * @returns The element vector of the given cell
   *
   * @note Only triangular cells are supported
   */
  Eigen::VectorXd Eval(const lf::mesh::Entity &entity) const;

  /**
   * @brief All entities are regarded as active
   */
  bool isActive(const lf::mesh::Entity &entity) const { return true; }

 private:
  const std::function<double(const Eigen::Vector3d &)> f_;
};

}  // end namespace assemble

}  // namespace projects::hldo_sphere

#endif  // THESIS_ASSEMBLE_LOAD_VECTOR_PROVIDER_H
