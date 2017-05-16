#ifndef REC_H
#define REC_H

#include "cam.h"
#include "solution.h"
#include "chunk_set.h"

namespace cam {

struct REC {
public:
    static auto solve(CAM const& cam) -> Solution {
        return solve(cam, cam.g());
    }

    template<class Graph>
    static auto solve(CAM const& cam, Graph const& graph, double delta = -1) -> Solution {
        REC rec{cam, delta};
        auto finish = rec.solve(graph, cam.t(), INVALID);

        for (auto& rem_c : rec.remaining_chunks_) {
            auto action = CAMAction(finish, rem_c, cam.t());
            rec.solution_.add_action(action);
            finish = rec.chunks_.apply(action);
        }

        rec.solution_.add_action(CAMAction(finish));
        return rec.solution_;
    }

private:
    REC(CAM const& cam) 
        : cam_{cam}, chunks_{cam}, solution_{cam}, initial_chunks_{cam.g()}, remaining_chunks_{} {
        for (auto c : chunks_.chunks()) {
            initial_chunks_[c.second.v()].emplace_back(c.first);
        }
    }

    REC(CAM const& cam, double delta)
        : cam_{cam}, chunks_{cam}, solution_{cam}, initial_chunks_{cam.g()}, remaining_chunks_{} {
        for (auto c : chunks_.chunks()) {
            if (c.second.size() > delta) {
                initial_chunks_[c.second.v()].emplace_back(c.first);
            } else {
                remaining_chunks_.emplace_back(c.first);
            }
        }
    }

    template<class Graph>
    size_t solve(Graph const& graph, typename Graph::Node n, typename Graph::Node parent) {
        auto merged_here = initial_chunks_[n];
        for (typename Graph::IncEdgeIt e(graph, n); e != INVALID; ++e) {
            if (graph.runningNode(e) != parent) {
                auto sub_root = solve(graph, graph.runningNode(e), n);
                if (sub_root != NOT_SET) {
                    merged_here.emplace_back(sub_root);
                }
            }
        }
        if (merged_here.empty()) {
            return NOT_SET;
        }
        auto result = merged_here[0];
        for (auto i = 1u; i < merged_here.size(); i++) {
            auto action = CAMAction(result, merged_here[i], n); 
            solution_.add_action(action);
            result = chunks_.apply(action);
        }
        return result;
    }

private:
    CAM const& cam_;

    ChunkSet chunks_;
    Solution solution_;

    Graph::NodeMap<vector<size_t>> initial_chunks_;
    vector<size_t> remaining_chunks_;
};

}

#endif // REC_H
