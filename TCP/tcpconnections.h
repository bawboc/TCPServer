#ifndef TCPCONNECTIONS_H
#define TCPCONNECTIONS_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTcpSocket>
#include <QMap>
#include <QReadWriteLock>
#include "tcpconnection.h"

class TcpConnections : public QObject
{
    Q_OBJECT
public:
    explicit TcpConnections(QObject *parent = nullptr);
    ~TcpConnections();

    /*
     * Number of clients
    */
    virtual int Count();

protected:
    QMap<QTcpSocket*, TcpConnection*> m_connections;
    void RemoveSocket(QTcpSocket* socket);

signals:
    void Quitting();
    void Finished();

protected slots:
    void Disconnected();
    void Error(QAbstractSocket::SocketError socketError);

public slots:

    void Start();
    void Quit();
    void Accept(qintptr handle, TcpConnection* connection);
};

#endif // TCPCONNECTIONS_H
