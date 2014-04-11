#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#define QSETTINGS_MAINWINDOW_KEYWORD "MagneeCuistax2014DataViewer_MainWindow"

namespace Ui {
    class MainWindow;
}

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:

        /*!
         * \brief MainWindow : default constructor
         * \param parent : pointer to a parent widget if exists
         */
        explicit MainWindow(QWidget* parent = 0);

        /*!
         * \brief ~MainWindow : virtual destructor
         */
        virtual ~MainWindow(void);

    protected:

        Ui::MainWindow* ui;
};

#endif /* __MAINWINDOW_HPP__ */
