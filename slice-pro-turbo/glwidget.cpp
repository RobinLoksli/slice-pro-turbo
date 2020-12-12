#include "glwidget.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "windows.h"
#include <QDebug>
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QVector3D>
#include <math.h>

struct point{
    float X;
    float Y;
    float Z;
};

struct triangle{
    point Normal;
    point p[3];
};

struct point2D{
    float x;
    float y;
};

///Структура хранящая ограничивающий многоугольник для процесса создания ячеек вороного
struct poligone{
    QVector <point2D> StackCorners;
};

struct state{
    bool grid;
};

float GabariteMaxX;
float GabariteMinX;
float GabariteMaxY;
float GabariteMinY;
float GabariteMaxZ;
float GabariteMinZ;

QVector <triangle>triangleBase;
QVector <state>states;

QVector <point>pointSeparation;
QVector <int>pointSeparationID;
QVector <point>OutLineSeparation;
QVector <int>OutLineSeparationID;
QVector <QVector <point> > OutLineLoop;
QVector <QVector <int> > OutLineLoopID;
int countLoops;
QVector <point>InnerPoints;
QVector <point2D> MeabyPoint;
QVector <poligone> PoligoneBase;

float SlicerHeight;
int offsetUpForLinePerimetr;
bool HideSolid;
float LayerHeight;




GLWidget::GLWidget(QWidget *parent)
{
    setFocusPolicy(Qt::StrongFocus);
    xRot = yRot = zRot = 0;
    yRot = 720;
    xRot = 480;
    xPos = yPos = 0;
    zoomScale = 0.05;

    setXRotation(xRot);
    setYRotation(yRot);
    setZRotation(zRot);
}

/// Initialization method, initializes lighting and stuff
void GLWidget::initializeGL()
{
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    // Настройки глута - нужны для верного отображения
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);

    // Настройка света и прозрачности
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

/// Render method, everything gets rendered here
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslated(xPos, yPos, -10.0);

    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

    // Отрисовка модели и сетки
    glPushMatrix();
        glScalef(zoomScale, zoomScale, zoomScale);
        drawGrid();

//        glutSolidTeapot(1.0);
        //строим модель
        glBegin(GL_TRIANGLES);
            glColor4f(0.8, 0.8, 0.1, 0.8);
            for (int i = 0; i < triangleBase.size(); i++){
                glNormal3f(triangleBase[i].Normal.X, triangleBase[i].Normal.Y, triangleBase[i].Normal.Z);
                glVertex3f(triangleBase[i].p[0].X,   triangleBase[i].p[0].Y,   triangleBase[i].p[0].Z);
                glVertex3f(triangleBase[i].p[1].X,   triangleBase[i].p[1].Y,   triangleBase[i].p[1].Z);
                glVertex3f(triangleBase[i].p[2].X,   triangleBase[i].p[2].Y,   triangleBase[i].p[2].Z);
            }
        glEnd();

//    glLineWidth(3);
//    glBegin(GL_LINES);
//        glColor4f(1,0,0,1);
//        glNormal3f(0,0,1);
//        for (int i=0;i<offsetUpForLinePerimetr-1;i++){
//            if (OutLineSeparationID[i+1]==OutLineSeparationID[i]){
//                glVertex3f(OutLineSeparation[i].X,OutLineSeparation[i].Y,OutLineSeparation[i].Z);
//                glVertex3f(OutLineSeparation[i+1].X,OutLineSeparation[i+1].Y,OutLineSeparation[i+1].Z);
//            }
//        }
//    glEnd();

    glLineWidth(2);
    for (int j=0;j<OutLineLoop.size();j++){
        glBegin(GL_LINES);
        glColor4f(0,1,0,1);
        glNormal3f(0,0,1);
        for (int i=0;i<OutLineLoop.at(j).size()-1;i++){
            if (OutLineLoopID.at(j).at(i+1)==OutLineLoopID.at(j).at(i)){
                glVertex3f(OutLineLoop.at(j).at(i).X,OutLineLoop.at(j).at(i).Y,OutLineLoop.at(j).at(i).Z);
                glVertex3f(OutLineLoop.at(j).at(i+1).X,OutLineLoop.at(j).at(i+1).Y,OutLineLoop.at(j).at(i+1).Z);
            }
        }
        glEnd();
    }

    glPopMatrix();

    // Отрисовка оси координат поверх всего
    drawOrigin();

    glLineWidth(1);


}


