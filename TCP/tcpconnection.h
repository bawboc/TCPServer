#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>

class TcpConnection : public QObject
{
    Q_OBJECT
public:
    explicit TcpConnection(QObject *parent = nullptr);
    ~TcpConnection();

    virtual void setSocket(QTcpSocket* socket);

protected:
    QTcpSocket* m_socket;
    QTcpSocket* getSocket();

signals:

public slots:
    virtual void connected();
    virtual void disconnected();
    virtual void readyRead();
    virtual void bytesWritten(qint64 bytes);
    virtual void stateChanged(QAbstractSocket::SocketState SocketState);
    virtual void error(QAbstractSocket::SocketError SocketError);
};

#endif // TCPCONNECTION_H
