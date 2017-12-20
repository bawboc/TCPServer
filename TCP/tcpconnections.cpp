#include "tcpconnections.h"

TcpConnections::TcpConnections(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Created";
}

TcpConnections::~TcpConnections()
{
    qDebug() << this << "Destroyed";
}

int TcpConnections::count()
{
    QReadWriteLock lock;
    lock.lockForRead();
    int value = m_connections.count();
    lock.unlock();
    return value;
}

void TcpConnections::removeSocket(QTcpSocket *socket)
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

    qDebug() << this << "Clients still connected = " << count();
}

void TcpConnections::disconnected()
{
    if  (!sender()) return;
    qDebug() << this << "Disconnecting socket" << sender();

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    if  (!socket) return;

    removeSocket(socket);
}

void TcpConnections::error(QAbstractSocket::SocketError socketError)
{
    if  (!sender()) return;
    qDebug() << this << "Error in socket " << sender() << " error " << socketError;

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    if  (!socket) return;

    removeSocket(socket);
}

void TcpConnections::start()
{
    qDebug() << this << "Starting " << QThread::currentThread();
}

void TcpConnections::quit()
{
    if  (!sender()) return;
    qDebug() << this << "Quitting";

    foreach (QTcpSocket* socket, m_connections.keys()) {
        qDebug() << this << "Closing socket" << socket;
        removeSocket(socket);
    }

    qDebug() << this << "Finishing quit";
    emit finished();
}

void TcpConnections::accept(qintptr handle, TcpConnection *connection)
{
    qDebug() << "***";
    QTcpSocket* socket = new QTcpSocket(this);

    if  (!socket->setSocketDescriptor(handle)){
        qWarning() << this << " Could not accept connection " << handle;
        connection->deleteLater();
        return;
    }
    connect(socket, &QTcpSocket::disconnected,this,&TcpConnections::disconnected);
    connect(socket,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&TcpConnections::error);

    connection->moveToThread(QThread::currentThread());
    connection->setSocket(socket);

    m_connections.insert(socket,connection);
    qDebug() << this << "Clients = " << count();
    emit socket->connected();
}
