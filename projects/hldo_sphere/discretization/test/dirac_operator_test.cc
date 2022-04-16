#include <dirac_operator.h>
#include <gtest/gtest.h>

#include <array>
#include <cmath>

TEST(projects_hldo_sphere_discretization, dirac_operator_test) {
  // Build LSE

  projects::hldo_sphere::discretization::DiracOperator<double> lse_builder;
  lse_builder.Compute();

  Eigen::SparseMatrix<double> Ae = lse_builder.GetGalerkinMatrix().makeSparse();

  Eigen::MatrixXd Ae_anal(26, 26);
  // clang-format off
  Ae_anal <<     
       0, 0, 0, 0, 0, 0,   -1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0,    1, 0, 0, 0,-1, 1,-1, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0,    0,-1, 0, 0, 1, 0, 0,-1, 1, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0,    0, 0,-1, 0, 0, 0, 0, 1, 0,-1, 1, 0,   0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0,    0, 0, 0,-1, 0,-1, 0, 0, 0, 1, 0, 1,   0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 1, 0,-1, 0,-1,-1,   0, 0, 0, 0, 0, 0, 0, 0,

       1,-1, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  -1, 0, 0, 0, 0, 0, 1, 0,
      -1, 0, 1, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  -1, 0, 1, 0, 0, 0, 0, 0,
      -1, 0, 0, 1, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0,-1, 0, 1, 0, 0, 0,
      -1, 0, 0, 0, 1, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0,-1, 0, 1, 0,
       0, 1,-1, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  -1, 1, 0, 0, 0, 0, 0, 0,
       0,-1, 0, 0, 1, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0,-1, 1,
       0, 1, 0, 0, 0,-1,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,-1, 0, 0, 0, 0, 0, 1,
       0, 0, 1,-1, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0,-1, 1, 0, 0, 0, 0,
       0, 0,-1, 0, 0, 1,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,-1, 0, 1, 0, 0, 0, 0,
       0, 0, 0, 1,-1, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0,-1, 1, 0, 0,
       0, 0, 0,-1, 0, 1,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0,-1, 0, 1, 0, 0,
       0, 0, 0, 0,-1, 1,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0,-1, 0, 1,

       0, 0, 0, 0, 0, 0,   -1,-1, 0, 0,-1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 1, 0,-1, 0,-1, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0,    0, 1,-1, 0, 0, 0, 0,-1, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 1, 1, 0,-1, 0,   0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0,    0, 0, 1,-1, 0, 0, 0, 0, 0,-1, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,-1,   0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0,    1, 0, 0, 1, 0,-1, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,   0, 0, 0, 0, 0, 0, 0, 0;
  // clang-format on

  Ae_anal.block(0, 6, 6, 12) *= 1. / std::sqrt(3);
  Ae_anal.block(6, 0, 12, 6) *= -1. / std::sqrt(3);
  Ae_anal.block(6, 18, 12, 8) *= -1;
  Ae_anal.block(18, 6, 8, 12) *= -1;

  // Assert that the two matrices are approximately equal
  ASSERT_EQ(Ae.rows(), Ae_anal.rows());
  ASSERT_EQ(Ae.cols(), Ae_anal.cols());
  for (int i = 0; i < Ae_anal.rows(); ++i) {
    for (int j = 0; j < Ae_anal.cols(); ++j) {
      EXPECT_NEAR(Ae.coeff(i, j), Ae_anal(i, j), 1e-12)
          << "mismatch in entry (" << i << ", " << j << ")";
    }
  }

  Eigen::VectorXd Vec = lse_builder.GetLoadVector();
  Eigen::VectorXd Vec_anal(26);
  Vec_anal.setZero();
  for (int j = 0; j < Ae_anal.cols(); ++j) {
    EXPECT_NEAR(Vec(j), Vec_anal(j), 1e-12)
        << "mismatch in entry (" << j << ")";
  }
}
