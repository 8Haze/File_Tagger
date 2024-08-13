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
}

bool Main_Window::legal_characters_in_tag_name(const QString& tag_name)
{
    auto legal_char = [](const QChar& character)
    {
        return (character >= '0' && character <= '9') ||
               (character >= 'a' && character <= 'z') ||
               (character >= 'A' && character <= 'Z') ||
               (character == '-') ||
               (character == '_');
    };

    for (const auto& character : tag_name)
        if (!legal_char(character))
            return false;

    return true;
}

bool Main_Window::verify_tag_name(const QString& tag_name)
{
    if (!legal_characters_in_tag_name(tag_name))
    {
        QMessageBox::warning(this, "Unacceptable character", "At least one provided character is illegal. Acceptable characters: a-z, A-Z, 0-9, -, _.");
        return false;
    }

    if (m_data.get_tags().contains(tag_name))
    {
        QMessageBox::information(this, "Tag already exists", "The tag name you requested already exists.");
        return false;
    }

    return true;
}

QString Main_Window::extract_tag_name(QString string)
{
    while (string.back() != ' ')
        string.chop(1);
    string.chop(3);

    return string;
}

// ================================================================
// | Main_Window Class - Private Slots                            |
// ================================================================

void Main_Window::on_create_tag_button_clicked()
{
    QString tag_name = QInputDialog::getText(this, "Create new tag", "Enter the name of the tag:");

    if (tag_name.isEmpty() || !verify_tag_name(tag_name))
        return;

    m_data.get_tags().insert(tag_name, 0);

    ui->tags_list->addItem(tag_name + " - 0");
    ui->tags_list->item(ui->tags_list->count() - 1)->setCheckState(Qt::Unchecked);
}

void Main_Window::on_edit_tag_button_clicked()
{
    if (!ui->tags_list->currentItem())
        return;

    QString old_name = extract_tag_name(ui->tags_list->currentItem()->text());
    int count = m_data.get_tags()[old_name];
    QString new_name = QInputDialog::getText(this, "Edit tag", "Enter the new name for \"" + old_name + "\":");

    if (new_name.isEmpty() || !verify_tag_name(new_name))
        return;

    m_data.get_tags().remove(old_name);
    m_data.get_tags().insert(new_name, count);

    ui->tags_list->currentItem()->setText(new_name + " - " + QString::number(count));
}

void Main_Window::on_remove_tag_button_clicked()
{
    if (!ui->tags_list->currentItem())
        return;

    QString tag_name = extract_tag_name(ui->tags_list->currentItem()->text());

    if (QMessageBox::question(this, "Confirm tag delete", "Are you sure you want to delete tag \"" + tag_name + "\"?") != QMessageBox::StandardButton::Yes)
        return;

    m_data.get_tags().remove(tag_name);

    QListWidgetItem* item = ui->tags_list->takeItem(ui->tags_list->currentRow());
    delete item;
}

// ================================================================
// | Main_Window Class - Destructor                               |
// ================================================================

Main_Window::~Main_Window()
{
    delete ui;
}

/*  for (int a = 0; a < ui->tags_list->count(); ++a)
        if (ui->tags_list->item(a)->checkState() == Qt::Checked)
            qDebug() << "Checked tag: " << ui->tags_list->item(a)->text();  */
