#ifndef DBSCLASS_H
#define DBSCLASS_H
//#include <QMainWindow>

#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <qcustomplot.h>
#include <QMessageBox>
#include <QWidget>
#include <QTableView>
#include <QFontComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QModelIndex>
#include <QFont>
#include <QPainter>
#include <QDesktopServices>
#include <QPrinter>
#include <QTextEdit>
#include <QStringListModel>
#include <qprintdialog.h>
#include <QPrinterInfo>
#include <QPrintPreviewDialog>


//#include <qwt/qwt_plot.h>
//#include <qwt/qwt_plot_curve.h>
//#include "pieplot.h"
#include "QVBoxLayout"
#include<nightcharts.h>

#include <cstdlib>
#include <ctime>

class dbsclass
{
public:
    dbsclass();
    void fill_combo(QFontComboBox *ctbx,QString qry);
    void fill_combobox(QComboBox *ctbx,QString qry);
    void fill_combobox(QComboBox *ctbx,QString qry,int v);
    void fill_tbx(QLineEdit *tbx,QString qry,int v);
    QString fill_string(QString qry,int v);
    QByteArray fill_blob(QString qry,int v);
    double fill_array(QString qry,int v);
    int tbl_sz(QString qry);
    void fill_label(QLabel *lbl,QString qry,int v);
    bool chkpass(QString qry);
    QString fill_array_string(QString qry,int v);
    void pie3d(QLabel *lbl,QString name,QString value);
    void bargraph3d(QLabel *lbl,QString name,QString value);

    QSqlDatabase db;
    bool conn(){
        db=QSqlDatabase::addDatabase("QMYSQL3");
        //db.setDatabaseName("/usr/share/mysql-workbench/data/data.db");
        db.setHostName("localhost");
        db.setDatabaseName("ral");
        db.setUserName("root");
        db.setPassword("root");
        //db.setHostName("127.0.0.1");
        db.setPort(3306);
        if(!db.open()){
            return false;
            QMessageBox msg;
            msg.setText("Connection Faild.");
            msg.exec();
        }
        else{
            return true;}
    }

    bool conn(QStringList lst){
        db=QSqlDatabase::addDatabase("QMYSQL3");
        //db.setDatabaseName("/usr/share/mysql-workbench/data/data.db");
        db.setHostName(lst.value(0));
        db.setDatabaseName(lst.value(1));
        db.setUserName(lst.value(2));
        db.setPassword(lst.value(3));
        db.setConnectOptions("CLIENT_SSL=1;CLIENT_IGNORE_SPACE=1");
        //db.setHostName("127.0.0.1");
        QString p=lst.value(5);
        db.setPort(p.toInt());
        if(!db.open()){
            return false;
            //QMessageBox msg;
            //msg.setText("Connection Faild.");
            // msg.exec();
        }
        else{
            return true;}
    }



    void conn_close(){
        db.close();
        db=QSqlDatabase();
        db.removeDatabase(QSqlDatabase::defaultConnection);
    }

    void queryfeeder(QString q){

        db.driver()->beginTransaction();
        QSqlQuery qry;
        qry.prepare(q);
        if(qry.exec()){
            /* QMessageBox msg;
            msg.setText("Operation Succesfull");
            msg.exec();*/
        }
        else{
            QSqlError er=qry.lastError();
            QString e=er.text();
            QMessageBox msg;
            msg.setText(e);
            msg.exec();
            db.rollback();
        }

        db.driver()->commitTransaction();

    }
    void queryfeeder(QString q,QByteArray ba,QString blb){

        db.driver()->beginTransaction();
        QSqlQuery qry;
        qry.prepare(q);
        qry.bindValue(blb,ba);
        if(qry.exec()){
            /* QMessageBox msg;
            msg.setText("Operation Succesfull");
            msg.exec();*/
        }
        else{
            QSqlError er=qry.lastError();
            QString e=er.text();
            QMessageBox msg;
            msg.setText(e);
            msg.exec();
            db.rollback();
        }

        db.driver()->commitTransaction();

    }
    void queryfeeder(QLineEdit *tbx,QString q,QByteArray ba,QString blb){

        db.driver()->beginTransaction();
        QSqlQuery qry;
        qry.prepare(q);
        qry.bindValue(blb,ba);
        if(qry.exec()){
            tbx->setText(qry.lastInsertId().toString());
        }
        else{
            QSqlError er=qry.lastError();
            QString e=er.text();
            QMessageBox msg;
            msg.setText(e);
            msg.exec();
            db.rollback();
        }

        db.driver()->commitTransaction();

    }
    void tv_display(QTableView *qtv,QString q);

    QString recent_time(){
        QString q="SELECT CURTIME()";
        return fill_string(q,0);
    }
    QString recent_date(){
        QString q="SELECT CURDATE()";
        return fill_string(q,0);
    }

    void queryfeeder(QLineEdit *tbx,QString q){

        db.driver()->beginTransaction();
        QSqlQuery qry;
        qry.prepare(q);
        if(qry.exec()){
            tbx->setText(qry.lastInsertId().toString());
        }
        else{
            QSqlError er=qry.lastError();
            QString e=er.text();
            QMessageBox msg;
            msg.setText(e);
            msg.exec();
            db.rollback();
        }

        db.driver()->commitTransaction();

    }
    void queryfeeder(QComboBox *ctbx,QString q){

        db.driver()->beginTransaction();
        QSqlQuery qry;
        qry.prepare(q);
        if(qry.exec()){
            ctbx->setEditText(qry.lastInsertId().toString());
        }
        else{
            QSqlError er=qry.lastError();
            QString e=er.text();
            QMessageBox msg;
            msg.setText(e);
            msg.exec();
            db.rollback();
        }

        db.driver()->commitTransaction();

    }

