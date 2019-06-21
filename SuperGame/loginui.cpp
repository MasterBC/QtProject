//local
#include "loginui.h"
#include "ui_loginui.h"
#include "network/handlemsg.h"

//Qt
#include <QUuid>
#include <QMouseEvent>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>


/////////////////////////
QString GetCPUManID()

{
    QString cpu_id = "";
    QProcess p(0);
    p.start("wmic CPU get ProcessorID");
    p.waitForStarted();
    p.waitForFinished();
    cpu_id = QString::fromLocal8Bit(p.readAllStandardOutput());
    cpu_id = cpu_id.remove("ProcessorId").trimmed();
    return cpu_id;
}
/////////////////////////

LoginUI::LoginUI(QWidget *parent) :
    UIbase(parent),
    ui(new Ui::LoginUI)
{
    ui->setupUi(this);
}

LoginUI::~LoginUI()
{
    delete ui;
}



///实现信息登录
void LoginUI::on_pushButtonEnter_clicked()
{
    // 获取用户名和密码
    //    QUuid id = QUuid::createUuid();
    //    QString strID = id.toString();
    //ui->comboBoxName->setCurrentText(strID);

    // QString CPU_ID = GetCPUManID();
    //ui->lineEditPassword->setText(CPU_ID);
    // 账号
    QString account = ui->comboBoxName->currentText();
    if(account.isEmpty()) account = "super";

    // 密码
    QString password = ui->lineEditPassword->text();
    if(password.isEmpty()) password = "000";

    // 验证码
    QString securitycode = "";
    if(securitycode.isEmpty()) securitycode = "I'm Code";

    // 机器码
    QString machinecode = "";
    if(machinecode.isEmpty()) machinecode = "I'm Mechine!";



    // 本地连接
    HandleMsg* msg = HandleMsg::GetInstance();
    if (msg->localHost(9560) && msg->ReqLogin(account.toStdString().c_str(),  password.toStdString().c_str(),  securitycode.toStdString().c_str(),  machinecode.toStdString().c_str()))
    {//
        qDebug()<<"登录成功";
    }
    else
    {
        QMessageBox::warning(this, tr("提示"), tr("登录失败"),QMessageBox::NoButton);
    }


}

