#include "grid.h"
#include "cell.h"

#include <QDebug>

//Initialize our non-const static values to appease the compiler
float Grid::portion_alive_ = 0.0;
float Grid::cell_width_ = 0.0;
float Grid::cell_height_ = 0.0;

Cell* Grid::grid_[Grid::width_][Grid::height_] = {};

//Constructor
Grid::Grid(float cell_width, float cell_height, QObject *parent) : QObject(parent) {
    cell_width_ = cell_width;
    cell_height_ = cell_height;

    //Initialize grid with new cells
    for (int i = 0; i < width_; i++) {
        for (int j = 0; j < height_; j++) {
            Cell* c = new Cell(i, j, cell_width_, cell_height_);
            grid_[i][j] = c;
        }
    }

    Find_Neighbors(); //Gather the neighbors for each cell

    //Update the quantity of living cells
    Census();
    qDebug() << 100.0 * portion_alive_ << "percent of the cells are alive";
}

//Determine the ratio of living cells to dead ones
void Grid::Census() {

    //Get the number of living cells
    int num_living = 0;

    for (int i = 0; i < width_; i++) {
        for (int j = 0; j < height_; j++) {
            if (grid_[i][j]->Is_Alive()) {
                num_living++;
            }
        }
    }

    //Update portion_alive_
    portion_alive_ = (float) num_living / (float) population_;
}

//Store pointers to all the neighboring cells
void Grid::Find_Neighbors() {

    int last_x = Grid::width_ - 1;
    int last_y = Grid::height_ - 1;

    int cur_x = 0;
    int cur_y = 0;
    Cell* cur_neighbors[8];

    for (int i = 0; i < Grid::width_; i++) {
        for (int j = 0; j < Grid::height_; j++) {
            Cell* c = Grid::grid_[i][j]; //Grab the current cell

            //TOP ROW
            //Top-Left neighbor
            cur_x = (i - 1 < 0 ? last_x : i - 1);
            cur_y = (j - 1 < 0 ? last_y : j - 1);
            cur_neighbors[0] = Grid::grid_[cur_x][cur_y];

            //Top-Center neighbor
            cur_y = (j - 1 < 0 ? last_y : j - 1);
            cur_neighbors[1] = Grid::grid_[i][cur_y];

            //Top-Right neighbor
            cur_x = (i + 1 > last_x ? 0 : i + 1);
            cur_y = (j - 1 < 0 ? last_y : j - 1);
            cur_neighbors[2] = Grid::grid_[cur_x][cur_y];

            //MIDDLE ROW
            //Middle-Left neighbor
            cur_x = (i - 1 < 0 ? last_x : i - 1);
            cur_neighbors[3] = Grid::grid_[cur_x][j];

            //Middle-Right neighbor
            cur_x = (i + 1 > last_x ? 0 : i + 1);
            cur_neighbors[4] = Grid::grid_[cur_x][j];

            //BOTTOM ROW
            //Bottom-Left neighbor
            cur_x = (i - 1 < 0 ? last_x : i - 1);
            cur_y = (j + 1 > last_y ? 0 : j + 1);
            cur_neighbors[5] = Grid::grid_[cur_x][cur_y];

            //Bottom-Center neighbor
            cur_y = (j + 1 > last_y ? 0 : j + 1);
            cur_neighbors[6] = Grid::grid_[i][cur_y];

            //Bottom-Right neighbor
            cur_x = (i + 1 > last_x ? 0 : i + 1);
            cur_y = (j + 1 > last_y ? 0 : j + 1);
            cur_neighbors[7] = Grid::grid_[cur_x][cur_y];

            c->Set_Neighbors(cur_neighbors); //Pass these neighbors to the current cell
        }
    }
}

//Determine the next state of each cell
void Grid::Determine_Next_Turn_Status() {

    //Find out whether each cell will live or die
    for (int i = 0; i < Grid::width_; i++) {
        for (int j = 0; j < Grid::height_; j++) {
            Grid::grid_[i][j]->Determine_Next_State();
        }
    }
}

//Change the grid to its next state
void Grid::Update() {
    Determine_Next_Turn_Status();

    for (int i = 0; i < Grid::width_; i++) {
        for (int j = 0; j < Grid::height_; j++) {
            //Set each cell to its value for the next turn
            Grid::grid_[i][j]->Progress();
            Grid::grid_[i][j]->Refresh();
        }
    }
    Census(); //Update the ratio of living cells
}
