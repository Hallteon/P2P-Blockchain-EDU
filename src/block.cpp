#include "block.h"

#include <QCryptographicHash>
#include <QJsonDocument>

Block::Block()
    : m_index(0), m_timestamp(0), m_nonce(0)
{
}

Block::Block(int index,
             qint64 timestamp,
             const QList<Transaction>& transactions,
             const QString& previousHash)
    : m_index(index),
      m_timestamp(timestamp),
      m_transactions(transactions),
      m_previousHash(previousHash),
      m_nonce(0)
{
    m_hash = calculateHash();
}

int Block::getIndex() const
{
    return m_index;
}

qint64 Block::getTimestamp() const
{
    return m_timestamp;
}

const QList<Transaction>& Block::getTransactions() const
{
    return m_transactions;
}

const QString& Block::getPreviousHash() const
{
    return m_previousHash;
}

const QString& Block::getHash() const
{
    return m_hash;
}

int Block::getNonce() const
{
    return m_nonce;
}

void Block::setHash(const QString& hash)
{
    m_hash = hash;
}

void Block::setNonce(int nonce)
{
    m_nonce = nonce;
}

QString Block::calculateHash() const
{
    QJsonArray txArray;
    for (const Transaction& tx : m_transactions) {
        txArray.append(tx.toJson());
    }

    QJsonObject data;
    data["index"] = m_index;
    data["timestamp"] = static_cast<qint64>(m_timestamp);
    data["transactions"] = txArray;
    data["previousHash"] = m_previousHash;
    data["nonce"] = m_nonce;

    const QByteArray jsonBytes = QJsonDocument(data).toJson(QJsonDocument::Compact);
    const QByteArray hash = QCryptographicHash::hash(jsonBytes, QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

void Block::mineBlock(int difficulty)
{
    const QString target = QString(difficulty, '0');
    while (!m_hash.startsWith(target)) {
        ++m_nonce;
        m_hash = calculateHash();
    }
}

QJsonObject Block::toJson() const
{
    QJsonArray txArray;
    for (const Transaction& tx : m_transactions) {
        txArray.append(tx.toJson());
    }

    QJsonObject obj;
    obj["index"] = m_index;
    obj["timestamp"] = static_cast<qint64>(m_timestamp);
    obj["transactions"] = txArray;
    obj["previousHash"] = m_previousHash;
    obj["hash"] = m_hash;
    obj["nonce"] = m_nonce;
    return obj;
}

Block Block::fromJson(const QJsonObject& obj)
{
    QList<Transaction> transactions;
    const QJsonArray txArray = obj.value("transactions").toArray();
    for (const QJsonValue& value : txArray) {
        transactions.append(Transaction::fromJson(value.toObject()));
    }

    Block block(
        obj.value("index").toInt(),
        obj.value("timestamp").toVariant().toLongLong(),
        transactions,
        obj.value("previousHash").toString()
    );

    block.m_hash = obj.value("hash").toString();
    block.m_nonce = obj.value("nonce").toInt();
    return block;
}
