#ifndef MAZE_H_INCLUDED
#define MAZE_H_INCLUDED


#include "unit.h"
#include <fstream>

using std::vector;

// Ein Graph, der Koordinaten von Knoten speichert.
class MazeGraph : public DistanceGraph
{
    private:
        vector<CellType> maze;
        size_t breit;

    protected:
        vector<NeighborT> nachbarn;

    public:
        const NeighborT& getNeighbors( VertexT v) const override;

        CostT estimatedCost( VertexT from, VertexT to) const override;

        CostT cost( VertexT from, VertexT to) const override;

    friend ifstream& operator >> (ifstream& ifs, const MazeGraph& graph);
};

#endif // MAZE_H_INCLUDED
