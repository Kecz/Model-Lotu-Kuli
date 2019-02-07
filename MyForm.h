#pragma once
#define _USE_MATH_DEFINES
#include <string> 
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>


namespace MMMprojekt {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;

	//Zmienne w³asne
	const int maksymalny_rozmiar = 5000000;

	double Tablica_x[maksymalny_rozmiar];
	double Tablica_Vx[maksymalny_rozmiar];
	double Tablica_y[maksymalny_rozmiar];
	double Tablica_Vy[maksymalny_rozmiar];

	int licznik_pierwszego_calkowania = 0;
	double calka_suma_x = 0;
	double calka_suma_y = 0;

	double v0 = 60;
	double Masa_liczba = 20;
	double fi = 45;
	double b = 0.1;
	double g = 9.81;
	double poczatkowe_Vx = v0*cos(fi*M_PI / 180);
	double poczatkowe_Vy = v0*sin(fi*M_PI / 180);
	double interval = 100;

	double zasieg_max = 0;
	double wysokosc_max = 0;
	double czas_do_wysokosc_max = 0;

	/*
	const double maksymalny_rozmiar = 1000;
	float* Tablica_x;
	double* Tablica_Vx;
	Tablica_x = new float[maksymalny_rozmiar];
	*/

	/// <summary>
	/// Podsumowanie informacji o MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	

		//Zmienne programu:
	
	private: System::Windows::Forms::TextBox^  textBox_masa;
	private: System::Windows::Forms::TextBox^  textBox_opor;
	private: System::Windows::Forms::TextBox^  textBox_kat;
	private: System::Windows::Forms::TextBox^  textBox_interval;

	private: System::Windows::Forms::Button^  button_masa;
	private: System::Windows::Forms::Button^  button_opor;
	private: System::Windows::Forms::Button^  button_kat;
	private: System::Windows::Forms::Button^  button_start_symulacji;
	private: System::Windows::Forms::Button^  button_interval;
	private: System::Windows::Forms::Button^  button_predkosc;

	private: System::Windows::Forms::Timer^  timer1;

	private: System::Windows::Forms::Label^  label_poczatkowe_Vx;
	private: System::Windows::Forms::Label^  label_zasieg;
	private: System::Windows::Forms::Label^  label_interval;
	private: System::Windows::Forms::Label^  label2_interval;
	private: System::Windows::Forms::Label^  label_dane;
	private: System::Windows::Forms::Label^  label2_masa;
	private: System::Windows::Forms::Label^  label2_opor;
	private: System::Windows::Forms::Label^  label2_kat;
	private: System::Windows::Forms::Label^  label2_predkosc;
	private: System::Windows::Forms::Label^  label_masa;
	private: System::Windows::Forms::Label^  label_opor;
	private: System::Windows::Forms::Label^  label_kat;
	private: System::Windows::Forms::Label^  label_aktualna_Vx;
	private: System::Windows::Forms::Label^  label_poczatkowe_Vy;
	private: System::Windows::Forms::Label^  label_aktualna_Vy;
	private: System::Windows::Forms::Label^  label_wysokosc;
	private: System::Windows::Forms::Label^  label_max_zasieg;
	private: System::Windows::Forms::Label^  label_max_wysokosc;
	private: System::Windows::Forms::Label^  label_czas_max_y;
	private: System::Windows::Forms::Label^  label_czas_max_x;
	private: System::Windows::Forms::Label^  label_opisowy;

	private: System::Windows::Forms::DataVisualization::Charting::Chart^  polozenie_wykres;

	

	public:
		MyForm(void)
		{
			InitializeComponent(); //konstruktor

			poczatkowe_przypisanie(); //nadanie wartoœci pocz¹tkowych

		}

	//Funkcje w³asne

