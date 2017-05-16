#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include "common.h"
#include <lemon/dijkstra.h>

namespace cam {

struct Closure {
public:
    Closure(Graph const& orig_graph, WeightMap const& orig_weights)
        : orig_graph_{orig_graph}, orig_weights_{orig_weights}, 
          graph_{}, w_{graph_}, 
          to_orig_{graph_}, to_closure_{orig_graph_, INVALID}, 
          sps_{orig_graph_} {
    }

    auto const& g() {
        return graph_;
    }

    auto const& w() const {
        return w_;
    }

    auto to_original(Graph::Node v) {
        return to_orig_[v];
    }

    auto to_closure(Graph::Node v) {
        return to_closure_[v];
    }

    void add_vertex(Graph::Node u) {
        if (!add_vertex_mapping(u)) {
            return;
        }

        auto const cu = to_closure(u);

        sps_[u] = std::make_shared<Dijkstra>(orig_graph_, orig_weights_);
        sps_[u]->run(u);

        for (Graph::NodeIt cv(graph_); cv != INVALID; ++cv) {
            add_edge(cu, cv, sps_[u]->dist(to_original(cv)));
        }
    }

    auto get_original_path(Graph::Edge ce) {
        auto const u = to_original(graph_.u(ce));
        auto const v = to_original(graph_.v(ce));
        auto const p = sps_[u]->path(v);

        std::vector<Graph::Edge> result;
        for (Dijkstra::Path::RevArcIt ep(p); ep != INVALID; ++ep) {
            result.emplace_back(Graph::Arc(ep));
        }
        return result;
    }

private:
    using Dijkstra = lemon::Dijkstra<Graph, Graph::EdgeMap<double>>;
    Graph const& orig_graph_;
    WeightMap const& orig_weights_;

    Graph graph_;
    WeightMap w_;

    Graph::NodeMap<Graph::Node> to_orig_;
    Graph::NodeMap<Graph::Node> to_closure_;
    Graph::NodeMap<std::shared_ptr<Dijkstra>> sps_;

    auto add_vertex_mapping(Graph::Node u) -> bool {
        if (to_closure(u) != INVALID) {
            return false;
        }

        auto cl_v = graph_.addNode();
        to_closure_[u] = cl_v;
        to_orig_[cl_v] = u;

        return true;
    }

    void add_edge(Graph::Node u, Graph::Node v, double weight) {
        auto e = graph_.addEdge(u, v);
        w_[e] = weight;
    }
};

inline void build_steiner_tree(
        Graph const& g, WeightMap const& w, vector<Graph::Node> const& vertices, 
        Graph::EdgeMap<bool> * out) {
    assert(out != nullptr);

    Closure closure{g, w};
    for (auto u : vertices) {
        closure.add_vertex(u);
    }

    std::vector<cam::Graph::Edge> closure_tree{};
    lemon::kruskal(closure.g(), closure.w(), std::back_inserter(closure_tree));
    
    Graph::EdgeMap<bool> orig_tree{g, false};
    for (auto ce : closure_tree) {
        for (auto e : closure.get_original_path(ce)) {
            orig_tree[e] = true;
        }
    }

    lemon::kruskal(filterEdges(g, orig_tree), w, *out);
}

}

#endif // GRAPH_UTILS_H
