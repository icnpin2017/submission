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

struct MCAMHPQ {
    void emplace(double value, size_t x, size_t y) {
        if (x == y)
            return;
        erase(x, y);

        rev_[std::make_tuple(x, y)] = pq_.emplace(std::make_tuple(value, x, y)).first;
    }

    void erase(size_t x, size_t y) {
        if (x == y)
            return;
        auto key = std::make_tuple(x, y);
        if (rev_.count(key)) {
            pq_.erase(rev_[key]);
            rev_.erase(key);
        }
    }

    auto top() const -> std::tuple<double, size_t, size_t> {
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
    using PrioritizedSet = std::set<std::tuple<double, size_t, size_t>>;
    std::map<std::tuple<size_t, size_t>, PrioritizedSet::iterator> rev_;
    PrioritizedSet pq_;
};

int main(int argc, char const * argv[]) {
    std::ios_base::sync_with_stdio(false);
    assert(argc > 1);
    // cam::CAM cam{argv[1]};
    cam::CAM cam = cam::generate_cam(argv[1]);

    std::cin >> cam;

    cam::ChunkSet c{cam};
    auto vs = cam::values(cam);

    MCAMHPQ pq{};
    for (auto x : c.chunks()) {
        for (auto y : c.chunks()) {
#ifdef MCAMLE
            if (x.second.size() <= y.second.size()) {
                pq.emplace(vs->value(x.second, y.second), x.first, y.first);
            }
#else
            pq.emplace(vs->value(x.second, y.second), x.first, y.first);
#endif
        }
    }

    cam::Solution s{cam};
    while (c.chunks().size() > 1) {
        size_t xi, yi;
        std::tie(std::ignore, xi, yi) = pq.top();
        auto a = cam::CAMAction(xi, yi, c.chunk(yi).v());
        for (auto z : c.chunks()) {
            pq.erase(z.first, xi);
            pq.erase(xi, z.first);
            pq.erase(z.first, yi);
            pq.erase(yi, z.first);
        }
        auto ri = c.apply(a);
        auto r = c.chunk(ri);

        for (auto z : c.chunks()) {
            pq.emplace(vs->value(z.second, r), z.first, ri);
            pq.emplace(vs->value(r, z.second), ri, z.first);
        }

        s.add_action(a);
    }
    s.add_action(cam::CAMAction(c.chunks().begin()->first));

    std::cout << s;
}
