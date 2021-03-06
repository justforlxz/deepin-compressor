/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     dongsen <dongsen@deepin.com>
 *
 * Maintainer: dongsen <dongsen@deepin.com>
 *             AaronZhang <ya.zhang@archermind.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "settingdialog.h"
#include "kprocess.h"
#include "customwidget.h"

#include <DPushButton>
#include <DApplicationHelper>
#include <DStandardPaths>
#include <DSuggestButton>
#include <DFileDialog>
#include <DComboBox>
#include <DLabel>
#include <DPalette>
#include <DSettingsWidgetFactory>

#include <QStandardItemModel>
#include <QDebug>
#include <QBoxLayout>
#include <QMessageBox>
#include <qsettingbackend.h>

DGUI_USE_NAMESPACE

SettingDialog::SettingDialog(QWidget *parent):
    DSettingsDialog(parent)
{
    m_associtionlist << "file_association.file_association_type.x-7z-compressed"
                     << "file_association.file_association_type.x-archive"
                     << "file_association.file_association_type.x-bcpio"
                     << "file_association.file_association_type.x-bzip"
                     << "file_association.file_association_type.x-cpio"
                     << "file_association.file_association_type.x-cpio-compressed"
                     << "file_association.file_association_type.vnd.debian.binary-package"
                     << "file_association.file_association_type.gzip"
                     << "file_association.file_association_type.x-java-archive"
                     << "file_association.file_association_type.x-lzma"
                     << "file_association.file_association_type.vnd.ms-cab-compressed"
                     << "file_association.file_association_type.vnd.rar"
                     << "file_association.file_association_type.x-rpm"
                     << "file_association.file_association_type.x-sv4cpio"
                     << "file_association.file_association_type.x-sv4crc"
                     << "file_association.file_association_type.x-tar"
                     << "file_association.file_association_type.x-bzip-compressed-tar"
                     << "file_association.file_association_type.x-compressed-tar"
                     << "file_association.file_association_type.x-lzip-compressed-tar"
                     << "file_association.file_association_type.x-lzma-compressed-tar"
                     << "file_association.file_association_type.x-tzo"
                     << "file_association.file_association_type.x-xz-compressed-tar"
                     << "file_association.file_association_type.x-tarz"
                     << "file_association.file_association_type.x-xar"
                     << "file_association.file_association_type.x-xz"
                     << "file_association.file_association_type.zip"
                     << "file_association.file_association_type.x-cd-image"
                     << "file_association.file_association_type.x-iso9660-appimage"
                     << "file_association.file_association_type.x-source-rpm";

    m_recommendedList << "file_association.file_association_type.x-7z-compressed"
                      << "file_association.file_association_type.x-archive"
                      << "file_association.file_association_type.x-bcpio"
                      << "file_association.file_association_type.x-bzip"
                      << "file_association.file_association_type.x-cpio"
                      << "file_association.file_association_type.x-cpio-compressed"
                      << "file_association.file_association_type.vnd.debian.binary-package"
                      << "file_association.file_association_type.gzip"
                      << "file_association.file_association_type.x-java-archive"
                      << "file_association.file_association_type.x-lzma"
                      << "file_association.file_association_type.vnd.ms-cab-compressed"
                      << "file_association.file_association_type.vnd.rar"
                      << "file_association.file_association_type.x-rpm"
                      << "file_association.file_association_type.x-sv4cpio"
                      << "file_association.file_association_type.x-sv4crc"
                      << "file_association.file_association_type.x-tar"
                      << "file_association.file_association_type.x-bzip-compressed-tar"
                      << "file_association.file_association_type.x-compressed-tar"
                      << "file_association.file_association_type.x-lzip-compressed-tar"
                      << "file_association.file_association_type.x-lzma-compressed-tar"
                      << "file_association.file_association_type.x-tzo"
                      << "file_association.file_association_type.x-xz-compressed-tar"
                      << "file_association.file_association_type.x-tarz"
                      << "file_association.file_association_type.x-xar"
                      << "file_association.file_association_type.x-xz"
                      << "file_association.file_association_type.zip";

    m_valuelist.clear();
    initUI();
    initConnect();
    readFromConfbf();//fill m_data;

    if (m_data.isEmpty() == true) {
        foreach (QString key, m_associtionlist) {
            m_valuelist.append(m_settings->value(key).toBool());
        }
    } else {
        foreach (QString key, m_associtionlist) {
            m_valuelist.append(m_data[key].toBool());//set value from m_data
        }
    }

    m_valuelisttemp = m_valuelist;

    foreach (QString key, m_associtionlist) {
        m_settings->setOption(key, m_data[key].toBool()); //update dsetting from m_data
    }
}

