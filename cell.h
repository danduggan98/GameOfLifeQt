#ifndef CELL_H
#define CELL_H

#include <QColor>
#include <QGraphicsItem>

class Cell : public QObject, public QGraphicsItem {

    Q_OBJECT

    private:
        int x_pos_;
        int y_pos_;
        float width_;
        float height_;

        bool live_;
        bool next_turn_status_; //Whether it lives or dies next turn
        QColor color_;
        Cell* neighbors_ [8]; //All neighboring cells

    public:
        Cell(int x_pos, int y_pos, float width, float height);
        int get_x() const { return x_pos_; };
        int get_y() const { return y_pos_; };
        bool Is_Alive() { return (live_ ? true : false); };

        void Set_Live(bool newLive) { live_ = newLive; };
        void Set_Neighbors(Cell* n[]);
        int Num_Live_Neighbors();

        void Determine_Next_State();
        void Progress() {live_ = next_turn_status_; };
        void Change_Color();
        void Refresh();

        QRectF boundingRect() const override;
        QPainterPath shape() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    signals:
        void CellLeftClicked(Cell *c);
        void CellRightClicked(Cell *c);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // CELL_H
