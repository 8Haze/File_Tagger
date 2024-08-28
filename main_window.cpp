#include "main_window.h"
#include "ui_main_window.h"

#include <QUrl>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QDesktopServices>

// ================================================================
// | Main_Window Class - Constructors                             |
// ================================================================

Main_Window::Main_Window(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Main_Window)
{
    ui->setupUi(this);

    setWindowTitle("File Tagger");

    init_connections();
    init_data();
}

// ================================================================
// | Main_Window Class - Private                                  |
// ================================================================

void Main_Window::init_connections()
{
    connect(ui->create_tag_button, &QPushButton::clicked, this, &Main_Window::do_on_create_tag_button_clicked);
    connect(ui->edit_tag_button, &QPushButton::clicked, this, &Main_Window::do_on_edit_tag_button_clicked);
    connect(ui->delete_tag_button, &QPushButton::clicked, this, &Main_Window::do_on_delete_tag_button_clicked);
    connect(ui->check_all_tags_button, &QPushButton::clicked, this, &Main_Window::do_on_check_all_tags_button);
    connect(ui->uncheck_all_tags_button, &QPushButton::clicked, this, &Main_Window::do_on_uncheck_all_tags_button);

    connect(ui->add_files_button, &QPushButton::clicked, this, &Main_Window::do_on_add_files_button_clicked);
    connect(ui->add_tags_to_files_button, &QPushButton::clicked, this, &Main_Window::do_on_add_tags_to_files_button_clicked);
    connect(ui->remove_tags_from_files_button, &QPushButton::clicked, this, &Main_Window::do_on_remove_tags_from_files_button_clicked);
    connect(ui->delete_file_button, &QPushButton::clicked, this, &Main_Window::do_on_delete_file_button_clicked);
    connect(ui->check_all_files_button, &QPushButton::clicked, this, &Main_Window::do_on_check_all_files_button);
    connect(ui->uncheck_all_files_button, &QPushButton::clicked, this, &Main_Window::do_on_uncheck_all_files_button);

    connect(ui->search_button, &QPushButton::clicked, this, &Main_Window::do_on_search_button_clicked);
    connect(ui->search_reset_button, &QPushButton::clicked, this, &Main_Window::do_on_search_reset_button_clicked);

    connect(ui->files_list, &QListWidget::itemSelectionChanged, this, &Main_Window::do_on_files_list_item_selection_changed);
    connect(ui->files_list, &QListWidget::doubleClicked, this, &Main_Window::do_on_files_list_double_clicked);

    connect(ui->tags_list, &QListWidget::itemChanged, this, &Main_Window::do_on_list_item_changed);
    connect(ui->files_list, &QListWidget::itemChanged, this, &Main_Window::do_on_list_item_changed);

    connect(ui->action_save, &QAction::triggered, this, &Main_Window::do_on_action_save_triggered);
    connect(ui->action_exit, &QAction::triggered, this, &Main_Window::do_on_action_exit_triggered);
}

void Main_Window::init_data()
{
    if (!m_main_serializer.load(m_data, sm_save_file_name))
        QMessageBox::critical(this, "Error", "Failed to load data. The destination file could not be opened.");

    for (auto [tag_name, count] : m_data.get_tags().asKeyValueRange())
        ui->tags_list->addItem(tag_to_ui_string(tag_name, count));

    for (const auto& file : m_data.get_files())
        ui->files_list->addItem(file.to_string());

    for (int a = 0; a < ui->tags_list->count(); ++a)
        ui->tags_list->item(a)->setCheckState(Qt::Unchecked);

    for (int a = 0; a < ui->files_list->count(); ++a)
        ui->files_list->item(a)->setCheckState(Qt::Unchecked);

    m_data.mark_as_saved();
}

void Main_Window::save_data()
{
    if (!m_main_serializer.save(m_data, sm_save_file_name))
        QMessageBox::critical(this, "Error", "Failed to save data. The destination file could not be opened.");

    m_data.mark_as_saved();
}

