/**
 * @file dirac_operator_debug_only_zero.cc
 * @brief Solve Dirac source problem for fixed function f and variable
 * k
 *
 * The derivations of the funcitons are given in the file testfunctionDirac.nb
 */

#include <dirac_operator_experiment.h>

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
    complex ret = Complex(0, 1) * k * (Cos(y) + Sin(x) + Sin(z));
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
            x * y * Sin(y)) /
               (Power(x, 2) + Power(y, 2) + Power(z, 2)),
        -((x * y * Cos(x) + y * z * Cos(z) +
           (Power(x, 2) + Power(z, 2)) * Sin(y)) /
          (Power(x, 2) + Power(y, 2) + Power(z, 2))),
        (-(x * z * Cos(x)) + (Power(x, 2) + Power(y, 2)) * Cos(z) +
         y * z * Sin(y)) /
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
    complex ret = 0;
    return ret;
  };

  // Compute the analytic solution of the problem
  auto u_zero = [&](const Eigen::Vector3d x_vec) -> complex {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec;
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    return Cos(y) + Sin(x) + Sin(z);
  };

  // Compute the analytic solution of the problem
  auto u_one = [&](const Eigen::Vector3d x_vec) -> Eigen::Vector3cd {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec;
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    Eigen::VectorXcd ret(3);

    ret << 0, 0, 0;
    return ret;
  };

  // Compute the analytic solution of the problem
  auto u_two = [&](const Eigen::Vector3d x_vec) -> complex {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec;
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    return 0;
  };

  projects::hldo_sphere::experiments::DiracOperatorExperiment experiment(
      u_zero, u_one, u_two, f_zero, f_one, f_two, k, "debug_only_zero");

  experiment.Compute(refinement_levels, ks);

  return 0;
}