void SettingDialog::initUI()
{
    this->widgetFactory()->registerWidget("custom-button", [this](QObject * obj) -> QWidget* {
        if (/*DSettingsOption *option = */qobject_cast<DSettingsOption *>(obj))
        {
            QWidget *buttonwidget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout();
            CustomPushButton *button1 = new CustomPushButton(tr("Select All"));
            CustomPushButton *button2 = new CustomPushButton(tr("Clear All"));
            CustomSuggestButton *button3 = new CustomSuggestButton(tr("Recommended"));
            button1->setMinimumSize(153, 36);
            button2->setMinimumSize(153, 36);
            button3->setMinimumSize(153, 36);
            layout->addStretch();
            layout->addWidget(button1);
            layout->addStretch();
            layout->addWidget(button2);
            layout->addStretch();
            layout->addWidget(button3);
            layout->addStretch();
            buttonwidget->setLayout(layout);

            connect(button1, &QPushButton::clicked, this, &SettingDialog::selectpressed);
            connect(button2, &QPushButton::clicked, this, &SettingDialog::cancelpressed);
            connect(button3, &QPushButton::clicked, this, &SettingDialog::recommandedPressed);
            return buttonwidget;
        }

        return nullptr;
    });

    this->widgetFactory()->registerWidget("pathbox", [this](QObject * obj) -> QWidget* {
        m_comboboxoption = qobject_cast<DSettingsOption *>(obj);
        if (m_comboboxoption)
        {
            DWidget *widget = new DWidget(this);
            QHBoxLayout *layout = new QHBoxLayout();

            DLabel *label = new DLabel(widget);
            label->setForegroundRole(DPalette::WindowText);
            label->setText(tr("Extract archives to") + ":");

            CustomCombobox *combobox = new CustomCombobox(widget);
            combobox->setMinimumWidth(300);
            combobox->setEditable(false);
            QStringList list;
            list << tr("Current directory") << tr("Desktop") << tr("Other directory");
            combobox->addItems(list);
            qDebug() << m_comboboxoption->value();
            if (QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) == m_comboboxoption->value()) {
                combobox->setCurrentIndex(1);
                m_curpath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
                m_index_last = 1;
            } else if ("" == m_comboboxoption->value()) {
                combobox->setCurrentIndex(0);
                m_curpath = "";
                m_index_last = 0;
            } else {
                combobox->setEditable(true);
                combobox->setCurrentIndex(2);
                m_curpath = m_comboboxoption->value().toString();
                combobox->setEditText(m_curpath);
                m_index_last = 2;
            }

            layout->addWidget(label, 0, Qt::AlignLeft);
            layout->addWidget(combobox, 0, Qt::AlignLeft);

            widget->setLayout(layout);

            connect(this, &SettingDialog::sigReset, this, [ = ] {
                combobox->setCurrentIndex(0);
            });

            connect(combobox, &DComboBox::currentTextChanged, [combobox, this] {
                if (tr("Current directory") == combobox->currentText())
                {
                    combobox->setEditable(false);
                    m_curpath = "";
                    m_index_last = 0;
                } else if (tr("Desktop") == combobox->currentText())
                {
                    combobox->setEditable(false);
                    m_curpath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
                    m_index_last = 1;
                } else if (tr("Other directory") == combobox->currentText())
                {
                    combobox->setEditable(true);
                    DFileDialog dialog(this);
                    dialog.setAcceptMode(DFileDialog::AcceptOpen);
                    dialog.setFileMode(DFileDialog::Directory);
                    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));

                    const int mode = dialog.exec();

                    if (mode != QDialog::Accepted) {
                        if (0 == m_index_last) {
                            combobox->setEditable(false);
                            combobox->setCurrentIndex(0);
                        } else if (1 == m_index_last) {
                            combobox->setEditable(false);
                            combobox->setCurrentIndex(1);
                        } else {
                            combobox->setEditText(m_curpath);
                        }
                        return;
                    }

                    QList<QUrl> pathlist = dialog.selectedUrls();
                    QString curpath = pathlist.at(0).toLocalFile();

                    combobox->setEditText(curpath);
                    m_curpath = curpath;
                    m_index_last = 2;
                } else
                {
                    m_curpath = combobox->currentText();
                    QDir dir(m_curpath);
                    DPalette plt = DApplicationHelper::instance()->palette(combobox);

                    if (!dir.exists()) {
                        plt.setBrush(DPalette::Text, plt.color(DPalette::TextWarning));
                    } else {
                        plt.setBrush(DPalette::Text, plt.color(DPalette::WindowText));
                    }

                    combobox->setPalette(plt);
                    m_index_last = 2;
                }

                m_comboboxoption->setValue(m_curpath);
            });

            qDebug() << m_curpath;
            return widget;
        }

        return nullptr;
    });

    this->widgetFactory()->registerWidget("deletebox", [this](QObject * obj) -> QWidget* {
        m_deleteArchiveOption = qobject_cast<DSettingsOption *>(obj);
        if (m_deleteArchiveOption)
        {
            DWidget *widget = new DWidget(this);
            QHBoxLayout *layout = new QHBoxLayout();

            DLabel *label = new DLabel(widget);
            label->setForegroundRole(DPalette::WindowText);
            label->setText(tr("Delete archives after extraction") + ":");

            CustomCombobox *combobox = new CustomCombobox(widget);
            combobox->setMinimumWidth(300);
            combobox->setEditable(false);
            QStringList list;
            list << tr("Never") << tr("Ask for confirmation") << tr("Always");
            combobox->addItems(list);

            if ("Always" == m_deleteArchiveOption->value()) {
                combobox->setCurrentIndex(2);
            } else if ("Ask for confirmation" == m_deleteArchiveOption->value()) {
                combobox->setCurrentIndex(1);
            } else {
                combobox->setCurrentIndex(0);
            }

            layout->addWidget(label, 0, Qt::AlignLeft);
            layout->addWidget(combobox, 0, Qt::AlignLeft);

            widget->setLayout(layout);

            connect(this, &SettingDialog::sigReset, this, [ = ] {
                combobox->setCurrentIndex(0);
            });

            connect(combobox, &DComboBox::currentTextChanged, [combobox, this] {
                if (tr("Never") == combobox->currentText())
                {
                    m_isAutoDeleteArchive = "Never";
                } else if (tr("Ask for confirmation") == combobox->currentText())
                {
                    m_isAutoDeleteArchive = "Ask for confirmation";
                } else if (tr("Always") == combobox->currentText())
                {
                    m_isAutoDeleteArchive = "Always";
                } else
                {
                    m_isAutoDeleteArchive = combobox->currentText();
                }

                m_deleteArchiveOption->setValue(m_isAutoDeleteArchive);
            });

            return widget;
        }

        return nullptr;
    });

    const QString confDir = DStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QString confPath = confDir + QDir::separator() + "deepin-compressor.conf";

    // 创建设置项存储后端
    auto backend = new QSettingBackend(confPath, this);

    // 通过json文件创建DSettings对象
    m_settings = DSettings::fromJsonFile(":assets/data/deepin-compressor.json");
    // 设置DSettings存储后端
    m_settings->setBackend(backend);

    // 通过DSettings对象构建设置界面
    updateSettings(m_settings);

    //moveToCenter();
}

