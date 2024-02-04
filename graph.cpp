#include "graph.h"
#include <QTime>
#include <algorithm>
#include <iostream>

Graph::Graph(QQuickItem *parent)
    : QQuickPaintedItem(parent),
    watchers(std::vector<QFutureWatcher<int>>(QThread::idealThreadCount() - 1))
{
    connect(this, &Graph::heightChanged, this, &Graph::contentsSizeChangedHandle);
    connect(this, &Graph::widthChanged, this, &Graph::contentsSizeChangedHandle);
    // // Заполняем данные (замените на свои данные)
    // for (int i = 0; i < 20'000'000; ++i) {
    //     points.push_back(QPointF(i / 1'000, 300 + 10 * qSin(i * 0.01)));
    //     points1.push_back(QPointF(i / 1'000, (i % 2 == 0) ? 300 : i < 8'000'000 ? 350 : 400));
    //     points2.push_back(QPointF(i / 1'000, (i % 3 == 0) ? 100 : i < 8'000'000 ? 150 : 200));
    // }


    // int tt = 20'000'000 / m.size();
    // for (int i = 0; i < m.size(); ++i)
    // {
    //     auto t1 = m.data() + i;
    //     QFuture<int>_future = QtConcurrent::run([this, tt](int x){
    //         auto t2 = points1.begin();
    //         std::advance(t2, (x + 1) * tt);
    //         auto t3 = points.begin();
    //         std::advance(t3, x * tt);
    //         std::copy(points1.begin() + x * tt, t2, t3);

    //         return x * tt;
    //     }, i);
    //     t1->setFuture(_future);

    //     connect (t1, &QFutureWatcher<void>::finished, this, [t1]()
    //             {
    //                 qInfo() << "AAA";
    //                 auto _brands = t1->result();
    //                 qDebug() << "Brands" << _brands;
    //             });
    // }
}

// void Graph::onHeightChanged() {
//     points.clear();
//     update();
// }

void Graph::paint(QPainter *painter)
{
    // Рисуем координатные оси
    painter->drawLine(150, height() - 50, width() - 50, height() - 50);  // Ось X
    painter->drawLine(150, height() - 50, 150, 50);  // Ось Y

    double yDel { (height() - 100) / 10 };
    for (int i {0}; i < 11; ++i)
    {
        painter->drawText(QRectF(50 - 32,
                                 height() - 50 - 8 - i * yDel,
                                 150 - 12,
                                 height() - 50 + 4 - i * yDel), QString::number(double(yMin + kY / (height() - 100) * yDel*i), 'e', 8));
    }

    double xDel { (width() - 200) / 10 };
    for (int i {0}; i < 11; ++i)
    {
        painter->drawText(QRectF(150 + i*xDel - 24,
                                 height() - 50 + 12,
                                 150 + i*xDel,
                                 height() - 50 + 20), QString::number(double(xMin + kX / (width() - 200) * xDel*i), 'e', 2));
    }

    // Рисуем миллионы точек
    painter->setPen(QPen(Qt::blue, 3));  // Устанавливаем цвет и толщину линии
    painter->drawPoints(points.data(), points.size());

    painter->setPen(QPen(Qt::blue, 1));  // Устанавливаем цвет и толщину линии
    painter->drawPolyline(points.data(), points.size());
}

void Graph::contentsSizeChangedHandle()
{
    if (points.size() > 0)
    {
        points.clear();
        update();
    }
}

