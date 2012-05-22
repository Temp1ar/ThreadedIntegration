#include <iostream>
#include <functional>
#include <boost/thread.hpp>
#include <numeric>

void calculate(std::function<double(double)> f, double a, double b, 
    double& result)
{
    result = (b - a) * ( f(a) + f(b) ) / 2;
}

double integrate(std::function<double(double)> f, double a, double b, 
    size_t chunks, size_t thread_number) 
{
    double step = (b - a) / chunks;
    std::vector<double> result(chunks + 1, 0);
    std::vector<boost::thread> threadArray;

    for(double i = a, j = 0; i < b - step; i += step, ++j) {
        threadArray.push_back(boost::thread 
            (boost::bind(calculate, f, i, i + step, std::ref(result[j]))));
    }

    std::for_each(threadArray.begin(), threadArray.end(), 
        [](boost::thread& t){ t.join(); });

    return std::accumulate(result.begin(), result.end(), 0.0);
}

double f(double x) 
{
    return x*x*x;
}

int main() 
{
    std::cout << integrate(f, 0, 5, 100, 100);
    return 0;
}