void SettingDialog::initConnect()
{
    connect(m_settings, &DSettings::valueChanged, this, &SettingDialog::settingsChanged);
}

void SettingDialog::done(int status)
{
    Q_UNUSED(status)

    QDir dir(m_curpath);

    if (!dir.exists()) {
        QMessageBox box;
        box.setText(tr("The default extraction path does not exist, please retry"));
        box.exec();
        return;
    }

    QDialog::done(status);

    int loop = 0;
    foreach (bool value, m_valuelisttemp) {
        if (m_valuelist.at(loop) != value) {
            QString key = m_associtionlist.at(loop);
            QString mimetype = "application/" + key.remove("file_association.file_association_type.");
            startcmd(mimetype, m_valuelisttemp.at(loop));
        }

        loop++;
    }

    m_valuelist = m_valuelisttemp;
//    writeToConfbf();
}

void SettingDialog::readFromConfbf()
{
    const QString confDir = DStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(confDir + QDir::separator())) {
        dir.mkpath(confDir + QDir::separator());
    }

    const QString confPath = confDir + QDir::separator() + "deepin-compressor.confbf";
    QFile file(confPath);

    if (!file.exists()) {
        file.open(QIODevice::WriteOnly | QIODevice::Text);

        foreach (QString key, m_associtionlist) {
            QString content = key + ":" + "true" + "\n";
            file.write(content.toUtf8());
        }

        file.close();
    }

    bool readStatus = file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (readStatus == true) {
        QByteArray t = file.readAll();
        if (t.length() > 0) {
            m_data.clear();
            QStringList lines = QString(t).split("\n");
            int count = lines.count();
            for (int i = 0; i < count; i++) {
                QStringList column = lines[i].split(":");
                if (column.length() > 1) {
                    m_data.insert(column[0], column[1]);
                }
            }
        }
    } else {
        m_data.clear();
    }

    file.close();
}

