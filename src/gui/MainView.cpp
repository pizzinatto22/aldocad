#include "BSplineDialog.h"
#include "MainView.h"
#include "LineDialog.h"
#include "Object3DDialog.h"
#include "PointDialog.h"
#include "PolygonDialog.h"
#include "Rotate3DDialog.h"
#include "RotateDialog.h"
#include "ScaleDialog.h"
#include "SurfaceDialog.h"
#include "TranslateDialog.h"

#include <iostream>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>


namespace gui {
    MainView::MainView(Facade *facade):
    QMainWindow{nullptr},
    facade_{facade}
    {
        view.setupUi(this);

        view.objectsTable->setColumnCount(2);
        view.objectsTable->setHorizontalHeaderLabels(QString("Object;Type").split(";"));



        connect(view.actionLoad_file,
					SIGNAL(triggered()),
					this,
					SLOT(menuLoadFileClicked()));

        connect(view.actionClear_world,
				SIGNAL(triggered()),
				this,
				SLOT(clearWorldClicked()));

        connect(view.actionAdd_point,
                SIGNAL(triggered()),
                this,
                SLOT(menuAddPointClicked()));

        connect(view.actionAdd_line,
                SIGNAL(triggered()),
                this,
                SLOT(menuAddLineClicked()));

        connect(view.actionAdd_polygon,
                SIGNAL(triggered()),
                this,
                SLOT(menuAddPolygonClicked()));

        connect(view.actionAdd_BSpline,
                SIGNAL(triggered()),
                this,
                SLOT(menuAddBSplineClicked()));

        connect(view.actionAdd_object_3D,
                SIGNAL(triggered()),
                this,
                SLOT(menuAddObject3DClicked()));

        connect(view.actionAdd_surface,
                SIGNAL(triggered()),
                this,
                SLOT(menuAddSurfaceClicked()));



        connect(view.translate,
                SIGNAL(clicked()),
                this,
                SLOT(translateClicked()));

        connect(view.scale,
                SIGNAL(clicked()),
                this,
                SLOT(scaleClicked()));

        connect(view.rotate,
                SIGNAL(clicked()),
                this,
                SLOT(rotateClicked()));


        connect(view.up,
                SIGNAL(clicked()),
                this,
                SLOT(upClicked()));

        connect(view.down,
                SIGNAL(clicked()),
                this,
                SLOT(downClicked()));

        connect(view.left,
                SIGNAL(clicked()),
                this,
                SLOT(leftClicked()));

        connect(view.right,
                SIGNAL(clicked()),
                this,
                SLOT(rightClicked()));

        connect(view.center,
                SIGNAL(clicked()),
                this,
                SLOT(centerClicked()));


        connect(view.zoomPlus,
                SIGNAL(clicked()),
                this,
                SLOT(zoomPlusClicked()));

        connect(view.zoomMinus,
                SIGNAL(clicked()),
                this,
                SLOT(zoomMinusClicked()));

        connect(view.rotateWorld,
                SIGNAL(clicked()),
                this,
                SLOT(rotateWorldClicked()));

        connect(view.cohenSutherlandClipping,
                SIGNAL(clicked()),
                this,
                SLOT(algorithmChanged()));

        connect(view.liangBarskyClipping,
                SIGNAL(clicked()),
                this,
                SLOT(algorithmChanged()));

        connect(view.parallel,
                SIGNAL(clicked()),
                this,
                SLOT(projectionChanged()));

        connect(view.perspective,
                SIGNAL(clicked()),
                this,
                SLOT(projectionChanged()));

        connect(view.dop,
                SIGNAL(valueChanged(int)),
                this,
                SLOT(projectionDistanceChanged(int)));

    }

    QGraphicsView *MainView::viewportUI()
    {
    	return view.worldView;
    }

    QTableWidget *MainView::objectsTableUI()
    {
    	return view.objectsTable;
    }

    void MainView::menuLoadFileClicked()
    {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		                                                 "",
		                                                 tr("Files (*.*)"));


		if (fileName.length())
			facade_->loadFile(fileName.toStdString());

    }

    void MainView::menuAddPointClicked()
    {
        PointDialog dialog{facade_};
        dialog.exec();
    }

    void MainView::menuAddLineClicked()
    {
        LineDialog dialog{facade_};
        dialog.exec();
    }

    void MainView::menuAddPolygonClicked()
    {
        PolygonDialog dialog{facade_};
        dialog.exec();
    }

    void MainView::menuAddBSplineClicked()
    {
        BSplineDialog dialog{facade_};
        dialog.exec();
    }

    void MainView::menuAddObject3DClicked()
    {
        Object3DDialog dialog{facade_};
        dialog.exec();
    }

    void MainView::menuAddSurfaceClicked()
    {
        SurfaceDialog dialog{facade_};
        dialog.exec();
    }

    void MainView::translateClicked()
    {
    	string name = objectName();
    	if (name.length() > 0) {
			TranslateDialog dialog{facade_,name};
			dialog.exec();
    	} else {
    		facade_->message("There's no selected object");
    	}

    }

    void MainView::scaleClicked()
    {
    	string name = objectName();
    	if (name.length() > 0) {
			ScaleDialog dialog{facade_,name};
			dialog.exec();
    	} else {
    		facade_->message("There's no selected object");
    	}
    }

    void MainView::rotateClicked()
    {
    	string name = objectName();
    	if (name.length() > 0) {
    		if (facade_->objectType(name) == OBJECT3D || facade_->objectType(name) == SURFACE){
    			Rotate3DDialog dialog{facade_,name};
    			dialog.exec();
    		} else {
				RotateDialog dialog{facade_,name};
				dialog.exec();
    		}
    	}
    }

    void MainView::upClicked()
    {
    	facade_->up();
    }

    void MainView::downClicked()
    {
    	facade_->down();
    }

    void MainView::leftClicked()
    {
    	facade_->left();
    }

    void MainView::rightClicked()
    {
    	facade_->right();
    }

    void MainView::centerClicked()
    {
    	facade_->centerWindow();
    }

    void MainView::zoomPlusClicked()
    {
    	facade_->zoomIn();
    }

    void MainView::zoomMinusClicked()
    {
    	facade_->zoomOut();
    }

    std::string MainView::objectName()
    {
    	QItemSelectionModel *select = view.objectsTable->selectionModel();

    	if (select->hasSelection()){
    		QModelIndexList rows = select->selectedRows();

    		int row = rows.at(0).row();

    		return view.objectsTable->item(row,0)->text().toStdString();
    	}

    	return "";
    }

    void MainView::rotateWorldClicked()
    {
    	const char *d = view.worldDegrees->text().toStdString().c_str();
    	facade_->rotateWorld(d);
    }

    void MainView::algorithmChanged()
    {
    	if (view.cohenSutherlandClipping->isChecked())
    		facade_->changeLineClippingAlgorithm(0);
    	else
    		facade_->changeLineClippingAlgorithm(1);

    }

    void MainView::projectionChanged()
    {
    	if (view.parallel->isChecked())
    		facade_->changeProjectionAlgorithm(0);
    	else
    		facade_->changeProjectionAlgorithm(1);
    }

    void MainView::projectionDistanceChanged(int i)
    {
    	facade_->changeProjectionDistance(i);
    }

    void MainView::clearWorldClicked()
    {
    	facade_->clearWorld();
    }


}
