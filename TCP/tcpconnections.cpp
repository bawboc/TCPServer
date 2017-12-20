#include "tcpconnections.h"

TcpConnections::TcpConnections(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Created";
}

TcpConnections::~TcpConnections()
{
    qDebug() << this << "Destroyed";
}

int TcpConnections::Count()
{
    QReadWriteLock lock;
    lock.lockForRead();
    int value = m_connections.count();
    lock.unlock();
    return value;
}

void TcpConnections::RemoveSocket(QTcpSocket *socket)
{
    if  (!socket) return;
    if  (!m_connections.contains(socket)) return;

    qDebug() << this << "Removing Socket = " << socket;

    if  (socket->isOpen()){
        qDebug() << this << "Attempting to close socket " << socket;
        socket->disconnect();
        socket->close();
    }
    qDebug() << this << "Deleting socket " << socket;
    m_connections.remove(socket);
    socket->deleteLater();

    qDebug() << this << "Clients still connected = " << Count();
}

void TcpConnections::Disconnected()
{
    if  (!sender()) return;
    qDebug() << this << "Disconnecting socket" << sender();

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    if  (!socket) return;

    RemoveSocket(socket);
}

void TcpConnections::Error(QAbstractSocket::SocketError socketError)
{
    if  (!sender()) return;
    qDebug() << this << "Error in socket " << sender() << " errir " << socketError;

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    if  (!socket) return;

    RemoveSocket(socket);
}

void TcpConnections::Start()
{
    qDebug() << this << "Starting " << QThread::currentThread();
}

void TcpConnections::Quit()
{
    if  (!sender()) return;
    qDebug() << this << "Quitting";

    foreach (QTcpSocket* socket, m_connections.keys()) {
        qDebug() << this << "Closing socket" << socket;
        RemoveSocket(socket);
    }

    qDebug() << this << "Finishing quit";
    emit Finished();
}

void TcpConnections::Accept(qintptr handle, TcpConnection *connection)
{
    QTcpSocket* socket = new QTcpSocket(this);

    if  (!socket->setSocketDescriptor(handle)){
        qWarning() << this << " Could not accept connection " << handle;
        connection->deleteLater();
        return;
    }
    connect(socket, &QTcpSocket::disconnected,this,&TcpConnections::Disconnected);
    connect(socket,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&TcpConnections::Error);

    qDebug() << this << "Clients = " << Count();
    emit socket->connected();
}
