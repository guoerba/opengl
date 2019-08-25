#include "camera.h"

Camera::Camera(QObject *parent, QVector3D eye, QVector3D center, QVector3D up)
    : QObject(parent)
    , cameraPos(eye)
    , cameraFront((center-eye).normalized())
    , cameraUp(up)
    , lastmousePos(QPoint())
    , currentmousePos(QPoint())
    , pitch(0)
    , yaw(0)
    , viewangle(0.0f)
{
    for(int i=0;i<STATE_NUMBER;i++)
        state[i] = false;
    parent->installEventFilter(this);//在Camera对象上安装过滤器来监视parent的事件
//根据摄像机位置向量(eye)和要观察的目标向量(center)设定俯仰角和偏航角的初相
    pitch += angle(asin(((double)cameraFront.y())));
    yaw += angle(atan(((double)cameraFront.z())/((double)cameraFront.x())));
}

bool Camera::eventFilter(QObject *watched, QEvent *ev)
{
 //   qDebug() << "watched:" << watched;
    if(watched == this->parent())
    {
     //   qDebug() << "event type:" << ev->type();
        if(ev->type() == QEvent::KeyPress)
        {
            QKeyEvent *event = static_cast<QKeyEvent*>(ev);
            switch(event->key())
            {
            case Qt::Key_W:
            case Qt::Key_Up:
                state[FORWARD] = true;
                break;
            case Qt::Key_S:
            case Qt::Key_Down:
                state[BACKWARD] = true;
                break;
            case Qt::Key_A:
            case Qt::Key_Left:
                state[MOVELEFT] = true;
                break;
            case Qt::Key_D:
            case Qt::Key_Right:
                state[MOVERIGHT] = true;
                break;
            case Qt::Key_Q:
                state[MOVEUP] = true;
                break;
            case Qt::Key_E:
                state[MOVEDOWN] = true;
                break;
            case Qt::Key_Space:
                state[SPACE] = true;
                break;
            }
        }
        else if(ev->type() == QEvent::KeyRelease)
        {
            QKeyEvent *event = static_cast<QKeyEvent*>(ev);
            switch(event->key())
            {
            case Qt::Key_W:
            case Qt::Key_Up:
                state[FORWARD] = false;
                break;
            case Qt::Key_S:
            case Qt::Key_Down:
                state[BACKWARD] = false;
                break;
            case Qt::Key_A:
            case Qt::Key_Left:
                state[MOVELEFT] = false;
                break;
            case Qt::Key_D:
            case Qt::Key_Right:
                state[MOVERIGHT] = false;
                break;
            case Qt::Key_Q:
                state[MOVEUP] = false;
                break;
            case Qt::Key_E:
                state[MOVEDOWN] = false;
                break;
            case Qt::Key_Space:
                state[SPACE] = false;
                break;
            }
        }
        else if(ev->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *event = static_cast<QMouseEvent*>(ev);
            if(event->button() == Qt::LeftButton)
            {
                qDebug() << "-----------------------------------------------";
                qDebug() << "cameraPos: " << cameraPos;
                qDebug() << "cameraFront: " << cameraFront;
                qDebug() << "objectvector: " << cameraPos - cameraFront;
                qDebug() << "cameraUp: " << cameraUp;
                qDebug() << "-----------------------------------------------";
            }
            if(event->button() == Qt::RightButton)//右键锁定视角
            {
                state[MOUSEHOLD] = true;
            }
        }
        else if(ev->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *event = static_cast<QMouseEvent*>(ev);
            if(event->button() == Qt::RightButton)
            {
                state[MOUSEHOLD] = false;
            }
        }
//pos：本地坐标(int);windowpos,localpos：本地坐标(float);screenpos：屏幕坐标(float);globalpos：屏幕坐标(int)
        else if(ev->type() == QEvent::MouseMove)
        {
            QMouseEvent *event = static_cast<QMouseEvent*>(ev);
            lastmousePos = currentmousePos;
            currentmousePos = event->pos();
            dorotate();
            /*qDebug() << "-----------------------------------------------";
            qDebug() << "lastmousePos: " << lastmousePos;
            qDebug() << "currentmousePos: " << currentmousePos;
            qDebug() << "-----------------------------------------------";*/
        }
        else if(ev->type() == QEvent::Enter)//鼠标进入窗口
        {
            QEnterEvent *event = static_cast<QEnterEvent*>(ev);
            lastmousePos = currentmousePos = event->pos();//防止出现一个很大的偏移量
        }
        else if(ev->type() == QEvent::Wheel)
        {
            QWheelEvent *event = static_cast<QWheelEvent*>(ev);
            viewangle -= ((float)(event->angleDelta().y()))/120.0f;
            qDebug() << "viewangle:" << viewangle;
            if(viewangle > 45.0f)
                viewangle = 45.0f;
            else if(viewangle < -44.0f)
                viewangle = -44.0f;
        }
    }
    return QObject::eventFilter(watched,ev);
}

