#pragma once

#include <QMainWindow>
#include <QFutureWatcher>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QTabWidget>
#include "blockchain.h"
#include "p2pnode.h"

// Главное окно приложения.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(quint16 port, QWidget* parent = nullptr);

private slots:
    void onConnectClicked();
    void onSendClicked();
    void onMineClicked();
    void onMiningFinished();
    void onPeerConnected(const QString& peerInfo);
    void onPeerDisconnected(const QString& peerInfo);
    void onPeerConnectionFailed(const QString& peerInfo, const QString& reason);
    void onNewBlock(const Block& block);
    void onChainUpdated();

private:
    void setupUi();
    void updateBalance();
    void updateChainView();
    QJsonObject buildMessage(const QString& type, const QJsonObject& data = QJsonObject()) const;

    quint16 m_port;
    QString m_address;
    Blockchain* m_blockchain;
    P2PNode* m_p2p;
    QFutureWatcher<bool> m_miningWatcher;

    QLabel* m_addressLabel;
    QLabel* m_balanceLabel;
    QLineEdit* m_peerPortEdit;
    QListWidget* m_peersList;
    QLineEdit* m_receiverEdit;
    QLineEdit* m_amountEdit;
    QPushButton* m_sendButton;
    QPushButton* m_mineButton;
    QTextEdit* m_chainView;
    QTextEdit* m_miningStatus;
};
