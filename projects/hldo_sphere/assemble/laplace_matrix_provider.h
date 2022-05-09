#ifndef THESIS_ASSEMBLE_LAPLACE_MATRIX_PROVIDER_H
#define THESIS_ASSEMBLE_LAPLACE_MATRIX_PROVIDER_H

/**
 * @file laplace_matrix_provider.h
 *
 */

#include <lf/mesh/entity.h>

#include <Eigen/Dense>

namespace projects::hldo_sphere {

namespace assemble {

/**
 * @brief An element matrix provider for piecewise linear (barycentric) basis
 * functions
 *
 * The locally evaluated bilinear form is
 * @f[
 * \int_{K} grad_{\Gamma}(u) grad_{\Gamma}(v) dx
 * @f]
 *
 *
 * @note Only triangular meshes are supported
 *
 */
class LaplaceMatrixProvider {
 public:
  /**
   * @brief Constructor
   */
  LaplaceMatrixProvider(){};

  /**
   * @brief Compute the element matrix for a given triangle of a mesh
   * @param entity The mesh triangle on which the element matrix will be
   * computed
   * @returns The 3 by 3 element matrix of the triangle
   *
   * @note Only triangular cells are supported
   */
  Eigen::MatrixXd Eval(const lf::mesh::Entity &entity) const;

  /**
   * @brief All entities are regarded as active
   */
  bool isActive(const lf::mesh::Entity &entity) const { return true; }

 private:
};

}  // end namespace assemble

}  // namespace projects::hldo_sphere

#endif  // THESIS_ASSEMBLE_LAPLACE_MATRIX_PROVIDER_H