#include "matrix.h"

using namespace std;

static const double eps = 0.001;

Matrix::Matrix() : mat(10, vector<int>(4)){

}

Matrix::~Matrix(){
    //Вектор сам видаляється?
   /* for(int i=0, size=mat.capacity(); i<size; ++i){
        mat[i].();
    } */
}

vector<vector<int>>& Matrix::getMatrix(){
    return mat;
}

int Matrix::getKstRow()const{
    return this->kstRow;
}
int Matrix::getKstSt()const{
    return this->kstSt;
}

void Matrix::Substitution(int numb, char* symb, int i, int j){
    int num;
    symb++;

    for(;*symb != '\0'; symb++){

        if(*symb >= 48 && *symb <= 57 && *(symb - 1) != 'x'){
            symb = getNumb(symb, &num);
            j++;
            mat[i][j] = num;
        }
    }

    for(int n = 0; n < this->kstSt - 1; n++){
        mat[i][n] = numb * this->processModel[n] + mat[i][n];
    }
}

char* Matrix::getNumb(char* symb, int* num)const{
    char* endPtr = symb, *startPtr = symb-1, *tmp;
    int k;
    *num = 0;

    while(*endPtr >= 48 && *endPtr <= 57)
        endPtr++;
    endPtr--;
    tmp = endPtr;//запам*ятаємо кінець числа

    for(k = 1; endPtr != startPtr; endPtr--, k*=10)
        *num += (*endPtr - 48) * k;

    if(*(startPtr) == '-')
        *num = ~(*num) + 1;

    return tmp;
}

void Matrix::readFromFile(MainWindow *w, string &fileName){
    ifstream file;
    file.open(fileName);

    int numb; //дістаємо число. Число може мати більше 1ї цифри,
    //тому для нього треба створити буф. змінну і туди записати повністю
    int i = 0, j = 0;//для позиції в матриці в яку треба записати коеф.
    char *symb;
    string line;

    w->show_Text_On_Scr("Read from file:\n", 1);

    if(file.is_open()){
        //отримуємо модель процесу
        getline(file, line);
        w->show_Equation_On_Scr(line);
        for(symb = &line[0]; *symb != '\0'; symb++){
            if(*symb >= 48 && *symb <= 57 && *(symb - 1) != 'x'){
                symb = getNumb(symb, &numb);
                this->processModel.push_back(numb);
                i++;
            }
        }
        this->kstSt = i+1;

        i = -1;
        while(file.good()){
            getline(file, line); //з файла в line
            w->show_Equation_On_Scr(line);
            //cout<<line<<endl;
            i++;
            j=0;
            for(symb = &line[0]; (*symb) != '\0'; symb++){
                if(*symb == '<' && i != 0){
                    changeSign(symb, i, j);
                    break;
                }

                if(*symb >= 48 && *symb <= 57 && *(symb-1) != 'x'){
                    symb = getNumb(symb, &numb);

                    //після ф-ї getNumb, symb стоїть на останній цифрі числа
                    //якщо після неї у, то виконуємо особливу підстановку
                    if(*(symb + 1) == 'y'){
                        Substitution(numb, symb, i, j);
                        break;
                    }
                    else{
                        this->mat[i][j] = numb;
                        j++;
                    }
                }
            }
        }
    }
    this->kstRow = i;
}

//приводимо і-те обмеження до канонічного виду
//щоб знак був <=
void Matrix::changeSign(char* symb, int i, int j){
    int num;
    while(*symb < 48 || *symb > 57)
        symb++;
    symb = getNumb(symb, &num);
    this->mat[i][j] = num;

    for(int n=0; n<this->kstSt; n++){
        mat[i][n] = ~mat[i][n] + 1;
    }
}

void Matrix::transportMatrix(){
    vector<vector<int>>::iterator it, endOfSt = mat.begin() + this->kstRow;
    vector<int>::iterator el, endOfRow;
    int oldKstSt;
    int tmp, i, j;

    //преревага ітераторів над індексами у тому,
    //що ітератор не рахує з початку масиву
    for(it = mat.begin(); it < endOfSt; it++){
        (*it).resize(this->kstRow);
    }
    oldKstSt = this-> kstSt;
    this->kstSt = this->kstRow;

    for(j = 0; j < this->kstSt; j++)
        for(i=j; i < this->kstRow; i++){
            tmp = mat[j][i];
            mat[j][i] = mat[i][j];
            mat[i][j] = tmp;
        }
    this->kstRow = oldKstSt;

}

void Matrix::shrihkMatSize(){
    vector<vector<int>>::iterator it;
    vector<int>::iterator el;

    mat.resize(this->kstRow);

    for(it = mat.begin(); it < mat.end(); it++)
            (*it).resize(this->kstSt);
}

void Matrix::printMatrix(){
    vector<vector<int>>::iterator it;
    vector<int>::iterator el;

    for(it = this->mat.begin(); it < this->mat.end(); it++){
        for(el = (*it).begin(); el < (*it).end(); el++){
            //вирівнювання по лівому краю в полі
            //довжиною в 5 позицій
           // printf("%-5d ", *el);
            cout<<*el<<"\t";
        }
        cout<<endl;
    }
}

