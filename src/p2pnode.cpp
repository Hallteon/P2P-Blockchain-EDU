#include "p2pnode.h"

#include <QDataStream>
#include <QHostAddress>
#include <QJsonDocument>

namespace {
QString normalizeHost(const QHostAddress& address)
{
    const QString raw = address.toString();
    if (raw.startsWith("::ffff:")) {
        return raw.mid(QString("::ffff:").size());
    }
    return raw;
}
}

P2PNode::P2PNode(Blockchain* blockchain, QObject* parent)
    : QObject(parent), m_server(new QTcpServer(this)), m_blockchain(blockchain)
{
    connect(m_server, &QTcpServer::newConnection, this, &P2PNode::onNewConnection);
    m_listenPort = 0;
}

void P2PNode::startServer(quint16 port)
{
    m_server->listen(QHostAddress::Any, port);
    m_listenPort = port;
}

void P2PNode::connectToPeer(const QString& host, quint16 port)
{
    QString normalizedHost = host;
    QHostAddress hostAddress;
    if (hostAddress.setAddress(host)) {
        normalizedHost = normalizeHost(hostAddress);
    }
    const QString endpoint = QString("%1:%2").arg(normalizedHost).arg(port);
    // запрет на подключение к самому себе
    if (m_listenPort != 0 && port == m_listenPort) {
        emit peerConnectionFailed(endpoint, "Нельзя подключаться к самому себе");
        return;
    }
    if (m_endpoints.contains(endpoint)) {
        return;
    }

    auto* socket = new QTcpSocket(this);
    m_endpoints.insert(endpoint);
    m_peerEndpoints.insert(socket, endpoint);

    connect(socket, &QTcpSocket::connected, this, &P2PNode::onConnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &P2PNode::onConnectionError);
    socket->connectToHost(host, port);

    connect(socket, &QTcpSocket::readyRead, this, &P2PNode::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &P2PNode::onDisconnected);
}

void P2PNode::broadcast(const QJsonObject& message)
{
    for (QTcpSocket* peer : m_peers) {
        sendMessage(peer, message);
    }
}

void P2PNode::onNewConnection()
{
    while (m_server->hasPendingConnections()) {
        QTcpSocket* socket = m_server->nextPendingConnection();
        // Входящее соединение: порт слушателя узнаём через HELLO
        m_peerEndpoints.insert(socket, QString());

        connect(socket, &QTcpSocket::readyRead, this, &P2PNode::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &P2PNode::onDisconnected);
        m_peers.append(socket);

        QJsonObject hello;
        hello["type"] = "HELLO";
        QJsonObject helloData;
        helloData["port"] = static_cast<int>(m_listenPort);
        hello["data"] = helloData;
        sendMessage(socket, hello);

        QJsonObject request;
        request["type"] = "REQUEST_CHAIN";
        sendMessage(socket, request);
    }
}

void P2PNode::onConnected()
{
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        return;
    }

    if (!m_peers.contains(socket)) {
        m_peers.append(socket);
    }

    const QString endpoint = m_peerEndpoints.value(socket);
    if (!endpoint.isEmpty()) {
        emit peerConnected(endpoint);
    }

    QJsonObject hello;
    hello["type"] = "HELLO";
    QJsonObject helloData;
    helloData["port"] = static_cast<int>(m_listenPort);
    hello["data"] = helloData;
    sendMessage(socket, hello);

    QJsonObject request;
    request["type"] = "REQUEST_CHAIN";
    sendMessage(socket, request);
}

void P2PNode::onConnectionError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        return;
    }

    const QString endpoint = m_peerEndpoints.value(socket, "неизвестный узел");
    emit peerConnectionFailed(endpoint, socket->errorString());

    m_peers.removeAll(socket);
    m_buffers.remove(socket);
    m_peerEndpoints.remove(socket);
    m_endpoints.remove(endpoint);
    socket->deleteLater();
}

