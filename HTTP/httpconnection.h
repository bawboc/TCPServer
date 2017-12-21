#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QCoreApplication>
#include <QMap>
#include <QStringList>
#include "tcpconnection.h"

// To do - make a rate controller

class HttpConnection : public TcpConnection
{
    Q_OBJECT
public:
    explicit HttpConnection(QObject *parent = nullptr);
    ~HttpConnection();

    QString root();
    void setRoot(QString path);
    int rate();
    void setRate(int rate);

protected:
    QFile* m_file;
    QString m_root;
    int m_rate;

    QMap<QString, QString> m_request;
    QMap<QString, QString> m_response;
    QMap<QString, QString> m_mime;

    // To do - make a rate controller

    void processGet(QByteArray data);
    void handleRequest();
    void sendFile(QString file);
    QString getMimeType(QString path);

signals:

public slots:
    virtual void connected();
    virtual void disconnected();
    virtual void readyRead();
    virtual void bytesWritten(qint64 bytes);
    virtual void stateChanged(QAbstractSocket::SocketState socketState);
    virtual void error(QAbstractSocket::SocketError socketError);

    void started();
    void transferred(int bytes);
    void finished();
    void transferError();
};

#endif // HTTPCONNECTION_H
