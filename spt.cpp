#include <lemon/dijkstra.h>

#include "cam.h"
#include "rec.h"

int main(int argc, char const * argv[]) {
    std::ios_base::sync_with_stdio(false);
    assert(argc > 1);
    cam::CAM cam = cam::generate_cam(argv[1]);

    std::cin >> cam;

    cam::Graph::EdgeMap<bool> tree{cam.g(), false};
    cam::Graph::NodeMap<cam::Graph::Edge> pred{cam.g()};
    lemon::dijkstra(cam.g(), cam.w()).predMap(pred).run(cam.t());
    for (cam::Graph::NodeIt n(cam.g()); n != cam::INVALID; ++n) {
        if (pred[n] != cam::INVALID) {
            tree[pred[n]] = true;
        }
    }

    std::cout << cam::REC::solve(cam, filterEdges(cam.g(), tree));
}
