#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "grid.h"
#include "graph.h"
#include <QTimer>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Grid* grid_ptr; //Pointer to grid created in constructor
        Graph* graph_ptr; //Pointer to graph cretaed in constructor

        QTimer *timer = new QTimer();
        QSlider *slider;
        int timer_interval;
        int turn;

        Ui::MainWindow *ui;
        QGraphicsScene *gridScene;
        QGraphicsScene *graphScene;

        void UpdateLabels();
        void NextTurn();
        void AddGraphBar();

    private slots:
        void CellLeftClickedSlot(Cell* c);
        void CellRightClickedSlot(Cell* c);
        void StepButtonClickedSlot();
        void PlayButtonClickedSlot();
        void PauseButtonClickedSlot();
        void TimerEnded(); //Go to next turn once the timer is over
        void UpdateInterval();
};
#endif // MAINWINDOW_H
