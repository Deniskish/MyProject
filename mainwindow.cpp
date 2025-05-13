// #include "clipper2/clipper.core.h"
// #include "clipper2/clipper.h"
// #include "clipper2/clipper.offset.h"

#include "clipper.hpp"
#include "mainwindow.h"
#include "openglwidget.h"
#include "ui_mainwindow.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/create_straight_skeleton_from_polygon_with_holes_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_straight_skeleton_2.h>
#include <CGAL/Straight_skeleton_2/IO/print.h>
#include <CGAL/draw_straight_skeleton_2.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Point_2.h>
#include <memory>
#include <cassert>


//#include <iomanip> библиотека для манипуляций с форматированием ввода/вывода.

using namespace ClipperLib;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K ;//Kernel
typedef K::Point_2                   Point ;
typedef CGAL::Polygon_2<K>           Polygon_2 ;
typedef CGAL::Polygon_with_holes_2<K> Polygon_with_holes;
typedef CGAL::Straight_skeleton_2<K> Ss ;

typedef std::shared_ptr<Ss> SsPtr ;




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // glWidget = ui -> widget;
    // if(glWidget){
    //     qDebug() << "ок";
    // }
    // else{
    //     qDebug() << "Ошибка";
    // }

    ui->glWidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->glWidget->clearAll();
    QVector<structPoints> vectorPoints;
    QVector<structPoints> vectorPointsLineEdit;
    /*Данные считываются с LineEdit и добавляются в QVector*/
    QString text = ui -> lineEditCoordinate -> text().trimmed();
    if (!text.isEmpty()){
        QStringList listLineEdit = text.split(" ", Qt::SkipEmptyParts);
        if (listLineEdit.size() == 2){
            structPoints pointLineEdit;
            pointLineEdit.x = listLineEdit[0].toFloat();
            pointLineEdit.y = listLineEdit[1].toFloat();
            vectorPointsLineEdit.append(pointLineEdit);
        }
    }
    /*Данные читаются из файла выбранного пользователем*/
    QString fileContur = QFileDialog::getOpenFileName(nullptr, tr("Open file"), "", tr("text file (*.txt)"));
    qDebug() << "Выбран файл" << fileContur;
    ui -> lineEdit_FILENAME -> insert(fileContur);

    if (fileContur.isEmpty()) return;//если файл пустой выход из окна

    QFile filePotok(fileContur);
    if (filePotok.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream inPotok(&filePotok);
        while (!inPotok.atEnd()){
            QString line = inPotok.readLine().trimmed();
            if (line.isEmpty()) continue;
            QStringList listLine = line.split(" ", Qt::SkipEmptyParts);
            if (listLine.size() == 2){
                structPoints points;
                points.x = listLine[0].toFloat();
                points.y = listLine[1].toFloat();
                vectorPoints.append(points);
            }
        }
        filePotok.close();
    }
    if (!vectorPoints.isEmpty())
    {
        ui->glWidget->p_structPoints = vectorPoints;
        ui->glWidget->p_structPointsLineEdit = vectorPointsLineEdit;
        ui->glWidget->update();
        for (const auto &vP : vectorPoints){
            qDebug() << "Точки" << vP.x << vP.y;
        }
        for (const auto &vPLE : vectorPointsLineEdit){
            qDebug() << "Точки введеные пользователем" << vPLE.x << vPLE.y;
        }
    }

}

