#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector>
#include <QDebug>

struct structPoints
{
    float x;
    float y;
};


class OpenGLWidget : public QOpenGLWidget
{
    // Q_OBJECT
public:
    OpenGLWidget(QWidget *parent = nullptr);
    void contour();
    void setContur(const structPoints &vectorPoints);
    void newContour();
    void drawBisLines();

    void clearAll();//очищаетполе opengl;



    QVector <structPoints> p_structPoints;// оригинальный контур
    QVector <structPoints> p_structPointsLineEdit;
    QVector <structPoints> p_structNewContur;// offset
    QVector <structPoints> p_structBisLines;// дополнительные линии
    QVector<QVector<structPoints>> p_structAllContours;
    QVector<QVector<structPoints>> p_structDopLines;
    bool isPointInPoligon(const QVector<structPoints>& p_structPoints,
                          const QVector<structPoints>& p_structPointsLineEdit);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    structPoints global_PointsContur;
};

#endif // OPENGLWIDGET_H
