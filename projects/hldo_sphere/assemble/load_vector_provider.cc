#include "load_vector_provider.h"

#include <lf/base/base.h>
#include <lf/quad/quad.h>
#include <lf/quad/quad_rule.h>
#include <lf/uscalfe/lagr_fe.h>

#include <iostream>

namespace projects::hldo_sphere::assemble {

LoadVectorProvider::LoadVectorProvider(
    std::function<double(const Eigen::Vector3d &)> f)
    : f_(f) {}

Eigen::VectorXd LoadVectorProvider::Eval(const lf::mesh::Entity &entity) const {
  const auto *const geom = entity.Geometry();

  // Only triangles are supported
  LF_VERIFY_MSG(entity.RefEl() == lf::base::RefEl::kTria(),
                "Unsupported cell type " << entity.RefEl());

  // Compute the global vertex coordinates
  Eigen::MatrixXd vertices = geom->Global(entity.RefEl().NodeCoords());

  // define quad rule with sufficiantly high degree since the
  // baricentric coordinate functions have degree 1
  lf::quad::QuadRule quadrule{lf::quad::make_TriaQR_EdgeMidpointRule()};

  const lf::uscalfe::FeLagrangeO1Tria<double> hat_func;
  const auto lambda_hat = [&](Eigen::Vector2d x_hat) -> Eigen::MatrixXd {
    return hat_func.EvalReferenceShapeFunctions(x_hat);
  };

  // returns evaluation of @f$ \lambda @f$ for at a given point on the reference
  // triangle f is evaluated at the global coordinates
  const auto f_tilde_hat = [&](Eigen::Vector2d x_hat) -> Eigen::MatrixXd {
    Eigen::Vector3d x = geom->Global(x_hat);
    return lambda_hat(x_hat) * f_(x);
  };

  // Compute the elements of the vector with given quadrature rule
  Eigen::Vector3d element_vector = Eigen::Vector3d::Zero();
  const Eigen::MatrixXd points = quadrule.Points();
  const Eigen::VectorXd weights =
      (geom->IntegrationElement(points).array() * quadrule.Weights().array())
          .matrix();
  for (lf::base::size_type n = 0; n < quadrule.NumPoints(); ++n) {
    const Eigen::Vector3d f_eval = f_tilde_hat(points.col(n));
    element_vector += weights[n] * f_eval;
  }
  return element_vector;
}

}  // namespace projects::hldo_sphere::assemble
