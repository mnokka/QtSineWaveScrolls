#include "mainwindow.h"


#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QTimer>
#include <random>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCore/QTimer>
#include <QtMath>
#include <random>
#include <QPen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    series(new QLineSeries()),
    axisX(new QValueAxis()),
    axisY(new QValueAxis()),
    phase(0),
    gen(rd()),
    dis(0.5, 2.5)
{
    // Luo kaavio ja lisää siniaaltosarja siihen
    //QChart *chart = new QChart();

     chart = new QChart();

    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Liikkuva siniaalto satunnaisella amplitudilla MPN 2025");
    chart->setAnimationOptions(QChart::SeriesAnimations | QChart::GridAxisAnimations); // pehmeä aplitudin muutospiirto

    chart->setTheme(QChart::ChartThemeDark);
    QPen pen;
    pen.setWidth(5); // Aseta viivan paksuudeksi 2 pikseliä
    pen.setColor(QColor(34, 139, 34)); //vihreä
    series->setPen(pen);



    // Aseta X- ja Y-akselit
    axisX->setRange(0, dataSize);
    axisX->setLabelFormat("%d");
    axisX->setTitleText("Aika");

    axisY->setRange(-2, 2);
    axisY->setTitleText("Arvo");

    // Lisää akselit kaavioon
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);


    // Luo kaavionäkymä ja aseta se pääikkunan keskuswidgetiksi
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(chartView);
    resize(800, 600);

    // Alusta siniaalto
    for (int i = 0; i < dataSize; ++i) {
        qreal amplitude = dis(gen);
        qreal y = amplitude * qSin(qDegreesToRadians(static_cast<qreal>(i) + phase));
        series->append(i, y);
    }

    connect(&timer, &QTimer::timeout, this, &MainWindow::updateChart);
    timer.start(20); // Päivitysnopeus (ms)
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateChart()
{
    // Poista ensimmäinen piste
    series->remove(0);

    // Päivitä jäljellä olevien pisteiden x-arvoja
    for (int i = 0; i < series->count(); ++i) {
        QPointF point = series->at(i);
        series->replace(i, QPointF(point.x() - 1, point.y()));
    }

    // Lisää uusi piste loppuun
    qreal amplitude = dis(gen);
    qreal y = amplitude * qSin(qDegreesToRadians(static_cast<qreal>(dataSize) + phase));
    series->append(dataSize, y);

    phase += 1; // Vaiheen muutos
}
