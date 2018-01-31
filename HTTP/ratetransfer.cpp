#include "ratetransfer.h"

RateTransfer::RateTransfer(QObject *parent) : QObject(parent)
{
    setDefaults();
}

int RateTransfer::rate()
{
    return m_rate;
}

void RateTransfer::setRate(int value)
{
    m_rate = value;
}

int RateTransfer::size()
{
    return m_size;
}

void RateTransfer::setSize(int value)
{
    m_size = value;
}

qint64 RateTransfer::maximum()
{
    return m_maximum;
}

void RateTransfer::setMaximum(qint64 value)
{
    m_maximum = value;
}

QIODevice *RateTransfer::source()
{
    return m_source;
}

void RateTransfer::setSource(QIODevice *device)
{
    m_source = device;

    if (m_source->isSequential()) connect(m_source,&QIODevice::readyRead,this,&RateTransfer::readyRead);
}

QIODevice *RateTransfer::destination()
{
    return m_destination;
}

void RateTransfer::setDestination(QIODevice *device)
{
    m_destination = device;
    if (m_destination->isSequential()) connect(m_destination,&QIODevice::bytesWritten,this,&RateTransfer::bytesWritten);
}

bool RateTransfer::isTransfering()
{
    return m_transferring;
}

QString RateTransfer::errorString()
{
    return m_errorString;
}

void RateTransfer::setDefaults()
{
    m_rate = 0;
    m_size = 0;
    m_maximum = 0;
    m_transferring = true;
    m_transferred = 0;
    m_source = 0;
    m_destination = 0;
    m_errorString = "";
    m_sheduled = false;
    m_timer.setInterval(5);
}

bool RateTransfer::checkDevices()
{
    if (!m_source){
        m_transferring = false;
        m_errorString = "No source";
        emit error();
        return false;
    }

    if (!m_destination){
        m_transferring = false;
        m_errorString = "No destination";
        emit error();
        return false;
    }

    if (!m_source->isOpen() || !m_source->isReadable()){
        m_transferring = false;
        m_errorString = "Source not open or not readable";
        emit error();
        return false;
    }

    if (!m_destination->isOpen() || !m_destination->isWritable()){
        m_transferring = false;
        m_errorString = "Destination either not open or not writeable";
        emit error();
        return false;
    }

    return true;
}

bool RateTransfer::checkTransfer()
{
    if(!m_transferring){
        m_errorString = "Not Transferring... Aborting";
        emit error();
        return false;
    }

    if(m_transferred >= m_rate){
        m_errorString = "Rate exceeded, not allowed to transfer.";
        emit error();
        return false;
    }

    return true;
}

void RateTransfer::scheduleTransfer()
{
    if (m_sheduled){
        qWarning() << this << "Exiting scheduleTransfer due to: waiting on timer";
        return;
    }

    if (!m_transferring){
        qWarning() << this << "Exiting scheduleTransfer due to: not transferring";
        return;
    }

    if (m_source->bytesAvailable() <= 0){
        qWarning() << this << "Exiting scheduleTransfer due to: waiting on timerno bytes available to read";
        return;
    }

    int prediction = m_transferred + m_size;
    if (prediction <= m_rate){
        transfer();
    }else {
        int current = QTime::currentTime().msec();
        int delay = 1000 - current;
        m_transferred = 0;
        m_sheduled = true;
    }
}

void RateTransfer::start()
{
    if (m_transferring){
        return;
    }

    m_errorString = "";
    if (!checkDevices()) return;

    m_transferring = true;
    m_transferred = 0;
    emit started();

    if (!m_source->isSequential() && m_source->bytesAvailable() > 0){
        scheduleTransfer();
    }
}

void RateTransfer::stop()
{
    m_timer.stop();
    m_transferring = false;
}

void RateTransfer::transfer()
{

}

void RateTransfer::readyRead()
{
    scheduleTransfer();
}

void RateTransfer::bytesWritten(qint64 bytes)
{
    scheduleTransfer();
}