    void search_tv(QString txt,QTableView *tv){
        QModelIndex index;
        int row=tv->model()->rowCount(QModelIndex());
        int col=tv->model()->columnCount(QModelIndex());
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++){
                index=tv->model()->index(i,j,QModelIndex());
                if(tv->model()->data(index).toString().contains(txt)){
                    tv->selectRow(i);
                    tv->scrollTo(index);
                }
            }
        }
    }
    void csv_exp(QString str,QTableView *tv){
        // QString newName = QFileDialog::getSaveFileName();
        QString filters("CSV files (*.csv);;Text files (*.txt);;All files (*.*)");
        QString defaultFilter("Text files (*.txt)");

        /* Static method approach */
        QString newName=QFileDialog::getSaveFileName(0, "Save file", QDir::currentPath(),filters, &defaultFilter);
        QString a="\n";
        int sz=tv->model()->columnCount(QModelIndex());
        for (int i=0;i<sz;i++){
            a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+",";
        }
        newName+=".csv";
        QFile file(newName);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out <<str<<a;
        QModelIndex index;
        int row=tv->model()->rowCount(QModelIndex());
        int col=tv->model()->columnCount(QModelIndex());

        out<<"\n";
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++){
                index=tv->model()->index(i,j,QModelIndex());
                out<<tv->model()->data(index).toString()<<",";
            }
            out<<"\n";
        }
        file.close();
    }

    void csv_exp(QTableView *tv,QString f_name){



        QString newName=f_name;
        QString a="";
        int sz=tv->model()->columnCount(QModelIndex());
        for (int i=0;i<sz;i++){
            if(i==sz-1){
                a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
            }
            else{
                a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+",";
            }
        }
        newName+=".csv";
        QFile file(newName);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out <<a;
        QModelIndex index;
        int row=tv->model()->rowCount(QModelIndex());
        int col=tv->model()->columnCount(QModelIndex());

        out<<"\n";
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++){
                if(j==col-1){
                    index=tv->model()->index(i,j,QModelIndex());
                    out<<tv->model()->data(index).toString();
                }
                else{
                    index=tv->model()->index(i,j,QModelIndex());
                    out<<tv->model()->data(index).toString()<<",";
                }
            }
            out<<"\n";
        }
        file.close();
    }
    void recipt_print(QString id,QString str,QTableView *tv,QString last){

        QString a=str;
        int sz=tv->model()->columnCount(QModelIndex());
        a+="SL\t";
        for (int i=0;i<sz;i++){
            a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"\t";
            if(i==0){
                a+="\t\t";
            }
        }
        a+="\n___________________________________________________________________________________________________\n";




        QModelIndex index;
        int row=tv->model()->rowCount(QModelIndex());
        int col=tv->model()->columnCount(QModelIndex());


        for(int i=0;i<row;i++){
            a+=QString::number(i+1)+"\t";
            for(int j=0;j<col;j++){
                index=tv->model()->index(i,j,QModelIndex());
                a+=tv->model()->data(index).toString()+"\t";
                if(j==0){
                    if(tv->model()->data(index).toString().length()<15){
                        a+="\t";
                    }
                }

            }
            a+="\n";
        }
        a+=last;
        QFile file(id);
        if (!file.open(QFile::ReadWrite | QFile::Text)){}
        file.seek(0);
        file.write(a.toUtf8());
        file.close();
        QDesktopServices::openUrl(id);

        QPrinter printer(QPrinter::ScreenResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName("abcd.pdf");

        QImage img(":/new/prefix1/images/logo_small.png");

        QTextDocument doc;
        doc.setPlainText(a);
        doc.print(&printer);

        QTextCursor cursor(&doc);
        cursor.movePosition(QTextCursor::End);
        cursor.insertImage(img);
        /*
        doc.addResource(QTextDocument::ImageResource,QUrl("spl_logo.png"), QVariant(img));
        QTextImageFormat imageFormat;
        imageFormat.setName("spl_logo.png");
        cursor.insertImage(imageFormat);


        QTextEdit parent;
        parent.setText(a);
        parent.print(&printer);
        QPainter paint;
        paint.begin(&printer);
        paint.drawImage(1,1,img);
        paint.end();*/


    }

    double max_value(QString qry,int v){
        QString str;
        QSqlQuery q;
        double max=0;
        q.prepare(qry);

        if( q.exec()){
            while(q.next()){
                str=q.value(v).toString();
                if(str.toDouble()>max){
                    max=str.toDouble();
                }
            }
        }
        else{
            QSqlError er=q.lastError();
            QString e=er.text();
            QMessageBox msg;
            msg.setText(e);
            msg.exec();

        }
        return max;
    }

    void generate_singel_graph(QCustomPlot *plot,QString qry,int x,int y,QString x_label,QString y_label,QString bar_name){
        /*/ set some pens, brushes and backgrounds:
        plot->xAxis->setBasePen(QPen(Qt::white, 1));
        plot->yAxis->setBasePen(QPen(Qt::white, 1));
        plot->xAxis->setTickPen(QPen(Qt::white, 1));
        plot->yAxis->setTickPen(QPen(Qt::white, 1));
        plot->xAxis->setSubTickPen(QPen(Qt::white, 1));
        plot->yAxis->setSubTickPen(QPen(Qt::white, 1));
        plot->xAxis->setTickLabelColor(Qt::white);
        plot->yAxis->setTickLabelColor(Qt::white);
        plot->xAxis->grid()->setPen(QPen(QColor(140, 140, 100), 1, Qt::DotLine));
        plot->yAxis->grid()->setPen(QPen(QColor(140, 140, 100), 1, Qt::DotLine));
        plot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
        plot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
        plot->xAxis->grid()->setSubGridVisible(true);
        plot->yAxis->grid()->setSubGridVisible(true);
        plot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
        plot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
        plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
        plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
        //plot->xAxis->setTickLabelRotation(40);
        QLinearGradient plotGradient;
        plotGradient.setStart(0, 0);
        plotGradient.setFinalStop(0, 350);
        plotGradient.setColorAt(0, QColor(108, 98, 88, 250));
        plotGradient.setColorAt(1, QColor(108, 98, 88, 255));
        plot->setBackground(plotGradient);
        QLinearGradient axisRectGradient;
        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(108, 98, 100, 250));
        axisRectGradient.setColorAt(1, QColor(108, 98, 105, 255));
        plot->axisRect()->setBackground(axisRectGradient);
        */

        plot->legend->setVisible(true);
        plot->axisRect()->setupFullAxesBox();
        QFont legendFont;
        legendFont.setPointSize(8);
        plot->legend->setFont(legendFont);
        plot->legend->setSelectedFont(legendFont);
        plot->legend->setSelectableParts(QCPLegend::spItems);
        plot->setContextMenuPolicy(Qt::CustomContextMenu);

        plot->xAxis->setLabelColor(QColor(243, 243, 243,100));
        plot->yAxis->setLabelColor(QColor(243, 243, 243,100));



        //----------------------------------------------------------------------------------
        QString b,b1,x_value=fill_array_string(qry,x),y_value=fill_array_string(qry,y);
        double x_range=max_value(qry,x)+5,y_range=max_value(qry,y)+80;
        if(x_range<20){
            x_range=x_range+(20-x_range);
        }
        if(y_range<100){
            y_range=y_range+(100-y_range);
        }
        QStringList x_list,y_list;
        x_list=x_value.split(",");
        y_list=y_value.split(",");
        QVector<double>value,tick;
        QVector<QString>labels;

        for(int i=0;i<x_list.size()-1;i++){
            b=x_list.value(i);
            b1=y_list.value(i);
            value<<b1.toDouble();
            tick<<i;
            labels<<b;
        }
        plot->clearPlottables();
        QCPBars *sum = new QCPBars(plot->xAxis, plot->yAxis);
        plot->addPlottable(sum);
        QPen pen;
        pen.setWidthF(1.2);
        sum->setName(bar_name);
        pen.setColor(QColor(45, 90, 230,170));
        sum->setPen(pen);
        sum->setBrush(QColor(67, 248, 75,170));


        plot->xAxis->setAutoTicks(false);
        plot->xAxis->setAutoTickLabels(false);
        plot->xAxis->setTickVector(tick);
        plot->xAxis->setTickVectorLabels(labels);
        plot->xAxis->setTickLabelRotation(30);
        plot->xAxis->setSubTickCount(0);
        plot->xAxis->setTickLength(0, 4);
        plot->xAxis->grid()->setVisible(true);
        plot->xAxis->setRange(-2,15);
        plot->xAxis->setLabel(x_label);

        plot->yAxis->setAutoTicks(true);
        plot->yAxis->setAutoTickLabels(true);
        plot->yAxis->setRange(-2, y_range);
        plot->yAxis->setPadding(20); // a bit more space to the left border
        plot->yAxis->setLabel(y_label);
        //plot->yAxis->setTickVectorLabels("Grade");
        plot->yAxis->grid()->setSubGridVisible(true);
        QPen gridPen;
        gridPen.setStyle(Qt::SolidLine);
        gridPen.setColor(QColor(0, 0, 0, 25));
        plot->yAxis->grid()->setPen(gridPen);
        gridPen.setStyle(Qt::DotLine);
        plot->yAxis->grid()->setSubGridPen(gridPen);

        sum->setData(tick, value);
        sum->keyAxis();

        plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
        plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |

                              QCP::iSelectLegend | QCP::iSelectPlottables);
        plot->replot();
    }

    //-----------------------------------------------------
    //-----------PDF-----------------------------
    void print_pdf(QTableView *tv,QString id){

        int sz=tv->model()->columnCount(QModelIndex());
        id="ABC_"+id+".pdf";
        QPrinter printer(QPrinter::ScreenResolution);
        printer.setPageSize(QPrinter::A4);

        // printer.setFromTo(1,5);
        //printer.setPageRange("%1-%5").arg(1).arg(5);


        /*
            QPrintDialog *dialog = new QPrintDialog(&printer);
            dialog->setMinMax(1, 5);
            dialog->addEnabledOption(QAbstractPrintDialog::PrintMultiplePageRanges);
            if (dialog->exec() == QDialog::Accepted) {
                document()->print(&printer);
            }*/

        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(id);


        QPainter painter;
        painter.begin(&printer);

        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        //painter.drawLine(x,y,x,y);
        painter.drawLine(0,26,760,26);
        painter.drawLine(0,30,760,30);

        QPixmap logo(":/new/prefix1/images/logo_small.png");
        painter.drawPixmap(5,60,logo);

        painter.setPen(QPen(Qt::black));
        painter.setFont(QFont("Arial",24));
        painter.drawText(235,90,"Supreme Pharmaceuticals Limited");

        painter.setFont(QFont("Arial",11));
        painter.drawText(235,120,"House : 38/1,Road:16 (New),Dhanmondi R/A,Dhaka-1209, Bangladesh.");
        painter.drawText(235,140,"Telephone : 88-02-8156577, 88-02-9114813, Fax : 88-02-9135406");
        painter.drawText(235,160,"Website   : www.supreme.com.bd , Email: spl@supreme.com.bd");

        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,200,760,200);
        painter.drawLine(0,204,760,204);

        painter.setPen(QPen(Qt::black));

        int x_pix=20;

        for (int i=0;i<sz;i++){
            //a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"\t";
            if(i==1){
                painter.drawText(x_pix+50,224,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else{
                painter.drawText(x_pix,224,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            x_pix+=170;
        }
        painter.drawLine(20,230,760,230);

        x_pix=20;
        int y_pix=242;

        QModelIndex index;
        int row=tv->model()->rowCount(QModelIndex());
        int col=tv->model()->columnCount(QModelIndex());


        for(int i=0;i<row;i++){
            x_pix=20;
            for(int j=0;j<col;j++){
                index=tv->model()->index(i,j,QModelIndex());

                if(j==1){
                    painter.drawText(x_pix+50,y_pix,tv->model()->data(index).toString());
                }
                else{
                    painter.drawText(x_pix,y_pix,tv->model()->data(index).toString());
                }
                x_pix+=170;
            }
            y_pix+=16;
            if(y_pix>printer.height()){
                printer.newPage();
                y_pix=224;
                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                //painter.drawLine(x,y,x,y);
                painter.drawLine(0,26,900,26);
                painter.drawLine(0,30,900,30);

                QPixmap logo(":/new/prefix1/images/logo_small.png");
                painter.drawPixmap(5,60,logo);

                painter.setPen(QPen(Qt::black));
                painter.setFont(QFont("Arial",24));
                painter.drawText(235,90,"Supreme Pharmaceuticals Limited");

                painter.setFont(QFont("Arial",11));
                painter.drawText(235,120,"House : 38/1,Road:16 (New),Dhanmondi R/A,Dhaka-1209, Bangladesh.");
                painter.drawText(235,140,"Telephone : 88-02-8156577, 88-02-9114813, Fax : 88-02-9135406");
                painter.drawText(235,160,"Website   : www.supreme.com.bd , Email: spl@supreme.com.bd");

                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,200,900,200);
                painter.drawLine(0,204,900,204);

                painter.setPen(QPen(Qt::black));

            }
        }





        //painter.drawText();

        painter.end();
        QDesktopServices::openUrl(id);



    }

    void print_pdf_sales_invoice(QStringList lst,QTableView *tv){
        QPrinter printer;
        printer.setPageSize(QPrinter::A4);
        printer.setFullPage(true);
        printer.setOrientation(QPrinter::Portrait);
        QString filep=lst.value(0);
        printer.setOutputFileName(filep);

        //Print Dialog---------------------------------------
        QPrintDialog *dialog = new QPrintDialog(&printer);
        dialog->setWindowTitle("Print Document, SPL");

        if (dialog->exec() != QDialog::Accepted){}

        QPainter painter;
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
        painter.begin(&printer);

        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        //painter.drawLine(start dot, end dot);
        painter.drawLine(0,15,900,15);
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,20,900,20);

        QPixmap logo(":/new/prefix1/images/logo_small.png");
        painter.drawPixmap(5,30,logo);

        painter.setPen(QPen(Qt::black));
        painter.setFont(QFont("Arial", 22));
        painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

        painter.setFont(QFont("Arial",11));
        painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

        painter.setFont(QFont("Verdana",11));
        painter.drawText(600,125,"CHALLAN & INVOICE");

        painter.setFont(QFont("Arial",11));
        painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
        painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,130,900,130);
        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,135,900,135);

        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.setFont(QFont("Arial",10));

        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,158,900,158);

        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,162,900,162);
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

        //<Invoice Information>
        painter.drawText(5,150, "Form No:");
        painter.drawText(65,150, lst.value(0));  //lineEdit to be replaced

        painter.drawText(190,150,"Effective Date:");
        painter.drawText(280,150,lst.value(1));  //Server Time


        painter.drawText(400,150,"Payment:");
        painter.drawText(460,150,lst.value(2));  //Cash,DBBL etc.

        painter.drawText(600,150,"Vat Reg No:");
        painter.drawText(672,150,lst.value(3));  //Vat Registration Number
        //</Invoice Information>

        //<Agent Data>
        //-- Left hand Side
        painter.drawText(25,180,"Agent ID: ");
        painter.drawText(130,180,lst.value(4));  //Agent ID

        painter.drawText(25,200,"Agent Name: ");
        painter.drawText(130,200,lst.value(5));  //Customer Name

        painter.drawText(25,220,"Address: ");
        painter.drawText(130,220,lst.value(6));

        painter.drawText(25,240,"Transaction : ");
        painter.drawText(130,240,lst.value(7));

        //-- Right hand Side
        painter.drawText(400,180,"Invoice: ");
        painter.drawText(510,180,lst.value(8));

        painter.drawText(400,200,"Inv. Date: ");
        painter.drawText(510,200,lst.value(9));

        painter.drawText(400,220,"Collec. Date: ");
        painter.drawText(510,220,lst.value(10));

        // painter.drawText(400,240,"Sr Name: ");
        // painter.drawText(510,240,lst.value(11));
        //</Agent Data>

        painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,250,900,250);
        painter.drawLine(0,253,900,253);

        //data loop here-------------------------
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        int sz=tv->model()->columnCount(QModelIndex());

        int x_pix=15;

        for (int i=0;i<sz;i++){
            //a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"\t";
            if(i==2){
                painter.drawText(x_pix+50,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==1){
                painter.drawText(x_pix-30,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==3){
                painter.drawText(x_pix+20,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else{
                painter.drawText(x_pix,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            x_pix+=90;
        }
        // painter.drawLine(20,230,760,230);
        painter.drawLine(0,282,900,282);

        x_pix=15;
        int y_pix=295;

        QModelIndex index;
        int row=tv->model()->rowCount(QModelIndex());
        int col=tv->model()->columnCount(QModelIndex());


        for(int i=0;i<row;i++){
            x_pix=20;
            for(int j=0;j<col;j++){
                index=tv->model()->index(i,j,QModelIndex());

                if(j==2){
                    painter.drawText(x_pix+50,y_pix,tv->model()->data(index).toString());
                }
                else if(j==1){
                    painter.drawText(x_pix-30,y_pix,tv->model()->data(index).toString());
                }
                else if(j==3){
                    painter.drawText(x_pix+20,y_pix,tv->model()->data(index).toString());
                }
                else{
                    painter.drawText(x_pix,y_pix,tv->model()->data(index).toString());
                }
                x_pix+=90;
            }
            y_pix+=16;
            if(y_pix>printer.height()){
                printer.newPage();
                y_pix=215;
                painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                painter.begin(&printer);

                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                //painter.drawLine(start dot, end dot);
                painter.drawLine(0,15,800,15);
                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,20,800,20);

                QPixmap logo(":/new/prefix1/images/logo_small.pngg");
                painter.drawPixmap(5,30,logo);

                painter.setPen(QPen(Qt::black));
                painter.setFont(QFont("Arial", 22));
                painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                painter.setFont(QFont("Arial",11));
                painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                painter.setFont(QFont("Verdana",11));
                painter.drawText(600,125,"CHALLAN & INVOICE");

                painter.setFont(QFont("Arial",11));
                painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,130,800,130);
                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,135,800,135);

                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.setFont(QFont("Arial",10));

                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,158,800,158);

                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,162,800,162);
                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                x_pix=15;

                for (int i=0;i<sz;i++){
                    //a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"\t";
                    if(i==1){
                        painter.drawText(x_pix+30,190,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else{
                        painter.drawText(x_pix,190,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    x_pix+=170;
                }
                painter.drawLine(0,202,900,202);

            }

            if(i==row-1){
                painter.drawLine(0,y_pix,900,y_pix);

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,900,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,900,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Total Quantity:");
                painter.drawText(545,y_pix,lst.value(12));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Total Amount:");
                painter.drawText(545,y_pix,lst.value(13));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Discount:");
                painter.drawText(545,y_pix,lst.value(14));

                painter.drawText(145,y_pix,"Due:");
                painter.drawText(245,y_pix,lst.value(19));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Vat:");
                painter.drawText(545,y_pix,lst.value(15));

                painter.drawText(145,y_pix,"Advance:");
                painter.drawText(245,y_pix,lst.value(20));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Total:");
                painter.drawText(545,y_pix,lst.value(16));

                painter.drawText(145,y_pix,"Payment:");
                painter.drawText(245,y_pix,lst.value(21));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Trans. Cost:");
                painter.drawText(545,y_pix,lst.value(17));

                painter.drawText(145,y_pix,"Return:");
                painter.drawText(245,y_pix,lst.value(22));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Net Payable:");
                painter.drawText(545,y_pix,lst.value(18));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawLine(0,y_pix,800,y_pix);

            }
        }

        if(y_pix>printer.height()){
            printer.newPage();
            y_pix=202;
            painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
            painter.begin(&printer);

            painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
            //painter.drawLine(start dot, end dot);
            painter.drawLine(0,15,800,15);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,20,800,20);

            QPixmap logo(":/new/prefix1/images/logo_small.png");
            painter.drawPixmap(5,30,logo);

            painter.setPen(QPen(Qt::black));
            painter.setFont(QFont("Arial", 22));
            painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

            painter.setFont(QFont("Arial",11));
            painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

            painter.setFont(QFont("Verdana",11));
            painter.drawText(600,125,"CHALLAN & INVOICE");

            painter.setFont(QFont("Arial",11));
            painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
            painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,130,800,130);
            painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,135,800,135);

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.setFont(QFont("Arial",10));

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,158,800,158);

            painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,162,800,162);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

            //<Signature>

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(20,1000,150,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(45,1025,"Signature 1"); //data should come from employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(220,1000,350,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(245,1025,"Signature 2"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(420,1000,550,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(445,1025,"Signature 3"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(620,1000,750,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(645,1025,"Signature 4"); //should be come employee ID
            //</Signature>

            //<footer>
            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1080,800,1080);

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(25,1095,"Company Laws & Claws");

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1105,800,1105);
            //</footer>

        }
        else{

            //<Signature>

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(20,1000,150,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(45,1025,"Signature 1"); //data should come from employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(220,1000,350,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(245,1025,"Signature 2"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(420,1000,550,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(445,1025,"Signature 3"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(620,1000,750,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(645,1025,"Signature 4"); //should be come employee ID
            //</Signature>

            //<footer>
            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1080,800,1080);

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(25,1095,"Company Laws & Claws");

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1105,800,1105);
            //</footer>

        }
        painter.end();


    }

    void print_distribution_sales(QStringList lst,QTableView *tv){
        QPrinter printer;
        printer.setPageSize(QPrinter::A4);
        printer.setFullPage(true);
        printer.setOrientation(QPrinter::Portrait);

        //Print Dialog---------------------------------------
        QPrintDialog *dialog = new QPrintDialog(&printer);
        dialog->setWindowTitle("Print Document, SPL");
        if (dialog->exec() != QDialog::Accepted){}

        QPainter painter;
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
        painter.begin(&printer);

        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        //painter.drawLine(start dot, end dot);
        painter.drawLine(0,15,900,15);
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,20,900,20);

        QPixmap logo(":/new/prefix1/images/logo_small.png");
        painter.drawPixmap(5,30,logo);

        painter.setPen(QPen(Qt::black));
        painter.setFont(QFont("Arial", 22));
        painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

        painter.setFont(QFont("Arial",11));
        painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

        painter.setFont(QFont("Verdana",11));
        painter.drawText(600,125,"CHALLAN & INVOICE");

        painter.setFont(QFont("Arial",11));
        painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
        painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,130,900,130);
        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,135,900,135);

        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.setFont(QFont("Arial",10));

        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,158,900,158);

        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,162,900,162);
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

        //<Invoice Information>
        painter.drawText(5,150, "Form No:");
        painter.drawText(65,150, lst.value(0));  //lineEdit to be replaced

        painter.drawText(190,150,"Effective Date:");
        painter.drawText(280,150,lst.value(1));  //Server Time


        //painter.drawText(400,150,"Payment:");
        // painter.drawText(460,150,lst.value(2));  //Cash,DBBL etc.

        painter.drawText(600,150,"Vat Reg No:");
        painter.drawText(672,150,lst.value(3));  //Vat Registration Number
        //</Invoice Information>

        //<Agent Data>
        //-- Left hand Side
        painter.drawText(25,180,"Agent ID: ");
        painter.drawText(130,180,lst.value(4));  //Agent ID

        painter.drawText(25,200,"Agent Name: ");
        painter.drawText(130,200,lst.value(5));  //Customer Name

        painter.drawText(25,220,"Address: ");
        painter.drawText(130,220,lst.value(6));

        // painter.drawText(25,240,"Transaction : ");
        // painter.drawText(130,240,lst.value(7));

        //-- Right hand Side
        painter.drawText(400,180,"Invoice: ");
        painter.drawText(510,180,lst.value(8));

        painter.drawText(400,200,"Inv. Date: ");
        painter.drawText(510,200,lst.value(9));

        painter.drawText(400,220,"Collec. Date: ");
        painter.drawText(510,220,lst.value(10));

        // painter.drawText(400,240,"Sr Name: ");
        // painter.drawText(510,240,lst.value(11));
        //</Agent Data>

        painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,250,900,250);
        painter.drawLine(0,253,900,253);

        //data loop here-------------------------
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        int sz=tv->model()->columnCount(QModelIndex());

        int x_pix=15;

        for (int i=0;i<sz;i++){
            //a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"\t";
            if(i==2){
                painter.drawText(x_pix+50,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==1){
                painter.drawText(x_pix-30,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==3){
                painter.drawText(x_pix+20,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else{
                painter.drawText(x_pix,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            x_pix+=90;
        }
        // painter.drawLine(20,230,760,230);
        painter.drawLine(0,282,900,282);

        x_pix=15;
        int y_pix=295;

        QModelIndex index;
        int row=tv->model()->rowCount(QModelIndex());
        int col=tv->model()->columnCount(QModelIndex());


        for(int i=0;i<row;i++){
            x_pix=20;
            for(int j=0;j<col;j++){
                index=tv->model()->index(i,j,QModelIndex());

                if(j==2){
                    painter.drawText(x_pix+50,y_pix,tv->model()->data(index).toString());
                }
                else if(j==1){
                    painter.drawText(x_pix-30,y_pix,tv->model()->data(index).toString());
                }
                else if(j==3){
                    painter.drawText(x_pix+20,y_pix,tv->model()->data(index).toString());
                }
                else{
                    painter.drawText(x_pix,y_pix,tv->model()->data(index).toString());
                }
                x_pix+=90;
            }
            y_pix+=16;
            if(y_pix>printer.height()){
                printer.newPage();
                y_pix=215;
                painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                painter.begin(&printer);

                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                //painter.drawLine(start dot, end dot);
                painter.drawLine(0,15,800,15);
                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,20,800,20);

                QPixmap logo(":/new/prefix1/images/logo_small.pngg");
                painter.drawPixmap(5,30,logo);

                painter.setPen(QPen(Qt::black));
                painter.setFont(QFont("Arial", 22));
                painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                painter.setFont(QFont("Arial",11));
                painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                painter.setFont(QFont("Verdana",11));
                painter.drawText(600,125,"CHALLAN & INVOICE");

                painter.setFont(QFont("Arial",11));
                painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,130,800,130);
                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,135,800,135);

                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.setFont(QFont("Arial",10));

                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,158,800,158);

                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,162,800,162);
                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                x_pix=15;

                for (int i=0;i<sz;i++){
                    //a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"\t";
                    if(i==1){
                        painter.drawText(x_pix+30,190,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else{
                        painter.drawText(x_pix,190,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    x_pix+=170;
                }
                painter.drawLine(0,202,900,202);

            }

            if(i==row-1){
                painter.drawLine(0,y_pix,900,y_pix);

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,900,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,900,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Total Quantity:");
                painter.drawText(545,y_pix,lst.value(12));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Total Amount:");
                painter.drawText(545,y_pix,lst.value(13));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Discount:");
                painter.drawText(545,y_pix,lst.value(14));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Vat:");
                painter.drawText(545,y_pix,lst.value(15));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Total:");
                painter.drawText(545,y_pix,lst.value(16));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                //painter.drawText(445,y_pix,"Trans. Cost:");
                // painter.drawText(545,y_pix,lst.value(17));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Net Payable:");
                painter.drawText(545,y_pix,lst.value(18));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"CHALLAN & INVOICE");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawLine(0,y_pix,800,y_pix);

            }
        }

        if(y_pix>printer.height()){
            printer.newPage();
            y_pix=202;
            painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
            painter.begin(&printer);

            painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
            //painter.drawLine(start dot, end dot);
            painter.drawLine(0,15,800,15);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,20,800,20);

            QPixmap logo(":/new/prefix1/images/logo_small.png");
            painter.drawPixmap(5,30,logo);

            painter.setPen(QPen(Qt::black));
            painter.setFont(QFont("Arial", 22));
            painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

            painter.setFont(QFont("Arial",11));
            painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

            painter.setFont(QFont("Verdana",11));
            painter.drawText(600,125,"CHALLAN & INVOICE");

            painter.setFont(QFont("Arial",11));
            painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
            painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,130,800,130);
            painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,135,800,135);

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.setFont(QFont("Arial",10));

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,158,800,158);

            painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,162,800,162);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

            //<Signature>

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(20,1000,150,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(45,1025,"Signature 1"); //data should come from employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(220,1000,350,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(245,1025,"Signature 2"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(420,1000,550,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(445,1025,"Signature 3"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(620,1000,750,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(645,1025,"Signature 4"); //should be come employee ID
            //</Signature>

            //<footer>
            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1080,800,1080);

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(25,1095,"Company Laws & Claws");

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1105,800,1105);
            //</footer>

        }
        else{

            //<Signature>

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(20,1000,150,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(45,1025,"Signature 1"); //data should come from employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(220,1000,350,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(245,1025,"Signature 2"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(420,1000,550,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(445,1025,"Signature 3"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(620,1000,750,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(645,1025,"Signature 4"); //should be come employee ID
            //</Signature>

            //<footer>
            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1080,800,1080);

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(25,1095,"Company Laws & Claws");

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1105,800,1105);
            //</footer>

        }
        painter.end();

    }

    void print_distribution_receive(QStringList lst,QTableView *tv){
        QPrinter printer;
        printer.setPageSize(QPrinter::A4);
        printer.setFullPage(true);
        printer.setOrientation(QPrinter::Portrait);

        //Print Dialog---------------------------------------
        QPrintDialog *dialog = new QPrintDialog(&printer);
        dialog->setWindowTitle("Print Document, SPL");
        if (dialog->exec() != QDialog::Accepted){}

        QPainter painter;
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
        painter.begin(&printer);

        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        //painter.drawLine(start dot, end dot);
        painter.drawLine(0,15,900,15);
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,20,900,20);

        QPixmap logo(":/new/prefix1/images/logo_small.png");
        painter.drawPixmap(5,30,logo);

        painter.setPen(QPen(Qt::black));
        painter.setFont(QFont("Arial", 22));
        painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

        painter.setFont(QFont("Arial",11));
        painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

        painter.setFont(QFont("Verdana",11));
        painter.drawText(600,125,"Receive Invoice");

        painter.setFont(QFont("Arial",11));
        painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
        painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,130,900,130);
        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,135,900,135);

        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.setFont(QFont("Arial",10));

        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,158,900,158);

        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,162,900,162);
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

        //<Invoice Information>
        painter.drawText(5,150, "Form No:");
        painter.drawText(65,150, lst.value(0));  //lineEdit to be replaced

        painter.drawText(190,150,"Effective Date:");
        painter.drawText(280,150,lst.value(1));  //Server Time


        //painter.drawText(400,150,"Payment:");
        // painter.drawText(460,150,lst.value(2));  //Cash,DBBL etc.

        painter.drawText(600,150,"Vat Reg No:");
        painter.drawText(672,150,lst.value(2));  //Vat Registration Number
        //</Invoice Information>

        //<Agent Data>
        //-- Left hand Side
        painter.drawText(25,180,"Receive By: ");
        painter.drawText(130,180,lst.value(3));  //Agent ID

        painter.drawText(25,200,"Emp Name: ");
        painter.drawText(130,200,lst.value(4));  //Customer Name

        // painter.drawText(25,220,"Address: ");
        //  painter.drawText(130,220,lst.value(6));

        // painter.drawText(25,240,"Transaction : ");
        // painter.drawText(130,240,lst.value(7));

        //-- Right hand Side
        painter.drawText(400,180,"Chalan No: ");
        painter.drawText(510,180,lst.value(5));

        painter.drawText(400,200,"Rec. Date: ");
        painter.drawText(510,200,lst.value(6));

        //painter.drawText(400,220,"Collec. Date: ");
        // painter.drawText(510,220,lst.value(10));

        // painter.drawText(400,240,"Sr Name: ");
        // painter.drawText(510,240,lst.value(11));
        //</Agent Data>

        painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,250,900,250);
        painter.drawLine(0,253,900,253);

        //data loop here-------------------------
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        int sz=tv->model()->columnCount(QModelIndex());

        int x_pix=15;

        for (int i=0;i<sz;i++){
            //a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"\t";
            if(i==2){
                painter.drawText(x_pix+50,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==1){
                painter.drawText(x_pix-30,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==3){
                painter.drawText(x_pix+20,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else{
                painter.drawText(x_pix,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            x_pix+=90;
        }
        // painter.drawLine(20,230,760,230);
        painter.drawLine(0,282,900,282);

        x_pix=15;
        int y_pix=295;

        QModelIndex index;
        int row=tv->model()->rowCount(QModelIndex());
        int col=tv->model()->columnCount(QModelIndex());


        for(int i=0;i<row;i++){
            x_pix=20;
            for(int j=0;j<col;j++){
                index=tv->model()->index(i,j,QModelIndex());

                if(j==2){
                    painter.drawText(x_pix+50,y_pix,tv->model()->data(index).toString());
                }
                else if(j==1){
                    painter.drawText(x_pix-30,y_pix,tv->model()->data(index).toString());
                }
                else if(j==3){
                    painter.drawText(x_pix+20,y_pix,tv->model()->data(index).toString());
                }
                else{
                    painter.drawText(x_pix,y_pix,tv->model()->data(index).toString());
                }
                x_pix+=90;
            }
            y_pix+=16;
            if(y_pix>printer.height()){
                printer.newPage();
                y_pix=215;
                painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                painter.begin(&printer);

                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                //painter.drawLine(start dot, end dot);
                painter.drawLine(0,15,800,15);
                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,20,800,20);

                QPixmap logo(":/new/prefix1/images/logo_small.pngg");
                painter.drawPixmap(5,30,logo);

                painter.setPen(QPen(Qt::black));
                painter.setFont(QFont("Arial", 22));
                painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                painter.setFont(QFont("Arial",11));
                painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                painter.setFont(QFont("Verdana",11));
                painter.drawText(600,125,"Receive Invoice");

                painter.setFont(QFont("Arial",11));
                painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,130,800,130);
                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,135,800,135);

                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.setFont(QFont("Arial",10));

                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,158,800,158);

                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,162,800,162);
                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                x_pix=15;

                for (int i=0;i<sz;i++){
                    //a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"\t";
                    if(i==1){
                        painter.drawText(x_pix+30,190,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else{
                        painter.drawText(x_pix,190,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    x_pix+=170;
                }
                painter.drawLine(0,202,900,202);

            }

            if(i==row-1){
                painter.drawLine(0,y_pix,900,y_pix);

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,900,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,900,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"Receive Invoice");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Total Quantity:");
                painter.drawText(545,y_pix,lst.value(7));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"Receive Invoice");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Total Amount:");
                painter.drawText(545,y_pix,lst.value(8));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"Receive Invoice");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }



                painter.drawLine(0,y_pix,800,y_pix);

            }
        }

        if(y_pix>printer.height()){
            printer.newPage();
            y_pix=202;
            painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
            painter.begin(&printer);

            painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
            //painter.drawLine(start dot, end dot);
            painter.drawLine(0,15,800,15);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,20,800,20);

            QPixmap logo(":/new/prefix1/images/logo_small.png");
            painter.drawPixmap(5,30,logo);

            painter.setPen(QPen(Qt::black));
            painter.setFont(QFont("Arial", 22));
            painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

            painter.setFont(QFont("Arial",11));
            painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

            painter.setFont(QFont("Verdana",11));
            painter.drawText(600,125,"Receive Invoice");

            painter.setFont(QFont("Arial",11));
            painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
            painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,130,800,130);
            painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,135,800,135);

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.setFont(QFont("Arial",10));

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,158,800,158);

            painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,162,800,162);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

            //<Signature>

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(20,1000,150,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(45,1025,"Prepared By"); //data should come from employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(220,1000,350,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(245,1025,"Checked By"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(420,1000,550,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(445,1025,"Authorized Sign"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(620,1000,750,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(645,1025,"Agent's Sign"); //should be come employee ID
            //</Signature>

            //<footer>
            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1080,800,1080);

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(25,1095,"Company Laws & Claws");

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1105,800,1105);
            //</footer>

        }
        else{

            //<Signature>

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(20,1000,150,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(45,1025,"Prepared By"); //data should come from employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(220,1000,350,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(245,1025,"Checked By"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(420,1000,550,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(445,1025,"Authorized Sign"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(620,1000,750,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(645,1025,"Agent's Sign"); //should be come employee ID
            //</Signature>

            //<footer>
            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1080,800,1080);

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(25,1095,"Company Laws & Claws");

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1105,800,1105);
            //</footer>

        }
        painter.end();

    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    void print_gatepass(QStringList lst,QTableView *tv){
        QPrinter printer;
        printer.setPageSize(QPrinter::A4);
        printer.setFullPage(true);
        printer.setOrientation(QPrinter::Portrait);
        QString filep=lst.value(0);
        printer.setOutputFileName(filep);

        //Print Dialog---------------------------------------
        QPrintDialog *dialog = new QPrintDialog(&printer);
        dialog->setWindowTitle("Print Document, SPL");
        if (dialog->exec() != QDialog::Accepted){}

        QPainter painter;
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
        painter.begin(&printer);

        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        //painter.drawLine(start dot, end dot);
        painter.drawLine(0,15,900,15);
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,20,900,20);

        QPixmap logo(":/new/prefix1/images/logo_small.png");
        painter.drawPixmap(5,30,logo);

        painter.setPen(QPen(Qt::black));
        painter.setFont(QFont("Arial", 22));
        painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

        painter.setFont(QFont("Arial",11));
        painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

        painter.setFont(QFont("Verdana",11));
        painter.drawText(600,125,"GATE PASS");

        painter.setFont(QFont("Arial",11));
        painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
        painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,130,900,130);
        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,135,900,135);

        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.setFont(QFont("Arial",10));

        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,158,900,158);

        painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,162,900,162);
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

        //<Invoice Information>
        // painter.drawText(5,150, "Form No:");
        // painter.drawText(65,150, lst.value(0));  //lineEdit to be replaced

        painter.drawText(190,150,"Effective Date:");
        painter.drawText(280,150,lst.value(3));  //Server Time


        //painter.drawText(400,150,"Payment:");
        // painter.drawText(460,150,lst.value(2));  //Cash,DBBL etc.

        painter.drawText(600,150,"Vat Reg No:");
        painter.drawText(672,150,lst.value(4));  //Vat Registration Number
        //</Invoice Information>

        //<Agent Data>
        //-- Left hand Side
        // painter.drawText(25,180,"Agent ID: ");
        //painter.drawText(130,180,lst.value(4));  //Agent ID

        //painter.drawText(25,200,"Agent Name: ");
        // painter.drawText(130,200,lst.value(5));  //Customer Name

        // painter.drawText(25,220,"Address: ");
        // painter.drawText(130,220,lst.value(6));

        // painter.drawText(25,240,"Transaction : ");
        // painter.drawText(130,240,lst.value(7));

        //-- Right hand Side
        //painter.drawText(400,180,"Invoice: ");
        // painter.drawText(510,180,lst.value(8));

        //  painter.drawText(400,200,"Inv. Date: ");
        // painter.drawText(510,200,lst.value(9));

        //  painter.drawText(400,220,"Collec. Date: ");
        //   painter.drawText(510,220,lst.value(10));

        // painter.drawText(400,240,"Sr Name: ");
        // painter.drawText(510,240,lst.value(11));


        painter.drawText(20,180,"The Following Products are being despatched by ____________ Through ____________ from Supreme Pharmaceuticals/HO");
        // painter.drawText(25,200,"from Supreme Pharmaceuticals Ltd.");

        //</Agent Data>

        painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
        painter.drawLine(0,250,900,250);
        painter.drawLine(0,253,900,253);

        //data loop here-------------------------
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        int sz=tv->model()->columnCount(QModelIndex());

        int x_pix=15;

        painter.setFont(QFont("Arial",8));

        for (int i=0;i<sz;i++){
            //a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"\t";
            if(i==3){
                painter.drawText(x_pix+40,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==4){
                painter.drawText(x_pix+30,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==5){
                painter.drawText(x_pix+45,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==2){
                painter.drawText(x_pix-40,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==8){
                painter.drawText(x_pix+100,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==6 ){
                painter.drawText(x_pix+25,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==1){
                painter.drawText(x_pix-35,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==7){
                painter.drawText(x_pix+100,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else{
                painter.drawText(x_pix,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            x_pix+=70;
        }


        /*
        for (int i=0;i<sz;i++)
        {
            if(i==0)
            {
               // painter.drawText(x_pix+0,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==1)
            {
                painter.drawText(x_pix+=25,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==2)
            {
                painter.drawText(x_pix+=25,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==3)
            {
                painter.drawText(x_pix+=250,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==4)
            {
                painter.drawText(x_pix+=100,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==5)
            {
                painter.drawText(x_pix+=170,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==6)
            {
                painter.drawText(x_pix+=250,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==7)
            {
                painter.drawText(x_pix+=25,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==8)
            {
                painter.drawText(x_pix+=25,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }

        }

            //a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"\t";



            if(i==3){
                painter.drawText(x_pix+450,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==4){
                painter.drawText(x_pix+30,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==5){
                painter.drawText(x_pix+35,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==2){
                painter.drawText(x_pix+100,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==8){
                painter.drawText(x_pix-35,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==6 ){
                painter.drawText(x_pix-20,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==1){
                painter.drawText(x_pix+25,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else if(i==7){
                painter.drawText(x_pix+40,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            else{
                painter.drawText(x_pix,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            }
            //x_pix+=70;
            */

        // painter.drawLine(20,230,760,230);
        painter.drawLine(0,282,900,282);

        x_pix=15;
        int y_pix=295;

        QModelIndex index;
        int row=tv->model()->rowCount(QModelIndex());
        int col=tv->model()->columnCount(QModelIndex());


        for(int i=0;i<row;i++){
            x_pix=15;
            for(int j=0;j<col;j++){
                index=tv->model()->index(i,j,QModelIndex());

                if(j==3){
                    painter.drawText(x_pix+40,y_pix,tv->model()->data(index).toString());
                }
                else if(j==4){
                    painter.drawText(x_pix+30,y_pix,tv->model()->data(index).toString());
                }
                else if(j==5){
                    painter.drawText(x_pix+45,y_pix,tv->model()->data(index).toString());
                }
                else if( j==2){
                    painter.drawText(x_pix-40,y_pix,tv->model()->data(index).toString());
                }
                else if(j==8){
                    painter.drawText(x_pix+100,y_pix,tv->model()->data(index).toString());
                }
                else if(j==6){
                    painter.drawText(x_pix+25,y_pix,tv->model()->data(index).toString());
                }
                else if(j==1){
                    painter.drawText(x_pix-35,y_pix,tv->model()->data(index).toString());
                }

                else if(j==7){
                    painter.drawText(x_pix+100,y_pix,tv->model()->data(index).toString());
                }
                else{
                    painter.drawText(x_pix,y_pix,tv->model()->data(index).toString());
                }
                x_pix+=70;
            }
            y_pix+=16;
            if(y_pix>printer.height()){
                printer.newPage();
                y_pix=215;
                painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                painter.begin(&printer);

                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                //painter.drawLine(start dot, end dot);
                painter.drawLine(0,15,800,15);
                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,20,800,20);

                QPixmap logo(":/new/prefix1/images/logo_small.pngg");
                painter.drawPixmap(5,30,logo);

                painter.setPen(QPen(Qt::black));
                painter.setFont(QFont("Arial", 22));
                painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                painter.setFont(QFont("Arial",11));
                painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                painter.setFont(QFont("Verdana",11));
                painter.drawText(600,125,"CHALLAN & INVOICE");

                painter.setFont(QFont("Arial",11));
                painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,130,800,130);
                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,135,800,135);

                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.setFont(QFont("Arial",10));

                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,158,800,158);

                painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                painter.drawLine(0,162,800,162);
                painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                x_pix=15;

                painter.setFont(QFont("Arial",6));

                /* for (int i=0;i<sz;i++){
                    //a+=tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+"\t";
                    if(i==3){
                        painter.drawText(x_pix+35,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==4){
                        painter.drawText(x_pix+30,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==5){
                        painter.drawText(x_pix+35,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==2){
                        painter.drawText(x_pix-35,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==8){
                        painter.drawText(x_pix-35,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==6 ){
                        painter.drawText(x_pix-20,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==1){
                        painter.drawText(x_pix-35,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==7){
                        painter.drawText(x_pix+40,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else{
                        painter.drawText(x_pix,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    x_pix+=70;
                }*/
                for (int i=0;i<sz;i++)
                {
                    if(i==0)
                    {
                        painter.drawText(x_pix+0,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==1)
                    {
                        painter.drawText(x_pix+=25,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==2)
                    {
                        painter.drawText(x_pix+=25,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==3)
                    {
                        painter.drawText(x_pix+=250,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==4)
                    {
                        painter.drawText(x_pix+=100,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==5)
                    {
                        painter.drawText(x_pix+=170,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==6)
                    {
                        painter.drawText(x_pix+=250,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==7)
                    {
                        painter.drawText(x_pix+=25,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }
                    else if(i==8)
                    {
                        painter.drawText(x_pix+=25,275,tv->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
                    }

                }

                painter.drawLine(0,202,900,202);

            }

            painter.setFont(QFont("Arial",10));

            if(i==row-1){
                painter.drawLine(0,y_pix,900,y_pix);

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,900,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,900,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"GATE PASS");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Grant Total:");
                painter.drawText(545,y_pix,lst.value(1));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"GATE PASS");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawText(445,y_pix,"Total Carton:");
                painter.drawText(545,y_pix,lst.value(2));

                y_pix+=20;
                if(y_pix>printer.height()-140){
                    printer.newPage();
                    y_pix=202;
                    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
                    painter.begin(&printer);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    //painter.drawLine(start dot, end dot);
                    painter.drawLine(0,15,800,15);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,20,800,20);

                    QPixmap logo(":/new/prefix1/images/logo_small.png");
                    painter.drawPixmap(5,30,logo);

                    painter.setPen(QPen(Qt::black));
                    painter.setFont(QFont("Arial", 22));
                    painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

                    painter.setFont(QFont("Verdana",11));
                    painter.drawText(600,125,"GATE PASS");

                    painter.setFont(QFont("Arial",11));
                    painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
                    painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,130,800,130);
                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,135,800,135);

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.setFont(QFont("Arial",10));

                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,158,800,158);

                    painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
                    painter.drawLine(0,162,800,162);
                    painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

                }


                painter.drawLine(0,y_pix,800,y_pix);

            }
        }

        if(y_pix>printer.height()){
            printer.newPage();
            y_pix=202;
            painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
            painter.begin(&printer);

            painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
            //painter.drawLine(start dot, end dot);
            painter.drawLine(0,15,800,15);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,20,800,20);

            QPixmap logo(":/new/prefix1/images/logo_small.png");
            painter.drawPixmap(5,30,logo);

            painter.setPen(QPen(Qt::black));
            painter.setFont(QFont("Arial", 22));
            painter.drawText(190,50,"Supreme Pharmaceuticals Ltd.");

            painter.setFont(QFont("Arial",11));
            painter.drawText(190,75, "Dhaka Sales Center. House No. 6/7, Block E, Lalmatia, Dhaka-1209");

            painter.setFont(QFont("Verdana",11));
            painter.drawText(600,125,"GATE PASS");

            painter.setFont(QFont("Arial",11));
            painter.drawText(190,93, "Telephone & Fax : 88-02-8116721, Email: spl@supreme.com.bd");
            painter.drawText(190,112,"Factory : Palash Bari, Savar, Dhaka, Phone: 77022482-3");

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,130,800,130);
            painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,135,800,135);

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.setFont(QFont("Arial",10));

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,158,800,158);

            painter.setPen(QPen(Qt::red,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,162,800,162);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));

            //<Signature>

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(20,1000,150,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(45,1025,"Prepared By"); //data should come from employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(220,1000,350,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(245,1025,"Distribution Dept"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(420,1000,550,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(445,1025,"Deliver Man"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(620,1000,750,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(645,1025,"Administration"); //should be come employee ID
            //</Signature>

            //<footer>
            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1080,800,1080);

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(25,1095,"Company Laws & Claws");

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1105,800,1105);
            //</footer>

        }
        else{

            //<Signature>

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(20,1000,150,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(45,1025,"Prepared By"); //data should come from employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(220,1000,350,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(245,1025,"Distribution Dept"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(420,1000,550,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(445,1025,"Deliver Man"); //should be come employee ID

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(620,1000,750,1000);
            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(645,1025,"Administration"); //should be come employee ID
            //</Signature>

            //<footer>
            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1080,800,1080);

            painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
            painter.drawText(25,1095,"Company Laws & Claws");

            painter.setPen(QPen(Qt::gray,1,Qt::DotLine,Qt::RoundCap));
            painter.drawLine(0,1105,800,1105);
            //</footer>

        }
        painter.end();

    }

};


#endif // DBSCLASS_H
