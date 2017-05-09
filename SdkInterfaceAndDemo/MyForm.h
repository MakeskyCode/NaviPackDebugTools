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
	private: System::Windows::Forms::Panel^  panel_all;
	private: System::Windows::Forms::Panel^  panel_display;
	private: System::Windows::Forms::Panel^  panel_control;
	protected:




private: System::Windows::Forms::StatusStrip^  statusStrip1;

private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel2;
private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel3;
	private: System::Windows::Forms::TabControl^  tabControl_control;
	private: System::Windows::Forms::TabPage^  tabPage_basic;




	private: System::Windows::Forms::CheckBox^  cb_tcp_or_com;
	private: System::Windows::Forms::Button^  btn_imu_calibrate;
	private: System::Windows::Forms::Button^  btn_record_video;




	private: System::Windows::Forms::CheckBox^  cb_connect_mpu;
	private: System::Windows::Forms::Button^  btn_init_location;
	private: System::Windows::Forms::Button^  btn_save_map;






	private: System::Windows::Forms::Button^  btn_build_map;

	private: System::Windows::Forms::TrackBar^  trackBar_map_scale;
	private: System::Windows::Forms::Button^  btn_set_map_scale;
	private: System::Windows::Forms::TextBox^  textBox_map_scale;






	private: System::Windows::Forms::TextBox^  textBox_ip_or_com;
	private: System::Windows::Forms::TextBox^  textBox_port_or_baud_rate;
	private: System::Windows::Forms::Button^  btn_connect;





	private: System::Windows::Forms::Label^  label_port_or_baud_rate;

	private: System::Windows::Forms::Label^  label_ip_or_com;
	private: System::Windows::Forms::TabPage^  tabPage_speed_control;
























private: System::Windows::Forms::TabPage^  tab_advanced;
	private: System::Windows::Forms::CheckBox^  cb_only_mpu;


private: System::Windows::Forms::Button^  btn_update_map_flag;
private: System::Windows::Forms::Timer^  send_speed_timer;
private: System::Windows::Forms::Timer^  MainTimer;
private: System::Windows::Forms::CheckBox^  cb_enable_drawing_pen;
private: System::Windows::Forms::Button^  btn_clear_virtual_obstacles;
private: System::Windows::Forms::Button^  btn_send_virtual_obstacles;




private: System::Windows::Forms::GroupBox^  groupBox_virtual_obstacles;






private: System::Windows::Forms::Button^  btn_optimize_map;
private: System::Windows::Forms::Button^  btn_do_clean_task;
private: System::Windows::Forms::Button^  btn_update_lidar;


private: System::Windows::Forms::Button^  btn_send_unified_sensor_data;

	private: System::Windows::Forms::Button^  btn_update_navipack;
private: System::Windows::Forms::Button^  btn_back_to_charge;


private: System::Windows::Forms::Button^  btn_auto_build_map;

private: System::Windows::Forms::TabControl^  tabControl_display;
private: System::Windows::Forms::TabPage^  tabPage_map_display;

private: System::Windows::Forms::Panel^  panel9;
private: System::Windows::Forms::PictureBox^  pictureBoxMap;
private: System::Windows::Forms::GroupBox^  groupBox_connection;
private: System::Windows::Forms::GroupBox^  groupBox_map;
private: System::Windows::Forms::Panel^  panel_speed_control;
private: System::Windows::Forms::Label^  label_focus_here;
private: System::Windows::Forms::Label^  label_w_step;



private: System::Windows::Forms::Label^  label_v_step;

private: System::Windows::Forms::TextBox^  textBox_v_step;
private: System::Windows::Forms::TextBox^  textBox_w_step;
private: System::Windows::Forms::Panel^  panel_D;

private: System::Windows::Forms::Label^  label_D;
private: System::Windows::Forms::Panel^  panel_w;


private: System::Windows::Forms::Label^  label_W;
private: System::Windows::Forms::Panel^  panel_A;


private: System::Windows::Forms::Label^  label_A;
private: System::Windows::Forms::Panel^  panel_S;


private: System::Windows::Forms::Label^  label_S;
private: System::Windows::Forms::TextBox^  textBox_speed_control_key;


private: System::Windows::Forms::Panel^  panel_space;
private: System::Windows::Forms::Label^  label_space;
private: System::Windows::Forms::GroupBox^  groupBox_update_firmware;
private: System::Windows::Forms::GroupBox^  groupBox_tasks;
private: System::Windows::Forms::TabPage^  tabPage1;
private: System::Windows::Forms::OpenFileDialog^  openFileDialog;
private: System::Windows::Forms::GroupBox^  groupBox1;
private: System::Windows::Forms::Button^  btnUpload;
private: System::Windows::Forms::Button^  btnDownloadMap;

