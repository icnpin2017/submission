#include "chunk_set.h"
#include "cam.h"
#include "rec.h"

int main(int argc, char const * argv[]) {
    std::ios_base::sync_with_stdio(false);

    cam::CAM cam = cam::generate_cam(argv[1]);

    std::cin >> cam;

    cam::ChunkSet c{cam};

    std::vector<size_t> cs{};
    for (auto cc : c.chunks())
        cs.emplace_back(cc.first);

    cam::Solution s{cam};
    while (cs.size() > 1) {
        auto a = cam::CAMAction(cs.front(), cs.back(), cam.t());
        s.add_action(a);
        cs.front() = c.apply(a);
        cs.pop_back();
    }
    s.add_action(cam::CAMAction(cs.front()));

    std::cout << s;
}

