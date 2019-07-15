#ifndef SURFACEDIALOG_H
#define SURFACEDIALOG_H

#include "Facade.h"
#include "ui_SurfaceDialog.h"

using namespace controller;

namespace gui {
    class SurfaceDialog : public QDialog {
        Q_OBJECT

        public:
            SurfaceDialog(Facade *facade);

        private slots:
        	void addClicked();
        	void okClicked();
        	void loadClicked();

        private:
            Ui_SurfaceDialog dialog;
            Facade *facade_;
    };
}


#endif
