#include "PhonixClient.h"

#include <QDebug>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

PhonixClient::PhonixClient(const QString& serverURL, QObject *parent) : QObject(parent),
    m_serverURL(serverURL),
    m_manager(new QNetworkAccessManager(this))
{
    m_manager->setAutoDeleteReplies(true);
}

bool PhonixClient::getPicture(const QString& key, const QString& id, QByteArray& byte)
{
    QNetworkRequest request;
    QString requestURL = m_serverURL + m_kBaseURL + id;
    request.setUrl(QUrl(requestURL));
    request.setRawHeader("User-Agent", "Hawk-Client 1.0");
    QNetworkReply * reply = m_manager->get(request);
    QEventLoop loop;
    QTimer timer;
    connect(m_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.setInterval(2000);
    timer.start();
    loop.exec();
    if (timer.isActive())
    {
        byte = reply->readAll();
        timer.stop();
        return true;
    }
    else
    {
        reply->abort();
        return false;
    }
}

bool PhonixClient::getPictureList(int pageSize, int pageId, QJsonArray& array)
{
    QNetworkRequest request;
    QString requestURL = m_serverURL + m_kBaseURL + "list/" +
            QString::number(pageSize) + "/" + QString::number(pageId);
    request.setUrl(QUrl(requestURL));
    request.setRawHeader("User-Agent", "Hawk-Client 1.0");
    QNetworkReply * reply = m_manager->get(request);
    QEventLoop loop;
    QTimer timer;
//    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(m_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.setInterval(2000);
    timer.start();
    loop.exec();
    if (timer.isActive())
    {
        QByteArray byte = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromRawData(byte.data(), byte.size());
        timer.stop();
        return true;
    }
    else
    {
        reply->abort();
        return false;
    }
}

bool PhonixClient::postPicture(const QString& key, const QString& id, const QByteArray& byte)
{
    QHttpMultiPart * form = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart keyPart;
    keyPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"key\""));
    keyPart.setBody(key.toLocal8Bit());

    QHttpPart idPart;
    idPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"id\""));
    idPart.setBody(id.toLocal8Bit());
    
    QHttpPart dataPart;
    dataPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    dataPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\"; filename=\"pic.jpg\""));
    dataPart.setBody(byte);
    form->append(keyPart);
    form->append(idPart);
    form->append(dataPart);
    QNetworkRequest request;
    QString requestURL = m_serverURL + m_kBaseURL;
    request.setUrl(QUrl(requestURL));
    request.setRawHeader("User-Agent", "Hawk-Client 1.0");
    QNetworkReply * reply = m_manager->post(request, form);
    form->setParent(reply);
    QEventLoop loop;
    QTimer timer;
    connect(m_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.setInterval(2000);
    timer.start();    loop.exec();
    if (timer.isActive())
    {
        QByteArray byte = reply->readAll();
        timer.stop();
        return true;
    }
    else
    {
        reply->abort();
        return false;
    }
}

bool PhonixClient::putPicture(const QString& key, const QString& id, const QByteArray& byte)
{
    QHttpMultiPart * form = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart keyPart;
    keyPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"key\""));
    keyPart.setBody(key.toLocal8Bit());

    QHttpPart idPart;
    idPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"id\""));
    idPart.setBody(id.toLocal8Bit());
    
    QHttpPart dataPart;
    dataPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    dataPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\"; filename=\"pic.jpg\""));
    dataPart.setBody(byte);
    form->append(keyPart);
    form->append(idPart);
    form->append(dataPart);
    QNetworkRequest request;
    QString requestURL = m_serverURL + m_kBaseURL + key;
    request.setUrl(QUrl(requestURL));
    request.setRawHeader("User-Agent", "Hawk-Client 1.0");
    QNetworkReply * reply = m_manager->put(request, form);
    form->setParent(reply);
    QEventLoop loop;
    QTimer timer;
    connect(m_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.setInterval(2000);
    timer.start();    loop.exec();
    if (timer.isActive())
    {
        QByteArray byte = reply->readAll();
        timer.stop();
        return true;
    }
    else
    {
        reply->abort();
        return false;
    }
}

bool PhonixClient::delPicture(const QString& key, const QString& id)
{
    QNetworkRequest request;
    QString requestURL = m_serverURL + m_kBaseURL + key;
    request.setUrl(QUrl(requestURL));
    request.setRawHeader("User-Agent", "Hawk-Client 1.0");
    QNetworkReply * reply = m_manager->deleteResource(request);
    QEventLoop loop;
    QTimer timer;
    connect(m_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.setInterval(2000);
    timer.start();    loop.exec();
    if (timer.isActive())
    {
        timer.stop();
        //TODO add json parsing
        return true;
    }
    else
    {
        reply->abort();
        return false;
    }
    return true;
}
