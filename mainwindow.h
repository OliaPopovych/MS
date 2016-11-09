#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <QPointF>

using namespace std;
//Тут тільки оголошення
namespace Ui {
class MainWindow;
}

//моє вікно наслідується від шаблонного класу
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui; //юі - це моє вікно
    //юі - вказівник на об. класу вікно

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void show_Equation_On_Scr(string line)const;
    void show_Graph_On_Scr(vector<vector<int> > &mat);
    void show_Points_On_Scr(double x1, double x2)const;
    void show_Extremum_On_Scr(QString nameExt, double func, double x1, double x2)const;
    void show_Text_On_Scr(QString text, int opt)const;

    void fill_Region(vector<QPointF> &points);
    //void show_LevLine(vector<vector<int>>::iterator it);
    void show_LevLine(vector<int> func);
};

#endif // MAINWINDOW_H
