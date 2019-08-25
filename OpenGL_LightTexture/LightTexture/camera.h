#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "vertices.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QWheelEvent>
#include <QVector3D>
#include <QPoint>
#include <QDebug>

#define PI                   ((float)3.1415926)
#define radian(x)            (x*PI/180.0)
#define angle(x)             (x*180.0/PI)
#define STATE_NUMBER         100
#define cameraSpeed          ((float)0.01)
#define cameraSensitivity    ((float)0.05)

class Camera : public QObject
{
    Q_OBJECT
public:
    Camera(QObject *parent = 0, QVector3D eye = QVector3D(), QVector3D center = QVector3D()
            , QVector3D up = QVector3D());
    enum KeyState{
        FORWARD = 0,
        BACKWARD,
        MOVELEFT,
        MOVERIGHT,
        MOVEUP,
        MOVEDOWN,
        SPACE,
        MOUSEHOLD
    };
    enum Direction{
        F_X,F_Y,F_Z
    };

    void SetCameraPos(QVector3D eye);
    void SetCameraFront(QVector3D front);
    void SetCameraUp(QVector3D up);
    void SetDeltatime(int elapsed);
    QVector3D CameraPos();
    QVector3D CameraFront();
    QVector3D CameraUp();
    void domovement();
    void dorotate();
    float doscale();
protected:
    bool eventFilter(QObject *watched, QEvent *ev);
signals:

public slots:
private:
    bool state[STATE_NUMBER];
    QVector3D cameraPos,cameraFront,cameraUp;
    int deltatime = 0;
    QPoint lastmousePos,currentmousePos;
    float pitch,yaw,viewangle;//俯仰角，偏航角
};

#endif // CAMERA_H
