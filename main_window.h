#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

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

private slots:

    void on_create_tag_button_clicked();
    void on_edit_tag_button_clicked();
    void on_remove_tag_button_clicked();

    void display_tags_list();
    void display_files_list();

public:

    Main_Window(QWidget* parent = nullptr);

    ~Main_Window();

signals:

    void signal_tags_data_modified();
    void signal_files_data_modified();

};

#endif // MAIN_WINDOW_H
