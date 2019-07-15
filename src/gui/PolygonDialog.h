#ifndef POLYGONDIALOG_H
#define POLYGONDIALOG_H

#include "Facade.h"
#include "ui_PolygonDialog.h"

using namespace controller;

namespace gui {
    class PolygonDialog : public QDialog {
        Q_OBJECT

        public:
            PolygonDialog(Facade *facade);

        private slots:
        	void addClicked();
        	void okClicked();

        private:
            Ui_PolygonDialog dialog;
            Facade *facade_;
    };
}


#endif
