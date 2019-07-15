#ifndef TRANSLATEDIALOG_H
#define TRANSLATEDIALOG_H

#include "Facade.h"
#include "ui_TranslateDialog.h"
#include "World.h"

using namespace controller;

namespace gui {
    class TranslateDialog : public QDialog {
        Q_OBJECT

        public:
            TranslateDialog(Facade *facade, std::string name);


        private slots:
        	void okClicked();

        private:
            Ui_TranslateDialog dialog;
            Facade *facade_;
            std::string name_;
    };
}


#endif
