#ifndef CHUNK_SET_H
#define CHUNK_SET_H

#include "chunk.h"
#include "cam.h"
#include "actions.h"

#include <memory>

namespace cam {

struct AbstractChunkSet {
    AbstractChunkSet(CAM const& cam) : chunks_{}, idx_{0}, t_{cam.t()} {
        for (auto c : cam.c()) {
            chunks_.emplace(idx_++, c);
        }
    }

    auto chunks() const -> unordered_map<size_t, Chunk> const& {
        return chunks_;
    }

    auto chunk(size_t idx) const -> Chunk {
        return chunks_.at(idx);
    }

    virtual auto apply_mu(Graph::Node v, Chunk const &x, Chunk const &y) const -> Chunk = 0;

    size_t apply(CAMAction const& action) {
        if (action.is_finish()) {
            auto x = chunks_.at(action.x());
            chunks_.erase(action.x());
            chunks_.emplace(idx_, Chunk(t_, x.size()));
        } else {
            auto x = chunks_.at(action.x());
            chunks_.erase(action.x());
            auto y = chunks_.at(action.y());
            chunks_.erase(action.y());
            chunks_.emplace(idx_, apply_mu(action.v(), x, y));
        }
        return idx_++;
    }

protected: 
    unordered_map<size_t, Chunk> chunks_;
    size_t idx_;
    Graph::Node t_;
};

struct ChunkSet : AbstractChunkSet {
    ChunkSet(CAM const& cam) : AbstractChunkSet(cam), mu_{cam.mu()} {}

    virtual auto apply_mu(Graph::Node v, Chunk const &x, Chunk const &y) const -> Chunk {
        return Chunk( v, mu_(x.size(), y.size()) );
    }
private:
    Function mu_;
};

struct VectorChunkSet : AbstractChunkSet {
    VectorChunkSet(CAM const& cam) : AbstractChunkSet(cam), mu_{cam.vmu()} {}

    virtual auto apply_mu(Graph::Node v, Chunk const &x, Chunk const &y) const -> Chunk {
        return Chunk( v, mu_(x.elems(), y.elems()) );
    }
private:
    VectorFunction mu_;
};


// template<class T>
inline std::ostream& operator<<(std::ostream& out, AbstractChunkSet const& c) {
    out << c.chunks().size() << "\n";
    for (auto chunk : c.chunks()) {
        out << chunk.first << " " << chunk.second.size();
        for (auto i=0; i < chunk.second.size(); ++i) {
            out << " " << chunk.second.elems()[i];
        }
        out << "\n";
    }
    return out;
};

std::unique_ptr<AbstractChunkSet> generate_chunkset(CAM const& cam) {
  if ( cam.vmu().name() != "" ) {
      return std::make_unique<VectorChunkSet>(cam);
  } else {
      return std::make_unique<ChunkSet>(cam);
  }
}


}

#endif // CHUNK_SET_H

// for f in tests/swdc_ring.graph.32.4/cam.nc10.maxe10.minc1.maxc10.mine1.maxe50.txt tests/jelly_fish.graph.20.20.30.20.16.10/cam.nc10.maxe10.minc1.maxc10.mine1.maxe50.txt tests/hypercube.graph.80.8/cam.nc10.maxe10.minc1.maxc10.mine1.maxe50.txt
// do
//     python3 sim_ng.py --func choosebest --dir csv_choosebest $f
// done

