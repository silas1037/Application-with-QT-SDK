#include "widget.h"
#include "ui_widget.h"
#include <QtSql>
#include <QtWidgets>
#include "pieview.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setFixedSize(750, 500);
    ui->stackedWidget->setCurrentIndex(0);

    QSqlQueryModel *typeModel = new QSqlQueryModel(this);
    typeModel->setQuery("select name from type");
    ui->sellTypeComboBox->setModel(typeModel);

    QSplitter *splitter = new QSplitter(ui->managePage);
    splitter->resize(700, 360);
    splitter->move(0, 50);

    splitter->addWidget(ui->toolBox);
    splitter->addWidget(ui->dailyList);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    on_sellCancelBtn_clicked();
    on_goodsCancelBtn_clicked();
    on_newCancelBtn_clicked();

    showDailyList();

    ui->typeComboBox->setModel(typeModel);
    ui->goodsTypeComboBox->setModel(typeModel);
    ui->newTypeComboBox->setModel(typeModel);

    createChartModelView();

}

Widget::~Widget()
{
    delete ui;
}

// 出售商品的商品类型改变时
void Widget::on_sellTypeComboBox_currentIndexChanged(QString type)
{
    if (type == "请选择类型")
    {
        // 进行其他部件的状态设置
        on_sellCancelBtn_clicked();
    }
    else
    {
        ui->sellBrandComboBox->setEnabled(true);
        QSqlQueryModel *sellBrandModel = new QSqlQueryModel(this);
        sellBrandModel->setQuery(QString("select name from brand where type='%1'").arg(type));
        ui->sellBrandComboBox->setModel(sellBrandModel);
        ui->sellCancelBtn->setEnabled(true);
    }
}

// 已有商品入库的商品类型改变时
void Widget::on_goodsTypeComboBox_currentIndexChanged(QString type)
{
    if (type == "请选择类型")
    {
        // 进行其他部件的状态设置
        on_goodsCancelBtn_clicked();
    }
    else
    {
        ui->goodsBrandComboBox->setEnabled(true);
        QSqlQueryModel *goodBrandModel = new QSqlQueryModel(this);
        goodBrandModel->setQuery(QString("select name from brand where type='%1'").arg(type));
        ui->goodsBrandComboBox->setModel(goodBrandModel);
        ui->goodsCancelBtn->setEnabled(true);
    }
}

// 新商品入库类型改变时
void Widget::on_newTypeComboBox_currentIndexChanged(QString type)
{
    if (type == "请选择类型")
    {
        // 进行其他部件的状态设置
        on_newCancelBtn_clicked();
    }
    else
    {
        ui->newBrandLineEdit->setEnabled(true);
        ui->newBrandLineEdit->setFocus();
    }
}

// 出售商品的品牌改变时
void Widget::on_sellBrandComboBox_currentIndexChanged(QString brand)
{
    ui->sellNumSpinBox->setValue(0);
    ui->sellNumSpinBox->setEnabled(false);
    ui->sellSumLineEdit->clear();
    ui->sellSumLineEdit->setEnabled(false);
    ui->sellOkBtn->setEnabled(false);

    QSqlQuery query;
    query.exec(QString("select price from brand where name='%1' and type='%2'")
               .arg(brand).arg(ui->sellTypeComboBox->currentText()));
    query.next();
    ui->sellPriceLineEdit->setEnabled(true);
    ui->sellPriceLineEdit->setReadOnly(true);
    ui->sellPriceLineEdit->setText(query.value(0).toString());

    query.exec(QString("select last from brand where name='%1' and type='%2'")
               .arg(brand).arg(ui->sellTypeComboBox->currentText()));
    query.next();
    int num = query.value(0).toInt();

    if (num == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("该商品已经售完！"), QMessageBox::Ok);
    }
    else
    {
        ui->sellNumSpinBox->setEnabled(true);
        ui->sellNumSpinBox->setMaximum(num);
        ui->sellLastNumLabel->setText(tr("剩余数量：%1").arg(num));
        ui->sellLastNumLabel->setVisible(true);
    }
}

// 已有商品入库的品牌改变时
void Widget::on_goodsBrandComboBox_currentIndexChanged(QString brand)
{
    ui->goodsNumSpinBox->setValue(0);
    ui->goodsNumSpinBox->setEnabled(true);
    ui->goodsSumLineEdit->clear();
    ui->goodsSumLineEdit->setEnabled(false);
    ui->goodsOkBtn->setEnabled(false);

    QSqlQuery query;
    query.exec(QString("select price from brand where name='%1' and type='%2'")
               .arg(brand).arg(ui->goodsTypeComboBox->currentText()));
    query.next();
    ui->goodsPriceLineEdit->setEnabled(true);
    ui->goodsPriceLineEdit->setReadOnly(true);
    ui->goodsPriceLineEdit->setText(query.value(0).toString());
}

