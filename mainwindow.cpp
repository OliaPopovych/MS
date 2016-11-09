#include "mainwindow.h"
#include "ui_mainwindow.h"


int n=0;
/*Все, пов*язане з вікнами, пишемо у цьому файлі*/
//Конструктор
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget = ui->widget;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_Equation_On_Scr(string line) const
{
    //MainWindow має доступ до своїх членів
    QString qstr = QString::fromStdString(line);
    ui->listWidget->addItem(qstr);
}

void MainWindow::show_Graph_On_Scr(vector<vector<int>> &mat)
{
    QVector<double> x(100), y(100); //одз і функція від одз
    //ітератори для проходу по матриці
    vector<vector<int>>::iterator rw;
    vector<int>::iterator st;
    size_t i;
    int k;

    QPen pen;
    QStringList lineNames;
    QString qstr;
    /*lineNames<<"0x1 +  1x2 <= 12"<<"1x1 -1x2 >= -10"<<"8x1 + 1x2 >= 16"
            <<"-1y + 5x2 >= 10"<<"1x1 + 3x2 >= 12"<<"-4x1 + 3x2 >= -48"
           <<"-1x1 + 3x2 >= -6"<<"-1x1 + 3x2 >= -6";*/
    for(rw = mat.begin() +1; rw < mat.end(); ++rw){
        for(i = 0; i < mat[0].size() - 1; ++i){
            if(i != 0 && (*rw)[i] > 0)
                qstr += " + " + QString::number((*rw)[i]) + "x" + QString::number(i+1);
            else
                if(i != 0)
                    qstr += " " + QString::number((*rw)[i]) + "x" + QString::number(i+1);
                else
                    qstr += QString::number((*rw)[i]) + "x" + QString::number(i+1);
        }
        qstr += " = " + QString::number((*rw)[i]);
        lineNames<<qstr;
        qstr.clear();
    }
    lineNames<<qstr;

    ui->widget->legend->setFont(QFont("Helvetica", 9));

    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");

    ui->widget->xAxis->setRange(-40, 40);
    ui->widget->yAxis->setRange(-30, 30);

   /* QVector<double> xes, v;
    xes.push_back(-40);
    xes.push_back(40);
    v.push_back(0);
    v.push_back(0);
    ui->widget->addGraph();
    ui->widget->graph()->setData(xes, v);*/

    //для кожного рядка матриці будуємо по лінії
    //Увага! ітератор mat.end() стає ЗА вектор
    //тоді rw доходить до останнього вектора
    for(rw = mat.begin() + 1; rw < mat.end(); rw++){
        for(i = 0, k = -40, st = (*rw).begin(); i < 100; i++, k++){
            x[i] = k;
            y[i] = ((-1)*st[0]*x[i] + st[2])/st[1];
        }
        ui->widget->addGraph();
        pen.setColor(QColor(qSin(n*1+1.2)*80+80, qSin(n*0.3+0)*80+80, qSin(n*0.3+1.5)*80+80));
        pen.setWidthF(2.5);
        ui->widget->graph(n)->setPen(pen);
        ui->widget->graph(n)->setData(x, y);
        ui->widget->graph(n)->setName(lineNames.at(n));

        n++;
    }
    ui->widget->graph(n-1)->removeFromLegend();
    ui->widget->legend->setVisible(true);
}

void MainWindow::show_Points_On_Scr(double x1, double x2)const
{
    QString qstr;

    qstr = QString::number(x1) + " " + QString::number(x2);
    ui->listWidget_2->addItem(qstr);
}

void MainWindow::show_Extremum_On_Scr(QString nameExt, double func, double x1, double x2)const
{
    QString qstr;

    qstr = nameExt + " of function = " +QString::number(func);
    ui->listWidget_2->addItem(qstr);
    qstr = "In point (" + QString::number(x1) + " , " + QString::number(x2) + ")";
    ui->listWidget_2->addItem(qstr);

    QVector<double> k, v;
    QPen pen;
    QString max = "Maximum", min = "Minimum";

    k.push_back(x1);
    v.push_back(x2);


    ui->widget->addGraph();
    if(nameExt == "\nMaximum"){
        pen.setColor(QColor(0, 0, 0, 255));
        ui->widget->graph(n)->setName(max);
    }
    else{
        pen.setColor(QColor(0, 160, 0, 255));
        ui->widget->graph(n)->setName(min);
    }
    pen.setWidth(2.5);
    ui->widget->graph(n)->setPen(pen);
    ui->widget->graph(n)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 15));
    ui->widget->graph(n)->setData(k, v);
    n++;

}

