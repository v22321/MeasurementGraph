#ifndef UIWRAPPER_H
#define UIWRAPPER_H

#include "graph/graphmodel.h"

#include <QString>
#include <QObject>
#include <QtCharts/QVXYModelMapper>
#include <QQmlContext>

class UIWrapper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList fileNames READ fileNames CONSTANT)
public:
    explicit UIWrapper (QObject *parent = nullptr);
    ~UIWrapper() {}

    void init(QQmlContext* _context);
    Q_INVOKABLE bool createGraph(const QString& _fileName = QString());

    QStringList fileNames();

private:
    QStringList m_filesList;
    QSharedPointer<GraphModel> m_graphData;
    QSharedPointer<QVXYModelMapper> m_seriesMapper;

signals:
    void s_hasError();
    void s_createGraph(const QString& _fileName);
};

#endif // UIWRAPPER_H
