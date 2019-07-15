#include "RotateDialog.h"

namespace gui {
    RotateDialog::RotateDialog(Facade *facade, std::string name):
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

    void RotateDialog::okClicked(){
    	QString x = dialog.x->displayText();
    	QString y = dialog.y->displayText();
    	QString degrees = dialog.degrees->displayText();
    	bool origin = dialog.origin->isChecked();
    	bool point = dialog.point->isChecked();
    	bool center = dialog.center->isChecked();

    	if ((origin && facade_->rotateObjectOrigin(name_,degrees.toStdString().c_str()))
    			|| (point && facade_->rotateObjectPoint(name_,degrees.toStdString().c_str(),x.toStdString().c_str(),y.toStdString().c_str()))
    			|| (center && facade_->rotateObjectCenter(name_,degrees.toStdString().c_str()))){
    		done(0);
    	}



    }

}
