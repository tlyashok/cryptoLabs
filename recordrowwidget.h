#ifndef RECORDROWWIDGET_H
#define RECORDROWWIDGET_H

#include "record.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSizePolicy>

class RecordRowWidget : public QWidget {
    Q_OBJECT
public:
    RecordRowWidget(Record* record, int index, QWidget* parent = nullptr);
    ~RecordRowWidget();
    void updateText();
private slots:
    void on_removeButton_clicked();
signals:
    void removeRecord(Record* record);
private:
    Record* record;
    QLabel numberLabel, nameLabel;
    QPushButton removeButton;
};

#endif // RECORDROWWIDGET_H