// 新商品品牌改变时
void Widget::on_newBrandLineEdit_textChanged(QString str)
{
    if (str == "")
    {
        ui->newCancelBtn->setEnabled(false);
        ui->newPriceSpinBox->setEnabled(false);
        ui->newNumSpinBox->setEnabled(false);
        ui->newSumLineEdit->setEnabled(false);
        ui->newSumLineEdit->clear();
        ui->newOkBtn->setEnabled(false);
    }
    else
    {
        ui->newCancelBtn->setEnabled(true);
        ui->newPriceSpinBox->setEnabled(true);
        ui->newNumSpinBox->setEnabled(true);
        ui->newSumLineEdit->setEnabled(true);
        qreal sum = ui->newPriceSpinBox->value() * ui->newNumSpinBox->value();
        ui->newSumLineEdit->setText(QString::number(sum));
        ui->newOkBtn->setEnabled(true);
    }
}

// 出售商品数量改变时
void Widget::on_sellNumSpinBox_valueChanged(int value)
{
    if (value == 0)
    {
        ui->sellSumLineEdit->clear();
        ui->sellSumLineEdit->setEnabled(false);
        ui->sellOkBtn->setEnabled(false);
    }
    else
    {
        ui->sellSumLineEdit->setEnabled(true);
        ui->sellSumLineEdit->setReadOnly(true);
        qreal sum = value * ui->sellPriceLineEdit->text().toInt();
        ui->sellSumLineEdit->setText(QString::number(sum));
        ui->sellOkBtn->setEnabled(true);
    }
}

// 已有商品入库数量改变时
void Widget::on_goodsNumSpinBox_valueChanged(int value)
{
    if (value == 0)
    {
        ui->goodsSumLineEdit->clear();
        ui->goodsSumLineEdit->setEnabled(false);
        ui->goodsOkBtn->setEnabled(false);
    }
    else
    {
        ui->goodsSumLineEdit->setEnabled(true);
        ui->goodsSumLineEdit->setReadOnly(true);
        qreal sum = value * ui->goodsPriceLineEdit->text().toInt();
        ui->goodsSumLineEdit->setText(QString::number(sum));
        ui->goodsOkBtn->setEnabled(true);
    }

}

// 新商品单价改变时
void Widget::on_newPriceSpinBox_valueChanged(int value)
{
    qreal sum = value * ui->newNumSpinBox->value();
    ui->newSumLineEdit->setText(QString::number(sum));
    ui->newOkBtn->setEnabled(true);
}

// 新商品数量
void Widget::on_newNumSpinBox_valueChanged(int value)
{
    qreal sum = value * ui->newPriceSpinBox->value();
    ui->newSumLineEdit->setText(QString::number(sum));
    ui->newOkBtn->setEnabled(true);
}

// 出售商品的取消按钮
void Widget::on_sellCancelBtn_clicked()
{
    ui->sellTypeComboBox->setCurrentIndex(0);
    ui->sellBrandComboBox->clear();
    ui->sellBrandComboBox->setEnabled(false);
    ui->sellPriceLineEdit->clear();
    ui->sellPriceLineEdit->setEnabled(false);
    ui->sellNumSpinBox->setValue(0);
    ui->sellNumSpinBox->setEnabled(false);
    ui->sellSumLineEdit->clear();
    ui->sellSumLineEdit->setEnabled(false);
    ui->sellOkBtn->setEnabled(false);
    ui->sellCancelBtn->setEnabled(false);
    ui->sellLastNumLabel->setVisible(false);
}

// 已有商品入库的取消按钮
void Widget::on_goodsCancelBtn_clicked()
{
    ui->goodsTypeComboBox->setCurrentIndex(0);
    ui->goodsBrandComboBox->clear();
    ui->goodsBrandComboBox->setEnabled(false);
    ui->goodsPriceLineEdit->clear();
    ui->goodsPriceLineEdit->setEnabled(false);
    ui->goodsNumSpinBox->setValue(0);
    ui->goodsNumSpinBox->setEnabled(false);
    ui->goodsSumLineEdit->clear();
    ui->goodsSumLineEdit->setEnabled(false);
    ui->goodsOkBtn->setEnabled(false);
    ui->goodsCancelBtn->setEnabled(false);
}

