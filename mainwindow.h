#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QGroupBox;
class QTable;
class QRadioButton;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //定义 进程/作业 控制块 PCB
    typedef struct DISK{        
        int Next_number;    //下一个磁道号
        int distance;       //移动距离
    }DISK;
    int Now_number;     //记录当前磁头位置

    QVector<DISK> disk, disk_sort;
    void DISK_AscStablesort(QVector<DISK> *vector); //对 磁道号 按优先级进行升序排序
    void DISK_DescStablesort(QVector<DISK> *vector); //对 磁道号 按优先级进行升序排序
    void getdiskTable();
    void setdiskTable();

    void FCFS();
    void SSTF();
    void SCAN();
    void CSCAN();
    void setResult();

private:
    void createDiskPage();   //定义创建进程调度页面函数

    QWidget *diskPage;

    QGroupBox *tableGroupBox;
    QTableWidget *diskTable;

    QGroupBox *algorithmGroupBox;
    QRadioButton *FCFSrbutton;  //先来先服务算法单选按钮
    QRadioButton *SSTFrbutton;  //最短寻道时间优先算法单选按钮
    QRadioButton *SCANrbutton;  //扫描算法单选按钮
    QRadioButton *CSCANrbutton; //循环扫描算法单选按钮
    QGroupBox *orderGroupBox;
    QRadioButton *increaserbutton;  //向磁道号增加方向访问单选按钮
    QRadioButton *decreaserbutton;  //向磁道号减少方向访问单选按钮

    QGroupBox *operationGroupBox;
    QLabel *disknumLabel;          //当前磁道号Label
    QLineEdit *numberEdit;      //当前磁道号
    QPushButton *runButton;      //运行按钮
    QPushButton *InitButton;    //初始化按钮

    QGroupBox *resultGroupBox;
    QLabel *moveLabel;          //移动磁道数
    QLabel *move_averageLabel;  //平均寻道长度

private slots:
    void Initclick();
    void Runclick();

    void setorder_on();
    void setorder_off();
};

#endif // MAINWINDOW_H
