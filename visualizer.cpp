#include "visualizer.h"

// Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
void CoordVisualizer::markVertex(VertexT vertex, VertexStatus status)
{

}

// Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
void CoordVisualizer::markEdge(EdgeT e, EdgeStatus status)
{

}

// Aktualisiere jegliche Daten eines Knotens.
void CoordVisualizer::updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status)
{

}

// Zeichne den aktuellen Zustand des Graphen.
void CoordVisualizer::draw() {
    // run the program as long as the window is open
    while (w->isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (w->pollEvent(event)) // event loop
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                w->close();
        }

        sf::CircleShape shape(100);
        shape.setFillColor(sf::Color(250, 100, 180));
        shape.setOrigin(shape.getRadius(), shape.getRadius());
        shape.setPosition(400,300);
        w->draw(shape);

        sf::CircleShape shape2(50);
        shape2.setFillColor(sf::Color(180, 100, 180));
        shape2.setOrigin(shape2.getRadius(), shape2.getRadius());
        shape2.setPosition(100,100);
        w->draw(shape2);


        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(shape.getPosition())),
            sf::Vertex(sf::Vector2f(shape2.getPosition()))
        };
        w->draw(line, 2, sf::Lines);

        w->display();
    }
}

void CoordVisualizer::setWindow(sf::RenderWindow& window)
{
    w = &window;
}





// Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
void markVertex(VertexT vertex, VertexStatus status) ;

// Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
void markEdge(EdgeT e, EdgeStatus status) ;

// Aktualisiere jegliche Daten eines Knotens.
void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status) ;

// Zeichne den aktuellen Zustand des Graphen.
void draw() ;

