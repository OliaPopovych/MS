/*коли ми вказуємо назву файлу в скобках, то кампілер шукає
 * файл в тій же директорії, що й проект, а не в окремій папці з лібами*/

/*Компіляція + компонування = сборка (build)
 * Компіляція (compile) - компілюється кожен фaйл програми окремо
 * Компонування (link) - cкомпільовані файли зв*язуються між собою в
  один виконуваний файл*/

#include "mainwindow.h"
#include <QApplication>

#include "matrix.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    cout << "Your plan:\n" << endl;
    string name = "E:\\Simpl.txt";

    Matrix plan;
    vector<double> interconPnts;

    //w передаємо по ссилці, бо w не має конструктора копіювання
    plan.readFromFile(&w, name);

    plan.shrihkMatSize();

    w.show_Graph_On_Scr(plan.getMatrix());

    plan.findPoint(&w);


    return a.exec();
}
