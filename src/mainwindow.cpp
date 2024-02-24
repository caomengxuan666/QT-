#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include"QStandardItem"
#include<QtWidgets>
#include<QString>
#include<string>
#include<fstream>

using namespace std;

struct User {
    int priority;
    string address;
    string name;
    string phonenumber;
    string group;
};

class ContactNode {
public:
    User user;
    ContactNode* next;

    ContactNode(const User& newUser) : user(newUser), next(nullptr) {}
};


class contact{

public:
    void removeFromContact(int rowIndex) {
        ifstream inFile("information.txt");
        ofstream outFile("temp.txt");

        if (!inFile.is_open() || !outFile.is_open()) {
            qDebug() << "打开文件时发生错误！";
            return;
        }

        string line;
        int currentRow = 0;

        while (getline(inFile, line)) {
            if (currentRow != rowIndex) {
                // 如果当前行不是目标行，将该行写入临时文件
                outFile << line << endl;
            }

            // 增加当前行计数
            currentRow++;
        }

        // 关闭文件
        inFile.close();
        outFile.close();

        // 删除原始文件
        if (remove("information.txt") != 0) {
            qDebug() << "删除原始文件时发生错误！";
            return;
        }

        // 将临时文件重命名为原始文件
        if (rename("temp.txt", "information.txt") != 0) {
            qDebug() << "重命名临时文件时发生错误！";
            return;
        }

        qDebug() << "成功从文件中删除行号为 " << rowIndex << " 的成员信息。";
    }

void wtf(User&user){
    static fstream fs;
    fs.open("information.txt",ios::app);
    fs<<endl;
    fs<<user.name<<" ";
    fs<<user.phonenumber<<" ";
    fs<<user.priority<<" ";
    fs<<user.address<<" ";
    fs<<user.group<<" ";
    fs<<endl;
    fs.close();
    qDebug()<<"successfullt write to file\n";
}

void setModel(QStandardItemModel *m) {
    model = m;
}

void removeFromContact(){

}

void addContact(const User& newUser) {
    ContactNode* newNode = new ContactNode(newUser);

    if (head == nullptr || newUser.priority > head->user.priority) {
        // 如果链表为空或新节点的优先级较高，将新节点插入到链表头
        newNode->next = head;
        head = newNode;
    } else {
        // 遍历链表找到适当位置插入新节点
        ContactNode* current = head;
        while (current->next != nullptr && current->next->user.priority >= newUser.priority) {
            current = current->next;
        }

        newNode->next = current->next;
        current->next = newNode;
    }
}

User getContactFromView(const QModelIndex &index) const {
    User user;
    user.name = model->data(index, Qt::DisplayRole).toString().toStdString();
    user.phonenumber = model->data(index.siblingAtColumn(1), Qt::DisplayRole).toString().toStdString();
    user.priority = model->data(index.siblingAtColumn(2), Qt::DisplayRole).toInt();
    user.address = model->data(index.siblingAtColumn(3), Qt::DisplayRole).toString().toStdString();
    user.group = model->data(index.siblingAtColumn(4), Qt::DisplayRole).toString().toStdString();
    return user;
}

void rff()
{
    // 清空链表
    while (head != nullptr)
    {
        ContactNode *temp = head;
        head = head->next;
        delete temp;
    }

    ifstream fs("information.txt", ios::in);
    if (!fs.is_open())
    {
        // 文件不存在，可以根据需要进行处理
        qDebug() << "NOT FOUND FILE";
    }

    string line;
    while (getline(fs, line))
    {
        if (line.empty())
        {
            continue; // 跳过空行
        }

        User user;
        istringstream iss(line);
        iss >> user.name >> user.phonenumber >> user.priority >> user.address >> user.group;

        addContact(user);
    }

    fs.close();
}

ContactNode* getHead(){
    return head;
}

private:
ContactNode* head;
QStandardItemModel*model;

};
static contact ct;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle("电话簿");
    setupModel();
    ui->tableView->setModel(model);//把view和数据关联起来
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    QStringList headers={"姓名","电话","优先级","地址","群组"};
    model->setHorizontalHeaderLabels(headers);
    // 设置姓名、电话、优先级、群组列的固定宽度
    ui->tableView->setColumnWidth(0, 100); // 姓名列
    ui->tableView->setColumnWidth(1, 150); // 电话列
    ui->tableView->setColumnWidth(2, 80);  // 优先级列
    ui->tableView->setColumnWidth(3,300);   //地址列
    ui->tableView->setColumnWidth(4, 160); // 群组列

    // 创建数据模型
    setupModel();

    // 设置源模型
    ui->tableView->setModel(model);


    // 创建过滤器模型
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);

    // 设置过滤器模型为表格视图的模型
    ui->tableView->setModel(proxyModel);

    // 设置过滤器模型过滤所有列的数据
    proxyModel->setFilterKeyColumn(-1);

    //connect(ui->ps, &QPushButton::clicked, this, &MainWindow::saveToFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::saveToFile(const QString &text)
{
    // 调用 contact 类的 wtf 函数，将文本保存到文件中
    User newUser;
    newUser.name = ui->lineEdit->text().toStdString();

    newUser.phonenumber =ui->lineEdit->text().toStdString();
    newUser.priority = ui->lineEdit->text().toInt();
    newUser.address = ui->lineEdit->text().toStdString();
    newUser.group = ui->lineEdit->text().toStdString();

    ct.wtf(newUser);
}


