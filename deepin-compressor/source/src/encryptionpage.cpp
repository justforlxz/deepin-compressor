#include "encryptionpage.h"
#include <QBoxLayout>
#include <utils.h>
#include <DDesktopServices>
#include <QDebug>

EncryptionPage::EncryptionPage(QWidget *parent)
    : QWidget(parent)
{
    m_inputflag = false;
    InitUI();
    InitConnection();
}

void EncryptionPage::InitUI()
{
    QFont ft;
    ft.setPixelSize(17);
    ft.setBold(true);

    DPalette pa;
    pa.setColor(DPalette::WindowText,QColor(0, 26, 46));

    m_encrypticon = Utils::renderSVG(":/images/lock.svg", QSize(128, 128));
    m_pixmaplabel = new DLabel();
    m_pixmaplabel->setPixmap(m_encrypticon);
    m_stringinfolabel = new DLabel();
    m_stringinfolabel->setFont(ft);
    m_stringinfolabel->setPalette(pa);
    m_stringinfolabel->setText(tr("此文件已加密，请输入解压密码"));
    m_nextbutton = new DPushButton();
    m_nextbutton->setFixedSize(340, 36);
    m_nextbutton->setText(tr("下一步"));
    m_password = new DPasswordEdit();
    m_password->setFixedSize(340, 36);
    QLineEdit* edit = m_password->lineEdit();
    edit->setPlaceholderText(tr("请输入密码以解压"));

    QVBoxLayout* mainlayout = new QVBoxLayout(this);
    mainlayout->addStretch();
    mainlayout->addWidget(m_pixmaplabel, 0 , Qt::AlignHCenter | Qt::AlignVCenter);
    mainlayout->addSpacing(4);
    mainlayout->addWidget(m_stringinfolabel, 0 , Qt::AlignHCenter | Qt::AlignVCenter);
    mainlayout->addSpacing(33);
    mainlayout->addWidget(m_password, 0 , Qt::AlignHCenter | Qt::AlignVCenter);
    mainlayout->addStretch();
    mainlayout->addWidget(m_nextbutton, 0 , Qt::AlignHCenter | Qt::AlignVCenter);
    mainlayout->addSpacing(10);

    pa.setColor(DPalette::Background,QColor(255, 255, 255));
    setPalette(pa);
}

void EncryptionPage::InitConnection()
{
    connect(m_nextbutton, &DPushButton::clicked, this, &EncryptionPage::nextbuttonClicked);
}

void EncryptionPage::nextbuttonClicked()
{
    m_inputflag = true;
    emit sigExtractPassword(m_password->text());
}

void EncryptionPage::wrongPassWordSlot()
{
    if(m_inputflag)
    {
        qDebug()<<"wrongPassWordSlot";
        m_password->setAlert(true);
        m_password->showAlertMessage(tr("密码错误"));
    }

}