private: System::Windows::Forms::SaveFileDialog^  saveFileDialog;
private: System::Windows::Forms::Button^  btnSaveMapInfo;







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
			this->panel_all = (gcnew System::Windows::Forms::Panel());
			this->panel_display = (gcnew System::Windows::Forms::Panel());
			this->tabControl_display = (gcnew System::Windows::Forms::TabControl());
			this->tabPage_map_display = (gcnew System::Windows::Forms::TabPage());
			this->panel9 = (gcnew System::Windows::Forms::Panel());
			this->pictureBoxMap = (gcnew System::Windows::Forms::PictureBox());
			this->panel_control = (gcnew System::Windows::Forms::Panel());
			this->tabControl_control = (gcnew System::Windows::Forms::TabControl());
			this->tabPage_basic = (gcnew System::Windows::Forms::TabPage());
			this->groupBox_map = (gcnew System::Windows::Forms::GroupBox());
			this->btn_set_map_scale = (gcnew System::Windows::Forms::Button());
			this->textBox_map_scale = (gcnew System::Windows::Forms::TextBox());
			this->btn_update_map_flag = (gcnew System::Windows::Forms::Button());
			this->trackBar_map_scale = (gcnew System::Windows::Forms::TrackBar());
			this->btn_build_map = (gcnew System::Windows::Forms::Button());
			this->btn_save_map = (gcnew System::Windows::Forms::Button());
			this->groupBox_connection = (gcnew System::Windows::Forms::GroupBox());
			this->label_ip_or_com = (gcnew System::Windows::Forms::Label());
			this->label_port_or_baud_rate = (gcnew System::Windows::Forms::Label());
			this->cb_only_mpu = (gcnew System::Windows::Forms::CheckBox());
			this->btn_connect = (gcnew System::Windows::Forms::Button());
			this->cb_tcp_or_com = (gcnew System::Windows::Forms::CheckBox());
			this->textBox_port_or_baud_rate = (gcnew System::Windows::Forms::TextBox());
			this->textBox_ip_or_com = (gcnew System::Windows::Forms::TextBox());
			this->cb_connect_mpu = (gcnew System::Windows::Forms::CheckBox());
			this->btn_imu_calibrate = (gcnew System::Windows::Forms::Button());
			this->btn_record_video = (gcnew System::Windows::Forms::Button());
			this->btn_init_location = (gcnew System::Windows::Forms::Button());
			this->tabPage_speed_control = (gcnew System::Windows::Forms::TabPage());
			this->panel_speed_control = (gcnew System::Windows::Forms::Panel());
			this->panel_space = (gcnew System::Windows::Forms::Panel());
			this->label_space = (gcnew System::Windows::Forms::Label());
			this->label_focus_here = (gcnew System::Windows::Forms::Label());
			this->label_w_step = (gcnew System::Windows::Forms::Label());
			this->label_v_step = (gcnew System::Windows::Forms::Label());
			this->textBox_v_step = (gcnew System::Windows::Forms::TextBox());
			this->textBox_w_step = (gcnew System::Windows::Forms::TextBox());
			this->panel_D = (gcnew System::Windows::Forms::Panel());
			this->label_D = (gcnew System::Windows::Forms::Label());
			this->panel_w = (gcnew System::Windows::Forms::Panel());
			this->label_W = (gcnew System::Windows::Forms::Label());
			this->panel_A = (gcnew System::Windows::Forms::Panel());
			this->label_A = (gcnew System::Windows::Forms::Label());
			this->panel_S = (gcnew System::Windows::Forms::Panel());
			this->label_S = (gcnew System::Windows::Forms::Label());
			this->textBox_speed_control_key = (gcnew System::Windows::Forms::TextBox());
			this->tab_advanced = (gcnew System::Windows::Forms::TabPage());
			this->groupBox_update_firmware = (gcnew System::Windows::Forms::GroupBox());
			this->btn_update_navipack = (gcnew System::Windows::Forms::Button());
			this->btn_update_lidar = (gcnew System::Windows::Forms::Button());
			this->groupBox_tasks = (gcnew System::Windows::Forms::GroupBox());
			this->btn_auto_build_map = (gcnew System::Windows::Forms::Button());
			this->btn_back_to_charge = (gcnew System::Windows::Forms::Button());
			this->btn_do_clean_task = (gcnew System::Windows::Forms::Button());
			this->btn_optimize_map = (gcnew System::Windows::Forms::Button());
			this->btn_send_unified_sensor_data = (gcnew System::Windows::Forms::Button());
			this->groupBox_virtual_obstacles = (gcnew System::Windows::Forms::GroupBox());
			this->btn_clear_virtual_obstacles = (gcnew System::Windows::Forms::Button());
			this->cb_enable_drawing_pen = (gcnew System::Windows::Forms::CheckBox());
			this->btn_send_virtual_obstacles = (gcnew System::Windows::Forms::Button());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->btnUpload = (gcnew System::Windows::Forms::Button());
			this->btnDownloadMap = (gcnew System::Windows::Forms::Button());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel3 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->send_speed_timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->MainTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->btnSaveMapInfo = (gcnew System::Windows::Forms::Button());
			this->panel_all->SuspendLayout();
			this->panel_display->SuspendLayout();
			this->tabControl_display->SuspendLayout();
			this->tabPage_map_display->SuspendLayout();
			this->panel9->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxMap))->BeginInit();
			this->panel_control->SuspendLayout();
			this->tabControl_control->SuspendLayout();
			this->tabPage_basic->SuspendLayout();
			this->groupBox_map->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar_map_scale))->BeginInit();
			this->groupBox_connection->SuspendLayout();
			this->tabPage_speed_control->SuspendLayout();
			this->panel_speed_control->SuspendLayout();
			this->panel_space->SuspendLayout();
			this->panel_D->SuspendLayout();
			this->panel_w->SuspendLayout();
			this->panel_A->SuspendLayout();
			this->panel_S->SuspendLayout();
			this->tab_advanced->SuspendLayout();
			this->groupBox_update_firmware->SuspendLayout();
			this->groupBox_tasks->SuspendLayout();
			this->groupBox_virtual_obstacles->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel_all
			// 
			this->panel_all->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panel_all->Controls->Add(this->panel_display);
			this->panel_all->Controls->Add(this->panel_control);
			this->panel_all->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel_all->Location = System::Drawing::Point(0, 0);
			this->panel_all->Name = L"panel_all";
			this->panel_all->Size = System::Drawing::Size(1155, 641);
			this->panel_all->TabIndex = 0;
			// 
			// panel_display
			// 
			this->panel_display->Controls->Add(this->tabControl_display);
			this->panel_display->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel_display->Location = System::Drawing::Point(0, 0);
			this->panel_display->Name = L"panel_display";
			this->panel_display->Size = System::Drawing::Size(831, 637);
			this->panel_display->TabIndex = 0;
			// 
			// tabControl_display
			// 
			this->tabControl_display->Controls->Add(this->tabPage_map_display);
			this->tabControl_display->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl_display->Location = System::Drawing::Point(0, 0);
			this->tabControl_display->Name = L"tabControl_display";
			this->tabControl_display->SelectedIndex = 0;
			this->tabControl_display->Size = System::Drawing::Size(831, 637);
			this->tabControl_display->TabIndex = 0;
			// 
			// tabPage_map_display
			// 
			this->tabPage_map_display->Controls->Add(this->panel9);
			this->tabPage_map_display->Location = System::Drawing::Point(4, 22);
			this->tabPage_map_display->Name = L"tabPage_map_display";
			this->tabPage_map_display->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_map_display->Size = System::Drawing::Size(823, 611);
			this->tabPage_map_display->TabIndex = 0;
			this->tabPage_map_display->Text = L"Map Display";
			this->tabPage_map_display->UseVisualStyleBackColor = true;
			// 
			// panel9
			// 
			this->panel9->AutoScroll = true;
			this->panel9->Controls->Add(this->pictureBoxMap);
			this->panel9->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel9->Location = System::Drawing::Point(3, 3);
			this->panel9->Name = L"panel9";
			this->panel9->Size = System::Drawing::Size(817, 605);
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
			// panel_control
			// 
			this->panel_control->Controls->Add(this->tabControl_control);
			this->panel_control->Dock = System::Windows::Forms::DockStyle::Right;
			this->panel_control->Location = System::Drawing::Point(831, 0);
			this->panel_control->Name = L"panel_control";
			this->panel_control->Size = System::Drawing::Size(320, 637);
			this->panel_control->TabIndex = 1;
			// 
			// tabControl_control
			// 
			this->tabControl_control->Controls->Add(this->tabPage_basic);
			this->tabControl_control->Controls->Add(this->tabPage_speed_control);
			this->tabControl_control->Controls->Add(this->tab_advanced);
			this->tabControl_control->Controls->Add(this->tabPage1);
			this->tabControl_control->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl_control->Font = (gcnew System::Drawing::Font(L"宋体", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->tabControl_control->Location = System::Drawing::Point(0, 0);
			this->tabControl_control->Name = L"tabControl_control";
			this->tabControl_control->SelectedIndex = 0;
			this->tabControl_control->Size = System::Drawing::Size(320, 637);
			this->tabControl_control->TabIndex = 0;
			// 
			// tabPage_basic
			// 
			this->tabPage_basic->Controls->Add(this->groupBox_map);
			this->tabPage_basic->Controls->Add(this->groupBox_connection);
			this->tabPage_basic->Controls->Add(this->btn_imu_calibrate);
			this->tabPage_basic->Controls->Add(this->btn_record_video);
			this->tabPage_basic->Controls->Add(this->btn_init_location);
			this->tabPage_basic->Location = System::Drawing::Point(4, 22);
			this->tabPage_basic->Name = L"tabPage_basic";
			this->tabPage_basic->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_basic->Size = System::Drawing::Size(312, 611);
			this->tabPage_basic->TabIndex = 0;
			this->tabPage_basic->Text = L"Basic";
			this->tabPage_basic->UseVisualStyleBackColor = true;
			// 
			// groupBox_map
			// 
			this->groupBox_map->Controls->Add(this->btn_set_map_scale);
			this->groupBox_map->Controls->Add(this->textBox_map_scale);
			this->groupBox_map->Controls->Add(this->btn_update_map_flag);
			this->groupBox_map->Controls->Add(this->trackBar_map_scale);
			this->groupBox_map->Controls->Add(this->btn_build_map);
			this->groupBox_map->Controls->Add(this->btn_save_map);
			this->groupBox_map->Location = System::Drawing::Point(6, 147);
			this->groupBox_map->Name = L"groupBox_map";
			this->groupBox_map->Size = System::Drawing::Size(300, 210);
			this->groupBox_map->TabIndex = 54;
			this->groupBox_map->TabStop = false;
			this->groupBox_map->Text = L"MAP";
			// 
			// btn_set_map_scale
			// 
			this->btn_set_map_scale->Location = System::Drawing::Point(12, 29);
			this->btn_set_map_scale->Margin = System::Windows::Forms::Padding(2);
			this->btn_set_map_scale->Name = L"btn_set_map_scale";
			this->btn_set_map_scale->Size = System::Drawing::Size(125, 25);
			this->btn_set_map_scale->TabIndex = 36;
			this->btn_set_map_scale->Text = L"Set Map Scale";
			this->btn_set_map_scale->UseVisualStyleBackColor = true;
			this->btn_set_map_scale->Click += gcnew System::EventHandler(this, &MyForm::set_pic_show_scale_Click);
			// 
			// textBox_map_scale
			// 
			this->textBox_map_scale->Location = System::Drawing::Point(141, 32);
			this->textBox_map_scale->Margin = System::Windows::Forms::Padding(2);
			this->textBox_map_scale->Name = L"textBox_map_scale";
			this->textBox_map_scale->Size = System::Drawing::Size(35, 21);
			this->textBox_map_scale->TabIndex = 37;
			this->textBox_map_scale->Text = L"2";
			// 
			// btn_update_map_flag
			// 
			this->btn_update_map_flag->Location = System::Drawing::Point(10, 169);
			this->btn_update_map_flag->Name = L"btn_update_map_flag";
			this->btn_update_map_flag->Size = System::Drawing::Size(157, 23);
			this->btn_update_map_flag->TabIndex = 52;
			this->btn_update_map_flag->Text = L"Enable Update Map";
			this->btn_update_map_flag->UseVisualStyleBackColor = true;
			this->btn_update_map_flag->Click += gcnew System::EventHandler(this, &MyForm::btn_update_map_flag_Click);
			// 
			// trackBar_map_scale
			// 
			this->trackBar_map_scale->Location = System::Drawing::Point(10, 60);
			this->trackBar_map_scale->Maximum = 100;
			this->trackBar_map_scale->Minimum = 1;
			this->trackBar_map_scale->Name = L"trackBar_map_scale";
			this->trackBar_map_scale->Size = System::Drawing::Size(268, 45);
			this->trackBar_map_scale->TabIndex = 39;
			this->trackBar_map_scale->Value = 1;
			this->trackBar_map_scale->Scroll += gcnew System::EventHandler(this, &MyForm::trackBar_map_scale_Scroll);
			// 
			// btn_build_map
			// 
			this->btn_build_map->Location = System::Drawing::Point(10, 111);
			this->btn_build_map->Name = L"btn_build_map";
			this->btn_build_map->Size = System::Drawing::Size(157, 23);
			this->btn_build_map->TabIndex = 40;
			this->btn_build_map->Text = L"Build Map";
			this->btn_build_map->UseVisualStyleBackColor = true;
			this->btn_build_map->Click += gcnew System::EventHandler(this, &MyForm::btn_build_map_Click);
			// 
			// btn_save_map
			// 
			this->btn_save_map->Location = System::Drawing::Point(10, 140);
			this->btn_save_map->Name = L"btn_save_map";
			this->btn_save_map->Size = System::Drawing::Size(157, 23);
			this->btn_save_map->TabIndex = 41;
			this->btn_save_map->Text = L"Save Map";
			this->btn_save_map->UseVisualStyleBackColor = true;
			this->btn_save_map->Click += gcnew System::EventHandler(this, &MyForm::btn_save_map_Click);
			// 
			// groupBox_connection
			// 
			this->groupBox_connection->Controls->Add(this->label_ip_or_com);
			this->groupBox_connection->Controls->Add(this->label_port_or_baud_rate);
			this->groupBox_connection->Controls->Add(this->cb_only_mpu);
			this->groupBox_connection->Controls->Add(this->btn_connect);
			this->groupBox_connection->Controls->Add(this->cb_tcp_or_com);
			this->groupBox_connection->Controls->Add(this->textBox_port_or_baud_rate);
			this->groupBox_connection->Controls->Add(this->textBox_ip_or_com);
			this->groupBox_connection->Controls->Add(this->cb_connect_mpu);
			this->groupBox_connection->Location = System::Drawing::Point(6, 10);
			this->groupBox_connection->Name = L"groupBox_connection";
			this->groupBox_connection->Size = System::Drawing::Size(300, 131);
			this->groupBox_connection->TabIndex = 53;
			this->groupBox_connection->TabStop = false;
			this->groupBox_connection->Text = L"CONNECTION";
			// 
			// label_ip_or_com
			// 
			this->label_ip_or_com->AutoSize = true;
			this->label_ip_or_com->Location = System::Drawing::Point(15, 31);
			this->label_ip_or_com->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label_ip_or_com->Name = L"label_ip_or_com";
			this->label_ip_or_com->Size = System::Drawing::Size(17, 12);
			this->label_ip_or_com->TabIndex = 24;
			this->label_ip_or_com->Text = L"IP";
			// 
			// label_port_or_baud_rate
			// 
			this->label_port_or_baud_rate->AutoSize = true;
			this->label_port_or_baud_rate->Location = System::Drawing::Point(13, 65);
			this->label_port_or_baud_rate->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label_port_or_baud_rate->Name = L"label_port_or_baud_rate";
			this->label_port_or_baud_rate->Size = System::Drawing::Size(29, 12);
			this->label_port_or_baud_rate->TabIndex = 25;
			this->label_port_or_baud_rate->Text = L"PORT";
			// 
			// cb_only_mpu
			// 
			this->cb_only_mpu->AutoSize = true;
			this->cb_only_mpu->Enabled = false;
			this->cb_only_mpu->Location = System::Drawing::Point(191, 89);
			this->cb_only_mpu->Name = L"cb_only_mpu";
			this->cb_only_mpu->Size = System::Drawing::Size(72, 16);
			this->cb_only_mpu->TabIndex = 51;
			this->cb_only_mpu->Text = L"ONLY_MPU";
			this->cb_only_mpu->UseVisualStyleBackColor = true;
			this->cb_only_mpu->Visible = false;
			// 
			// btn_connect
			// 
			this->btn_connect->Location = System::Drawing::Point(13, 90);
			this->btn_connect->Margin = System::Windows::Forms::Padding(2);
			this->btn_connect->Name = L"btn_connect";
			this->btn_connect->Size = System::Drawing::Size(163, 25);
			this->btn_connect->TabIndex = 21;
			this->btn_connect->Text = L"Connect";
			this->btn_connect->UseVisualStyleBackColor = true;
			this->btn_connect->Click += gcnew System::EventHandler(this, &MyForm::btn_connect_Click);
			// 
			// cb_tcp_or_com
			// 
			this->cb_tcp_or_com->AutoSize = true;
			this->cb_tcp_or_com->Checked = true;
			this->cb_tcp_or_com->CheckState = System::Windows::Forms::CheckState::Checked;
			this->cb_tcp_or_com->Location = System::Drawing::Point(191, 34);
			this->cb_tcp_or_com->Name = L"cb_tcp_or_com";
			this->cb_tcp_or_com->Size = System::Drawing::Size(84, 16);
			this->cb_tcp_or_com->TabIndex = 49;
			this->cb_tcp_or_com->Text = L"TCP_or_COM";
			this->cb_tcp_or_com->UseVisualStyleBackColor = true;
			this->cb_tcp_or_com->CheckedChanged += gcnew System::EventHandler(this, &MyForm::cb_tcp_or_com_CheckedChanged);
			// 
			// textBox_port_or_baud_rate
			// 
			this->textBox_port_or_baud_rate->Location = System::Drawing::Point(77, 59);
			this->textBox_port_or_baud_rate->Margin = System::Windows::Forms::Padding(2);
			this->textBox_port_or_baud_rate->Name = L"textBox_port_or_baud_rate";
			this->textBox_port_or_baud_rate->Size = System::Drawing::Size(100, 21);
			this->textBox_port_or_baud_rate->TabIndex = 23;
			this->textBox_port_or_baud_rate->Text = L"9977";
			// 
			// textBox_ip_or_com
			// 
			this->textBox_ip_or_com->Location = System::Drawing::Point(77, 31);
			this->textBox_ip_or_com->Margin = System::Windows::Forms::Padding(2);
			this->textBox_ip_or_com->Name = L"textBox_ip_or_com";
			this->textBox_ip_or_com->Size = System::Drawing::Size(100, 21);
			this->textBox_ip_or_com->TabIndex = 22;
			this->textBox_ip_or_com->Text = L"192.168.17.1";
			// 
			// cb_connect_mpu
			// 
			this->cb_connect_mpu->AutoSize = true;
			this->cb_connect_mpu->Enabled = false;
			this->cb_connect_mpu->Location = System::Drawing::Point(191, 63);
			this->cb_connect_mpu->Name = L"cb_connect_mpu";
			this->cb_connect_mpu->Size = System::Drawing::Size(66, 16);
			this->cb_connect_mpu->TabIndex = 45;
			this->cb_connect_mpu->Text = L"GET_MPU";
			this->cb_connect_mpu->UseVisualStyleBackColor = true;
			this->cb_connect_mpu->Visible = false;
			this->cb_connect_mpu->CheckedChanged += gcnew System::EventHandler(this, &MyForm::cb_connect_mpu_CheckedChanged);
			// 
			// btn_imu_calibrate
			// 
			this->btn_imu_calibrate->Location = System::Drawing::Point(45, 415);
			this->btn_imu_calibrate->Name = L"btn_imu_calibrate";
			this->btn_imu_calibrate->Size = System::Drawing::Size(155, 23);
			this->btn_imu_calibrate->TabIndex = 48;
			this->btn_imu_calibrate->Text = L"IMU Calibrate";
			this->btn_imu_calibrate->UseVisualStyleBackColor = true;
			this->btn_imu_calibrate->Click += gcnew System::EventHandler(this, &MyForm::btn_imu_calibrate_Click);
			// 
			// btn_record_video
			// 
			this->btn_record_video->Location = System::Drawing::Point(45, 469);
			this->btn_record_video->Name = L"btn_record_video";
			this->btn_record_video->Size = System::Drawing::Size(155, 23);
			this->btn_record_video->TabIndex = 47;
			this->btn_record_video->Text = L"Record Video";
			this->btn_record_video->UseVisualStyleBackColor = true;
			this->btn_record_video->Click += gcnew System::EventHandler(this, &MyForm::btn_record_video_Click);
			// 
			// btn_init_location
			// 
			this->btn_init_location->Location = System::Drawing::Point(45, 386);
			this->btn_init_location->Name = L"btn_init_location";
			this->btn_init_location->Size = System::Drawing::Size(155, 23);
			this->btn_init_location->TabIndex = 42;
			this->btn_init_location->Text = L"Init Location";
			this->btn_init_location->UseVisualStyleBackColor = true;
			this->btn_init_location->Click += gcnew System::EventHandler(this, &MyForm::btn_init_location_Click);
			// 
			// tabPage_speed_control
			// 
			this->tabPage_speed_control->Controls->Add(this->panel_speed_control);
			this->tabPage_speed_control->Location = System::Drawing::Point(4, 22);
			this->tabPage_speed_control->Name = L"tabPage_speed_control";
			this->tabPage_speed_control->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_speed_control->Size = System::Drawing::Size(312, 611);
			this->tabPage_speed_control->TabIndex = 1;
			this->tabPage_speed_control->Text = L"Speed Control";
			this->tabPage_speed_control->UseVisualStyleBackColor = true;
			// 
			// panel_speed_control
			// 
			this->panel_speed_control->AccessibleName = L"";
			this->panel_speed_control->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel_speed_control->Controls->Add(this->panel_space);
			this->panel_speed_control->Controls->Add(this->label_focus_here);
			this->panel_speed_control->Controls->Add(this->label_w_step);
			this->panel_speed_control->Controls->Add(this->label_v_step);
			this->panel_speed_control->Controls->Add(this->textBox_v_step);
			this->panel_speed_control->Controls->Add(this->textBox_w_step);
			this->panel_speed_control->Controls->Add(this->panel_D);
			this->panel_speed_control->Controls->Add(this->panel_w);
			this->panel_speed_control->Controls->Add(this->panel_A);
			this->panel_speed_control->Controls->Add(this->panel_S);
			this->panel_speed_control->Controls->Add(this->textBox_speed_control_key);
			this->panel_speed_control->Location = System::Drawing::Point(19, 24);
			this->panel_speed_control->Name = L"panel_speed_control";
			this->panel_speed_control->Size = System::Drawing::Size(262, 237);
			this->panel_speed_control->TabIndex = 50;
			// 
			// panel_space
			// 
			this->panel_space->Controls->Add(this->label_space);
			this->panel_space->Location = System::Drawing::Point(68, 126);
			this->panel_space->Name = L"panel_space";
			this->panel_space->Size = System::Drawing::Size(95, 30);
			this->panel_space->TabIndex = 39;
			// 
			// label_space
			// 
			this->label_space->AutoSize = true;
			this->label_space->Font = (gcnew System::Drawing::Font(L"宋体", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->label_space->Location = System::Drawing::Point(12, 4);
			this->label_space->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label_space->Name = L"label_space";
			this->label_space->Size = System::Drawing::Size(70, 21);
			this->label_space->TabIndex = 36;
			this->label_space->Text = L"SPACE";
			// 
			// label_focus_here
			// 
			this->label_focus_here->AutoSize = true;
			this->label_focus_here->Location = System::Drawing::Point(16, 7);
			this->label_focus_here->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label_focus_here->Name = L"label_focus_here";
			this->label_focus_here->Size = System::Drawing::Size(167, 12);
			this->label_focus_here->TabIndex = 52;
			this->label_focus_here->Text = L"Please Focus in the TextBox";
			// 
			// label_w_step
			// 
			this->label_w_step->AutoSize = true;
			this->label_w_step->Location = System::Drawing::Point(139, 173);
			this->label_w_step->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label_w_step->Name = L"label_w_step";
			this->label_w_step->Size = System::Drawing::Size(83, 12);
			this->label_w_step->TabIndex = 51;
			this->label_w_step->Text = L"w_step(rad/s)";
			// 
			// label_v_step
			// 
			this->label_v_step->AutoSize = true;
			this->label_v_step->Location = System::Drawing::Point(37, 172);
			this->label_v_step->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label_v_step->Name = L"label_v_step";
			this->label_v_step->Size = System::Drawing::Size(71, 12);
			this->label_v_step->TabIndex = 50;
			this->label_v_step->Text = L"v_step(m/s)";
			// 
			// textBox_v_step
			// 
			this->textBox_v_step->Location = System::Drawing::Point(36, 195);
			this->textBox_v_step->Margin = System::Windows::Forms::Padding(2);
			this->textBox_v_step->Name = L"textBox_v_step";
			this->textBox_v_step->Size = System::Drawing::Size(76, 21);
			this->textBox_v_step->TabIndex = 40;
			this->textBox_v_step->Text = L"0.1";
			// 
			// textBox_w_step
			// 
			this->textBox_w_step->Location = System::Drawing::Point(140, 195);
			this->textBox_w_step->Margin = System::Windows::Forms::Padding(2);
			this->textBox_w_step->Name = L"textBox_w_step";
			this->textBox_w_step->Size = System::Drawing::Size(76, 21);
			this->textBox_w_step->TabIndex = 39;
			this->textBox_w_step->Text = L"0.2";
			// 
			// panel_D
			// 
			this->panel_D->Controls->Add(this->label_D);
			this->panel_D->Location = System::Drawing::Point(133, 90);
			this->panel_D->Name = L"panel_D";
			this->panel_D->Size = System::Drawing::Size(30, 30);
			this->panel_D->TabIndex = 38;
			// 
			// label_D
			// 
			this->label_D->AutoSize = true;
			this->label_D->Font = (gcnew System::Drawing::Font(L"宋体", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->label_D->Location = System::Drawing::Point(4, 6);
			this->label_D->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label_D->Name = L"label_D";
			this->label_D->Size = System::Drawing::Size(22, 21);
			this->label_D->TabIndex = 36;
			this->label_D->Text = L"D";
			// 
			// panel_w
			// 
			this->panel_w->Controls->Add(this->label_W);
			this->panel_w->Location = System::Drawing::Point(98, 54);
			this->panel_w->Name = L"panel_w";
			this->panel_w->Size = System::Drawing::Size(30, 30);
			this->panel_w->TabIndex = 38;
			// 
			// label_W
			// 
			this->label_W->AutoSize = true;
			this->label_W->Font = (gcnew System::Drawing::Font(L"宋体", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->label_W->Location = System::Drawing::Point(4, 5);
			this->label_W->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label_W->Name = L"label_W";
			this->label_W->Size = System::Drawing::Size(22, 21);
			this->label_W->TabIndex = 36;
			this->label_W->Text = L"W";
			// 
			// panel_A
			// 
			this->panel_A->Controls->Add(this->label_A);
			this->panel_A->Location = System::Drawing::Point(68, 90);
			this->panel_A->Name = L"panel_A";
			this->panel_A->Size = System::Drawing::Size(30, 30);
			this->panel_A->TabIndex = 38;
			// 
			// label_A
			// 
			this->label_A->AutoSize = true;
			this->label_A->Font = (gcnew System::Drawing::Font(L"宋体", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->label_A->Location = System::Drawing::Point(4, 5);
			this->label_A->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label_A->Name = L"label_A";
			this->label_A->Size = System::Drawing::Size(22, 21);
			this->label_A->TabIndex = 36;
			this->label_A->Text = L"A";
			// 
			// panel_S
			// 
			this->panel_S->Controls->Add(this->label_S);
			this->panel_S->Location = System::Drawing::Point(102, 90);
			this->panel_S->Name = L"panel_S";
			this->panel_S->Size = System::Drawing::Size(30, 30);
			this->panel_S->TabIndex = 38;
			// 
			// label_S
			// 
			this->label_S->AutoSize = true;
			this->label_S->Font = (gcnew System::Drawing::Font(L"宋体", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(134)));
			this->label_S->Location = System::Drawing::Point(4, 5);
			this->label_S->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label_S->Name = L"label_S";
			this->label_S->Size = System::Drawing::Size(22, 21);
			this->label_S->TabIndex = 36;
			this->label_S->Text = L"S";
			// 
			// textBox_speed_control_key
			// 
			this->textBox_speed_control_key->Location = System::Drawing::Point(62, 28);
			this->textBox_speed_control_key->Margin = System::Windows::Forms::Padding(2);
			this->textBox_speed_control_key->Name = L"textBox_speed_control_key";
			this->textBox_speed_control_key->Size = System::Drawing::Size(107, 21);
			this->textBox_speed_control_key->TabIndex = 33;
			this->textBox_speed_control_key->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::textBox_speed_control_key_KeyPress);
			// 
			// tab_advanced
			// 
			this->tab_advanced->Controls->Add(this->groupBox_update_firmware);
			this->tab_advanced->Controls->Add(this->groupBox_tasks);
			this->tab_advanced->Controls->Add(this->btn_optimize_map);
			this->tab_advanced->Controls->Add(this->btn_send_unified_sensor_data);
			this->tab_advanced->Controls->Add(this->groupBox_virtual_obstacles);
			this->tab_advanced->Location = System::Drawing::Point(4, 22);
			this->tab_advanced->Name = L"tab_advanced";
			this->tab_advanced->Size = System::Drawing::Size(312, 611);
			this->tab_advanced->TabIndex = 2;
			this->tab_advanced->Text = L"Advanced";
			this->tab_advanced->UseVisualStyleBackColor = true;
			// 
			// groupBox_update_firmware
			// 
			this->groupBox_update_firmware->Controls->Add(this->btn_update_navipack);
			this->groupBox_update_firmware->Controls->Add(this->btn_update_lidar);
			this->groupBox_update_firmware->Location = System::Drawing::Point(20, 426);
			this->groupBox_update_firmware->Name = L"groupBox_update_firmware";
			this->groupBox_update_firmware->Size = System::Drawing::Size(272, 108);
			this->groupBox_update_firmware->TabIndex = 64;
			this->groupBox_update_firmware->TabStop = false;
			this->groupBox_update_firmware->Text = L"UPDATE FIRMWARE";
			// 
			// btn_update_navipack
			// 
			this->btn_update_navipack->Enabled = false;
			this->btn_update_navipack->Location = System::Drawing::Point(15, 33);
			this->btn_update_navipack->Name = L"btn_update_navipack";
			this->btn_update_navipack->Size = System::Drawing::Size(185, 23);
			this->btn_update_navipack->TabIndex = 57;
			this->btn_update_navipack->Text = L"Update Navipack";
			this->btn_update_navipack->UseVisualStyleBackColor = true;
			this->btn_update_navipack->Click += gcnew System::EventHandler(this, &MyForm::btn_update_navipack_Click);
			// 
			// btn_update_lidar
			// 
			this->btn_update_lidar->Enabled = false;
			this->btn_update_lidar->Location = System::Drawing::Point(15, 64);
			this->btn_update_lidar->Name = L"btn_update_lidar";
			this->btn_update_lidar->Size = System::Drawing::Size(185, 23);
			this->btn_update_lidar->TabIndex = 59;
			this->btn_update_lidar->Text = L"Update Lidar";
			this->btn_update_lidar->UseVisualStyleBackColor = true;
			this->btn_update_lidar->Click += gcnew System::EventHandler(this, &MyForm::btn_update_lidar_Click);
			// 
			// groupBox_tasks
			// 
			this->groupBox_tasks->Controls->Add(this->btn_auto_build_map);
			this->groupBox_tasks->Controls->Add(this->btn_back_to_charge);
			this->groupBox_tasks->Controls->Add(this->btn_do_clean_task);
			this->groupBox_tasks->Location = System::Drawing::Point(20, 271);
			this->groupBox_tasks->Name = L"groupBox_tasks";
			this->groupBox_tasks->Size = System::Drawing::Size(272, 134);
			this->groupBox_tasks->TabIndex = 63;
			this->groupBox_tasks->TabStop = false;
			this->groupBox_tasks->Text = L"TASKS";
			// 
			// btn_auto_build_map
			// 
			this->btn_auto_build_map->Enabled = false;
			this->btn_auto_build_map->Location = System::Drawing::Point(15, 35);
			this->btn_auto_build_map->Name = L"btn_auto_build_map";
			this->btn_auto_build_map->Size = System::Drawing::Size(185, 23);
			this->btn_auto_build_map->TabIndex = 62;
			this->btn_auto_build_map->Text = L"Auto Build Map";
			this->btn_auto_build_map->UseVisualStyleBackColor = true;
			this->btn_auto_build_map->Click += gcnew System::EventHandler(this, &MyForm::btn_auto_build_map_Click);
			// 
			// btn_back_to_charge
			// 
			this->btn_back_to_charge->Enabled = false;
			this->btn_back_to_charge->Location = System::Drawing::Point(15, 64);
			this->btn_back_to_charge->Name = L"btn_back_to_charge";
			this->btn_back_to_charge->Size = System::Drawing::Size(185, 23);
			this->btn_back_to_charge->TabIndex = 56;
			this->btn_back_to_charge->Text = L"Back to Charge";
			this->btn_back_to_charge->UseVisualStyleBackColor = true;
			this->btn_back_to_charge->Click += gcnew System::EventHandler(this, &MyForm::btn_back_to_charge_Click);
			// 
			// btn_do_clean_task
			// 
			this->btn_do_clean_task->Enabled = false;
			this->btn_do_clean_task->Location = System::Drawing::Point(15, 93);
			this->btn_do_clean_task->Name = L"btn_do_clean_task";
			this->btn_do_clean_task->Size = System::Drawing::Size(185, 23);
			this->btn_do_clean_task->TabIndex = 60;
			this->btn_do_clean_task->Text = L"Do Clean Task";
			this->btn_do_clean_task->UseVisualStyleBackColor = true;
			this->btn_do_clean_task->Click += gcnew System::EventHandler(this, &MyForm::btn_do_clean_task_Click);
			// 
			// btn_optimize_map
			// 
			this->btn_optimize_map->Location = System::Drawing::Point(21, 62);
			this->btn_optimize_map->Name = L"btn_optimize_map";
			this->btn_optimize_map->Size = System::Drawing::Size(232, 23);
			this->btn_optimize_map->TabIndex = 61;
			this->btn_optimize_map->Text = L"Optimize Map";
			this->btn_optimize_map->UseVisualStyleBackColor = true;
			this->btn_optimize_map->Click += gcnew System::EventHandler(this, &MyForm::btn_optimize_map_Click);
			// 
			// btn_send_unified_sensor_data
			// 
			this->btn_send_unified_sensor_data->Location = System::Drawing::Point(20, 21);
			this->btn_send_unified_sensor_data->Name = L"btn_send_unified_sensor_data";
			this->btn_send_unified_sensor_data->Size = System::Drawing::Size(233, 23);
			this->btn_send_unified_sensor_data->TabIndex = 58;
			this->btn_send_unified_sensor_data->Text = L"Send Unified Sensor Data";
			this->btn_send_unified_sensor_data->UseVisualStyleBackColor = true;
			this->btn_send_unified_sensor_data->Click += gcnew System::EventHandler(this, &MyForm::btn_send_unified_sensor_data_Click);
			// 
			// groupBox_virtual_obstacles
			// 
			this->groupBox_virtual_obstacles->Controls->Add(this->btn_clear_virtual_obstacles);
			this->groupBox_virtual_obstacles->Controls->Add(this->cb_enable_drawing_pen);
			this->groupBox_virtual_obstacles->Controls->Add(this->btn_send_virtual_obstacles);
			this->groupBox_virtual_obstacles->Location = System::Drawing::Point(20, 111);
			this->groupBox_virtual_obstacles->Name = L"groupBox_virtual_obstacles";
			this->groupBox_virtual_obstacles->Size = System::Drawing::Size(272, 136);
			this->groupBox_virtual_obstacles->TabIndex = 3;
			this->groupBox_virtual_obstacles->TabStop = false;
			this->groupBox_virtual_obstacles->Text = L"VIRTUAL OBSTACLES";
			// 
			// btn_clear_virtual_obstacles
			// 
			this->btn_clear_virtual_obstacles->Location = System::Drawing::Point(15, 56);
			this->btn_clear_virtual_obstacles->Name = L"btn_clear_virtual_obstacles";
			this->btn_clear_virtual_obstacles->Size = System::Drawing::Size(185, 23);
			this->btn_clear_virtual_obstacles->TabIndex = 2;
			this->btn_clear_virtual_obstacles->Text = L"Clear Virtual Obstacles";
			this->btn_clear_virtual_obstacles->UseVisualStyleBackColor = true;
			this->btn_clear_virtual_obstacles->Click += gcnew System::EventHandler(this, &MyForm::btn_clear_virtual_obstacles_Click);
			// 
			// cb_enable_drawing_pen
			// 
			this->cb_enable_drawing_pen->AutoSize = true;
			this->cb_enable_drawing_pen->Location = System::Drawing::Point(15, 31);
			this->cb_enable_drawing_pen->Name = L"cb_enable_drawing_pen";
			this->cb_enable_drawing_pen->Size = System::Drawing::Size(132, 16);
			this->cb_enable_drawing_pen->TabIndex = 0;
			this->cb_enable_drawing_pen->Text = L"Enable Drawing Pen";
			this->cb_enable_drawing_pen->UseVisualStyleBackColor = true;
			// 
			// btn_send_virtual_obstacles
			// 
			this->btn_send_virtual_obstacles->Location = System::Drawing::Point(15, 87);
			this->btn_send_virtual_obstacles->Name = L"btn_send_virtual_obstacles";
			this->btn_send_virtual_obstacles->Size = System::Drawing::Size(185, 23);
			this->btn_send_virtual_obstacles->TabIndex = 1;
			this->btn_send_virtual_obstacles->Text = L"Send Virtual Obstacles";
			this->btn_send_virtual_obstacles->UseVisualStyleBackColor = true;
			this->btn_send_virtual_obstacles->Click += gcnew System::EventHandler(this, &MyForm::btn_send_virtual_obstacles_Click);
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->btnSaveMapInfo);
			this->tabPage1->Controls->Add(this->groupBox1);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Size = System::Drawing::Size(312, 611);
			this->tabPage1->TabIndex = 3;
			this->tabPage1->Text = L"Update";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->btnUpload);
			this->groupBox1->Controls->Add(this->btnDownloadMap);
			this->groupBox1->Location = System::Drawing::Point(34, 115);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(272, 108);
			this->groupBox1->TabIndex = 65;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"UPLOAD AND DOWMLOAD MAP";
			// 
			// btnUpload
			// 
			this->btnUpload->Location = System::Drawing::Point(15, 33);
			this->btnUpload->Name = L"btnUpload";
			this->btnUpload->Size = System::Drawing::Size(185, 23);
			this->btnUpload->TabIndex = 57;
			this->btnUpload->Text = L"UploadMAp";
			this->btnUpload->UseVisualStyleBackColor = true;
			this->btnUpload->Click += gcnew System::EventHandler(this, &MyForm::btnUploadMap);
			// 
			// btnDownloadMap
			// 
			this->btnDownloadMap->Location = System::Drawing::Point(15, 64);
			this->btnDownloadMap->Name = L"btnDownloadMap";
			this->btnDownloadMap->Size = System::Drawing::Size(185, 23);
			this->btnDownloadMap->TabIndex = 59;
			this->btnDownloadMap->Text = L"DownloadMap";
			this->btnDownloadMap->UseVisualStyleBackColor = true;
			this->btnDownloadMap->Click += gcnew System::EventHandler(this, &MyForm::btnDownloadMap_Click);
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
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog";
			// 
			// btnSaveMapInfo
			// 
			this->btnSaveMapInfo->Location = System::Drawing::Point(49, 399);
			this->btnSaveMapInfo->Name = L"btnSaveMapInfo";
			this->btnSaveMapInfo->Size = System::Drawing::Size(97, 32);
			this->btnSaveMapInfo->TabIndex = 66;
			this->btnSaveMapInfo->Text = L"SaveMapInfo";
			this->btnSaveMapInfo->UseVisualStyleBackColor = true;
			this->btnSaveMapInfo->Click += gcnew System::EventHandler(this, &MyForm::btnSaveMapInfo_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1155, 663);
			this->Controls->Add(this->panel_all);
			this->Controls->Add(this->statusStrip1);
			this->Name = L"MyForm";
			this->Text = L"NaviPackSDKDemo";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->panel_all->ResumeLayout(false);
			this->panel_display->ResumeLayout(false);
			this->tabControl_display->ResumeLayout(false);
			this->tabPage_map_display->ResumeLayout(false);
			this->panel9->ResumeLayout(false);
			this->panel9->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxMap))->EndInit();
			this->panel_control->ResumeLayout(false);
			this->tabControl_control->ResumeLayout(false);
			this->tabPage_basic->ResumeLayout(false);
			this->groupBox_map->ResumeLayout(false);
			this->groupBox_map->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar_map_scale))->EndInit();
			this->groupBox_connection->ResumeLayout(false);
			this->groupBox_connection->PerformLayout();
			this->tabPage_speed_control->ResumeLayout(false);
			this->panel_speed_control->ResumeLayout(false);
			this->panel_speed_control->PerformLayout();
			this->panel_space->ResumeLayout(false);
			this->panel_space->PerformLayout();
			this->panel_D->ResumeLayout(false);
			this->panel_D->PerformLayout();
			this->panel_w->ResumeLayout(false);
			this->panel_w->PerformLayout();
			this->panel_A->ResumeLayout(false);
			this->panel_A->PerformLayout();
			this->panel_S->ResumeLayout(false);
			this->panel_S->PerformLayout();
			this->tab_advanced->ResumeLayout(false);
			this->groupBox_update_firmware->ResumeLayout(false);
			this->groupBox_tasks->ResumeLayout(false);
			this->groupBox_virtual_obstacles->ResumeLayout(false);
			this->groupBox_virtual_obstacles->PerformLayout();
			this->tabPage1->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: 
		System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e);
		System::Void MainTimer_Tick(System::Object^  sender, System::EventArgs^  e);
		System::Void send_speed_timer_tick(System::Object^  sender, System::EventArgs^  e);
		
		System::Void drawMap();
		System::Void drawLidarData(IplImage* img);
		
		System::Void textBox_speed_control_key_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
		System::Void btn_build_map_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btn_save_map_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btn_auto_build_map_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btn_init_location_Click(System::Object^  sender, System::EventArgs^  e);

		System::Void btn_connect_Click(System::Object^  sender, System::EventArgs^  e);

		System::Void drawRobot(IplImage* img);
		System::Void drawAstarPoints(IplImage* img);
		System::Void drawMotionTrajectory(IplImage* img);


		System::Void trackBar_map_scale_Scroll(System::Object^  sender, System::EventArgs^  e);
		System::Void set_pic_show_scale_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void pictureBoxMap_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
		System::Void pictureBoxMap_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);

		System::Void btn_record_video_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btn_imu_calibrate_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void cb_tcp_or_com_CheckedChanged(System::Object^  sender, System::EventArgs^  e);

		System::Void cb_connect_mpu_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void btn_update_map_flag_Click(System::Object^  sender, System::EventArgs^  e);


		System::Void btn_clear_virtual_obstacles_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btn_send_virtual_obstacles_Click(System::Object^  sender, System::EventArgs^  e);

		System::Void btn_back_to_charge_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btn_update_navipack_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btn_update_lidar_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btn_do_clean_task_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btn_optimize_map_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btn_send_unified_sensor_data_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void pictureBoxMap_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);

		 System::Void btnUploadMap(System::Object^  sender, System::EventArgs^  e);
		 System::Void btnDownloadMap_Click(System::Object ^ sender, System::EventArgs ^ e);
         System::Void btnSaveMapInfo_Click(System::Object^  sender, System::EventArgs^  e);
};

}
