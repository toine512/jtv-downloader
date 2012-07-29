#include "NewUpdateTab.h"
#include <QDebug>
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
    QString notice = QString("Update %1 is available. <a href=\"%2\">%2</a>").arg(new_version_human, dl_link);
    lab_notice->setText(notice);
    if(b_auto_insert)
        p_bar->addTab(this, QString("Update available! (v. %1)").arg(new_version_human));
}

void NewUpdateTab::setUpdateNotes(const QString &notes)
{
    pte_notes->setPlainText(notes);
}