// 新商品入库的取消按钮
void Widget::on_newCancelBtn_clicked()
{
    ui->newTypeComboBox->setCurrentIndex(0);
    ui->newBrandLineEdit->clear();
    ui->newBrandLineEdit->setEnabled(false);
    ui->newPriceSpinBox->setEnabled(false);
    ui->newPriceSpinBox->setValue(1);
    ui->newNumSpinBox->setEnabled(false);
    ui->newNumSpinBox->setValue(1);
    ui->newSumLineEdit->setText("1");
    ui->newSumLineEdit->setEnabled(false);
    ui->newOkBtn->setEnabled(false);
    ui->newCancelBtn->setEnabled(false);
}


// 出售商品的确定按钮
void Widget::on_sellOkBtn_clicked()
{
    QString type = ui->sellTypeComboBox->currentText();
    QString name = ui->sellBrandComboBox->currentText();
    int value = ui->sellNumSpinBox->value();
    // cellNumSpinBox的最大值就是以前的剩余量
    int last = ui->sellNumSpinBox->maximum() - value;

    QSqlQuery query;

    // 获取以前的销售量
    query.exec(QString("select sell from brand where name='%1' and type='%2'")
               .arg(name).arg(type));
    query.next();
    int sell = query.value(0).toInt() + value;

    // 事务操作
    QSqlDatabase::database().transaction();
    bool rtn = query.exec(
                QString("update brand set sell=%1,last=%2 where name='%3' and type='%4'")
                .arg(sell).arg(last).arg(name).arg(type));

    if (rtn) {
        QSqlDatabase::database().commit();
        QMessageBox::information(this, tr("提示"), tr("购买成功！"), QMessageBox::Ok);
        writeXml();
        showDailyList();
    } else {
        QSqlDatabase::database().rollback();
        QMessageBox::information(this, tr("提示"), tr("购买失败，无法访问数据库！"), QMessageBox::Ok);
    }
    on_sellCancelBtn_clicked();
}

// 已有商品入库的确定按钮
void Widget::on_goodsOkBtn_clicked()
{
    QString type = ui->goodsTypeComboBox->currentText();
    QString name = ui->goodsBrandComboBox->currentText();
    int value = ui->goodsNumSpinBox->value();

    QSqlQuery query;
    // 获取以前的总量
    query.exec(QString("select sum from brand where name='%1' and type='%2'")
               .arg(name).arg(type));
    query.next();
    int sum = query.value(0).toInt() + value;

    // 获取以前的剩余量
    query.exec(QString("select last from brand where name='%1' and type='%2'")
               .arg(name).arg(type));
    query.next();
    int last = query.value(0).toInt() + value;

    // 事务操作
    QSqlDatabase::database().transaction();
    bool rtn = query.exec(
                QString("update brand set sum=%1,last=%2 where name='%3' and type='%4'")
                .arg(sum).arg(last).arg(name).arg(type));
    if (rtn)
    {
        QSqlDatabase::database().commit();
        QMessageBox::information(this, tr("提示"), tr("入库成功！"), QMessageBox::Ok);
    }
    else
    {
        QSqlDatabase::database().rollback();
        QMessageBox::information(this, tr("提示"), tr("入库失败，无法访问数据库！"), QMessageBox::Ok);
    }
    on_goodsCancelBtn_clicked();
}

// 新商品的确定按钮
void Widget::on_newOkBtn_clicked()
{
    QString type = ui->newTypeComboBox->currentText();
    QString brand = ui->newBrandLineEdit->text();
    qint16 price = ui->newPriceSpinBox->value();
    qint16 num = ui->newNumSpinBox->value();

    QSqlQuery query;
    query.exec("select id from brand");
    query.last();
    qreal temp = query.value(0).toInt() + 1;

    QString id;
    if (temp < 10)
    {
        id = "0" + QString::number(temp);
    }
    else
    {
        id = QString::number(temp);
    }

    qDebug() << "hello" <<id << type << brand << price << num;


    // 事务操作
    QSqlDatabase::database().transaction();
    bool rtn = query.exec(QString("insert into brand values('%1', '%2', '%3', %4, %5, 0, %6)")
                .arg(id).arg(brand).arg(type).arg(price).arg(num).arg(num));
    if (rtn)
    {
        QSqlDatabase::database().commit();
        QMessageBox::information(this, tr("提示"), tr("入库成功！"), QMessageBox::Ok);
    }
    else
    {
        QSqlDatabase::database().rollback();
        QMessageBox::information(this, tr("提示"), tr("入库失败，无法访问数据库！"), QMessageBox::Ok);
    }

    on_newCancelBtn_clicked();
}

