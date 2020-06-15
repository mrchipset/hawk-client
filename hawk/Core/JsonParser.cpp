#include "JsonParser.h"


bool JsonUtility::str2Json(const QByteArray& text, QJsonDocument & doc)
{
    QJsonParseError err;
    doc = QJsonDocument::fromJson(text, &err);
    if (err.error != QJsonParseError::NoError)
    {
        return false;
    }
    return true;
}

JsonParser::JsonParser(const QByteArray& text, QObject * parent) : QObject(parent)
{
    m_content = text;
}

bool JsonParser::parse()
{
    QJsonDocument doc;
    if (JsonUtility::str2Json(m_content, doc))
    {
        m_result = doc.object();
        return true;
    }
    return false;
}

bool JsonParser::result()
{
    if (m_result.contains("status"))
    {
        return m_result.value("status").toBool();
    }
    return false;
}

QString JsonParser::errorMessage()
{
    QString err = "UnKnown Error";
    if (m_result.contains("message"))
    {
        err = m_result.value("message").toString();
    }
    return err;
}

ListResultParser::ListResultParser(const QByteArray& text, QObject * parent) :
    JsonParser(text, parent)
{

}

bool ListResultParser::getArray(const QString& key, QJsonArray & array)
{
    if (m_result.contains(key))
    {
        array = m_result.value(key).toArray();
        return true;
    }
    return false;
}