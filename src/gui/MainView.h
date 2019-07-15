#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "Facade.h"
#include "ui_MainView.h"

#include <QtWidgets/QGraphicsView>

using namespace controller;


namespace gui {
    class MainView : public QMainWindow {
        Q_OBJECT

        public:
            MainView(Facade *facade);
            QGraphicsView *viewportUI();
            QTableWidget *objectsTableUI();

        private slots:
        	void menuLoadFileClicked();
            void menuAddPointClicked();
            void menuAddLineClicked();
            void menuAddPolygonClicked();
            void menuAddBSplineClicked();
            void menuAddObject3DClicked();
            void menuAddSurfaceClicked();

            void translateClicked();
            void scaleClicked();
            void rotateClicked();

            void upClicked();
            void downClicked();
            void leftClicked();
            void rightClicked();

            void centerClicked();

            void zoomPlusClicked();
            void zoomMinusClicked();

            void rotateWorldClicked();

            void algorithmChanged();
            void projectionChanged();
            void projectionDistanceChanged(int i);

            void clearWorldClicked();

        private:
            Ui_MainWindow view;
            Facade *facade_;
            std::string objectName();
    };
}


#endif
