#ifndef HISTOGRAM_WIDGET_H
#define HISTOGRAM_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QMutex>
#include "qcustomplot.h"
#include "frame_worker.h"
#include "image_type.h"

class histogram_widget : public QWidget
{
    Q_OBJECT
    QMutex maxMux;
    QVBoxLayout qvbl;
    QCustomPlot * qcp;
    QCPBars *histogram;
    int frHeight;
    int frWidth;
    int fps;
    double dataMax;
    QVector<double> histo_bins;
    QVector<double> histo_data;

    frameWorker * fw;
public:
    explicit histogram_widget(frameWorker * fw,image_t image_type ,QWidget *parent = 0);
    ~histogram_widget();

private:
    void initQCPStuff();
    void update_histo_data();
signals:
    
public slots:
    void handleNewFrame();
    void histogramScrolledX(const QCPRange &newRange);
    void histogramScrolledY(const QCPRange &newRange);

};

#endif // HISTOGRAM_WIDGET_H