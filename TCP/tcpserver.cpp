#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    qDebug() << this << " created on " << QThread::currentThread();
}

TcpServer::~TcpServer()
{
    qDebug() << this << "Destroyed";
}

bool TcpServer::listen(const QHostAddress &Address, quint16 Port)
{
    if  (!QTcpServer::listen(Address,Port)) {
        qDebug() << this << "Failed to listen on port";
        return false;
    }
    m_thread = new QThread(this);
    m_connections = new TcpConnections();

    connect(m_thread,&QThread::started,m_connections,&TcpConnections::start, Qt::QueuedConnection);
    connect(this,&TcpServer::accepting,m_connections,&TcpConnections::accept,Qt::QueuedConnection);
    connect(this,&TcpServer::finished,m_connections,&TcpConnections::quit,Qt::QueuedConnection);
    connect(m_connections,&TcpConnections::finished,this,&TcpServer::complete,Qt::QueuedConnection);

    m_connections->moveToThread(m_thread);
    m_thread->start();

    return true;
}

void TcpServer::close()
{
    qDebug() << this << "Closing Server";
    emit finished();
    QTcpServer::close();
}

qint64 TcpServer::port()
{
    if  (isListening()){
        return this->serverPort();
    }else {
        return 1000;
    }
}

void TcpServer::incomingConnection(qintptr Descriptor)
{
    qDebug() << this << "attemping to accept a new connection " << Descriptor;
    TcpConnection* connection = new TcpConnection();
    accept(Descriptor,connection);
}

void TcpServer::accept(qintptr Descriptor, TcpConnection *Connection)
{
    qDebug() << this << "Accepting the connection" << Descriptor;
    Connection->moveToThread(m_thread);
    emit accepting(Descriptor,Connection);
}

void TcpServer::complete()
{
    if  (!m_thread){
        qWarning() << this << "Exiting complete, there was no thread";
        return;
    }

    qDebug() << this << "Complete called, destroying thread";
    delete m_connections;

    qDebug() << this << "Quitting thread";
    m_thread->quit();
    m_thread->wait();

    delete m_thread;

    qDebug() << this << "complete";
}
