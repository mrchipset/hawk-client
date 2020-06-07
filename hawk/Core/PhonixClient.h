#ifndef PHONIXCLIENT_H
#define PHONIXCLIENT_H

#include <QObject>

class PhonixClient : public QObject
{
    Q_OBJECT
public:
    explicit PhonixClient(QObject *parent = nullptr);

signals:

};

#endif // PHONIXCLIENT_H
