#include <QDebug>
#include "mylog.h"
#include <QHBoxLayout>
#include "widlogs.h"

widLogs::widLogs(QWidget *parent) : QDialog(parent)
{
    QFont font = this->font();
    font.setFamily("Arial");
    font.setPointSize(12);
    font.setBold(false);
    setFont(font);

    resize(700, 700);
    setWindowTitle("Logs");
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    QString  LE_style = "background-color: black;\n"
                        "color: white";

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    m_edit = new QPlainTextEdit();

    m_edit->resize(size());
    m_edit->setReadOnly(true);
    m_edit->setFont(font);
    m_edit->setStyleSheet(LE_style);
    layout->addWidget(m_edit);

    load_logs();
}

void widLogs::load_logs()
{
    qDebug() << "widLogs::" << __FUNCTION__;
    QFile log("hhq.log");
    log.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream log_stream(&log);
    QStringList log_lines;
    int lines = 0;
    while (!log_stream.atEnd())
    {
        lines ++;
        log_lines.append(log_stream.readLine());

    }
    log.close();

    qDebug() << "lines=" <<lines;

    if (0)
    {
        //do display it with normal order
        QList<QString>::Iterator it = log_lines.begin(),itend = log_lines.end();
        for (;it != itend; it++)
        {
            m_edit->appendPlainText(*it);
        }
    }
    else
    {
        //to display the logs with latest-first order
        for(int i = log_lines.size()-1; i >=0; i--)
        {
            m_edit->appendPlainText(log_lines[i]);
        }
        //move the cursor to the beginning
        QTextCursor cursor = m_edit->textCursor();
        cursor.setPosition(0);
        m_edit->setTextCursor(cursor);
        m_edit->ensureCursorVisible();
    }

}
widLogs::~widLogs()
{
    qDebug() << __FUNCTION__;
}
