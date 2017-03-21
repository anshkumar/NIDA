#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H
#include <QObject>
#include <qjsonrpc/qjsonrpchttpserver.h>
#include <qjsonrpc/qjsonrpchttpclient.h>
#include <QAuthenticator>

class HttpClient : public QJsonRpcHttpClient
{
    Q_OBJECT
   public:
       HttpClient(const QString &endpoint, QObject *parent = 0)
           : QJsonRpcHttpClient(endpoint, parent)
       {
           // defaults added for my local test server
           m_username = "bitcoinrpc";
           m_password = "232fb3276bbb7437d265298ea48bdc46";
       }

       void setUsername(const QString &username) {
           m_username = username;
       }

       void setPassword(const QString &password) {
           m_password = password;
       }

   private Q_SLOTS:
       virtual void handleAuthenticationRequired(QNetworkReply *reply, QAuthenticator * authenticator)
       {
           Q_UNUSED(reply);
           authenticator->setUser(m_username);
           authenticator->setPassword(m_password);
       }

   private:
       QString m_username;
       QString m_password;
};

#endif // HTTPCLIENT_H