void MainWindow::on_pushButton_2_clicked()
{
//Создается вектор, читаются данные из LineEditCoordinate и добавляются в вектор. Рисуется точка по вектору
    QVector<structPoints> vectorPointsLineEdit;
    QString text = ui -> lineEditCoordinate -> text().trimmed();
    if (!text.isEmpty()){
        QStringList listLineEdit = text.split(" ", Qt::SkipEmptyParts);
        if (listLineEdit.size() == 2){
            structPoints pointLineEdit;
            pointLineEdit.x = listLineEdit[0].toFloat();
            pointLineEdit.y = listLineEdit[1].toFloat();
            vectorPointsLineEdit.append(pointLineEdit);
        }
    }

//Если вектор не пустой, то в glWidget(OpenGLWidget) заносятся 2 точки
    if (!vectorPointsLineEdit.isEmpty())
    {
        ui->glWidget->p_structPointsLineEdit = vectorPointsLineEdit;
        ui->glWidget->update();
        for (const auto &vPLE : vectorPointsLineEdit){
            qDebug() << "Точки введеные пользователем" << vPLE.x << vPLE.y;
        }
    }

    if (ui->glWidget->isPointInPoligon(ui->glWidget->p_structPoints,ui->glWidget->p_structPointsLineEdit)){//выозов функции проверки нахождения точки в контуре из openglwidget.cpp
        qDebug() << "Значение пользователя в контуре";
    }
    else {
        qDebug() << "Значение пользователя не в контуре";
    }
}




void MainWindow::on_pushButton_3_clicked()
{
        // Path subject;
        // subject << IntPoint(0, 0)
        //         << IntPoint(5, 0)
        //         << IntPoint(5, 5)
        //         << IntPoint(0, 5);

        // // Смещаем контур внутрь
        // ClipperOffset co;
        // co.AddPath(sub   ject, jtRound, etClosedPolygon);
        // Paths solution;
        // co.Execute(solution, -1);  // смещение внутрь
        // QVector <structPoints> qvpos;

        // // Выводим результат
        // for (const auto& pt : solution[0]) {
        //     std::cout << pt.X << ", " << pt.Y << std::endl;
        //     structPoints qvposs;
        //     qvposs.x = pt.X;
        //     qvposs.y = pt.Y;
        //     qvpos.append(qvposs);

        // }
        // ui->glWidget->p_structNewContur = qvpos;
        // ui->glWidget->update();


    //offset отрисовка фигуры, clipper
    QVector<structPoints> vectorPoints;
    vectorPoints.append(ui->glWidget->p_structPoints);
    bool ok = false;
    float znachenieINLineEdit = ui->lineEdit_NewContur->text().toDouble(&ok); //смещение
    double scale = 10000.0;//больше лучше

    Path path;

    for (const auto &vP : vectorPoints){
        path << IntPoint(static_cast<cInt>(vP.x * scale), static_cast<cInt>(vP.y * scale));
    }

    ClipperOffset clipper;
    clipper.AddPath(path, jtMiter, etClosedPolygon);
    Paths paths;
    clipper.Execute(paths, static_cast<cInt>(-znachenieINLineEdit * scale));
    QVector<QVector <structPoints>> qvpos;


    for(const auto &ptPaths : paths)
    {
        QVector <structPoints> OneContur;
        for(const auto &ptPath : ptPaths)
        {
            structPoints p;
            p.x = ptPath.X / scale;
            p.y = ptPath.Y / scale;
            OneContur.append(p);
        }
        qvpos.append(OneContur);
    }
    ui->glWidget->p_structAllContours = qvpos;//p_structAllContours = qvpos это QVector<QVector<structPoints>>, то есть список списков точек.
    ui->glWidget->update();


    //buildStraightSkeleton();//вызов функции
    buildStraightSkeletonExample();
}


// void MainWindow::buildStraightSkeleton(){//построение скелета
//     Polygon_2 poly;//это CGAL-многоугольник, котоый заполняется точками, которые указал пользователь.
//     for(const structPoints& vectorPoints : ui->glWidget->p_structPoints){
//         poly.push_back(Point(vectorPoints.x, vectorPoints.y));
//     }
//     if (!poly.is_simple()) {//обязательно использовать
//         qDebug() << "Контур самопересекается!";
//         return;
//     }
//     if (!poly.is_counterclockwise_oriented()) {//Проверка, что точки идут против часовой стрелки (это важно для CGAL).Если вдруг они идут по часовой — разворачиваем.
//         poly.reverse_orientation();
//     }

