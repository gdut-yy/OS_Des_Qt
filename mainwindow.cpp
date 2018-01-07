/**
* @brief    操作系统课程设计_磁盘调度算法
* @details  要求：
*           设计主界面以灵活选择某算法，且以下算法都要实现
*           1、先来先服务算法（FCFS）
*           2、最短寻道时间优先算法（SSTF）
*           3、扫描算法（SCAN）
*           4、循环扫描算法（CSCAN）
*           并求出每种算法的平均寻道长度。
* @author   3115005124_张逸扬
* @date     2018年1月7日18:35:28
*/
#include "mainwindow.h"

#include <QGridLayout>
#include <QLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("3115005124_张逸扬_操作系统课程设计_磁盘调度算法"));   //设置标题

    createDiskPage();

    setCentralWidget(diskPage);
}

MainWindow::~MainWindow(){
}

//页面布局 实现
void MainWindow::createDiskPage(){
    diskPage = new QWidget;
    //tableGroupBox初始化及布局
    tableGroupBox = new QGroupBox(tr("表格"));
    diskTable = new QTableWidget(9, 2);

    QLayout *tableLayout = new QVBoxLayout;
    tableLayout->addWidget(diskTable);
    tableGroupBox->setLayout(tableLayout);
    QStringList tableLabels;
    tableLabels << "被访问的\n下一个磁道号" << "移动距离\n（磁道数）";
    diskTable->setHorizontalHeaderLabels(tableLabels);
    diskTable->verticalHeader()->hide();   //隐藏垂直标题
    diskTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    diskTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    //algorithmGroupBox初始化及布局
    algorithmGroupBox = new QGroupBox(tr("算法"));
    FCFSrbutton = new QRadioButton(tr("先来先服务算法（FCFS）"));        //先来先服务算法
    SSTFrbutton = new QRadioButton(tr("最短寻道时间优先算法（SSTF）"));     //最短寻道时间优先算法
    SCANrbutton = new QRadioButton(tr("扫描算法（SCAN）"));               //扫描算法
    CSCANrbutton = new QRadioButton(tr("循环扫描算法（CSCAN）"));       //循环扫描算法
        connect(FCFSrbutton, SIGNAL(clicked(bool)), this, SLOT(setorder_off()));
        connect(SSTFrbutton, SIGNAL(clicked(bool)), this, SLOT(setorder_off()));
        connect(SCANrbutton, SIGNAL(clicked(bool)), this, SLOT(setorder_on()));
        connect(CSCANrbutton, SIGNAL(clicked(bool)), this, SLOT(setorder_on()));

    orderGroupBox = new QGroupBox(tr("请选择磁头移动方向"));
    increaserbutton = new QRadioButton(tr("向磁道号增加方向访问"));
    decreaserbutton = new QRadioButton(tr("向磁道号减少方向访问"));
        increaserbutton->setChecked(true);
    QVBoxLayout *orderLayout = new QVBoxLayout;
    orderLayout->addWidget(increaserbutton);
    orderLayout->addWidget(decreaserbutton);
    orderGroupBox->setLayout(orderLayout);
    orderGroupBox->setEnabled(false);

    QVBoxLayout *suanfaLayout = new QVBoxLayout;
    suanfaLayout->addWidget(FCFSrbutton);
    suanfaLayout->addWidget(SSTFrbutton);
    suanfaLayout->addWidget(SCANrbutton);
    suanfaLayout->addWidget(CSCANrbutton);
    suanfaLayout->addWidget(orderGroupBox);
    algorithmGroupBox->setLayout(suanfaLayout);


    //operationGroupBox初始化及布局
    operationGroupBox = new QGroupBox(tr("操作"));
    disknumLabel = new QLabel(tr("当前磁道号："));
    numberEdit = new QLineEdit;      //当前磁道号
    runButton = new QPushButton(tr("运行"));      //运行按钮
        runButton->setEnabled(false);
        connect(runButton, SIGNAL(clicked(bool)), this, SLOT(Runclick()));
    InitButton = new QPushButton(tr("初始化"));;    //初始化按钮
        connect(InitButton, SIGNAL(clicked(bool)), this, SLOT(Initclick()));

    QGridLayout *caozuoLayout = new QGridLayout;
    caozuoLayout->setColumnStretch(0, 1);
    caozuoLayout->setColumnStretch(1, 1);

    caozuoLayout->addWidget(disknumLabel, 0, 0);
    caozuoLayout->addWidget(numberEdit, 0, 1);
    caozuoLayout->addWidget(runButton, 1, 0);
    caozuoLayout->addWidget(InitButton, 1, 1);
    operationGroupBox->setLayout(caozuoLayout);


    //resultGroupBox初始化及布局
    resultGroupBox = new QGroupBox(tr("结果"));
    moveLabel = new QLabel(tr("移动磁道数："));              //移动磁道数
    move_averageLabel = new QLabel(tr("平均寻道长度："));     //平均寻道长度

    QVBoxLayout *resultLayout = new QVBoxLayout;
    resultLayout->addWidget(moveLabel);
    resultLayout->addWidget(move_averageLabel);
    resultGroupBox->setLayout(resultLayout);


    //页面布局
    QGridLayout *pageLayout = new QGridLayout;
    pageLayout->setMargin(0);
    pageLayout->setRowStretch(0, 2);
    pageLayout->setRowStretch(1, 1);
    pageLayout->setRowStretch(2, 1);
    pageLayout->addWidget(tableGroupBox, 0, 0, 3, 1);
    pageLayout->addWidget(algorithmGroupBox, 0, 1, 1, 1);
    pageLayout->addWidget(operationGroupBox, 1, 1, 1, 1);
    pageLayout->addWidget(resultGroupBox, 2, 1, 1, 1);
    diskPage->setLayout(pageLayout);
}

