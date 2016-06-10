#include "qconfigmenu.h"
#include "qclient.h"
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QTextEdit>
#include <QComboBox>
#include <QProcess>
#include <QFile>
#include <QFileInfo>
#include <QRegExp>
#include <QSpinBox>
#include <QCloseEvent>
#include <QTextStream>

QConfigMenu::QConfigMenu()
{
    m_groupBox = new QGroupBox(tr("��Ԫ��������"), this);

    QRegExp regExp("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");

    m_hostIPlabel = new QLabel(tr("��������ip��ַ"));
    m_hostIPEdit = new QLineEdit(tr("192.192.192.198"));
    m_hostIPEdit->setValidator(new QRegExpValidator(regExp,this));

    m_loopbacklabel = new QLabel(tr("���˻���ip��ַ"));
    m_loopbackEdit = new QLineEdit(tr("1.1.1.1"));
    m_loopbackEdit->setValidator(new QRegExpValidator(regExp,this));

    m_peerloopbacklabel = new QLabel(tr("�Զ˻���ip��ַ"));
    m_peerloopbackEdit = new QLineEdit(tr("2.2.2.2"));
    m_peerloopbackEdit->setValidator(new QRegExpValidator(regExp,this));


    m_vlanlabel = new QLabel(tr("����VLAN��ip��ַ"));
    m_vlanEdit = new QLineEdit(tr("100.0.0.1"));
    m_vlanEdit->setValidator(new QRegExpValidator(regExp,this));

    m_peervlanlabel = new QLabel(tr("�Զ�VLAN��ip��ַ"));
    m_peervlanEdit = new QLineEdit(tr("100.0.0.2"));
    m_peervlanEdit->setValidator(new QRegExpValidator(regExp,this));



    m_peerMacLabel = new QLabel(tr("�Զ��豸MAC��ַ"));
    QRegExp peerMacRegExp("[0-9a-f]{4}\\.[0-9a-f]{4}\\.[0-9a-f]{4}");
    m_peerMacEdit = new QLineEdit(tr("00d0.d0c0.0081"));
    m_peerMacEdit->setValidator(new QRegExpValidator(peerMacRegExp,this));
    m_peerMacEdit->setToolTip(tr("MAC��ַ��д�����ڶԶ��豸������show lacp sys-id�õ�����Ҫ�����λ��1"));




    m_cepSlotNoLabel = new QLabel(tr("CEP�߿����ڲ�λ��"));
    m_cepSlotNoBox = new QSpinBox;
    m_cepSlotNoBox->setRange(1,12);
    m_cepSlotNoBox->setValue(7);



    m_cepPortNoLabel = new QLabel(tr("CEPҵ��󶨶˿ں�"));
    m_cepPortNoBox = new QComboBox;
    m_cepPortNoBox->addItem(tr("1"));
    m_cepPortNoBox->addItem(tr("2"));
    m_cepPortNoBox->addItem(tr("3"));
    m_cepPortNoBox->addItem(tr("4"));


    m_geSlotNoLabel = new QLabel(tr("GE�߿����ڲ�λ��"));
    m_geSlotNoBox = new QSpinBox;
    m_geSlotNoBox->setRange(1,12);
    m_geSlotNoBox->setValue(3);
    m_gePortNoLabel = new QLabel(tr("GE�߿�ת���˿ں�"));
    m_gePortNoBox = new QComboBox;
    m_gePortNoBox->addItem(tr("1"));
    m_gePortNoBox->addItem(tr("2"));
    m_gePortNoBox->addItem(tr("3"));
    m_gePortNoBox->addItem(tr("4"));
    m_gePortNoBox->addItem(tr("5"));
    m_gePortNoBox->addItem(tr("6"));
    m_gePortNoBox->addItem(tr("7"));
    m_gePortNoBox->addItem(tr("8"));

    m_configBox = new QGroupBox(tr("��Ԫ���ò���"), this);
    m_configBox->setFixedSize(450,60);
    m_configBox->setToolTip("���մ����ҵ�˳�����β��������������ýű���Ȼ�������ýű��������ִ�����ýű���");
    mkConfigBtn = new QPushButton(tr("���������ļ�"));
    mkConfigBtn->setFixedWidth(100);
    m_showConfigBtn = new QPushButton(tr("������ýű�"));
    m_showConfigBtn->setFixedWidth(100);

    m_runBtn = new QPushButton(tr("����ҵ��ʵ��"));
    m_runBtn->setFixedWidth(100);
    client = new QClient;//���Telnet�Զ����ý���


    m_runConfigEdit = new QTextEdit;//������Ϣ����ı���
    m_runConfigEdit->setReadOnly(true);
    m_runConfigEdit->setFixedSize(480,320);
    m_runConfigEdit->setWindowTitle(tr("CEP���ýű�"));
    m_runConfigEdit->setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);

    QGridLayout *glayout = new QGridLayout;

    glayout->addWidget(m_hostIPlabel,1,0);
    glayout->addWidget(m_hostIPEdit,1,1);
    glayout->addWidget(m_loopbacklabel,2,0);
    glayout->addWidget(m_loopbackEdit,2,1);
    glayout->addWidget(m_peerloopbacklabel,3,0);
    glayout->addWidget(m_peerloopbackEdit,3,1);
    glayout->addWidget(m_vlanlabel,4,0);
    glayout->addWidget(m_vlanEdit,4,1);
    glayout->addWidget(m_peervlanlabel,5,0);
    glayout->addWidget(m_peervlanEdit,5,1);
    glayout->addWidget(m_peerMacLabel,6,0);
    glayout->addWidget(m_peerMacEdit,6,1);
    glayout->addWidget(m_cepSlotNoLabel,7,0);
    glayout->addWidget(m_cepSlotNoBox,7,1);
    glayout->addWidget(m_cepPortNoLabel,8,0);
    glayout->addWidget(m_cepPortNoBox,8,1);
    glayout->addWidget(m_geSlotNoLabel,9,0);
    glayout->addWidget(m_geSlotNoBox,9,1);
    glayout->addWidget(m_gePortNoLabel,10,0);
    glayout->addWidget(m_gePortNoBox,10,1);
    m_groupBox->setLayout(glayout);



    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(mkConfigBtn);
    hlayout->addWidget(m_showConfigBtn);
    hlayout->addWidget(m_runBtn);
    m_configBox->setLayout(hlayout);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(m_groupBox);
    vlayout->addWidget(m_configBox);
    setLayout(vlayout);



    setFixedSize(480,480);
    setWindowTitle(tr("��ԪCEPҵ�����ù���"));
    setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);

    connect(m_showConfigBtn, SIGNAL(clicked()), this,SLOT(on_showConfigBtn()));
    connect(m_runBtn, SIGNAL(clicked()), this, SLOT(on_runBtn()));
    connect(mkConfigBtn, SIGNAL(clicked()), this, SLOT(on_mkConfigBtn()));