void MainWindow::show_Text_On_Scr(QString text, int opt) const
{
    if(opt == 1)
        ui->listWidget->addItem(text);
    else
        if(opt == 2)
            ui->listWidget_2->addItem(text);
}

void MainWindow::fill_Region(vector<QPointF>& points)
{
    auto it = points.begin();
    double i;
    QVector<double> x, y;
    x.reserve(100);
    y.reserve(100);
    size_t k = 0;


    for(; it < points.end() -1 && ( it->x() < (it+1)->x() ); ++it){
        k = 0;
        for(i = it->x(); i <= (it+1)->x(); i += 0.01){
            x.push_back(i);
            y.push_back( ( (it+1)->y() - it->y() ) * ( x[k] - it->x() ) / ( (it+1)->x() - it->x() ) + it->y() );
            k++;

        }
        ui->widget->addGraph();
        ui->widget->graph(n)->addData(x, y);
        ui->widget->graph(n)->setPen(QPen(Qt::red, 3));
        ui->widget->graph(n)->removeFromLegend();

        n++;

        x.clear();
        y.clear();

    }

    for(; it < points.end() -1; ++it){
        k = 0;
        for(i = it->x(); i >= (it+1)->x(); i -= 0.1){
            x.push_back(i);
            y.push_back(  ( (it+1)->y() - it->y() ) * ( x[k] - it->x() ) / ( (it+1)->x() - it->x() ) + it->y() );
            k++;

        }
        ui->widget->addGraph();
        ui->widget->graph(n)->addData(x, y);
        ui->widget->graph(n)->setPen(QPen(Qt::red, 3));
        ui->widget->graph(n)->removeFromLegend();

        x.clear();
        y.clear();
        n++;
    }


    k = 0;
    for(i = it->x(); i >= points[0].x(); i -= 0.1){
        x.push_back(i);
        y.push_back(  ( points[0].y() - it->y() ) * ( x[k] - it->x() ) / ( points[0].x() - it->x() ) + it->y() );
        k++;

    }
    ui->widget->addGraph();
    ui->widget->graph(n)->addData(x, y);
    ui->widget->graph(n)->setPen(QPen(Qt::red, 3));
    ui->widget->graph(n)->removeFromLegend();

    x.clear();
    y.clear();
    n++;
}

void MainWindow::show_LevLine(vector<int> func)
{
    //auto it = points.begin();
    int i, k;
    double rand;
    QVector<double> x(50), y(50);
    QPen pen;

    if(func[0] > func[1])
        rand = func[0];
    else
       rand = func[1];

    for(i = 0, k = -40; i < 50; i++, k++){
        x[i] = k;
        y[i] = ((-1)*func[0]*x[i] + rand)/func[1];
    }
    ui->widget->addGraph();
    ui->widget->graph(n)->addData(x, y);
    pen.setColor(Qt::black);
    pen.setWidthF(1.5);
    pen.setStyle(Qt::DashLine);
    ui->widget->graph(n)->setPen(pen);
    ui->widget->graph(n)->removeFromLegend();

    // by default, the legend is in the inset layout of the main axis rect.
    //So this is how we access it to change legend placement:
    ui->widget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignLeft);

    n++;
    x.clear();
    y.clear();

    //lets set direction vector

   /* for(i = 0; i < 20; i++){
        x.push_back(i);
        y.push_back(  x[i]/-7 );
    }
    /*ui->widget->addGraph();
    ui->widget->graph(n)->addData(x, y);
    pen.setColor(Qt::black);
    pen.setWidthF(1.5);
    pen.setStyle(Qt::DashLine);
    ui->widget->graph(n)->setPen(pen);
    ui->widget->graph(n)->removeFromLegend();*/

    // add the arrow:
    QCPItemLine *arrow = new QCPItemLine(ui->widget);
    arrow->start->setCoords(0, 0);
    arrow->end->setCoords(4.9, -0.7);
    arrow->setHead(QCPLineEnding::esSpikeArrow);

}
