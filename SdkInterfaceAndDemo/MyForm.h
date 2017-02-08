#pragma once
//opencv 
#include "opencv2/opencv.hpp"

namespace ProjectInterface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm 摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO:  在此处添加构造函数代码
			//
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;


	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;











































	private: System::Windows::Forms::PictureBox^  pictureBoxMap;




















































private: System::Windows::Forms::Panel^  panel9;

private: System::Windows::Forms::StatusStrip^  statusStrip1;

private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel2;
private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel3;









































































private: System::Windows::Forms::Timer^  send_speed_timer;
private: System::Windows::Forms::Timer^  MainTimer;
private: System::Windows::Forms::TabControl^  tabControl2;
private: System::Windows::Forms::TabPage^  tp_comFunc;
private: System::Windows::Forms::Button^  btn_update_map_flag;
private: System::Windows::Forms::CheckBox^  CB_ONLY_MPU;
private: System::Windows::Forms::Button^  BTN_UnifiedSensor;
private: System::Windows::Forms::CheckBox^  CB_UseTcp;
private: System::Windows::Forms::Button^  button_IMU_calibrate;
private: System::Windows::Forms::Button^  button_video_record;
private: System::Windows::Forms::Button^  button1;
private: System::Windows::Forms::CheckBox^  cb_connectMpu;
private: System::Windows::Forms::Button^  back_charge;
private: System::Windows::Forms::Button^  button_start_build_map_auto;
private: System::Windows::Forms::Button^  button_init_map;
private: System::Windows::Forms::Button^  button_stop_build_map;
private: System::Windows::Forms::Button^  button_start_build_map;
private: System::Windows::Forms::TrackBar^  trackBar1;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Button^  set_pic_show_scale;
private: System::Windows::Forms::TextBox^  pic_scale;
private: System::Windows::Forms::TextBox^  tb_ip;
private: System::Windows::Forms::TextBox^  tb_port;
private: System::Windows::Forms::Button^  button_connect;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::TabPage^  tp_directionCtrl;
private: System::Windows::Forms::Panel^  panel11;
private: System::Windows::Forms::Label^  label40;
private: System::Windows::Forms::Label^  label41;
private: System::Windows::Forms::TextBox^  textBox_roll_step;
private: System::Windows::Forms::TextBox^  textBox_head_roll_control;

private: System::Windows::Forms::Label^  label35;
private: System::Windows::Forms::Label^  label34;
private: System::Windows::Forms::Label^  label32;
private: System::Windows::Forms::Label^  label33;
private: System::Windows::Forms::Label^  label31;
private: System::Windows::Forms::TextBox^  textBox_yaw_step;
private: System::Windows::Forms::TextBox^  textBox_v_step;
private: System::Windows::Forms::TextBox^  textBox_pitch_step;
private: System::Windows::Forms::TextBox^  textBox_w_step;
private: System::Windows::Forms::Panel^  panel8;
private: System::Windows::Forms::Label^  label14;
private: System::Windows::Forms::Panel^  panel6;
private: System::Windows::Forms::Label^  label12;
private: System::Windows::Forms::Panel^  panel5;
private: System::Windows::Forms::Label^  label9;
private: System::Windows::Forms::Panel^  panel7;
private: System::Windows::Forms::Label^  label13;

