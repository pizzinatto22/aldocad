#include "SurfaceDialog.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTableWidgetItem>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

namespace gui {
	SurfaceDialog::SurfaceDialog(Facade *facade):
    QDialog{nullptr},
    facade_{facade}
    {
        dialog.setupUi(this);

        dialog.tableWidget->setColumnCount(4);
        dialog.tableWidget->setHorizontalHeaderLabels(QString("Line;X;Y;Z").split(";"));


        connect(dialog.addButton,
                SIGNAL(clicked()),
                this,
                SLOT(addClicked()));

        connect(dialog.buttonBox,
                SIGNAL(accepted()),
                this,
                SLOT(okClicked()));

        connect(dialog.load,
                SIGNAL(clicked()),
                this,
                SLOT(loadClicked()));

    }

	void SurfaceDialog::addClicked()
	{
		QString line = dialog.line->displayText();
    	QString x = dialog.xCoordinate->displayText();
    	QString y = dialog.yCoordinate->displayText();
    	QString z = dialog.zCoordinate->displayText();

    	int rowCount = dialog.tableWidget->rowCount();
    	dialog.tableWidget->setRowCount(rowCount+1);

    	dialog.tableWidget->setItem(rowCount,0,new QTableWidgetItem(line));
    	dialog.tableWidget->setItem(rowCount,1,new QTableWidgetItem(x));
    	dialog.tableWidget->setItem(rowCount,2,new QTableWidgetItem(y));
    	dialog.tableWidget->setItem(rowCount,3,new QTableWidgetItem(z));

    	dialog.xCoordinate->setText("");
    	dialog.yCoordinate->setText("");
    	dialog.zCoordinate->setText("");

    	dialog.line->setFocus();
	}

	void SurfaceDialog::okClicked()
	{
		vector<tuple<string, string, string, string>> coordinates;

    	int rowCount = dialog.tableWidget->rowCount();

    	for (int i = 0; i < rowCount; i++){
    		QTableWidgetItem* line = dialog.tableWidget->item(i, 0);
    		QTableWidgetItem* x = dialog.tableWidget->item(i, 1);
    		QTableWidgetItem* y = dialog.tableWidget->item(i, 2);
    		QTableWidgetItem* z = dialog.tableWidget->item(i, 3);

    		cout << line->text().toStdString() << endl;

    		tuple <string, string, string, string> coordinate = make_tuple(
    				line->text().toStdString(),
    				x->text().toStdString(),
    				y->text().toStdString(),
    				z->text().toStdString());
    		coordinates.push_back(coordinate);
    	}

    	if (facade_->addSurface(coordinates)){
    		done(0);
    	}
	}

	void SurfaceDialog::loadClicked()
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		                                                 "",
		                                                 tr("Files (*.*)"));

		if (fileName.length()) {
			vector<vector<string>> file = facade_->loadCSV(fileName.toStdString());

			dialog.tableWidget->clearContents();

			dialog.tableWidget->setRowCount(file.size());

			int row = 0;

			for (auto l:file){

				int col = 0;
		    	for (auto c:l){
		    		dialog.tableWidget->setItem(row,col,new QTableWidgetItem(QString::fromStdString(c)));
		    		col++;
		    	}

		    	row++;

			}

		}

	}
}
