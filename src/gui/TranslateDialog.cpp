#include "TranslateDialog.h"

namespace gui {
    TranslateDialog::TranslateDialog(Facade *facade, std::string name):
    QDialog{nullptr},
    facade_{facade},
    name_{name}
    {
        dialog.setupUi(this);

        connect(dialog.buttonBox,
                SIGNAL(accepted()),
                this,
                SLOT(okClicked()));

    }

    void TranslateDialog::okClicked(){
    	QString dx = dialog.dx->displayText();
    	QString dy = dialog.dy->displayText();
    	QString dz = dialog.dz->displayText();

    	if (facade_->translateObject(name_,
    			dx.toStdString().c_str(),
    			dy.toStdString().c_str(),
    			dz.toStdString().c_str()))
    		done(0);
    }

}
