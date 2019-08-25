#ifndef SPHERE_H
#define SPHERE_H

#include <QObject>
#include <QVector3D>
#include <QVector>
#include <QList>
#include <QMap>
#include <QDebug>

const static QVector3D axis_X = QVector3D(1,0,0);
const static QVector3D axis_Y = QVector3D(0,1,0);
const static QVector3D axis_Z = QVector3D(0,0,1);

class Sphere : public QObject
{
    Q_OBJECT
public:
    explicit Sphere(int depth, QObject *parent = 0);

    void GetSphere();
    QVector<int> outputOrder();
    QVector<float> outputVertices();
    int outputOrderLen();
    int outputVerticesLen();
signals:
public slots:
private:
    int N;
    int getStride();
    int accelerate(int num);
    QVector<float> completeVertices;
    QVector<int> completeVerticesOrder;

    QMap<int, QList<QVector3D> > getVertices();
    QList<int> getOrder(QMap<int,QList<QVector3D>> vertices);
    QList<float> getAllVertex(QMap<int ,QList<QVector3D> > vertices);
};

#endif // SPHERE_H
