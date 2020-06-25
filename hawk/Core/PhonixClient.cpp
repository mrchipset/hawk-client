#include "PhonixClient.h"

#include <QDebug>
#include <QEventLoop>
#include <QFile>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

#include "JsonParser.h"

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
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
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
        JsonParser parser(byte);
        if (parser.parse())
        {
            return false;
        }else
        {
            return true;
        }
        
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
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

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
        timer.stop();
        ListResultParser parser(byte);
        if (parser.parse())
        {
            if (parser.result())
            {
                if (parser.getArray("list", array))
                {
                    return true;
                }
                return false;
            }
            else
            {
                m_err = parser.errorMessage(); 
            }
            
        }
        return false;
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
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
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
        JsonParser parser(byte);
        if (parser.parse())
        {
            qDebug() << byte;
            m_err = parser.errorMessage();
            return parser.result();
        }
        m_err = "Invalid server message";
        return false;
    }
    else
    {
        reply->abort();
        return false;
    }
}

bool PhonixClient::postPicture(const QString &filePath)
{
    QUrl urlPath(filePath);
    QFile file(urlPath.toLocalFile());
    if (!file.open(QFile::ReadOnly))
    {
        return false;
    }
    bool b = postPicture("key", "id", file.readAll());
    file.close();
    return b;
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
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

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
        JsonParser parser(byte);
        if (parser.parse())
        {
            m_err = parser.errorMessage();
            return parser.result();
        }
        m_err = "Invalid server message";
        return false;
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
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    QNetworkReply * reply = m_manager->deleteResource(request);
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
        JsonParser parser(byte);
        if (parser.parse())
        {
            m_err = parser.errorMessage();
            return parser.result();
        }
        m_err = "Invalid server message";
        return false;
    }
    else
    {
        reply->abort();
        return false;
    }
    return true;
}

QString PhonixClient::error() const
{
    return m_err;
}
