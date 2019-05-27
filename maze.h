#ifndef MAZE_H_INCLUDED
#define MAZE_H_INCLUDED


#include "unit.h"
#include <fstream>

using std::vector;
using::std::ifstream;

// Ein Graph, der Koordinaten von Knoten speichert.
class MazeGraph : public DistanceGraph
{
    private:
        vector<CellType> maze;
        size_t hoe, breit;
        VertexT start, ziel;

    protected:
        vector<NeighborT> nachbarn;

    public:
        const NeighborT& getNeighbors( VertexT v) const override;

        CostT estimatedCost( VertexT from, VertexT to) const override;

        CostT cost( VertexT from, VertexT to) const override;\

        void setVertexCount(size_t cow) { vertexCount = cow; }

        void setzeNachbarn();

        void setzeMaze(vector<CellType> m) { maze = m; }

        void setzeHoehe(int h) { hoe = h; }

        void setzeBreite(int b) { breit = b; }

        size_t getStart() const { return start; }

        size_t getZiel() const { return ziel; }

    friend ifstream& operator >> (ifstream& ifs, MazeGraph& graph);
};

#endif // MAZE_H_INCLUDED
