#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include "common.h"

namespace cam {

struct ASPFunction {
    ASPFunction(Graph const& graph, Graph::EdgeMap<double> const& weight) 
        : dist_{graph} {
        for (Graph::NodeIt n(graph); n != INVALID; ++n) {
            dist_[n] = std::make_shared<Graph::NodeMap<double>>(graph);
            lemon::dijkstra(graph, weight).distMap(*dist_[n]).run(n);
        }
    }

    double dist(Graph::Node u, Graph::Node v) const {
        return (*dist_[u])[v];
    }

private:
    Graph::NodeMap<std::shared_ptr<Graph::NodeMap<double>>> dist_;
};

inline auto all_shorest_paths(Graph const& graph, Graph::EdgeMap<double> const& weight) {
    return std::make_unique<ASPFunction>(graph, weight);
}

struct ValueFunction {
    ValueFunction(CAM const& cam) 
        : mu_{cam.mu()}, t_(cam.t()), asp_{cam.g(), cam.w()} {
    }

    auto value(Chunk const& x, Chunk const& y, Graph::Node v) {
        return move(x, v) + move(y, v) + mu_(x.size(), y.size()) * asp_.dist(v, t_);
    }

    auto value(Chunk const& x, Chunk const& y) {
        return move(x, y.v()) + mu_(x.size(), y.size()) * asp_.dist(y.v(), t_);
    }

private:
    auto move(Chunk const& x, Graph::Node v) -> double {
        return x.size() * (asp_.dist(x.v(), v) - asp_.dist(x.v(), t_));
    }

private:
    Function mu_;
    Graph::Node t_;
    ASPFunction asp_;
};

inline auto values(CAM const& cam) {
    return make_unique<ValueFunction>(cam);
}



}
#endif
