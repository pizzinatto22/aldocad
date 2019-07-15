#include "Object3DDialog.h"

#include <QtWidgets/QTableWidgetItem>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

namespace gui {
	Object3DDialog::Object3DDialog(Facade *facade):
    QDialog{nullptr},
    facade_{facade}
    {
        dialog.setupUi(this);

        dialog.tableWidget->setColumnCount(6);
        dialog.tableWidget->setHorizontalHeaderLabels(QString("X0;Y0;Z0;X1;Y1;Z1").split(";"));

        connect(dialog.addButton,
                SIGNAL(clicked()),
                this,
                SLOT(addClicked()));

        connect(dialog.buttonBox,
                SIGNAL(accepted()),
                this,
                SLOT(okClicked()));
    }

	void Object3DDialog::addClicked()
	{
    	QString x0 = dialog.x0Coordinate->displayText();
    	QString y0 = dialog.y0Coordinate->displayText();
    	QString z0 = dialog.z0Coordinate->displayText();

    	QString x1 = dialog.x1Coordinate->displayText();
    	QString y1 = dialog.y1Coordinate->displayText();
    	QString z1 = dialog.z1Coordinate->displayText();

    	int rowCount = dialog.tableWidget->rowCount();
    	dialog.tableWidget->setRowCount(rowCount+1);

    	dialog.tableWidget->setItem(rowCount,0,new QTableWidgetItem(x0));
    	dialog.tableWidget->setItem(rowCount,1,new QTableWidgetItem(y0));
    	dialog.tableWidget->setItem(rowCount,2,new QTableWidgetItem(z0));
    	dialog.tableWidget->setItem(rowCount,3,new QTableWidgetItem(x1));
    	dialog.tableWidget->setItem(rowCount,4,new QTableWidgetItem(y1));
    	dialog.tableWidget->setItem(rowCount,5,new QTableWidgetItem(z1));


    	dialog.x0Coordinate->setText("");
    	dialog.y0Coordinate->setText("");
    	dialog.z0Coordinate->setText("");
    	dialog.x1Coordinate->setText("");
    	dialog.y1Coordinate->setText("");
    	dialog.z1Coordinate->setText("");


    	dialog.x0Coordinate->setFocus();
	}

	void Object3DDialog::okClicked()
	{
		vector<tuple <string, string, string>> coordinates;

    	int rowCount = dialog.tableWidget->rowCount();

    	for (int i = 0; i < rowCount; i++){
    		QTableWidgetItem* x0 = dialog.tableWidget->item(i, 0);
    		QTableWidgetItem* y0 = dialog.tableWidget->item(i, 1);
    		QTableWidgetItem* z0 = dialog.tableWidget->item(i, 2);
    		QTableWidgetItem* x1 = dialog.tableWidget->item(i, 3);
    		QTableWidgetItem* y1 = dialog.tableWidget->item(i, 4);
    		QTableWidgetItem* z1 = dialog.tableWidget->item(i, 5);

    		tuple <string, string, string> coordinate = make_tuple(
    				x0->text().toStdString(),
    				y0->text().toStdString(),
    				z0->text().toStdString());
    		coordinates.push_back(coordinate);

    		coordinate = make_tuple(
    				x1->text().toStdString(),
    				y1->text().toStdString(),
    				z1->text().toStdString());
    		coordinates.push_back(coordinate);

    	}

    	if (facade_->addObject3D(coordinates)){
    		done(0);
    	}
	}
}
