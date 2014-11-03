#ifndef FINDREPLACE_H
#define FINDREPLACE_H

#include <QWidget>

namespace Ui
{
class FindReplace;
}

class FindReplace : public QWidget
{
        Q_OBJECT

    public:
        explicit FindReplace (QWidget *parent = 0);
        ~FindReplace();

    private:
        Ui::FindReplace *ui;
};

#endif // FINDREPLACE_H
