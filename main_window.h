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

    bool legal_characters_in_tag_name(const QString& tag_name);
    bool verify_tag_name(const QString& tag_name);
    QString extract_tag_name(QString string);

private slots:

    void on_create_tag_button_clicked();
    void on_edit_tag_button_clicked();
    void on_remove_tag_button_clicked();

public:

    Main_Window(QWidget* parent = nullptr);

    ~Main_Window();

signals:



};

#endif // MAIN_WINDOW_H
