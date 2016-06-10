#ifndef QTELNET_H
#define QTELNET_H

#include <QtNetwork/QTcpSocket>
class QTimer;
class QFile;

class QTelnet:public QObject
{
    Q_OBJECT
public:
     QTelnet();//�û��������룬�豸IP���˿�23
    ~QTelnet();
     void telnetToHost(QString serverAddr, QString portNo = 23);
     void stop();
     void runCmds(QString cmds);//ִ������
     void runCmds(QFile *pfile); //����Ĭ�Ͻű�
     bool isConnected() const { return  isconnected;} //���������Ƿ�ɹ�
private slots:
    void slotreadMessage();
    void slotsendMessage();
    void slotshowLog();
    void slotgetStatus();
    void slotstartConnection(void);
    void slotdisplayError(QAbstractSocket::SocketError socketError);

private:
    void delaymsec(int msec);



private:
    QTcpSocket *m_pSocket;
    QString output;
    QString m_username;
    QString m_password;
    QString m_serverAddr;
    QString m_portNo;

    bool isconnected;
    bool loginflag;

    QTimer *loginTimer;
    QString cmds;
    QStringList filesList;


};

#endif // QTELNET_H
