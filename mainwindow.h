#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QVector>
#include <QEvent>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void add_point(double x, double y);
    void clear_points();
    void plot();
    void get_param();
    void make_graph();
    void statistic();
    void clicked_graph(QMouseEvent* event);
    ~MainWindow();

private:
    QVector<double> px, py;
    QVector<double> gx, gy;
    QVector<double> ax, ay;
    int generation, population;
    float mutation;
    QWidget* stat;
    QCustomPlot* ppp;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
