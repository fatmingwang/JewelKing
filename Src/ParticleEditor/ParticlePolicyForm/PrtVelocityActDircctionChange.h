#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ParticlePolicyForm {

	/// <summary>
	/// Summary for PrtVelocityActDircctionChange
	/// </summary>
	public ref class PrtVelocityActDircctionChange : public System::Windows::Forms::UserControl
	{
	public:
		PrtVelocityActDircctionChange(Vector3*e_pvStopTime,Vector3*e_pvNewSpeed,bool *e_pbAcceleration	)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			NewDirection_xyzNumeriaclControl->SetValue(e_pvNewSpeed);
			StopTime_xyzNumeriaclControl->SetValue(e_pvStopTime);
			Acceleration_checkBoxControl->SetValue(e_pbAcceleration);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PrtVelocityActDircctionChange()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: DotNetComponent::XYZNumeriaclControl^  NewDirection_xyzNumeriaclControl;
	private: DotNetComponent::XYZNumeriaclControl^  StopTime_xyzNumeriaclControl;
	private: DotNetComponent::CheckBoxControl^  Acceleration_checkBoxControl;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->StopTime_xyzNumeriaclControl = (gcnew DotNetComponent::XYZNumeriaclControl());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->NewDirection_xyzNumeriaclControl = (gcnew DotNetComponent::XYZNumeriaclControl());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->Acceleration_checkBoxControl = (gcnew DotNetComponent::CheckBoxControl());
			this->SuspendLayout();
			// 
			// StopTime_xyzNumeriaclControl
			// 
			this->StopTime_xyzNumeriaclControl->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->StopTime_xyzNumeriaclControl->Location = System::Drawing::Point(1, 13);
			this->StopTime_xyzNumeriaclControl->Margin = System::Windows::Forms::Padding(0);
			this->StopTime_xyzNumeriaclControl->Name = L"StopTime_xyzNumeriaclControl";
			this->StopTime_xyzNumeriaclControl->Size = System::Drawing::Size(60, 87);
			this->StopTime_xyzNumeriaclControl->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(3, 1);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(50, 12);
			this->label1->TabIndex = 6;
			this->label1->Text = L"StopTime";
			// 
			// NewDirection_xyzNumeriaclControl
			// 
			this->NewDirection_xyzNumeriaclControl->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->NewDirection_xyzNumeriaclControl->Location = System::Drawing::Point(61, 12);
			this->NewDirection_xyzNumeriaclControl->Margin = System::Windows::Forms::Padding(0);
			this->NewDirection_xyzNumeriaclControl->Name = L"NewDirection_xyzNumeriaclControl";
			this->NewDirection_xyzNumeriaclControl->Size = System::Drawing::Size(60, 88);
			this->NewDirection_xyzNumeriaclControl->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(59, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(69, 12);
			this->label2->TabIndex = 7;
			this->label2->Text = L"NewDirection";
			// 
			// Acceleration_checkBoxControl
			// 
			this->Acceleration_checkBoxControl->CheckboxName = L"Acceleration";
			this->Acceleration_checkBoxControl->ForeColor = System::Drawing::Color::White;
			this->Acceleration_checkBoxControl->Location = System::Drawing::Point(0, 103);
			this->Acceleration_checkBoxControl->Name = L"Acceleration_checkBoxControl";
			this->Acceleration_checkBoxControl->Size = System::Drawing::Size(111, 21);
			this->Acceleration_checkBoxControl->TabIndex = 2;
			// 
			// PrtVelocityActDircctionChange
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlDark;
			this->Controls->Add(this->label2);
			this->Controls->Add(this->Acceleration_checkBoxControl);
			this->Controls->Add(this->NewDirection_xyzNumeriaclControl);
			this->Controls->Add(this->StopTime_xyzNumeriaclControl);
			this->Controls->Add(this->label1);
			this->ForeColor = System::Drawing::SystemColors::ControlText;
			this->Name = L"PrtVelocityActDircctionChange";
			this->Size = System::Drawing::Size(128, 130);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
