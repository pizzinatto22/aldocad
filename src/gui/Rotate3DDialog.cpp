#include "Rotate3DDialog.h"

namespace gui {
    Rotate3DDialog::Rotate3DDialog(Facade *facade, std::string name):
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

    void Rotate3DDialog::okClicked(){
    	QString x = dialog.x->displayText();
    	QString y = dialog.y->displayText();
    	QString z = dialog.z->displayText();

    	QString dx = dialog.dx->displayText();
    	QString dy = dialog.dy->displayText();
    	QString dz = dialog.dz->displayText();

    	bool origin = dialog.origin->isChecked();
    	bool point = dialog.point->isChecked();
    	bool center = dialog.center->isChecked();

    	if ((origin && facade_->rotateObject3DOrigin(name_,dx.toStdString().c_str(),dy.toStdString().c_str(),dz.toStdString().c_str()))
    			|| (point && facade_->rotateObject3DPoint(name_,dx.toStdString().c_str(),dy.toStdString().c_str(),dz.toStdString().c_str(),x.toStdString().c_str(),y.toStdString().c_str(),z.toStdString().c_str()))
    			|| (center && facade_->rotateObject3DCenter(name_,dx.toStdString().c_str(),dy.toStdString().c_str(),dz.toStdString().c_str()))){
    		done(0);
    	}



    }

}
