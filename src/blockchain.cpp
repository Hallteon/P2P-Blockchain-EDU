#include "blockchain.h"

#include <QDateTime>

Blockchain::Blockchain()
    : m_difficulty(3), m_miningReward(10.0)
{
    createGenesisBlock();
}

void Blockchain::createGenesisBlock()
{
    QMutexLocker locker(&m_mutex);
    if (!m_chain.isEmpty()) {
        return;
    }

    Block genesis(0, QDateTime::currentMSecsSinceEpoch(), {}, "0");
    m_chain.append(genesis);
}

Block Blockchain::getLatestBlock() const
{
    QMutexLocker locker(&m_mutex);
    return m_chain.last();
}

QList<Block> Blockchain::getChain() const
{
    QMutexLocker locker(&m_mutex);
    return m_chain;
}

bool Blockchain::addTransaction(const Transaction& tx)
{
    if (tx.getSender().isEmpty() || tx.getReceiver().isEmpty() || tx.getAmount() <= 0.0) {
        return false;
    }

    QMutexLocker locker(&m_mutex);
    m_pendingTransactions.append(tx);
    return true;
}

bool Blockchain::minePendingTransactions(const QString& miningRewardAddress)
{
    QList<Transaction> transactionsToMine;
    {
        QMutexLocker locker(&m_mutex);
        if (m_pendingTransactions.isEmpty()) {
            return false;
        }
        transactionsToMine = m_pendingTransactions;
        transactionsToMine.append(Transaction("SYSTEM", miningRewardAddress, m_miningReward,
                                              QDateTime::currentMSecsSinceEpoch()));
    }

    Block newBlock(m_chain.size(), QDateTime::currentMSecsSinceEpoch(), transactionsToMine,
                   getLatestBlock().getHash());
    newBlock.mineBlock(m_difficulty);

    QMutexLocker locker(&m_mutex);
    m_chain.append(newBlock);
    m_pendingTransactions.clear();
    return true;
}

double Blockchain::getBalanceOfAddress(const QString& address) const
{
    double balance = 0.0;
    QMutexLocker locker(&m_mutex);
    for (const Block& block : m_chain) {
        for (const Transaction& tx : block.getTransactions()) {
            if (tx.getSender() == address) {
                balance -= tx.getAmount();
            }
            if (tx.getReceiver() == address) {
                balance += tx.getAmount();
            }
        }
    }
    return balance;
}

bool Blockchain::isChainValid(const QList<Block>& chainToCheck) const
{
    if (chainToCheck.isEmpty()) {
        return false;
    }

    const QString target = QString(m_difficulty, '0');

    for (int i = 1; i < chainToCheck.size(); ++i) {
        const Block& current = chainToCheck[i];
        const Block& previous = chainToCheck[i - 1];

        if (current.getHash() != current.calculateHash()) {
            return false;
        }
        if (current.getPreviousHash() != previous.getHash()) {
            return false;
        }
        if (!current.getHash().startsWith(target)) {
            return false;
        }
    }

    return true;
}

bool Blockchain::isChainValid() const
{
    QMutexLocker locker(&m_mutex);
    return isChainValid(m_chain);
}

bool Blockchain::replaceChain(const QList<Block>& newChain)
{
    QMutexLocker locker(&m_mutex);
    if (newChain.size() <= m_chain.size()) {
        return false;
    }
    if (!isChainValid(newChain)) {
        return false;
    }

    m_chain = newChain;
    m_pendingTransactions.clear();
    return true;
}

bool Blockchain::addBlock(const Block& block)
{
    QMutexLocker locker(&m_mutex);
    const Block& latest = m_chain.last();
    if (block.getIndex() != latest.getIndex() + 1) {
        return false;
    }
    if (block.getPreviousHash() != latest.getHash()) {
        return false;
    }
    if (block.getHash() != block.calculateHash()) {
        return false;
    }
    if (!block.getHash().startsWith(QString(m_difficulty, '0'))) {
        return false;
    }

    m_chain.append(block);
    m_pendingTransactions.clear();
    return true;
}

QJsonArray Blockchain::toJsonArray() const
{
    QMutexLocker locker(&m_mutex);
    QJsonArray array;
    for (const Block& block : m_chain) {
        array.append(block.toJson());
    }
    return array;
}

QList<Block> Blockchain::fromJsonArray(const QJsonArray& array)
{
    QList<Block> chain;
    for (const QJsonValue& value : array) {
        chain.append(Block::fromJson(value.toObject()));
    }
    return chain;
}

int Blockchain::getDifficulty() const
{
    return m_difficulty;
}

double Blockchain::getMiningReward() const
{
    return m_miningReward;
}
