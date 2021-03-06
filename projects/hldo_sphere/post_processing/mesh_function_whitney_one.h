#ifndef HLDO_SPHERE_MESH_FUNCTION_WHITNEY_ONE_H
#define HLDO_SPHERE_MESH_FUNCTION_WHITNEY_ONE_H
/*
 * @file mesh_function_whitney_one.h
 */

#include <lf/uscalfe/uscalfe.h>

namespace projects::hldo_sphere::post_processing {

/**
 * @brief Provides Mesh Function for given basis expansion coefficients
 *
 * @tparam SCALAR type of the return vector
 *
 */
template <typename SCALAR>
class MeshFunctionWhitneyOne {
 public:
  /**
   * @brief basic constructor
   *
   * Mesh Function on the global mesh built using the basis expansion
   * coefficiants passed in the argument and the whitney 1-form basis functions.
   *
   * The whitney 1-forms, surface edge elements are associated with
   * edges and defined as
   *
   * @f[
   *  b_i = s_i (\lambda_i \mathbf{grad}_{\Gamma}(\lambda_{i+1}) - \lambda_{i+1}
   * \mathbf{grad}_{\Gamma}(\lambda_{i}))
   * @f]
   *
   * @param mu vector containing the basis function expansion coefficiants in
   * global ordering
   *
   * @param mesh containing the mesh on which to evaluate
   *
   * @note Only triangular meshes are supported
   */
  MeshFunctionWhitneyOne(const Eigen::Matrix<SCALAR, Eigen::Dynamic, 1>& mu,
                         const std::shared_ptr<const lf::mesh::Mesh> mesh)
      : mu_(mu), mesh_(mesh) {
    // make sure mu has the right size
    lf::base::size_type n = mesh->NumEntities(1);
    int mu_size = mu.size();
    LF_VERIFY_MSG(
        n == mu_size,
        "Not the right number of basis expansion coefficiants in mu expected: "
            << n << " given: " << mu_size);
  };

  /**
   *
   * @brief Evaluates the whitney one form basis function at reference points on
   * a given cell
   *
   * @param e on which to evaluate
   * @param local coordinates on the reference triangle for the
   * evaluation. Each column contains one evaluation point
   *
   *
   * @returns a vector of Vectors containing the results for the
   * evaluation of the whithey one form basis functions at the input coordintes
   * in `local`.
   *
   * Evaluates the functions
   *
   * @f[
   *    b_0 := \pm \lambda_0 grad(\lambda_1) - \lambda_1 grad(\lambda_0) \\
   *    b_1 := \pm \lambda_1 grad(\lambda_2) - \lambda_2 grad(\lambda_1) \\
   *    b_2 := \pm \lambda_2 grad(\lambda_0) - \lambda_0 grad(\lambda_2)
   * ]@f
   *
   * where the $\lambda$ are the barycentric basis functions
   *
   * And returns a vector containing the vector of the sum of the scaled basis
   * functions
   *
   * @f[
   *      \begin{pmatrix}
   *          \mu_{l_0} \bm{b}_{0}(\bm{x}_i) + \mu_{l_1}\bm{b}_1(\bm{x}_i) +
   *              \mu_{l_2}\bm{b}_2(\bm{x}_i)
   *     \end{pmatrix}
   * ]@f
   *
   *
   * @note only triangles are supported
   *
   */
  std::vector<Eigen::Matrix<SCALAR, Eigen::Dynamic, 1>> operator()(
      const lf::mesh::Entity& e, const Eigen::MatrixXd& local) const {
    // Only triangles are supported
    LF_VERIFY_MSG(e.RefEl() == lf::base::RefEl::kTria(),
                  "Unsupported cell type " << e.RefEl());

    // Get the geometry of the entity
    const auto* geom = e.Geometry();

    // Compute the global vertex coordinates
    Eigen::MatrixXd vertices = geom->Global(e.RefEl().NodeCoords());

    // get the lambdas
    const lf::uscalfe::FeLagrangeO1Tria<SCALAR> hat_func;

    // The gradients are constant on the triangle
    const Eigen::Matrix<SCALAR, Eigen::Dynamic, Eigen::Dynamic> ref_grads =
        hat_func.GradientsReferenceShapeFunctions(Eigen::VectorXd::Zero(2))
            .transpose();

    // The JacobianInverseGramian is constant on the triangle
    const Eigen::Matrix<SCALAR, Eigen::Dynamic, Eigen::Dynamic> J_inv_trans =
        geom->JacobianInverseGramian(Eigen::VectorXd::Zero(2));

    // get the gradients
    const Eigen::Matrix<SCALAR, Eigen::Dynamic, Eigen::Dynamic> grad =
        J_inv_trans * ref_grads;

    // get lambda values
    const Eigen::Matrix<SCALAR, Eigen::Dynamic, Eigen::Dynamic> lambda =
        hat_func.EvalReferenceShapeFunctions(local);

    // correct for orientation
    auto edgeOrientations = e.RelativeOrientations();

    // define return vector
    std::vector<Eigen::Matrix<SCALAR, Eigen::Dynamic, 1>> vals(local.cols());

    // get global indices of the edges
    std::vector<lf::base::size_type> global_indices(3);
    auto edges = e.SubEntities(1);
    for (lf::base::size_type i = 0; i < 3; i++) {
      global_indices[i] = mesh_->Index(*edges[i]);
    }

    // for each point evaluate the basis functions and add the
    // values together
    for (int i = 0; i < local.cols(); i++) {
      Eigen::Matrix<SCALAR, Eigen::Dynamic, Eigen::Dynamic> bs(grad.rows(), 3);
      bs.col(0) = lambda(0, i) * grad.col(1) - lambda(1, i) * grad.col(0);
      bs.col(1) = lambda(1, i) * grad.col(2) - lambda(2, i) * grad.col(1);
      bs.col(2) = lambda(2, i) * grad.col(0) - lambda(0, i) * grad.col(2);

      bs.col(0) *= lf::mesh::to_sign(edgeOrientations[0]);
      bs.col(1) *= lf::mesh::to_sign(edgeOrientations[1]);
      bs.col(2) *= lf::mesh::to_sign(edgeOrientations[2]);

      vals[i] = mu_(global_indices[0]) * bs.col(0) +
                mu_(global_indices[1]) * bs.col(1) +
                mu_(global_indices[2]) * bs.col(2);
    }

    return vals;
  }

 private:
  const Eigen::Matrix<SCALAR, Eigen::Dynamic, 1>& mu_;
  const std::shared_ptr<const lf::mesh::Mesh> mesh_;
};

}  // namespace projects::hldo_sphere::post_processing
#endif  // HLDO_SPHERE_MESH_FUNCTION_WHITNEY_ONE_H
