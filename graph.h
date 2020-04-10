#ifndef GRAPH_H
#define GRAPH_H

#include "bar.h"

#include <QObject>
#include <QGraphicsScene>

class Graph : public QObject {
    Q_OBJECT

    public:
        explicit Graph(int width, int height, QObject *parent = nullptr);

        const static int numBars_ = 30;
        static float barWidth_;
        static int graphWidth_;
        static int graphHeight_;

        static Bar* bars_ [numBars_];
        static int size;

        void AddBar(float portion_alive);
        void DrawGraph(QGraphicsScene* graph);
};

#endif // GRAPH_H
