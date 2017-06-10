#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <set>
#include <stack>
#include <sstream>
#include <map>
using namespace std;

string ipath;
map<int,int> a2n;
QString to16(int x) {
    stringstream buf;
    buf << "0x" << std::hex << x;
    return QString::fromStdString(buf.str());
}
QString toq(const string &s) {
    return QString::fromStdString(s);
}

QColor colW = QColor(100,255,255);
QColor colM = QColor(255,100,255);
QColor colE = QColor(255,155,155);
QColor colD = QColor(170,170,125);
QColor colF = QColor(155,155,255);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    QString path = QFileDialog::getOpenFileName(this, tr("Open a file"), ".", tr("Y86 Files(*.yo)"));
    if(path.length() == 0) {
        exit(0);
    }
    ipath = path.toStdString();
    ui->setupUi(this);
    timer = new QTimer(this);

    QPalette qp;
    /*
    qp.setColor(QPalette::Active, QPalette::Background, colW);
    ui->lW->setAutoFillBackground(true);
    ui->lW->setPalette(qp);
    qp.setColor(QPalette::Active, QPalette::Background, colM);
    ui->lM->setAutoFillBackground(true);
    ui->lM->setPalette(qp);
    qp.setColor(QPalette::Active, QPalette::Background, colF);
    ui->lF->setAutoFillBackground(true);
    ui->lF->setPalette(qp);
    qp.setColor(QPalette::Active, QPalette::Background, colE);
    ui->lE->setAutoFillBackground(true);
    ui->lE->setPalette(qp);
    qp.setColor(QPalette::Active, QPalette::Background, colD);
    ui->lD->setAutoFillBackground(true);
    ui->lD->setPalette(qp);
    */

    qp.setColor(QPalette::WindowText, colW);
    ui->lW->setPalette(qp);
    qp.setColor(QPalette::WindowText, colM);
    ui->lM->setPalette(qp);
    qp.setColor(QPalette::WindowText, colF);
    ui->lF->setPalette(qp);
    qp.setColor(QPalette::WindowText, colE);
    ui->lE->setPalette(qp);
    qp.setColor(QPalette::WindowText, colD);
    ui->lD->setPalette(qp);

    connect(timer, SIGNAL(timeout()), this, SLOT(stepOne()));
    pipeReset();
}

int intv = 1000;

void MainWindow::timerGo() {
    timer->start(intv);
}

void MainWindow::timerPause() {
    timer->stop();
}

void MainWindow::stepOne() {

    if (ended) {
        QMessageBox::information(NULL, tr(""), tr("Program has ended."));
        timerPause();
        return;
    }

    cerr << "step one called!" << endl;
    proc_one();
    showAll();
}

void MainWindow::stepNext() {
    cerr << "step next called!" << endl;
    timer->stop();
    stepOne();
}

void MainWindow::pipeReset() {
    transfer(ipath);
    pipe_init("__ins_buf__");
    auto tbl = read_from_yo(ipath);
    ui->mem_watch->resizeColumnsToContents();
    ui->mem_watch->setRowCount(1000);
    ui->ins_watch->resizeColumnsToContents();
    ui->ins_watch->setRowCount(tbl.first.size());
    //for (int i=0;i<ui->ins_watch->rowCount();i++)
    //    setcol(i,QColor(255,255,255));

    int cnt = 0;
    for (auto i:tbl.first) {
        ui->ins_watch->setItem(cnt,0,new QTableWidgetItem(to16(i.addr)));
        //ui->ins_watch->setItem(cnt,1,new QTableWidgetItem(toq(i.code)));
        ui->ins_watch->setItem(cnt,1,new QTableWidgetItem(toq(i.content)));
        cnt++;
    }
    a2n = tbl.second;
    showAll();
}

void ST(QLineEdit *w, int x) {
    w->setText(to16(x));
}

void ST(QLineEdit *w, string x) {
    w->setText(QString::fromStdString(x));
}

void MainWindow::setcol(int rn, QColor col) {
    for (int i=0;i<2;i++) {
        QTableWidgetItem *t = new QTableWidgetItem(*ui->ins_watch->item(rn,i));
        t->setBackgroundColor(col);
        ui->ins_watch->setItem(rn,i,t);
    }
}

