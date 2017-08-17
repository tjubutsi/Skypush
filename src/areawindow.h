#ifndef AREAWINDOW_H
#define AREAWINDOW_H

#include <QRubberBand>

class Skypush;

class AreaWindow : public QWidget
{
    Q_OBJECT

    public:
        AreaWindow(QRect rect, Skypush *parent = nullptr);

        //external
        Skypush *skypush;

    public slots:
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);

    private:
        QPoint origin;
        QRubberBand *rubberBand;
};

#endif // AREAWINDOW_H
