#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widget->setInteraction(QCP::iRangeDrag, true);
    ui->widget->setInteraction(QCP::iRangeZoom, true);
    ui->widget->addGraph();
    ui->widget->addGraph();
    ui->widget->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);

    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::statistic);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::make_graph);
    connect(ui->widget, &QCustomPlot::mousePress, this, &MainWindow::clicked_graph);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::clear_points);
}

void MainWindow::add_point(double x, double y)
{
    px.append(x);
    py.append(y);
}

void MainWindow::clear_points()
{
    px.clear();
    py.clear();
    gx.clear();
    gy.clear();
    plot();
    ui->label->setText(" ");
}

void MainWindow::plot()
{
    ui->widget->graph(0)->setData(px, py);
    ui->widget->graph(1)->setData(gx, gy);
    ui->widget->replot();
    ui->widget->update();
}

void MainWindow::clicked_graph(QMouseEvent* event)
{
    if(event->button() == Qt::RightButton)
    {
        QPoint p = event->pos();
        add_point(ui->widget->xAxis->pixelToCoord(p.x()), ui->widget->yAxis->pixelToCoord(p.y()));
        plot();
    }
}

void MainWindow::get_param()
{
    mutation = ui->doubleSpinBox->value();
    population = ui->spinBox->value();
    generation = ui->spinBox_2->value();
}

void MainWindow::make_graph()
{
    if(px.size() && py.size())
    {
        gx.clear();
        gy.clear();
        ax.clear();
        ay.clear();
        plot();
        get_param();
        std::vector<double> X = px.toStdVector();
        std::vector<double> Y = py.toStdVector();
        std::ofstream f("points.txt");
        for(int i = 0; i < X.size(); i++)
        {
            f<<X[i]<<" "<<Y[i]<<"\n";
        }
        f.close();
        QProcess* p = new QProcess;
        QStringList s;
        s.append(QString(std::to_string(population).c_str()));
        s.append(QString(std::to_string(generation).c_str()));
        s.append(QString(std::to_string(mutation).c_str()));
        s.append(QString(std::to_string(*std::min_element(px.constBegin(), px.constEnd()) - 10).c_str()));
        s.append(QString(std::to_string(*std::max_element(px.constBegin(), px.constEnd()) + 10).c_str()));
        p->start("C:\\Users\\Professional\\Desktop\\OOOP\\Project\\project\\symbolregres.exe", s);
        p->waitForFinished(-1);
        delete p;
        std::ifstream ff("function_points.txt");
        std::string sssx, sssy;
        while(ff >> sssx && ff >> sssy)
        {
            gx.push_back(atof(sssx.c_str()));
            gy.push_back(atof(sssy.c_str()));
        }
        ff.close();
        std::ifstream fff("function.txt");
        std::getline(fff, sssx);
        std::getline(fff, sssx);
        ui->label->setText("F(x) = " + QString(sssx.c_str()));
        fff.close();
        std::ifstream ffff("information.txt");
        while(ffff >> sssx && ffff >> sssy)
        {
            ax.push_back(atof(sssx.c_str()));
            ay.push_back(1/atof(sssy.c_str()));
        }
        ffff.close();
        plot();
    }
}

void MainWindow::statistic()
{
    stat = new QWidget();
    ppp = new QCustomPlot(stat);
    QHBoxLayout* h = new QHBoxLayout(stat);
    stat->setMinimumSize(1000, 800);
    ppp->setInteraction(QCP::iRangeDrag, true);
    ppp->setInteraction(QCP::iRangeZoom, true);
    ppp->addGraph();
    ppp->graph()->setData(ax, ay);
    ppp->graph()->setScatterStyle(QCPScatterStyle::ssCircle);
    ppp->graph()->setLineStyle(QCPGraph::lsNone);
    ppp->replot();
    ppp->update();
    h->addWidget(ppp);
    stat->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

