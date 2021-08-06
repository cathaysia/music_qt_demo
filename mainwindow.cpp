#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QStringList>
#include <QMessageBox>
#include <QSlider>
#include <QListWidget>
#include <QUrl>
/*已知BUG:
 * 1.某些mp3文件无法播放（5min以上？）
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player =new QMediaPlayer(this);
    playlist =new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);
    setColor();
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(setPlaySate()));//显示播放进度
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionMusic_triggered()//添加音乐（单项或多项）
{
    int num=playlist->currentIndex();
    QStringList filelist=QFileDialog::getOpenFileNames(this,"选择音乐文件",QDir::currentPath(),
                                                       "音频文件(*.mp3 *.wav *.wma)");
    if(filelist.count()<1)
        return;
    for(int i=0;i<filelist.count();i++)
    {
        QString aFile=filelist.at(i);
        playlist->addMedia(QUrl::fromLocalFile(aFile));
        QFileInfo fileInfo(aFile);
        ui->lstWgt_List->addItem(fileInfo.fileName());
    }
    if(player->state()!=QMediaPlayer::PlayingState)//若未播放音乐，将焦点移到刚添加的文件上
    {
        playlist->setCurrentIndex(num+1);
        setPlaySate();
    }
}

void MainWindow::on_btn_Player_clicked()//播放音乐
{
    if(!playlist->mediaCount())
        QMessageBox::information(this,"注意","请先添加文件");
    else
    {
        player->play();
        ui->btn_Player->setEnabled(false);
        setPlaySate();//设置当前播放音乐名
    }
}

void MainWindow::on_actionExit_triggered()//退出按钮
{
    close();
}

void MainWindow::setPlaySate(void)//设置播放标签和播放列表焦点
{
    ui->lstWgt_List->setCurrentRow(playlist->currentIndex());
    ui->lab_State->setText(ui->lstWgt_List->currentItem()->text());
    ui->hs_rate->setMaximum(player->duration());
    ui->hs_rate->setValue(player->position());
    int secs=player->duration()/1000;//若将int改为qint64会导致不显秒数
    int mins=secs/60;
    secs%=60;
    int duration=player->position();
    QString durStr=QString::asprintf("%d:%d",duration/60000,duration/1000%60);
    ui->lbl_Time->setText(durStr+"/"+QString::asprintf("%d:%d",mins,secs));
    saveMusicData();
}

void MainWindow::on_pushButton_clicked()//暂停按钮
{
    player->pause();
    ui->btn_Player->setEnabled(true);
}

void MainWindow::on_btn_Former_clicked()//上一首
{
    if(player->state()!=QMediaPlayer::PlayingState)
        return;
    playlist->previous();
    setPlaySate();

}

void MainWindow::on_btn_Next_clicked()//下一首
{
    if(player->state()!=QMediaPlayer::PlayingState)
        return;
    playlist->next();
    setPlaySate();
}

void MainWindow::on_hs_rate_sliderMoved(int position)//调整播放进度
{
    if(player->state()!=QMediaPlayer::PlayingState)
        return;
    player->setPosition(position);
}

void MainWindow::on_actionDir_triggered()//添加目录
{
    int num=playlist->currentIndex();
    QString get_Path=QFileDialog::getExistingDirectory(this,"选择目录","/");
    if(get_Path.isEmpty())
    {
        return;
    }
    QFileInfoList infoList=QDir(get_Path).entryInfoList();//获取当前目录下所有文件
    for(int i=0;i<infoList.size();i++)
    {
        QString suffix=infoList[i].suffix();
        if(suffix.compare("mp3")==0||suffix.compare("wma")==0||suffix.compare("wav")==0)
        {
            playlist->addMedia(QUrl::fromLocalFile(infoList[i].filePath()));
            ui->lstWgt_List->addItem(infoList[i].fileName());
        }
    }
    if(player->state()!=QMediaPlayer::PlayingState)//若未播放音乐，将焦点移到刚添加的文件上
    {
        playlist->setCurrentIndex(num+1);
        setPlaySate();
    }

}

void MainWindow::on_lstWgt_List_doubleClicked(const QModelIndex &index)//双击播放
{
    playlist->setCurrentIndex(index.row());
    setPlaySate();
    if(player->state()!=QMediaPlayer::PlayingState)
        player->play();
}

void MainWindow::on_lstWgt_List_customContextMenuRequested(const QPoint &pos)//列表右键菜单
{
    QMenu* popMenu=new QMenu(this);
    QAction* addMusic=new QAction("添加音乐",this);
    QAction* addFolder=new QAction("添加音乐文件夹",this);
    popMenu->addAction(addMusic);
    popMenu->addAction(addFolder);
    /*if(ui->lstWgt_List->itemAt(mapFromGlobal(QCursor::pos()))!=nullptr)//若在item上右键(该功能尚未实现
    {
        popMenu->addAction(new QAction("从列表中删除",this));
        popMenu->addAction(new QAction("上移一位",this));
        popMenu->addAction(new QAction("下移一位",this));
    }*/
    connect(addMusic,SIGNAL(triggered()),this,SLOT(on_actionMusic_triggered()));
    connect(addFolder,SIGNAL(triggered()),this,SLOT(on_actionDir_triggered()));

    popMenu->exec(QCursor::pos());//菜单出现在鼠标位置

    delete popMenu;
    delete addMusic;
    delete addFolder;
}

void MainWindow::setColor()//修改窗口外观
{
}

void MainWindow::saveMusicData()//报存音乐列表
{

}

void MainWindow::setToolButton()//设置toolButton样式
{

}

void MainWindow::on_action_Save_triggered()
{
    QFile musicList("D:\\qtMusic.m3u");
    if(!musicList.open(QIODevice::WriteOnly))
       {
        QMessageBox::information(this,"警告","文件打开失败");
        return;
        }

    if(!playlist->save(&musicList,"m3u"))
        QMessageBox::information(this,"","文件保存失败");
    else
        QMessageBox::information(this,"","保存成功");
    musicList.close();
}

void MainWindow::on_btn_volume_clicked()
{
    QSlider* slider=new QSlider(this);
    slider->setMaximum(100);
    slider->setMinimum(0);
    slider->setValue(player->volume());
    connect(slider,SIGNAL(sliderMoved()),this,SLOT(set_volum(slider)));
    delete slider;
}

void MainWindow::set_volum(QSlider* slider)
{
    player->setVolume(slider->value());
}
