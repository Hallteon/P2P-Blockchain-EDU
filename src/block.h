#pragma once

#include <QList>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include "transaction.h"

// Блок блокчейна: хранит список транзакций и хеш-ссылки.
class Block
{
public:
    Block();
    Block(int index,
          qint64 timestamp,
          const QList<Transaction>& transactions,
          const QString& previousHash);

    int getIndex() const;
    qint64 getTimestamp() const;
    const QList<Transaction>& getTransactions() const;
    const QString& getPreviousHash() const;
    const QString& getHash() const;
    int getNonce() const;

    void setHash(const QString& hash);
    void setNonce(int nonce);

    QString calculateHash() const;
    void mineBlock(int difficulty);

    QJsonObject toJson() const;
    static Block fromJson(const QJsonObject& obj);

private:
    int m_index;
    qint64 m_timestamp;
    QList<Transaction> m_transactions;
    QString m_previousHash;
    QString m_hash;
    int m_nonce;
};