/// Window resize handler
void GLWidget::resizeGL(int w, int h)
{
//    int side = qMin(w, h);
//    glViewport((w - 2.0 * side) / 2, (h - 2.0 * side) / 2, 2.0 * side, 2.0 * side);

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//#ifdef QT_OPENGL_ES_1
//    glOrthof(-2.0, +2.0, -2.0, +2.0, 4.0, 15.0);
//#else
//    glOrtho(-2.0, +2.0, -2.0, +2.0, 4.0, 15.0);
//#endif
//    glMatrixMode(GL_MODELVIEW);

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::drawOrigin()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glLineWidth(4);
    glBegin(GL_LINES);
        glNormal3f(0, 0, 1);
        // X
        glColor4f(1.0, 0.0, 0.0, 0.2);
        glVertex3f(-5.0,  0.0,  0.0);
        glVertex3f( 5.0,  0.0,  0.0);
        // Y
        glColor4f(0.0, 1.0, 0.0, 0.5);
        glVertex3f( 0.0, -5.0, 0.0);
        glVertex3f( 0.0,  5.0, 0.0);
        // Z
        glColor4f(0.0, 0.0, 1.0, 0.5);
        glVertex3f( 0.0,  0.0, -5.0);
        glVertex3f( 0.0,  0.0,  5.0);
    glEnd();
    glEnable(GL_LIGHTING);
}


void GLWidget::drawGrid()
{
    glColor4f(0.5, 0.5, 0.5, 1.0);
    glBegin(GL_LINES);
    for (GLfloat i = -20.0; i <= 20.0; i += 2.0) {
        glVertex3f(i, 0, 20.0); glVertex3f(i, 0, -20.0);
        glVertex3f(20.0, 0, i); glVertex3f(-20.0, 0, i);
    }
    glEnd();
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
//    setFocus();
    lastPos = event->pos();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
//    setFocus();
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() == Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() == Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    } else if (event->buttons() == Qt::MiddleButton) {
//        xPos += dx / ((double)width() / 3.0);
//        yPos -= dy / ((double)height() / 2.0);
        xPos += (10.0 * dx) / ((double)width());
        yPos -= (8.0  * dy) / ((double)height());
        updateGL();
    }

    lastPos = event->pos();

}


/// reset all camera movements
void GLWidget::wheelEvent(QWheelEvent *event)
{
//    setFocus();
    QPoint numDegrees = event->angleDelta();
    if (numDegrees.y() < 0) zoomScale /= 1.5;
    if (numDegrees.y() > 0) zoomScale *= 1.5;

    updateGL();
}


void GLWidget::keyPressEvent(QKeyEvent *event)
{
//    setFocus();

    if (event->key() == Qt::Key_R) {
        yRot = 720;
        xRot = 480;
        zRot = 0;

        xPos = yPos = 0;
        zoomScale = 0.05;

        setXRotation(xRot);
        setYRotation(yRot);
        setZRotation(zRot);
    }
    updateGL();
}


void GLWidget::setXRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}


void GLWidget::setYRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}


void GLWidget::setZRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}


//полученние данных с файла и отрисовка модели
void GLWidget::getStl(QFile* file)
{
    triangleBase.clear();
    int
        state       = 0,
        type        = 0,
        countCoords = 0;
    float points[3];
    triangle form;

    QTextStream in(file);
    QString text      = in.readAll();
    QStringList words = text.split(" ");
    file->close();

    for(int i = 0; i < words.size(); i++){
        QString word = words[i];

        if(word != ""){
            if(state == 1){
                countCoords--;
                points[countCoords] = word.toFloat();

                if(!countCoords){
                  if(type == 1){
                      form.Normal.X = points[2];
                      form.Normal.Y = points[1];
                      form.Normal.Z = points[0];
                      triangleBase.push_back(form);
                      qDebug() << triangleBase[triangleBase.size() - 1].p[type - 2].X;
                      qDebug() << triangleBase[triangleBase.size() - 1].p[type - 2].X;
                      qDebug() << triangleBase[triangleBase.size() - 1].p[type - 2].X;
                  }else if(type > 1){
                      triangleBase[triangleBase.size() - 1].p[type - 2].X = points[2];
                      triangleBase[triangleBase.size() - 1].p[type - 2].Y = points[1];
                      triangleBase[triangleBase.size() - 1].p[type - 2].Z = points[0];
                  }
                  state = 0;
                }
            }else if(state == 0){
                if(word == "normal"){
                  state       = 1;
                  countCoords = 3;
                  type        = 1;
                }else if(word == "vertex"){
                  state       = 1;
                  countCoords = 3;
                  type        = type + 1;
                }
            }
        }
    }

    findGabarite();

    qDebug() << triangleBase.size();

    updateGL();
}

