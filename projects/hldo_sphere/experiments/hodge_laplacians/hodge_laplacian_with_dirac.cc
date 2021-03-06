/**
 * @file hodge_laplacian_test.cc
 * @brief Solve hodge laplacian source problem for fixed function f and variable
 * k
 */

#include <hodge_laplacian_experiment.h>

using complex = std::complex<double>;
/**
 * @brief Prints the L2 norm errors and the Supremum of the experiment and
 * creates vtk plots for a function
 *
 * @f$ u((x,y,z)) = \sin(x) + \cos(y) +
 * \sin(z)@f$
 *
 * for the zero and two form and
 *
 * @f$ u((x,y,z)) = (I - nn^T)(\sin(y), \sin(z), \sin(x))
 *
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
  auto Power = [](double a, double b) -> double { return std::pow(a, b); };
  auto Sin = [](double a) -> double { return std::sin(a); };
  auto Cos = [](double a) -> double { return std::cos(a); };
  auto Sqrt = [](double a) -> double { return std::sqrt(a); };
  auto Complex = [](double a, double b) -> complex { return complex(a, b); };

  double r = 1.;

  // righthandside for the zero and two form
  auto f_zero_dirac = [&](const Eigen::Vector3d &x_vec) -> complex {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec / x_vec.norm();
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    // autogenerated by mathematica
    complex ret = (2 * x * Cos(x) +
                   ((1 + Power(k, 2)) * Power(x, 2) + Power(z, 2) +
                    Power(k, 2) * (Power(y, 2) + Power(z, 2))) *
                       Cos(y) +
                   2 * z * Cos(z) + Power(k, 2) * Power(x, 2) * Sin(x) +
                   Power(y, 2) * Sin(x) + Power(k, 2) * Power(y, 2) * Sin(x) +
                   Power(z, 2) * Sin(x) + Power(k, 2) * Power(z, 2) * Sin(x) -
                   2 * y * Sin(y) + Power(x, 2) * Sin(z) +
                   Power(k, 2) * Power(x, 2) * Sin(z) + Power(y, 2) * Sin(z) +
                   Power(k, 2) * Power(y, 2) * Sin(z) +
                   Power(k, 2) * Power(z, 2) * Sin(z)) /
                  (Power(x, 2) + Power(y, 2) + Power(z, 2));
    return ret;
  };

  // righthandside for the one form
  auto f_one_dirac = [&](const Eigen::Vector3d &x_vec) -> Eigen::VectorXcd {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec / x_vec.norm();
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    // autogenerated by mathematica
    Eigen::VectorXcd ret(3);
    ret << ((-(Power(x, 2) * z) + 3 * z * (Power(y, 2) + Power(z, 2)) +
             Complex(0, 1) * k * (Power(y, 2) + Power(z, 2)) *
                 (Power(x, 2) + Power(y, 2) + Power(z, 2))) *
                Cos(x) +
            y * (-3 * Power(x, 2) + Power(y, 2) + Power(z, 2)) * Cos(y) -
            Complex(0, 1) * k * Power(x, 3) * z * Cos(z) -
            4 * x * y * z * Cos(z) -
            Complex(0, 1) * k * x * Power(y, 2) * z * Cos(z) -
            Complex(0, 1) * k * x * Power(z, 3) * Cos(z) - 2 * x * z * Sin(x) -
            x * Power(y, 2) * z * Sin(x) - x * Power(z, 3) * Sin(x) +
            Complex(0, 1) * k * Power(x, 3) * y * Sin(y) +
            2 * Power(y, 2) * Sin(y) + Power(x, 2) * Power(y, 2) * Sin(y) +
            Complex(0, 1) * k * x * Power(y, 3) * Sin(y) +
            2 * Power(z, 2) * Sin(y) +
            Complex(0, 1) * k * x * y * Power(z, 2) * Sin(y) -
            2 * x * y * Sin(z) + x * y * Power(z, 2) * Sin(z)) /
                   Power(Power(x, 2) + Power(y, 2) + Power(z, 2), 2) +
               (-(z * Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) * Cos(x)) +
                y * Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) * Cos(y) +
                Complex(0, 1) * k * Power(x, 2) * y * Cos(z) +
                Complex(0, 1) * k * Power(y, 3) * Cos(z) +
                Complex(0, 1) * k * y * Power(z, 2) * Cos(z) +
                Complex(0, 1) * k * Power(x, 2) * z * Sin(y) +
                Complex(0, 1) * k * Power(y, 2) * z * Sin(y) +
                Complex(0, 1) * k * Power(z, 3) * Sin(y) +
                Power(z, 2) * Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) *
                    Sin(y) -
                x * y * Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) *
                    Sin(z)) /
                   Power(Power(x, 2) + Power(y, 2) + Power(z, 2), 1.5) -
               Complex(0, 1) * k *
                   (((Power(y, 2) + Power(z, 2)) * Cos(x) - x * z * Cos(z) +
                     x * y * Sin(y)) /
                        (Power(x, 2) + Power(y, 2) + Power(z, 2)) +
                    (y * Cos(z) + z * Sin(y)) /
                        Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) +
                    (Complex(0, 1) * k *
                     (-(x * z * Sin(x)) + (Power(y, 2) + Power(z, 2)) * Sin(y) -
                      x * y * Sin(z))) /
                        (Power(x, 2) + Power(y, 2) + Power(z, 2))),
        (Complex(0, -1) *
         (x * y *
              (Complex(0, -4) * z +
               k * (Power(x, 2) + Power(y, 2) + Power(z, 2))) *
              Cos(x) +
          Complex(0, 1) * x *
              (3 * Power(x, 2) - Power(y, 2) + 3 * Power(z, 2)) * Cos(y) +
          Complex(0, 1) * Power(x, 2) * z * Cos(z) +
          k * Power(x, 2) * y * z * Cos(z) -
          Complex(0, 3) * Power(y, 2) * z * Cos(z) +
          k * Power(y, 3) * z * Cos(z) + Complex(0, 1) * Power(z, 3) * Cos(z) +
          k * y * Power(z, 3) * Cos(z) - Complex(0, 2) * y * z * Sin(x) +
          Complex(0, 1) * Power(x, 2) * y * z * Sin(x) +
          k * Power(x, 4) * Sin(y) - Complex(0, 2) * x * y * Sin(y) -
          Complex(0, 1) * Power(x, 3) * y * Sin(y) +
          k * Power(x, 2) * Power(y, 2) * Sin(y) +
          2 * k * Power(x, 2) * Power(z, 2) * Sin(y) -
          Complex(0, 1) * x * y * Power(z, 2) * Sin(y) +
          k * Power(y, 2) * Power(z, 2) * Sin(y) + k * Power(z, 4) * Sin(y) +
          Complex(0, 2) * Power(x, 2) * Sin(z) +
          Complex(0, 2) * Power(z, 2) * Sin(z) +
          Complex(0, 1) * Power(y, 2) * Power(z, 2) * Sin(z))) /
                Power(Power(x, 2) + Power(y, 2) + Power(z, 2), 2) +
            (Complex(0, 1) * k * z * (Power(x, 2) + Power(y, 2) + Power(z, 2)) *
                 Cos(x) -
             x * Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) * Cos(y) -
             Complex(0, 1) * k * Power(x, 3) * Cos(z) -
             Complex(0, 1) * k * x * Power(y, 2) * Cos(z) -
             Complex(0, 1) * k * x * Power(z, 2) * Cos(z) +
             z * Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) * Cos(z) -
             y * z * Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) * Sin(x) +
             Power(x, 2) * Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) *
                 Sin(z)) /
                Power(Power(x, 2) + Power(y, 2) + Power(z, 2), 1.5) -
            Complex(0, 1) * k *
                ((z * Cos(x) - x * Cos(z)) /
                     Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) -
                 (x * y * Cos(x) + y * z * Cos(z) +
                  (Power(x, 2) + Power(z, 2)) * Sin(y)) /
                     (Power(x, 2) + Power(y, 2) + Power(z, 2)) +
                 (Complex(0, 1) * k *
                  (-(y * z * Sin(x)) - x * y * Sin(y) +
                   (Power(x, 2) + Power(z, 2)) * Sin(z))) /
                     (Power(x, 2) + Power(y, 2) + Power(z, 2))),
        ((x * Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) -
          Complex(0, 1) * k * y * (Power(x, 2) + Power(y, 2) + Power(z, 2))) *
             Cos(x) -
         y * Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) * Cos(z) +
         Power(y, 2) * Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) * Sin(x) -
         Complex(0, 1) * k * Power(x, 3) * Sin(y) -
         Complex(0, 1) * k * x * Power(y, 2) * Sin(y) -
         Complex(0, 1) * k * x * Power(z, 2) * Sin(y) -
         x * z * Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2)) * Sin(y)) /
                Power(Power(x, 2) + Power(y, 2) + Power(z, 2), 1.5) +
            (x *
                 (Power(z, 2) * (-3. - Complex(0, 1) * k * z) +
                  Power(x, 2) * (1. - Complex(0, 1) * k * z) +
                  Power(y, 2) * (1. - Complex(0, 1) * k * z)) *
                 Cos(x) -
             4 * x * y * z * Cos(y) + Complex(0, 1) * k * Power(x, 4) * Cos(z) +
             3 * Power(x, 2) * y * Cos(z) +
             Complex(0, 2) * k * Power(x, 2) * Power(y, 2) * Cos(z) +
             3 * Power(y, 3) * Cos(z) +
             Complex(0, 1) * k * Power(y, 4) * Cos(z) +
             Complex(0, 1) * k * Power(x, 2) * Power(z, 2) * Cos(z) -
             y * Power(z, 2) * Cos(z) +
             Complex(0, 1) * k * Power(y, 2) * Power(z, 2) * Cos(z) +
             2 * Power(x, 2) * Sin(x) + 2 * Power(y, 2) * Sin(x) +
             Power(x, 2) * Power(z, 2) * Sin(x) - 2 * x * z * Sin(y) +
             Complex(0, 1) * k * Power(x, 2) * y * z * Sin(y) +
             x * Power(y, 2) * z * Sin(y) +
             Complex(0, 1) * k * Power(y, 3) * z * Sin(y) +
             Complex(0, 1) * k * y * Power(z, 3) * Sin(y) - 2 * y * z * Sin(z) -
             Power(x, 2) * y * z * Sin(z) - Power(y, 3) * z * Sin(z)) /
                Power(Power(x, 2) + Power(y, 2) + Power(z, 2), 2) -
            Complex(0, 1) * k *
                (-((y * Cos(x) + x * Sin(y)) /
                   Sqrt(Power(x, 2) + Power(y, 2) + Power(z, 2))) +
                 (-(x * z * Cos(x)) + (Power(x, 2) + Power(y, 2)) * Cos(z) +
                  y * z * Sin(y)) /
                     (Power(x, 2) + Power(y, 2) + Power(z, 2)) +
                 (Complex(0, 1) * k *
                  ((Power(x, 2) + Power(y, 2)) * Sin(x) -
                   z * (x * Sin(y) + y * Sin(z)))) /
                     (Power(x, 2) + Power(y, 2) + Power(z, 2)));
    return ret;
  };

  // righthandside for the two form
  auto f_two_dirac = [&](const Eigen::Vector3d &x_vec) -> complex {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec / x_vec.norm();
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    // autogenerated by mathematica
    complex ret = (2 * x * Cos(x) +
                   ((1 + Power(k, 2)) * Power(x, 2) + Power(z, 2) +
                    Power(k, 2) * (Power(y, 2) + Power(z, 2))) *
                       Cos(y) +
                   2 * z * Cos(z) + Power(k, 2) * Power(x, 2) * Sin(x) +
                   Power(y, 2) * Sin(x) + Power(k, 2) * Power(y, 2) * Sin(x) +
                   Power(z, 2) * Sin(x) + Power(k, 2) * Power(z, 2) * Sin(x) -
                   2 * y * Sin(y) + Power(x, 2) * Sin(z) +
                   Power(k, 2) * Power(x, 2) * Sin(z) + Power(y, 2) * Sin(z) +
                   Power(k, 2) * Power(y, 2) * Sin(z) +
                   Power(k, 2) * Power(z, 2) * Sin(z)) /
                  (Power(x, 2) + Power(y, 2) + Power(z, 2));
    return ret;
  };

  // Compute the analytic solution of the problem
  auto u_zero = [&](const Eigen::Vector3d x_vec) -> complex {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec / x_vec.norm();
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    return Cos(y) + Sin(x) + Sin(z);
  };

  // Compute the analytic solution of the problem
  auto u_one = [&](const Eigen::Vector3d x_vec) -> Eigen::Vector3cd {
    // first scale to the circle
    Eigen::Vector3d x_ = x_vec / x_vec.norm();
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
    Eigen::Vector3d x_ = x_vec / x_vec.norm();
    double x = x_(0);
    double y = x_(1);
    double z = x_(2);

    return Cos(y) + Sin(x) + Sin(z);
  };

  projects::hldo_sphere::experiments::HodgeLaplacianExperiment<complex>
      experiment(u_zero, u_one, u_two, f_zero_dirac, f_one_dirac, f_two_dirac,
                 k, "test_dirac");

  experiment.Compute(refinement_levels, ks);

  return 0;
}
