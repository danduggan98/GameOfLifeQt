#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "grid.h"
#include "graph.h"

#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtMath>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set random seed
    qsrand(static_cast<unsigned>(QTime::currentTime().msec()));

    //Create a scene to render our grid
    QGraphicsView *graphicsGrid = ui->grid;
    gridScene = new QGraphicsScene;
    graphicsGrid->setScene(gridScene);

    //Create a scene to render our graph
    QGraphicsView *graphBox = ui->graph;
    graphScene = new QGraphicsScene;
    graphBox->setScene(graphScene);
    graphBox->scale(1, -1); //Flip it so (0,0) is the bottom left corner

    //Calculate constants for the width+height of our grid, graph, and cells
    const int gridSceneWidth = graphicsGrid->frameSize().width();
    const int gridSceneHeight = graphicsGrid->frameSize().height();

    const int graphWidth = graphBox->frameSize().width();
    const int graphHeight = graphBox->frameSize().height();

    const float cellWidth = (float) gridSceneWidth / (float) Grid::width_;
    const float cellHeight = (float) gridSceneHeight / (float) Grid::height_;

    //Adjust the graph to fit in the right space
    graphBox->setSceneRect(Graph::barWidth_ + 1, 0, graphWidth, graphHeight);

    //Create a graph object
    Graph graph(graphWidth, graphHeight);
    this->graph_ptr = &graph;

    //Create a grid object
    Grid grid(cellWidth, cellHeight);
    this->grid_ptr = &grid;

    //Visually create the grid by filling our scene with cells
    for (int i = 0; i < Grid::width_; i++) {
        for (int j = 0; j < Grid::height_; j++) {

            //Add the cell to the scene
            Cell* c = Grid::grid_[i][j];
            gridScene->addItem(c);

            //Connect the cell to its slots
            connect(c, &Cell::CellLeftClicked, this, &MainWindow::CellLeftClickedSlot);
            connect(c, &Cell::CellRightClicked, this, &MainWindow::CellRightClickedSlot);
        }
    }

    //Begin counting turns
    turn = 0;
    UpdateLabels();

    //Setup the speed slider and timer
    slider = ui->speedSlider;
    connect(slider, &QAbstractSlider::valueChanged, this, &MainWindow::UpdateInterval);

    UpdateInterval();
    connect(timer, &QTimer::timeout, this, &MainWindow::TimerEnded);

    //Connect the buttons to their slots
    connect(ui->stepButton, &QAbstractButton::pressed, this, &MainWindow::StepButtonClickedSlot);
    connect(ui->playButton, &QAbstractButton::pressed, this, &MainWindow::PlayButtonClickedSlot);
    connect(ui->pauseButton, &QAbstractButton::pressed, this, &MainWindow::PauseButtonClickedSlot);
}

//Change the turn and population labels to reflect the current state
void MainWindow::UpdateLabels() {
    grid_ptr->Census();

    //Determine the population size and the percentage alive, rounded to one decimal place
    QString num_alive = QString::number((float) Grid::population_ * Grid::portion_alive_);
    float percent_alive = 100.0 * Grid::portion_alive_;
    QString percent_rounded = " (" + QString::number((int)(10.0 * percent_alive) / 10.0) + "% alive)";

    //Update the labels
    ui->turnValue->setText(QString::number(turn));
    ui->populationValue->setText(" " + num_alive + percent_rounded);
}

//Activates when a cell is left clicked
void MainWindow::CellLeftClickedSlot(Cell* c) {
    //Revive the cell if it was dead
    if (!c->Is_Alive()) {
        c->Set_Live(true);
    }
    UpdateLabels();
}

//Activates when a cell is right clicked
void MainWindow::CellRightClickedSlot(Cell* c) {
    //Kill the cell if it was alive
    if (c->Is_Alive()) {
        c->Set_Live(false);
    }
    UpdateLabels();
}

//Update the grid when the 'step' button is selected
void MainWindow::StepButtonClickedSlot() {
    //Proceed as long as the game is not currently playing
    if (!timer->isActive()) {
        qDebug() << "Stepping to next turn" << endl;
        NextTurn();
    }
    else {
        qDebug() << "Can not step until game is paused" << endl;
    }
}

//Begin the game when the 'play' button is clicked
void MainWindow::PlayButtonClickedSlot() {
    qDebug() << "Pressed play";
    if (!timer->isActive()) {
        qDebug() << "Resuming game" << endl;
        timer->start();
    }
    else {
        qDebug() << "Already playing" << endl;
    }
}

//Pause the game when the 'pause' button is clicked
void MainWindow::PauseButtonClickedSlot() {
    qDebug() << "Pressed pause";
    if (timer->isActive()) {
        qDebug() << "Pausing game" << endl;
        timer->stop();
    }
    else {
        qDebug() << "Already paused" << endl;
    }
}

//Go to the next turn, adding a new bar and updating the state/labels
void MainWindow::NextTurn() {
    grid_ptr->Update();
    turn++;
    AddGraphBar();

    UpdateLabels();
    update();
}

//When the timer interval ends, go to the next turn and restart the timer
void MainWindow::TimerEnded() {
    NextTurn();
    timer->start();
}

//Update the timer duration and change the label to reflect this
void MainWindow::UpdateInterval() {
    int val = slider->value() + 1; //Add one to make it 1-100(%), not 0-99

    //Change the time between generations based on the slider input
    timer_interval = (10 * (100-val)) + 100; //Between 1.1s and 0.1s
    timer->setInterval(timer_interval);

    ui->speedValue->setText(QString::number(val) + "%"); //Update the label
}

//Add a bar to the graph
void MainWindow::AddGraphBar() {
    graph_ptr->AddBar(grid_ptr->portion_alive_);
    graph_ptr->DrawGraph(graphScene);
}

//Deconstructor
MainWindow::~MainWindow() {
    delete ui;
}
