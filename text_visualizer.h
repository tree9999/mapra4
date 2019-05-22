/*****************************************************************
*  Name       : text_visualizer.h                                *
*  Verwendung : Implementation einer "Visualisierung" ueber      *
*               die Konsole.                                     *
*  Autor      : Sven Gross, Hauke Sass, Johannes Stemick,        *
*               IGPM RWTH Aachen                                 *
*  Datum      : Mai 2018                                         *
*****************************************************************/

#pragma once


#include "unit.h"

#include <iostream>

class TextVisualizer : public GraphVisualizer
{
  public:
    void markVertex(VertexT vertex, VertexStatus status) override {
        static const char* TEXT[] = {
            "Unbekannt", "In Queue", "Aktiv", "Fertig", "Ziel"
        };
        
        std::cerr << "VERTEX " << vertex << " - Neuer Status: " << TEXT[static_cast<int>(status)]  << '\n';
    }
    
    void markEdge(EdgeT e, EdgeStatus status) override {
        static const char* TEXT[] = {
            " - Unbekannt", " - Besucht", " - Aktiv", " - Optimal"
        };
        
        std::cerr << "KANTE  (" << e.first << ", " << e.second << ")" << TEXT[static_cast<int>(status)]  << '\n';
    }
    
    void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status) override {
        static const char* TEXT[] = {
            " - Unbekannt", " - In Queue", " - Aktiv", " - Fertig", " - Ziel"
        };
        
        
        std::cerr << "VERTEX " << vertex << TEXT[static_cast<int>(status)]  << ", Vaterknoten ";
        
        if (parent != undefinedVertex) {
            std::cerr << parent;
        } else {
            std::cerr << '?';
        }
        
        std::cerr << ", Kosten " << (cost+estimate) << " = " << cost << "+" << estimate << '\n';
    }
    
    void draw() override {
        // Nothing to draw.
    }
};
