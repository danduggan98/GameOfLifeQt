#include "graph.h"

#include <QDebug>

//Initialize our non-const static values to appease the compiler
float Graph::barWidth_ = 0.0;
int Graph::graphWidth_ = 0;
int Graph::graphHeight_ = 0;

Bar* Graph::bars_[Graph::numBars_] = {};
int Graph::size = 0;

//Constructor
Graph::Graph(int width, int height, QObject *parent)
    : QObject(parent)
{
    graphWidth_ = width;
    graphHeight_ = height;
    barWidth_ = (float) graphWidth_ / (float) numBars_;
}

//Add a new bar to our array
void Graph::AddBar(float portion_alive) {

    //Create and add the new bar
    Bar *b = new Bar(barWidth_, ((float) graphHeight_ * portion_alive));

    //If the graph is not yet full, add the bar
    if (Graph::size < numBars_) {
        Graph::bars_[Graph::size] = b;
        Graph::size++;
    }

    //If the graph is already full, shift everything down, then add the bar
    else {
        for (int i = 0; i < Graph::size - 1; i++) {
            bars_[i] = bars_[i+1];
        }
        Graph::bars_[Graph::size - 1] = b;
    }
}

//Draw the contents of the graph
void Graph::DrawGraph(QGraphicsScene *graph) {

    graph->clear(); //Erase the current graph

    //Draw every bar in the array
    for (int i = 0; i < Graph::size; i++) {
        graph->addRect(i * barWidth_, 0, bars_[i]->get_width(), bars_[i]->get_height());
    }
}
