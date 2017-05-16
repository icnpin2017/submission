#ifndef ACTIONS_H
#define ACTIONS_H

#include "common.h"

namespace cam {

struct CAMAction {
    CAMAction(size_t x, size_t y, Graph::Node v)
        : x_{x}, y_{y}, v_{v} {
    }

    CAMAction(size_t x)
        : x_{x}, y_{NOT_SET}, v_{INVALID} {
    }

    auto x() const {
        return x_;
    }

    auto y() const {
        return y_;
    }

    auto v() const {
        return v_;
    }

    auto is_finish() const {
        return v_ == INVALID;
    }

private:
    size_t x_;
    size_t y_;
    Graph::Node v_;
};

} // namespace cam

#endif // ACTIONS_H