//初始化 按钮响应
void MainWindow::Initclick(){
    InitButton->setEnabled(false);
    runButton->setEnabled(true);

    moveLabel->setText(tr("移动磁道数："));
    move_averageLabel->setText(tr("平均寻道长度："));
    disk.clear();

    //预输入数据1
    diskTable->clearContents();
    diskTable->setItem(0, 0, new QTableWidgetItem(QString::number(55)));
    diskTable->setItem(1, 0, new QTableWidgetItem(QString::number(58)));
    diskTable->setItem(2, 0, new QTableWidgetItem(QString::number(39)));
    diskTable->setItem(3, 0, new QTableWidgetItem(QString::number(18)));
    diskTable->setItem(4, 0, new QTableWidgetItem(QString::number(90)));
    diskTable->setItem(5, 0, new QTableWidgetItem(QString::number(160)));
    diskTable->setItem(6, 0, new QTableWidgetItem(QString::number(150)));
    diskTable->setItem(7, 0, new QTableWidgetItem(QString::number(38)));
    diskTable->setItem(8, 0, new QTableWidgetItem(QString::number(184)));

    numberEdit->setText(QString::number(100));

    //预输入数据2
//    diskTable->clearContents();
//    diskTable->setItem(0, 0, new QTableWidgetItem(QString::number(86)));
//    diskTable->setItem(1, 0, new QTableWidgetItem(QString::number(147)));
//    diskTable->setItem(2, 0, new QTableWidgetItem(QString::number(91)));
//    diskTable->setItem(3, 0, new QTableWidgetItem(QString::number(177)));
//    diskTable->setItem(4, 0, new QTableWidgetItem(QString::number(94)));
//    diskTable->setItem(5, 0, new QTableWidgetItem(QString::number(150)));
//    diskTable->setItem(6, 0, new QTableWidgetItem(QString::number(102)));
//    diskTable->setItem(7, 0, new QTableWidgetItem(QString::number(175)));
//    diskTable->setItem(8, 0, new QTableWidgetItem(QString::number(130)));

//    numberEdit->setText(QString::number(143));
}

//运行按钮响应
void MainWindow::Runclick(){
    if(FCFSrbutton->isChecked()){
        //取界面信息
        runButton->setEnabled(false);
        getdiskTable();
        Now_number = numberEdit->text().toInt();    //获取当前磁头位置
        //调用算法
        FCFS();
        //界面显示结果
        setResult();
        setdiskTable();
        InitButton->setEnabled(true);
    }else if(SSTFrbutton->isChecked()){
        //取界面信息
        runButton->setEnabled(false);
        getdiskTable();
        Now_number = numberEdit->text().toInt();    //获取当前磁头位置
        //调用算法
        SSTF();
        //界面显示结果
        setResult();
        setdiskTable();
        InitButton->setEnabled(true);
    }else if(SCANrbutton->isChecked()){
        //取界面信息
        runButton->setEnabled(false);
        getdiskTable();
        Now_number = numberEdit->text().toInt();    //获取当前磁头位置
        //调用算法
        SCAN();
        //界面显示结果
        setResult();
        setdiskTable();
        InitButton->setEnabled(true);
    }else if(CSCANrbutton->isChecked()){
        //取界面信息
        runButton->setEnabled(false);
        getdiskTable();
        Now_number = numberEdit->text().toInt();    //获取当前磁头位置
        //调用算法
        CSCAN();
        //界面显示结果
        setResult();
        setdiskTable();
        InitButton->setEnabled(true);
    }else {
        QMessageBox::information(NULL, "提示", "请选择算法");
    }
}

