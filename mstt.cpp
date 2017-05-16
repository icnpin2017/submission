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

    std::cout << cam::REC::solve(cam, filterEdges(cam.g(), steiner_tree));
}