//    connect(m_loopbackEdit,SIGNAL(textChanged(QString)), this, SLOT(on_mkConfigBtn()));
//    connect(m_peerloopbackEdit, SIGNAL(textChanged(QString)), this, SLOT(on_mkConfigBtn()));
//    connect(m_vlanEdit, SIGNAL(textChanged(QString)), this, SLOT(on_mkConfigBtn()));
//    connect(m_peervlanEdit, SIGNAL(textChanged(QString)), this, SLOT(on_mkConfigBtn()));
//    connect(m_peerMacEdit, SIGNAL(textChanged(QString)), this, SLOT(on_mkConfigBtn()));
    connect(m_cepSlotNoBox, SIGNAL(valueChanged(QString)), this, SLOT(on_mkConfigBtn()));
    connect(m_cepPortNoBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_mkConfigBtn()));
    connect(m_geSlotNoBox, SIGNAL(valueChanged(QString)), this, SLOT(on_mkConfigBtn()));
    connect(m_gePortNoBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_mkConfigBtn()));





}

QConfigMenu::~QConfigMenu()
{

}

void QConfigMenu::closeEvent(QCloseEvent *e)
{
    this->hide();
    e->ignore();
}

void QConfigMenu::on_runBtn()
{
    output = "";

    if(m_peerMacEdit->text().isEmpty())
    {
        QMessageBox::critical(this,tr("������ʾ"),"�������ò���Ϊ��",tr("ȷ��"));
        return;
    }

    client->setWindowModality(Qt::ApplicationModal);
    client->show();

//    QFileInfo runExeFile("autoconfigcep.exe");

//    if(runExeFile.exists() == false)
//    {
//       QMessageBox::critical(this,tr("ִ�г���"),"����CEPִ�г����𻵻��߲����ڣ����飡",tr("ȷ��"));
//    }
//    QString str = "autoconfigcep.exe " + m_hostIPEdit->text();


//    if(cmd->state() == QProcess::Running)
//    {
//        QMessageBox::critical(this,tr("����"),tr("����һ��ʵ�����������У����Ժ����ԣ�"),tr("ȷ��"));
//        return;
//    }
//    cmd->start(str);

//    outputEdit->setText(output);


}

