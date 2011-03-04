#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->stopLabel->setVisible(false);
    m_r.setReconstructorListener(this);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateImage()));
    m_imageChanged = false;
    m_rt.setReconstructor(&m_r);

    m_file = "";
    m_transform = 2;
    m_blockSize = 0;
    m_sigmaLambda  = 0.01;
    m_colorSpace = true;
    m_nthreads = 4;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Widget::on_loadPushButton_clicked() {
    std::string path;
    QString nom;
    getFileName(path, nom, "Images (*.ppm)");
    ui->nameLineEdit->setText(nom);

    m_file = path;
    m_orig = QPixmap(path.c_str());
    m_pixmap = m_orig;
    m_image  = m_pixmap.toImage();
    ui->imageLabel->setPixmap(m_pixmap);
    ui->widthLabel->setText(QString::number(m_pixmap.width()));
    ui->heightLabel->setText(QString::number(m_pixmap.height()));

}

void Widget::blockCompleted(int block, const Matrix &R, const Matrix &G, const Matrix &B) {
    int colsBlocks = m_image.width() / m_r.blockSize;

    int r = (block / colsBlocks) * m_r.blockSize;
    int c = (block % colsBlocks) * m_r.blockSize;

    for (int i = 0; i < m_r.blockSize; i++) {
        for (int j = 0; j < m_r.blockSize; j++) {
            unsigned int ir, ig, ib;
            if (m_r.yuv) {
                double Y = R(i, j);
                double V = G(i, j);
                double U = B(i, j);

                ib = 1.164*(Y - 16) + 2.018*(U - 128);
                ig = 1.164*(Y - 16) - 0.813*(V - 128) - 0.391*(U - 128);
                ir = 1.164*(Y - 16) + 1.596*(V - 128);

            }
            else {
                ir = R(i, j);
                ig = G(i, j);
                ib = B(i, j);
            }
            uint rgb = (ir<<16) | (ig<<8) | (ib);
            m_image.setPixel(c+j, r+i, rgb);
        }
    }
    m_imageChanged = true;

    m_nblocks++;
}

void Widget::updateImage() {
    int second  = m_time.elapsed()/1000;
    int hour = second / 3600;
    second %= 3600;
    int minute = second / 60;
    second %= 60;

    QString h = QString::number(hour);
    QString m = QString::number(minute);
    QString s = QString::number(second);

    QString time;
    time += h;
    time += ":";
    if (m.length() < 2) time += "0";
    time += m;
    time += ":";
    if (s.length() < 2) time += "0";
    time += s;

    ui->timeLabel->setText(time);

    if (m_imageChanged) {
        m_pixmap = QPixmap::fromImage(m_image);
        ui->imageLabel->setPixmap(m_pixmap);
        m_imageChanged = false;

        int totalblocks = (m_pixmap.width() / m_r.blockSize) * (m_pixmap.height() / m_r.blockSize);
        int progress = m_nblocks * 100 / totalblocks;
        ui->progressBar->setValue(progress);

    }
}



void Widget::getFileName(std::string& path, QString& nom, const QString& ext)
{
       QString file =
               QFileDialog::getOpenFileName(
                               this,
                               tr("Load Image"),
                               "./img",
                               ext);

       if (!file.isNull()) {
               QByteArray ba = file.toAscii();
               const char* filename = ba.constData();
               path = std::string(filename);

               int slash = file.lastIndexOf('/')+1;
               nom = file.mid(slash);
       }
       else {
           path = "";
           nom = QString("");
       }
}

void Widget::on_startButton_clicked() {
    if (m_rt.isRunning()) return;
    m_image = m_orig.toImage();
    ui->imageLabel->setPixmap(m_orig);
    ui->progressBar->setValue(0);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    m_nblocks = 0;

    m_r.setFile(m_file);
    m_r.setTransform(m_transform);
    switch(m_blockSize) {
        case 0:
            m_r.setBlockSize(16);
        break;
        case 1:
            m_r.setBlockSize(32);
        break;
    }
//    m_r.setSigmaLambda(m_sigmaLambda);
    m_r.setColorSpace(!m_colorSpace);
    m_r.setNThreads(m_nthreads);

    m_time.start();
    m_rt.start();

    m_timer.setSingleShot(false);
    m_timer.start(500);
}

void Widget::on_stopButton_clicked() {
    ui->stopLabel->setVisible(true);
    m_r.stop();
}

void Widget::finished() {
    std::cerr << "RECONSTRUCTOR STOPED" << std::endl;
    m_timer.stop();
    m_timer.setSingleShot(true);
    m_timer.start(0);
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->stopLabel->setVisible(false);
}

