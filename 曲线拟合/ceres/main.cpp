#include <iostream>
#include <opencv2/core/core.hpp>
#include <ceres/ceres.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string> 
using namespace std;

// 代价函数的计算模型
struct CURVE_FITTING_COST
{
    CURVE_FITTING_COST ( double x, double y ) : _x ( x ), _y ( y ) {}
    // 残差的计算
    template <typename T>
    bool operator() (
            const T* const abc,     // 模型参数，有3维
            T* residual ) const     // 残差
    {
        residual[0] = T ( _y ) - abc[0]/abs(T ( _x ))-abc[1]; // y-exp(ax^2+bx+c)
        return true;
    }
    const double _x, _y;    // x,y数据
};

int main ( int argc, char** argv )
{
    double a=5.0;         // 真实参数值
    int N=100;                          // 数据点
    double w_sigma=1.0;                 // 噪声Sigma值
    cv::RNG rng;                        // OpenCV随机数产生器
    double abc[2] = {0,0};            // abc参数的估计值

    stringstream ss;

    vector<double> x_data, y_data;      // 数据
    ifstream infile;
    infile.open("points.txt");
    string s;
    int i=0;
    while(getline(infile,s))
    {   
        double x,y;
        istringstream ss(s);
        ss>>x>>y;
        x_data.push_back(x);
        y_data.push_back(y);
       // cout<<x<<" "<<y<<"\n";
        
    }
    for(int j=0; j<x_data.size();j++)   
    {
        cout<<x_data[j]<<" ";
        cout<<y_data[j]<<endl;
    }
    
   // cout<<"generating data: "<<endl;
    // for ( int i=0; i<N; i++ )
    // {
        
    //     double x = i/100.0;
    //     x_data.push_back ( x );
    //     y_data.push_back (
    //             a*x //+ rng.gaussian ( w_sigma )
    //     );
    //     cout<<x_data[i]<<" "<<y_data[i]<<endl;
    //     ss << x_data[i] << " " << y_data[i] << endl;
    // }

    // //将生成的点坐标保存到points.txt
    // ofstream file("points.txt");
    // file << ss.str();
    
    // 构建最小二乘问题
    ceres::Problem problem;
    for ( int i=0; i<x_data.size(); i++ )
    {
        problem.AddResidualBlock (     // 向问题中添加误差项
                // 使用自动求导，模板参数：误差类型，输出维度，输入维度，维数要与前面struct中一致
                new ceres::AutoDiffCostFunction<CURVE_FITTING_COST, 1, 2> (
                        new CURVE_FITTING_COST ( x_data[i], y_data[i] )
                ),
                nullptr,            // 核函数，这里不使用，为空
                abc                 // 待估计参数
        );
    }

    // 配置求解器
    ceres::Solver::Options options;     // 这里有很多配置项可以填
    options.linear_solver_type = ceres::DENSE_QR;  // 增量方程如何求解
    options.minimizer_progress_to_stdout = true;   // 输出到cout

    ceres::Solver::Summary summary;                // 优化信息
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    ceres::Solve ( options, &problem, &summary );  // 开始优化
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
    //cout<<"solve time cost = "<<time_used.count()<<" seconds. "<<endl;

    // 输出结果
    cout<<summary.BriefReport() <<endl;
    cout<<"estimated "<<"\n";
    cout<<abc[0]<<" "<<abc[1]<<"\n";
    cout<<endl;

    return 0;
}
