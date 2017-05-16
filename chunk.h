#ifndef CHUNK_H
#define CHUNK_H

#include "common.h"

namespace cam {

struct Chunk{
    Chunk() 
        : v_{}, size_{-1} {
    }

    Chunk(Graph::Node v, double size)
        : v_{v}, size_{size}{
    }

    Chunk(Graph::Node v, vector<int> elems)
        : v_{v}, size_{double(elems.size())}, elems_{elems}{
    }

    Chunk(Graph::Node v, Chunk const &chunk)
        : v_{v}, size_{chunk.size()}, elems_{chunk.elems()}{
    }

    Graph::Node v() const {
        return v_;
    }
    
    double size() const {
        return size_;
    }

    vector<int> elems() const {
        return elems_;
    }

private:
    Graph::Node v_;
    double const size_;
    vector<int> const elems_;
};

} // namespace cam

#endif // CHUNK_H
