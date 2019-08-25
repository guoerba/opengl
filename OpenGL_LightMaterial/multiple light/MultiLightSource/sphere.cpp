#include "sphere.h"

Sphere::Sphere(int depth, QObject *parent)
    : QObject(parent)
    , N(depth)
{

}

int Sphere::getStride()
{
    int stride = 1;
    for(int i=0;i<N;i++)
        stride *= 2;
    return stride;
}

int Sphere::accelerate(int num)
{
    for(int i=num-1;i>0;i--)
        num += i;
    return num;
}

QMap<int,QList<QVector3D> > Sphere::getVertices()
{
    int stride = getStride();
    QVector3D vector_YZ = axis_Z - axis_Y;
    QVector3D vector_YX = axis_X - axis_Y;

    QList<QVector3D> YZ;
    for(int i=0;i<stride+1;i++)
    {
        double scale = ((double)i)/((double)stride);
        YZ.insert(i,((float)scale)*vector_YZ);
    }

    QList<QVector3D> YX;
    for(int i=0;i<stride+1;i++)
    {
        double scale = ((double)i)/((double)stride);
        YX.insert(i,((float)scale)*vector_YX);
    }

    QMap<int,QList<QVector3D> > vertices;
    for(int i=0;i<stride+1;i++)
    {
        for(int j=0;j<stride+1-i;j++)
        {
            vertices[i].insert(j,(YX[i] + YZ[j] + axis_Y).normalized());
        }
    }

    return vertices;
}

QVector<int> Sphere::outputOrder()
{
    return completeVerticesOrder;
}

QVector<float> Sphere::outputVertices()
{
    return completeVertices;
}

void Sphere::GetSphere()
{
    QMap<int,QList<QVector3D> > vertices = getVertices();
    completeVertices = getAllVertex(vertices).toVector();
    completeVerticesOrder = getOrder(vertices).toVector();
}

int Sphere::outputOrderLen()
{
    return (completeVerticesOrder.size())*sizeof(float);
}

int Sphere::outputVerticesLen()
{
    return (completeVertices.size())*sizeof(int);
}

QList<int> Sphere::getOrder(QMap<int, QList<QVector3D> > vertices)
{
    int maxsize = vertices.values().at(0).size();
    //int total = accelerate(maxsize);
    QMap<int,QList<int> > number;
    int count = 0;
    for(int i=0;i<maxsize;i++)
    {
        for(int j=0;j<maxsize-i;j++)
        {
         //   qDebug() << tr("(i,j) = (%1,%2)").arg(i).arg(j);
            number[i].insert(j,count);
            count++;
        }
    }

    int size = number.size();
    QList<int> order;
    for(int i=0;i<size;i++)
    {
        int listsize = number[i].size();
        for(int j=0;j<listsize;j++)
        {
            if(i < size-1)
            {
                if(j==0 && j<listsize-1)
                {
                   /* qDebug() << tr("(%1,%2)").arg(i).arg(j) << number[i].at(j);
                    qDebug() << tr("(%1,%2)").arg(i+1).arg(j) << number[i+1].at(j);
                    qDebug() << tr("(%1,%2)").arg(i).arg(j+1) << number[i].at(j+1);*/
                    order.append(number[i].at(j));
                    order.append(number[i+1].at(j));
                    order.append(number[i].at(j+1));
                }
                else if(j!=0 && j<listsize-1)
                {
                    order.append(number[i].at(j));
                    order.append(number[i+1].at(j-1));
                    order.append(number[i+1].at(j));

                    order.append(number[i].at(j));
                    order.append(number[i+1].at(j));
                    order.append(number[i].at(j+1));
                }
            }
        }
    }
 //   qDebug() << "order: " << order;
    int ordersize = order.size();
    qDebug() << "ordersize: " << ordersize;
    for(int i=1;i<8;i++)
    {
        for(int j=0;j<ordersize;j++)
        {
            int atom = order.at(j);
            order.append(atom + i*(completeVertices.size()/24));
        }
    }
 //   qDebug() << "order: " << order;

    return order;
}

QList<float> Sphere::getAllVertex(QMap<int ,QList<QVector3D> > vertices)
{
    int size = vertices.values().size();
    QList<float> allvertices;
    for(int n=0;n<8;n++)
    {
        for(int i=0;i<size;i++)
        {
            QList<QVector3D> temp = vertices.value(i);
            int tempsize = temp.size();
            for(int j=0;j<tempsize;j++)
            {
                float x = temp.at(j).x();
                float y = temp.at(j).y();
                float z = temp.at(j).z();
                switch(n)
                {
                case 1:
                    z = -z;
                    break;
                case 2:
                    x = -x;
                    z = -z;
                    break;
                case 3:
                    x = -x;
                    break;
                case 4:
                    y = -y;
                    break;
                case 5:
                    y = -y;
                    z = -z;
                    break;
                case 6:
                    x = -x;
                    y = -y;
                    z = -z;
                    break;
                case 7:
                    x = -x;
                    y = -y;
                    break;
                }          
               allvertices.append(x);
               allvertices.append(y);
               allvertices.append(z);
            }
        }
    }
  //  qDebug() << "final result: " << allvertices;


    return allvertices;
}
