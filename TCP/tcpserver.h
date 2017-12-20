#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QThread>

#include "tcpconnections.h"
#include "tcpconnection.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();

    virtual bool   listen(const QHostAddress& Address, quint16 port);
    virtual void   close();
    virtual qint64 port();

protected:
    QThread* m_thread;
    TcpConnections* m_connections;
    virtual void incomingConnection(qintptr Descriptor);  // qint64, qHandle, uint could also work
    virtual void accept(qintptr Descriptor, TcpConnection* Connection);

signals:
    void accepting(qintptr Handle, TcpConnection* Connection);
    void finished();

public slots:
    void complete();
};

#endif // TCPSERVER_H
