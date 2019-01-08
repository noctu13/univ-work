#include <QApplication>
#include <QDesktopWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QKeyEvent>
#include "widget.h"
#include "combine.h"

Widget::Widget(QWidget *parent):QWidget(parent)
{
    init(800, 600);
    QVBoxLayout* vlayout = new QVBoxLayout;  
    QHBoxLayout* hlayout = new QHBoxLayout;
    QLabel* label = new QLabel(tr("Введите точку (x, y)"));
    hlayout->addWidget(label);
    QPushButton* push = new QPushButton(tr("Ввод"),this);
    push->setFixedWidth(56);
    connect(push,SIGNAL(clicked()),this,SLOT(pushClicked()));
    hlayout->addWidget(push);
    hlayout->addStretch();
    vlayout->addLayout(hlayout);
    g_edit1 = new QLineEdit(this);
    g_edit1->setFixedWidth(200);
    vlayout->addWidget(g_edit1);
    g_edit2 = new QLineEdit(this);
    g_edit2->setFixedWidth(200);
    vlayout->addWidget(g_edit2);
    QLabel* label0 = new QLabel(tr("Координаты мыши:"));
    vlayout->addWidget(label0);
    g_label1 = new QLabel(tr("X: %1"));
    vlayout->addWidget(g_label1);
    g_label2 = new QLabel(tr("Y: %1"));
    vlayout->addWidget(g_label2);
    QLabel* labelo = new QLabel(tr("Максимальный радиус:"));
    vlayout->addWidget(labelo);
    g_label3 = new QLabel(tr("ожидание ввода..."));
    vlayout->addWidget(g_label3);
    QLabel* labelc = new QLabel(tr("Число точек:"));
    vlayout->addWidget(labelc);
    g_label4 = new QLabel("0");
    vlayout->addWidget(g_label4);
    QLabel* labeln = new QLabel(tr("Введите точность:"));
    vlayout->addWidget(labeln);

    QHBoxLayout* h2layout = new QHBoxLayout;
    g_edit3 = new QLineEdit(this);
    g_edit3->setFixedWidth(50);
    h2layout->addWidget(g_edit3);
    QPushButton* push3 = new QPushButton(tr("Точки!"),this);
    push3->setFixedWidth(130);
    connect(push3,SIGNAL(clicked()),this,SLOT(push3Clicked()));
    h2layout->addWidget(push3);
    h2layout->addStretch();
    vlayout->addLayout(h2layout);

    QLabel* labelt = new QLabel(tr("Число точек на сетке:"));
    vlayout->addWidget(labelt);
    g_label5 = new QLabel("0");
    vlayout->addWidget(g_label5);
    QLabel* labelm = new QLabel(tr("Введите количество точек:"));
    vlayout->addWidget(labelm);
    g_edit4 = new QLineEdit(this);
    g_edit4->setFixedWidth(190);
    vlayout->addWidget(g_edit4);
    QPushButton* push2 = new QPushButton(tr("Расчет!"),this);
    push2->setFixedWidth(190);
    connect(push2,SIGNAL(clicked()),this,SLOT(push2Clicked()));
    vlayout->addWidget(push2);

    vlayout->setAlignment(Qt::AlignTop);
    setLayout(vlayout);
    show();
    startTimer(100);
}

void Widget::init(int width, int height)
{
    G_WIDTH = width;
    G_HEIGHT = height;
    G_XCENTER = width/2+100;
    G_YCENTER = height/2;
    G_RADIUS = G_YCENTER-5;
    G_POINT_RADIUS = 2;
    G_POINTS_COUNTER = 0;
    G_SLOTS_COUNTER = 0;
    G_MAX = 0;
    QDesktopWidget* desktop = QApplication::desktop();
    setGeometry(
        (desktop->width()-width)/2,
        (desktop->height()-height)/2,
        width,
        height
    );
}

void Widget::pushClicked()
{
    if(g_edit1->text() != "" && g_edit2->text() != "")
    {
        Point tmp(g_edit1->text().toDouble(),g_edit2->text().toDouble());
        if(tmp.distance(Point())<1)
        {
            RPTS.push_back(tmp);
            PTS.push_back(tmp.correction(EXP,G_XCENTER, G_YCENTER, G_RADIUS));
            G_POINTS_COUNTER++;
            G_MAX = recalc_max();
            repaint();
        }
    }
}