void MainWindow::showAll() {
    const PIPE_ &p = PIPE[clock_cnt];
    cerr << "showAll called!!" << endl;
    ST(ui->tcond,to_string(p.ALU.ZF)+","+to_string(p.ALU.SF) +","+to_string(p.ALU.OF));
    /*
    printf("\"RegisterFiles\" : { ");
    printf("\"eax\" : %d, ",val[0]);
    printf("\"ecx\" : %d, ",val[1]);
    printf("\"edx\" : %d, ",val[2]);
    printf("\"ebx\" : %d, ",val[3]);
    printf("\"esp\" : %d, ",val[4]);
    printf("\"ebp\" : %d, ",val[5]);
    printf("\"esi\" : %d, ",val[6]);
    printf("\"edi\" : %d ",val[7]);
    printf("}\n");
    */
    ST(ui->tclk, to_string(clock_cnt));

    ST(ui->teax, p.RegisterFiles.val[0]);
    ST(ui->tecx, p.RegisterFiles.val[1]);
    ST(ui->tedx, p.RegisterFiles.val[2]);
    ST(ui->tebx, p.RegisterFiles.val[3]);
    ST(ui->tesp, p.RegisterFiles.val[4]);
    ST(ui->tebp, p.RegisterFiles.val[5]);
    ST(ui->tesi, p.RegisterFiles.val[6]);
    ST(ui->tedi, p.RegisterFiles.val[7]);

    ST(ui->WdstE, p.W.dstE);
    ST(ui->WdstM, p.W.dstM);
    ST(ui->Wicode, p.W.icode);
    ST(ui->Wstat, p.W.stat);
    ST(ui->WvalE, p.W.valE);
    ST(ui->WvalM, p.W.valM);
    //ST(ui->Wstall, p.W.stall);

    ST(ui->MCnd, p.M.Cnd);
    ST(ui->MdstE, p.M.dstE);
    ST(ui->MdstM, p.M.dstM);
    ST(ui->Micode, p.M.icode);
    ST(ui->Mstat, p.M.stat);
    ST(ui->MvalA, p.M.valA);
    ST(ui->MvalE, p.M.valE);
    //p.M.bubble

    ST(ui->EdstE, p.E.dstE);
    ST(ui->EdstM, p.E.dstM);
    ST(ui->Eicode, p.E.icode);
    ST(ui->Eifun, p.E.ifun);
    ST(ui->EsrcA, p.E.srcA);
    ST(ui->EsrcB, p.E.srcB);
    ST(ui->Estat, p.E.stat);
    ST(ui->EvalA, p.E.valA);
    ST(ui->EvalB, p.E.valB);
    ST(ui->EvalE, p.E.valC);
    //         ^
    //p.E.bubble

    ST(ui->Dicode, p.D.icode);
    ST(ui->Difun, p.D.ifun);
    ST(ui->DrA, p.D.rA);
    ST(ui->DrB, p.D.rB);
    ST(ui->Dstat, p.D.stat);
    ST(ui->DvalC, p.D.valC);
    ST(ui->DvalP, p.D.valP);
    //p.D.bubble, p.D.ifun

    ST(ui->tpredPC, p.F.predPC);
    //for ()

    //ui->mem_watch->clear();
    for (int i=0; i<1000; i++) {
        ui->mem_watch->setItem(i,0,new QTableWidgetItem(to16(i)));
        ui->mem_watch->setItem(i,1,new QTableWidgetItem(to16(p.Datamemory.Val[i])));
    }
    //WMEDF

    for (int i=0;i<ui->ins_watch->rowCount();i++)
        setcol(i,QColor(255,255,255));
    if (a2n.count(p.W.begin)) setcol(a2n[p.W.begin],colW);
    if (a2n.count(p.M.begin)) setcol(a2n[p.M.begin],colM);
    if (a2n.count(p.E.begin)) setcol(a2n[p.E.begin],colE);
    if (a2n.count(p.D.begin)) setcol(a2n[p.D.begin],colD);
    if (a2n.count(p.F.predPC)) setcol(a2n[p.F.predPC],colF);
    ui->mem_watch->resizeColumnsToContents();
    ui->mem_watch->resizeRowsToContents();
    ui->ins_watch->resizeColumnsToContents();
    ui->ins_watch->resizeRowsToContents();
}

void MainWindow::on_MainWindow_destroyed() {

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bReset_clicked()
{

}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lspd->setText(QString::fromStdString(to_string(value)+"Hz"));
    intv = 1000 / value;
}

void MainWindow::on_bBack_clicked()
{
    if (clock_cnt) clock_cnt--;
    showAll();
}

void MainWindow::on_bExit_clicked()
{
    exit(0);
}
