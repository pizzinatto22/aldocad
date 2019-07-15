#ifndef POINTDIALOG_H
#define POINTDIALOG_H

#include "Facade.h"
#include "ui_PointDialog.h"
#include "World.h"

using namespace controller;

namespace gui {
    class PointDialog : public QDialog {
        Q_OBJECT

        public:
            PointDialog(Facade *facade);


        private slots:
        	void okClicked();

        private:
            Ui_PointDialog dialog;
            Facade *facade_;
    };
}


#endif
