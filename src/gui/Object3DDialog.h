#ifndef OBJCT3DDIALOG_H
#define OBJCT3DDIALOG_H

#include "Facade.h"
#include "ui_Object3DDialog.h"

using namespace controller;

namespace gui {
    class Object3DDialog : public QDialog {
        Q_OBJECT

        public:
            Object3DDialog(Facade *facade);

        private slots:
        	void addClicked();
        	void okClicked();

        private:
            Ui_Object3DDialog dialog;
            Facade *facade_;
    };
}


#endif