		//Przypisanie pocz¹tkowych wartoœci do parametrów
		void poczatkowe_przypisanie(void)
		{
			String^ predkosc_poczatkowa = Convert::ToString(v0);
			String^ masa = Convert::ToString(Masa_liczba);
			String^ opor = Convert::ToString(b);
			String^ kat = Convert::ToString(fi);
			String^ string_interval = Convert::ToString(interval);
			String^ string_poczatkowe_Vx = Convert::ToString(poczatkowe_Vx);
			String^ string_poczatkowe_Vy = Convert::ToString(poczatkowe_Vy);

			label2_predkosc->Text = "Prêdkoœæ pocz¹tkowa: " + predkosc_poczatkowa + " [m/s]";
			label2_masa->Text = "Masa: " + masa + " [kg]";
			label2_opor->Text = "Opór powietrza: " + opor + " [N]";
			label2_kat->Text = "K¹t wystrza³u: " + kat + " [°]";
			label2_interval->Text = "Interwa³: " + interval + " [ms]";
			label_poczatkowe_Vx->Text = "Prêdkoœæ pocz¹tkowa Vx: " + string_poczatkowe_Vx + " [m/s]";
			label_poczatkowe_Vy->Text = "Prêdkoœæ pocz¹tkowa Vy: " + string_poczatkowe_Vy + " [m/s]";
			label_aktualna_Vx->Text = "Prêdkoœæ aktualna Vx: " + string_poczatkowe_Vx + " [m/s]";
			label_aktualna_Vy->Text = "Prêdkoœæ aktualna Vy: " + string_poczatkowe_Vy + " [m/s]";

			Tablica_Vx[0] = poczatkowe_Vx;
			Tablica_x[0] = 0;
			Tablica_Vy[0] = poczatkowe_Vy;
			Tablica_y[0] = 0;
		}

		//Czyszczenie danych symulacji przed rozpoczêciem kolejnej
		void Czyszczenie_tablic(void)
		{
			for (int i = 0; i < maksymalny_rozmiar; i++)
			{
				Tablica_Vx[i] = 0;
				Tablica_x[i] = 0;
				Tablica_Vy[i] = 0;
				Tablica_y[i] = 0;
			}
		}
		
		//Oba ca³kowania w osi x
		void Liczenie_calek_w_x (int licznik)
		{

			Tablica_Vx[licznik + 1] = Tablica_Vx[licznik] - (b / Masa_liczba)*abs(Tablica_Vx[licznik])*timer1->Interval / 1000; //pierwsze ca³kowanie metod¹ prostok¹tów licz¹ce prêdkoœæ
			calka_suma_x += (Tablica_Vx[licznik] + Tablica_Vx[licznik + 1])*timer1->Interval/1000 / 2;  //drugie ca³kowanie metod¹ trapezów licz¹ce przemieszczenie
			Tablica_x[licznik+1] = calka_suma_x;
		}

		//Oba ca³kowania w osi y
		void Liczenie_calek_w_y(int licznik)
		{
			Tablica_Vy[licznik + 1] = Tablica_Vy[licznik] - (b / Masa_liczba)*abs(Tablica_Vy[licznik])*timer1->Interval / 1000 - g*timer1->Interval/1000; //pierwsze ca³kowanie metod¹ prostok¹tów licz¹ce prêdkoœæ
			calka_suma_y += (Tablica_Vy[licznik] + Tablica_Vy[licznik + 1])*timer1->Interval / 1000 / 2; //drugie ca³kowanie metod¹ trapezów licz¹ce przemieszczenie
			Tablica_y[licznik + 1] = calka_suma_y;
		}

	protected:
		/// <summary>
		/// Wyczyœæ wszystkie u¿ywane zasoby.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label_predkosc;
	protected:

	private: System::Windows::Forms::TextBox^  textBox_predkosc;
	private: System::ComponentModel::IContainer^  components;
	protected:

	protected:

	protected:


	protected:

