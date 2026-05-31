#include "mainwindow.h"

#include <QDateTime>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QtConcurrent>

MainWindow::MainWindow(quint16 port, QWidget* parent)
    : QMainWindow(parent),
      m_port(port),
      m_address(QString::number(port)),
      m_blockchain(new Blockchain()),
      m_p2p(new P2PNode(m_blockchain, this))
{
    setupUi();

    connect(&m_miningWatcher, &QFutureWatcher<bool>::finished, this, &MainWindow::onMiningFinished);
    connect(m_p2p, &P2PNode::peerConnected, this, &MainWindow::onPeerConnected);
    connect(m_p2p, &P2PNode::peerDisconnected, this, &MainWindow::onPeerDisconnected);
    connect(m_p2p, &P2PNode::peerConnectionFailed, this, &MainWindow::onPeerConnectionFailed);
    connect(m_p2p, &P2PNode::newBlockReceived, this, &MainWindow::onNewBlock);
    connect(m_p2p, &P2PNode::chainUpdated, this, &MainWindow::onChainUpdated);

    m_p2p->startServer(port);

    updateBalance();
    updateChainView();
}

void MainWindow::setupUi()
{
    setWindowTitle(QString("P2P Blockchain Node %1").arg(m_address));
    resize(1100, 600);

    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* mainLayout = new QVBoxLayout(central);

    auto* infoLayout = new QHBoxLayout();
    m_addressLabel = new QLabel(QString("Адрес: Node_%1").arg(m_address), this);
    m_balanceLabel = new QLabel("Баланс: 0", this);
    infoLayout->addWidget(m_addressLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(m_balanceLabel);
    mainLayout->addLayout(infoLayout);

    auto* contentLayout = new QHBoxLayout();
    mainLayout->addLayout(contentLayout, 1);

    auto* networkBox = new QGroupBox("Сеть", this);
    auto* networkLayout = new QVBoxLayout(networkBox);
    m_peerPortEdit = new QLineEdit(this);
    m_peerPortEdit->setPlaceholderText("Порт (например, 5002)");
    auto* connectButton = new QPushButton("Connect", this);
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    m_peersList = new QListWidget(this);
    networkLayout->addWidget(m_peerPortEdit);
    networkLayout->addWidget(connectButton);
    networkLayout->addWidget(m_peersList, 1);

    auto* actionsBox = new QGroupBox("Действия", this);
    auto* actionsLayout = new QVBoxLayout(actionsBox);
    auto* tabs = new QTabWidget(this);

    auto* transferTab = new QWidget(this);
    auto* transferLayout = new QVBoxLayout(transferTab);
    m_receiverEdit = new QLineEdit(this);
    m_receiverEdit->setPlaceholderText("Адрес получателя");
    m_amountEdit = new QLineEdit(this);
    m_amountEdit->setPlaceholderText("Сумма");
    m_sendButton = new QPushButton("Send", this);
    connect(m_sendButton, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    transferLayout->addWidget(m_receiverEdit);
    transferLayout->addWidget(m_amountEdit);
    transferLayout->addWidget(m_sendButton);
    transferLayout->addStretch();

    auto* miningTab = new QWidget(this);
    auto* miningLayout = new QVBoxLayout(miningTab);
    m_mineButton = new QPushButton("Mine Block", this);
    connect(m_mineButton, &QPushButton::clicked, this, &MainWindow::onMineClicked);
    m_miningStatus = new QTextEdit(this);
    m_miningStatus->setReadOnly(true);
    m_miningStatus->setPlaceholderText("Статус майнинга...");
    miningLayout->addWidget(m_mineButton);
    miningLayout->addWidget(m_miningStatus, 1);

    tabs->addTab(transferTab, "Transfer");
    tabs->addTab(miningTab, "Mining");
    actionsLayout->addWidget(tabs);

    auto* chainBox = new QGroupBox("Цепочка блоков", this);
    auto* chainLayout = new QVBoxLayout(chainBox);
    m_chainView = new QTextEdit(this);
    m_chainView->setReadOnly(true);
    chainLayout->addWidget(m_chainView);

    contentLayout->addWidget(networkBox, 1);
    contentLayout->addWidget(actionsBox, 2);
    contentLayout->addWidget(chainBox, 2);
}

void MainWindow::onConnectClicked()
{
    const QString portText = m_peerPortEdit->text().trimmed();
    bool ok = false;
    const int port = portText.toInt(&ok);
    if (!ok || port <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный порт");
        return;
    }

    // Запрет подключаться к собственному порту
    if (port == static_cast<int>(m_port)) {
        QMessageBox::warning(this, "Ошибка", "Нельзя подключиться к самому себе");
        return;
    }

    m_p2p->connectToPeer("127.0.0.1", static_cast<quint16>(port));
}

void MainWindow::onSendClicked()
{
    const QString receiver = m_receiverEdit->text().trimmed();
    bool ok = false;
    const double amount = m_amountEdit->text().toDouble(&ok);

    if (receiver.isEmpty() || !ok || amount <= 0.0) {
        QMessageBox::warning(this, "Ошибка", "Введите адрес и сумму");
        return;
    }

    // Проверяем, что receiver — это порт, и что с ним есть активное соединение
    bool okPort = false;
    const int recvPort = receiver.toInt(&okPort);
    if (!okPort || recvPort <= 0) {
        QMessageBox::warning(this, "Ошибка", "Адрес получателя должен быть портом (номер порта)");
        return;
    }

    if (!m_p2p->hasPeerWithPort(receiver)) {
        QMessageBox::warning(this, "Ошибка", "Получатель не подключён к сети");
        return;
    }

    // Запрет переводить себе самим
    if (receiver == m_address) {
        QMessageBox::warning(this, "Ошибка", "Нельзя отправлять деньги самому себе");
        return;
    }

    Transaction tx(m_address, receiver, amount, QDateTime::currentMSecsSinceEpoch());
    if (!m_blockchain->addTransaction(tx)) {
        QMessageBox::warning(this, "Ошибка", "Транзакция не прошла проверку");
        return;
    }

    QJsonObject message = buildMessage("NEW_TRANSACTION", tx.toJson());
    m_p2p->broadcast(message);

    m_miningStatus->append(QString("Создана транзакция на %1").arg(amount));
}

void MainWindow::onMineClicked()
{
    m_mineButton->setEnabled(false);
    m_miningStatus->append("Майнинг запущен...");

    QFuture<bool> future = QtConcurrent::run([this]() {
        return m_blockchain->minePendingTransactions(m_address);
    });
    m_miningWatcher.setFuture(future);
}

void MainWindow::onMiningFinished()
{
    m_mineButton->setEnabled(true);
    const bool mined = m_miningWatcher.result();
    if (!mined) {
        m_miningStatus->append("Нет транзакций для майнинга");
        return;
    }

    Block latest = m_blockchain->getLatestBlock();
    QJsonObject message = buildMessage("NEW_BLOCK", latest.toJson());
    m_p2p->broadcast(message);

    m_miningStatus->append(QString("Блок %1 успешно найден").arg(latest.getIndex()));
    updateChainView();
    updateBalance();
}

void MainWindow::onPeerConnected(const QString& peerInfo)
{
    QList<QListWidgetItem*> existing = m_peersList->findItems(peerInfo, Qt::MatchExactly);
    if (existing.isEmpty()) {
        m_peersList->addItem(peerInfo);
    }
}

void MainWindow::onPeerDisconnected(const QString& peerInfo)
{
    QList<QListWidgetItem*> items = m_peersList->findItems(peerInfo, Qt::MatchExactly);
    for (QListWidgetItem* item : items) {
        delete item;
    }
}

void MainWindow::onPeerConnectionFailed(const QString& peerInfo, const QString& reason)
{
    m_miningStatus->append(QString("Не удалось подключиться к %1: %2").arg(peerInfo, reason));
    QMessageBox::warning(this, "Ошибка подключения",
                         QString("Не удалось подключиться к %1\nПричина: %2").arg(peerInfo, reason));
}

void MainWindow::onNewBlock(const Block& block)
{
    Q_UNUSED(block);
    m_miningStatus->append("Получен новый блок от сети");
    updateChainView();
    updateBalance();
}

void MainWindow::onChainUpdated()
{
    m_miningStatus->append("Цепочка синхронизирована");
    updateChainView();
    updateBalance();
}

void MainWindow::updateBalance()
{
    const double balance = m_blockchain->getBalanceOfAddress(m_address);
    m_balanceLabel->setText(QString("Баланс: %1").arg(balance, 0, 'f', 2));
}

void MainWindow::updateChainView()
{
    QStringList lines;
    const QList<Block> chain = m_blockchain->getChain();
    for (const Block& block : chain) {
        lines << QString("Block #%1 | Hash: %2 | Txs: %3")
                    .arg(block.getIndex())
                    .arg(block.getHash().left(16) + "...")
                    .arg(block.getTransactions().size());
    }
    m_chainView->setText(lines.join("\n"));
}

QJsonObject MainWindow::buildMessage(const QString& type, const QJsonObject& data) const
{
    QJsonObject message;
    message["type"] = type;
    if (!data.isEmpty()) {
        message["data"] = data;
    }
    return message;
}
