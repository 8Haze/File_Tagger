#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QCloseEvent>

#include "data.h"
#include "binary_serializer.h"
#include "searcher.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Main_Window; }
QT_END_NAMESPACE

class Main_Window : public QMainWindow
{
private:

    Q_OBJECT

    inline static const QString sm_save_file_name = "data.bin";

    Ui::Main_Window* ui;

    Data m_data;
    Searcher searcher;
    Binary_Serializer m_main_serializer;

    void init_connections();

    void init_data();
    void save_data();

    bool legal_characters_in_tag_name(const QString& tag_name);
    bool verify_tag_name(const QString& tag_name);

    QString extract_tag_name(QString string);
    QString tag_to_ui_string(const QString& tag_name, int count);

    void refresh_ui_tags_list(bool checked_only = false);

    void reset_search_results();

private slots:

    // Avoiding Qt's connect-by-name mechanism on purpose

    void do_on_create_tag_button_clicked();
    void do_on_edit_tag_button_clicked();
    void do_on_delete_tag_button_clicked();
    void do_on_check_all_tags_button();
    void do_on_uncheck_all_tags_button();

    void do_on_add_files_button_clicked();
    void do_on_delete_file_button_clicked();
    void do_on_add_tags_to_files_button_clicked();
    void do_on_remove_tags_from_files_button_clicked();
    void do_on_check_all_files_button();
    void do_on_uncheck_all_files_button();

    void do_on_search_button_clicked();
    void do_on_search_reset_button_clicked();

    void do_on_files_list_double_clicked(const QModelIndex& index);

    void do_on_action_save_triggered();
    void do_on_action_exit_triggered();

protected:

    void closeEvent(QCloseEvent*) override;

public:

    Main_Window(QWidget* parent = nullptr);

    ~Main_Window();

};

#endif // MAIN_WINDOW_H
