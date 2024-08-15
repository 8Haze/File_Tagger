#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

#include "data.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Main_Window; }
QT_END_NAMESPACE

class Main_Window : public QMainWindow
{
private:

    Q_OBJECT

    Ui::Main_Window* ui;

    Data m_data;

    bool legal_characters_in_tag_name(const QString& tag_name);
    bool verify_tag_name(const QString& tag_name);
    QString extract_tag_name(QString string);
    QString tag_to_ui_string(const QString& tag_name, int count);

    void refresh_ui_tags_list(bool checked_only = false);

private slots:

    void on_create_tag_button_clicked();
    void on_edit_tag_button_clicked();
    void on_delete_tag_button_clicked();

    void on_add_files_button_clicked();
    void on_add_tags_to_files_button_clicked();
    void on_remove_tags_from_files_button_clicked();
    void on_delete_file_button_clicked();

    void on_search_button_clicked();
    void on_search_reset_button_clicked();

    // double-click works

public:

    Main_Window(QWidget* parent = nullptr);

    ~Main_Window();

signals:

    void signal_data_modified();

};

#endif // MAIN_WINDOW_H
