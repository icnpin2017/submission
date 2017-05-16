#ifndef SOLUTION_H
#define SOLUTION_H

#include "actions.h"
#include "cam.h"

namespace cam {

struct Solution {
    Solution(CAM const& cam)
        : cam_{cam} {
    }

    void add_action(CAMAction const& action) {
        actions_.emplace_back(action);
    }

    auto actions() const -> vector<CAMAction> const& {
        return actions_;
    }

    auto cam() const -> CAM const& {
        return cam_;
    }

private:
    CAM const& cam_;
    vector<CAMAction> actions_;
};

inline std::ostream& operator<<(std::ostream& out, Solution const& solution) {
    out << solution.actions().size() << "\n";
    for (auto a : solution.actions()) {
        out << a.is_finish() << " ";
        if (!a.is_finish()) {
            out << a.x() << " " << a.y() << " " << solution.cam().idx(a.v()) << "\n";
        } else {
            out << a.x() << " " << solution.cam().idx(a.v()) << "\n";
        }
    }
    return out;
}

inline std::istream& operator>>(std::istream& in, Solution& solution) {
    unordered_map<int, Graph::Node> node_map{};
    for (Graph::NodeIt n(solution.cam().g()); n != INVALID; ++n) {
        node_map[solution.cam().idx(n)] = n;
    }

    int num_actions;
    in >> num_actions;
    for (auto i = 0; i < num_actions; ++i) {
        bool is_finish;
        in >> is_finish;
        if (!is_finish) {
            size_t x, y; int idx;
            in >> x >> y >> idx;
            solution.add_action(CAMAction(x, y, node_map[idx]));
        } else {
            size_t x;
            in >> x;
            solution.add_action(CAMAction(x));
        }
    }
    return in;
}

}

#endif // SOLUTION_H
