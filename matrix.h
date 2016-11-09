/*директива препроцесора ifndef перевіряє, чи існує
 * константа MATRIX_H.  Якщо так, то переходимо до #endif
 * якщо ні, то define створює константу MATRIX_H і виконується оголошення.
 * Тобто ці три диоективи захищають від повторного оголошегння файлу*/

#ifndef MATRIX_H
#define MATRIX_H

#include "mainwindow.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <QPointF>

using namespace std;

/*Можна оголосити простір імен до якого віднести наш клас, якщо
в прозі будуть 2 класи з однаковим іменем.
    Можна також зробити клас шаблонним, щоб юзати його потім для будь-чого.
Але увага! для шаблонних класів реалізацію методів пишемо прямо в заголовочних файлах*/
class Matrix
{
    vector<vector<int>> mat;
    vector<int> processModel;
    int kstRow, kstSt;
public:
    Matrix();

    ~Matrix();

    //Цей метод не виходить зробити константним
    vector<vector<int>>& getMatrix();

    int getKstRow()const;
    int getKstSt()const;

    void Substitution(int numb, char* symb, int i, int j);

    char* getNumb(char* symb, int* num)const;

    void readFromFile(MainWindow *w, string &fileName);

    //приводимо і-те обмеження до канонічного виду
    //щоб знак був <=
    void changeSign(char* symb, int i, int j);

    void transportMatrix();

    void shrihkMatSize();

    //Цей метод також не вдається зробити константним
    void printMatrix();

    //Знаходить точку перетину прямих методом крамера
    void findPoint(MainWindow *w);

    bool fits_To_Feasible_Region(double x1, double x2);
    double find_Func(double x1, double x2);

    void sort(vector<QPointF> &points);
    void QuickHull(vector<QPointF> set, QPointF pMin, QPointF pMax, vector<QPointF> &res);

    vector<QPointF> to_QPointF(double* points_x, double* points_y, size_t kst);


};

#endif // MATRIX_H
