#include "mainwindow.h"
#include "ui_mainwindow.h"

//constructeur de la fenetre
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Pile& P = controleur.getPile();
    ui->vuePile->setRowCount(P.getNbToAffiche());
    ui->vuePile->setColumnCount(1);
    ui->message->setStyleSheet("background: cyan; color: black");
    ui->message->setReadOnly(true);
    ui->vuePile->setStyleSheet("background: darker; color: white");
    ui->vuePile->verticalHeader()->setStyleSheet("color: black");
    ui->vuePile->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->vuePile->horizontalHeader()->setVisible(false);
    ui->vuePile->horizontalHeader()->setStretchLastSection(true);
    QStringList numberlist;
    for(unsigned int i=P.getNbToAffiche();i>0;i--){
            QString str = QString::number(i);
            str+=" : ";
            numberlist<<str;
            ui->vuePile->setItem(i-1,0,new QTableWidgetItem(""));
        }
    ui->vuePile->setVerticalHeaderLabels(numberlist);
    ui->vuePile->setFixedHeight(P.getNbToAffiche()*ui->vuePile->rowHeight(0)+2);
    connect(&P,SIGNAL(modificationEtat()),this,SLOT(refresh()));
    connect(ui->commande,SIGNAL(returnPressed()),this,SLOT(getNextCommande()));
    ui->commande->setFocus();
    MainWindow::on_activeClavier_clicked();
    ui->message->setText("Bonjour");
}
//destructeur
MainWindow::~MainWindow()
{
    delete ui;
}
//clavier
void MainWindow::on_button0_clicked(){
    ui->commande->insert("0");
}
void MainWindow::on_button1_clicked(){
    ui->commande->insert("1");
}
void MainWindow::on_button2_clicked(){
    ui->commande->insert("2");
}
void MainWindow::on_button3_clicked(){
    ui->commande->insert("3");
}
void MainWindow::on_button4_clicked(){
    ui->commande->insert("4");
}
void MainWindow::on_button5_clicked(){
    ui->commande->insert("5");
}
void MainWindow::on_button6_clicked(){
    ui->commande->insert("6");
}
void MainWindow::on_button7_clicked(){
    ui->commande->insert("7");
}
void MainWindow::on_button8_clicked(){
    ui->commande->insert("8");
}
void MainWindow::on_button9_clicked(){
    ui->commande->insert("9");
}
void MainWindow::on_buttonPlus_clicked(){
    ui->commande->insert("+");
}
void MainWindow::on_buttonMoins_clicked(){
    ui->commande->insert("-");
}
void MainWindow::on_buttonFois_clicked(){
    ui->commande->insert("*");
}
void MainWindow::on_buttonDiv_clicked(){
    ui->commande->insert("/");
}
void MainWindow::on_buttonPoint_clicked(){
    ui->commande->insert(".");
}
void MainWindow::on_buttonEnter_clicked(){
    ui->commande->returnPressed();
}
void MainWindow::on_activeClavier_clicked(){
    if(ui->activeClavier->isChecked()){
        ui->button0->show();
        ui->button1->show();
        ui->button2->show();
        ui->button3->show();
        ui->button4->show();
        ui->button5->show();
        ui->button6->show();
        ui->button7->show();
        ui->button8->show();
        ui->button9->show();
        ui->buttonPlus->show();
        ui->buttonMoins->show();
        ui->buttonFois->show();
        ui->buttonDiv->show();
        ui->buttonPoint->show();
        ui->buttonEnter->show();
    }
    else{
        ui->button0->hide();
        ui->button1->hide();
        ui->button2->hide();
        ui->button3->hide();
        ui->button4->hide();
        ui->button5->hide();
        ui->button6->hide();
        ui->button7->hide();
        ui->button8->hide();
        ui->button9->hide();
        ui->buttonPlus->hide();
        ui->buttonMoins->hide();
        ui->buttonFois->hide();
        ui->buttonDiv->hide();
        ui->buttonPoint->hide();
        ui->buttonEnter->hide();
    }
}
void MainWindow::on_taillePile_valueChanged(){
    Pile& P = Controleur::getInstance().getPile();
    P.setNbToAffiche((unsigned)ui->afficheTaillePile->text().toInt());
    for(int i=ui->vuePile->rowCount();i>0;i--){
         ui->vuePile->removeRow(i);
    }
    ui->vuePile->setRowCount(P.getNbToAffiche());
    QStringList numberlist;
    for(unsigned int i=P.getNbToAffiche();i>0;i--){
            QString str = QString::number(i);
            str+=" : ";
            numberlist<<str;
            ui->vuePile->setItem(i-1,0,new QTableWidgetItem(""));
        }
    ui->vuePile->setVerticalHeaderLabels(numberlist);
    ui->vuePile->setFixedHeight(P.getNbToAffiche()*ui->vuePile->rowHeight(0)+2);
    refresh();
}

void MainWindow::refresh(){
    //Le message
    ui->message->clear();
    //Effacer tout
    Pile& P = controleur.getInstance().getPile();
    for(unsigned int i=0;i<P.getNbToAffiche();i++)
        ui->vuePile->item(i,0)->setText("");
    //Mettre a jour de la vuePile
    unsigned int nb=1;
    for(QVector<Item*>::iterator It=P.itTab.begin(); It!=P.itTab.end() && nb<=P.getNbToAffiche();++It,++nb)
        ui->vuePile->item(P.getNbToAffiche()-nb,0)->setText((*It)->getLitterale().toString());
    //Mettre a jour de la vueVar
    //QHash<QString,LitteraleNumeric*> Var;
    nb=0;
    for(QHash<QString,LitteraleNumeric*>::iterator It=controleur.getVar().begin(); It!=controleur.getVar().end();++It,nb++)
        ui->vueVar->item(nb,0)->setText((*It)->toString());

}
void MainWindow::getNextCommande(){
    QSound ring ("beep.wav");
    //Recuperation du texte de la ligne de commande
    QString c = ui->commande->text();
    //Extraction de chaque element de la ligne
    QTextStream stream(&c);
    QString com;
    do{
        try{
            stream>>com;//extraction d'un element
            //envoyer la commande au controleur
            if(com!="") controleur.commande(com);
            ui->commande->clear();
            //ui->message->clear();
        }
        catch (LitteraleException e) { ui->message->setText(e.getInfo()); ring.play(); }
        catch (OperateurException o) { ui->message->setText(o.getInfo()); ring.play(); }
        catch (PileException p) { ui->message->setText(p.getInfo()); ring.play(); }
    }while(com!="");
    //Ligne de commande a zero
    ui->commande->clear();
}
