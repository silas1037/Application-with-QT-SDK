#include "widget.h"
#include "ui_widget.h"
#include <QtGui>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->prjpathLineEdit->setText(qApp->applicationDirPath());
    ui->prjpathLineEdit->setReadOnly(true);

    m_showTextEdit = new QTextEdit;
    m_showTextEdit->setFixedHeight(220);
    m_showTextEdit->setReadOnly(true);


    cmd = new QProcess;
    output = "";
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addWidget(m_showTextEdit,0, Qt::AlignBottom);
    setLayout(vlayout);
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    setFixedSize(540, 420);


    connect(cmd, SIGNAL(readyRead()), this, SLOT(readOutput()));
    connect(m_showTextEdit, SIGNAL(textChanged()), this, SLOT(autoScroll()));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event)
{
    switch( QMessageBox::information( this, tr("�ر���ʾ"),
                                     tr("ȷ���˳�����"),tr("ȷ��"), tr("ȡ��"),0, 1))
    {
    case 0:
        event->accept();
        break;
    case 1:
    default:
        event->ignore();
        break;
    }
}

void Widget::autoScroll()
{
   QTextCursor cursor = m_showTextEdit->textCursor();
   cursor.movePosition(QTextCursor::End);
   m_showTextEdit->setTextCursor(cursor);

}

void Widget::on_prjpathBtn_clicked()
{
    QString dirpath = QFileDialog::getExistingDirectory(this, tr("��Ŀ¼"),
                                                       ui->prjpathLineEdit->text(),
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);

    if(!dirpath.isEmpty())
    {
        ui->prjpathLineEdit->setText(dirpath);
    }
}


void Widget::readOutput()
{
    output += cmd->readAll();
    m_showTextEdit->setText(output);
}

void Widget::on_runBtn_clicked()
{
    output = "";

    QString path = ui->prjpathLineEdit->text();
    path.replace("/","\\");//����ַ�е�"/"�滻Ϊ"\"����Ϊ��Windows��ʹ�õ���"\"��

    if(ui->sysverlineEdit->text().isEmpty())
    {
        QMessageBox::information(this,tr("������ʾ"),"��������Ϊ��,����ÿ�չ����汾����",tr("ȷ��"));
        QString str = "6130dailybuild.bat " + path;
        qDebug()<<str<<endl;
        cmd->start(str);
        m_showTextEdit->setText(output);
        return;


    }

    QString str = "6130dailybuild.bat " + path \
                                   +" " + ui->sysverlineEdit->text();
    qDebug()<<str<<endl;
    cmd->start(str);
    m_showTextEdit->setText(output);
}