private: System::Windows::Forms::TextBox^  textBox_key;
	private: System::ComponentModel::IContainer^  components;
	protected:

	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->panel9 = (gcnew System::Windows::Forms::Panel());
			this->pictureBoxMap = (gcnew System::Windows::Forms::PictureBox());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->tabControl2 = (gcnew System::Windows::Forms::TabControl());
			this->tp_comFunc = (gcnew System::Windows::Forms::TabPage());
			this->btn_update_map_flag = (gcnew System::Windows::Forms::Button());
			this->CB_ONLY_MPU = (gcnew System::Windows::Forms::CheckBox());
			this->BTN_UnifiedSensor = (gcnew System::Windows::Forms::Button());
			this->CB_UseTcp = (gcnew System::Windows::Forms::CheckBox());
			this->button_IMU_calibrate = (gcnew System::Windows::Forms::Button());
			this->button_video_record = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->cb_connectMpu = (gcnew System::Windows::Forms::CheckBox());
			this->back_charge = (gcnew System::Windows::Forms::Button());
			this->button_start_build_map_auto = (gcnew System::Windows::Forms::Button());
			this->button_init_map = (gcnew System::Windows::Forms::Button());
			this->button_stop_build_map = (gcnew System::Windows::Forms::Button());
			this->button_start_build_map = (gcnew System::Windows::Forms::Button());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->set_pic_show_scale = (gcnew System::Windows::Forms::Button());
			this->pic_scale = (gcnew System::Windows::Forms::TextBox());
			this->tb_ip = (gcnew System::Windows::Forms::TextBox());
			this->tb_port = (gcnew System::Windows::Forms::TextBox());
			this->button_connect = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tp_directionCtrl = (gcnew System::Windows::Forms::TabPage());
			this->panel11 = (gcnew System::Windows::Forms::Panel());
			this->label40 = (gcnew System::Windows::Forms::Label());
			this->label41 = (gcnew System::Windows::Forms::Label());
			this->textBox_roll_step = (gcnew System::Windows::Forms::TextBox());
			this->textBox_head_roll_control = (gcnew System::Windows::Forms::TextBox());
			this->label35 = (gcnew System::Windows::Forms::Label());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->textBox_yaw_step = (gcnew System::Windows::Forms::TextBox());
			this->textBox_v_step = (gcnew System::Windows::Forms::TextBox());
			this->textBox_pitch_step = (gcnew System::Windows::Forms::TextBox());
			this->textBox_w_step = (gcnew System::Windows::Forms::TextBox());
			this->panel8 = (gcnew System::Windows::Forms::Panel());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->panel6 = (gcnew System::Windows::Forms::Panel());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->panel5 = (gcnew System::Windows::Forms::Panel());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->panel7 = (gcnew System::Windows::Forms::Panel());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->textBox_key = (gcnew System::Windows::Forms::TextBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel3 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->send_speed_timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->MainTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->panel1->SuspendLayout();
			this->panel2->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->panel9->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxMap))->BeginInit();
			this->panel3->SuspendLayout();
			this->tabControl2->SuspendLayout();
			this->tp_comFunc->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			this->tp_directionCtrl->SuspendLayout();
			this->panel11->SuspendLayout();
			this->panel8->SuspendLayout();
			this->panel6->SuspendLayout();
			this->panel5->SuspendLayout();
			this->panel7->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panel1->Controls->Add(this->panel2);
			this->panel1->Controls->Add(this->panel3);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel1->Location = System::Drawing::Point(0, 25);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(1155, 616);
			this->panel1->TabIndex = 0;
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->tabControl1);
			this->panel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel2->Location = System::Drawing::Point(0, 0);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(871, 612);
			this->panel2->TabIndex = 0;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(871, 612);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->panel9);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(863, 586);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"tabPage1";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// panel9
			// 
			this->panel9->AutoScroll = true;
			this->panel9->Controls->Add(this->pictureBoxMap);
			this->panel9->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel9->Location = System::Drawing::Point(3, 3);
			this->panel9->Name = L"panel9";
			this->panel9->Size = System::Drawing::Size(857, 580);
			this->panel9->TabIndex = 1;
			// 
			// pictureBoxMap
			// 
			this->pictureBoxMap->Location = System::Drawing::Point(3, 9);
			this->pictureBoxMap->Name = L"pictureBoxMap";
			this->pictureBoxMap->Size = System::Drawing::Size(512, 512);
			this->pictureBoxMap->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBoxMap->TabIndex = 0;
			this->pictureBoxMap->TabStop = false;
			this->pictureBoxMap->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBoxMap_MouseDown);
			this->pictureBoxMap->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBoxMap_MouseMove);
			this->pictureBoxMap->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBoxMap_MouseUp);
			// 
			// tabPage2
			// 
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(863, 586);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"tabPage2";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// panel3
			// 
			this->panel3->Controls->Add(this->tabControl2);
			this->panel3->Dock = System::Windows::Forms::DockStyle::Right;
			this->panel3->Location = System::Drawing::Point(871, 0);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(280, 612);
			this->panel3->TabIndex = 1;
			// 
			// tabControl2
			// 
			this->tabControl2->Controls->Add(this->tp_comFunc);
			this->tabControl2->Controls->Add(this->tp_directionCtrl);
			this->tabControl2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl2->Font = (gcnew System::Drawing::Font(L"宋体", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->tabControl2->Location = System::Drawing::Point(0, 0);
			this->tabControl2->Name = L"tabControl2";
			this->tabControl2->SelectedIndex = 0;
			this->tabControl2->Size = System::Drawing::Size(280, 612);
			this->tabControl2->TabIndex = 0;
			// 
			// tp_comFunc
			// 
			this->tp_comFunc->Controls->Add(this->btn_update_map_flag);
			this->tp_comFunc->Controls->Add(this->CB_ONLY_MPU);
			this->tp_comFunc->Controls->Add(this->BTN_UnifiedSensor);
			this->tp_comFunc->Controls->Add(this->CB_UseTcp);
			this->tp_comFunc->Controls->Add(this->button_IMU_calibrate);
			this->tp_comFunc->Controls->Add(this->button_video_record);
			this->tp_comFunc->Controls->Add(this->button1);
			this->tp_comFunc->Controls->Add(this->cb_connectMpu);
			this->tp_comFunc->Controls->Add(this->back_charge);
			this->tp_comFunc->Controls->Add(this->button_start_build_map_auto);
			this->tp_comFunc->Controls->Add(this->button_init_map);
			this->tp_comFunc->Controls->Add(this->button_stop_build_map);
			this->tp_comFunc->Controls->Add(this->button_start_build_map);
			this->tp_comFunc->Controls->Add(this->trackBar1);
			this->tp_comFunc->Controls->Add(this->label5);
			this->tp_comFunc->Controls->Add(this->set_pic_show_scale);
			this->tp_comFunc->Controls->Add(this->pic_scale);
			this->tp_comFunc->Controls->Add(this->tb_ip);
			this->tp_comFunc->Controls->Add(this->tb_port);
			this->tp_comFunc->Controls->Add(this->button_connect);
			this->tp_comFunc->Controls->Add(this->label2);
			this->tp_comFunc->Controls->Add(this->label1);
			this->tp_comFunc->Location = System::Drawing::Point(4, 22);
			this->tp_comFunc->Name = L"tp_comFunc";
			this->tp_comFunc->Padding = System::Windows::Forms::Padding(3);
			this->tp_comFunc->Size = System::Drawing::Size(272, 586);
			this->tp_comFunc->TabIndex = 0;
			this->tp_comFunc->Text = L"NormalCtrl";
			this->tp_comFunc->UseVisualStyleBackColor = true;
			// 
			// btn_update_map_flag
			// 
			this->btn_update_map_flag->Location = System::Drawing::Point(19, 265);
			this->btn_update_map_flag->Name = L"btn_update_map_flag";
			this->btn_update_map_flag->Size = System::Drawing::Size(228, 23);
			this->btn_update_map_flag->TabIndex = 52;
			this->btn_update_map_flag->Text = L"Enable Map Upgrade";
			this->btn_update_map_flag->UseVisualStyleBackColor = true;
			this->btn_update_map_flag->Click += gcnew System::EventHandler(this, &MyForm::btn_update_map_flag_Click);
			// 
			// CB_ONLY_MPU
			// 
			this->CB_ONLY_MPU->AutoSize = true;
			this->CB_ONLY_MPU->Enabled = false;
			this->CB_ONLY_MPU->Location = System::Drawing::Point(100, 75);
			this->CB_ONLY_MPU->Name = L"CB_ONLY_MPU";
			this->CB_ONLY_MPU->Size = System::Drawing::Size(72, 16);
			this->CB_ONLY_MPU->TabIndex = 51;
			this->CB_ONLY_MPU->Text = L"ONLY_MPU";
			this->CB_ONLY_MPU->UseVisualStyleBackColor = true;
			// 
			// BTN_UnifiedSensor
			// 
			this->BTN_UnifiedSensor->Location = System::Drawing::Point(19, 441);
			this->BTN_UnifiedSensor->Name = L"BTN_UnifiedSensor";
			this->BTN_UnifiedSensor->Size = System::Drawing::Size(228, 23);
			this->BTN_UnifiedSensor->TabIndex = 50;
			this->BTN_UnifiedSensor->Text = L"Sensor Transfer By Yourself";
			this->BTN_UnifiedSensor->UseVisualStyleBackColor = true;
			this->BTN_UnifiedSensor->Click += gcnew System::EventHandler(this, &MyForm::BTN_UnifiedSensor_Click);
			// 
			// CB_UseTcp
			// 
			this->CB_UseTcp->AutoSize = true;
			this->CB_UseTcp->Checked = true;
			this->CB_UseTcp->CheckState = System::Windows::Forms::CheckState::Checked;
			this->CB_UseTcp->Location = System::Drawing::Point(201, 16);
			this->CB_UseTcp->Name = L"CB_UseTcp";
			this->CB_UseTcp->Size = System::Drawing::Size(66, 16);
			this->CB_UseTcp->TabIndex = 49;
			this->CB_UseTcp->Text = L"USE_TCP";
			this->CB_UseTcp->UseVisualStyleBackColor = true;
			this->CB_UseTcp->CheckedChanged += gcnew System::EventHandler(this, &MyForm::CB_UseTcp_CheckedChanged);
			// 
			// button_IMU_calibrate
			// 
			this->button_IMU_calibrate->Location = System::Drawing::Point(19, 383);
			this->button_IMU_calibrate->Name = L"button_IMU_calibrate";
			this->button_IMU_calibrate->Size = System::Drawing::Size(228, 23);
			this->button_IMU_calibrate->TabIndex = 48;
			this->button_IMU_calibrate->Text = L"IMU Calibrate";
			this->button_IMU_calibrate->UseVisualStyleBackColor = true;
			this->button_IMU_calibrate->Click += gcnew System::EventHandler(this, &MyForm::button_IMU_calibrate_Click);
			// 
			// button_video_record
			// 
			this->button_video_record->Location = System::Drawing::Point(19, 412);
			this->button_video_record->Name = L"button_video_record";
			this->button_video_record->Size = System::Drawing::Size(228, 23);
			this->button_video_record->TabIndex = 47;
			this->button_video_record->Text = L"Start Recording Video";
			this->button_video_record->UseVisualStyleBackColor = true;
			this->button_video_record->Click += gcnew System::EventHandler(this, &MyForm::button_video_record_Click);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(19, 323);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(228, 23);
			this->button1->TabIndex = 46;
			this->button1->Text = L"Update Navipack";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// cb_connectMpu
			// 
			this->cb_connectMpu->AutoSize = true;
			this->cb_connectMpu->Location = System::Drawing::Point(12, 75);
			this->cb_connectMpu->Name = L"cb_connectMpu";
			this->cb_connectMpu->Size = System::Drawing::Size(90, 16);
			this->cb_connectMpu->TabIndex = 45;
			this->cb_connectMpu->Text = L"Connect MPU";
			this->cb_connectMpu->UseVisualStyleBackColor = true;
			this->cb_connectMpu->CheckedChanged += gcnew System::EventHandler(this, &MyForm::cb_connectMpu_CheckedChanged);
			// 
			// back_charge
			// 
			this->back_charge->Location = System::Drawing::Point(19, 354);
			this->back_charge->Name = L"back_charge";
			this->back_charge->Size = System::Drawing::Size(228, 23);
			this->back_charge->TabIndex = 44;
			this->back_charge->Text = L"Back Charge";
			this->back_charge->UseVisualStyleBackColor = true;
			this->back_charge->Click += gcnew System::EventHandler(this, &MyForm::back_charge_Click);
			// 
			// button_start_build_map_auto
			// 
			this->button_start_build_map_auto->Location = System::Drawing::Point(19, 207);
			this->button_start_build_map_auto->Name = L"button_start_build_map_auto";
			this->button_start_build_map_auto->Size = System::Drawing::Size(228, 23);
			this->button_start_build_map_auto->TabIndex = 43;
			this->button_start_build_map_auto->Text = L"Auto BuildMap(NOT USE)";
			this->button_start_build_map_auto->UseVisualStyleBackColor = true;
			this->button_start_build_map_auto->Click += gcnew System::EventHandler(this, &MyForm::button_start_build_map_auto_Click);
			// 
			// button_init_map
			// 
			this->button_init_map->Location = System::Drawing::Point(19, 294);
			this->button_init_map->Name = L"button_init_map";
			this->button_init_map->Size = System::Drawing::Size(228, 23);
			this->button_init_map->TabIndex = 42;
			this->button_init_map->Text = L"Init Location";
			this->button_init_map->UseVisualStyleBackColor = true;
			this->button_init_map->Click += gcnew System::EventHandler(this, &MyForm::button_init_map_Click);
			// 
			// button_stop_build_map
			// 
			this->button_stop_build_map->Location = System::Drawing::Point(19, 236);
			this->button_stop_build_map->Name = L"button_stop_build_map";
			this->button_stop_build_map->Size = System::Drawing::Size(228, 23);
			this->button_stop_build_map->TabIndex = 41;
			this->button_stop_build_map->Text = L"Stop Building & Save Map";
			this->button_stop_build_map->UseVisualStyleBackColor = true;
			this->button_stop_build_map->Click += gcnew System::EventHandler(this, &MyForm::button_stop_build_map_Click);
			// 
			// button_start_build_map
			// 
			this->button_start_build_map->Location = System::Drawing::Point(19, 173);
			this->button_start_build_map->Name = L"button_start_build_map";
			this->button_start_build_map->Size = System::Drawing::Size(228, 23);
			this->button_start_build_map->TabIndex = 40;
			this->button_start_build_map->Text = L"Manual BuildMap";
			this->button_start_build_map->UseVisualStyleBackColor = true;
			this->button_start_build_map->Click += gcnew System::EventHandler(this, &MyForm::button_start_build_map_Click);
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(4, 103);
			this->trackBar1->Maximum = 100;
			this->trackBar1->Minimum = 1;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(268, 45);
			this->trackBar1->TabIndex = 39;
			this->trackBar1->Value = 1;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &MyForm::trackBar1_Scroll);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(10, 150);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(83, 12);
			this->label5->TabIndex = 38;
			this->label5->Text = L"Dispaly Ratio";
			// 
			// set_pic_show_scale
			// 
			this->set_pic_show_scale->Location = System::Drawing::Point(182, 144);
			this->set_pic_show_scale->Margin = System::Windows::Forms::Padding(2);
			this->set_pic_show_scale->Name = L"set_pic_show_scale";
			this->set_pic_show_scale->Size = System::Drawing::Size(90, 25);
			this->set_pic_show_scale->TabIndex = 36;
			this->set_pic_show_scale->Text = L" Set Scale";
			this->set_pic_show_scale->UseVisualStyleBackColor = true;
			this->set_pic_show_scale->Click += gcnew System::EventHandler(this, &MyForm::set_pic_show_scale_Click);
			// 
			// pic_scale
			// 
			this->pic_scale->Location = System::Drawing::Point(101, 147);
			this->pic_scale->Margin = System::Windows::Forms::Padding(2);
			this->pic_scale->Name = L"pic_scale";
			this->pic_scale->Size = System::Drawing::Size(67, 21);
			this->pic_scale->TabIndex = 37;
			this->pic_scale->Text = L"2";
			// 
			// tb_ip
			// 
			this->tb_ip->Location = System::Drawing::Point(96, 12);
			this->tb_ip->Margin = System::Windows::Forms::Padding(2);
			this->tb_ip->Name = L"tb_ip";
			this->tb_ip->Size = System::Drawing::Size(100, 21);
			this->tb_ip->TabIndex = 22;
			this->tb_ip->Text = L"192.168.17.1";
			// 
			// tb_port
			// 
			this->tb_port->Location = System::Drawing::Point(97, 43);
			this->tb_port->Margin = System::Windows::Forms::Padding(2);
			this->tb_port->Name = L"tb_port";
			this->tb_port->Size = System::Drawing::Size(100, 21);
			this->tb_port->TabIndex = 23;
			this->tb_port->Text = L"9977";
			// 
			// button_connect
			// 
			this->button_connect->Location = System::Drawing::Point(177, 75);
			this->button_connect->Margin = System::Windows::Forms::Padding(2);
			this->button_connect->Name = L"button_connect";
			this->button_connect->Size = System::Drawing::Size(90, 25);
			this->button_connect->TabIndex = 21;
			this->button_connect->Text = L"Connect";
			this->button_connect->UseVisualStyleBackColor = true;
			this->button_connect->Click += gcnew System::EventHandler(this, &MyForm::button_connect_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(17, 45);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(29, 12);
			this->label2->TabIndex = 25;
			this->label2->Text = L"Port";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(10, 16);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(95, 12);
			this->label1->TabIndex = 24;
			this->label1->Text = L"IP Address/Port";
			// 
			// tp_directionCtrl
			// 
			this->tp_directionCtrl->Controls->Add(this->panel11);
			this->tp_directionCtrl->Location = System::Drawing::Point(4, 22);
			this->tp_directionCtrl->Name = L"tp_directionCtrl";
			this->tp_directionCtrl->Padding = System::Windows::Forms::Padding(3);
			this->tp_directionCtrl->Size = System::Drawing::Size(272, 586);
			this->tp_directionCtrl->TabIndex = 1;
			this->tp_directionCtrl->Text = L"SpeedControl";
			this->tp_directionCtrl->UseVisualStyleBackColor = true;
			// 
			// panel11
			// 
			this->panel11->AccessibleName = L"";
			this->panel11->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel11->Controls->Add(this->label40);
			this->panel11->Controls->Add(this->label41);
			this->panel11->Controls->Add(this->textBox_roll_step);
			this->panel11->Controls->Add(this->textBox_head_roll_control);
			this->panel11->Controls->Add(this->label35);
			this->panel11->Controls->Add(this->label34);
			this->panel11->Controls->Add(this->label32);
			this->panel11->Controls->Add(this->label33);
			this->panel11->Controls->Add(this->label31);
			this->panel11->Controls->Add(this->textBox_yaw_step);
			this->panel11->Controls->Add(this->textBox_v_step);
			this->panel11->Controls->Add(this->textBox_pitch_step);
			this->panel11->Controls->Add(this->textBox_w_step);
			this->panel11->Controls->Add(this->panel8);
			this->panel11->Controls->Add(this->panel6);
			this->panel11->Controls->Add(this->panel5);
			this->panel11->Controls->Add(this->panel7);
			this->panel11->Controls->Add(this->textBox_key);
			this->panel11->Location = System::Drawing::Point(6, 25);
			this->panel11->Name = L"panel11";
			this->panel11->Size = System::Drawing::Size(262, 330);
			this->panel11->TabIndex = 50;
			// 
			// label40
			// 
			this->label40->AutoSize = true;
			this->label40->Location = System::Drawing::Point(17, 213);
			this->label40->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label40->Name = L"label40";
			this->label40->Size = System::Drawing::Size(89, 12);
			this->label40->TabIndex = 56;
			this->label40->Text = L"HeadCtrl(roll)";
			// 
			// label41
			// 
			this->label41->AutoSize = true;
			this->label41->Location = System::Drawing::Point(105, 213);
			this->label41->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label41->Name = L"label41";
			this->label41->Size = System::Drawing::Size(29, 12);
			this->label41->TabIndex = 55;
			this->label41->Text = L"roll";
			// 
			// textBox_roll_step
			// 
			this->textBox_roll_step->Location = System::Drawing::Point(107, 227);
			this->textBox_roll_step->Margin = System::Windows::Forms::Padding(2);
			this->textBox_roll_step->Name = L"textBox_roll_step";
			this->textBox_roll_step->Size = System::Drawing::Size(76, 21);
			this->textBox_roll_step->TabIndex = 54;
			this->textBox_roll_step->Text = L"10";
			// 
			// textBox_head_roll_control
			// 
			this->textBox_head_roll_control->Location = System::Drawing::Point(18, 227);
			this->textBox_head_roll_control->Margin = System::Windows::Forms::Padding(2);
			this->textBox_head_roll_control->Name = L"textBox_head_roll_control";
			this->textBox_head_roll_control->Size = System::Drawing::Size(76, 21);
			this->textBox_head_roll_control->TabIndex = 53;
			// 
			// label35
			// 
			this->label35->AutoSize = true;
			this->label35->Location = System::Drawing::Point(16, 24);
			this->label35->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(95, 12);
			this->label35->TabIndex = 52;
			this->label35->Text = L"Carrier Control";
			// 
			// label34
			// 
			this->label34->AutoSize = true;
			this->label34->Location = System::Drawing::Point(93, 142);
			this->label34->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(35, 12);
			this->label34->TabIndex = 51;
			this->label34->Text = L"pitch";
			// 
			// label32
			// 
			this->label32->AutoSize = true;
			this->label32->Location = System::Drawing::Point(93, 70);
			this->label32->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(41, 12);
			this->label32->TabIndex = 51;
			this->label32->Text = L"w_step";
			// 
			// label33
			// 
			this->label33->AutoSize = true;
			this->label33->Location = System::Drawing::Point(18, 142);
			this->label33->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(23, 12);
			this->label33->TabIndex = 50;
			this->label33->Text = L"yaw";
			// 
			// label31
			// 
			this->label31->AutoSize = true;
			this->label31->Location = System::Drawing::Point(18, 70);
			this->label31->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(41, 12);
			this->label31->TabIndex = 50;
			this->label31->Text = L"v_step";
			// 
			// textBox_yaw_step
			// 
			this->textBox_yaw_step->Location = System::Drawing::Point(17, 165);
			this->textBox_yaw_step->Margin = System::Windows::Forms::Padding(2);
			this->textBox_yaw_step->Name = L"textBox_yaw_step";
			this->textBox_yaw_step->Size = System::Drawing::Size(76, 21);
			this->textBox_yaw_step->TabIndex = 40;
			this->textBox_yaw_step->Text = L"100";
			// 
			// textBox_v_step
			// 
			this->textBox_v_step->Location = System::Drawing::Point(17, 93);
			this->textBox_v_step->Margin = System::Windows::Forms::Padding(2);
			this->textBox_v_step->Name = L"textBox_v_step";
			this->textBox_v_step->Size = System::Drawing::Size(76, 21);
			this->textBox_v_step->TabIndex = 40;
			this->textBox_v_step->Text = L"0.1";
			// 
			// textBox_pitch_step
			// 
			this->textBox_pitch_step->Location = System::Drawing::Point(104, 165);
			this->textBox_pitch_step->Margin = System::Windows::Forms::Padding(2);
			this->textBox_pitch_step->Name = L"textBox_pitch_step";
			this->textBox_pitch_step->Size = System::Drawing::Size(76, 21);
			this->textBox_pitch_step->TabIndex = 39;
			this->textBox_pitch_step->Text = L"100";
			// 
			// textBox_w_step
			// 
			this->textBox_w_step->Location = System::Drawing::Point(104, 93);
			this->textBox_w_step->Margin = System::Windows::Forms::Padding(2);
			this->textBox_w_step->Name = L"textBox_w_step";
			this->textBox_w_step->Size = System::Drawing::Size(76, 21);
			this->textBox_w_step->TabIndex = 39;
			this->textBox_w_step->Text = L"0.2";
			// 
			// panel8
			// 
			this->panel8->Controls->Add(this->label14);
			this->panel8->Location = System::Drawing::Point(220, 47);
			this->panel8->Name = L"panel8";
			this->panel8->Size = System::Drawing::Size(30, 30);
			this->panel8->TabIndex = 38;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Font = (gcnew System::Drawing::Font(L"宋体", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->label14->Location = System::Drawing::Point(4, 6);
			this->label14->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(22, 21);
			this->label14->TabIndex = 36;
			this->label14->Text = L"D";
			// 
			// panel6
			// 
			this->panel6->Controls->Add(this->label12);
			this->panel6->Location = System::Drawing::Point(185, 11);
			this->panel6->Name = L"panel6";
			this->panel6->Size = System::Drawing::Size(30, 30);
			this->panel6->TabIndex = 38;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"宋体", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->label12->Location = System::Drawing::Point(4, 5);
			this->label12->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(22, 21);
			this->label12->TabIndex = 36;
			this->label12->Text = L"W";
			// 
			// panel5
			// 
			this->panel5->Controls->Add(this->label9);
			this->panel5->Location = System::Drawing::Point(155, 47);
			this->panel5->Name = L"panel5";
			this->panel5->Size = System::Drawing::Size(30, 30);
			this->panel5->TabIndex = 38;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"宋体", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->label9->Location = System::Drawing::Point(4, 5);
			this->label9->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(22, 21);
			this->label9->TabIndex = 36;
			this->label9->Text = L"A";
			// 
			// panel7
			// 
			this->panel7->Controls->Add(this->label13);
			this->panel7->Location = System::Drawing::Point(189, 47);
			this->panel7->Name = L"panel7";
			this->panel7->Size = System::Drawing::Size(30, 30);
			this->panel7->TabIndex = 38;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"宋体", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->label13->Location = System::Drawing::Point(4, 5);
			this->label13->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(22, 21);
			this->label13->TabIndex = 36;
			this->label13->Text = L"S";
			// 
			// textBox_key
			// 
			this->textBox_key->Location = System::Drawing::Point(17, 47);
			this->textBox_key->Margin = System::Windows::Forms::Padding(2);
			this->textBox_key->Name = L"textBox_key";
			this->textBox_key->Size = System::Drawing::Size(76, 21);
			this->textBox_key->TabIndex = 33;
			this->textBox_key->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::textBox_key_KeyPress);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->fileToolStripMenuItem,
					this->editToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1155, 25);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 21);
			this->fileToolStripMenuItem->Text = L"file";
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(42, 21);
			this->editToolStripMenuItem->Text = L"edit";
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->toolStripStatusLabel1,
					this->toolStripStatusLabel2, this->toolStripStatusLabel3
			});
			this->statusStrip1->Location = System::Drawing::Point(0, 641);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(1155, 22);
			this->statusStrip1->TabIndex = 2;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(131, 17);
			this->toolStripStatusLabel1->Text = L"toolStripStatusLabel1";
			// 
			// toolStripStatusLabel2
			// 
			this->toolStripStatusLabel2->Name = L"toolStripStatusLabel2";
			this->toolStripStatusLabel2->Size = System::Drawing::Size(131, 17);
			this->toolStripStatusLabel2->Text = L"toolStripStatusLabel2";
			// 
			// toolStripStatusLabel3
			// 
			this->toolStripStatusLabel3->Name = L"toolStripStatusLabel3";
			this->toolStripStatusLabel3->Size = System::Drawing::Size(131, 17);
			this->toolStripStatusLabel3->Text = L"toolStripStatusLabel3";
			// 
			// send_speed_timer
			// 
			this->send_speed_timer->Enabled = true;
			this->send_speed_timer->Tick += gcnew System::EventHandler(this, &MyForm::send_speed_timer_tick);
			// 
			// MainTimer
			// 
			this->MainTimer->Enabled = true;
			this->MainTimer->Tick += gcnew System::EventHandler(this, &MyForm::MainTimer_Tick);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1155, 663);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->statusStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->panel1->ResumeLayout(false);
			this->panel2->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->panel9->ResumeLayout(false);
			this->panel9->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxMap))->EndInit();
			this->panel3->ResumeLayout(false);
			this->tabControl2->ResumeLayout(false);
			this->tp_comFunc->ResumeLayout(false);
			this->tp_comFunc->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			this->tp_directionCtrl->ResumeLayout(false);
			this->panel11->ResumeLayout(false);
			this->panel11->PerformLayout();
			this->panel8->ResumeLayout(false);
			this->panel8->PerformLayout();
			this->panel6->ResumeLayout(false);
			this->panel6->PerformLayout();
			this->panel5->ResumeLayout(false);
			this->panel5->PerformLayout();
			this->panel7->ResumeLayout(false);
			this->panel7->PerformLayout();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: 
		System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e);
		System::Void MainTimer_Tick(System::Object^  sender, System::EventArgs^  e);
		System::Void textBox_key_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);


		System::Void drawMap();
		System::Void drawLidarData(IplImage* img);

		//建图
		System::Void button_start_build_map_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void button_stop_build_map_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void button_start_build_map_auto_Click(System::Object^  sender, System::EventArgs^  e);
		//定位
		System::Void button_init_map_Click(System::Object^  sender, System::EventArgs^  e);

private: System::Void button_send_target_Click(System::Object^  sender, System::EventArgs^  e);

private: 
private: System::Void button_connect_Click(System::Object^  sender, System::EventArgs^  e);

private: System::Void drawRobot(IplImage* img);
private: System::Void drawAstarPoints(IplImage* img);
private: System::Void drawMotionTrajectory(IplImage* img);


private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e);
private: System::Void set_pic_show_scale_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void pictureBoxMap_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void pictureBoxMap_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void pictureBoxMap_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void button_config_Click(System::Object^  sender, System::EventArgs^  e);
		// private: System::Void ValidateSimulationOrRealMode();
private: System::Void checkBox_simulation_CheckedChanged(System::Object^  sender, System::EventArgs^  e);

private: System::Void button_save_simplemap_Click(System::Object^  sender, System::EventArgs^  e);

private: System::Void back_charge_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e);

private: System::Void button_video_record_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void button_IMU_calibrate_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CB_UseTcp_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void BTN_UnifiedSensor_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void cb_connectMpu_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void btn_update_map_flag_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void send_speed_timer_tick(System::Object^  sender, System::EventArgs^  e);
};

}
