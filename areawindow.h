#ifndef AREAWINDOW_H
#define AREAWINDOW_H

#include <QDialog>
#include <QRubberBand>

class Skypush;

class AreaWindow : public QWidget
{
    Q_OBJECT

    public:
        AreaWindow(QRect rect, Skypush *parent = nullptr);
        Skypush *skypush;
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);
        QRubberBand *rubberBand {rubberBand = NULL};
        QPoint origin;
};

#endif // AREAWINDOW_H
