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

    // load & init data

    for (int a = 0; a < ui->tags_list->count(); ++a)
        ui->tags_list->item(a)->setCheckState(Qt::Unchecked);

    for (int a = 0; a < ui->files_list->count(); ++a)
        ui->files_list->item(a)->setCheckState(Qt::Unchecked);
}

// ================================================================
// | Main_Window Class - Private                                  |
// ================================================================

bool Main_Window::legal_characters_in_tag_name(const QString& tag_name)
{
    auto legal_character = [](const QChar& character)
    {
        return (character >= '0' && character <= '9') ||
               (character >= 'a' && character <= 'z') ||
               (character >= 'A' && character <= 'Z') ||
               (character == '-') ||
               (character == '_');
    };

    for (const auto& character : tag_name)
        if (!legal_character(character))
            return false;

    return true;
}

bool Main_Window::verify_tag_name(const QString& tag_name)
{
    if (!legal_characters_in_tag_name(tag_name))
    {
        QMessageBox::warning(this, "Unacceptable character", "At least one provided character is illegal. Acceptable characters: a-z A-Z 0-9 - _");
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

QString Main_Window::tag_to_ui_string(const QString& tag_name, int count)
{
    QString result = tag_name;
    result += " - ";
    result += QString::number(count);

    return result;
}

void Main_Window::refresh_ui_tags_list(bool checked_only)
{
    for (int a = 0; a < ui->tags_list->count(); ++a)
        if (!(checked_only && ui->tags_list->item(a)->checkState() != Qt::Checked))
        {
            QString tag_name = extract_tag_name(ui->tags_list->item(a)->text());
            ui->tags_list->item(a)->setText(tag_to_ui_string(tag_name, m_data.get_tags()[tag_name]));
        }
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

    ui->tags_list->addItem(tag_to_ui_string(tag_name, 0));
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

    auto& files = m_data.get_files();

    for (size_t a = 0; a < files.size(); ++a)
        if (files[a].get_tags().contains(old_name))
        {
            files[a].update_tag(old_name, new_name);
            ui->files_list->item(a)->setText(files[a].to_string());
        }
}

void Main_Window::on_delete_tag_button_clicked()
{
    if (!ui->tags_list->currentItem())
        return;

    QString tag_name = extract_tag_name(ui->tags_list->currentItem()->text());

    if (QMessageBox::question(this, "Confirm tag delete", "Are you sure you want to delete tag \"" + tag_name + "\"?") != QMessageBox::StandardButton::Yes)
        return;

    m_data.get_tags().remove(tag_name);

    delete ui->tags_list->takeItem(ui->tags_list->currentRow());

    auto& files = m_data.get_files();

    for (size_t a = 0; a < files.size(); ++a)
        if (files[a].get_tags().contains(tag_name))
        {
            files[a].remove_tag(tag_name);
            ui->files_list->item(a)->setText(files[a].to_string());
        }
}

void Main_Window::on_add_files_button_clicked()
{
    auto file_paths = QFileDialog::getOpenFileNames(this, "Select one or more files to add to your catalog");

    // call reset button because we want to un-hide all files

    for (const auto& file_path : file_paths)
    {
        m_data.get_files().emplace_back(file_path, m_data.get_tags());

        if (ui->add_files_tags_checkbox->isChecked())
            for (int a = 0; a < ui->tags_list->count(); ++a)
                if (ui->tags_list->item(a)->checkState() == Qt::Checked)
                    m_data.get_files().back().add_tag(extract_tag_name(ui->tags_list->item(a)->text()));

        ui->files_list->addItem(m_data.get_files().back().to_string());
        ui->files_list->item(ui->files_list->count() - 1)->setCheckState(Qt::Unchecked);
    }

    refresh_ui_tags_list(true);

    // debug
    for (const auto& file : m_data.get_files())
        qDebug() << file.to_string();
    qDebug() << "";
}

void Main_Window::on_add_tags_to_files_button_clicked()
{
    for (int a = 0; a < ui->files_list->count(); ++a)
        if (ui->files_list->item(a)->checkState() == Qt::Checked)
        {
            for (int b = 0; b < ui->tags_list->count(); ++b)
                if (!ui->files_list->item(a)->isHidden() && (ui->tags_list->item(b)->checkState() == Qt::Checked))
                    m_data.get_files()[a].add_tag(extract_tag_name(ui->tags_list->item(b)->text()));

            ui->files_list->item(a)->setText(m_data.get_files()[a].to_string());
        }
    qDebug() << "works";

    refresh_ui_tags_list(true);

    // debug
    for (const auto& file : m_data.get_files())
        qDebug() << file.to_string();
    qDebug() << "";
}

void Main_Window::on_remove_tags_from_files_button_clicked()
{
    for (int a = 0; a < ui->files_list->count(); ++a)
        if (ui->files_list->item(a)->checkState() == Qt::Checked)
        {
            for (int b = 0; b < ui->tags_list->count(); ++b)
                if (!ui->files_list->item(a)->isHidden() && (ui->tags_list->item(b)->checkState() == Qt::Checked))
                    m_data.get_files()[a].remove_tag(extract_tag_name(ui->tags_list->item(b)->text()));

            ui->files_list->item(a)->setText(m_data.get_files()[a].to_string());
        }

    refresh_ui_tags_list(true);

    // debug
    for (const auto& file : m_data.get_files())
        qDebug() << file.to_string();
    qDebug() << "";
}

void Main_Window::on_delete_file_button_clicked()
{
    if (!ui->files_list->currentItem() || ui->files_list->currentItem()->isHidden())
        return;

    if (QMessageBox::question(this, "Confirm file delete", "Are you sure you want to delete this file?") != QMessageBox::StandardButton::Yes)
        return;

    auto& files = m_data.get_files();
    auto& tags = m_data.get_tags();
    int row = ui->files_list->currentRow();

    auto iter = files.begin();
    iter += row;

    for (const auto& tag_name : iter->get_tags())
        --tags[tag_name];

    files.erase(iter);

    delete ui->files_list->takeItem(row);

    refresh_ui_tags_list(false);

    // debug
    for (const auto& file : files)
        qDebug() << file.to_string();
    qDebug() << "";
}

void Main_Window::on_search_button_clicked()
{
    // debug
    ui->files_list->item(2)->setHidden(true);
    ui->files_list->item(4)->setHidden(true);
    ui->files_list->item(6)->setHidden(true);
}

void Main_Window::on_search_reset_button_clicked()
{
    ui->search_line->clear(); // not sure if we want this

    for (int a = 0; a < ui->files_list->count(); ++a)
        ui->files_list->item(a)->setHidden(false);
}

// ================================================================
// | Main_Window Class - Destructor                               |
// ================================================================

Main_Window::~Main_Window()
{
    delete ui;
}