//把 DISK容器数据 显示到 表格
void MainWindow::setdiskTable(){
    diskTable->clearContents();
    for(int i=0; i<disk.size(); i++){
        diskTable->setItem(i, 0, new QTableWidgetItem(QString::number(disk[i].Next_number)));
        if(disk[i].distance!=-1)
            diskTable->setItem(i, 1, new QTableWidgetItem(QString::number(disk[i].distance)));
    }
}

//把 表格数据 记录到 DISK容器
void MainWindow::getdiskTable(){
    for(int i=0; i<9; i++){
        disk.append(DISK{
                        diskTable->item(i, 0)->text().toInt(),
                        -1
                    });
    }
}

//FCFS算法
void MainWindow::FCFS(){
    //计算并记录 磁头 移动距离
    for(int i=0; i<disk.size(); i++){
        disk[i].distance = qAbs(Now_number - disk[i].Next_number);
        Now_number = disk[i].Next_number;
    }
}

//SSTF算法
void MainWindow::SSTF(){
    disk_sort = disk;
    DISK_AscStablesort(&disk_sort);
    if(Now_number<disk_sort.first().Next_number){
        //当 当前磁头位置 小于 磁道号升序序列最小值 升序排序
        DISK_AscStablesort(&disk);
    }else if(Now_number>disk_sort.last().Next_number){
        //当 当前磁头位置 大于 磁道号升序序列最大值 降序排序
        DISK_DescStablesort(&disk);
    }else {
        //求出 disk 下一个磁道号 队列
        for(int i=0; i<disk.size(); i++){
            for(int j=0; j<disk_sort.size(); j++){
                if(Now_number>=disk_sort[j].Next_number && Now_number<=disk_sort[j+1].Next_number){
                    if(qAbs(Now_number-disk_sort[j].Next_number)<=qAbs(Now_number-disk_sort[j+1].Next_number)){
                        disk[i].Next_number = disk_sort[j].Next_number;
                        Now_number = disk[i].Next_number;
                        disk_sort.remove(j);
                        break;
                    }else{
                        disk[i].Next_number = disk_sort[j+1].Next_number;
                        Now_number = disk[i].Next_number;
                        disk_sort.remove(j+1);
                        break;
                    }
                }else if(Now_number<disk_sort.first().Next_number){
                    //当 当前磁头位置 小于 余下 队列最小值
                    disk[i].Next_number = disk_sort.first().Next_number;
                    Now_number = disk[i].Next_number;
                    disk_sort.removeFirst();
                    break;
                }else if(Now_number>disk_sort.last().Next_number){
                    //当 当前磁头位置 大于 余下 队列最大值
                    disk[i].Next_number = disk_sort.last().Next_number;
                    Now_number = disk[i].Next_number;
                    disk_sort.removeLast();
                    break;
                }
            }
        }
    }

    //计算并记录 磁头 移动距离
    Now_number = numberEdit->text().toInt();
    for(int i=0; i<disk.size(); i++){
        disk[i].distance = qAbs(Now_number - disk[i].Next_number);
        Now_number = disk[i].Next_number;
    }
}

//SCAN算法
void MainWindow::SCAN(){
    if(increaserbutton->isChecked()){
        //当 单选按钮 为 向磁道号增加方向
        disk_sort = disk;
        DISK_AscStablesort(&disk_sort);

        if(Now_number<disk_sort.first().Next_number || Now_number>disk_sort.last().Next_number){
            //当 当前磁头位置 小于 磁道号升序序列最小值 或 大于 最大值时
            DISK_AscStablesort(&disk);
        } else {
            //remember 记录 有磁道号升序序列中 第一个大于  磁道号 位置
            int remember=0;
            for(int j=0; j<disk_sort.size()-1; j++){
                if(Now_number>=disk_sort[j].Next_number && Now_number<=disk_sort[j+1].Next_number){
                    remember=j;
                    break;
                }
            }
            //求出 disk 下一个磁道号 队列
            for(int i=0, r = remember; i<disk.size(); i++){
                if(r<disk_sort.size()-1){
                    disk[i].Next_number = disk_sort[r+1].Next_number;
                    r++;
                }else if(r==disk_sort.size()-1){
                    disk[i].Next_number = disk_sort[disk_sort.size()-1-i].Next_number;
                }
            }
        }
    } else if(decreaserbutton->isChecked()){
        //当 单选按钮 为 向磁道号减少方向
        disk_sort = disk;
        DISK_AscStablesort(&disk_sort);

        if(Now_number<disk_sort.first().Next_number || Now_number>disk_sort.last().Next_number){
            //当 当前磁头位置 小于 磁道号升序序列最小值 或 大于 最大值时
            DISK_DescStablesort(&disk);
        } else {
            //remember 记录 有磁道号升序序列中 第一个大于  磁道号 位置
            int remember=0;
            for(int j=0; j<disk_sort.size()-1; j++){
                if(Now_number>=disk_sort[j].Next_number && Now_number<=disk_sort[j+1].Next_number){
                    remember=j;
                    break;
                }
            }
            //求出 disk 下一个磁道号 队列
            for(int i=0, r = remember; i<disk.size(); i++){
                if(r!=-1){
                    disk[i].Next_number = disk_sort[r].Next_number;
                    r--;
                }else if(r==-1){
                    disk[i].Next_number = disk_sort[i].Next_number;
                }
            }
        }
    }

    //计算并记录 磁头 移动距离
    Now_number = numberEdit->text().toInt();
    for(int i=0; i<disk.size(); i++){
        disk[i].distance = qAbs(Now_number - disk[i].Next_number);
        Now_number = disk[i].Next_number;
    }
}

