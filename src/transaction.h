#pragma once

#include <QString>
#include <QJsonObject>

// Класс транзакции: перевод монет от одного пользователя к другому.
class Transaction
{
public:
    Transaction();
    Transaction(const QString& sender, const QString& receiver, double amount, qint64 timestamp);

    const QString& getSender() const;
    const QString& getReceiver() const;
    double getAmount() const;
    qint64 getTimestamp() const;

    void setSender(const QString& sender);
    void setReceiver(const QString& receiver);
    void setAmount(double amount);
    void setTimestamp(qint64 timestamp);

    QJsonObject toJson() const;
    static Transaction fromJson(const QJsonObject& obj);

private:
    QString m_sender;
    QString m_receiver;
    double m_amount;
    qint64 m_timestamp;
};
