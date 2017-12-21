#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <QObject>

class HttpConnection : public QObject
{
    Q_OBJECT
public:
    explicit HttpConnection(QObject *parent = nullptr);

signals:

public slots:
};

#endif // HTTPCONNECTION_H