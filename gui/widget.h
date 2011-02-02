#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QThread>
#include "Reconstructor.h"
#include "ReconstructorListener.h"

namespace Ui {
    class Widget;
}

class ReconstructThread : public QThread {
public:
    void setReconstructor(Reconstructor* r) {
        m_r = r;
    }

    virtual void run() {
        m_r->reconstruct();
    }
private:
    Reconstructor* m_r;
};

class Widget : public QWidget, ReconstructorListener {
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();


    void blockCompleted(int block, const Matrix &R, const Matrix &G, const Matrix &B);
    void finished();

public slots:
    void updateImage();

protected:
    void changeEvent(QEvent *e);

protected slots:

    void on_loadPushButton_clicked();
    void on_transformCombo_currentIndexChanged(int);
    void on_blockCombo_currentIndexChanged(int);
    void on_sigmaLambdaSB_valueChanged(double);
    void on_rgbRadio_toggled(bool);
    void on_threadsSB_valueChanged(int);
    void on_startButton_clicked();
    void on_stopButton_clicked();

private:
    Ui::Widget *ui;
    Reconstructor m_r;
    QPixmap m_pixmap;
    QPixmap m_orig;
    QImage  m_image;
    QTimer m_timer;
    bool m_imageChanged;
    ReconstructThread m_rt;
    int m_nblocks;
    QTime m_time;

    void getFileName(std::string& path, QString& nom, const QString& ext);

    std::string m_file;
    int m_transform;
    int m_blockSize;
    double m_sigmaLambda;
    bool m_colorSpace;
    int m_nthreads;

};

inline void Widget::on_transformCombo_currentIndexChanged(int i) {
    m_transform = i;
}

inline void Widget::on_blockCombo_currentIndexChanged(int i) {
    m_blockSize = i;
}

inline void Widget::on_sigmaLambdaSB_valueChanged(double d) {
    m_sigmaLambda = d;
}

inline void Widget::on_rgbRadio_toggled(bool b) {
    m_colorSpace = b;
}

inline void Widget::on_threadsSB_valueChanged(int nt) {
    m_nthreads = nt;
}



#endif // WIDGET_H