//     Polygon_with_holes polyWithHoles(poly);//для внутренних контуров
//     for (const QVector<structPoints>& holeVec : ui->glWidget->p_structAllContours) {
//         Polygon_2 hole;
//         for (const structPoints& pt : holeVec) {//из списка QVector берем по структуре и заносим в Polygon_2 из примера https://doc.cgal.org/latest/Straight_skeleton_2/Straight_skeleton_2_2Create_straight_skeleton_from_polygon_with_holes_2_8cpp-example.html
//             hole.push_back(Point(pt.x, pt.y));
//         }
//         if (!hole.is_clockwise_oriented()) {//проверка идет ли счет точек по часовой стрелке, важно только для внутренних контуров
//             hole.reverse_orientation();
//         }
//         polyWithHoles.add_hole(hole);
//     }//поиск внутренних контуров и их проверка закончена

//     SsPtr iss = CGAL::create_interior_straight_skeleton_2(polyWithHoles);//Построение внутреннего скелета
//     //SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());//Построение внутреннего скелета только для изначального контура по точкам из файла
//     CGAL::Straight_skeletons_2::IO::print_straight_skeleton(*iss);//текстовый отчет


//     //Функция возвращает `Straight_skeleton_2`, который содержит вершины и рёбра внутреннего скелета.
//     //vertices-вершины
//     //halfedges-ребра


//     QVector<QVector<structPoints>> skeletonLines;//список для ребер скелета

//     for (CGAL::Straight_skeleton_2<K>::Halfedge_iterator he = iss->halfedges_begin(); he != iss->halfedges_end(); ++he) {//Halfedge_iterator — это итератор по каждому полуребру структуры скелета.
//         if(he->is_bisector())//
//         {
//             Point a = he->opposite()->vertex()->point();//другой конец (противоположное полуребро ведёт к той же вершине с другой стороны).
//             Point b = he->vertex()->point();//один конец ребра,
//             if (he->opposite()->vertex()->is_skeleton() && he->vertex()->is_skeleton())
//                 continue;
//             structPoints aStruct;//=structPoints aStruct{ float(a.x()), float(a.y()) };
//             aStruct.x = a.x();
//             aStruct.y = a.y();
//             structPoints bStruct;//=structPoints bStruct{ float(b.x()), float(b.y()) };
//             bStruct.x = b.x();
//             bStruct.y = b.y();

//             // кладём в QVector<structPoints> для двух элементов
//             QVector<structPoints> seg;//вектор из двух точек, seg описывает одно ребро скелета
//             seg.append(aStruct);//первый элемент
//             seg.append(bStruct);//второй элемент
//             // добавляем в общий список
//             skeletonLines.append(seg);//добавляю в список ребер
//         }
//     }
//     qDebug()<<skeletonLines.size();
//     ui->glWidget->p_structDopLines = skeletonLines;//p_structAllContours = qvpos это QVector<QVector<structPoints>>, то есть список списков точек.
//     ui->glWidget->update();
// }


