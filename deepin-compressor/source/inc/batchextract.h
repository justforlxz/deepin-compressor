#ifndef BATCHEXTRACT_H
#define BATCHEXTRACT_H

#include "queries.h"
#include "batchjobs.h"
#include "archivemodel.h"
#include "structs.h"

#include <QMap>
#include <QObject>
#include <QVector>
#include <QUrl>

class SettingDialog;
class BatchExtract : public BatchJobs
{
    Q_OBJECT

public:

    explicit BatchExtract(QObject *parent = nullptr);

    void addExtraction(const QUrl &url);

    bool doKill() override;
    void start() override;
    bool autoSubfolder() const;
    void setAutoSubfolder(bool value);
    void addInput(const QUrl &url);
    bool showExtractDialog();
    QString destinationFolder() const;
    void setDestinationFolder(const QString &folder);
    bool openDestinationAfterExtraction() const;
    void setOpenDestinationAfterExtraction(bool value);
    bool preservePaths() const;
    void setPreservePaths(bool value);
    void setBatchTotalSize(qint64 size);    //批量解压压缩包总大小

Q_SIGNALS:
    void batchProgress(KJob *job, ulong progress);
    void batchFilenameProgress(KJob *job, const QString &name);
    void sendCurFile(const QString  &filename);
    void sendFailFile(const QString  &filename);
    void signalUserQuery(Query *query);

private Q_SLOTS:
    void forwardProgress(KJob *job, unsigned long percent);
    void showFailedFiles();
    void slotResult(KJob *job) override;
    //void slotUserQuery(Query *query);
    void slotStartJob();

    void SlotProgressFile(KJob *job, const QString &name);

private:
    int m_initialJobCount;
    QMap<KJob *, QPair<QString, QString> > m_fileNames;
    bool m_autoSubfolder;

    QList<QUrl> m_inputs;
    QString m_destinationFolder;
    QStringList m_failedFiles;
    bool m_preservePaths;
    bool m_openDestinationAfterExtraction;

    qint64 m_batchTotalSize;     //批量解压压缩包总大小
    int m_numOfExtracting = 0;   //解压到第几个压缩文件
    qint64 m_lastPercent = 0;    //解压完成的压缩文件占总大小的百分比
    SettingDialog *m_settingDialog = nullptr;
    Settings_Extract_Info *m_pSettingInfo = nullptr;

    KJob *m_job;
};

#endif // BATCHEXTRACT_H
