#ifndef MTX_MKVTOOLNIX_GUI_JOBS_JOB_MODEL_H
#define MTX_MKVTOOLNIX_GUI_JOBS_JOB_MODEL_H

#include "common/common_pch.h"

#include "mkvtoolnix-gui/jobs/job.h"

#include <QStandardItemModel>
#include <QList>
#include <QMutex>
#include <QSet>

class QAbstractItemView;

namespace mtx { namespace gui { namespace Jobs {

enum class QueueStatus {
  Stopped,
  Running,
};

class Model: public QStandardItemModel {
  Q_OBJECT;
protected:
  QHash<uint64_t, JobPtr> m_jobsById;
  QSet<Job const *> m_toBeProcessed;
  QHash<uint64_t, bool> m_toBeRemoved;
  QMutex m_mutex;
  QIcon m_warningsIcon, m_errorsIcon;

  bool m_started, m_dontStartJobsNow, m_running;

  QDateTime m_queueStartTime;
  int m_queueNumDone;

public:
  // labels << QY("Status") << QY("Description") << QY("Type") << QY("Progress") << QY("Date added") << QY("Date started") << QY("Date finished");
  static int const StatusColumn       = 0;
  static int const StatusIconColumn   = 1;
  static int const DescriptionColumn  = 2;
  static int const TypeColumn         = 3;
  static int const ProgressColumn     = 4;
  static int const DateAddedColumn    = 5;
  static int const DateStartedColumn  = 6;
  static int const DateFinishedColumn = 7;

  static int const RowNotFound        = -1;

public:
  Model(QObject *parent);
  virtual ~Model();

  void retranslateUi();

  uint64_t idFromRow(int row) const;
  Job *fromId(uint64_t id) const;
  int rowFromId(uint64_t id) const;
  bool hasJobs() const;
  bool hasRunningJobs();
  bool isRunning() const;

  void withSelectedJobs(QAbstractItemView *view, std::function<void(Job &)> const &worker);
  void withAllJobs(std::function<void(Job &)> const &worker);
  void withJob(uint64_t id, std::function<void(Job &)> const &worker);

  void removeJobsIf(std::function<bool(Job const &)> predicate);
  void add(JobPtr const &job);

  void start();
  void stop();

  void startNextAutoJob();
  void startJobImmediately(Job &job);

  void loadJobs();

  virtual Qt::DropActions supportedDropActions() const;
  virtual Qt::ItemFlags flags(QModelIndex const &index) const;
  virtual bool canDropMimeData(QMimeData const *data, Qt::DropAction action, int row, int column, QModelIndex const &parent) const override;
  virtual bool dropMimeData(QMimeData const *data, Qt::DropAction action, int row, int column, QModelIndex const &parent) override;

  QDateTime queueStartTime() const;

signals:
  void progressChanged(int progress, int totalProgress);
  void jobStatsChanged(int numPendingAutomatic, int numPendingManual, int numRunning, int numOther);
  void numUnacknowledgedWarningsOrErrorsChanged(int numWarnings, int numErrors);

  void queueStatusChanged(QueueStatus status);

public slots:
  void onStatusChanged(uint64_t id, mtx::gui::Jobs::Job::Status oldStatus, mtx::gui::Jobs::Job::Status newStatus);
  void onProgressChanged(uint64_t id, unsigned int progress);
  void onNumUnacknowledgedWarningsOrErrorsChanged(uint64_t id, int numWarnings, int numErrors);
  void removeScheduledJobs();

  void acknowledgeAllWarnings();
  void acknowledgeSelectedWarnings(QAbstractItemView *view);
  void acknowledgeAllErrors();
  void acknowledgeSelectedErrors(QAbstractItemView *view);

  void resetTotalProgress();

  void saveJobs();

  void runProgramOnQueueStop(QueueStatus status);

protected:
  QList<QStandardItem *> createRow(Job const &job) const;
  void setRowText(QList<QStandardItem *> const &items, Job const &job) const;
  QList<QStandardItem *> itemsForRow(QModelIndex const &idx);

  void updateProgress();
  void updateJobStats();
  void updateNumUnacknowledgedWarningsOrErrors();

  void processAutomaticJobRemoval(uint64_t id, Job::Status status);
  void scheduleJobForRemoval(uint64_t id);

  QList<Job *> selectedJobs(QAbstractItemView *view);

public:
  static void convertJobQueueToSeparateIniFiles();
};

}}}

#endif  // MTX_MKVTOOLNIX_GUI_JOBS_JOB_MODEL_H