void MainWindow::buildStraightSkeleton(){//построение скелета
    Polygon_2 poly;//это CGAL-многоугольник, котоый заполняется точками, которые указал пользователь.
    for(const structPoints& vectorPoints : ui->glWidget->p_structPoints){
        poly.push_back(Point(vectorPoints.x, vectorPoints.y));
    }
    if (!poly.is_simple()) {//обязательно использовать
        qDebug() << "Контур самопересекается!";
        return;
    }
    if (!poly.is_counterclockwise_oriented()) {//Проверка, что точки идут против часовой стрелки (это важно для CGAL).Если вдруг они идут по часовой — разворачиваем.
        poly.reverse_orientation();
    }

    Polygon_with_holes polyWithHoles(poly);//для внутренних контуров
    for (const QVector<structPoints>& holeVec : ui->glWidget->p_structAllContours) {
        Polygon_2 hole;
        for (const structPoints& pt : holeVec) {//из списка QVector берем по структуре и заносим в Polygon_2 из примера https://doc.cgal.org/latest/Straight_skeleton_2/Straight_skeleton_2_2Create_straight_skeleton_from_polygon_with_holes_2_8cpp-example.html
            hole.push_back(Point(pt.x, pt.y));
        }
        if (!hole.is_clockwise_oriented()) {//проверка идет ли счет точек по часовой стрелке, важно только для внутренних контуров
            hole.reverse_orientation();
        }
        polyWithHoles.add_hole(hole);
    }//поиск внутренних контуров и их проверка закончена

    SsPtr iss = CGAL::create_interior_straight_skeleton_2(polyWithHoles);//Построение внутреннего скелета
    //SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());//Построение внутреннего скелета только для изначального контура по точкам из файла
    CGAL::Straight_skeletons_2::IO::print_straight_skeleton(*iss);//текстовый отчет


    //Функция возвращает `Straight_skeleton_2`, который содержит вершины и рёбра внутреннего скелета.
    //vertices-вершины
    //halfedges-ребра
    QVector<QVector<structPoints>> skeletonLines;//список для ребер скелета

    for (CGAL::Straight_skeleton_2<K>::Halfedge_iterator he = iss->halfedges_begin(); he != iss->halfedges_end(); ++he) {//Halfedge_iterator — это итератор по каждому полуребру структуры скелета.
        if(he->is_bisector())//
        {
            Point a = he->opposite()->vertex()->point();//другой конец (противоположное полуребро ведёт к той же вершине с другой стороны).
            Point b = he->vertex()->point();//один конец ребра,
            structPoints aStruct;//=structPoints aStruct{ float(a.x()), float(a.y()) };
            aStruct.x = a.x();
            aStruct.y = a.y();
            structPoints bStruct;//=structPoints bStruct{ float(b.x()), float(b.y()) };
            bStruct.x = b.x();
            bStruct.y = b.y();

            // кладём в QVector<structPoints> для двух элементов
            QVector<structPoints> seg;//вектор из двух точек, seg описывает одно ребро скелета
            seg.append(aStruct);//первый элемент
            seg.append(bStruct);//второй элемент
            // добавляем в общий список
            skeletonLines.append(seg);//добавляю в список ребер
        }
    }
    qDebug()<<skeletonLines.size();
    ui->glWidget->p_structDopLines = skeletonLines;//p_structAllContours = qvpos это QVector<QVector<structPoints>>, то есть список списков точек.
    ui->glWidget->update();
}


