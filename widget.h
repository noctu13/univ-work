#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include "edge.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    int G_WIDTH,
        G_HEIGHT,
        G_XCENTER,
        G_YCENTER,
        G_RADIUS,
        G_POINT_RADIUS,
        G_POINTS_COUNTER,
        G_SLOTS_COUNTER;
    std::vector<Point> PTS, RPTS, SLOT_RPTS, SAVE_PTS;
    std::vector<double> GMAX;
    double G_MAX;
    Point GPT;

    QLineEdit* g_edit1;
    QLineEdit* g_edit2;
    QLineEdit* g_edit3;
    QLineEdit* g_edit4;
    QLabel* g_label1;
    QLabel* g_label2;
    QLabel* g_label3;
    QLabel* g_label4;
    QLabel* g_label5;
    Widget(QWidget *parent = 0);
    void init(int width, int height);
    bool event(QEvent* event);
    double recalc_max(void);
public slots:
    void pushClicked();
    void push2Clicked();
    void push3Clicked();
};

#endif // WIDGET_H
