#include "findreplace.h"
#include "ui_findreplace.h"

FindReplace::FindReplace (QWidget *parent) :
    QWidget (parent),
    ui (new Ui::FindReplace) {
    ui->setupUi (this);
}

FindReplace::~FindReplace() {
    delete ui;
}