void MainWindow::buildStraightSkeletonExample()//тестовая функция пример с сайта https://doc.cgal.org/latest/Straight_skeleton_2/Straight_skeleton_2_2Create_straight_skeleton_from_polygon_with_holes_2_8cpp-example.html
{//добавил вывод на экран opengl
    // пример из документации:
    Polygon_2 outer;
    outer.push_back(Point(-1,-1));
    outer.push_back(Point(0,-12));
    outer.push_back(Point(1,-1));
    outer.push_back(Point(12,0));
    outer.push_back(Point(1,1));
    outer.push_back(Point(0,12));
    outer.push_back(Point(-1,1));
    outer.push_back(Point(-12,0));

    QVector <structPoints> qvposParent;
    qvposParent.append( structPoints{ -1, -1 } );
    qvposParent.append( structPoints{  0,-12 } );
    qvposParent.append( structPoints{  1, -1 } );
    qvposParent.append( structPoints{ 12,  0 } );
    qvposParent.append( structPoints{  1,  1 } );
    qvposParent.append( structPoints{  0, 12 } );
    qvposParent.append( structPoints{ -1,  1 } );
    qvposParent.append( structPoints{ -12, 0 } );
    ui->glWidget->p_structNewContur = qvposParent;
    ui->glWidget->update();


    Polygon_2 hole;
    hole.push_back(Point(-1,0));
    hole.push_back(Point(0,1));
    hole.push_back(Point(1,0));
    hole.push_back(Point(0,-1));
    QVector <structPoints> qvposHole;
    QVector<QVector<structPoints>> qvposHoleParent;
    qvposHole.append(structPoints{-1,0});
    qvposHole.append(structPoints{0,1});
    qvposHole.append(structPoints{1,0});
    qvposHole.append(structPoints{0,-1});
    qvposHoleParent.append(qvposHole);
    ui->glWidget->p_structAllContours = qvposHoleParent;
    ui->glWidget->update();


    assert(outer.is_counterclockwise_oriented());
    assert(hole.is_clockwise_oriented());

    Polygon_with_holes poly( outer );
    poly.add_hole( hole );

    SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly);
    // и выводим его на консоль
    CGAL::Straight_skeletons_2::IO::print_straight_skeleton(*iss);
    QVector<QVector<structPoints>> skeletonLines;//список для ребер слкелета

    for (CGAL::Straight_skeleton_2<K>::Halfedge_iterator he = iss->halfedges_begin(); he != iss->halfedges_end(); ++he) {//Halfedge_iterator — это итератор по каждому полуребру структуры скелета.
        if(!he->is_bisector()) continue;
        if (he->is_bisector()) {//проверка является ли ребро биссектрисой

            Point a = he->opposite()->vertex()->point();//другой конец (противоположное полуребро ведёт к той же вершине с другой стороны).
            Point b = he->vertex()->point();//один конец ребра,
            //if(poly.count(a)==0&&pol)
            structPoints aStruct;//=structPoints aStruct{ float(a.x()), float(a.y()) };
            aStruct.x = a.x();
            aStruct.y = a.y();
            structPoints bStruct;//=structPoints bStruct{ float(b.x()), float(b.y()) };
            bStruct.x = b.x();
            bStruct.y = b.y();

            // кладём в QVector<structPoints> для двух элементов
            QVector<structPoints> seg;//вектор из двух точек, seg описывает одно ребро скелета
            seg.append(aStruct);//первый элемент
            seg.append(bStruct);//второй элемент
            // добавляем в общий список
            skeletonLines.append(seg);//добавляю в список ребер
        }
    }
    ui->glWidget->p_structDopLines = skeletonLines;//p_structAllContours = qvpos это QVector<QVector<structPoints>>, то есть список списков точек.
    ui->glWidget->update();
}



// void MainWindow::on_pushButton_3_clicked()
// {
//     // Path subject;
//     // subject << IntPoint(0, 0)
//     //         << IntPoint(5, 0)
//     //         << IntPoint(5, 5)
//     //         << IntPoint(0, 5);

//     // // Смещаем контур внутрь
//     // ClipperOffset co;
//     // co.AddPath(sub   ject, jtRound, etClosedPolygon);
//     // Paths solution;
//     // co.Execute(solution, -1);  // смещение внутрь
//     // QVector <structPoints> qvpos;

//     // // Выводим результат
//     // for (const auto& pt : solution[0]) {
//     //     std::cout << pt.X << ", " << pt.Y << std::endl;
//     //     structPoints qvposs;
//     //     qvposs.x = pt.X;
//     //     qvposs.y = pt.Y;
//     //     qvpos.append(qvposs);

//     // }
//     // ui->glWidget->p_structNewContur = qvpos;
//     // ui->glWidget->update();


//     //offset отрисовка фигуры, clipper
//     QVector<structPoints> vectorPoints;
//     vectorPoints.append(ui->glWidget->p_structPoints);
//     bool ok = false;
//     float znachenieINLineEdit = ui->lineEdit_NewContur->text().toDouble(&ok); //смещение
//     double scale = 10000.0;//больше лучше

