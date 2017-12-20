#include "tcpconnection.h"

TcpConnection::TcpConnection(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Created";
}

TcpConnection::~TcpConnection()
{
    qDebug() << this << "Destroyed";
}

void TcpConnection::SetSocket(QTcpSocket *socket)
{
    m_socket = socket;
    connect(m_socket,&QTcpSocket::connected,this,&TcpConnection::Connected);
    connect(m_socket,&QTcpSocket::disconnected,this,&TcpConnection::Disconnected);
    connect(m_socket,&QTcpSocket::readyRead,this,&TcpConnection::ReadyRead);
    connect(m_socket,&QTcpSocket::bytesWritten,this,&TcpConnection::BytesWritten);
    connect(m_socket,&QTcpSocket::stateChanged,this,&TcpConnection::StateChanged);
    connect(m_socket,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&TcpConnection::Error);
}

QTcpSocket *TcpConnection::GetSocket()
{
    if  (!sender()) return 0;
    return static_cast<QTcpSocket*>(sender());
}

void TcpConnection::Connected()
{
    if  (!sender()) return;
    qDebug() << this << " conected " << sender();
}

void TcpConnection::Disconnected()
{
    if  (!sender()) return;
    qDebug() << this << " disconnected " << GetSocket();
}

void TcpConnection::ReadyRead()
{
    if  (!sender()) return;
    qDebug() << this << " readyRead " << GetSocket();
}

void TcpConnection::BytesWritten(qint64 bytes)
{
    if  (!sender()) return;
    qDebug() << this << " BytedWritten " << GetSocket() << " number of bytes =  " << bytes;
}

void TcpConnection::StateChanged(QAbstractSocket::SocketState SocketState)
{
    if  (!sender()) return;
    qDebug() << this << " StateChanged " << GetSocket() << " state =  " << SocketState;
}

void TcpConnection::Error(QAbstractSocket::SocketError SocketError)
{
    if  (!sender()) return;
    qDebug() << this << " Error " << GetSocket() << " error =  " << SocketError;
}
