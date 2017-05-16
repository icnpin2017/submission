#include <vector>
#include <iterator>
#include <iostream>
#include <set>
#include <map>

#include <lemon/dijkstra.h>
#include <lemon/kruskal.h>
#include <lemon/edge_set.h>

#include "cam.h"
#include "rec.h"
#include "utils.h"

struct CAMHPQ {
    void emplace(double value, size_t x, size_t y, cam::Graph::Node v) {
        if (x == y)
            return;
        if (x > y)
            std::swap(x, y);
        erase(x, y, v);

        rev_[std::make_tuple(x, y, v)] = pq_.emplace(std::make_tuple(value, x, y, v)).first;
    }

    void erase(size_t x, size_t y, cam::Graph::Node v) {
        if (x == y)
            return;
        if (x > y)
            std::swap(x, y);
        auto key = std::make_tuple(x, y, v);
        if (rev_.count(key)) {
            pq_.erase(rev_[key]);
            rev_.erase(key);
        }
    }

    auto top() const -> std::tuple<double, size_t, size_t, cam::Graph::Node> {
        return *pq_.begin();
    }

    void print() {
        std::set<size_t> touched;
        for (auto x : pq_) {
            touched.emplace(std::get<1>(x));
            touched.emplace(std::get<2>(x));
        }
        std::cerr << "touched: ";
        for (auto x : touched) {
            std::cerr << x << " ";
        }
        std::cerr << "\n";
    }

private:
    using PrioritizedSet = std::set<std::tuple<double, size_t, size_t, cam::Graph::Node>>;
    std::map<std::tuple<size_t, size_t, cam::Graph::Node>, PrioritizedSet::iterator> rev_;
    PrioritizedSet pq_;
};

int main(int argc, char const * argv[]) {
    std::ios_base::sync_with_stdio(false);
    assert(argc > 1);
    
    // cam::CAM cam{argv[1]};
    cam::CAM cam = cam::generate_cam(argv[1]);

    std::cin >> cam;

    std::unique_ptr<cam::AbstractChunkSet> c = cam::generate_chunkset(cam);
    // {cam};
    auto vs = cam::values(cam);

    CAMHPQ pq{};
    for (auto x : c->chunks()) {
        for (auto y : c->chunks()) {
            for (cam::Graph::NodeIt n(cam.g()); n != cam::INVALID; ++n) {
                pq.emplace(vs->value(x.second, y.second, n), x.first, y.first, n);
            }
        }
    }

    cam::Solution s{cam};
    while (c->chunks().size() > 1) {
        size_t xi, yi; cam::Graph::Node v;
        std::tie(std::ignore, xi, yi, v) = pq.top();
        auto a = cam::CAMAction(xi, yi, v);
        for (auto z : c->chunks()) {
            for (cam::Graph::NodeIt n(cam.g()); n != cam::INVALID; ++n) {
                pq.erase(z.first, xi, n);
                pq.erase(z.first, yi, n);
            }
        }
        auto ri = c->apply(a);
        auto r = c->chunk(ri);

        for (auto z : c->chunks()) {
            for (cam::Graph::NodeIt n(cam.g()); n != cam::INVALID; ++n) {
                pq.emplace(vs->value(z.second, r, n), z.first, ri, n);
            }
        }

        // std::cout << "chunks:\n";
        // std::cout << (*c);
        s.add_action(a);
    }
    s.add_action(cam::CAMAction(c->chunks().begin()->first));

    std::cout << s;
}
