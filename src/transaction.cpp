#include "transaction.h"

Transaction::Transaction()
    : m_amount(0.0), m_timestamp(0)
{
}

Transaction::Transaction(const QString& sender, const QString& receiver, double amount, qint64 timestamp)
    : m_sender(sender), m_receiver(receiver), m_amount(amount), m_timestamp(timestamp)
{
}

const QString& Transaction::getSender() const
{
    return m_sender;
}

const QString& Transaction::getReceiver() const
{
    return m_receiver;
}

double Transaction::getAmount() const
{
    return m_amount;
}

qint64 Transaction::getTimestamp() const
{
    return m_timestamp;
}

void Transaction::setSender(const QString& sender)
{
    m_sender = sender;
}

void Transaction::setReceiver(const QString& receiver)
{
    m_receiver = receiver;
}

void Transaction::setAmount(double amount)
{
    m_amount = amount;
}

void Transaction::setTimestamp(qint64 timestamp)
{
    m_timestamp = timestamp;
}

QJsonObject Transaction::toJson() const
{
    QJsonObject obj;
    obj["sender"] = m_sender;
    obj["receiver"] = m_receiver;
    obj["amount"] = m_amount;
    obj["timestamp"] = static_cast<qint64>(m_timestamp);
    return obj;
}

Transaction Transaction::fromJson(const QJsonObject& obj)
{
    return Transaction(
        obj.value("sender").toString(),
        obj.value("receiver").toString(),
        obj.value("amount").toDouble(),
        obj.value("timestamp").toVariant().toLongLong()
    );
}
