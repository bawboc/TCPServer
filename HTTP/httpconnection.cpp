#include "httpconnection.h"

HttpConnection::HttpConnection(QObject *parent) : TcpConnection(parent)
{
    qDebug() << this << "Created";

    m_mime.insert("txt","text/plain");
    m_mime.insert("htm","text/html");
    m_mime.insert("html","text/html");
    m_mime.insert("js","text/javascript");
    m_mime.insert("rtf","text/rtf");
    m_mime.insert("jpg","image/jpg");
    m_mime.insert("jpeg","image/jpeg");
    m_mime.insert("gif","image/gif");
    m_mime.insert("png","image/png");
    m_mime.insert("avi","video/avi");
    m_mime.insert("mpg","video/mpg");
    m_mime.insert("mpeg","video/mpeg");
    m_mime.insert("mp4","video/mp4");
    m_mime.insert("mkv","video/x-matroska");
    m_mime.insert("qt","video/quicktime");
    m_mime.insert("webm","video/webm");
    m_mime.insert("wmv","video/x-ms-wmv");
    m_mime.insert("flv","video/x-flv");
    m_mime.insert("mp3","audio/mp3");
    m_mime.insert("wav","audio/vnd.wav");
    m_mime.insert("tar","application/x-tar");
    m_mime.insert("zip","application/zip");
    m_mime.insert("pdf","application/pdf");
    m_mime.insert("xml","text/xml");
}

HttpConnection::~HttpConnection()
{
    qDebug() << this << "Destroyed";
}

QString HttpConnection::root()
{
    return m_root;
}

void HttpConnection::setRoot(QString path)
{
    m_root = path;
    qDebug() << this << "Root set to: " << m_root;
}

int HttpConnection::rate()
{
    return m_rate;
}

void HttpConnection::setRate(int rate)
{
    m_rate = rate;
    qDebug() << this << "Rate set to: " << rate;
}

void HttpConnection::processGet(QByteArray data)
{
    qDebug() << this << "Processing GET";

    QByteArray eoh;
    eoh.append("\r\n\r\n");

    QString header = m_request.value("svr_header","");
    header.append(data);
    m_request.insert("svr_header",header);

    if  (header.contains(eoh)){
        // End of header found, parse the header
        QStringList lines = header.split("\r\n");
        for (int i = 0; i < lines.count(); i++){
            QString line = lines.at(i);
            if  (i==0){
                // First line has 3 values -> METHOD PATH VERSION
                QStringList items = line.split(" ");
                if  (items.count() >= 1){
                    m_request.insert("http_method",items.at(0).trimmed());
                }else if  (items.count() >= 2){
                    QString path = items.at(1).trimmed();
                    path = QUrl::fromPercentEncoding(path.toLatin1());
                    m_request.insert("http_path",path);
                }else if  (items.count() >= 3){
                    m_request.insert("http_version",items.at(2).trimmed());
                }
            }else {
                //  All other lines are key value pairs
                if  (!line.contains(":") && !line.isEmpty()){
                    qWarning() << this << "Skipping line due to missing ':' " << line;
                    continue;
                }

                int pos = line.indexOf(":");
                QString key = line.mid(0,pos);
                QString value = line.mid(pos + 1);
                m_request.insert(key.trimmed(),value.trimmed());
            }
        }
        // attempt to handle the request
        handleRequest();
    }
}

void HttpConnection::handleRequest()
{
    qDebug() << this << "Handle the Request";

    QString file = m_request.value("http_path","");

    // Strip out any directory jumps
    file = file.replace("..","");

    QString actualFile = m_root + file;
    QFileInfo fi(actualFile);
    QByteArray response;

    // If it is a directory, check for index.html
    if  (fi.isDir()){
        qDebug() << this << "Client is requesting a directory";
        QString indexFile = actualFile + "index.html";
        QFileInfo fIndex(indexFile);
        if  (fIndex.exists()){
            qDebug() << this << "Setting / to /index.html";
            fi.setFile(indexFile);
        }
    }

    // Send the file if it exists
    if  (fi.exists() && fi.isFile()){ // It exists

        QString mime = getMimeType(fi.fileName());
        qDebug() << this << "Sending file: " << fi.filePath();
        m_response.insert("code","200");
        m_response.insert("path",fi.filePath());

        /*
         * Basice HTTP header
        */
        response.append("HTTP/1.1 200 OK\r\n");
        response.append("Content-Type: " + mime + "\r\n");
        response.append("Content-Length: " + QString::number(fi.size()) +"\r\n");
        response.append("Connection: close\r\n");
        response.append("\r\n");
        /* End header */
    }else { // It does not exist
        /*
         * Basice HTTP header
        */
        m_response.insert("code","404");

        response.append("HTTP/1.1 404 NOT FOUND\r\n");
        response.append("Connection: close\r\n");
        response.append("\r\n");
        /* End header */
    }

    m_socket->write(response);
}

void HttpConnection::sendFile(QString file)
{
    // To do - make a rate controller
}

QString HttpConnection::getMimeType(QString path)
{
    int pos = path.indexOf(".");
    if  (pos <= 0) return "application/octet-stream";

    QString ext = path.mid(pos + 1).toLower();
    if  (m_mime.contains(ext)){
        return m_mime.value(ext);
    }

    return "application/octet-stream";
}

void HttpConnection::connected()
{
    qDebug() << this << "Connected" << getSocket();
}

void HttpConnection::disconnected()
{
    qDebug() << this << "Disconnected" << getSocket();
}

void HttpConnection::readyRead()
{
    if  (!m_socket) return;
    qDebug() << this << "readyRead" << getSocket();
    QByteArray data = m_socket->readAll();
    processGet(data);
}

void HttpConnection::bytesWritten(qint64 bytes)
{
    if  (!m_socket) return;

    QString code = m_response.value("code","");

    if  (code == "200"){
        QString file = m_response.value("path","");
        sendFile(file);
        // close the socket when done
    }
    if (code == "404") {
        m_socket->close();
        return;
    }
}

void HttpConnection::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << this << "stateChanged" << m_socket << socketState;
}

void HttpConnection::error(QAbstractSocket::SocketError socketError)
{
    qDebug() << this << "socketError" << m_socket << socketError;
}

void HttpConnection::started()
{
    qDebug() << this << "File transfer started!";
}

void HttpConnection::transferred(int bytes)
{
    qDebug() << this << "File transferred" << bytes;
}

void HttpConnection::finished()
{
    qDebug() << this << "File transfer finished";
    m_file->close();
    m_socket->close();
}

void HttpConnection::transferError()
{
    qDebug() << this << "File transfer error: ";// << m_transfer->errorString();
}
