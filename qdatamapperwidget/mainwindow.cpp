#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("QDataWidgetMapper "
                         "База данных на виджетах через мэппер");
    /* Первым делом необходимо создать объект для работы с базой данных
     * и инициализировать подключение к базе данных
     * */
    db = new DataBase();
    db->connectToDataBase();

    /* Инициализируем модели для представления данных
     * с заданием названий колонок
     * */
    this->setupModel(DEVICE,
                     QStringList() << tr("id")
                                         << tr("Имя хоста")
                                         << tr("IP адрес")
                                         << tr("MAC-адрес")
               );
    /* Инициализируем внешний вид таблицы с данными
     * */
    this->createUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Метод для инициализации модели
 * -------------------------------------------------------------------------------------------------
 * */
void MainWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * */
    modelDevice = new QSqlTableModel(this);
    modelDevice->setTable(tableName);
    modelDevice->select();
    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
    for(int i = 0, j = 0; i < modelDevice->columnCount(); i++, j++){
        modelDevice->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
}

void MainWindow::createUI()
{
    ui->deviceTableView->setModel(modelDevice);     // Устанавливаем модель на TableView
    ui->deviceTableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    // Разрешаем выделение строк
    ui->deviceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->deviceTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->deviceTableView->resizeColumnsToContents();
    //ui->deviceTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//отключение режима
    //редактирования строк
    ui->deviceTableView->horizontalHeader()->setStretchLastSection(true);

    connect(ui->deviceTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotEditRecord(QModelIndex)));
    connect(ui->DeleteDeviceButton,SIGNAL(clicked()), this, SLOT(slotDeleteRecord()));
}

/* Метод для активации диалога добавления записей
 * ------------------------------------------------------------------------------------------------------
 * */
void MainWindow::on_addDeviceButton_clicked()
{
    /* Создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных
     * */
    DialogAddDevice *addDeviceDialog = new DialogAddDevice();
    connect(addDeviceDialog, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Выполняем запуск диалогового окна
     * */
    addDeviceDialog->setWindowTitle(tr("Добавить Устройство"));
    addDeviceDialog->exec();
}

/* Слот обновления модели представления данных
 * ------------------------------------------------------------------------------------------------------
 * */
void MainWindow::slotUpdateModels()
{

    modelDevice->select();

}


/* Слот для удаления записи
 * -------------------------------------------------------------------------------------------------
 * */

void MainWindow::slotDeleteRecord()
{
    QMessageBox::StandardButton reply;
         reply = QMessageBox::question(this, "Delete", "Do you really want to delete?",
                                       QMessageBox::Yes|QMessageBox::No);
         if (reply == QMessageBox::Yes) {
           qDebug() << "Yes was clicked";
    modelDevice->removeRows(ui->deviceTableView->currentIndex().row(), 1);

               ui->deviceTableView->update();
                modelDevice->submitAll();
                modelDevice->select();
}
         else {
            qDebug() << "Yes was *not* clicked";

       }
   }


/* Слот для активации диалога добавления записей в режиме редактирования
 * с передачей индекса выбранной строки
 * -------------------------------------------------------------------------------------------------
 * */
void MainWindow::slotEditRecord(QModelIndex index)
{
    /* Также создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных, но передаём
     * в качестве параметров строку записи
     * */
    DialogAddDevice *addDeviceDialog = new DialogAddDevice(index.row());
    connect(addDeviceDialog, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));
    connect(addDeviceDialog, SIGNAL(signalDeleteRecord()), this, SLOT(slotDeleteRecord()));

    /* Выполняем запуск диалогового окна
     * */
    addDeviceDialog->setWindowTitle(tr("Редактировать Устройство"));
    addDeviceDialog->exec();
}
