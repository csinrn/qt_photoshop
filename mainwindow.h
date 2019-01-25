#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "opencv2/opencv.hpp"
#include <QDebug>
#include <vector>
#include <QFileDialog>
#include "imageprocessor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::vector<cv::Mat> past;
    std::vector<cv::Mat> past_bc;
    std::vector<cv::Mat> past_cvt;
    int pastNum = 10;
    cv::Mat pic;
    cv::Mat origin;
    cv::Mat base_bc;
    cv::Mat base_cvt;
    std::string winName = "pic";

    unsigned long long B=0,G=0,R=0;
    float alpha=1, beta=0;
    ImageProcessor* imageP = nullptr;

    void update(cv::Mat newPic);
    //void filtering(int* filter);
    void filter(int Type);
    void channel(int type,float ratio);
    void setSlider();
    void sliderChanged(int channel);

signals:
    void doneVertices();

private slots:
    void on_Load_clicked();
    void on_Save_clicked();
    void on_Undo_clicked();
    void on_Clear_clicked();

    void on_Btn_kernelRun_clicked();

    void on_slider_b_valueChanged(int value);

    void on_slider_b_sliderPressed();

    void on_slider_g_valueChanged(int value);

    void on_slider_g_sliderPressed();

    void on_slider_r_valueChanged(int value);

    void on_slider_r_sliderPressed();

    void on_slider_brightness_valueChanged(int value);

    void on_slider_brightness_sliderPressed();

    void on_slider_contrast_valueChanged(int value);

    void on_slider_contrast_sliderPressed();

    void on_Btn_cvt_clicked();

    void on_Btn_findCorner_clicked();

    void on_Btn_persChange_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
