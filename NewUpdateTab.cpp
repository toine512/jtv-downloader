#include "NewUpdateTab.h"

NewUpdateTab::NewUpdateTab(bool auto_self_insert, QTabWidget *tab_widget, QWidget *parent) :
    QWidget(parent)
{
    if(auto_self_insert && tab_widget != 0)
    {
        p_bar = tab_widget;
        b_auto_insert = true;
    }
    else
    {
        p_bar = 0;
        b_auto_insert = false;
    }

    lab_notice = new QLabel;
    lab_notice->setOpenExternalLinks(true);

    pte_notes = new QPlainTextEdit;
    pte_notes->setReadOnly(true);

    layout = new QVBoxLayout;
    layout->addWidget(lab_notice);
    layout->addWidget(pte_notes);

    setLayout(layout);
}

void NewUpdateTab::setUpdateInfos(const QString &new_version_human, const QString &dl_link)
{
#ifdef Q_OS_WIN
    lab_notice->setText(QString("Update <b>%1</b> is available. <a href=\"%2\">%2</a>").arg(new_version_human, dl_link));
#else
    lab_notice->setText(QString("Update <b>%1</b> is available.<br />Please wait until the PPA is updated.<br />If you've installed manually, get the packet here: <a href=\"%2\">%2</a>.<br />Or recompile from <a href=\"https://github.com/toine512/jtv-downloader\">sources</a>.").arg(new_version_human, dl_link));
#endif
    if(b_auto_insert)
        p_bar->addTab(this, QString("Update available! (v. %1)").arg(new_version_human));
}

void NewUpdateTab::setUpdateNotes(const QString &notes)
{
    pte_notes->setPlainText(notes);
}
