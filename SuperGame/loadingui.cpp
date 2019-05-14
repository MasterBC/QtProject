#include "loadingui.h"
#include "ui_loadingui.h"

LoadingUI::LoadingUI(QWidget *parent) :
    UIbase(parent),
    ui(new Ui::LoadingUI)
{
    ui->setupUi(this);
}

LoadingUI::~LoadingUI()
{
    delete ui;
}