void Graph::updateData(QSharedPointer<QVector<QPointF>> _points)
{
    if (!_points)
        return;

    points.resize(_points->size());
    // _pointsTmp = *_points;
    // points.swap(*(_points));
    qInfo() << "FIN";
    // update();

    for (int i = 0; i < watchers.size(); ++i)
    {
        // auto t1 = m.data() + i;
        QFuture<int>_future = QtConcurrent::run([this, _points](int x) {
            if (!_points || _points->size() == 0)
            {
                qInfo() << "NO";
                return -1;
            }
            int tt = _points->size() / watchers.size();
            qInfo() << "Thread points" << tt;


            // QVector<QPointF>::iterator startOt = _pointsTmp.begin();
            // std::advance(startOt, x * tt);
            // QVector<QPointF>::iterator startDo = _pointsTmp.begin();
            // if (x < watchers.size() - 1)
            //     std::advance(startDo, (x + 1) * tt - 1);
            // else
            //     startDo = _pointsTmp.end() - 1;



            int num = x * tt;
            int nDo = x < watchers.size() - 1 ? (x + 1) * tt : _points->size();
            // QVector<QPointF>* pPoints = &points;
            //std::for_each(startOt, startDo, [num, this](QPointF _point11) mutable {
            for (int ik {num}; ik < nDo; ++ik) {
    // //             // points[ik] = QPointF(ik++, 100);
    // // //         //     // long double dXPer = (_point11.x() - xMin) / kX * 100.0d;
    // // //         //     // qInfo() << "Percent X: " << dXPer;

    // // //         //     long double integralPart;
    // //             // double dYPer = (_point11.y() - yMin) / kY  * height();
    // //             // double dXPer = (_point11.x() - xMin) / kX  * width() + 50;
    // //             // qInfo() << "Curr point: " << _point11.x() << "   -   " << _point11.y();
                // QPointF& t = (*pPoints)[num];
                auto& t1 {points[ik]};

                // for (auto& el : resultPoints)
                // {
                double dYPer = height() - 50 - (_points->at(ik).y() - yMin) / kY  * (height() - 100);
                double dXPer = (_points->at(ik).x() - xMin) / kX  * (width() - 200) + 150;

                    t1.setX(dXPer);
                    t1.setY(dYPer);
                // }


                // t1.setY(300 - _points->at(ik).y());
                // t1.setX(_points->at(ik).x());
            }
                // points[num].setY(_point11.y());
                // ++num;
    // // //             // qInfo() << "Val: " << _point11.y();
    // // //             // qInfo() << "Val min: " << double(yMin);


    // // //         //     // long double myDouble {_maxMinXYPair.first.x()};
    // // //         //     // long double fractionalPart = std::modf(myDouble, &fractionalPart);

    // // //         //     // std::cout << "Исходное число: " << myDouble << std::endl;
    // // //         //     // std::cout << "Целая часть: " << integerPart << std::endl;
    // // //         //     // std::cout << "Дробная часть: " << fractionalPart << std::endl;

    // //             // _point11.setX(dXPer);
    // //             // _point11.setY(dYPer);

            // });



    // // //         // auto t2 = _pointsPL.begin();
    // // //         // std::advance(t2, (x + 1) * tt);
    // // //         // auto t3 = points.begin();
    // // //         // std::advance(t3, x * tt);
    // // //         // std::copy(_pointsPL.begin() + x * tt, t2, t3);

    // //         // update();
            return x * tt;
        }, i);
        QFutureWatcher<int>* t1 = &watchers[i];
    //     // qInfo() << "VOT";
    //     // t1->cancel();
        t1->setFuture(_future);


        connect (t1, &QFutureWatcher<void>::finished, this, [this, t1]()
                {
                    qInfo() << "AAA";
                    auto _brands = t1->result();
                    qDebug() << "Brands" << _brands;
                    // points.swap(_pointsPL);
                    this->update();
                });
    }
    // std::for_each(_pointsPL.begin(), _pointsPL.end(), [this](QPointF& _point11){
    //     // long double dXPer = (_point11.x() - xMin) / kX * 100.0d;
    //     // qInfo() << "Percent X: " << dXPer;

    //     long double integralPart;
    //     long double dYPer = (_point11.y() - yMin) / kY  * height();
    //     long double dXPer = (_point11.x() - xMin) / kX  * width() + 50;
    //     // qInfo() << "Percent Y: " << double(dYPer);
    //     // qInfo() << "Val: " << double(std::modf(_point11.y(), &integralPart));
    //     // qInfo() << "Val min: " << double(std::modf(yMin, &integralPart));


    //     // long double myDouble {_maxMinXYPair.first.x()};
    //     // long double fractionalPart = std::modf(myDouble, &fractionalPart);

    //     // std::cout << "Исходное число: " << myDouble << std::endl;
    //     // std::cout << "Целая часть: " << integerPart << std::endl;
    //     // std::cout << "Дробная часть: " << fractionalPart << std::endl;

    //     _point11.setX(dXPer);
    //     _point11.setY(dYPer);

    // });
    // qInfo() << "VEC: " << _pointsPL;
    // points.swap(_pointsPL);

    // int t = 1;
    // int tt = _pointsPL.size() / m.size();
    // for (int i = 0; i < m.size() - 1; ++i)
    // {
    //     auto t1 = m.data() + i;
    //     QFuture<int>_future = QtConcurrent::run([this, tt](int x){
    //         auto t2 = _pointsPL.begin();
    //         std::advance(t2, (x + 1) * tt);
    //         auto t3 = points.begin();
    //         std::advance(t3, x * tt);
    //         std::copy(_pointsPL.begin() + x * tt, t2, t3);


    //         return x * tt;
    //     }, i);
    //     t1->setFuture(_future);

    //     connect (t1, &QFutureWatcher<void>::finished, this, [this, t1]()
    //             {
    //                 qInfo() << "AAA";
    //                 auto _brands = t1->result();
    //                 qDebug() << "Brands" << _brands;
    //                 update();
    //             });
    // }
    // Выполняем обновление данных (например, изменяем координаты точек)
    // for (int i = 0; i < 100; ++i) {
    //     points[i].setY(300 + 100 * qSin(i * 0.01 + QTime::currentTime().msecsSinceStartOfDay() / 1000.0));
    // }
    // static bool i = true;
    // if (i)
    // {
    //     std::copy(points1.begin(), points1.end(), points.begin());
    //     i = false;
    // }
    // else
    // {
    //     std::copy(points2.begin(), points2.end(), points.begin());
    //     i = true;
    // }

    // points.swap(points1);

    // Вызываем перерисовку элемента
    qInfo() << "GOGOGO";
    // update();
}

void Graph::setBorders(const QPair<QPointF, QPointF> &_maxMinXYPair)
{
    long double integralPart;
    qInfo() << _maxMinXYPair;
    qInfo() << double(std::modf(_maxMinXYPair.first.y(), &integralPart)) << double(std::modf(_maxMinXYPair.second.y(), &integralPart));
    // kX = width() / (_maxMinXYPair.first.x() - _maxMinXYPair.second.x());
    // kY = height() / (_maxMinXYPair.first.y() - _maxMinXYPair.second.y());

    kX = _maxMinXYPair.first.x() - _maxMinXYPair.second.x();
    kY = _maxMinXYPair.first.y() - _maxMinXYPair.second.y();

    // long double myDouble {_maxMinXYPair.first.x()};
    // long double fractionalPart = std::modf(myDouble, &fractionalPart);

    // std::cout << "Исходное число: " << myDouble << std::endl;
    // std::cout << "Целая часть: " << integerPart << std::endl;
    // std::cout << "Дробная часть: " << fractionalPart << std::endl;


    xMin = _maxMinXYPair.second.x();
    yMin = _maxMinXYPair.second.y();
    // qInfo() << "X_MIN" << xMin;
    // qInfo() << "Y_MIN" << yMin;
    // qInfo() << "HEIGHT: " << height();
    // qInfo() << "WIDTH: "  << width();
    // qInfo() << kX;
    // qInfo() << kY;
}
