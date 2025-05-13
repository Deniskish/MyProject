#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget()
{
    //qDebug() << Q_FUNC_INFO;
}


void OpenGLWidget::initializeGL()
{
    //qDebug() << Q_FUNC_INFO;
    // initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}



void OpenGLWidget::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glFrustum(-1.5f, 1.5f, -1.5f, 1.5f, 1.0f, 100.0f);
}



void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0f, 0.0f, -10.0f);
    //qDebug()<<"работает";
    contour();
    newContour();


    //p_structAllContours — это QVector<QVector<structPoints>>, то есть список списков точек.
    for (const auto &contour : p_structAllContours) {//contour координата из списка p_structAllContours
        glBegin(GL_LINE_LOOP);
        for (const auto &pt : contour) {//pt точка для каждой координаты contour
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex2f(pt.x, pt.y);
        }
        glEnd();
    }


    //попытка отрисовки линий
    for (const auto &line : p_structDopLines) {
        if (line.size() == 2) {
            glBegin(GL_LINES);           // начинаем рисовать одиночный отрезок
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex2f(line[0].x, line[0].y); // первая точка отрезка
            glVertex2f(line[1].x, line[1].y); // вторая точка отрезка
            glEnd();                      // завершаем отрисовку отрезка
        }
    }



}


// void OpenGLWidget::setContur(const structPoints &vectorPoints)
// {
//     global_PointsContur = vectorPoints;
//     update();
// }


void OpenGLWidget::newContour()
{
    glBegin(GL_LINE_LOOP);
    glLineWidth(5.0f);
    glColor3f(1.0f, 0.0f, 0.0f); // Красный
    for (const auto &gl_vP : p_structNewContur){
        glVertex2f(gl_vP.x, gl_vP.y);
    }
    glEnd();
}


void OpenGLWidget::contour()//
{
    glPointSize(10.0f);



    //начало отрисовки точки проверяющей находится ли в контуре
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 1.0f);
    for (const auto &gl_vPLE : p_structPointsLineEdit){
        glVertex2f(gl_vPLE.x, gl_vPLE.y);
    }
    glEnd();
    //конец отрисовки точки проверяющей находится ли в контуре


    //начало отрисовки контура
    glBegin(GL_LINE_LOOP);
    glLineWidth(5.0f);
    glColor3f(1.0f, 0.0f, 0.0f); // Красный
    for (const auto &gl_vP : p_structPoints){
        glVertex2f(gl_vP.x, gl_vP.y);
    }
    glEnd();
    //конец отрисовки контура
    glLineWidth(5.0f); // Толщина линий


    // //начало отрисовки координатных линий
    // glBegin(GL_LINES);
    // glColor3f(0.0f, 0.0f, 0.0f);
    // glVertex3f(-15.0f, 0.0f, 0.0f);
    // glVertex3f(15.0f, 0.0f, 0.0f); // Линия по X

    // glColor3f(0.0f, 0.0f, 0.0f); // синий
    // glVertex3f(0.0f, -15.0f, 0.0f);
    // glVertex3f(0.0f, 15.0f, 0.0f); // Линия по Y
    // glEnd();
    // //конец отрисовки координатных линий
}

void OpenGLWidget::drawBisLines() {
    glLineWidth(2.0f);
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i+1 < p_structBisLines.size(); i += 2) {
        glVertex2f(p_structBisLines[i].x,   p_structBisLines[i].y);
        glVertex2f(p_structBisLines[i+1].x, p_structBisLines[i+1].y);
    }
    glEnd();
}

void OpenGLWidget::clearAll()
{
    p_structPoints.clear();
    p_structPointsLineEdit.clear();
    p_structNewContur.clear();
    p_structAllContours.clear();
    p_structDopLines.clear();
    p_structBisLines.clear();
    // после очистки перерисовывает пустое окно
    update();
}

//Функция для определения, условия нахождения точки в контуре.
/*Проводим луч из точки вправо. Считаем сколько раз луч пересекает стороны многоугольник.
Нечетное число пересечений значит, точка внутри, четное снаружи.*/
bool OpenGLWidget::isPointInPoligon(const QVector<structPoints>& p_structPoints,
                                    const QVector<structPoints>& p_structPointsLineEdit)
{

    int n = p_structPoints.size();
    bool inside = false;

    for (const auto &pointsLineEdit : p_structPointsLineEdit){
        for(int i = 0; i < n; i++){
            structPoints p1 = p_structPoints[i];
            structPoints p2 = p_structPoints[(i + 1) % n];

            bool yCheck = (p1.y > pointsLineEdit.y) != (p2.y > pointsLineEdit.y);

            float xIntersect = (p2.x - p1.x) * (pointsLineEdit.y - p1.y) / (p2.y - p1.y) + p1.x;

            if (yCheck && pointsLineEdit.x < xIntersect) {
                inside =!inside;
            }
            //qDebug() << inside;
        }
    }

    return inside;
}
