#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include <QSet>
#include <QJsonObject>
#include "blockchain.h"

// Сетевой слой P2P-узла.
class P2PNode : public QObject
{
    Q_OBJECT

public:
    explicit P2PNode(Blockchain* blockchain, QObject* parent = nullptr);

    void startServer(quint16 port);
    void connectToPeer(const QString& host, quint16 port);
    void broadcast(const QJsonObject& message);
    bool hasPeerWithPort(const QString& portStr) const;

signals:
    void peerConnected(const QString& peerInfo);
    void peerDisconnected(const QString& peerInfo);
    void peerConnectionFailed(const QString& peerInfo, const QString& reason);
    void newTransactionReceived(const Transaction& tx);
    void newBlockReceived(const Block& block);
    void chainUpdated();

private slots:
    void onNewConnection();
    void onConnected();
    void onConnectionError(QAbstractSocket::SocketError socketError);
    void onReadyRead();
    void onDisconnected();

private:
    void sendMessage(QTcpSocket* socket, const QJsonObject& message);
    void processMessage(const QJsonObject& message, QTcpSocket* senderSocket);

    QTcpServer* m_server;
    QList<QTcpSocket*> m_peers;
    QHash<QTcpSocket*, QByteArray> m_buffers;
    QHash<QTcpSocket*, QString> m_peerEndpoints;
    QSet<QString> m_endpoints;
    quint16 m_listenPort;
    Blockchain* m_blockchain;
};
