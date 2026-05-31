#pragma once

#include <QList>
#include <QMutex>
#include <QJsonArray>
#include "block.h"

// Класс блокчейна: хранит цепь блоков и пул неподтвержденных транзакций.
class Blockchain
{
public:
    Blockchain();

    void createGenesisBlock();
    Block getLatestBlock() const;
    QList<Block> getChain() const;

    bool addTransaction(const Transaction& tx);
    bool minePendingTransactions(const QString& miningRewardAddress);
    double getBalanceOfAddress(const QString& address) const;
    bool isChainValid(const QList<Block>& chainToCheck) const;
    bool isChainValid() const;
    bool replaceChain(const QList<Block>& newChain);
    bool addBlock(const Block& block);

    QJsonArray toJsonArray() const;
    static QList<Block> fromJsonArray(const QJsonArray& array);

    int getDifficulty() const;
    double getMiningReward() const;

private:
    mutable QMutex m_mutex;
    QList<Block> m_chain;
    QList<Transaction> m_pendingTransactions;
    int m_difficulty;
    double m_miningReward;
};
