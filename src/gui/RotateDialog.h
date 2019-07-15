#ifndef ROTATEDIALOG_H
#define ROTATEDIALOG_H

#include "Facade.h"
#include "ui_RotateDialog.h"
#include "World.h"

using namespace controller;

namespace gui {
    class RotateDialog : public QDialog {
        Q_OBJECT

        public:
            RotateDialog(Facade *facade, std::string name);


        private slots:
        	void okClicked();

        private:
            Ui_RotateDialog dialog;
            Facade *facade_;
            std::string name_;
    };
}


#endif