bool P2PNode::hasPeerWithPort(const QString& portStr) const
{
    quint16 port = static_cast<quint16>(portStr.toUShort());
    for (QTcpSocket* s : m_peers) {
        if (s && s->state() == QAbstractSocket::ConnectedState) {
            const QString endpoint = m_peerEndpoints.value(s);
            const int separator = endpoint.lastIndexOf(":");
            if (separator > 0) {
                bool ok = false;
                const quint16 endpointPort = endpoint.mid(separator + 1).toUShort(&ok);
                if (ok && endpointPort == port) {
                    return true;
                }
            }
        }
    }
    return false;
}

void P2PNode::onReadyRead()
{
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        return;
    }

    QByteArray& buffer = m_buffers[socket];
    buffer.append(socket->readAll());

    while (true) {
        if (buffer.size() < static_cast<int>(sizeof(quint32))) {
            break;
        }

        QDataStream sizeStream(buffer.left(sizeof(quint32)));
        sizeStream.setByteOrder(QDataStream::BigEndian);
        quint32 messageSize = 0;
        sizeStream >> messageSize;

        if (buffer.size() < static_cast<int>(sizeof(quint32) + messageSize)) {
            break;
        }

        QByteArray messageData = buffer.mid(sizeof(quint32), messageSize);
        buffer.remove(0, sizeof(quint32) + messageSize);

        QJsonDocument doc = QJsonDocument::fromJson(messageData);
        if (doc.isObject()) {
            processMessage(doc.object(), socket);
        }
    }
}

void P2PNode::onDisconnected()
{
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        return;
    }

    const QString endpoint = m_peerEndpoints.value(socket,
        QString("%1:%2").arg(socket->peerAddress().toString()).arg(socket->peerPort()));
    m_peers.removeAll(socket);
    m_buffers.remove(socket);
    m_peerEndpoints.remove(socket);
    m_endpoints.remove(endpoint);
    emit peerDisconnected(endpoint);
    socket->deleteLater();
}

void P2PNode::sendMessage(QTcpSocket* socket, const QJsonObject& message)
{
    if (!socket || socket->state() != QAbstractSocket::ConnectedState) {
        return;
    }

    QJsonDocument doc(message);
    QByteArray payload = doc.toJson(QJsonDocument::Compact);

    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << static_cast<quint32>(payload.size());
    packet.append(payload);

    socket->write(packet);
}

void P2PNode::processMessage(const QJsonObject& message, QTcpSocket* senderSocket)
{
    const QString type = message.value("type").toString();

    if (type == "HELLO") {
        const QJsonObject data = message.value("data").toObject();
        const int port = data.value("port").toInt();
        if (port > 0) {
            const QString endpoint = QString("%1:%2").arg(normalizeHost(senderSocket->peerAddress())).arg(port);

            const QString previousEndpoint = m_peerEndpoints.value(senderSocket);
            if (previousEndpoint == endpoint && !previousEndpoint.isEmpty()) {
                return;
            }

            if (!previousEndpoint.isEmpty()) {
                m_endpoints.remove(previousEndpoint);
            }

            if (m_endpoints.contains(endpoint)) {
                senderSocket->disconnectFromHost();
                return;
            }

            m_endpoints.insert(endpoint);
            m_peerEndpoints.insert(senderSocket, endpoint);
            emit peerConnected(endpoint);
        }
    } else if (type == "NEW_TRANSACTION") {
        Transaction tx = Transaction::fromJson(message.value("data").toObject());
        if (m_blockchain->addTransaction(tx)) {
            emit newTransactionReceived(tx);
        }
    } else if (type == "NEW_BLOCK") {
        Block block = Block::fromJson(message.value("data").toObject());
        if (m_blockchain->addBlock(block)) {
            emit newBlockReceived(block);
        } else {
            QJsonObject request;
            request["type"] = "REQUEST_CHAIN";
            sendMessage(senderSocket, request);
        }
    } else if (type == "REQUEST_CHAIN") {
        QJsonObject response;
        response["type"] = "RESPONSE_CHAIN";
        response["data"] = m_blockchain->toJsonArray();
        sendMessage(senderSocket, response);
    } else if (type == "RESPONSE_CHAIN") {
        const QJsonArray array = message.value("data").toArray();
        QList<Block> incomingChain = Blockchain::fromJsonArray(array);
        if (m_blockchain->replaceChain(incomingChain)) {
            emit chainUpdated();
        }
    }
}
