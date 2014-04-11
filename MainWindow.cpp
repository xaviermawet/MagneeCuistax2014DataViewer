#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    /* The value is used by the QSettings class when it is constructed using
     * the empty constructor. This saves having to repeat this information each
     * time a QSettings object is created.
     */
    QCoreApplication::setOrganizationName("N4k1m");
    QCoreApplication::setApplicationName("MagneeCuistax2014DataViewer");

    // Encoding configuration
    #if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    #endif

    // GUI Configuration
    this->ui->setupUi(this);

    // Restore previous MainWindow layout settings
    this->readSettings();

    // Connect to previous database if exists
    if (DataBaseManager::restorePreviousDataBase())
    {
        // Display project file path in the main window title
        QFileInfo dbFile(QSqlDatabase::database().databaseName());
        this->setWindowTitle(tr("Cuistax Data Viewer - ") + dbFile.baseName());

        this->ui->statusBar->showMessage(
                    tr("Latest project automatically loaded"), 4000);
    }
}

MainWindow::~MainWindow(void)
{
    delete this->ui;
}

void MainWindow::centerOnScreen(void)
{
    QDesktopWidget screen;

    QRect screenGeom = screen.screenGeometry(this);

    int screenCenterX = screenGeom.center().x();
    int screenCenterY = screenGeom.center().y();

    this->move(screenCenterX - width () / 2, screenCenterY - height() / 2);
}

void MainWindow::readSettings(void)
{
    // Restore MainWindow settings
    this->readLayoutSettings(QSETTINGS_MAINWINDOW_KEYWORD);

    // Other settings to restore...
}

void MainWindow::writeSettings(void) const
{
    // Save MainWindow settings
    this->writeLayoutSettings(QSETTINGS_MAINWINDOW_KEYWORD);

    // Other settings to save...
}

void MainWindow::readLayoutSettings(const QString& settingsGroup)
{
    QSettings settings;

    settings.beginGroup(settingsGroup);

    this->restoreGeometry(settings.value("geometry").toByteArray());

    // Other MainWindow Settings ...

    settings.endGroup();
}

void MainWindow::writeLayoutSettings(const QString& settingsGroup) const
{
    QSettings settings;

    settings.beginGroup(settingsGroup);

    //settings.setValue("isMaximized",this->isMaximized());
    settings.setValue("geometry", this->saveGeometry());

    // Other MainWindow Settings ...

    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // Save the state of the MainWindow and its widgets
    this->writeSettings();

    QMainWindow::closeEvent(event);
}

bool MainWindow::updateDataBase(const QString &dbFilePath,
                                bool (*dataBaseAction)(const QString &))
{
    /* ---------------------------------------------------------------------- *
     *                 Delete all sql models based on table(s)                *
     * ---------------------------------------------------------------------- */

    // TODO

    /* ---------------------------------------------------------------------- *
     *                           Action on data base                          *
     * ---------------------------------------------------------------------- */
    bool actionSucceed = (*dataBaseAction)(dbFilePath);

    // Hide or show settings menu
    // TODO

    // if action failed. Nothing else to do. return false
    if (!actionSucceed)
        return false;

    // Otherwise, display project file path in the main window title
    QFileInfo dbFile(QSqlDatabase::database().databaseName());
    this->setWindowTitle(tr("Cuistax Data Viewer - ") + dbFile.baseName());

    // Create models based on the database
    // TODO

    return true;
}

void MainWindow::on_actionOpenProject_triggered(void)
{
    // Get existing project file path
    QString dbFilePath = QFileDialog::getOpenFileName(
                this, tr("Open project file"), QDir::homePath(),
                tr("Projet Magnee Cuistax (*.db)"));

    if (dbFilePath.isEmpty()) // User canceled (nothing to do)
        return;

    try
    {
        // Open the database
        if (this->updateDataBase(dbFilePath, DataBaseManager::openExistingDataBase))
            this->statusBar()->showMessage(
                    tr("Project successfully opened"), 4000);
        else
            this->statusBar()->showMessage(
                    tr("Error : project not opened"), 4000);
    }
    catch (NException const& exception)
    {
        QMessageBox::warning(this, tr("Enable to open project file"),
                             exception.what());
    }
}

void MainWindow::on_actionQuit_triggered(void)
{
    // Save the state of the MainWindow and its widgets
    this->writeSettings();

    qApp->quit();
}
