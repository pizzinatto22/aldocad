#ifndef BSPLINEDIALOG_H
#define BSPLINEDIALOG_H

#include "Facade.h"
#include "ui_BSplineDialog.h"

using namespace controller;

namespace gui {
    class BSplineDialog : public QDialog {
        Q_OBJECT

        public:
            BSplineDialog(Facade *facade);

        private slots:
        	void addClicked();
        	void okClicked();

        private:
            Ui_BSplineDialog dialog;
            Facade *facade_;
    };
}


#endif