//     Path path;

//     for (const auto &vP : vectorPoints){
//         path << IntPoint(static_cast<cInt>(vP.x * scale), static_cast<cInt>(vP.y * scale));
//     }

//     ClipperOffset clipper;
//     clipper.AddPath(path, jtMiter, etClosedPolygon);
//     Paths paths;
//     clipper.Execute(paths, static_cast<cInt>(-znachenieINLineEdit * scale));
//     QVector<QVector <structPoints>> qvpos;


//     for(const auto &ptPaths : paths)
//     {
//         QVector <structPoints> OneContur;
//         for(const auto &ptPath : ptPaths)
//         {
//             structPoints p;
//             p.x = ptPath.X / scale;
//             p.y = ptPath.Y / scale;
//             OneContur.append(p);
//         }
//         qvpos.append(OneContur);
//     }
//     ui->glWidget->p_structAllContours = qvpos;//p_structAllContours = qvpos это QVector<QVector<structPoints>>, то есть список списков точек.
//     ui->glWidget->update();


//     buildStraightSkeleton();//вызов функции
//     //buildStraightSkeletonExample();
// }


// void MainWindow::buildStraightSkeleton(){//построение скелета
//     Polygon_2 poly;//это CGAL-многоугольник, котоый заполняется точками, которые указал пользователь.
//     for(const structPoints& vectorPoints : ui->glWidget->p_structPoints){
//         poly.push_back(Point(vectorPoints.x, vectorPoints.y));
//     }
//     if (!poly.is_simple()) {//обязательно использовать
//         qDebug() << "Контур самопересекается!";
//         return;
//     }
//     if (!poly.is_counterclockwise_oriented()) {//Проверка, что точки идут против часовой стрелки (это важно для CGAL).Если вдруг они идут по часовой — разворачиваем.
//         poly.reverse_orientation();
//     }

//     Polygon_with_holes polyWithHoles(poly);//для внутренних контуров
//     for (const QVector<structPoints>& holeVec : ui->glWidget->p_structAllContours) {//тут хранятся уменьшенные контуры
//         Polygon_2 hole;
//         for (const structPoints& pt : holeVec) {//из списка QVector берем по структуре и заносим в Polygon_2 из примера https://doc.cgal.org/latest/Straight_skeleton_2/Straight_skeleton_2_2Create_straight_skeleton_from_polygon_with_holes_2_8cpp-example.html
//             hole.push_back(Point(pt.x, pt.y));
//         }
//         if (!hole.is_clockwise_oriented()) {//проверка идет ли счет точек по часовой стрелке, важно только для внутренних контуров
//             hole.reverse_orientation();
//         }
//         polyWithHoles.add_hole(hole);
//     }//поиск внутренних контуров и их проверка закончена

//     //SsPtr iss = CGAL::create_interior_straight_skeleton_2(polyWithHoles);//Построение внутреннего скелета для изначального контура, но с дурками других уменьшенных контуров
//     //SsPtr iss0 = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());//Построение внутреннего скелета только для изначального контура по точкам из файла(ребро если верно понял)
//     //SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end(), polyWithHoles);
//     //CGAL::Straight_skeletons_2::IO::print_straight_skeleton(*iss);//текстовый отчет


//     //Функция возвращает `Straight_skeleton_2`, который содержит вершины и рёбра внутреннего скелета.
//     //vertices-вершины
//     //halfedges-ребра