bool Main_Window::legal_characters_in_tag_name(const QString& tag_name)
{
    auto legal_character = [](const QChar& character)
    {
        return (character >= '0' && character <= '9') ||
               (character >= 'a' && character <= 'z') ||
               (character >= 'A' && character <= 'Z') ||
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
        QMessageBox::warning(this, "Unacceptable character", "At least one provided character is illegal. Acceptable characters: a-z A-Z 0-9 _");
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

void Main_Window::rewrite_ui_tags_list()
{
    ui->tags_list->clear();

    for (const auto& [tag_name, count] : m_data.get_tags().asKeyValueRange())
    {
        ui->tags_list->addItem(tag_to_ui_string(tag_name, count));
        ui->tags_list->item(ui->tags_list->count() - 1)->setCheckState(Qt::Unchecked);
    }
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

void Main_Window::reset_search_results()
{
    searcher.reset();

    for (int a = 0; a < ui->files_list->count(); ++a)
        ui->files_list->item(a)->setHidden(false);
}

// ================================================================
// | Main_Window Class - Private Slots                            |
// ================================================================

void Main_Window::do_on_create_tag_button_clicked()
{
    QString tag_name = QInputDialog::getText(this, "Create new tag", "Enter the name of the tag:");

    if (tag_name.isEmpty() || !verify_tag_name(tag_name))
        return;

    m_data.get_tags().insert(tag_name, 0);

    rewrite_ui_tags_list();
}

void Main_Window::do_on_edit_tag_button_clicked()
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

    rewrite_ui_tags_list();

    auto& files = m_data.get_files();

    for (size_t a = 0; a < files.size(); ++a)
        if (files[a].get_tags().contains(old_name))
        {
            files[a].update_tag(old_name, new_name);
            ui->files_list->item(a)->setText(files[a].to_string());
        }
}

void Main_Window::do_on_delete_tag_button_clicked()
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

void Main_Window::do_on_check_all_tags_button()
{
    for (int a = 0; a < ui->tags_list->count(); ++a)
        if (!ui->tags_list->item(a)->isHidden())
            ui->tags_list->item(a)->setCheckState(Qt::Checked);
}

void Main_Window::do_on_uncheck_all_tags_button()
{
    for (int a = 0; a < ui->tags_list->count(); ++a)
        if (!ui->tags_list->item(a)->isHidden())
            ui->tags_list->item(a)->setCheckState(Qt::Unchecked);
}

void Main_Window::do_on_add_files_button_clicked()
{
    auto file_paths = QFileDialog::getOpenFileNames(this, "Select one or more files to add to your catalog");

    reset_search_results();

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
}

void Main_Window::do_on_delete_file_button_clicked()
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

    do_on_files_list_item_selection_changed();
}

void Main_Window::do_on_add_tags_to_files_button_clicked()
{
    for (int a = 0; a < ui->files_list->count(); ++a)
        if (ui->files_list->item(a)->checkState() == Qt::Checked)
        {
            for (int b = 0; b < ui->tags_list->count(); ++b)
                if (!ui->files_list->item(a)->isHidden() && (ui->tags_list->item(b)->checkState() == Qt::Checked))
                    m_data.get_files()[a].add_tag(extract_tag_name(ui->tags_list->item(b)->text()));

            ui->files_list->item(a)->setText(m_data.get_files()[a].to_string());
        }

    refresh_ui_tags_list(true);
}

void Main_Window::do_on_remove_tags_from_files_button_clicked()
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
}

void Main_Window::do_on_check_all_files_button()
{
    for (int a = 0; a < ui->files_list->count(); ++a)
        if (!ui->files_list->item(a)->isHidden())
            ui->files_list->item(a)->setCheckState(Qt::Checked);
}

void Main_Window::do_on_uncheck_all_files_button()
{
    for (int a = 0; a < ui->files_list->count(); ++a)
        if (!ui->files_list->item(a)->isHidden())
            ui->files_list->item(a)->setCheckState(Qt::Unchecked);
}

void Main_Window::do_on_search_button_clicked()
{
    reset_search_results();

    if (!searcher.parse(ui->search_line->text()))
    {
        QMessageBox::warning(this, "Parsing Error", "The search line input contains an error. Check it and try again.");
        return;
    }

    auto to_hide = searcher.search(m_data.get_files());

    for (const auto& index : to_hide)
        ui->files_list->item(index)->setHidden(true);
}

void Main_Window::do_on_search_reset_button_clicked()
{
    ui->search_line->clear();

    reset_search_results();
}

void Main_Window::do_on_files_list_item_selection_changed()
{
    if (!ui->files_list->currentItem() || ui->files_list->currentItem()->isHidden())
    {
        ui->thumbnail_label->setText("No Thumbnail Available");
        return;
    }

    auto& files = m_data.get_files();
    int row = ui->files_list->currentRow();

    files[row].generate_thumbnail(ui->thumbnail_label->size());

    if (files[row].get_thumbnail())
    {
        QPixmap pixmap = QPixmap::fromImage(*files[row].get_thumbnail());
        ui->thumbnail_label->setPixmap(pixmap);
    }
    else
        ui->thumbnail_label->setText("No Thumbnail Available");
}

void Main_Window::do_on_files_list_double_clicked(const QModelIndex& index)
{
    if (index.row() < 0 || index.row() >= ui->files_list->count() || ui->files_list->item(index.row())->isHidden())
        return;

    if (!QDesktopServices::openUrl(QUrl("file:///" + m_data.get_files()[index.row()].get_path(), QUrl::TolerantMode)))
        QMessageBox::critical(this, "File could not be opened", "The file could not be opened. If it does not exist anymore, it should be removed from the program.");
}

void Main_Window::do_on_list_item_changed(QListWidgetItem* item)
{
    if (item)
    {
        QFont font = item->font();
        font.setBold(item->checkState() == Qt::Checked);
        item->setFont(font);
    }
}

void Main_Window::do_on_action_save_triggered()
{
    save_data();
}

void Main_Window::do_on_action_exit_triggered()
{
    auto yes = QMessageBox::Yes;
    auto no = QMessageBox::No;
    auto cancel = QMessageBox::Cancel;

    if (!m_data.is_saved())
    {
        auto answer = QMessageBox::question(this, "Unsaved data", "You have unsaved data. Save before exiting?", QMessageBox::StandardButtons(yes | no | cancel));

        if (answer == QMessageBox::StandardButton::Yes)
        {
            save_data();
            QMainWindow::close();
        }
        else if (answer == QMessageBox::StandardButton::No)
            QMainWindow::close();
    }
    else
        QMainWindow::close();
}

// ================================================================
// | Main_Window Class - Protected                                |
// ================================================================

void Main_Window::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return)
    {
        if (ui->files_list->hasFocus())
            do_on_files_list_double_clicked(ui->files_list->currentIndex());
        else if (ui->search_line->hasFocus())
            do_on_search_button_clicked();
    }

    QMainWindow::keyPressEvent(event);
}

void Main_Window::closeEvent(QCloseEvent* event)
{
    if (m_data.is_saved())
        event->accept();
    else
    {
        do_on_action_exit_triggered();
        event->ignore();
    }
}

// ================================================================
// | Main_Window Class - Destructor                               |
// ================================================================

Main_Window::~Main_Window()
{
    delete ui;
}
