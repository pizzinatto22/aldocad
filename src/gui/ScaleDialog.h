#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H

#include "Facade.h"
#include "ui_ScaleDialog.h"
#include "World.h"

using namespace controller;

namespace gui {
    class ScaleDialog : public QDialog {
        Q_OBJECT

        public:
            ScaleDialog(Facade *facade, std::string name);


        private slots:
        	void okClicked();

        private:
            Ui_ScaleDialog dialog;
            Facade *facade_;
            std::string name_;
    };
}


#endif
