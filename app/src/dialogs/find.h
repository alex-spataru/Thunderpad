#ifndef FIND_H
#define FIND_H

#include <QWidget>

namespace Ui
{
class Find;
}

class Find : public QWidget
{
        Q_OBJECT

    public:
        explicit Find (QWidget *parent = 0);
        ~Find();

    private:
        Ui::Find *ui;
};

#endif // FIND_H
