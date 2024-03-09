 #include "recordrowwidget.h"

RecordRowWidget::RecordRowWidget(Record* record, int index, QByteArray &pin_key, QWidget* parent)
    : QWidget(parent){
    this->record = record;
    numberLabel.setText(QString("%1").arg(index));
    nameLabel.setText(record->getDatas(pin_key).recordName);
    removeButton.setText("X");
    numberLabel.setMaximumWidth(60);
    removeButton.setMaximumWidth(30);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(&numberLabel);
    layout->addWidget(&nameLabel);
    layout->addWidget(&removeButton);
    connect(&removeButton, &QPushButton::clicked, this, &RecordRowWidget::on_removeButton_clicked);
    this->setLayout(layout);
}

void RecordRowWidget::updateText() {
    nameLabel.setText(record->recordName);
    removeButton.setText("X");
}

RecordRowWidget::~RecordRowWidget() {}

Record* RecordRowWidget::getRecord()
{
    return record;
}

void RecordRowWidget::on_removeButton_clicked() {
    emit removeRecord(record);
}
