#include "controlsbox.h"
#include "frameview_widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
ControlsBox::ControlsBox(QTabWidget *tw, QWidget *parent) :
    QGroupBox(parent)
{
    cur_frameview=NULL;
    qtw = tw;
    //Collection Buttons


    run_collect_button.setText("Run Collect");
    run_display_button.setText("Run Display");
    stop_collect_button.setText("Stop Collect");
    stop_display_button.setText("Stop Display");

    collect_dark_frames_button.setText("Collect Dark Frames");
    stop_dark_collection_button.setText("Stop Dark Collection");

    load_mask_from_file.setText("Load Mask From File");
    fps_label.setText("Warning: no data recieved");

    //First Row
    collections_layout.addWidget(&run_display_button,1,1,1,1);
    //collections_layout->addWidget(&run_display_button,1,2);
    collections_layout.addWidget(&stop_display_button,1,2,1,1);
    //collections_layout->addWidget(&stop_display_button,1,4);

    //Second Row
    collections_layout.addWidget(&collect_dark_frames_button,2,1,1,1);
    collections_layout.addWidget(&stop_dark_collection_button,2,2,1,1);
    collections_layout.addWidget(&load_mask_from_file,3,2,1,1);

    //Third Row
    collections_layout.addWidget(&fps_label,3,1,1,1);

    CollectionButtonsBox.setLayout(&collections_layout);

    //Slider Thresholding Buttons
    //QGridLayout * sliders_layout = new QGridLayout();
    std_dev_N_slider.setOrientation(Qt::Horizontal);
    std_dev_N_slider.setMinimum(1);
    std_dev_N_slider.setMaximum(MAX_N-1);
    std_dev_N_slider.setValue(std_dev_N_slider.maximum());


    ceiling_slider.setOrientation(Qt::Horizontal);
    ceiling_slider.setMaximum(BIG_MAX);
    ceiling_slider.setMinimum(BIG_MIN);

    floor_slider.setOrientation(Qt::Horizontal);
    floor_slider.setMaximum(BIG_MAX);
    floor_slider.setMinimum(BIG_MIN);

    std_dev_N_edit.setMinimum(1);
    std_dev_N_edit.setMaximum(MAX_N-1);
    std_dev_N_edit.setButtonSymbols(QAbstractSpinBox::NoButtons);
    std_dev_N_edit.setValue(std_dev_N_edit.maximum());

    ceiling_edit.setMaximum(BIG_MAX);
    ceiling_edit.setMinimum(BIG_MIN);
    ceiling_edit.setButtonSymbols(QAbstractSpinBox::NoButtons);

    floor_edit.setMaximum(BIG_MAX);
    floor_edit.setMinimum(BIG_MIN);
    floor_edit.setButtonSymbols(QAbstractSpinBox::NoButtons);

    low_increment_cbox.setText("Slider Low Increment?");
    ceiling_slider.setTickInterval(BIG_TICK);
    floor_slider.setTickInterval(BIG_TICK);

    //First Row
    sliders_layout.addWidget(new QLabel("Std. Dev. N:"),1,1,1,1);
    sliders_layout.addWidget(&std_dev_N_slider,1,2,1,7);
    sliders_layout.addWidget(&std_dev_N_edit,1,10,1,1);
    //Second Row
    sliders_layout.addWidget(&low_increment_cbox,2,1,1,1);

    //Third Row
    sliders_layout.addWidget(new QLabel("Ceiling:"),3,1,1,1);
    sliders_layout.addWidget(&ceiling_slider,3,2,1,7);
    sliders_layout.addWidget(&ceiling_edit,3,10,1,1);

    //Fourth Row
    sliders_layout.addWidget(new QLabel("Floor:"),4,1,1,1);
    sliders_layout.addWidget(&floor_slider,4,2,1,7);
    sliders_layout.addWidget(&floor_edit,4,10,1,1);
    ThresholdingSlidersBox.setLayout(&sliders_layout);

    //Save Buttons

    save_frame_button.setText("Save Frame");
    save_dark_button.setText("Save Dark");

    save_frames_button.setText("Save Frames");
    stop_saving_frames_button.setText("Stop Saving");
    frames_save_num_edit.setButtonSymbols(QAbstractSpinBox::NoButtons);

/*
    QGridLayout * save_layout = new QGridLayout();
    QGroupBox * single_save_box = new QGroupBox();
    QVBoxLayout * single_save_layout = new QVBoxLayout;
    */


    single_save_layout.addWidget(new QLabel("Single Save"));
    single_save_layout.addWidget(&save_frame_button);
    single_save_layout.addWidget(&save_dark_button);

    single_save_box.setLayout(&single_save_layout);


    save_layout.addWidget(&single_save_box,1,1,3,1);

    //Column 2
    save_layout.addWidget(&save_frames_button,1,2,1,1);
    save_layout.addWidget(new QLabel("Frames to save:"),2,2,1,1);
    save_layout.addWidget(new QLabel("Filename:"),3,2,1,1);

    //Column 3
    save_layout.addWidget(&stop_saving_frames_button,1,3,1,1);
    save_layout.addWidget(&frames_save_num_edit,2,3,1,1);
    save_layout.addWidget(&filename_edit,3,3,1,1);
    SaveButtonsBox.setLayout(&save_layout);

    controls_layout.addWidget(&CollectionButtonsBox,2);
    controls_layout.addWidget(&ThresholdingSlidersBox,3);
    controls_layout.addWidget(&SaveButtonsBox,2);
    this->setLayout(&controls_layout);
    this->setMaximumHeight(150);

    connect(&std_dev_N_edit,SIGNAL(valueChanged(int)),&std_dev_N_slider,SLOT(setValue(int)));
    connect(&std_dev_N_slider,SIGNAL(valueChanged(int)),&std_dev_N_edit,SLOT(setValue(int)));

    connect(&ceiling_edit,SIGNAL(valueChanged(int)),&ceiling_slider,SLOT(setValue(int)));
    connect(&ceiling_slider,SIGNAL(valueChanged(int)),&ceiling_edit,SLOT(setValue(int)));

    connect(&floor_edit,SIGNAL(valueChanged(int)),&floor_slider,SLOT(setValue(int)));
    connect(&floor_slider,SIGNAL(valueChanged(int)),&floor_edit,SLOT(setValue(int)));

    connect(&load_mask_from_file,SIGNAL(clicked()),this,SLOT(getMaskFile()));
    connect(&save_frames_button,SIGNAL(clicked()),this,SLOT(save_button_slot()));

    connect(&low_increment_cbox,SIGNAL(toggled(bool)),this,SLOT(increment_slot(bool)));
}
void ControlsBox::getMaskFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select mask file"),
                                                    "",
                                                    tr("Files (*.raw)"));
    if(fileName.isEmpty())
    {
        return;
    }
    std::string utf8_text = fileName.toUtf8().constData();
    emit mask_selected(utf8_text.c_str());

}
void ControlsBox::increment_slot(bool t)
{
    if(t)
    {
        ceiling_slider.setTickInterval(LIL_TICK);
        floor_slider.setTickInterval(LIL_TICK);
        ceiling_slider.setMaximum(LIL_MAX);
        ceiling_slider.setMinimum(LIL_MIN);
        floor_slider.setMaximum(LIL_MAX);
        floor_slider.setMinimum(LIL_MIN);
    }
    else
    {
        ceiling_slider.setTickInterval(BIG_TICK);
        floor_slider.setTickInterval(BIG_TICK);
        ceiling_slider.setMaximum(BIG_MAX);
        ceiling_slider.setMinimum(BIG_MIN);
        floor_slider.setMaximum(BIG_MAX);
        floor_slider.setMinimum(BIG_MIN);
    }
}

void ControlsBox::save_button_slot()
{
    emit startSaving(filename_edit.text().toLocal8Bit().data());
}
void ControlsBox::tabChangedSlot(int index)
{
    frameview_widget * fvw = qobject_cast<frameview_widget*>(qtw->widget(index));
    if(cur_frameview != NULL)
    {
        disconnect(&ceiling_slider, SIGNAL(valueChanged(int)), cur_frameview, SLOT(updateCeiling(int)));
        disconnect(&floor_slider, SIGNAL(valueChanged(int)), cur_frameview, SLOT(updateFloor(int)));
    }
    cur_frameview = fvw; //Gets set to null if not frameview widget

    if(cur_frameview != NULL)
    {
        ceiling_edit.setValue(cur_frameview->getCeiling());
        floor_edit.setValue(cur_frameview->getFloor());
        connect(&ceiling_slider, SIGNAL(valueChanged(int)), cur_frameview, SLOT(updateCeiling(int)));
        connect(&floor_slider, SIGNAL(valueChanged(int)), cur_frameview, SLOT(updateFloor(int)));
    }
}
