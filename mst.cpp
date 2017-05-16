#include <lemon/kruskal.h>

#include "cam.h"
#include "rec.h"

int main(int argc, char const * argv[]) {
    std::ios_base::sync_with_stdio(false);
    assert(argc > 1);

    // cam::CAM cam{argv[1]};
    cam::CAM cam = cam::generate_cam(argv[1]);
    std::cin >> cam;

    cam::Graph::EdgeMap<bool> tree{cam.g(), false};
    lemon::kruskal(cam.g(), cam.w(), tree);

    std::cout << cam::REC::solve(cam, filterEdges(cam.g(), tree));
}

