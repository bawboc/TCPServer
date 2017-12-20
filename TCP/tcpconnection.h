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

    virtual void SetSocket(QTcpSocket* socket);

protected:
    QTcpSocket* m_socket;
    QTcpSocket* GetSocket();

signals:

public slots:
    virtual void Connected();
    virtual void Disconnected();
    virtual void ReadyRead();
    virtual void BytesWritten(qint64 bytes);
    virtual void StateChanged(QAbstractSocket::SocketState SocketState);
    virtual void Error(QAbstractSocket::SocketError SocketError);
};

#endif // TCPCONNECTION_H
