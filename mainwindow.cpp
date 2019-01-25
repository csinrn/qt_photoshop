#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionLoad_Image,SIGNAL(triggered()),this,SLOT(on_Load_clicked()));
    connect(ui->actionSave_Image,SIGNAL(triggered()),this,SLOT(on_Save_clicked()));
    connect(ui->actionUndo,SIGNAL(triggered()),this,SLOT(on_Undo_clicked()));
    connect(ui->actionClear,SIGNAL(triggered()),this,SLOT(on_Clear_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Load_clicked(){
    QString s = QFileDialog::getOpenFileName(this,tr("open"),"/", tr("Img(*.jpg *.png)"));
    if(s!= ""){
        namedWindow(winName,WINDOW_KEEPRATIO&WINDOW_FULLSCREEN&WINDOW_NORMAL);
        pic = imread(s.toStdString());
        origin = pic.clone();
        base_bc = origin.clone();
        base_cvt = origin.clone();
        past.clear();
        update(pic);
        setSlider();
        imshow(winName,pic);
    }
}

void MainWindow::on_Save_clicked(){
    QString s = QFileDialog::getSaveFileName(this,"save","/", tr("Img(*.jpg)"));
    if(s!= ""){
        imwrite(s.toStdString(),pic);
    }
}
void MainWindow::on_Undo_clicked(){
    if( !past.empty() ){
        qDebug()<<past.size()<<"un";
        pic = past.back().clone();
        base_bc = past_bc.back().clone();
        base_cvt = past_cvt.back().clone();

        past.pop_back();
        past_bc.pop_back();
        past_cvt.pop_back();

        imshow(winName,pic);
    }
}

void MainWindow::on_Clear_clicked(){
    past.clear();
    pic = origin.clone();
    base_bc = origin.clone();
    base_cvt = origin.clone();
    alpha = 1;
    beta = 0;
    setSlider();
    update(pic);
}

void MainWindow::update(Mat newPic){
    if(past.size() >= pastNum){
        past.erase(past.begin());
        past_bc.erase(past_bc.begin());
        past_cvt.erase(past_cvt.begin());
    }
    past.push_back(pic.clone());
    past_bc.push_back(base_bc.clone());
    past_cvt.push_back(base_cvt.clone());

    pic = newPic.clone();

    qDebug()<<"up";
    imshow(winName,pic);
}

void MainWindow::filter(int type){
    Size kernel = Size(ui->spinBox->value(),ui->spinBox->value());
    Mat dst;

    if(type == 0){  // average
        blur(pic,dst,kernel);
    }else if(type == 1){  //gaussian
        GaussianBlur(pic,dst,kernel,0);
    }else if(type == 2){  // median
        medianBlur(pic,dst,ui->spinBox->value());
    }else if(type == 3){  // laplace
        Mat kernel = (Mat_<float>(3,3) << 0,-1,0,-1,5,-1,0,-1,0);
        filter2D(pic,dst,pic.depth(),kernel);
    }
    update(dst);
}

void MainWindow::on_Btn_kernelRun_clicked()
{
    if(ui->radio_average->isChecked()){
        filter(0);
    }else if(ui->radio_gaussian->isChecked()){
        filter(1);
    }else if(ui->radio_median->isChecked()){
        filter(2);
    }else if(ui->radio_laplace->isChecked()){
        filter(3);
    }
}

void MainWindow::setSlider()
{
    Mat_<Vec3b>::iterator it = pic.begin<Vec3b>();
    Mat_<Vec3b>::iterator itend = pic.end<Vec3b>();
    for(;it!=itend;it++){
        B += (*it)[0];
        G += (*it)[1];
        R += (*it)[2];
    }
    B/= pic.cols*pic.rows;
    G/=pic.cols*pic.rows;
    R/=pic.cols*pic.rows;

    qDebug()<<B<<G<<R;
    ui->slider_b->setValue(B);
    ui->slider_g->setValue(G);
    ui->slider_r->setValue(R);
    ui->slider_brightness->setValue(0);
    ui->slider_contrast->setValue(0);
}

void MainWindow::sliderChanged(int channel){
    int val;
    if(channel == 0)
        val = ui->slider_b->value() - B;
    else if(channel == 1)
        val = ui->slider_g->value() - G;
    else
        val = ui->slider_r->value() - R;

    pic = origin.clone();
    for(int i=0;i<pic.rows;i++){
        for(int j=0;j<pic.cols;j++){
            int value = origin.at<Vec3b>(i,j)[channel] + val;
            if(value >= 255)
                pic.at<Vec3b>(i,j)[channel] =255;
            else if(value <=0)
                pic.at<Vec3b>(i,j)[channel] =0;
            else
                pic.at<Vec3b>(i,j)[channel] = value;
        }
    }
    base_bc = pic.clone();
    pic.convertTo(pic,-1,alpha,beta);
    base_cvt = pic.clone();
    imshow(winName,pic);
}

void MainWindow::on_slider_b_valueChanged(int value)
{
    sliderChanged(0);
}

void MainWindow::on_slider_b_sliderPressed()
{
    update(pic);
}

void MainWindow::on_slider_g_valueChanged(int value)
{
    sliderChanged(1);
}

void MainWindow::on_slider_g_sliderPressed()
{
    update(pic);
}

void MainWindow::on_slider_r_valueChanged(int value)
{
    sliderChanged(2);
}

void MainWindow::on_slider_r_sliderPressed()
{
    update(pic);
}

void MainWindow::on_slider_brightness_valueChanged(int value)
{
    beta = value;
    base_bc.convertTo(pic,-1,alpha,beta);
    base_cvt = pic.clone();
    imshow(winName,pic);
}

void MainWindow::on_slider_brightness_sliderPressed()
{
    update(pic);
}

void MainWindow::on_slider_contrast_valueChanged(int value)
{
    alpha = 1 + (float)value/10;
    base_bc.convertTo(pic,-1,alpha,beta);
    base_cvt = pic.clone();
    imshow(winName,pic);
}

void MainWindow::on_slider_contrast_sliderPressed()
{
    update(pic);
}

void MainWindow::on_Btn_cvt_clicked()
{
    update(pic);
    if(ui->radio_cvtHSV->isChecked()){
        cvtColor(base_cvt,pic,CV_BGR2HSV);
    }else if(ui->radio_cvtHSL->isChecked()){
        cvtColor(base_cvt,pic,CV_BGR2HLS);
    }else if(ui->radio_cvtGray->isChecked()){
        cvtColor(base_cvt,pic,CV_BGR2GRAY);
    }else if(ui->radio_cvtYCrCb->isChecked()){
        cvtColor(base_cvt,pic,CV_BGR2YCrCb);
    }
    imshow(winName,pic);
}

void MainWindow::on_Btn_findCorner_clicked()
{
    if(imageP != nullptr){
        delete imageP;
        imageP = nullptr;
    }
    imageP = new ImageProcessor();
    connect(this,SIGNAL(doneVertices()),imageP,SLOT(doneModify()));
    imageP->run(pic);
}

void MainWindow::on_Btn_persChange_clicked()
{
    emit doneVertices();
    pic = imageP->final.clone();
    imshow(winName,pic);
    delete imageP;
    imageP = nullptr;
}
