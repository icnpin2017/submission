#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cmath>

namespace cam {

struct Function {
    Function() = default; 

    Function(std::string const& name) : name_(name) {
        if (name == "dict") {
            mu_ = [] (double x, double y) -> double {
                return std::log(std::exp(x) + std::exp(y));
            };
        } else if (name == "max") {
            mu_ = [] (double x, double y) -> double {
                return std::max(x, y);
            };
        } else if (name == "min") {
            mu_ = [] (double x, double y) -> double {
                return std::min(x, y);
            };
        } else if (name == "euqlid") {
            mu_ = [] (double x, double y) -> double {
               return std::sqrt(x * x + y * y);
            };
        } else if (name == "") {
            mu_ = [] (double x, double y) -> double {
               return 0;
            };
        } else {
            abort();
        }
    }

    auto operator()(double x, double y) const -> double {
        return mu_(x, y);
    }

    auto name() const -> std::string const& {
        return name_;
    }

private:
    std::function<double(double, double)> mu_;
    std::string name_;
};

struct VectorFunction {
    VectorFunction() = default; 

    VectorFunction(std::string const& name) : name_(name) {
        if (name == "setunion") {
            mu_ = [] (std::vector<int> x, std::vector<int> y) -> std::vector<int> {
                std::vector<int> res;
                std::set_union(x.begin(), x.end(), y.begin(), y.end(), std::back_inserter(res));
                return res;
            };
        } else if (name == "choosebest") {
            mu_ = [] (std::vector<int> x, std::vector<int> y) -> std::vector<int> {
                if (x.size() == 0) {
                    return y;
                }
                if (y.size() == 0) {
                    return x;
                }
                if (y[0] > x[0]) {
                    return y;
                }
                return x;
            };
        // } else if (name == "min") {
        //     mu_ = [] (double x, double y) -> double {
        //         return std::min(x, y);
        //     };
        // } else if (name == "euqlid") {
        //     mu_ = [] (double x, double y) -> double {
        //        return std::sqrt(x * x + y * y);
        //     };
        } else if (name == "") {
            mu_ = [] (const std::vector<int> &x, const std::vector<int> &y) -> std::vector<int> {
               return std::vector<int>();
            };
        } else {
            abort();
        }
    }

    auto operator()(const std::vector<int> &x, const std::vector<int> &y) const -> std::vector<int> {
        return mu_(x, y);
    }

    auto name() const -> std::string const& {
        return name_;
    }

private:
    std::function<std::vector<int>(const std::vector<int> &, const std::vector<int> &)> mu_;
    std::string name_;
};

}

#endif // FUNCTIONS_H
