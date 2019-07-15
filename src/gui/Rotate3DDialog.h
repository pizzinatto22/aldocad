#ifndef ROTATE3DDIALOG_H
#define ROTATE3DDIALOG_H

#include "Facade.h"
#include "ui_Rotate3DDialog.h"
#include "World.h"

using namespace controller;

namespace gui {
    class Rotate3DDialog : public QDialog {
        Q_OBJECT

        public:
            Rotate3DDialog(Facade *facade, std::string name);


        private slots:
        	void okClicked();

        private:
            Ui_Rotate3DDialog dialog;
            Facade *facade_;
            std::string name_;
    };
}


#endif