void Widget::push3Clicked()
{
    PTS.clear();
    G_SLOTS_COUNTER = 0;
    int n = g_edit3->text().toInt();//убрать в глобал
    double t=1.0/(n+1), sqrt3=1.732050807568877;
    int m = 1.0/(sqrt3*t)+1;
    for(int i=0;i<=2*(n+1);i++)
    {
        if(i%2==0)
        {
            Point p(-1+i*t,0);
            if (p.distance(Point())+t<1)
            {
                SLOT_RPTS.push_back(p);
                G_SLOTS_COUNTER++;
                //PTS.push_back(p.correction(EXP,G_XCENTER, G_YCENTER, G_RADIUS));
                //G_POINTS_COUNTER++;
            }
        }
        for(int l=-1;l<2;l+=2)
            for(int j=1;j<m;j+=2)
            {
                if(i%2==0)
                {
                    Point p(-1+i*t,(j+1)*t*sqrt3*l);
                    if (p.distance(Point())+t<1)
                    {
                        SLOT_RPTS.push_back(p);
                        G_SLOTS_COUNTER++;
                    }
                }else
                {
                    Point p(-1+i*t,j*t*sqrt3*l);
                    if (p.distance(Point())+t<1)
                    {
                        SLOT_RPTS.push_back(p);
                        G_SLOTS_COUNTER++;
                    }
                }
            }
    }
    repaint();
}

void Widget::push2Clicked()
{
    //Построение равномерной гексагональной сетки
    push3Clicked();
    //главный цикл программы!!!
    int k = g_edit4->text().toInt();
    Combine A(G_SLOTS_COUNTER,k);
    int GEN_MSG = A.init();
    if (GEN_MSG!=GEN_ERRO || GEN_MSG!=GEN_VOID)
        while(GEN_MSG == GEN_NEXT)
         {
            RPTS.clear();
            G_POINTS_COUNTER = 0;
            for(int i=0; i<k; i++)
            {
                RPTS.push_back(SLOT_RPTS[A.index[i]]);
                G_POINTS_COUNTER++;
            }
            GMAX.push_back(recalc_max());
            SAVE_PTS.push_back(GPT);
            GEN_MSG = A.next();
         }
    if(A.counter>0)
        G_MAX = GMAX[0];
    for(int i=1;i<A.counter;i++)
        if(GMAX[i]<G_MAX)
        {
            G_MAX = GMAX[i];
            GPT = SAVE_PTS[i];
        }
    RPTS.clear();
    G_POINTS_COUNTER = 0;
    SLOT_RPTS.clear();
    GMAX.clear();
    SAVE_PTS.clear();
    repaint();
}

bool Widget::event(QEvent* event)
{
    if (event->type() == QEvent::Timer) {
        Point tmp(QCursor::pos().x()-this->x()-8, QCursor::pos().y()-this->y()-30);
        tmp = tmp.correction(CON, G_XCENTER, G_YCENTER, G_RADIUS);
        g_label1->setText(QString(tr("X: %1")).arg(tmp.X));
        g_label2->setText(QString(tr("Y: %1")).arg(tmp.Y));
        return true;
    }else if(event->type() == QEvent::Paint){//вывод
        QPainter paint(this);
        paint.setRenderHint(QPainter::Antialiasing, true);
        paint.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
        paint.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
        paint.drawEllipse(G_XCENTER-G_RADIUS,G_YCENTER-G_RADIUS,2*G_RADIUS,2*G_RADIUS);
        for(int i = 0; i < G_POINTS_COUNTER; i++)
            paint.drawEllipse(PTS[i].X-G_POINT_RADIUS,PTS[i].Y-G_POINT_RADIUS,2*G_POINT_RADIUS,2*G_POINT_RADIUS);
        if (G_MAX>0)
            paint.drawEllipse(GPT.X-G_MAX*G_RADIUS,GPT.Y-G_MAX*G_RADIUS,2*G_MAX*G_RADIUS,2*G_MAX*G_RADIUS);
        g_label3->setText(QString("%1").arg(G_MAX));
        g_label4->setText(QString("%1").arg(G_POINTS_COUNTER));
        g_label5->setText(QString("%1").arg(G_SLOTS_COUNTER));
        return true;
    }else if(event->type() == QEvent::MouseButtonPress)
    {
        Point tmp(QCursor::pos().x()-this->x()-8, QCursor::pos().y()-this->y()-30),
            ctmp = tmp.correction(CON, G_XCENTER, G_YCENTER, G_RADIUS);
        if(ctmp.distance(Point())<1)
        {
            PTS.push_back(tmp);
            RPTS.push_back(ctmp);
            G_POINTS_COUNTER++;
            G_MAX = recalc_max();
            repaint();
        }
        return true;
    }else if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = (QKeyEvent*)event;
        if (keyEvent->key() == Qt::Key_Escape)
        {
            G_MAX=0;
            PTS.clear();
            RPTS.clear();
            G_POINTS_COUNTER = 0;
            SLOT_RPTS.clear();
            G_SLOTS_COUNTER = 0;
            GMAX.clear();
            SAVE_PTS.clear();
            repaint();
        }
        return true;
    }
    return QWidget::event(event);
}

