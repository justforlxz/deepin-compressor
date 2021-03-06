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
#include "compresssetting.h"
#include "DApplicationHelper"
#include "DFontSizeManager"
#include "utils.h"
#include "customwidget.h"

#include <DDialog>
#include <DFileDialog>
#include <DStyle>
#include <DRadioButton>

#include <QDebug>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QFormLayout>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QScrollArea>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QKeyEvent>
#include <QRegExp>

TypeLabel::TypeLabel(QWidget *parent) : DLabel(parent)
{
}

void TypeLabel::mousePressEvent(QMouseEvent *event)
{
    emit labelClickEvent(event);
    DLabel::mousePressEvent(event);
}

CompressSetting::CompressSetting(QWidget *parent) : DWidget(parent)
{
}

void CompressSetting::InitUI()
{
    // DWidget *leftwidget = new DWidget(this);
    QHBoxLayout *typelayout = new QHBoxLayout;
    QHBoxLayout *layout = new QHBoxLayout;
    m_pixmaplabel = new DLabel(this);

    m_compresstype = new TypeLabel(this);
    m_compresstype->setObjectName("CompressType");
    DPalette pa;
    pa = DApplicationHelper::instance()->palette(m_compresstype);
    pa.setBrush(DPalette::Text, pa.color(DPalette::ToolTipText));
    m_compresstype->setMinimumHeight(25);
    m_compresstype->setFocusPolicy(Qt::TabFocus);
    m_compresstype->installEventFilter(this);

    DStyle style;
    QPixmap pixmap = style.standardIcon(DStyle::StandardPixmap::SP_ReduceElement).pixmap(QSize(10, 10));

    typepixmap = new TypeLabel(this);
    typepixmap->setMinimumHeight(25);
    typepixmap->setPixmap(pixmap);
    m_compresstype->setText("zip");
    m_compresstype->setWordWrap(true);
    m_compresstype->setPalette(pa);
    m_compresstype->setAlignment(Qt::AlignCenter);
    //    QFont font = DFontSizeManager::instance()->get(DFontSizeManager::T5);
    //    font.setWeight(QFont::DemiBold);
    //    m_compresstype->setFont(font);
    DFontSizeManager::instance()->bind(m_compresstype, DFontSizeManager::T5, QFont::DemiBold);
    typelayout->addStretch();
    typelayout->addWidget(m_compresstype, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    typelayout->addWidget(typepixmap, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    typelayout->addStretch();

    m_clicklabel = new TypeLabel(this);
    m_clicklabel->setMinimumSize(125, 40);
    m_clicklabel->setLayout(typelayout);
    layout->addWidget(m_clicklabel, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    m_typemenu = new DMenu(this);
    m_typemenu->setMinimumWidth(162);
    for (const QString &type : qAsConst(m_supportedMimeTypes)) {
        if (QMimeDatabase().mimeTypeForName(type).preferredSuffix() == "zip") {
            m_typemenu->addAction("tar.7z");
            break;
        } else {
            m_typemenu->addAction(QMimeDatabase().mimeTypeForName(type).preferredSuffix());
        }
    }

    m_typemenu->addAction("zip");
    setTypeImage("zip");

    QFormLayout *filelayout = new QFormLayout();
    m_filename = new DLineEdit(this);
    m_filename->setMinimumSize(260, 36);
    m_filename->setText(tr("New Archive"));
    QLineEdit *qfilename = m_filename->lineEdit();
    qfilename->setMaxLength(70);

    m_savepath = new DFileChooserEdit(this);
    m_savepath->setFileMode(DFileDialog::Directory);
    m_savepath->setText(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    m_savepath->setMinimumSize(260, 36);

    filelayout->addRow(tr("Name") + ":", m_filename);
    filelayout->addRow(tr("Save to") + ":", m_savepath);
    filelayout->setLabelAlignment(Qt::AlignLeft);
    filelayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    filelayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    DLabel *moresetlabel = new DLabel(tr("Advanced Options"), this);
    moresetlabel->setForegroundRole(DPalette::WindowText);
    m_moresetbutton = new CustomSwitchButton(this);
    m_moresetlayout = new QHBoxLayout();
    m_moresetlayout->addWidget(moresetlabel, 0, Qt::AlignLeft);
    m_moresetlayout->addWidget(m_moresetbutton, 1, Qt::AlignRight);
    m_encryptedlabel = new DLabel(tr("Encrypt the archive") + ":", this);
    m_encryptedlabel->setToolTip(tr("Support zip, 7z type only"));

    m_encryptedlabel->setForegroundRole(DPalette::WindowText);
    m_password = new DPasswordEdit(this);
    QLineEdit *edit = m_password->lineEdit();
    edit->setPlaceholderText(tr("Password"));
    m_password->setMinimumSize(260, 36);
    m_encryptedfilelistlabel = new DLabel(tr("Encrypt the file list too"), this);
    m_encryptedfilelistlabel->setToolTip(tr("Support 7z type only"));
    m_encryptedfilelistlabel->setEnabled(false);
    m_file_secret = new CustomSwitchButton(this);
    m_file_secretlayout = new QHBoxLayout();
    m_file_secretlayout->addWidget(m_encryptedfilelistlabel, 0, Qt::AlignLeft);
    m_file_secretlayout->addWidget(m_file_secret, 1, Qt::AlignRight);
    m_splitcompress = new CustomCheckBox(tr("Split to volumes") + ":", this);
    m_splitcompress->setEnabled(false);
    m_splitcompress->setToolTip(tr("Support 7z type only"));
    m_splitnumedit = new DDoubleSpinBox(this);
    m_splitnumedit->setMinimumSize(260, 36);
    m_splitnumedit->setSuffix("MB");
    m_splitnumedit->setRange(0.0, 1000000);
    m_splitnumedit->setSingleStep(0.1);
    m_splitnumedit->setDecimals(1);
    m_splitnumedit->setValue(0.0);
    m_splitnumedit->setSpecialValueText(" ");

    QVBoxLayout *typeLayout = new QVBoxLayout;
    typeLayout->addSpacing(65);
    typeLayout->addWidget(m_pixmaplabel, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    typeLayout->addLayout(layout);
    typeLayout->addStretch();
    // leftwidget->setLayout(typeLayout);

    m_fileLayout = new QVBoxLayout;
    m_fileLayout->addLayout(filelayout);
    m_fileLayout->addLayout(m_moresetlayout);
    m_fileLayout->addWidget(m_encryptedlabel);
    m_fileLayout->addWidget(m_password);
    m_fileLayout->addLayout(m_file_secretlayout);
    m_fileLayout->addWidget(m_splitcompress);
    m_fileLayout->addWidget(m_splitnumedit);
    m_fileLayout->addStretch();

    DWidget *m_rightwidget = new DWidget(this);
    m_rightwidget->setLayout(m_fileLayout);

    QScrollArea *m_scroll = new QScrollArea(this);
    m_scroll->setWidget(m_rightwidget);
    // m_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scroll->setFrameShape(QFrame::NoFrame);
    m_scroll->setWidgetResizable(true);
    m_scroll->setMinimumHeight(345);

    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addStretch();
    infoLayout->addLayout(typeLayout, 8);
    infoLayout->addStretch();
    infoLayout->addWidget(m_scroll, 10);
    infoLayout->addStretch();
    infoLayout->setContentsMargins(0, 0, 50, 0);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(infoLayout, 10);
    mainLayout->addStretch();

    m_nextbutton = new CustomPushButton(tr("Compress"), this);
    m_nextbutton->setMinimumSize(340, 36);
    QHBoxLayout *buttonHBoxLayout = new QHBoxLayout;
    buttonHBoxLayout->addStretch(1);
    buttonHBoxLayout->addWidget(m_nextbutton, 2);
    buttonHBoxLayout->addStretch(1);

    mainLayout->addLayout(buttonHBoxLayout);

    mainLayout->setContentsMargins(12, 6, 20, 20);

    m_splitnumedit->setEnabled(false);
    m_password->setEnabled(true);
    m_file_secret->setEnabled(false);

    m_password->lineEdit()->setAttribute(Qt::WA_InputMethodEnabled, false);

    setBackgroundRole(DPalette::Base);

    setTabOrder(m_compresstype, m_filename);
}

void CompressSetting::InitConnection()
{
    connect(m_nextbutton, &DPushButton::clicked, this, &CompressSetting::onNextButoonClicked);
    connect(m_moresetbutton, &DSwitchButton::toggled, this, &CompressSetting::onAdvanceButtonClicked);
    connect(m_splitcompress, &DCheckBox::stateChanged, this, &CompressSetting::onSplitChanged);
    connect(m_password, &DPasswordEdit::echoModeChanged, this, &CompressSetting::slotEchoModeChanged);

    connect(DApplicationHelper::instance(), &DApplicationHelper::themeTypeChanged, this, &CompressSetting::onThemeChanged);
    connect(m_savepath, &DFileChooserEdit::textChanged, this, &CompressSetting::onThemeChanged);

    connect(m_filename, &DLineEdit::textChanged, this, [ = ] {
        DPalette plt;
        plt = DApplicationHelper::instance()->palette(m_filename);

        if (!m_filename->text().isEmpty())
        {
            if (false == checkfilename(m_filename->text())) {
                plt.setBrush(DPalette::Text, plt.color(DPalette::TextWarning));
            } else {
                plt.setBrush(DPalette::Text, plt.color(DPalette::WindowText));
            }
        } else
        {
            plt.setBrush(DPalette::Text, plt.color(DPalette::WindowText));
        }

        m_filename->setPalette(plt);

    });

    connect(typepixmap, SIGNAL(labelClickEvent(QMouseEvent *)), this, SLOT(showRightMenu(QMouseEvent *)));
    connect(m_compresstype, SIGNAL(labelClickEvent(QMouseEvent *)), this, SLOT(showRightMenu(QMouseEvent *)));
    connect(m_clicklabel, SIGNAL(labelClickEvent(QMouseEvent *)), this, SLOT(showRightMenu(QMouseEvent *)));
    connect(m_typemenu, &DMenu::triggered, this, &CompressSetting::ontypeChanged);
}

void CompressSetting::initWidget()
{
    if (m_nextbutton == nullptr) {
        m_supportedMimeTypes = m_pluginManger.supportedWriteMimeTypes(PluginManager::SortByComment);

        InitUI();
        InitConnection();
    }
}

void CompressSetting::showRightMenu(QMouseEvent * /*e*/)
{
    QPoint pos;
    pos.setX(window()->x() + m_pixmaplabel->x() + 60);
    pos.setY(window()->y() + m_pixmaplabel->y() + 240);
    m_typemenu->popup(pos);
}

void CompressSetting::onNextButoonClicked()
{
    QDir dir(m_savepath->text());
    QString name = m_filename->text().remove(" ");
    if (!checkfilename(name)) {
        showWarningDialog(tr("Invalid file name"));
        return;
    }

    if ((m_savepath->text().remove(" ")).isEmpty()) {
        showWarningDialog(tr("Please enter the path"));
        return;
    }

    if (false == dir.exists()) {
        showWarningDialog(tr("The path does not exist, please retry"));
        return;
    }

    for (int i = 0; i < m_pathlist.count(); i++) {
        QFileInfo m_fileName(m_pathlist.at(i));
        if (!m_fileName.exists()) {
            filePermission = false;
            showWarningDialog(tr("%1 was changed on the disk, please import it again.").arg(Utils::toShortString(m_fileName.fileName())));
            return;
        } /*else if (m_fileName.isDir() && m_fileName.exists()) {
            QString dirFilePath = checkDirFileExit(m_fileName.path());
            qDebug() << "dirfilepath is : " << dirFilePath;
            if (!fileReadable) {
                QFileInfo file(dirFilePath);
                showWarningDialog(tr("%1 was changed on the disk, please import it again.").arg(file.fileName()));
                return;
            }
            QDir dir(m_fileName.path());
            foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::Files)) {
                qDebug() << "file is :" << fileInfo.fileName();
            }
        }*/
        if (m_fileName.isFile()) {
            if (!m_fileName.isReadable()) {
                filePermission = false;
                showWarningDialog(tr("You do not have permission to compress %1").arg(Utils::toShortString(m_fileName.fileName())), i);
                return;
            }
        } else if (m_fileName.isDir()) {
            if (!m_fileName.isReadable()) {
                filePermission = false;
                showWarningDialog(tr("You do not have permission to compress %1").arg(Utils::toShortString(m_fileName.fileName())), i);
                return;
            } else {
                filePermission = checkFilePermission(m_fileName.absoluteFilePath());
                if (!filePermission) {
                    showWarningDialog(tr("You do not have permission to compress %1").arg(Utils::toShortString(m_fileName.fileName())), i);
                    return;
                }
            }
        }
    }

    QFileInfo m_fileDestinationPath(m_savepath->text());
    if (!(m_fileDestinationPath.isWritable() && m_fileDestinationPath.isExecutable())) {
        showWarningDialog(tr("You do not have permission to save files here, please change and retry"));
        return;
    }

    if ((((m_getFileSize / 1024 / 1024) / (m_splitnumedit->value())) > 10) && m_compresstype->text().contains("7z") && m_splitcompress->isChecked()) {
        showWarningDialog(tr("Too many volumes, please change and retry"));
        //  Up to 10 volumes, please change and retry
        return;
    }

    QMap< QString, QString > m_openArgs;
    const QString password = m_password->text();
    QString fixedMimeType;
    QString tmpCompresstype = m_compresstype->text();
    QString strTar7z;
    if (0 == QString("tar.7z").compare(tmpCompresstype, Qt::CaseInsensitive)) {
        tmpCompresstype = "7z";
        strTar7z = ".tar";
        m_openArgs[QStringLiteral("createtar7z")] = QString("true");
        m_openArgs[QStringLiteral("selectFilesSize")] = QString::number(m_getFileSize);
    }

//    QString tmpCompresstype = (0 == QString("tar.7z").compare(m_compresstype->text(), Qt::CaseInsensitive)) ? QString("7z") : m_compresstype->text();

    for (const QString &type : qAsConst(m_supportedMimeTypes)) {
        if (0 == QMimeDatabase().mimeTypeForName(type).preferredSuffix().compare(tmpCompresstype, Qt::CaseInsensitive)) {
            fixedMimeType = type;
            break;
        }
    }

    m_openArgs[QStringLiteral("createNewArchive")] = QStringLiteral("true");
    m_openArgs[QStringLiteral("fixedMimeType")] = fixedMimeType;
    if ("application/x-tar" == fixedMimeType || "application/x-tarz" == fixedMimeType) {
        m_openArgs[QStringLiteral("compressionLevel")] = "-1";  //-1 is unuseful
    } else if ("application/zip" == fixedMimeType) {
        m_openArgs[QStringLiteral("compressionLevel")] = "3";  // 1:Extreme 3:Fast 5:Standard

        //        if (password.contains(QRegExp("[\\x4e00-\\x9fa5]+"))) {
        //            showWarningDialog(tr("The zip format does not support Chinese characters as compressed passwords"));
        //            return;
        //        }
    } else {
        m_openArgs[QStringLiteral("compressionLevel")] = "6";  // 6 is default
    }

    qDebug() << m_splitnumedit->value();
    if (m_splitnumedit->value() > 0 && m_splitcompress->isChecked()) {
        m_openArgs[QStringLiteral("volumeSize")] = QString::number(static_cast< int >(m_splitnumedit->value() * 1024));
    }

    //    if (!dialog.data()->compressionMethod().isEmpty()) {
    //        m_openArgs.metaData()[QStringLiteral("compressionMethod")] = dialog.data()->compressionMethod();
    //    }

    qDebug() << m_openArgs[QStringLiteral("volumeSize")];
    if (!m_password->text().isEmpty()) {
        m_openArgs[QStringLiteral("encryptionMethod")] = "AES256";  // 5 is default
    }

    m_openArgs[QStringLiteral("encryptionPassword")] = password;

    if (m_file_secret->isChecked()) {
        m_openArgs[QStringLiteral("encryptHeader")] = QStringLiteral("true");
    }

    m_openArgs[QStringLiteral("localFilePath")] = m_savepath->text();
    m_openArgs[QStringLiteral("filename")] =
        m_filename->text() + strTar7z + "."  + QMimeDatabase().mimeTypeForName(fixedMimeType).preferredSuffix();

    //check exist file
    QFileInfo eFile(m_savepath->text() + QDir::separator() + m_filename->text() + strTar7z + "." + QMimeDatabase().mimeTypeForName(fixedMimeType).preferredSuffix());
    if (eFile.exists()) {
        if (existSameFileName() == true) {
            QFile file(eFile.filePath());
            file.remove();
        } else {
            return;
        }
    }

    //check if folderName valid
    QString unvalidStr = "";
    foreach (QString file, m_pathlist) {
        QString globalWorkDir = file;
        if (globalWorkDir.right(1) == QLatin1String("/")) {
            globalWorkDir.chop(1);
        }

        QFileInfo fileInfo(globalWorkDir);
        globalWorkDir = fileInfo.dir().absolutePath();
        if (fileInfo.baseName().left(1) == "@") {
            unvalidStr = fileInfo.baseName();
        }
    }

    const QString fixedType = m_openArgs[QStringLiteral("fixedMimeType")];
    if (unvalidStr != "") {
        QSet<QString> special = {"application/x-7z-compressed", "application/zip", "application/x-java-archive", "application/x-tar"};
        if (special.contains(fixedType) == true) {
//            int limitCounts = 16;
//            int left = 8, right = 8;
//            QString displayName = "";
//            displayName = unvalidStr.length() > limitCounts ? unvalidStr.left(left) + "..." + unvalidStr.right(right) : unvalidStr;
//            QString strTips = tr("%1 :unvalid name,can't start with '@'.").arg(displayName);
            QString strTips = tr("Files that begin with '@' cannot be compressed");
            showWarningDialog(strTips);
            special.clear();
            return;
        }
    }
    //check end

    emit sigCompressPressed(m_openArgs);

    m_openArgs.remove(QStringLiteral("createNewArchive"));
    m_openArgs.remove(QStringLiteral("fixedMimeType"));
    m_openArgs.remove(QStringLiteral("compressionLevel"));
    m_openArgs.remove(QStringLiteral("encryptionPassword"));
    m_openArgs.remove(QStringLiteral("encryptHeader"));
    m_openArgs.remove(QStringLiteral("localFilePath"));
    m_openArgs.remove(QStringLiteral("filename"));
    m_openArgs.remove(QStringLiteral("createtar7z"));
    m_openArgs.remove(QStringLiteral("selectFilesSize"));
}

void CompressSetting::onAdvanceButtonClicked(bool status)
{
    if (status) {
        m_encryptedlabel->setVisible(true);
        m_password->setVisible(true);
        m_encryptedfilelistlabel->setVisible(true);
        m_file_secret->setVisible(true);
        m_splitcompress->setVisible(true);
        m_splitnumedit->setVisible(true);
    } else {
        m_encryptedlabel->setVisible(false);
        m_password->setVisible(false);
        m_encryptedfilelistlabel->setVisible(false);
        m_file_secret->setVisible(false);
        m_splitcompress->setVisible(false);
        m_splitnumedit->setVisible(false);
        m_password->setText("");
        m_file_secret->setChecked(false);
        m_splitcompress->setChecked(false);
        m_splitnumedit->setValue(0);
        isSplitChecked = false;
    }
}

void CompressSetting::setTypeImage(QString type)
{
    QFileIconProvider provider;
    QIcon icon = provider.icon(QFileInfo("temp." + type));

    m_pixmaplabel->setPixmap(icon.pixmap(128, 128));
}

void CompressSetting::setDefaultPath(QString path)
{
    initWidget();

    m_savepath->setText(path);
    QUrl dir(path);
    m_savepath->setDirectoryUrl(dir);
}

void CompressSetting::setDefaultName(QString name)
{
    initWidget();

    onAdvanceButtonClicked(m_moresetbutton->isChecked());
//    name = name + "." + m_compresstype->text();
    m_filename->setText(name);
    QLineEdit *qfilename = m_filename->lineEdit();
    qfilename->selectAll();
    qfilename->setFocus();
}

quint64 CompressSetting::dirFileSize(const QString &path)
{
    QDir dir(path);
    quint64 size = 0;
    // dir.entryInfoList(QDir::Files)返回文件信息
    foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::Files)) {
        //计算文件大小
        size += fileInfo.size();
    }

    // dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot)返回所有子目录，并进行过滤
    foreach (QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        //若存在子目录，则递归调用dirFileSize()函数
        size += dirFileSize(path + QDir::separator() + subDir);
    }

    return size;
}

bool CompressSetting::checkfilename(QString str)
{
    if (str.length() == 0) {
        return false;
    }

    if (str.length() > 255) {
        return false;
    }

    QList< QChar > black_list;
    black_list << '/';
//    black_list << '-';
//    black_list << '.';
//    black_list << ',';
    if (black_list.contains(str.at(0))) {
        return false;
    }

//----------------:;39;“\][=-~!*()；//command : #
    black_list.clear();
    black_list = {'/'};
//    black_list = { '/', '\t', '\b', '@', '#', '$', '%', '^', '&' }; /*, '*', '(', ')', '[', ']'*/
    foreach (QChar black_char, black_list) {
        if (str.contains(black_char)) {
            return false;
        }
    }

    return true;
}

bool CompressSetting::checkFilePermission(const QString &path)
{
    bool filePermissionFlag = true;
    QDir dir(path);
    QFileInfo fileInfo;

    foreach (fileInfo, dir.entryInfoList()) {
        if (!fileInfo.isReadable()) {
            filePermissionFlag = false;
            return filePermissionFlag;
        }
    }

    foreach (QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        filePermissionFlag = checkFilePermission(path + QDir::separator() + subDir);
    }

    return filePermissionFlag;
}

void CompressSetting::setSelectedFileSize(qint64 size)
{
    m_getFileSize = size;
}

void CompressSetting::clickTitleBtnResetAdvancedOptions()
{
    m_moresetbutton->setChecked(false);
}

QList<QAction *> CompressSetting::getTypemenuActions()
{
    return m_typemenu->actions();
}

DLineEdit *CompressSetting::getFilenameLineEdit()
{
    return m_filename;
}

CustomPushButton *CompressSetting::getNextbutton()
{
    return m_nextbutton;
}

TypeLabel *CompressSetting::getCompresstype()
{
    return m_compresstype;
}

void CompressSetting::showEvent(QShowEvent *event)
{
    initWidget();

    DWidget::showEvent(event);
}

void CompressSetting::keyPressEvent(QKeyEvent *event)
{
    if (nullptr == event) {
        return;
    }

    if (event->key() == Qt::Key_F2) {
        QLineEdit *qfilename = m_filename->lineEdit();
        QLineEdit *qpath = m_savepath->lineEdit();
        QLineEdit *qpassword = m_password->lineEdit();
        if (qfilename->hasFocus()) {
            qfilename->selectAll();
        } else if (qpath->hasFocus()) {
            qpath->selectAll();
        } else if (qpassword->hasFocus()) {
            qpassword->selectAll();
        } else if (m_splitnumedit->hasFocus()) {
            m_splitnumedit->selectAll();
        }
    }
}

void CompressSetting::setFilepath(QStringList pathlist)
{
    m_pathlist = pathlist;
}

void CompressSetting::onSplitChanged(int /*status*/)
{
    if (m_splitcompress->isChecked() && "7z" == m_compresstype->text()) {
        m_splitnumedit->setEnabled(true);
//        if ((m_getFileSize / 1024 / 1024) >= 1) { //1M以上的文件
//            m_splitnumedit->setValue(m_getFileSize / 1024.0 / 1024.0 / 2.0 + 0.1);
//        } else if ((m_getFileSize / 1024) > 102 && (m_getFileSize / 1024) <= 1024) { //0.1M－1M的文件
//            m_splitnumedit->setValue(m_getFileSize / 1024.0 / 1024.0 / 2.0);
//        }
        QString size = Utils::humanReadableSize(m_getFileSize, 1);
        m_splitnumedit->setToolTip(tr("Total size: %1").arg(size));
        isSplitChecked = true;
    } else {
        m_splitnumedit->setEnabled(false);
        m_splitnumedit->clear();
    }
}

void CompressSetting::ontypeChanged(QAction *action)
{
//    qDebug() << action->text();
    QString selectType = action->text();
    setTypeImage(selectType);
    m_compresstype->setText(selectType);

    if (0 == selectType.compare("tar.7z")) {
        m_encryptedlabel->setEnabled(true);
        m_password->setEnabled(true);
        m_encryptedfilelistlabel->setEnabled(true);
        m_file_secret->setEnabled(true);
        m_splitcompress->setChecked(false);
        m_splitcompress->setEnabled(false); //tar.7z暂不支持分卷压缩
//        m_splitnumedit->setRange(0.0, 1000000);
        m_splitnumedit->setEnabled(false);
        m_splitnumedit->setValue(0.0);
        isSplitChecked = false;
    } else if (0 == selectType.compare("7z")) {
        if (m_splitcompress->isChecked()) {
            m_splitnumedit->setEnabled(true);
        }

        m_encryptedlabel->setEnabled(true);
        m_password->setEnabled(true);
        m_encryptedfilelistlabel->setEnabled(true);
        m_file_secret->setEnabled(true);
        m_splitcompress->setEnabled(true);
    } else if (0 == selectType.compare("zip")) {
        m_splitnumedit->setEnabled(false);
        m_encryptedlabel->setEnabled(true);
        m_password->setEnabled(true);
        m_encryptedfilelistlabel->setEnabled(false);
        m_file_secret->setEnabled(false);
        m_file_secret->setChecked(false);
        m_splitcompress->setEnabled(false);
        m_splitcompress->setChecked(false);
        //m_splitnumedit->setRange(0.0, 1000000);
        m_splitnumedit->setValue(0.0);
        isSplitChecked = false;
    } else {
        m_splitnumedit->setEnabled(false);
        m_encryptedlabel->setEnabled(false);
        m_password->setEnabled(false);
        m_encryptedfilelistlabel->setEnabled(false);
        m_file_secret->setEnabled(false);
        m_file_secret->setChecked(false);
        m_splitcompress->setEnabled(false);
        m_splitcompress->setChecked(false);
        //m_splitnumedit->setRange(0.0, 1000000);
        m_splitnumedit->setValue(0.0);
        isSplitChecked = false;
    }
}

void CompressSetting::onThemeChanged()
{
    DPalette plt;
    plt = DApplicationHelper::instance()->palette(m_filename);

    if (!m_filename->text().isEmpty()) {
        if (false == checkfilename(m_filename->text())) {
            plt.setBrush(DPalette::Text, plt.color(DPalette::TextWarning));
        } else {
            plt.setBrush(DPalette::Text, plt.color(DPalette::WindowText));
        }
    } else {
        plt.setBrush(DPalette::Text, plt.color(DPalette::WindowText));
    }

    m_filename->setPalette(plt);
}

bool CompressSetting::onSplitChecked()
{
    return isSplitChecked;
}

void CompressSetting::slotEchoModeChanged(bool echoOn)
{
    qDebug() << echoOn;
    m_password->lineEdit()->setAttribute(Qt::WA_InputMethodEnabled, echoOn);
}

void CompressSetting::autoCompressEntry(const QString &compresspath, const QStringList &path, Archive::Entry *pWorkEntry)
{
    qDebug() << "开始执行添加操作！" << ";compresspath:" << compresspath << ";path:" << path;
    for (int i = 0; i < path.count(); i++) {
        if (compresspath == path.at(i)) {
            DDialog *pDialog = new DDialog(this);
            pDialog->getButton(pDialog->addButton(tr("Close")))->setShortcut(Qt::Key_C);
            showWarningDialog(tr("You cannot add the archive to itself"), 0, "", pDialog);
            return;
        }
    }

    QStringList cFileInfoList = path ;
    QFileInfo cFileInfo(compresspath);
    QDir dir(cFileInfo.path());//compress path

    QString name = cFileInfo.fileName();
    if (!checkfilename(name)) {
        showWarningDialog(tr("Invalid file name"));
        return;
    }

    if (name.isEmpty()) {
        showWarningDialog(tr("Please enter the path"));
        return;
    }

    if (false == dir.exists()) {
        showWarningDialog(tr("The path does not exist, please retry"));
        return;
    }

    for (int i = 0; i < cFileInfoList.count(); i++) {
        QFileInfo m_fileName(cFileInfoList.at(i));
        if (!m_fileName.exists()) {
            filePermission = false;
            showWarningDialog(tr("%1 was changed on the disk, please import it again.").arg(m_fileName.fileName()));
            return;
        }

        if (m_fileName.isFile()) {
            if (!m_fileName.isReadable()) {
                filePermission = false;
                showWarningDialog(tr("You do not have permission to compress %1").arg(m_fileName.fileName()), i);
                return;
            }
        } else if (m_fileName.isDir()) {
            if (!m_fileName.isReadable()) {
                filePermission = false;
                showWarningDialog(tr("You do not have permission to compress %1").arg(m_fileName.fileName()), i);
                return;
            } else {
                filePermission = checkFilePermission(m_fileName.absoluteFilePath());
                if (!filePermission) {
                    showWarningDialog(tr("You do not have permission to compress %1").arg(m_fileName.fileName()), i);
                    return;
                }
            }
        }
    }

    QFileInfo m_fileDestinationPath(cFileInfo.path());
    if (!(m_fileDestinationPath.isWritable() && m_fileDestinationPath.isExecutable())) {
        showWarningDialog(tr("You do not have permission to save files here, please change and retry"));
        return;
    }

    qDebug() << "开始设置参数！";
    QMap< QString, QString > m_openArgs;
    const QString password = "";
    QString fixedMimeType;
    if (!m_supportedMimeTypes.size()) {
        m_supportedMimeTypes = m_pluginManger.supportedWriteMimeTypes(PluginManager::SortByComment);
    }

    for (const QString &type : qAsConst(m_supportedMimeTypes)) {
        if (0 == QMimeDatabase().mimeTypeForName(type).preferredSuffix().compare(cFileInfo.completeSuffix(), Qt::CaseInsensitive)) {
            fixedMimeType = type;
            break;
        }
    }

    m_openArgs[QStringLiteral("createNewArchive")] = QStringLiteral("false");
    m_openArgs[QStringLiteral("fixedMimeType")] = fixedMimeType;
    if ("application/x-tar" == fixedMimeType || "application/x-tarz" == fixedMimeType) {
        m_openArgs[QStringLiteral("compressionLevel")] = "-1";  //-1 is unuseful
    } else if ("application/zip" == fixedMimeType) {
        m_openArgs[QStringLiteral("compressionLevel")] = "3";  // 1:Extreme 3:Fast 4:Standard
    } else {
        m_openArgs[QStringLiteral("compressionLevel")] = "6";  // 6 is default
    }

    if (m_splitnumedit && m_splitnumedit->value() > 0) {
        m_openArgs[QStringLiteral("volumeSize")] = QString::number(static_cast< int >(m_splitnumedit->value() * 1024));
    }

    //    if (!dialog.data()->compressionMethod().isEmpty()) {
    //        m_openArgs.metaData()[QStringLiteral("compressionMethod")] = dialog.data()->compressionMethod();
    //    }

    if (m_password && !m_password->text().isEmpty()) {
        m_openArgs[QStringLiteral("encryptionMethod")] = "AES256";  // 5 is default
    }

    m_openArgs[QStringLiteral("encryptionPassword")] = password;

    m_openArgs[QStringLiteral("encryptHeader")] = QStringLiteral("false");

    m_openArgs[QStringLiteral("localFilePath")] = cFileInfo.path();
    m_openArgs[QStringLiteral("filename")] = cFileInfo.fileName();
//        cFileInfo.baseName() + "." + QMimeDatabase().mimeTypeForName(fixedMimeType).preferredSuffix();

    m_openArgs[QStringLiteral("sourceFilePath")] = compresspath;
    if (path.size()) {
        QString paths = path[0];
        for (int i = 1 ; i < path.size() ; i++) {
            paths += "--" + path[i];
        }

        m_openArgs[QStringLiteral("ToCompressFilePath")] = paths;
    }

    emit sigUncompressStateAutoCompressEntry(m_openArgs, pWorkEntry);

    m_openArgs.remove(QStringLiteral("createNewArchive"));
    m_openArgs.remove(QStringLiteral("fixedMimeType"));
    m_openArgs.remove(QStringLiteral("compressionLevel"));
    m_openArgs.remove(QStringLiteral("encryptionPassword"));
    m_openArgs.remove(QStringLiteral("encryptHeader"));
    m_openArgs.remove(QStringLiteral("localFilePath"));
    m_openArgs.remove(QStringLiteral("filename"));
    m_openArgs.remove(QStringLiteral("sourceFilePath"));
    m_openArgs.remove(QStringLiteral("ToCompressFilePath"));
}

void CompressSetting::autoCompress(const QString &compresspath, const QStringList &path)
{
    qDebug() << "开始执行添加操作！" << ";compresspath:" << compresspath << ";path:" << path;

    for (int i = 0; i < path.count(); i++) {
        if (compresspath == path.at(i)) {
            DDialog *pDialog = new DDialog(this);
            pDialog->addButton(tr("OK"));
            showWarningDialog(tr("You cannot add the archive to itself"), 0, "", pDialog);
            return;
        }
    }

    QStringList cFileInfoList = path ;
    QFileInfo cFileInfo(compresspath);
    QDir dir(cFileInfo.path());//compress path

    QString name = cFileInfo.fileName();
    if (!checkfilename(name)) {
        showWarningDialog(tr("Invalid file name"));
        return;
    }

    if (name.isEmpty()) {
        showWarningDialog(tr("Please enter the path"));
        return;
    }

    if (false == dir.exists()) {
        showWarningDialog(tr("The path does not exist, please retry"));
        return;
    }

    for (int i = 0; i < cFileInfoList.count(); i++) {
        QFileInfo m_fileName(cFileInfoList.at(i));
        if (!m_fileName.exists()) {
            filePermission = false;
            showWarningDialog(tr("%1 was changed on the disk, please import it again.").arg(m_fileName.fileName()));
            return;
        }

        if (m_fileName.isFile()) {
            if (!m_fileName.isReadable()) {
                filePermission = false;
                showWarningDialog(tr("You do not have permission to compress %1").arg(m_fileName.fileName()), i);
                return;
            }
        } else if (m_fileName.isDir()) {
            if (!m_fileName.isReadable()) {
                filePermission = false;
                showWarningDialog(tr("You do not have permission to compress %1").arg(m_fileName.fileName()), i);
                return;
            } else {
                filePermission = checkFilePermission(m_fileName.absoluteFilePath());
                if (!filePermission) {
                    showWarningDialog(tr("You do not have permission to compress %1").arg(m_fileName.fileName()), i);
                    return;
                }
            }
        }
    }

    QFileInfo m_fileDestinationPath(cFileInfo.path());
    if (!(m_fileDestinationPath.isWritable() && m_fileDestinationPath.isExecutable())) {
        showWarningDialog(tr("You do not have permission to save files here, please change and retry"));
        return;
    }

    qDebug() << "开始设置参数！";
    QMap< QString, QString > m_openArgs;
    const QString password = "";
    QString fixedMimeType;
    if (!m_supportedMimeTypes.size()) {
        m_supportedMimeTypes = m_pluginManger.supportedWriteMimeTypes(PluginManager::SortByComment);
    }

    for (const QString &type : qAsConst(m_supportedMimeTypes)) {
        if (0 == QMimeDatabase().mimeTypeForName(type).preferredSuffix().compare(cFileInfo.completeSuffix(), Qt::CaseInsensitive)) {
            fixedMimeType = type;
            break;
        }
    }

    m_openArgs[QStringLiteral("createNewArchive")] = QStringLiteral("false");
    m_openArgs[QStringLiteral("fixedMimeType")] = fixedMimeType;
    if ("application/x-tar" == fixedMimeType || "application/x-tarz" == fixedMimeType) {
        m_openArgs[QStringLiteral("compressionLevel")] = "-1";  //-1 is unuseful
    } else if ("application/zip" == fixedMimeType) {
        m_openArgs[QStringLiteral("compressionLevel")] = "3";  // 1:Extreme 3:Fast 4:Standard
    } else {
        m_openArgs[QStringLiteral("compressionLevel")] = "6";  // 6 is default
    }

    if (m_splitnumedit && m_splitnumedit->value() > 0) {
        m_openArgs[QStringLiteral("volumeSize")] = QString::number(static_cast< int >(m_splitnumedit->value() * 1024));
    }

    //    if (!dialog.data()->compressionMethod().isEmpty()) {
    //        m_openArgs.metaData()[QStringLiteral("compressionMethod")] = dialog.data()->compressionMethod();
    //    }

    if (m_password && !m_password->text().isEmpty()) {
        m_openArgs[QStringLiteral("encryptionMethod")] = "AES256";  // 5 is default
    }

    m_openArgs[QStringLiteral("encryptionPassword")] = password;

    m_openArgs[QStringLiteral("encryptHeader")] = QStringLiteral("false");

    m_openArgs[QStringLiteral("localFilePath")] = cFileInfo.path();
    m_openArgs[QStringLiteral("filename")] = cFileInfo.fileName();
//        cFileInfo.baseName() + "." + QMimeDatabase().mimeTypeForName(fixedMimeType).preferredSuffix();

    m_openArgs[QStringLiteral("sourceFilePath")] = compresspath;
    if (path.size()) {
        QString paths = path[0];
        for (int i = 1 ; i < path.size() ; i++) {
            paths += "--" + path[i];
        }

        m_openArgs[QStringLiteral("ToCompressFilePath")] = paths;
    }

    emit sigUncompressStateAutoCompress(m_openArgs);

    m_openArgs.remove(QStringLiteral("createNewArchive"));
    m_openArgs.remove(QStringLiteral("fixedMimeType"));
    m_openArgs.remove(QStringLiteral("compressionLevel"));
    m_openArgs.remove(QStringLiteral("encryptionPassword"));
    m_openArgs.remove(QStringLiteral("encryptHeader"));
    m_openArgs.remove(QStringLiteral("localFilePath"));
    m_openArgs.remove(QStringLiteral("filename"));
    m_openArgs.remove(QStringLiteral("sourceFilePath"));
    m_openArgs.remove(QStringLiteral("ToCompressFilePath"));
}

void CompressSetting::autoMoveToArchive(const QStringList &files, const QString &archive)
{
    qDebug() << "开始执行移动操作！" << ";movepath:" << archive << ";path:" << files[0];
    for (int i = 0; i < files.count(); i++) {
        if (archive == files.at(i)) {
            showWarningDialog(tr("You cannot add the archive to itself"));
            return;
        }
    }

    QStringList cFileInfoList = files ;
    QFileInfo cFileInfo(archive);
    QDir dir(cFileInfo.path());//relative compress path

    QString name = cFileInfo.fileName();
    if (!checkfilename(name)) {
        showWarningDialog(tr("Invalid file name"));
        return;
    }

    if (name.isEmpty()) {
        showWarningDialog(tr("Please enter the path"));
        return;
    }

    if (false == dir.exists()) {
        showWarningDialog(tr("The path does not exist, please retry"));
        return;
    }

    for (int i = 0; i < cFileInfoList.count(); i++) {
        QFileInfo m_fileName(cFileInfoList.at(i));
        if (!m_fileName.exists()) {
            filePermission = false;
            showWarningDialog(tr("%1 was changed on the disk, please import it again.").arg(m_fileName.fileName()));
            return;
        }

        if (m_fileName.isFile()) {
            if (!m_fileName.isReadable()) {
                filePermission = false;
                showWarningDialog(tr("You do not have permission to compress %1").arg(m_fileName.fileName()), i);
                return;
            }
        } else if (m_fileName.isDir()) {
            if (!m_fileName.isReadable()) {
                filePermission = false;
                showWarningDialog(tr("You do not have permission to compress %1").arg(m_fileName.fileName()), i);
                return;
            } else {
                filePermission = checkFilePermission(m_fileName.absoluteFilePath());
                if (!filePermission) {
                    showWarningDialog(tr("You do not have permission to compress %1").arg(m_fileName.fileName()), i);
                    return;
                }
            }
        }
    }

    qDebug() << "开始设置参数！";
    QMap< QString, QString > m_openArgs;
    const QString password = "";
    QString fixedMimeType;
    if (!m_supportedMimeTypes.size()) {
        m_supportedMimeTypes = m_pluginManger.supportedWriteMimeTypes(PluginManager::SortByComment);
    }

    for (const QString &type : qAsConst(m_supportedMimeTypes)) {
        if (0 == QMimeDatabase().mimeTypeForName(type).preferredSuffix().compare(cFileInfo.completeSuffix(), Qt::CaseInsensitive)) {
            fixedMimeType = type;
            break;
        }
    }

    m_openArgs[QStringLiteral("createNewArchive")] = QStringLiteral("false");
    m_openArgs[QStringLiteral("fixedMimeType")] = fixedMimeType;
    if ("application/x-tar" == fixedMimeType || "application/x-tarz" == fixedMimeType) {
        m_openArgs[QStringLiteral("compressionLevel")] = "-1";  //-1 is unuseful
    } else {
        m_openArgs[QStringLiteral("compressionLevel")] = "6";  // 6 is default
    }

    if (m_splitnumedit && m_splitnumedit->value() > 0) {
        m_openArgs[QStringLiteral("volumeSize")] = QString::number(static_cast< int >(m_splitnumedit->value() * 1024));
    }

    //    if (!dialog.data()->compressionMethod().isEmpty()) {
    //        m_openArgs.metaData()[QStringLiteral("compressionMethod")] = dialog.data()->compressionMethod();
    //    }

    if (m_password && !m_password->text().isEmpty()) {
        m_openArgs[QStringLiteral("encryptionMethod")] = "AES256";  // 5 is default
    }

    m_openArgs[QStringLiteral("encryptionPassword")] = password;

    m_openArgs[QStringLiteral("encryptHeader")] = QStringLiteral("false");

    m_openArgs[QStringLiteral("localFilePath")] = cFileInfo.path();
    m_openArgs[QStringLiteral("filename")] = cFileInfo.fileName();
//        cFileInfo.baseName() + "." + QMimeDatabase().mimeTypeForName(fixedMimeType).preferredSuffix();

    m_openArgs[QStringLiteral("sourceFilePath")] = archive;
    if (files.size()) {
        QString paths = files[0];
        for (int i = 1 ; i < files.size() ; i++) {
            paths += "--" + files[i];
        }

        m_openArgs[QStringLiteral("ToCompressFilePath")] = paths;
    }

    emit sigMoveFilesToArchive(m_openArgs);

    m_openArgs.remove(QStringLiteral("createNewArchive"));
    m_openArgs.remove(QStringLiteral("fixedMimeType"));
    m_openArgs.remove(QStringLiteral("compressionLevel"));
    m_openArgs.remove(QStringLiteral("encryptionPassword"));
    m_openArgs.remove(QStringLiteral("encryptHeader"));
    m_openArgs.remove(QStringLiteral("localFilePath"));
    m_openArgs.remove(QStringLiteral("filename"));
    m_openArgs.remove(QStringLiteral("sourceFilePath"));
    m_openArgs.remove(QStringLiteral("ToCompressFilePath"));
}

int CompressSetting::showWarningDialog(const QString &msg, int index, const QString &strTitle, DDialog *pDialogShow)
{
    DDialog *dialog = pDialogShow;
    if (dialog == nullptr) {
        dialog = new DDialog(this);
    }

    QPixmap pixmap = Utils::renderSVG(":assets/icons/deepin/builtin/icons/compress_warning_32px.svg", QSize(32, 32));
    dialog->setIcon(pixmap);
    // dialog->addSpacing(32);
    int wMin = 380;
    dialog->setMinimumSize(wMin, 140);
    if (pDialogShow == nullptr) {
        dialog->addButton(tr("OK"), true, DDialog::ButtonNormal);
    }

    DPalette pa;

    DWidget *pWidget = new DWidget(dialog);
    QVBoxLayout *pLayout = new QVBoxLayout(pWidget);
    pLayout->setContentsMargins(0, 0, 0, 0);

    if (!strTitle.isEmpty()) {
        //dialog->setMinimumSize(wMin, 180);
        DLabel *pTitle = new DLabel(strTitle/*, dialog*/);
        pTitle->setMinimumSize(QSize(154, 20));
        pTitle->setAlignment(Qt::AlignmentFlag::AlignHCenter);
        pa = DApplicationHelper::instance()->palette(pTitle);
        pa.setBrush(DPalette::Text, pa.color(DPalette::ToolTipText));
        DFontSizeManager::instance()->bind(pTitle, DFontSizeManager::T5, QFont::Medium);
        pTitle->setMinimumWidth(dialog->width());
        //pTitle->move(dialog->width() / 2 - pTitle->width() / 2, 60);

        DPalette palette = DApplicationHelper::instance()->palette(pTitle);
        QColor color;
        if (DGuiApplicationHelper::LightType == DGuiApplicationHelper::instance()->themeType()) {
            color = palette.color(DPalette::ToolTipText);
        } else {
            color = palette.color(DPalette::TextLively);
        }

        color.setAlphaF(1);
        palette.setColor(DPalette::Foreground, color);
        DApplicationHelper::instance()->setPalette(pTitle, palette);

        pLayout->addWidget(pTitle, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    }

    DLabel *pContent = new DLabel(msg/*, dialog*/);
    pContent->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    pa = DApplicationHelper::instance()->palette(pContent);
    pa.setBrush(DPalette::Text, pa.color(DPalette::ButtonText));
    DFontSizeManager::instance()->bind(pContent, DFontSizeManager::T6, QFont::Medium);
    pContent->setMinimumWidth(dialog->width());
    //pContent->move(dialog->width() / 2 - pContent->width() / 2, /*dialog->height() / 2 - pContent->height() / 2 - 10 */iMoveY);

    pLayout->addWidget(pContent, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    pWidget->setLayout(pLayout);
    dialog->addContent(pWidget);

    if (isMinimized()) {
        setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    }

    int res = dialog->exec();
    delete dialog;

    if (!filePermission) {
        //emit sigFileUnreadable(m_pathlist, index);
        emit sigFileUnreadable(m_pathlist, index);
    }

    filePermission = true;

    return res;
}

bool CompressSetting::existSameFileName()
{
    DDialog *dialog = new DDialog(this);
//    dialog->setMinimumSize(QSize(380, 190));
    dialog->setMinimumSize(380, 134);
    QPixmap pixmap = Utils::renderSVG(":assets/icons/deepin/builtin/icons/compress_warning_32px.svg", QSize(64, 64));
    dialog->setIcon(pixmap);

    DLabel *strlabel = new DLabel(dialog);
    strlabel->setMinimumSize(QSize(154, 20));
    strlabel->setAlignment(Qt::AlignCenter);
    DFontSizeManager::instance()->bind(strlabel, DFontSizeManager::T6, QFont::Medium);
    strlabel->setText(tr("Another file with the same name already exists, replace it?"));

    dialog->addButton(tr("Cancel"));
    dialog->addButton(tr("Replace"), true, DDialog::ButtonWarning);

    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->setContentsMargins(0, 0, 0, 0);
    mainlayout->addWidget(strlabel, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    DWidget *widget = new DWidget(dialog);

    widget->setLayout(mainlayout);
    dialog->addContent(widget);

    const int mode = dialog->exec();

    if (mode == QDialog::Accepted) {
        return true;
    } else {
        return false;
    }
}

bool CompressSetting::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_compresstype) {
        if (QEvent::KeyPress == event->type()) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (Qt::Key_Enter == keyEvent->key() || Qt::Key_Return == keyEvent->key()) { //响应"回车键"
                m_typemenu->popup(m_compresstype->mapToGlobal(m_compresstype->pos()));
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        // pass the event on to the parent class
        return DWidget::eventFilter(watched, event);
    }
}