void GLWidget::toggleWireframe(bool show)
{
    wireframe(show);
}


void GLWidget::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}

void GLWidget::wireframe(bool show)
{
    if (show) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    update();
}

void GLWidget::findGabarite()
{
    float minX=triangleBase[0].p[0].X;
    float maxX=triangleBase[0].p[0].X;
    float minY=triangleBase[0].p[0].Y;
    float maxY=triangleBase[0].p[0].Y;
    float minZ=triangleBase[0].p[0].Z;
    float maxZ=triangleBase[0].p[0].Z;

    for(int i=0;i<triangleBase.size();i++){
        for (int j=0;j<3;j++){
            if (triangleBase[i].p[j].X>maxX) maxX=triangleBase[i].p[j].X;
            if (triangleBase[i].p[j].Y>maxY) maxY=triangleBase[i].p[j].Y;
            if (triangleBase[i].p[j].Z>maxZ) maxZ=triangleBase[i].p[j].Z;
            if (triangleBase[i].p[j].X<minX) minX=triangleBase[i].p[j].X;
            if (triangleBase[i].p[j].Y<minY) minY=triangleBase[i].p[j].Y;
            if (triangleBase[i].p[j].Z<minZ) minZ=triangleBase[i].p[j].Z;
        }
    }

    GabariteMaxX=maxX;
    GabariteMinX=minX;
    GabariteMaxY=maxY;
    GabariteMinY=minY;
    GabariteMaxZ=maxZ;
    GabariteMinZ=minZ;
}


