#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QObject>

class JsonUtility
{
public:
    static bool str2Json(const QByteArray& text, QJsonDocument & doc);
};

class JsonParser : public QObject
{
    Q_OBJECT
public:
    JsonParser(const QByteArray& text, QObject * parent=nullptr);
    virtual bool parse();
    virtual bool result();
    QString errorMessage();
private:
    QByteArray m_content;
protected:
    QJsonObject m_result;
};

class ListResultParser : public JsonParser
{
    Q_OBJECT
public:
    ListResultParser(const QByteArray& text, QObject * parent=nullptr);
    virtual bool getArray(const QString& key, QJsonArray & array);
};