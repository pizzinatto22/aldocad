#include "PointDialog.h"

namespace gui {
    PointDialog::PointDialog(Facade *facade):
    QDialog{nullptr},
    facade_{facade}
    {
        dialog.setupUi(this);

        connect(dialog.buttonBox,
                SIGNAL(accepted()),
                this,
                SLOT(okClicked()));

    }

    void PointDialog::okClicked(){
    	QString x = dialog.xCoordinate->displayText();
    	QString y = dialog.yCoordinate->displayText();

    	if (facade_->addPoint(x.toStdString().c_str(),
    			y.toStdString().c_str()))
    		done(0);

    }

}
