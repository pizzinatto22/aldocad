#ifndef LINEDIALOG_H
#define LINEDIALOG_H

#include "Facade.h"
#include "ui_LineDialog.h"

using namespace controller;

namespace gui {
    class LineDialog : public QDialog {
        Q_OBJECT

        public:
            LineDialog(Facade *facade);

        private slots:
        	void okClicked();

        private:
            Ui_LineDialog dialog;
            Facade *facade_;

    };
}


#endif
