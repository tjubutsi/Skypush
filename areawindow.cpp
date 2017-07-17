#include <QtWidgets>
#include <QRubberBand>
#include <QRect>
#include <QPalette>
#include "areawindow.h"
#include "skypush.h"

AreaWindow::AreaWindow(QRect rect, Skypush *parent)
{
    skypush = parent;
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow | Qt::X11BypassWindowManagerHint);
    setParent(0);
    setProperty("windowOpacity", 0.5);
    showNormal();
    raise();
    activateWindow();
    move(rect.x(), rect.y());
    resize(rect.width(), rect.height());
    setCursor(Qt::CrossCursor);
}

void AreaWindow::mousePressEvent(QMouseEvent *event)
{
    origin = event->pos();
    if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();
}

void AreaWindow::mouseMoveEvent(QMouseEvent *event)
{
    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
}

void AreaWindow::mouseReleaseEvent(QMouseEvent *event)
{
    rubberBand->hide();
    this->hide();
    QRect rect = QRect(rubberBand->x(), rubberBand->y(), rubberBand->width(), rubberBand->height());
    #ifdef Q_OS_WIN
        //no sleep needed
    #else
        struct timespec ts = { 200 / 1000, (200 % 1000) * 1000 * 1000 };
        nanosleep(&ts, NULL);
    #endif
    skypush->grabAreaAndUpload(rect);
    this->close();
    this->deleteLater();
}

void AreaWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
        this->deleteLater();
    }
}
