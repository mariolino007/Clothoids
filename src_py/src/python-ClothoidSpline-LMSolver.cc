/**
 * PYTHON Wrapper for Clothoids
 *
 * License MIT - See LICENSE file
 * 2019 Matteo Ragni, Claudio Kerov Ghiglianovich,
 *      Enrico Bertolazzi, Marco Frego
 */

#ifdef LMSOLVE_CLOTHOID_SPLINE

#include "python-ClothoidSpline-LMSolver.hh"

#include <algorithm>

namespace G2lib {
namespace Interpolation {

  LMSolver::ClothoidSplineProblem::ClothoidSplineProblem(LMSolver & solver) : 
    SparseFunctor(solver.theta_size(), solver.constraints_size()),
    m_solver(solver) {
    m_jacobian_rows.reserve(m_solver.jacobian_pattern_size());
    m_jacobian_cols.reserve(m_solver.jacobian_pattern_size());
    m_jacobian_result.reserve(m_solver.jacobian_pattern_size());
    m_solver.spline().jacobian_pattern(&m_jacobian_rows.front(), &m_jacobian_cols.front());
  }

  int LMSolver::ClothoidSplineProblem::operator()(
    const SparseFunctor::InputType & theta, 
    SparseFunctor::ValueType & constraints_value) const {
    if (m_solver.spline().constraints(theta.data(), constraints_value.data()))
      return 0;
    return 1;
  }

  int LMSolver::ClothoidSplineProblem::df(
    const SparseFunctor::InputType & theta, 
    SparseFunctor::JacobianType & jacobian_value) {
    if (!(m_solver.spline().jacobian(theta.data(), &m_jacobian_result.front())))
      return 1;
    for (int i = 0; i < m_solver.jacobian_pattern_size(); i++) {
      jacobian_value.coeffRef(m_jacobian_rows[i], m_jacobian_cols[i]) = m_jacobian_result[i];
    }
    jacobian_value.makeCompressed();
    return 0;
  }

  bool LMSolver::solve() {
    LMSolver::ClothoidSplineProblem problem(*this);
    Eigen::LevenbergMarquardt<LMSolver::ClothoidSplineProblem> lm(problem);
    lm.setFtol(1e-20);

    Eigen::VectorXd theta_opts = Eigen::VectorXd::Map(theta_solution().data(), theta_solution().size());
    lm.minimize(theta_opts);
    Eigen::ComputationInfo info = lm.info();
    std::copy(theta_opts.data(), theta_opts.data() + theta_opts.size(), theta_solution().begin());
    return (info == Eigen::ComputationInfo::Success);
  }

} /* namespace Interpolation */
} /* namespace G2lib */

#endif