QVector3D Camera::CameraPos()
{
    return cameraPos;
}

QVector3D Camera::CameraFront()
{
    return cameraFront;
}

QVector3D Camera::CameraUp()
{
    return cameraUp;
}

void Camera::SetCameraPos(QVector3D eye)
{
    cameraPos = eye;
}

void Camera::SetCameraFront(QVector3D front)
{
    cameraFront = front;
}

void Camera::SetCameraUp(QVector3D up)
{
    cameraUp = up;
}

void Camera::SetDeltatime(int elapsed)
{
   // qDebug() << "deltatime: " << elapsed;
    if(deltatime == 0)
        deltatime = 1;
    else
        deltatime = elapsed;
}

void Camera::domovement()
{
    if(state[FORWARD])
    {
        cameraPos += cameraFront.normalized()*cameraSpeed;//摄像机现在的位置等于摄像机以前的位置加上摄像机正向量
    }
    if(state[BACKWARD])
    {
        cameraPos -= cameraFront.normalized()*cameraSpeed;
    }
    if(state[MOVELEFT])
    {
        cameraPos -= QVector3D::crossProduct(cameraFront,cameraUp).normalized()*cameraSpeed;
    }
    if(state[MOVERIGHT])
    {
        cameraPos += QVector3D::crossProduct(cameraFront,cameraUp).normalized()*cameraSpeed;
    }
    if(state[MOVEUP])
    {
        QVector3D right = QVector3D::crossProduct(cameraFront,cameraUp).normalized();
        cameraPos += QVector3D::crossProduct(right,cameraFront).normalized()*cameraSpeed;
    }
    if(state[MOVEDOWN])
    {
        QVector3D right = QVector3D::crossProduct(cameraFront,cameraUp).normalized();
        cameraPos -= QVector3D::crossProduct(right,cameraFront).normalized()*cameraSpeed;
    }
    if(state[SPACE])
    {
        qDebug() << "________________________________";
        qDebug() << "cameraPos: " << cameraPos;
        qDebug() << "cameraFront: " << cameraFront;
        qDebug() << "cameraUp: " << cameraUp;
        qDebug() <<"_________________________________";
    }
}
//浮点数运算有误差，所以需要将这个函数放入鼠标事件中处理，以便与鼠标位置更新同步
void Camera::dorotate()
{
    float offsetX = 0;
    float offsetY = 0;

    if(!state[MOUSEHOLD])
    {
        offsetX = (float)((currentmousePos - lastmousePos).x());
        offsetY = (float)((lastmousePos - currentmousePos).y());
        offsetX *= cameraSensitivity;
        offsetY *= cameraSensitivity;
    }

    yaw += offsetX;//用鼠标偏移量控制偏航角
    pitch += offsetY;//用鼠标偏移量控制俯仰角
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    float direction[3];
    direction[F_X] = cos(radian(pitch))*cos(radian(yaw));
    direction[F_Y] = sin(radian(pitch));
    direction[F_Z] = cos(radian(pitch))*sin(radian(yaw));

    cameraFront = QVector3D(direction[F_X],direction[F_Y],direction[F_Z]).normalized();
}

float Camera::doscale()
{
    return viewangle;
}
