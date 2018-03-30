#include<iostream>
#include<ceres/ceres.h>
//使用自动求导数
//非线性最小二乘　初值影响结果
//f1=x1^2+2*x2^2-1
//f2=2*x1+x2-2
//min(f1^2+f2^2)
using namespace std;
using namespace ceres;
//提供残差　初值

//第一部分：构建代价函数，重载（）符号，仿函数
struct CostFunctor1 {
   template <typename T>
   bool operator()(const T* const x0,const T* const x1, T* residual) const {
	residual[0] = x0[0]*x0[0]+T(2.0)*x1[0]*x1[0]-T(1.0);
	return true;
   }
};

struct CostFunctor2 {
   template <typename T>
   bool operator()(const T* const x0,const T* const x1, T* residual) const {
   	residual[0] = T(2.0)*x0[0] + x1[0]-T(2.0);
    return true;
   }
};


//主函数
int main(int argc, char** argv) {

   //初始值设置
	double x0 = 1.2;
	double x1 = 0.1;
  
  // 第二部分：构建寻优问题
	Problem problem;
	problem.AddResidualBlock(
	new AutoDiffCostFunction<CostFunctor1, 1, 1,1>(new CostFunctor1),//AutoDiffCostFunction自动求导　NumericDiffCostFunction数值求导
	NULL,&x0,&x1
                );
	problem.AddResidualBlock(
	new AutoDiffCostFunction<CostFunctor2, 1, 1,1>(new CostFunctor2),
 	NULL,&x0,&x1
                );

    //另一种写法
    //CostFunction* cost_function =
    //new AutoDiffCostFunction<CostFunctor, 1, 1,1>(new CostFunctor); //使用自动求导，将之前的代价函数结构体传入，第一个1是输出维度，即残差的维度，第二个1是输入维度，即待寻优参数x的维度。
    //problem.AddResidualBlock(cost_function, NULL, x); //向问题中添加误差项，本问题比较简单，添加一个就行。
//参数NULL是指不使用核函数，&x表示x是待寻优参数
  //第三部分： 配置并运行求解器
    Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR; //配置增量方程的解法
    options.minimizer_progress_to_stdout = true;//输出到cout
    Solver::Summary summary;//优化信息
    Solve(options, &problem, &summary);//求解!!!

    std::cout << summary.BriefReport() << "\n";//输出优化的简要信息
//最终结果
    std::cout << " -> " << x0<<" "<<x1 << "\n";
    
    return 0;
}