void SettingDialog::writeToConfbf()
{
    const QString confDir = DStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(confDir + QDir::separator())) {
        dir.mkpath(confDir + QDir::separator());
    }

    const QString confPath = confDir + QDir::separator() + "deepin-compressor.confbf";

    QFile file(confPath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QMap<QString, QVariant>::iterator it = m_data.begin();
    while (it != m_data.end()) {
        it.key();
        QString content = it.key() + ":" + it.value().toString() + "\n";
        file.write(content.toUtf8());
        it++;
    }

    file.close();
}

QString SettingDialog::getCurExtractPath()
{
    qDebug() << m_curpath;
    return m_curpath;
}

bool SettingDialog::isAutoCreatDir()
{
    return m_settings->value("base.decompress.create_folder").toBool();
}

bool SettingDialog::isAutoOpen()
{
    return m_settings->value("base.decompress.open_folder").toBool();
}

bool SettingDialog::isAutoDeleteFile()
{
    return m_settings->value("base.file_management.delete_file").toBool();
}

QString SettingDialog::isAutoDeleteArchive()
{
    return m_deleteArchiveOption->value().toString();
}

void SettingDialog::settingsChanged(const QString &key, const QVariant &value)
{
//    qDebug() << key << value;
    m_data[key] = value;
    if (key.contains("file_association_type")) {
        int index = m_associtionlist.indexOf(key);
        if (index > -1) {
            m_valuelisttemp.replace(index, value.toBool());
        }
    } else if ((key.contains("default_path") || key.contains("delete_compressed_file")) && value.toString() == "") {
        emit sigReset();
    }

    writeToConfbf();
}

void SettingDialog::selectpressed()
{
    foreach (QString key, m_associtionlist) {
        m_settings->setOption(key, true);
        m_data[key] = QVariant(true);
    }
}

void SettingDialog::cancelpressed()
{
    foreach (QString key, m_associtionlist) {
        m_settings->setOption(key, false);
        m_data[key] = QVariant(false);
    }
}

void SettingDialog::recommandedPressed()
{
    QMap<QString, QVariant>::iterator it = m_data.begin();
    while (it != m_data.end()) {
        it.value() = QVariant(false);
        it++;
    }

    foreach (QString key, m_associtionlist) {
        m_settings->setOption(key, false);
    }

    foreach (QString key, m_recommendedList) {
        m_settings->setOption(key, true);
        m_data[key] = QVariant(true);
    }
}

void SettingDialog::startcmd(QString &mimetype, bool state)
{
    QString programPath = QStandardPaths::findExecutable("xdg-mime");
    if (programPath.isEmpty()) {
        qDebug() << "error can't find xdg-mime";
        return;
    }

    if (nullptr == m_process) {
        m_process = new KProcess(this);
    }

    QStringList arguments;

    if (state) {
        arguments << "default" << "deepin-compressor.desktop" << mimetype;
    } else {
        arguments << "default" << ".desktop" << mimetype;
    }

    qDebug() << mimetype << arguments;

    m_process->setOutputChannelMode(KProcess::MergedChannels);
    m_process->setNextOpenMode(QIODevice::ReadWrite | QIODevice::Unbuffered | QIODevice::Text);
    m_process->setProgram(programPath, arguments);
    m_process->start();
    m_process->waitForStarted();
    m_process->waitForFinished();
}
