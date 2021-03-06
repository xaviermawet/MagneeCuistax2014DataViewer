#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    _comboBoxRaceList(NULL), _modelRaceList(NULL), _currentRaceID(-1)
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
    this->createToolBar();

    // Restore previous MainWindow layout settings
    this->readSettings();

    // Connect to previous database if exists
    if (DataBaseManager::restorePreviousDataBase())
    {
        // Create model(s)
        this->createModelRaceList();

        // Display project file path in the main window title
        QFileInfo dbFile(QSqlDatabase::database().databaseName());
        this->setWindowTitle(tr("Cuistax Data Viewer - ") + dbFile.baseName());

        this->ui->statusBar->showMessage(
                    tr("Latest project automatically loaded"), 4000);
    }
}

MainWindow::~MainWindow(void)
{
    // Widgets
    delete this->ui;
    delete this->_comboBoxRaceList;

    // Models
    delete this->_modelRaceList;
}

void MainWindow::createToolBar(void)
{
    // Mainly developed with Qt Designer

    /* ---------------------------------------------------------------------- *
     *                           Race list combobox                           *
     * ---------------------------------------------------------------------- */
    if(this->_comboBoxRaceList != NULL)
        delete this->_comboBoxRaceList;

    // Create comboBox
    this->_comboBoxRaceList = new QComboBox(this->ui->mainToolBar);
    this->_comboBoxRaceList->setEditable(false);
    this->_comboBoxRaceList->setSizePolicy(QSizePolicy::Expanding,
                                           QSizePolicy::Maximum);

    // Add the comboBox to the mainToolBar
    this->ui->mainToolBar->addWidget(this->_comboBoxRaceList);

    // Update the current race id
    connect(this->_comboBoxRaceList, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentRaceChanged(int)));
}

void MainWindow::createModelRaceList(void)
{
    // Stop if the comboBox (container) doesn't exist
    if (!this->_comboBoxRaceList)
        return;

    if (this->_modelRaceList != NULL)
        delete this->_modelRaceList;

    // Create model
    this->_modelRaceList = new NSqlQueryModel(this);

    // Apply the new model to the comboBobx
    this->_comboBoxRaceList->setModel(this->_modelRaceList);

    // Populate the model
    this->_modelRaceList->setQuery("SELECT name, id FROM RACE");

    #if (QT_VERSION > QT_VERSION_CHECK(5, 0, 0))
    this->_comboBoxRaceList->setCurrentIndex(0);
    #endif
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

    delete this->_modelRaceList;
    this->_modelRaceList = NULL;

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
    this->createModelRaceList();

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

void MainWindow::currentRaceChanged(int currentRaceComboBoxIndex)
{
    if (currentRaceComboBoxIndex < 0) // No row selected in the comboBox
        return;

    this->_currentRaceID = this->_modelRaceList->index(
                currentRaceComboBoxIndex, 1).data().toInt();

    qDebug() << "Mise à jour du race id" << this->_currentRaceID;
}
