#include <vector>
#include <iterator>
#include <iostream>

#include <lemon/kruskal.h>
#include <lemon/edge_set.h>

#include "cam.h"
#include "rec.h"

#include "graph_utils.h"

int main(int argc, char const * argv[]) {
    std::ios_base::sync_with_stdio(false);
    assert(argc > 1);
    cam::CAM cam = cam::generate_cam(argv[1]);

    std::cin >> cam;

    std::vector<cam::Graph::Node> vertices{};
    vertices.emplace_back(cam.t());
    for (auto c : cam.c()) {
        vertices.emplace_back(c.v());
    }

    cam::Graph::EdgeMap<bool> steiner_tree{cam.g(), false};
    cam::build_steiner_tree(cam.g(), cam.w(), vertices, &steiner_tree);

    size_t M = 0;
    for (auto c : cam.c()) {
        if (c.size() > M) {
            M = c.size();
        }
    }

    size_t cur_M = M / 2;

    // init steiner elements
    cam::Graph::EdgeMap<bool> steiner_elements{cam.g()};
    for (cam::Graph::EdgeIt e(cam.g()); e!=lemon::INVALID; ++e) {
        steiner_elements[e] = false;
    }
    while (cur_M >= 0) {
        // filter edges
        cam::WeightMap wm{cam.g()};
        for (cam::Graph::EdgeIt e(cam.g()); e!=lemon::INVALID; ++e) {
            if (steiner_elements[e]) {
                wm[e] = 0;
            } else {
                wm[e] = cam.w()[e];
            }
        }
        // filter chunks
        std::vector<cam::Graph::Node> vertices{};
        vertices.emplace_back(cam.t());
        for (auto c : cam.c()) {
            if (c.size() > cur_M) {
                vertices.emplace_back(c.v());
            }
        }
        cam::Graph::EdgeMap<bool> steiner_tree{cam.g(), false};
        cam::build_steiner_tree(cam.g(), wm, vertices, &steiner_tree);
        // add to steiner_elements
        for (cam::Graph::EdgeIt e(cam.g()); e!=lemon::INVALID; ++e) {
            if (steiner_tree[e]) {
                steiner_elements[e] = true;
            }
        }
        if (cur_M == 0) {
            std::cout << cam::REC::solve(cam, filterEdges(cam.g(), steiner_tree));
            break;
        }
        cur_M = cur_M / 2;
    }

}
