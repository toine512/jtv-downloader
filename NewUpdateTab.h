#ifndef NEWUPDATETAB_H
#define NEWUPDATETAB_H

#include <QString>

#include <QWidget>
#include <QTabWidget>
#include <QIcon>
#include <QLabel>
#include <QPlainTextEdit>
#include <QVBoxLayout>

class NewUpdateTab : public QWidget
{
    Q_OBJECT

public:
    explicit NewUpdateTab(bool auto_self_insert = false, QTabWidget *tab_widget = 0, QWidget *parent = 0);
    
public slots:
    void setUpdateInfos(const QString &new_version_human, const QString &dl_link);
    void setUpdateNotes(const QString &notes);

protected:
    bool b_auto_insert;
    QTabWidget *p_bar;

    QLabel *lab_notice;
    QPlainTextEdit *pte_notes;
    QVBoxLayout *layout;
};

#endif // NEWUPDATETAB_H