//     SsPtr iss0 = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());//Построение внутреннего скелета только для изначального контура по точкам из файла(ребро если верно понял)
//     QVector<QVector<structPoints>> skeletonLines;//список для ребер скелета
//     for (CGAL::Straight_skeleton_2<K>::Halfedge_iterator he = iss0->halfedges_begin(); he != iss0->halfedges_end(); ++he) {//Halfedge_iterator — это итератор по каждому полуребру структуры скелета.
//         if(he->is_bisector())//
//         {
//             Point a = he->opposite()->vertex()->point();//другой конец (противоположное полуребро ведёт к той же вершине с другой стороны).
//             Point b = he->vertex()->point();//один конец ребра,
//             if (!(he->opposite()->vertex()->is_skeleton() ^ he->vertex()->is_skeleton()))//условие, которое проверяет, связан внешний контур, с одним из внутренних
//                 continue;
//             structPoints aStruct;//=structPoints aStruct{ float(a.x()), float(a.y()) };
//             aStruct.x = a.x();
//             aStruct.y = a.y();
//             structPoints bStruct;//=structPoints bStruct{ float(b.x()), float(b.y()) };
//             bStruct.x = b.x();
//             bStruct.y = b.y();

//             // кладём в QVector<structPoints> для двух элементов
//             QVector<structPoints> seg;//вектор из двух точек, seg описывает одно ребро скелета
//             seg.append(aStruct);//первый элемент
//             seg.append(bStruct);//второй элемент
//             // добавляем в общий список
//             skeletonLines.append(seg);//добавляю в список ребер
//         }

//     }
//     qDebug()<<skeletonLines.size();
//     // ui->glWidget->p_structDopLines = skeletonLines;//p_structAllContours = qvpos это QVector<QVector<structPoints>>, то есть список списков точек.
//     // ui->glWidget->update();

//     SsPtr iss1 = CGAL::create_interior_straight_skeleton_2(polyWithHoles);//Построение внутреннего скелета только для изначального контура, но с дурками других уменьшенных контуров
//     //QVector<QVector<structPoints>> skeletonLines1;//список для ребер скелета
//     for (CGAL::Straight_skeleton_2<K>::Halfedge_iterator he = iss1->halfedges_begin(); he != iss1->halfedges_end(); ++he) {//Halfedge_iterator — это итератор по каждому полуребру структуры скелета.
//         if (he->is_bisector()){
//             Point a = he->opposite()->vertex()->point();//другой конец (противоположное полуребро ведёт к той же вершине с другой стороны).
//             Point b = he->vertex()->point();//один конец ребра,
//             if (!(he->opposite()->vertex()->is_skeleton() ^ he->vertex()->is_skeleton()))
//                 continue;
//             structPoints aStruct;//=structPoints aStruct{ float(a.x()), float(a.y()) };
//             aStruct.x = a.x();
//             aStruct.y = a.y();
//             structPoints bStruct;//=structPoints bStruct{ float(b.x()), float(b.y()) };
//             bStruct.x = b.x();
//             bStruct.y = b.y();

//             // кладём в QVector<structPoints> для двух элементов
//             QVector<structPoints> seg;//вектор из двух точек, seg описывает одно ребро скелета
//             seg.append(aStruct);//первый элемент
//             seg.append(bStruct);//второй элемент
//             // добавляем в общий список
//             skeletonLines.append(seg);//добавляю в список ребер
//         }

//     }
//     ui->glWidget->p_structDopLines = skeletonLines;//p_structAllContours = qvpos это QVector<QVector<structPoints>>, то есть список списков точек.
//     ui->glWidget->update();
// }


// // void MainWindow::buildStraightSkeleton(){//построение скелета
// //     Polygon_2 poly;//это CGAL-многоугольник, котоый заполняется точками, которые указал пользователь.
// //     for(const structPoints& vectorPoints : ui->glWidget->p_structPoints){
// //         poly.push_back(Point(vectorPoints.x, vectorPoints.y));
// //     }
// //     if (!poly.is_simple()) {//обязательно использовать
// //         qDebug() << "Контур самопересекается!";
// //         return;
// //     }
// //     if (!poly.is_counterclockwise_oriented()) {//Проверка, что точки идут против часовой стрелки (это важно для CGAL).Если вдруг они идут по часовой — разворачиваем.
// //         poly.reverse_orientation();
// //     }

