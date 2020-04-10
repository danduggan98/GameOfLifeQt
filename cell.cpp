#include "cell.h"

#include <QDebug>
#include <QtWidgets>

//Constructor
Cell::Cell(int x_pos, int y_pos, float width, float height) {
    x_pos_ = x_pos;
    y_pos_ = y_pos;
    width_ = width;
    height_ = height;

    live_ = (qrand() % 100 < 50? true : false); //50% random chance to live
    next_turn_status_ = live_;
    Change_Color();
}

//Get pointers to all cells next to this one
void Cell::Set_Neighbors(Cell* n[]) {
    for (int i = 0; i < 8; i++) {
        neighbors_[i] = n[i];
    }
}

//Figure out how many neighbor cells are alive
int Cell::Num_Live_Neighbors() {
    int num_living = 0;
    for (int i = 0; i < 8; i++) {
        if (neighbors_[i]->Is_Alive()) {
            num_living++;
        }
    }
    return num_living;
}

//Apply Conways' rule to see whether this cell will be alive (true) or dead (false) next turn
void Cell::Determine_Next_State() {
    int living_neighbors = Num_Live_Neighbors();

    //Reproduction rule
    if (!live_ && living_neighbors == 3) {
        next_turn_status_ = true;
    }
    else if (live_) {
        //  v Underpopulation v      v Overpopulation v
        if (living_neighbors < 2 || living_neighbors > 3) {
            next_turn_status_ = false;
        }
        else {
            //Stable otherwise
            next_turn_status_ = true;
        }
    }
}

//Update the color
void Cell::Change_Color() {
    color_ = (live_ ? QColor(0,255,0) : QColor(255,255,255)); //Green if alive, white if dead
}

//Physical boundaries of the cell
QRectF Cell::boundingRect() const {
    return QRectF(x_pos_ * this->width_, y_pos_ * this->height_, width_, height_);
}

//Coordinates to draw the cell
QPainterPath Cell::shape() const {
    QPainterPath path;
    path.addRect(x_pos_ * this->width_, y_pos_ * this->height_, width_, height_);
    return path;
}

//Actually draw the cell
void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));
    painter->drawRect(QRect(this->x_pos_ * this->width_, this->y_pos_ * this->height_, this->width_, this->height_));
    painter->setBrush(b);
}

//Respond when the cell is clicked
void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    //Left Click
    if (event->button() == Qt::LeftButton) {
        emit CellLeftClicked(this);
    }
    //Right click
    else if (event->button() == Qt::RightButton) {
        emit CellRightClicked(this);
    }
    Refresh();
}

//Update the color and load the change
void Cell::Refresh() {
    Change_Color();
    update();
}
