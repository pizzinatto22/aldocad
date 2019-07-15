#include "LineDialog.h"

namespace gui {
	LineDialog::LineDialog(Facade *facade):
    QDialog{nullptr},
    facade_{facade}
    {
        dialog.setupUi(this);

        connect(dialog.buttonBox,
                SIGNAL(accepted()),
                this,
                SLOT(okClicked()));
    }

    void LineDialog::okClicked()
    {
    	QString x1 = dialog.x1Coordinate->displayText();
    	QString y1 = dialog.y1Coordinate->displayText();

    	QString x2 = dialog.x2Coordinate->displayText();
    	QString y2 = dialog.y2Coordinate->displayText();

    	if (facade_->addLine(x1.toStdString().c_str(),
    			y1.toStdString().c_str(),
    			x2.toStdString().c_str(),
    			y2.toStdString().c_str())){
    		done(0);
    	}

    }
}