//CSCAN算法
void MainWindow::CSCAN(){
    if(increaserbutton->isChecked()){
        //当 单选按钮 为 向磁道号增加方向
        disk_sort = disk;
        DISK_AscStablesort(&disk_sort);

        if(Now_number<disk_sort.first().Next_number || Now_number>disk_sort.last().Next_number){
            //当 当前磁头位置 小于 磁道号升序序列最小值 或 大于 最大值时
            DISK_AscStablesort(&disk);
        } else {
            //remember 记录 有磁道号升序序列中 第一个大于  磁道号 位置
            int remember=0;
            for(int j=0; j<disk_sort.size()-1; j++){
                if(Now_number>=disk_sort[j].Next_number && Now_number<=disk_sort[j+1].Next_number){
                    remember=j;
                    break;
                }
            }
            //求出 disk 下一个磁道号 队列
            for(int i=0, r = remember; i<disk.size(); i++){
                if(r<disk_sort.size()-1){
                    disk[i].Next_number = disk_sort[r+1].Next_number;
                    r++;
                }else if(r==disk_sort.size()-1){
                    disk[i].Next_number = disk_sort[i-(r-remember)].Next_number;
                }
            }
        }

    } else if(decreaserbutton->isChecked()){
        //当 单选按钮 为 向磁道号减少方向
        disk_sort = disk;
        DISK_AscStablesort(&disk_sort);

        if(Now_number<disk_sort.first().Next_number || Now_number>disk_sort.last().Next_number){
            //当 当前磁头位置 小于 磁道号升序序列最小值 或 大于 最大值时
            DISK_DescStablesort(&disk);
        } else {
            //remember 记录 有磁道号升序序列中 第一个大于  磁道号 位置
            int remember=0;
            for(int j=0; j<disk_sort.size()-1; j++){
                if(Now_number>=disk_sort[j].Next_number && Now_number<=disk_sort[j+1].Next_number){
                    remember=j;
                    break;
                }
            }
            //求出 disk 下一个磁道号 队列
            for(int i=0, r = remember; i<disk.size(); i++){
                if(r!=-1){
                    disk[i].Next_number = disk_sort[r].Next_number;
                    r--;
                } else if(r==-1){
                    disk[i].Next_number = disk_sort[remember+disk_sort.size()-i].Next_number;
                }
            }
        }
    }

    //计算并记录 磁头 移动距离
    Now_number = numberEdit->text().toInt();
    for(int i=0; i<disk.size(); i++){
        disk[i].distance = qAbs(Now_number - disk[i].Next_number);
        Now_number = disk[i].Next_number;
    }
}

//计算 移动磁道数 和 平均寻道长度 并显示
void MainWindow::setResult(){
    //计算 移动磁道数 和 平均寻道长度
    int move_Count = 0;
    for(int i=0; i<disk.size(); i++){
        move_Count += disk[i].distance;
    }
    double aver_move_Count = move_Count / (double)disk.size();
    //界面显示结果
    moveLabel->setText("移动磁道数：" + QString::number(move_Count));
    move_averageLabel->setText("平均寻道长度：" + QString::number(aver_move_Count));
}

//设置 排序GroupBox 可选
void MainWindow::setorder_on(){
    orderGroupBox->setEnabled(true);
}

//设置 排序GroupBox 不可选
void MainWindow::setorder_off(){
    orderGroupBox->setEnabled(false);
}

//按 下一次磁道号 升序排序
void MainWindow::DISK_AscStablesort(QVector<DISK> *vector){
    std::stable_sort(vector->begin(),vector->end(),[](const DISK &A,const DISK &B){
        return A.Next_number < B.Next_number;});
}

//按 下一次磁道号 降序排序
void MainWindow::DISK_DescStablesort(QVector<DISK> *vector){
    std::stable_sort(vector->begin(),vector->end(),[](const DISK &A,const DISK &B){
        return A.Next_number > B.Next_number;});
}
