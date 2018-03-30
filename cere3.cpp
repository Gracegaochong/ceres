#include "ceres/ceres.h"
#include "glog/logging.h"
//自己计算雅各比矩阵
//f1=x1^2+2*x2^2-1
//f2=2*x1+x2-2
//min(f1^2+f2^2)

class f1 : public ceres::FirstOrderFunction {
 public:
  virtual ~f1() {}

  virtual bool Evaluate(const double* parameters,
                        double* cost,
                        double* gradient) const {
    const double x = parameters[0];
    const double y = parameters[1];

    cost[0] = (x * x + 2.0*y*y-1.0)*(x * x + 2.0*y*y-1.0)+(2.0*x+y-2.0)*(2.0*x+y-2.0);
    if (gradient != NULL) {
      gradient[0] = 2.0 *(x * x + 2.0*y*y-1.0)+2*(2.0*x+y-2.0);
      gradient[1] = 2.0 *(x * x + 2.0*y*y-1.0)*4.0*y+2*(2.0*x+y-2.0);
    }
    return true;
  }

  virtual int NumParameters() const { return 2; }
};


int main(int argc, char** argv) {

  double parameters[2] = {1, 0.1};
  ceres::GradientProblem problem(new f1());

  ceres::GradientProblemSolver::Options options;//3配置并运行求解器
  options.minimizer_progress_to_stdout = true;//输出到cout

  ceres::GradientProblemSolver::Summary summary;//优化信息

  ceres::Solve(options, problem, parameters, &summary);//求解

  std::cout << summary.FullReport() << "\n";

  std::cout << " x: " << parameters[0]
            << " y: " << parameters[1] << "\n";
  return 0;
}
