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
    Q_INVOKABLE bool getPicture(const QString& key, const QString& id, QByteArray& byte);
    Q_INVOKABLE bool getPictureList(int pageSize, int pageId, QJsonArray& array);
    Q_INVOKABLE bool postPicture(const QString& key, const QString& id, const QByteArray& byte);
    Q_INVOKABLE bool postPicture(const QString& filePath);
    Q_INVOKABLE bool putPicture(const QString& key, const QString& id, const QByteArray& byte);
    Q_INVOKABLE bool delPicture(const QString& key, const QString& id);
    Q_INVOKABLE QString error() const;
signals:

private:
    const QString m_kBaseURL = "PictureItems/";
    QString m_serverURL;
    QNetworkAccessManager* m_manager;
    QString m_err;
};

#endif // PHONIXCLIENT_H
