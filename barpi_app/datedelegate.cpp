#include "datedelegate.h"
#include <QDateEdit>


QWidget* DateDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QDateEdit* editor = new QDateEdit(parent);
  editor->setDisplayFormat("MM/dd/yyyy");
  //editor->setCalendarPopup(true);
  return (editor);
}

void DateDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
  QDateEdit* DateEditor = static_cast<QDateEdit*>(editor);
  DateEditor->setDisplayFormat("MM/dd/yyyy");
  DateEditor->setDateTime(index.data(Qt::EditRole).toDateTime());
}

void DateDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
  QDateEdit *DateEditor = static_cast<QDateEdit *>(editor);
  DateEditor->interpretText();
  model->setData(index, DateEditor->dateTime().date(),Qt::EditRole);
  DateEditor->setDisplayFormat("MM/dd/yyyy");
}

void DateDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  editor->setGeometry(option.rect);
}