//Знаходить точку перетину прямих методом крамера
void Matrix::findPoint(MainWindow *w)
{
    int i, n, j, kst = 0, delta, delta1, delta2;
    double x1, x2;
    double max = 0, min = 0, max_x1, max_x2, min_x1, min_x2, funcValue;

    //Увага! Точок перетину може бути максимум 20
    double *points_x = new double[20];
    double *points_y = new double[20];

    w->show_Text_On_Scr("Points of interconnection: ", 2);

    for(i=1; i < mat.end() - mat.begin() -1; ++i){
        for(n = 1; n < this->kstRow - i; ++n){
            j = 0;
            delta = mat[i][j] * mat[i+n][j+1] - mat[i+n][j]*mat[i][j+1];
            if(delta == 0){
                cout<<"\nParalel lines";
               // return 0;
                continue;
            }

            delta1 = mat[i][j+2] * mat[i+n][j+1] - mat[i+n][j+2] * mat[i][j+1];
            delta2 = mat[i][j] * mat[i+n][j+2] - mat[i][j+2] * mat[i+n][j];

            x1 = (double)delta1/delta;
            x2 = (double)delta2/delta;



            if(x1 >= 0 && x2 >=0 && fits_To_Feasible_Region(x1, x2)){
                points_x[kst] = x1;
                points_y[kst] = x2;
                kst++;

                w->show_Points_On_Scr(x1, x2);
                funcValue = find_Func(x1, x2);
                if(funcValue > max){
                    max = funcValue;
                    max_x1 = x1;   max_x2 = x2;
                }
                else
                    if(funcValue < min){
                        min = funcValue;
                        min_x1 = x1;   min_x2 = x2;
                    }
            }
        }
    }


    vector<QPointF> points = to_QPointF(points_x, points_y, kst);
    sort(points);

    w->fill_Region(points);
    w->show_LevLine(mat[0]);
    //w->show_LevLine(this->mat.begin());

    w->show_Extremum_On_Scr("\nMaximum", max, max_x1, max_x2);
    w->show_Extremum_On_Scr("Minimum", min, min_x1, min_x2);


}

bool Matrix::fits_To_Feasible_Region(double x1, double x2)
{
    vector<vector<int>>::iterator it;

    //підставляємо точку в кожну нерівність і
    //перевіряємо, чи вона справджується
    for(it = mat.begin() + 1; it < mat.end(); ++it){
        if( (*it)[0] * x1 + (*it)[1] * x2 < (*it)[2] - eps )
            return 0;
    }

    return 1;
}

double Matrix::find_Func(double x1, double x2)
{
    return mat[0][0] * x1 + mat[0][1] * x2;
}


vector<QPointF> Matrix::to_QPointF(double *points_x, double *points_y, size_t kst)
{
    vector<QPointF> vec;
    vec.reserve(20);
    QPointF p;

    for(size_t i=0; i < kst; ++i){
        p.setX((qreal)points_x[i]);
        p.setY((qreal)points_y[i]);
        vec.push_back(p);
    }

    vec.shrink_to_fit();
    return vec;
}

void Matrix::sort(vector<QPointF> &points)
{

    auto ccw = [](QPointF p1, QPointF p2, QPointF p3)
    {return (p2.x() - p1.x())*(p3.y() - p1.y()) - (p2.y() - p1.y())*(p3.x() - p1.x());};

    vector<QPointF> res;

    double max = 0, min = 1e9;
    QPointF pMax,pMin;

    for(auto point:points)
    {
        if(point.x() > max)
            pMax = point, max = point.x();
        if(point.x() < min)
            pMin = point, min = point.x();
    }

    vector<QPointF> upper, lower;
    for(auto point:points)
    {
        if(point == pMax || point == pMin)
            continue;
        if(ccw(pMin,point,pMax)>=0)
            upper.push_back(point);
        else
            lower.push_back(point);
    }

    res.push_back(pMin);
    QuickHull(upper, pMin,pMax,res);
    res.push_back(pMax);
    QuickHull(lower,pMax,pMin,res);

    //Видаляємо точки, які повторюються у векторі
    for(auto it = res.begin(); it < res.end() -1; ++it){
        if(it->x() == (it + 1)->x() && it->y() == (it + 1)->y())
            res.erase(it);
    }

    points = res;
}

void Matrix::QuickHull(vector<QPointF> set, QPointF pMin, QPointF pMax, vector<QPointF> &res)
{
    if(set.empty())
        return;

    auto len = [](QPointF A, QPointF B, QPointF C)
    {
        double a = B.y() - A.y();
        double b = A.x() - B.x();
        return fabs((a*C.x() + b*C.y() + a*A.x() + b*B.x())) / sqrt(a*a + b*b);
    };

    auto ccw = [](QPointF p1, QPointF p2, QPointF p3)
    {return (p2.x() - p1.x())*(p3.y() - p1.y()) - (p2.y() - p1.y())*(p3.x() - p1.x());};

    double l = 0;

    QPointF max;
    for(auto point:set)
    {
        if(l < len(pMin,pMax,point))
            l = len(pMin,pMax,point), max = point;
    }

    vector<QPointF> upper, lower;
    for(auto point:set)
    {
        if(point == pMax || point == pMin)
            continue;
        if(ccw(pMin,point,max)>0)
            upper.push_back(point);
        if(ccw(max,point,pMin)>0)
            lower.push_back(point);
    }

    QuickHull(upper,pMin,max,res);
    res.push_back(max);
    QuickHull(lower,max,pMax,res);

}


