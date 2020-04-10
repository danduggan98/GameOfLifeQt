#ifndef GRID_H
#define GRID_H

#include <QObject>
#include "cell.h"

class Grid : public QObject {
    Q_OBJECT

    public:
        const static int width_ = 30;
        const static int height_ = 15;
        const static int population_ = width_ * height_;

        static float portion_alive_;
        static float cell_width_;
        static float cell_height_;

        static Cell* grid_ [width_][height_];

        explicit Grid(float cell_width, float cell_height, QObject *parent = nullptr);
        void Find_Neighbors();
        void Determine_Next_Turn_Status();
        void Update();
        void Census();
};

#endif // GRID_H