// //     Polygon_with_holes polyWithHoles(poly);//для внутренних контуров
// //     for (const QVector<structPoints>& holeVec : ui->glWidget->p_structAllContours) {
// //         Polygon_2 hole;
// //         for (const structPoints& pt : holeVec) {//из списка QVector берем по структуре и заносим в Polygon_2 из примера https://doc.cgal.org/latest/Straight_skeleton_2/Straight_skeleton_2_2Create_straight_skeleton_from_polygon_with_holes_2_8cpp-example.html
// //             hole.push_back(Point(pt.x, pt.y));
// //         }
// //         if (!hole.is_clockwise_oriented()) {//проверка идет ли счет точек по часовой стрелке, важно только для внутренних контуров
// //             hole.reverse_orientation();
// //         }
// //         polyWithHoles.add_hole(hole);
// //     }//поиск внутренних контуров и их проверка закончена

// //     SsPtr iss = CGAL::create_interior_straight_skeleton_2(polyWithHoles);//Построение внутреннего скелета
// //     //SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());//Построение внутреннего скелета только для изначального контура по точкам из файла
// //     CGAL::Straight_skeletons_2::IO::print_straight_skeleton(*iss);//текстовый отчет


// //     //Функция возвращает `Straight_skeleton_2`, который содержит вершины и рёбра внутреннего скелета.
// //     //vertices-вершины
// //     //halfedges-ребра
// //     QVector<QVector<structPoints>> skeletonLines;//список для ребер скелета

// //     for (CGAL::Straight_skeleton_2<K>::Halfedge_iterator he = iss->halfedges_begin(); he != iss->halfedges_end(); ++he) {//Halfedge_iterator итератор по каждому полуребру структуры скелета.
// //         if(he->is_bisector())//
// //         {
// //             Point a = he->opposite()->vertex()->point();//другой конец (противоположное полуребро ведёт к той же вершине с другой стороны).
// //             Point b = he->vertex()->point();//один конец ребра,
// //             auto v1 = he->opposite()->vertex();
// //             auto v2 = he->vertex();
// //             bool a_inside = poly.bounded_side(v1->point()) == CGAL::ON_BOUNDED_SIDE;
// //             bool b_inside = poly.bounded_side(v2->point()) == CGAL::ON_BOUNDED_SIDE;
// //             if(a_inside && b_inside)
// //             {
// //                 structPoints aStruct;//=structPoints aStruct{ float(a.x()), float(a.y()) };
// //                 aStruct.x = a.x();
// //                 aStruct.y = a.y();
// //                 structPoints bStruct;//=structPoints bStruct{ float(b.x()), float(b.y()) };
// //                 bStruct.x = b.x();
// //                 bStruct.y = b.y();

// //                 // кладём в QVector<structPoints> для двух элементов
// //                 QVector<structPoints> seg;//вектор из двух точек, seg описывает одно ребро скелета
// //                 seg.append(aStruct);//первый элемент
// //                 seg.append(bStruct);//второй элемент
// //                 // добавляем в общий список
// //                 skeletonLines.append(seg);//добавляю в список ребер
// //             }
// //         }
// //     }
// //     qDebug()<<skeletonLines.size();
// //     ui->glWidget->p_structDopLines = skeletonLines;//p_structAllContours = qvpos это QVector<QVector<structPoints>>, то есть список списков точек.
// //     ui->glWidget->update();
// // }
// /*ну вот щас проблема такая, из большого и уменьшенного и большого контура идут биссектрисы, они соединяются в некоторой точке,
// и по этим точкам строится по сути еще один контур, мне надо как то убрать этот контур,
// но при этом биссектрисы должны соединятся для соединения большого и уменьшенного контура,
// вот как эту  задачу даже сформулировать не знаю, не то что решить. может какой то математический подход тут есть.*/
