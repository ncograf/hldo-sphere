/**
 * @file dirac_operator_test.cc
 * @brief Solve hodge laplacian source problem for fixed function f and variable
 * k
 */

#include <dirac_operator_example.h>

using lf::uscalfe::operator-;
using complex = std::complex<double>;

/**
 * @brief Prints the L2 norm errors and the Supremum of the experiment and
 * creates vtk plots for a function u
 */
int main(int argc, char *argv[]) {
  if (argc != 4 && argc != 5) {
    std::cerr << "Usage: " << argv[0]
              << " max_refinement_level min_k max_k step=0.1 " << std::endl;
    exit(1);
  }

  double min_k;
  double max_k;
  double step = 0.1;
  const unsigned refinement_level = atoi(argv[1]);
  sscanf(argv[2], "%lf", &min_k);
  sscanf(argv[3], "%lf", &max_k);
  if (argc == 5) sscanf(argv[4], "%lf", &step);
  std::cout << "max_refinement_level : " << refinement_level << std::endl;
  std::cout << "min_k : " << min_k << ", max_k = " << max_k
            << " step = " << step << std::endl;

  // needs to be a reference such that it can be reassigned
  double &k = min_k;

  std::vector<unsigned> refinement_levels(refinement_level + 1);
  for (int i = 0; i < refinement_level + 1; i++) {
    refinement_levels[i] = i;
  }

  std::vector<double> ks;
  for (double i = min_k; i <= max_k; i += step) {
    ks.push_back(i);
  }

  // mathematica function output requries the following helpers
  auto Power = [](complex a, complex b) -> complex { return std::pow(a, b); };
  auto Complex = [](double a, double b) -> complex { return complex(a, b); };
  auto Sin = [](complex a) -> complex { return std::sin(a); };
  auto Cos = [](complex a) -> complex { return std::cos(a); };
  auto Sqrt = [](complex a) -> complex { return std::sqrt(a); };

  // righthandside for the zero and two form
  auto f_zero = [&](const Eigen::Vector3d &x_vec) -> complex {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec;
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    // autogenerated by mathematica
    complex ret = Complex(0, 1) * k * (Cos(y) + Sin(x) + Sin(z)) +
                  (x * z * Cos(x) + x * y * Cos(y) + y * z * Cos(z) +
                   2 * z * Sin(x) + 2 * x * Sin(y) + 2 * y * Sin(z)) /
                      (Power(x, 2) + Power(y, 2) + Power(z, 2));
    return ret;
  };

  // righthandside for the one form
  auto f_one = [&](const Eigen::Vector3d &x_vec) -> Eigen::VectorXcd {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec;
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    // autogenerated by mathematica
    Eigen::VectorXcd ret(3);
    ret << ((Power(y, 2) + Power(z, 2)) * Cos(x) - x * z * Cos(z) +
            x * y * Sin(y) +
            Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) *
                (y * Cos(z) + z * Sin(y)) -
            Complex(0, 1) * k *
                (x * z * Sin(x) - (Power(y, 2) + Power(z, 2)) * Sin(y) +
                 x * y * Sin(z))) /
               (Power(x, 2) + Power(y, 2) + Power(z, 2)),
        -((x * y * Cos(x) + y * z * Cos(z) +
           Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) *
               (-(z * Cos(x)) + x * Cos(z)) +
           (Power(x, 2) + Power(z, 2)) * Sin(y) +
           Complex(0, 1) * k *
               (y * z * Sin(x) + x * y * Sin(y) -
                (Power(x, 2) + Power(z, 2)) * Sin(z))) /
          (Power(x, 2) + Power(y, 2) + Power(z, 2))),
        (-(x * z * Cos(x)) + (Power(x, 2) + Power(y, 2)) * Cos(z) +
         y * z * Sin(y) -
         Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) *
             (y * Cos(x) + x * Sin(y)) +
         Complex(0, 1) * k *
             ((Power(x, 2) + Power(y, 2)) * Sin(x) -
              z * (x * Sin(y) + y * Sin(z)))) /
            (Power(x, 2) + Power(y, 2) + Power(z, 2));
    return ret;
  };

  // righthandside for the two form
  auto f_two = [&](const Eigen::Vector3d &x_vec) -> complex {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec;
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    // autogenerated by mathematica
    complex ret = (y * Cos(x) + z * Cos(y) + x * Cos(z)) /
                      Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) +
                  Complex(0, 1) * k * (Cos(y) + Sin(x) + Sin(z));
    return ret;
  };

  // Compute the analytic solution of the problem
  auto u_zero = [&](const Eigen::Vector3d x_vec) -> complex {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec;
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    return Cos(z) + Sin(x) + Sin(y);
  };

  // Compute the analytic solution of the problem
  auto u_one = [&](const Eigen::Vector3d x_vec) -> Eigen::Vector3cd {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec;
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    Eigen::VectorXcd ret(3);

    // mathematica autocompute
    ret << (-(x * z * Sin(x)) + (Power(y, 2) + Power(z, 2)) * Sin(y) -
            x * y * Sin(z)) /
               (Power(x, 2) + Power(y, 2) + Power(z, 2)),
        (-(y * z * Sin(x)) - x * y * Sin(y) +
         (Power(x, 2) + Power(z, 2)) * Sin(z)) /
            (Power(x, 2) + Power(y, 2) + Power(z, 2)),
        ((Power(x, 2) + Power(y, 2)) * Sin(x) - z * (x * Sin(y) + y * Sin(z))) /
            (Power(x, 2) + Power(y, 2) + Power(z, 2));
    return ret;
  };

  // Compute the analytic solution of the problem
  auto u_two = [&](const Eigen::Vector3d x_vec) -> complex {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec;
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    return Cos(z) + Sin(x) + Sin(y);
  };

  projects::hldo_sphere::examples::DiracOperatorExample example(
      u_zero, u_one, u_two, f_zero, f_one, f_two, k, "test");

  example.Compute(refinement_levels, ks);

  return 0;
}
