#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionMusic_triggered();

    void on_btn_Player_clicked();

    void on_actionExit_triggered();

    void on_pushButton_clicked();

    void on_btn_Former_clicked();

    void on_btn_Next_clicked();

    void setPlaySate(void);
    void on_hs_rate_sliderMoved(int position);

    void on_actionDir_triggered();

    void on_lstWgt_List_doubleClicked(const QModelIndex &index);

    void on_lstWgt_List_customContextMenuRequested(const QPoint &pos);

    void on_action_Save_triggered();

    void on_btn_volume_clicked();

    void set_volum(QSlider*);
private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;//播放器
    QMediaPlaylist *playlist;//播放列表
    QString curTime;
    QString curPosition;
    void setColor(void);
    void saveMusicData(void);
    void setToolButton(void);

};

#endif // MAINWINDOW_H
