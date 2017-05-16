#include <memory>

#include <lemon/dijkstra.h>

#include "solution.h"
#include "chunk_set.h"
#include "utils.h"

int main(int argc, char const * argv[]) {
    std::ios_base::sync_with_stdio(false);
    assert(argc > 1);
    cam::CAM cam = cam::generate_cam(argv[1]);

    std::cin >> cam;
    cam::Solution s{cam};
    std::cin >> s;
    cam::ChunkSet c{cam};

    auto asp = cam::all_shorest_paths(cam.g(), cam.w()); 

    double res = 0;

    for (auto a : s.actions()) {
        if (!a.is_finish()) {
            res += c.chunk(a.x()).size() * asp->dist(c.chunk(a.x()).v(), a.v());
            res += c.chunk(a.y()).size() * asp->dist(c.chunk(a.y()).v(), a.v());
        } else {
            res += c.chunk(a.x()).size() * asp->dist(c.chunk(a.x()).v(), cam.t());
        }
        c.apply(a);
    }

    std::cout << res;
}
