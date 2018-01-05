#ifndef RATETRANSFER_H
#define RATETRANSFER_H

#include <QObject>
#include <QIODevice>
#include <QTimer>
#include <QTime>
#include <QDebug>

class RateTransfer : public QObject
{
    Q_OBJECT
public:
    explicit RateTransfer(QObject *parent = nullptr);
    ~RateTransfer();

    int rate();
    void setRate(int value);

    int size();
    void setSize(int value);

    qint64 maximum();
    void setMaximum(qint64 value);

    QIODevice* source();
    void setSource(QIODevice* device);

    QIODevice* destination();
    void setDestination(QIODevice* device);

    bool isTransfering();
    QString errorString();

protected:
    int m_rate;
    int m_size;
    bool m_transferring;
    qint64 m_maximum;
    qint64 m_transferred;
    QIODevice* m_source;
    QIODevice* m_destination;
    QTimer m_timer;
    QString m_errorString;
    bool m_sheduled;

signals:

public slots:
};

#endif // RATETRANSFER_H
