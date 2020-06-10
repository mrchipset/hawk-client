#ifndef PHONIXCLIENT_H
#define PHONIXCLIENT_H

#include <QByteArray>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QObject>
#include <QString>

class PhonixClient : public QObject
{
    Q_OBJECT
public:
    explicit PhonixClient(const QString& serverURL, QObject *parent = nullptr);
    bool getPicture(const QString& key, const QString& id, QByteArray& byte);
    bool getPictureList(int pageSize, int pageId, QJsonArray& array);
    bool postPicture(const QString& key, const QString& id, const QByteArray& byte);
    bool putPicture(const QString& key, const QString& id, const QByteArray& byte);
    bool delPicture(const QString& key, const QString& id);
signals:

private:
    const QString m_kBaseURL = "PictureItems/";
    QString m_serverURL;
    QNetworkAccessManager* m_manager;
};

#endif // PHONIXCLIENT_H