void GLWidget::sliceAuto()
{


    ///Подготовка к слайсингу и определение параметров слайсинга
    if (OutLineSeparation.size()>0){
//        cout<<"Count vertex = "<<OutLineSeparation.size()<<endl;
    }

    ///Отчистка слоев и запоминание текущей высоты слайсинга
    float tempSliceHight = SlicerHeight;
    QVector <point> tempLoop;
    QVector <int> tempLoopID;

    OutLineLoop.clear();
    OutLineLoopID.clear();

    ///Цикл слайсинга с использованием базовой фунции слайсинга слоя
    for (float height=LayerHeight/2;height<GabariteMaxZ;height+=LayerHeight){
        SlicerHeight=height;

        //findSeparatePoint
        int vert2;
        pointSeparation.clear();
        pointSeparationID.clear();
        ///Основной цикл прохода по всем треугольниками модели
        for(int trianN=0;trianN<triangleBase.size();trianN++){
            ///Преобразование с определением к какой грани относится та или иная точка на трегуольнике
            for (int vert1=0;vert1<3;vert1++){
                if (vert1==0) vert2=1;
                if (vert1==1) vert2=2;
                if (vert1==2) vert2=0;
                ///Оплетка для проверки факта парарельности треугольника секущей плоскости
                if (triangleBase[trianN].p[vert1].Z==SlicerHeight) {

                    pointSeparation.clear();
                    pointSeparationID.clear();
                    return;
                }

                ///Определение факта того что плоскость пересекает треугольник (она должна пересекать точно 2 грани, не может быть 1 или 3)
                if ((triangleBase[trianN].p[vert1].Z<SlicerHeight && triangleBase[trianN].p[vert2].Z>SlicerHeight)||
                    (triangleBase[trianN].p[vert1].Z>SlicerHeight && triangleBase[trianN].p[vert2].Z<SlicerHeight)){
                        float difX=triangleBase[trianN].p[vert2].X-triangleBase[trianN].p[vert1].X;
                        float difY=triangleBase[trianN].p[vert2].Y-triangleBase[trianN].p[vert1].Y;
                        float difZ=triangleBase[trianN].p[vert2].Z-triangleBase[trianN].p[vert1].Z;
                        float t=(SlicerHeight-triangleBase[trianN].p[vert1].Z)/(difZ);
                        if (pointSeparationID.size()>0){
                            if (pointSeparationID[pointSeparationID.size()-1]==trianN &&
                                (fabs(pointSeparation[pointSeparation.size()-1].X-(triangleBase[trianN].p[vert1].X+difX*t)))<0.001 &&
                                (fabs(pointSeparation[pointSeparation.size()-1].Y-(triangleBase[trianN].p[vert1].Y+difY*t)))<0.001){
                                    pointSeparation.pop_back();
                                    pointSeparationID.pop_back();
                                    continue;
                            }
                        }
                        point temp;
                        temp.X=triangleBase[trianN].p[vert1].X+difX*t;
                        temp.Y=triangleBase[trianN].p[vert1].Y+difY*t;
                        temp.Z=triangleBase[trianN].p[vert1].Z+difZ*t;
                        pointSeparation.push_back(temp);
                        pointSeparationID.push_back(trianN);
                }
            }
        }

        //fingSeparatePoint
        int vert22;
        pointSeparation.clear();
        pointSeparationID.clear();
        ///Основной цикл прохода по всем треугольниками модели
        for(int trianN=0;trianN<triangleBase.size();trianN++){
            ///Преобразование с определением к какой грани относится та или иная точка на трегуольнике
            for (int vert1=0; vert1<3; vert1++){
                if (vert1==0) vert22=1;
                if (vert1==1) vert22=2;
                if (vert1==2) vert22=0;
                ///Оплетка для проверки факта парарельности треугольника секущей плоскости
                if (triangleBase[trianN].p[vert1].Z==SlicerHeight) {

                    pointSeparation.clear();
                    pointSeparationID.clear();
                    return;
                }

                ///Определение факта того что плоскость пересекает треугольник (она должна пересекать точно 2 грани, не может быть 1 или 3)
                if ((triangleBase[trianN].p[vert1].Z<SlicerHeight && triangleBase[trianN].p[vert22].Z>SlicerHeight)||
                    (triangleBase[trianN].p[vert1].Z>SlicerHeight && triangleBase[trianN].p[vert22].Z<SlicerHeight)){
                        float difX=triangleBase[trianN].p[vert22].X-triangleBase[trianN].p[vert1].X;
                        float difY=triangleBase[trianN].p[vert22].Y-triangleBase[trianN].p[vert1].Y;
                        float difZ=triangleBase[trianN].p[vert22].Z-triangleBase[trianN].p[vert1].Z;
                        float t=(SlicerHeight-triangleBase[trianN].p[vert1].Z)/(difZ);
                        if (pointSeparationID.size()>0){
                            if (pointSeparationID[pointSeparationID.size()-1]==trianN &&
                                (fabs(pointSeparation[pointSeparation.size()-1].X-(triangleBase[trianN].p[vert1].X+difX*t)))<0.001 &&
                                (fabs(pointSeparation[pointSeparation.size()-1].Y-(triangleBase[trianN].p[vert1].Y+difY*t)))<0.001){
                                    pointSeparationID.pop_back();
                                    continue;
                            }

                        }
                        point temp;
                        temp.X=triangleBase[trianN].p[vert1].X+difX*t;
                        temp.Y=triangleBase[trianN].p[vert1].Y+difY*t;
                        temp.Z=triangleBase[trianN].p[vert1].Z+difZ*t;
                        pointSeparation.push_back(temp);
                        pointSeparationID.push_back(trianN);
                }
            }
        }


        //findSeparateLayerOutline
        OutLineSeparation.clear();
        OutLineSeparationID.clear();
        point tempPoint;
        int tempID;
        int thisIDLine;
        tempPoint.X=pointSeparation[0].X;
        tempPoint.Y=pointSeparation[0].Y;
        tempPoint.Z=pointSeparation[0].Z;
        tempID=0;
        thisIDLine=0;
        OutLineSeparation.push_back(tempPoint);
        OutLineSeparationID.push_back(thisIDLine);

        ///Бесконечный цикл прохода по всем точкам пересечения плоскости с моделью
        while (pointSeparationID.size()>0){
            for(int i=0;i<pointSeparation.size();i++){


                if (fabs(pointSeparation[i].X-tempPoint.X)<0.001 &&
                    fabs(pointSeparation[i].Y-tempPoint.Y)<0.001 &&
                    fabs(pointSeparation[i].Z-tempPoint.Z)<0.001){
                        for (int j=0;j<pointSeparation.size();j++){
                            if ((pointSeparation[j].X!=pointSeparation[i].X ||
                                pointSeparation[j].Y!=pointSeparation[i].Y ||
                                pointSeparation[j].Z!=pointSeparation[i].Z) &&
                                pointSeparationID[j]==pointSeparationID[i]){
                                    tempPoint.X=pointSeparation[j].X;
                                    tempPoint.Y=pointSeparation[j].Y;
                                    tempPoint.Z=pointSeparation[j].Z;
                                    OutLineSeparation.push_back(tempPoint);
                                    OutLineSeparationID.push_back(thisIDLine);
                                    tempID=pointSeparationID[i];

                                    ///Фильтрация получившегося констра на совпадение ID
                                    for(int k=0;k<pointSeparationID.size();k++){
                                        if (pointSeparationID[k]==tempID){
                                            pointSeparation.erase(pointSeparation.begin()+k,pointSeparation.begin()+k+1);
                                            pointSeparationID.erase(pointSeparationID.begin()+k,pointSeparationID.begin()+k+1);
                                            break;
                                        }
                                    }
                                    for(int k=0;k<pointSeparationID.size();k++){
                                        if (pointSeparationID[k]==tempID){
                                            pointSeparation.erase(pointSeparation.begin()+k,pointSeparation.begin()+k+1);
                                            pointSeparationID.erase(pointSeparationID.begin()+k,pointSeparationID.begin()+k+1);
                                            break;
                                        }
                                    }
                                    qDebug () << "NewTemp X=" << tempPoint.X;
                                    break;
                            }
                        }
                        break;
                }
                ///Оплетка поиска количества контуров
                if (i==pointSeparation.size()-1){
                    tempPoint.X=pointSeparation[0].X;
                    tempPoint.Y=pointSeparation[0].Y;
                    tempPoint.Z=pointSeparation[0].Z;
                    tempID=0;
                    OutLineSeparation.push_back(tempPoint);
                    OutLineSeparationID.push_back(thisIDLine+1);
                    thisIDLine++;
                }
            }
        }

        ///Дополнительная фильтрация массива контура для удаления лишних вершин в узле которой линия не изгибается
        int thisPointStartLine=0;
        while (thisPointStartLine<OutLineSeparation.size()-2) {
            if (OutLineSeparationID[thisPointStartLine]!=OutLineSeparationID[thisPointStartLine+2]) {
                thisPointStartLine+=2;
            }
            float dx1=fabs(OutLineSeparation[thisPointStartLine].X-OutLineSeparation[thisPointStartLine+1].X);
            float dy1=fabs(OutLineSeparation[thisPointStartLine].Y-OutLineSeparation[thisPointStartLine+1].Y);
            float dx2=fabs(OutLineSeparation[thisPointStartLine+1].X-OutLineSeparation[thisPointStartLine+2].X);
            float dy2=fabs(OutLineSeparation[thisPointStartLine+1].Y-OutLineSeparation[thisPointStartLine+2].Y);
            if ((dx1/dy1)==(dx2/dy2)){
                OutLineSeparation[thisPointStartLine+1].X=OutLineSeparation[thisPointStartLine+2].X;
                OutLineSeparation[thisPointStartLine+1].Y=OutLineSeparation[thisPointStartLine+2].Y;
                OutLineSeparation[thisPointStartLine+1].Z=OutLineSeparation[thisPointStartLine+2].Z;
                OutLineSeparation.erase(OutLineSeparation.begin()+thisPointStartLine+2,OutLineSeparation.begin()+thisPointStartLine+3);
                OutLineSeparationID.erase(OutLineSeparationID.begin()+thisPointStartLine+2,OutLineSeparationID.begin()+thisPointStartLine+3);
                thisPointStartLine=0;
            }
            else thisPointStartLine+=1;
        }

        offsetUpForLinePerimetr=OutLineSeparation.size();
        countLoops=thisIDLine+1;

        tempLoop=OutLineSeparation;
        tempLoopID=OutLineSeparationID;
        OutLineLoop.push_back(tempLoop);
        OutLineLoopID.push_back(tempLoopID);
    }

    OutLineSeparation.clear();
    OutLineSeparationID.clear();

    SlicerHeight=tempSliceHight;
    update();

}