void QConfigMenu::on_mkConfigBtn()
{
    QFile configfile("config.txt");
       if (!configfile.open(QIODevice::WriteOnly | QIODevice::Text))
       {
           QMessageBox::critical(this, tr("������ʾ"), tr("�޷����������ļ�"), tr("ȷ��"));
           return;
       }

       QTextStream out(&configfile);
       out << "who" << endl
           << "who" << endl
           << "en" << endl
           << "zxr10" << endl
           << "con t" << endl
           << "no tunnel 100" << endl
           << "no cip 10" << m_cepPortNoBox->currentText() <<endl
           << "no vll 10" << m_cepPortNoBox->currentText() <<endl
           << "no pw 10" << m_cepPortNoBox->currentText() <<endl
           << "interf loopback1" << endl
           << "ip add " << m_loopbackEdit->text() << " 255.255.255.255" << endl
           << "exit" << endl
           << "vlan 100" << endl
           << "exit"<< endl
           << "interface gei_" << m_geSlotNoBox->text() << "/" << m_gePortNoBox->currentText()<< endl
           << "switchport mode trunk" << endl
           << "switchport trunk vlan 100" <<endl
           << "exit"<< endl
           << "interface vlan 100" << endl
           << "ip address " << m_vlanEdit->text() << " 255.255.255.0" << endl
           << "set arp per " << m_peervlanEdit->text() << " " << m_peerMacEdit->text() << endl
           << "!"<< endl
           << "mac add per " << m_peerMacEdit->text() << " interface gei_"
           << m_geSlotNoBox->text() << "/" << m_gePortNoBox->currentText()<< " vlan 100" << endl
           << "mpls traffic-eng tunnels" << endl
           << "tunnel 100" << endl
           << "tu mode tr static" << endl
           << "tu static type bi role ingress" << endl
           << "tu static ingress " << m_loopbackEdit->text() <<" egress " << m_peerloopbackEdit->text()
           << " out-port gei_" << m_geSlotNoBox->text() << "/" << m_gePortNoBox->currentText()
           << " out-label 1000100 next-hop " << m_peervlanEdit->text() << endl
           << "tu static ingress " << m_peerloopbackEdit->text() <<" egress " << m_loopbackEdit->text()
           << " in-port gei_" << m_geSlotNoBox->text() << "/" << m_gePortNoBox->currentText()
           << " in-label 1000100" << endl
           << "tu enable" << endl
           << "exit" << endl
           << "controller stm1_" << m_cepSlotNoBox->text() << "/" << m_cepPortNoBox->currentText() << endl
           << "framing sdh" << endl
           << "!" << endl
           << "pw 10" << m_cepPortNoBox->currentText() << endl
           << "pwtype cep-packet"<< endl
           << "mode s" << endl
           << "peer " << m_peerloopbackEdit->text() << " vcid 10" << m_cepPortNoBox->currentText()
           << " local-label 100010" << m_cepPortNoBox->currentText()
           << " remote-label 100010" << m_cepPortNoBox->currentText() << endl
           << "tunnel 100" << endl
           << "apply pw-class 1:0:0" << endl
           << "encapsulated-delay 32" << endl
           << "sequence enable" << endl
           << "cep type basic" << endl
           << "payload 783" << endl
           << "exit" << endl
           << "vll 10" << m_cepPortNoBox->currentText() << endl
           << "service-type tdm" << endl
           << "mpls xconnect pw 10" << m_cepPortNoBox->currentText() <<endl
           << "exit" << endl
           << "cip 10" << m_cepPortNoBox->currentText() <<endl
           << "service-type tdm cep stm1_"
           <<  m_cepSlotNoBox->text() << "/" << m_cepPortNoBox->currentText() << " vc4 1" << endl
           <<  "xconnect 10" << m_cepPortNoBox->currentText() << endl
           << "exit" << endl;
       configfile.close();
       QMessageBox::information(this, tr("������ʾ"), tr("�ɹ�����CEP���ýű���"), tr("ȷ��"));



}

void QConfigMenu::on_showConfigBtn()
{

    QFile configfile("config.txt");
    if(!configfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("����ʾ"), tr("�����ļ�������,�������������ļ���"), tr("ȷ��"));
        return;
    }
    QTextStream in(&configfile);

    QString configOutput = in.readAll();

    m_runConfigEdit->setText(configOutput);
    m_runConfigEdit->setWindowModality(Qt::ApplicationModal);
    m_runConfigEdit->show();
    configfile.close();
}
