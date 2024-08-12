#include "main_window.h"
#include "ui_main_window.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>

#include <QDebug> // debug only

// ================================================================
// | Main_Window Class - Constructors                             |
// ================================================================

Main_Window::Main_Window(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Main_Window)
{
    ui->setupUi(this);

    setWindowTitle("File Tagger");

    for (int a = 0; a < ui->tags_list->count(); ++a)
        ui->tags_list->item(a)->setCheckState(Qt::Unchecked);

    for (int a = 0; a < ui->files_list->count(); ++a)
        ui->files_list->item(a)->setCheckState(Qt::Unchecked);

    if (ui->tags_list->count() > 0) ui->tags_list->setCurrentRow(0);
    if (ui->files_list->count() > 0) ui->files_list->setCurrentRow(0);

    connect(this, &Main_Window::signal_tags_data_modified, this, &Main_Window::display_tags_list);
    connect(this, &Main_Window::signal_files_data_modified, this, &Main_Window::display_files_list);
}

// ================================================================
// | Main_Window Class - Private Slots                            |
// ================================================================

void Main_Window::on_create_tag_button_clicked()
{
    auto tag_name = QInputDialog::getText(this, "Create a new tag", "Enter the name of the tag:");
    bool available = true;

    // todo: one more check - legal characters (only a-z, A-Z, 0-9, -, _)

    for (auto [key, value] : m_data.get_tags().asKeyValueRange())
        if (key == tag_name)
        {
            available = false;
            break;
        }

    if (!available)
    {
        QMessageBox::information(this, "Tag already exists", "The tag name you requested already exists.");
        return;
    }

    m_data.get_tags().insert(tag_name, 0);

    emit signal_tags_data_modified();
}

void Main_Window::on_edit_tag_button_clicked()
{


    emit signal_tags_data_modified();
}

void Main_Window::on_remove_tag_button_clicked()
{


    emit signal_tags_data_modified();
}

void Main_Window::display_tags_list()
{
    ui->tags_list->clear();

    for (auto [key, value] : m_data.get_tags().asKeyValueRange())
    {
        QString string = key + " - ";
        string += std::to_string(value);
        ui->tags_list->addItem(string);
    }

    for (int a = 0; a < ui->tags_list->count(); ++a)
        ui->tags_list->item(a)->setCheckState(Qt::Unchecked);
}

void Main_Window::display_files_list()
{

}

// ================================================================
// | Main_Window Class - Destructor                               |
// ================================================================

Main_Window::~Main_Window()
{
    delete ui;
}

/*
{
    for (int a = 0; a < ui->tags_list->count(); ++a)
        if (ui->tags_list->item(a)->checkState() == Qt::Checked)
            qDebug() << "Checked tag: " << ui->tags_list->item(a)->text();
}

{
    qDebug() << "Selected file: " << ui->files_list->selectedItems().at(0)->text();
}*/