	private:
		/// <summary>
		/// Wymagana zmienna projektanta.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Wymagana metoda obs³ugi projektanta — nie nale¿y modyfikowaæ 
		/// zawartoœæ tej metody z edytorem kodu.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->label_predkosc = (gcnew System::Windows::Forms::Label());
			this->textBox_predkosc = (gcnew System::Windows::Forms::TextBox());
			this->button_predkosc = (gcnew System::Windows::Forms::Button());
			this->label2_predkosc = (gcnew System::Windows::Forms::Label());
			this->label_masa = (gcnew System::Windows::Forms::Label());
			this->textBox_masa = (gcnew System::Windows::Forms::TextBox());
			this->button_masa = (gcnew System::Windows::Forms::Button());
			this->label_opor = (gcnew System::Windows::Forms::Label());
			this->label_kat = (gcnew System::Windows::Forms::Label());
			this->textBox_opor = (gcnew System::Windows::Forms::TextBox());
			this->button_opor = (gcnew System::Windows::Forms::Button());
			this->textBox_kat = (gcnew System::Windows::Forms::TextBox());
			this->button_kat = (gcnew System::Windows::Forms::Button());
			this->label_dane = (gcnew System::Windows::Forms::Label());
			this->label2_masa = (gcnew System::Windows::Forms::Label());
			this->label2_opor = (gcnew System::Windows::Forms::Label());
			this->label2_kat = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->button_start_symulacji = (gcnew System::Windows::Forms::Button());
			this->label_poczatkowe_Vx = (gcnew System::Windows::Forms::Label());
			this->label_zasieg = (gcnew System::Windows::Forms::Label());
			this->label_interval = (gcnew System::Windows::Forms::Label());
			this->textBox_interval = (gcnew System::Windows::Forms::TextBox());
			this->button_interval = (gcnew System::Windows::Forms::Button());
			this->label2_interval = (gcnew System::Windows::Forms::Label());
			this->label_aktualna_Vx = (gcnew System::Windows::Forms::Label());
			this->label_poczatkowe_Vy = (gcnew System::Windows::Forms::Label());
			this->label_aktualna_Vy = (gcnew System::Windows::Forms::Label());
			this->label_wysokosc = (gcnew System::Windows::Forms::Label());
			this->label_max_zasieg = (gcnew System::Windows::Forms::Label());
			this->label_max_wysokosc = (gcnew System::Windows::Forms::Label());
			this->label_czas_max_y = (gcnew System::Windows::Forms::Label());
			this->label_czas_max_x = (gcnew System::Windows::Forms::Label());
			this->label_opisowy = (gcnew System::Windows::Forms::Label());
			this->polozenie_wykres = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->polozenie_wykres))->BeginInit();
			this->SuspendLayout();
			// 
			// label_predkosc
			// 
			this->label_predkosc->AutoSize = true;
			this->label_predkosc->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.25F));
			this->label_predkosc->Location = System::Drawing::Point(52, 36);
			this->label_predkosc->Name = L"label_predkosc";
			this->label_predkosc->Size = System::Drawing::Size(183, 17);
			this->label_predkosc->TabIndex = 0;
			this->label_predkosc->Text = L"Prêdkoœæ pocz¹tkowa [m/s]:";
			// 
			// textBox_predkosc
			// 
			this->textBox_predkosc->Location = System::Drawing::Point(243, 34);
			this->textBox_predkosc->Name = L"textBox_predkosc";
			this->textBox_predkosc->Size = System::Drawing::Size(142, 21);
			this->textBox_predkosc->TabIndex = 1;
			this->textBox_predkosc->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
			// 
			// button_predkosc
			// 
			this->button_predkosc->Location = System::Drawing::Point(391, 33);
			this->button_predkosc->Name = L"button_predkosc";
			this->button_predkosc->Size = System::Drawing::Size(87, 22);
			this->button_predkosc->TabIndex = 2;
			this->button_predkosc->Text = L"ZatwierdŸ";
			this->button_predkosc->UseVisualStyleBackColor = true;
			this->button_predkosc->Click += gcnew System::EventHandler(this, &MyForm::button_predkosc_Click);
			// 
			// label2_predkosc
			// 
			this->label2_predkosc->AutoSize = true;
			this->label2_predkosc->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.25F));
			this->label2_predkosc->Location = System::Drawing::Point(691, 38);
			this->label2_predkosc->Name = L"label2_predkosc";
			this->label2_predkosc->Size = System::Drawing::Size(153, 17);
			this->label2_predkosc->TabIndex = 3;
			this->label2_predkosc->Text = L"Prêdkoœæ pocz¹tkowa: ";
			// 
			// label_masa
			// 
			this->label_masa->AutoSize = true;
			this->label_masa->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.25F));
			this->label_masa->Location = System::Drawing::Point(52, 64);
			this->label_masa->Name = L"label_masa";
			this->label_masa->Size = System::Drawing::Size(73, 17);
			this->label_masa->TabIndex = 4;
			this->label_masa->Text = L"Masa [kg]:";
			// 
			// textBox_masa
			// 
			this->textBox_masa->Location = System::Drawing::Point(131, 62);
			this->textBox_masa->Name = L"textBox_masa";
			this->textBox_masa->Size = System::Drawing::Size(142, 21);
			this->textBox_masa->TabIndex = 5;
			// 
			// button_masa
			// 
			this->button_masa->Location = System::Drawing::Point(279, 61);
			this->button_masa->Name = L"button_masa";
			this->button_masa->Size = System::Drawing::Size(87, 22);
			this->button_masa->TabIndex = 6;
			this->button_masa->Text = L"ZatwierdŸ";
			this->button_masa->UseVisualStyleBackColor = true;
			this->button_masa->Click += gcnew System::EventHandler(this, &MyForm::button_masa_Click);
			// 
			// label_opor
			// 
			this->label_opor->AutoSize = true;
			this->label_opor->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.25F));
			this->label_opor->Location = System::Drawing::Point(52, 92);
			this->label_opor->Name = L"label_opor";
			this->label_opor->Size = System::Drawing::Size(130, 17);
			this->label_opor->TabIndex = 7;
			this->label_opor->Text = L"Opór powietrza [N]:";
			// 
			// label_kat
			// 
			this->label_kat->AutoSize = true;
			this->label_kat->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.25F));
			this->label_kat->Location = System::Drawing::Point(52, 119);
			this->label_kat->Name = L"label_kat";
			this->label_kat->Size = System::Drawing::Size(113, 17);
			this->label_kat->TabIndex = 8;
			this->label_kat->Text = L"K¹t wystrza³u [°]:";
			// 
			// textBox_opor
			// 
			this->textBox_opor->Location = System::Drawing::Point(257, 92);
			this->textBox_opor->Name = L"textBox_opor";
			this->textBox_opor->Size = System::Drawing::Size(142, 21);
			this->textBox_opor->TabIndex = 9;
			// 
			// button_opor
			// 
			this->button_opor->Location = System::Drawing::Point(405, 93);
			this->button_opor->Name = L"button_opor";
			this->button_opor->Size = System::Drawing::Size(87, 22);
			this->button_opor->TabIndex = 10;
			this->button_opor->Text = L"ZatwierdŸ";
			this->button_opor->UseVisualStyleBackColor = true;
			this->button_opor->Click += gcnew System::EventHandler(this, &MyForm::button_opor_Click);
			// 
			// textBox_kat
			// 
			this->textBox_kat->Location = System::Drawing::Point(164, 118);
			this->textBox_kat->Name = L"textBox_kat";
			this->textBox_kat->Size = System::Drawing::Size(142, 21);
			this->textBox_kat->TabIndex = 11;
			// 
			// button_kat
			// 
			this->button_kat->Location = System::Drawing::Point(312, 117);
			this->button_kat->Name = L"button_kat";
			this->button_kat->Size = System::Drawing::Size(87, 22);
			this->button_kat->TabIndex = 12;
			this->button_kat->Text = L"ZatwierdŸ";
			this->button_kat->UseVisualStyleBackColor = true;
			this->button_kat->Click += gcnew System::EventHandler(this, &MyForm::button_kat_Click);
			// 
			// label_dane
			// 
			this->label_dane->AutoSize = true;
			this->label_dane->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_dane->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->label_dane->Location = System::Drawing::Point(685, 9);
			this->label_dane->Name = L"label_dane";
			this->label_dane->Size = System::Drawing::Size(175, 20);
			this->label_dane->TabIndex = 13;
			this->label_dane->Text = L"Obecne dane wejœciowe:";
			// 
			// label2_masa
			// 
			this->label2_masa->AutoSize = true;
			this->label2_masa->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.25F));
			this->label2_masa->Location = System::Drawing::Point(691, 66);
			this->label2_masa->Name = L"label2_masa";
			this->label2_masa->Size = System::Drawing::Size(46, 17);
			this->label2_masa->TabIndex = 14;
			this->label2_masa->Text = L"Masa:";
			// 
			// label2_opor
			// 
			this->label2_opor->AutoSize = true;
			this->label2_opor->Location = System::Drawing::Point(691, 93);
			this->label2_opor->Name = L"label2_opor";
			this->label2_opor->Size = System::Drawing::Size(101, 16);
			this->label2_opor->TabIndex = 15;
			this->label2_opor->Text = L"Opór powietrza:";
			// 
			// label2_kat
			// 
			this->label2_kat->AutoSize = true;
			this->label2_kat->Location = System::Drawing::Point(691, 123);
			this->label2_kat->Name = L"label2_kat";
			this->label2_kat->Size = System::Drawing::Size(93, 16);
			this->label2_kat->TabIndex = 16;
			this->label2_kat->Text = L"K¹t wystrza³u: ";
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// button_start_symulacji
			// 
			this->button_start_symulacji->Location = System::Drawing::Point(94, 222);
			this->button_start_symulacji->Name = L"button_start_symulacji";
			this->button_start_symulacji->Size = System::Drawing::Size(141, 38);
			this->button_start_symulacji->TabIndex = 17;
			this->button_start_symulacji->Text = L"Start symulacji";
			this->button_start_symulacji->UseVisualStyleBackColor = true;
			this->button_start_symulacji->Click += gcnew System::EventHandler(this, &MyForm::button_start_symulacji_Click);
			// 
			// label_poczatkowe_Vx
			// 
			this->label_poczatkowe_Vx->AutoSize = true;
			this->label_poczatkowe_Vx->Location = System::Drawing::Point(694, 180);
			this->label_poczatkowe_Vx->Name = L"label_poczatkowe_Vx";
			this->label_poczatkowe_Vx->Size = System::Drawing::Size(165, 16);
			this->label_poczatkowe_Vx->TabIndex = 18;
			this->label_poczatkowe_Vx->Text = L"Prêdkoœæ pocz¹tkowa Vx: ";
			// 
			// label_zasieg
			// 
			this->label_zasieg->AutoSize = true;
			this->label_zasieg->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->label_zasieg->Location = System::Drawing::Point(1084, 92);
			this->label_zasieg->Name = L"label_zasieg";
			this->label_zasieg->Size = System::Drawing::Size(181, 16);
			this->label_zasieg->TabIndex = 19;
			this->label_zasieg->Text = L"Aktualne po³o¿enie w x: 0 [m]";
			// 
			// label_interval
			// 
			this->label_interval->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->label_interval->Location = System::Drawing::Point(52, 146);
			this->label_interval->Name = L"label_interval";
			this->label_interval->Size = System::Drawing::Size(325, 63);
			this->label_interval->TabIndex = 20;
			this->label_interval->Text = L"Okres pomiêdzy kolejnymi iteracjami obliczeñ - Interwa³ zegara ( w milisekundach)"
				L": ";
			// 
			// textBox_interval
			// 
			this->textBox_interval->Location = System::Drawing::Point(378, 155);
			this->textBox_interval->Name = L"textBox_interval";
			this->textBox_interval->Size = System::Drawing::Size(100, 21);
			this->textBox_interval->TabIndex = 21;
			// 
			// button_interval
			// 
			this->button_interval->Location = System::Drawing::Point(484, 153);
			this->button_interval->Name = L"button_interval";
			this->button_interval->Size = System::Drawing::Size(75, 23);
			this->button_interval->TabIndex = 22;
			this->button_interval->Text = L"ZatwierdŸ";
			this->button_interval->UseVisualStyleBackColor = true;
			this->button_interval->Click += gcnew System::EventHandler(this, &MyForm::button_interval_Click);
			// 
			// label2_interval
			// 
			this->label2_interval->AutoSize = true;
			this->label2_interval->Location = System::Drawing::Point(694, 148);
			this->label2_interval->Name = L"label2_interval";
			this->label2_interval->Size = System::Drawing::Size(106, 16);
			this->label2_interval->TabIndex = 23;
			this->label2_interval->Text = L"Interwa³ zegaru: ";
			// 
			// label_aktualna_Vx
			// 
			this->label_aktualna_Vx->AutoSize = true;
			this->label_aktualna_Vx->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238)));
			this->label_aktualna_Vx->Location = System::Drawing::Point(1084, 36);
			this->label_aktualna_Vx->Name = L"label_aktualna_Vx";
			this->label_aktualna_Vx->Size = System::Drawing::Size(144, 16);
			this->label_aktualna_Vx->TabIndex = 24;
			this->label_aktualna_Vx->Text = L"Prêdkoœæ aktualna Vx: ";
			// 
			// label_poczatkowe_Vy
			// 
			this->label_poczatkowe_Vy->AutoSize = true;
			this->label_poczatkowe_Vy->Location = System::Drawing::Point(694, 212);
			this->label_poczatkowe_Vy->Name = L"label_poczatkowe_Vy";
			this->label_poczatkowe_Vy->Size = System::Drawing::Size(166, 16);
			this->label_poczatkowe_Vy->TabIndex = 25;
			this->label_poczatkowe_Vy->Text = L"Prêdkoœæ pocz¹tkowa Vy: ";
			// 
			// label_aktualna_Vy
			// 
			this->label_aktualna_Vy->AutoSize = true;
			this->label_aktualna_Vy->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238)));
			this->label_aktualna_Vy->Location = System::Drawing::Point(1084, 61);
			this->label_aktualna_Vy->Name = L"label_aktualna_Vy";
			this->label_aktualna_Vy->Size = System::Drawing::Size(145, 16);
			this->label_aktualna_Vy->TabIndex = 26;
			this->label_aktualna_Vy->Text = L"Prêdkoœæ aktualna Vy: ";
			// 
			// label_wysokosc
			// 
			this->label_wysokosc->AutoSize = true;
			this->label_wysokosc->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->label_wysokosc->Location = System::Drawing::Point(1084, 117);
			this->label_wysokosc->Name = L"label_wysokosc";
			this->label_wysokosc->Size = System::Drawing::Size(185, 16);
			this->label_wysokosc->TabIndex = 27;
			this->label_wysokosc->Text = L"Aktualne po³o¿enie w y: 0 [m] ";
			// 
			// label_max_zasieg
			// 
			this->label_max_zasieg->AutoSize = true;
			this->label_max_zasieg->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238)));
			this->label_max_zasieg->Location = System::Drawing::Point(1084, 158);
			this->label_max_zasieg->Name = L"label_max_zasieg";
			this->label_max_zasieg->Size = System::Drawing::Size(195, 16);
			this->label_max_zasieg->TabIndex = 28;
			this->label_max_zasieg->Text = L"Uzyskany maksymalny zasiêg: ";
			// 
			// label_max_wysokosc
			// 
			this->label_max_wysokosc->AutoSize = true;
			this->label_max_wysokosc->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238)));
			this->label_max_wysokosc->Location = System::Drawing::Point(1084, 180);
			this->label_max_wysokosc->Name = L"label_max_wysokosc";
			this->label_max_wysokosc->Size = System::Drawing::Size(217, 16);
			this->label_max_wysokosc->TabIndex = 29;
			this->label_max_wysokosc->Text = L"Uzyskana maksymalna wysokoœæ: ";
			// 
			// label_czas_max_y
			// 
			this->label_czas_max_y->AutoSize = true;
			this->label_czas_max_y->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238)));
			this->label_czas_max_y->Location = System::Drawing::Point(1084, 212);
			this->label_czas_max_y->Name = L"label_czas_max_y";
			this->label_czas_max_y->Size = System::Drawing::Size(282, 16);
			this->label_czas_max_y->TabIndex = 30;
			this->label_czas_max_y->Text = L"Czas do osi¹gniêcia maksymalnej wysokoœci:";
			// 
			// label_czas_max_x
			// 
			this->label_czas_max_x->AutoSize = true;
			this->label_czas_max_x->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238)));
			this->label_czas_max_x->Location = System::Drawing::Point(1084, 234);
			this->label_czas_max_x->Name = L"label_czas_max_x";
			this->label_czas_max_x->Size = System::Drawing::Size(149, 16);
			this->label_czas_max_x->TabIndex = 31;
			this->label_czas_max_x->Text = L"Czas ca³kowity lotu kuli:";
			// 
			// label_opisowy
			// 
			this->label_opisowy->AutoSize = true;
			this->label_opisowy->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label_opisowy->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->label_opisowy->Location = System::Drawing::Point(1113, 7);
			this->label_opisowy->Name = L"label_opisowy";
			this->label_opisowy->Size = System::Drawing::Size(87, 22);
			this->label_opisowy->TabIndex = 32;
			this->label_opisowy->Text = L"Symulacja:";
			this->label_opisowy->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// polozenie_wykres
			// 
			chartArea1->Name = L"ChartArea1";
			this->polozenie_wykres->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->polozenie_wykres->Legends->Add(legend1);
			this->polozenie_wykres->Location = System::Drawing::Point(181, 322);
			this->polozenie_wykres->Name = L"polozenie_wykres";
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
			series1->Legend = L"Legend1";
			series1->Name = L"Po³o¿enie kuli";
			this->polozenie_wykres->Series->Add(series1);
			this->polozenie_wykres->Size = System::Drawing::Size(1020, 383);
			this->polozenie_wykres->TabIndex = 33;
			this->polozenie_wykres->Text = L"chart1";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1522, 943);
			this->Controls->Add(this->polozenie_wykres);
			this->Controls->Add(this->label_opisowy);
			this->Controls->Add(this->label_czas_max_x);
			this->Controls->Add(this->label_czas_max_y);
			this->Controls->Add(this->label_max_wysokosc);
			this->Controls->Add(this->label_max_zasieg);
			this->Controls->Add(this->label_wysokosc);
			this->Controls->Add(this->label_aktualna_Vy);
			this->Controls->Add(this->label_poczatkowe_Vy);
			this->Controls->Add(this->label_aktualna_Vx);
			this->Controls->Add(this->label2_interval);
			this->Controls->Add(this->button_interval);
			this->Controls->Add(this->textBox_interval);
			this->Controls->Add(this->label_interval);
			this->Controls->Add(this->label_zasieg);
			this->Controls->Add(this->label_poczatkowe_Vx);
			this->Controls->Add(this->button_start_symulacji);
			this->Controls->Add(this->label2_kat);
			this->Controls->Add(this->label2_opor);
			this->Controls->Add(this->label2_masa);
			this->Controls->Add(this->label_dane);
			this->Controls->Add(this->button_kat);
			this->Controls->Add(this->textBox_kat);
			this->Controls->Add(this->button_opor);
			this->Controls->Add(this->textBox_opor);
			this->Controls->Add(this->label_kat);
			this->Controls->Add(this->label_opor);
			this->Controls->Add(this->button_masa);
			this->Controls->Add(this->textBox_masa);
			this->Controls->Add(this->label_masa);
			this->Controls->Add(this->label2_predkosc);
			this->Controls->Add(this->button_predkosc);
			this->Controls->Add(this->textBox_predkosc);
			this->Controls->Add(this->label_predkosc);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.25F));
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->polozenie_wykres))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}

	// Button ZatwierdŸ-prêdkoœæ pocz¹tkowa
	private: System::Void button_predkosc_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		String^ predkosc_poczatkowa = textBox_predkosc->Text;
		v0 = Convert::ToDouble(predkosc_poczatkowa);
		label2_predkosc->Text = "Prêdkoœæ pocz¹tkowa: " + predkosc_poczatkowa + " [m/s]";

		poczatkowe_Vx = v0*cos(fi*M_PI/180);
		label_poczatkowe_Vx->Text = "Prêdkoœæ pocz¹tkowa Vx: " + poczatkowe_Vx + " [m/s]";
		label_aktualna_Vx->Text = "Prêdkoœæ aktualna Vx: " + poczatkowe_Vx + " [m/s]";

		poczatkowe_Vy = v0*sin(fi*M_PI / 180);
		label_poczatkowe_Vy->Text = "Prêdkoœæ pocz¹tkowa Vy: " + poczatkowe_Vy + " [m/s]";
		label_aktualna_Vy->Text = "Prêdkoœæ aktualna Vy: " + poczatkowe_Vy + " [m/s]";
	}

	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}
	
	// Button ZatwierdŸ-masa
	private: System::Void button_masa_Click(System::Object^  sender, System::EventArgs^  e) {
		String^ masa = textBox_masa->Text;
		Masa_liczba = Convert::ToDouble(masa);
		label2_masa->Text = "Masa: " + masa + " [kg]";
	}

	// Button ZatwierdŸ-opór
	private: System::Void button_opor_Click(System::Object^  sender, System::EventArgs^  e) {
		String^ opor = textBox_opor->Text;
		b = Convert::ToDouble(opor);
		label2_opor->Text = "Opór powietrza: " + opor + " [N]";
	}

	// Button ZatwierdŸ-k¹t wystrza³u
	private: System::Void button_kat_Click(System::Object^  sender, System::EventArgs^  e) {
		String^ kat = textBox_kat->Text;
		fi = Convert::ToDouble(kat);
		label2_kat->Text = "K¹t wystrza³u: " + kat + " [°]";

		poczatkowe_Vx = v0*cos(fi*M_PI / 180);
		label_poczatkowe_Vx->Text = "Prêdkoœæ pocz¹tkowa Vx: " + poczatkowe_Vx + " [m/s]";
		label_aktualna_Vx->Text = "Prêdkoœæ aktualna Vx: " + poczatkowe_Vx + " [m/s]";

		poczatkowe_Vy = v0*sin(fi*M_PI / 180);
		label_poczatkowe_Vy->Text = "Prêdkoœæ pocz¹tkowa Vy: " + poczatkowe_Vy + " [m/s]";
		label_aktualna_Vy->Text = "Prêdkoœæ aktualna Vy: " + poczatkowe_Vy + " [m/s]";
	}

	 // Button Rozpocznij symulacjê 
	private: System::Void button_start_symulacji_Click(System::Object^  sender, System::EventArgs^  e)
	{
		timer1->Stop();
		Czyszczenie_tablic();
		Tablica_Vx[0] = poczatkowe_Vx;
		Tablica_x[0] = 0;
		Tablica_Vy[0] = poczatkowe_Vy;
		Tablica_y[0] = 0;

		licznik_pierwszego_calkowania = 0;
		calka_suma_x = 0;
		calka_suma_y = 0;
		wysokosc_max = 0;
		czas_do_wysokosc_max = 0;
		zasieg_max = 0;

		polozenie_wykres->Series->Clear();
		polozenie_wykres->Series->Add("Po³o¿enie kuli"); // ->ChartType = SeriesChartType.Spline;
		polozenie_wykres->Series["Po³o¿enie kuli"]->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
		polozenie_wykres->Series["Po³o¿enie kuli"]->Points->AddXY(Tablica_x[0], Tablica_y[0]);

		timer1->Start();
	}
	 
	// Button ZatwierdŸ interwa³ czasu
	private: System::Void button_interval_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		String^ string_interval = textBox_interval->Text;
		interval = Convert::ToInt16(string_interval);
		label2_interval->Text = "Interwa³: " + interval + " [ms]";
		timer1->Interval = interval;
	}
	
	//Timer obs³uguj¹cy tworzenie symulacji w czasie rzeczywistym
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
	{

			//Sprawdzanie czy nie przepe³ni siê pamiêæ
			if (licznik_pierwszego_calkowania < maksymalny_rozmiar-1)
			{

				Liczenie_calek_w_x(licznik_pierwszego_calkowania);
				Liczenie_calek_w_y(licznik_pierwszego_calkowania);

				//Zapisywanie wartoœci maksymalnej wysokoœci
				if (Tablica_y[licznik_pierwszego_calkowania + 1] > wysokosc_max)
				{
					wysokosc_max = Tablica_y[licznik_pierwszego_calkowania + 1];
					czas_do_wysokosc_max = double(licznik_pierwszego_calkowania + 1)*timer1->Interval / 1000;
				}

				//Zatrzymanie symulacji w przypadku uderzenia kuli w ziemiê
				if (Tablica_y[licznik_pierwszego_calkowania+1] < 0)
				{
					timer1->Stop();
					Tablica_y[licznik_pierwszego_calkowania + 1] = 0;
					zasieg_max = Tablica_x[licznik_pierwszego_calkowania + 1];

					label_max_zasieg->Text = "Uzyskany maksymalny zasieg: " + zasieg_max + " [m]";
					label_max_wysokosc->Text = "Uzyskana maksymalna wysokosc: " + wysokosc_max + " [m]";
					label_czas_max_x->Text = "Czas ca³kowity lotu kuli: " + double(licznik_pierwszego_calkowania+1)*timer1->Interval/1000 + " [s]";
					label_czas_max_y->Text = "Czas do osi¹gniêcia maksymalnej wysokoœci: " + czas_do_wysokosc_max + " [s]";
					label_aktualna_Vx->Text = "Prêdkoœæ aktualna Vx: 0 [m/s]";
					label_aktualna_Vy->Text = "Prêdkoœæ aktualna Vy: 0 [m/s]";

				}

				//Aktualizacja danych w interfejsie
				if (Tablica_y[licznik_pierwszego_calkowania + 1] > 0)
				{
					label_aktualna_Vx->Text = "Prêdkoœæ aktualna Vx: " + Tablica_Vx[licznik_pierwszego_calkowania + 1] + " [m/s]";
					label_aktualna_Vy->Text = "Prêdkoœæ aktualna Vy: " + Tablica_Vy[licznik_pierwszego_calkowania + 1] + " [m/s]";
				}

				label_zasieg->Text = "Aktualne po³o¿enie w x: " + Tablica_x[licznik_pierwszego_calkowania + 1] + " [m]";
				label_wysokosc->Text = "Aktualne po³o¿enie w y: " + Tablica_y[licznik_pierwszego_calkowania+1] + " [m]";
				

				/*
				if (abs(Tablica_x[licznik_pierwszego_calkowania-1] - Tablica_x[licznik_pierwszego_calkowania]) < dokladnosc)  
					timer1->Stop();
				*/

				//Rysowanie wykresu w czasie rzeczywistym
				this->polozenie_wykres->Series["Po³o¿enie kuli"]->Points->AddXY(Tablica_x[licznik_pierwszego_calkowania+1], Tablica_y[licznik_pierwszego_calkowania+1]);

				licznik_pierwszego_calkowania++;

			}
			else timer1->Stop();

	}

};
}
