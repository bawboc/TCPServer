#include "tcpconnection.h"

TcpConnection::TcpConnection(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Created";
}

TcpConnection::~TcpConnection()
{
    qDebug() << this << "Destroyed";
}

void TcpConnection::setSocket(QTcpSocket *socket)
{
    m_socket = socket;
    connect(m_socket,&QTcpSocket::connected,this,&TcpConnection::connected);
    connect(m_socket,&QTcpSocket::disconnected,this,&TcpConnection::disconnected);
    connect(m_socket,&QTcpSocket::readyRead,this,&TcpConnection::readyRead);
    connect(m_socket,&QTcpSocket::bytesWritten,this,&TcpConnection::bytesWritten);
    connect(m_socket,&QTcpSocket::stateChanged,this,&TcpConnection::stateChanged);
    connect(m_socket,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&TcpConnection::error);
}

QTcpSocket *TcpConnection::getSocket()
{
    if  (!sender()) return 0;
    return static_cast<QTcpSocket*>(sender());
}

void TcpConnection::connected()
{
    if  (!sender()) return;
    qDebug() << this << " conected " << sender();
}

void TcpConnection::disconnected()
{
    if  (!sender()) return;
    qDebug() << this << " disconnected " << getSocket();
}

void TcpConnection::readyRead()
{
    if  (!sender()) return;
    qDebug() << this << " readyRead " << getSocket();
}

void TcpConnection::bytesWritten(qint64 bytes)
{
    if  (!sender()) return;
    qDebug() << this << " BytedWritten " << getSocket() << " number of bytes =  " << bytes;
}

void TcpConnection::stateChanged(QAbstractSocket::SocketState SocketState)
{
    if  (!sender()) return;
    qDebug() << this << " StateChanged " << getSocket() << " state =  " << SocketState;
}

void TcpConnection::error(QAbstractSocket::SocketError SocketError)
{
    if  (!sender()) return;
    qDebug() << this << " Error " << getSocket() << " error =  " << SocketError;
}