// 获取当前的日期或者时间
QString Widget::getDateTime(Widget::DateTimeType type)
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString date = datetime.toString("yyyy-MM-dd");
    QString time = datetime.toString("hh:mm");
    QString dateAndTime = datetime.toString("yyyy-MM-dd dddd hh:mm");
    if(type == Date) return date;
    else if(type == Time) return time;
    else return dateAndTime;
}

// 读取XML文档
bool Widget::docRead()
{
    QFile file("data.xml");
    if(!file.open(QIODevice::ReadOnly))
        return false;
    if(!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    file.close();
    return true;
}

// 写入xml文档
bool Widget::docWrite()
{
    QFile file("data.xml");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;
    QTextStream out(&file);
    doc.save(out,4);
    file.close();
    return true;
}

// 将销售记录写入文档
void Widget::writeXml()
{
    // 先从文件读取
    if (docRead())
    {
        QString currentDate = getDateTime(Date);
        QDomElement root = doc.documentElement();
        // 根据是否有日期节点进行处理
        if (!root.hasChildNodes())
        {
            QDomElement date = doc.createElement(QString("日期"));
            QDomAttr curDate = doc.createAttribute("date");
            curDate.setValue(currentDate);
            date.setAttributeNode(curDate);
            root.appendChild(date);
            createNodes(date);
        }
        else
        {
            QDomElement date = root.lastChild().toElement();
            // 根据是否已经有今天的日期节点进行处理
            if (date.attribute("date") == currentDate)
            {
                createNodes(date);
            }
            else
            {
                QDomElement date = doc.createElement(QString("日期"));
                QDomAttr curDate = doc.createAttribute("date");
                curDate.setValue(currentDate);
                date.setAttributeNode(curDate);
                root.appendChild(date);
                createNodes(date);
            }
        }
        // 写入到文件
        docWrite();
    }
}

// 创建节点
void Widget::createNodes(QDomElement &date)
{
    QDomElement time = doc.createElement(QString("时间"));
    QDomAttr curTime = doc.createAttribute("time");
    curTime.setValue(getDateTime(Time));
    time.setAttributeNode(curTime);
    date.appendChild(time);

    QDomElement type = doc.createElement(QString("类型"));
    QDomElement brand = doc.createElement(QString("品牌"));
    QDomElement price = doc.createElement(QString("单价"));
    QDomElement num = doc.createElement(QString("数量"));
    QDomElement sum = doc.createElement(QString("金额"));
    QDomText text;
    text = doc.createTextNode(QString("%1")
                              .arg(ui->sellTypeComboBox->currentText()));
    type.appendChild(text);
    text = doc.createTextNode(QString("%1")
                              .arg(ui->sellBrandComboBox->currentText()));
    brand.appendChild(text);
    text = doc.createTextNode(QString("%1")
                              .arg(ui->sellPriceLineEdit->text()));
    price.appendChild(text);
    text = doc.createTextNode(QString("%1")
                              .arg(ui->sellNumSpinBox->value()));
    num.appendChild(text);
    text = doc.createTextNode(QString("%1")
                              .arg(ui->sellSumLineEdit->text()));
    sum.appendChild(text);

    time.appendChild(type);
    time.appendChild(brand);
    time.appendChild(price);
    time.appendChild(num);
    time.appendChild(sum);
}


// 显示日销售清单
void Widget::showDailyList()
{
    ui->dailyList->clear();
    if (docRead())
    {
        QDomElement root = doc.documentElement();
        QString title = root.tagName();
        QListWidgetItem *titleItem = new QListWidgetItem;
        titleItem->setText(QString("-----%1-----").arg(title));
        titleItem->setTextAlignment(Qt::AlignCenter);
        ui->dailyList->addItem(titleItem);

        if (root.hasChildNodes())
        {
            QString currentDate = getDateTime(Date);
            QDomElement dateElement = root.lastChild().toElement();
            QString date = dateElement.attribute("date");
            if (date == currentDate)
            {
                ui->dailyList->addItem("");
                ui->dailyList->addItem(QString("日期：%1").arg(date));
                ui->dailyList->addItem("");
                QDomNodeList children = dateElement.childNodes();
                // 遍历当日销售的所有商品
                for (int i=0; i<children.count(); i++)
                {
                    QDomNode node = children.at(i);
                    QString time = node.toElement().attribute("time");
                    QDomNodeList list = node.childNodes();
                    QString type = list.at(0).toElement().text();
                    QString brand = list.at(1).toElement().text();
                    QString price = list.at(2).toElement().text();
                    QString num = list.at(3).toElement().text();
                    QString sum = list.at(4).toElement().text();

                    QString str = time + " 出售 " + brand + type
                            + " " + num + "台， " + "单价：" + price
                            + "元， 共" + sum + "元";
                    QListWidgetItem *tempItem = new QListWidgetItem;
                    tempItem->setText("**************************");
                    tempItem->setTextAlignment(Qt::AlignCenter);
                    ui->dailyList->addItem(tempItem);
                    ui->dailyList->addItem(str);
                }
            }
        }
    }
}


// 创建销售记录图表的模型和视图
void Widget::createChartModelView()
{
    chartModel = new QStandardItemModel(this);
    chartModel->setColumnCount(2);
    chartModel->setHeaderData(0, Qt::Horizontal, QString("品牌"));
    chartModel->setHeaderData(1, Qt::Horizontal, QString("销售数量"));

    QSplitter *splitter = new QSplitter(ui->chartPage);
    splitter->resize(700, 320);
    splitter->move(0, 80);
    QTableView *table = new QTableView;
    PieView *pieChart = new PieView;
    splitter->addWidget(table);
    splitter->addWidget(pieChart);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    table->setModel(chartModel);
    pieChart->setModel(chartModel);

    QItemSelectionModel *selectionModel = new QItemSelectionModel(chartModel);
    table->setSelectionModel(selectionModel);
    pieChart->setSelectionModel(selectionModel);
}

// 显示销售记录图表
void Widget::showChart()
{
    QSqlQuery query;
    query.exec(QString("select name,sell from brand where type='%1'")
               .arg(ui->typeComboBox->currentText()));

    chartModel->removeRows(0, chartModel->rowCount(QModelIndex()), QModelIndex());

    int row = 0;

    while(query.next())
    {
        int r = qrand() % 256;
        int g = qrand() % 256;
        int b = qrand() % 256;

        chartModel->insertRows(row, 1, QModelIndex());

        chartModel->setData(chartModel->index(row, 0, QModelIndex()),
                            query.value(0).toString());
        chartModel->setData(chartModel->index(row, 1, QModelIndex()),
                            query.value(1).toInt());
        chartModel->setData(chartModel->index(row, 0, QModelIndex()),
                            QColor(r, g, b), Qt::DecorationRole);
        row++;
    }
}

// 销售统计页面的类型选择框
void Widget::on_typeComboBox_currentIndexChanged(QString type)
{
    if (type != "请选择类型")
        showChart();
}

// 更新显示按钮
void Widget::on_updateBtn_clicked()
{
    if (ui->typeComboBox->currentText() != "请选择类型")
        showChart();
}

// 商品管理按钮
void Widget::on_manageBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// 销售统计按钮
void Widget::on_chartBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// 修改密码按钮
void Widget::on_passwordBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// 修改密码确定按钮
void Widget::on_changePwdBtn_clicked()
{
    if (ui->oldPwdLineEdit->text().isEmpty() ||
            ui->newPwdLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请将信息填写完整！"),
                             QMessageBox::Ok);
    }
    else
    {
        QSqlQuery query;
        query.exec("select pwd from login");
        query.next();
        if (query.value(0).toString() == ui->oldPwdLineEdit->text())
        {
            bool temp = query.exec(QString("update password set pwd='%1' where pwd='%2'")
                                   .arg(ui->newPwdLineEdit->text()).arg(ui->oldPwdLineEdit->text()));
            if (temp)
            {
                QMessageBox::information(this, tr("提示"), tr("密码修改成功！"),
                                         QMessageBox::Ok);
                ui->oldPwdLineEdit->clear();
                ui->newPwdLineEdit->clear();
            }
            else
            {
                QMessageBox::information(this, tr("提示"), tr("密码修改失败，无法访问数据库！"),
                                         QMessageBox::Ok);
            }
        }
        else
        {
            QMessageBox::warning(this, tr("警告"), tr("原密码错误，请重新填写！"),
                                 QMessageBox::Ok);
            ui->oldPwdLineEdit->clear();
            ui->newPwdLineEdit->clear();
            ui->oldPwdLineEdit->setFocus();
        }
    }

}
