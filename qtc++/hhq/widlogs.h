#ifndef WIDLOGS_H
#define WIDLOGS_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QPlainTextEdit>

class widLogs : public QDialog
{
    Q_OBJECT
public:
    explicit widLogs(QWidget *parent = nullptr);
    ~widLogs();
signals:

public slots:

private:

    QPlainTextEdit *m_edit;

    void load_logs();
};

#endif // WIDLOGS_H