void MainWindow::setupModel(){
    model=new QStandardItemModel(0,5,this);
 ct.setModel(model);
    ct.rff();

     ContactNode *current = ct.getHead();
    // 遍历链表，将数据添加到模型中
    int row = 0;
    while (current != nullptr)
    {
    QStandardItem *item = new QStandardItem(QString::fromStdString(current->user.name));
    model->setItem(row, 0, item);

    item = new QStandardItem(QString::fromStdString(current->user.phonenumber));
    model->setItem(row, 1, item);

    item = new QStandardItem(QString::number(current->user.priority));
    model->setItem(row, 2, item);

    item = new QStandardItem(QString::fromStdString(current->user.address));
    model->setItem(row, 3, item);

    item = new QStandardItem(QString::fromStdString(current->user.group));
    model->setItem(row, 4, item);

    // 移动到下一个节点
    current = current->next;
    ++row;
    }

    QStringList headers = { "姓名", "电话", "优先级", "地址", "群组" };
                            model->setHorizontalHeaderLabels(headers);

    /*
    QStringList names;
    names<<"cmx"<<"hhw"<<"cyx"<<"lwb"<<"cyz";

    QStringList phonenumber;
    phonenumber<<"xxxxxxxxxxx"<<"9"<<"666"<<"6"<<"111";

    QStringList priority;
    priority<<"5"<<"3"<<"1"<<"2"<<"4";

    QStringList address;
    address<<"中国安徽省黄山市休宁县海阳镇"<<"bz"<<"mas"<<"hf"<<"hs";

    QStringList group;
    group<<"family"<<"boys"<<"mate"<<"friends"<<"classmate";

    for(int row=0;row<5;++row){
        QStandardItem*item=new QStandardItem(names[row]);
        model->setItem(row,0,item);

        item=new QStandardItem(phonenumber[row]);
        model->setItem(row,1,item);

        item=new QStandardItem(priority[row]);
        model->setItem(row,2,item);

        item=new QStandardItem(address[row]);
        model->setItem(row,3,item);

        item=new QStandardItem(group[row]);
        model->setItem(row,4,item);
    }
*/
}


void MainWindow::on_p0_clicked()
{

                            // 在最后一行插入新行
                            int newRow = model->rowCount();
                            model->insertRow(newRow);

                            // 获取最后一行的索引
                            QModelIndex index0 = model->index(newRow, 0);
                            QModelIndex index1 = model->index(newRow, 1);
                            QModelIndex index2 = model->index(newRow, 2);
                            QModelIndex index3 = model->index(newRow, 3);
                            QModelIndex index4 = model->index(newRow, 4);

                            // 设置新联系人的默认数据
                            model->setData(index0, "new_person", Qt::DisplayRole);
                            model->setData(index1, "new_phonenumber", Qt::DisplayRole);
                            model->setData(index2, 1, Qt::DisplayRole);  // 设置默认优先级为1
                            model->setData(index3, "new_address", Qt::DisplayRole);
                            model->setData(index4, "new_group", Qt::DisplayRole);


                            // 从文件中读取真实的数据
                            User newUser = ct.getContactFromView(index0);

    //ct.wtf(newUser);
}


void MainWindow::on_p1_clicked()
{
    // 获取当前选中的模型索引
    QModelIndex currentIndex = ui->tableView->currentIndex();

    // 检查是否有选中的行
    if (currentIndex.isValid()) {
        // 如果当前行正在编辑，结束编辑以确保数据被提交
        if (ui->tableView->isRowHidden(currentIndex.row()))
            ui->tableView->closePersistentEditor(currentIndex);

        // 删除选中的行
        model->removeRow(currentIndex.row());
    } else {
        qDebug() << "No row selected to delete.";
    }
    ct.removeFromContact(currentIndex.row());

}

// 在输入框输入内容时触发的槽函数
void MainWindow::on_lineEdit_textChanged(const QString &filterText)
{
    // 设置过滤器字符串
    QRegularExpression regExp(filterText, QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(regExp);
}

void MainWindow::updateTableView()
{
    model->clear();
    setupModel();
}

void MainWindow::on_ps_clicked()
{
    // 获取当前选中的模型索引
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        // 获取选中行的第一个索引（整行的第一列）
        int selectedRow = selectedIndexes.first().row();
        // 从文件中删除老数据
        ct.removeFromContact(selectedRow);

        // 获取整行的数据
        User selectedUser;
        selectedUser.name = model->index(selectedRow, 0).data(Qt::DisplayRole).toString().toStdString();
        selectedUser.phonenumber = model->index(selectedRow, 1).data(Qt::DisplayRole).toString().toStdString();
        selectedUser.priority = model->index(selectedRow, 2).data(Qt::DisplayRole).toInt();
        selectedUser.address = model->index(selectedRow, 3).data(Qt::DisplayRole).toString().toStdString();
        selectedUser.group = model->index(selectedRow, 4).data(Qt::DisplayRole).toString().toStdString();


        // 将数据保存到文件
        ct.wtf(selectedUser);
    } else {
        qDebug() << "No row selected.";
    } // 获取当前选中的模型索引

}

