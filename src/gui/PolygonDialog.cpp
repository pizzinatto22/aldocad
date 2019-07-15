#include "PolygonDialog.h"

#include <QtWidgets/QTableWidgetItem>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

namespace gui {
	PolygonDialog::PolygonDialog(Facade *facade):
    QDialog{nullptr},
    facade_{facade}
    {
        dialog.setupUi(this);

        dialog.tableWidget->setColumnCount(2);
        dialog.tableWidget->setHorizontalHeaderLabels(QString("X;Y").split(";"));

        connect(dialog.addButton,
                SIGNAL(clicked()),
                this,
                SLOT(addClicked()));

        connect(dialog.buttonBox,
                SIGNAL(accepted()),
                this,
                SLOT(okClicked()));
    }

	void PolygonDialog::addClicked()
	{
    	QString x = dialog.xCoordinate->displayText();
    	QString y = dialog.yCoordinate->displayText();

    	int rowCount = dialog.tableWidget->rowCount();
    	dialog.tableWidget->setRowCount(rowCount+1);

    	dialog.tableWidget->setItem(rowCount,0,new QTableWidgetItem(x));
    	dialog.tableWidget->setItem(rowCount,1,new QTableWidgetItem(y));

    	dialog.xCoordinate->setText("");
    	dialog.yCoordinate->setText("");

    	dialog.xCoordinate->setFocus();
	}

	void PolygonDialog::okClicked()
	{
		vector<tuple <string, string, string>> coordinates;

    	int rowCount = dialog.tableWidget->rowCount();

    	for (int i = 0; i < rowCount; i++){
    		QTableWidgetItem* x = dialog.tableWidget->item(i, 0);
    		QTableWidgetItem* y = dialog.tableWidget->item(i, 1);
    		tuple <string, string, string> coordinate = make_tuple(
    				x->text().toStdString(),
    				y->text().toStdString(),
    				"0.0");
    		coordinates.push_back(coordinate);
    	}

    	if (facade_->addPolygon(coordinates)){
    		done(0);
    	}
	}
}
