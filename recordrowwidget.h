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
    RecordRowWidget(Record* record, int index, QByteArray &pin_key, QWidget* parent = nullptr);
    ~RecordRowWidget();
    void updateText();
    Record* getRecord();
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
