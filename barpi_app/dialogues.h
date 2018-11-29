#ifndef DIALOGUES_H
#define DIALOGUES_H

#include <QWidget>

class QString;
class QStringList;

class Dialogues : public QWidget
{
    Q_OBJECT

public:
    Dialogues();

private slots:
    void addCatalogueItem();
    QStringList listCatalogue();
    QString findItem(QString);
    void delCatalogueItem(QString);

private:
    void createLayout();
};


#endif // DIALOGUES_H
