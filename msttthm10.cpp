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

    double cmin = 1e10;
    double cmax = 0;
    for (cam::Graph::EdgeIt e(cam.g()); e != lemon::INVALID; ++e) {
        if (cam.w()[e] < cmin) {
            cmin = cam.w()[e];
        }
        if (cam.w()[e] > cmax) {
            cmax = cam.w()[e];
        }
    }
    double delta = sqrt(2*cmin / (lemon::countNodes(cam.g()) * cmax));

    // std::cout << delta << std::endl;

    std::vector<cam::Graph::Node> vertices{};
    vertices.emplace_back(cam.t());
    for (auto c : cam.c()) {
        if (c.size() > delta) {
            vertices.emplace_back(c.v());
        }
    }

    cam::Graph::EdgeMap<bool> steiner_tree{cam.g(), false};
    cam::build_steiner_tree(cam.g(), cam.w(), vertices, &steiner_tree);

    std::cout << cam::REC::solve(cam, filterEdges(cam.g(), steiner_tree), delta);
}
