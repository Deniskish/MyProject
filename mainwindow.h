
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "openglwidget.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QFileDialog>
#include <QOpenGLFunctions>
#include <QOpenGLWindow>
#include <QTimer>
#include <gl/GL.h>
#include <QMatrix4x4>
#include <QVector>
#include <QPolygonF>
#include <QPoint>
#include <QtOpenGLWidgets/QtOpenGLWidgets>
#include <QOpenGLWidget>
#include <QtMath>
#include <cmath>
#include <cassert>



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    // OpenGLWidget *glWidget;
    void buildStraightSkeleton();
    void buildStraightSkeletonExample();
};



#endif // MAINWINDOW_H