double Widget::recalc_max(void)
{
    std::vector<Point> circ_cens;
    std::vector<double> array0;
    double EPS = 0.001;//_!!

    for (int i = 0; i < G_POINTS_COUNTER; i++)
    {
        Edge t_diagonal(RPTS[i]);
        Point C1 = RPTS[i].between(t_diagonal._p0),
            C2 = RPTS[i].between(t_diagonal._p1);
        double R1 = RPTS[i].distance(C1),
            R2 = RPTS[i].distance(C2);
        bool cond1 = false, cond2 = false;
        for (int j = 0; j < G_POINTS_COUNTER && !cond1; j++)
            if ((j != i) && (C1.distance(RPTS[j]) < R1))
                cond1 = true;
        for (int j = 0; j < G_POINTS_COUNTER && !cond2; j++)
            if ((j != i) && (C2.distance(RPTS[j]) < R2))
                cond2 = true;
        if (!(cond1))
        {
            array0.push_back(R1);
            circ_cens.push_back(C1);
        }
        if (!(cond2))
        {
            array0.push_back(R2);
            circ_cens.push_back(C2);
        }
    }

    for (int i = 0; i < G_POINTS_COUNTER; i++)
        for (int j = i + 1; j < G_POINTS_COUNTER; j++)
        {
            double dt = EPS / 2, R, t, resVal;//_!!!
            if(RPTS[i].distance(RPTS[j])>EPS)
            {
                Point origin(RPTS[i].between(RPTS[j])), essen, cen;
                Edge Edge0(RPTS[i], RPTS[j]), tEdge;
                Edge0 = Edge0.rotate().extrapolation();
                std::vector<Edge>Edges;
                Edges.push_back(Edge(origin, Edge0._p0));
                Edges.push_back(Edge(origin, Edge0._p1));
                bool cond;
                for (int k = 0; k < 2; k++)
                {
                    t = 1;
                    cond = false;
                    while (t > 0)
                    {
                        cen = Edges[k].point(t);
                        tEdge = Edge(cen);
                        essen = tEdge.essential(cen);
                        resVal = cen.resVal(RPTS[i],essen);
                        if (resVal < EPS)
                        {
                            R = cen.distance(RPTS[i]);
                            for (int k = 0; k < G_POINTS_COUNTER && !cond; k++)
                                if ((k != i) && (k != j) && (cen.distance(RPTS[k]) < R))
                                    cond = true;
                            if (!(cond))
                            {
                                array0.push_back(R);
                                circ_cens.push_back(cen);
                            }
                            else cond = false;
                        }
                        t -= dt;
                    }
                }
            }
        }

    for (int i = 0; i < G_POINTS_COUNTER; i++)
        for (int j = i + 1; j < G_POINTS_COUNTER; j++)
            for (int k = j + 1; k < G_POINTS_COUNTER; k++)
            {
                if(RPTS[i].distance(RPTS[j])>EPS &&
                   RPTS[j].distance(RPTS[k])>EPS &&
                   RPTS[k].distance(RPTS[i])>EPS)
                {
                    Edge e1(RPTS[i], RPTS[j]),
                        e2(RPTS[j], RPTS[k]);
                    Point ptx = e1.normalCross(e2);
                    double R = ptx.distance(RPTS[i]);
                    bool cond = false;
                    if (R + ptx.distance(Point()) < 1)
                    {
                        for (int t = 0; (t < G_POINTS_COUNTER) && !cond; t++)
                            if ((t != i) && (t != j) && (t != k) && (ptx.distance(RPTS[t]) < R))
                                cond = true;
                    }
                    else cond = true;
                    if (!(cond))
                    {
                        array0.push_back(R);
                        circ_cens.push_back(ptx);
                    }
                }
            }

    double MAX = array0[0];
    GPT = circ_cens[0].correction(EXP, G_XCENTER, G_YCENTER, G_RADIUS);
    for (uint i = 1; i < array0.size(); i++)
        if (array0[i] > MAX)
        {
            MAX = array0[i];
            GPT = circ_cens[i].correction(EXP, G_XCENTER, G_YCENTER, G_RADIUS);
        }
    return MAX;
}
