//CopyRight by Igor Kisin ВСЕ ПРАВА ЗАЩИЩЕНЫ СЛАВА ПИЛОТАМ ЕВАНГЕЛИОНОВ

/************************** Библиотеки *****************************************/
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <strsafe.h>
#include <locale.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <iostream>
#pragma comment(lib,"Winmm.lib")

using namespace std;

/************************** Макросы *******************************************/

#define EnterKeyCode 13 //Код клавиши ввода (по умолчанию 13 - Enter)
#define EscapeKeyCode 27 //Код клавиши отмены (по умолчанию 27 - Escape)
#define UpKeyCode 72 //Код клавиши перехода наверх (по умолчанию 72 - стрелка вверх)
#define DownKeyCode 80 //Код клавиши перехода вниз (по умолчанию 80 - стрелка вниз)
#define EscapeKeyCode 27 //Код клавиши отмены (по умолчанию 27 - Escape)
#define DirectorNameSize 15 //На сколько большое может быть имя у директора
#define BinFileName "Autobases.db" //Путь/имя файла сохранения/чтения бинарной таблицы
#define FormatString "|    %-7d|   %-18s|      %-21f|             %-12d|\n"

/************************** Константы *****************************************/

const int MAXMENUITEM = 11; //Максимальное количество элементов на главном меню
const int MAXMENUCHOISEITEM = 4;//Максимальное количество элементов на меню выбора
const int MAXMENUSUREITEM = 3;

/*********************** Глобальные переменные ****************************/

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
struct node* beg, * back; //Указатели на начало и конец списка
int NodesCount = 0;//Количество нод в списке
int width, height;
enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

char ImportFileName[255]; //Имя файла импорта записей
char ExportFileName[255]; //Имя файла экспорта записей
//Основное меню
const char* items[] =
{
	"      ___________  СВЕДЕНИЯ О РАСХОДЕ ТОПЛИВА  ____________",
	"               Импорт таблицы из файла                 ",//1
	"                  Просмотр таблицы                     ",//2
	"           Добавление новой записи в таблицу           ",//3
	"                   Удаление записи                     ",//4
	"             Корректировка записи в таблице            ",//5
	"                   Сотрировка таблицы                  ",//6
	"                 Поиск записи в таблице                ",//7
	"                 Экспорт данных в файл                 ",//Приказ
	"   Обработка таблицы и просмотр результатов обработки  ",//9
	"                _______  Выход  _______                " //10
};

const char* choiseOFaverageITEMS[] =
{
	"  _____________________   Выберите пункт   ________________________",
	"  Подсчитать средний расход топлива на одну машину по каждой базе  ",
	"          Подсчитать средний расход топлива по городу              ",
	"  назад  "
};

const char* choiseOFSaveITEMS[] =
{
	"__ Какой тип файла вам нужен для сохранения данных об автобазах? __",
	"                             Текстовый                             ",
	"                             Бинарный                              ",
	"  назад  "
};

const char* choiseOFLoadITEMS[] =
{
	"      __________  Какого типа файл вы хотите импортировать?  ____________",
	"                             Текстовый                             ",
	"                             Бинарный                              ",
	"  назад  "
};

const char* choiseOFSortITEMS[] =
{
	"______________  Как вы хотите сортировать список?  ________________",
	"                         По возрастанию                        ",
	"                         По убыванию                           ",
	"  назад  "
};

struct AutoBase
{
	char
		Director[DirectorNameSize];
	float FuelPOTRACHENO;
	int   CarCount,
		ABnomber;
};

struct node
{
	struct AutoBase info;
	struct node* prev = 0;
	struct node* next = 0;
};

/********************* Прототипы функций ***********************************/
int Menu(); //меню
void PrintMenu(int item, const char** itemshow, const int maxel); //Отрисовка пунктов меню

void TextLoad(); //Импорт таблицы из текстового файла
void BinLoad(); //Загрузка данных из бинарного файла
void LoadInterface();

void TextSave(); //Экспорт таблицы в текстовый файл
void BinSave(); //Сохранение таблицы в бинарный файл
void SaveInterface();

void sEl(node* beg); //Поиск записи по ключу
void SortListUP(); //Сортировка по возрастанию
void SortListDOWN(); //Сортировка по убыванию
void SortInterface();

void PrintTable(node* beg);//Печать таблицы на экран

void Delete(); //Удаление таблицы (освобождение памяти)
void DeleteElement(struct node* _node); //удаление элемента
void DeleteInterface();

void CorrectElement(struct node* temp);//Редактирование элемента
void CorrElInterface();

void CreateElement();//добавление элемента

void Average1(node* beg); //средний расход топлива на одну машину по каждой базе
void AverageInCityHeroSevastopol(node* beg); //средний расход топлева по городу ГЕРОЮ Севастополю
void AverageChoise();//у каждого есть выбор
void SovyetFlag();
void HolyPlus();
void SevSUCool();

void setColor(ConsoleColor text, ConsoleColor background)
{
	SetConsoleTextAttribute(hConsole, (background << 4) | text);
}

void setPos(COORD& c)
{
	SetConsoleCursorPosition(hConsole, c);
}
void gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}

int main() {
	COORD asd = { 12, 11 };
	setPos(asd);

	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setColor(Yellow, Red);
	//HANDLE hCon;
	// вытаскиваем ширину и высоту 
	//hCon = GetStdHandle(-12);
	//CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	/*if (GetConsoleScreenBufferInfo(hCon, &consoleInfo))
	{
		width = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
		height = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
	}*/
	//gotoxy((width / 2) - 6, (height / 2) - 3);
	//ShowWindow(GetConsoleWindow(), SW_MAXIMIZE); // полноэкранный режим
	while (1)
	{

		switch (Menu())
		{
		case 1: LoadInterface();
			break;
		case 2: PrintTable(beg);
			break;
		case 3: CreateElement();
			break;
		case 4: DeleteInterface();
			break;
		case 5: CorrElInterface();
			break;
		case 6: SortInterface();
			break;
		case 7: sEl(beg);
			break;
		case 8: SaveInterface();
			break;
		case 9: AverageChoise();
			break;
		case 10:
			return 0;
		default:
			cin.get();
			break;
		}
	}
	return 0;
}

void DrawAffirmationItems(int Item)
{
	if (Item == 1)
	{
		printf("\n$-> Нет <-$\n");
		printf("    Да    ");
	}
	else if (Item == 2)
	{
		printf("\n    Нет    \n");
		printf("-$> Да <$-");
	}
}

void DeleteInterface()
{
	int n;
	system("cls");
	cout << "Ведите номер автобазы, которую хотите удалить: ";
	cin >> n;
	node* temp = beg;
	while (temp != back)
	{
		if (temp->info.ABnomber == n)
			break;
		temp = temp->next;

	}
	if (temp->info.ABnomber != n)
	{
		cout << "Нет такой автобазы...";
		cin.get();
		return;
	}
	system("cls");
	printf("Автобаза %d будет удалена!\n", n);
	printf("Вы уверены?");
	char c;
	int MenuItem = 1;
	DrawAffirmationItems(MenuItem);
	while (c = _getch()) //Получаем номер пукнта меню
	{
		if (c == EnterKeyCode) break;
		else if (c == UpKeyCode && MenuItem > 1) MenuItem--;
		else if (c == DownKeyCode && MenuItem < 2)	MenuItem++;
		system("cls");
		printf("Автобаза %d будет удалена!\n", n);
		printf("Вы уверены?");
		DrawAffirmationItems(MenuItem);
	}
	switch (MenuItem)
	{
	case 1:
		return;
		break;
	case 2:
		system("cls");
		DeleteElement(temp);
		printf("Автобаза %d удалена\n", n);
		system("pause");
		break;
	}
	return;
}



void CorrectElement(struct node* temp)
{
	system("cls");
	printf("Редактирование элемента:\n");
	struct node* ab = new node;
	ab = beg;
	struct AutoBase* temp1 = new AutoBase;

	printf("\nИзменить номер автобазы %d на: ", temp->info.ABnomber);
	char a[999];
	memset(a, 0, 999);
	while (getchar() != '\n' && !feof(stdin));
	gets_s(a);
	int m = atoi(a);
	while (ab->next != 0)
	{
		if (m == ab->info.ABnomber)
		{
			cout << "Автобаза под таким номером уже сужествует!\n";
			system("pause");
			CorrectElement(temp);
			int n = -842150451;
			node* delete_temp = beg;
			while (delete_temp != back)
			{
				if (delete_temp->info.ABnomber == n)
				{
					DeleteElement(delete_temp);
					break;
				}
				delete_temp = delete_temp->next;
			}
			return;
		}
		ab = ab->next;
	}
	if (ab->info.ABnomber != m)
		temp->info.ABnomber = atoi(a);

	printf("\nИзменить имя директора \"%s\" на: ", temp->info.Director);
	gets_s(a);
	strcpy_s(temp->info.Director, a);

	printf("\nИзменить затраты на топливо с %f на: ", temp->info.FuelPOTRACHENO);
	gets_s(a);
	temp->info.FuelPOTRACHENO = atof(a);

	printf("\nИзменить количетсво автомобилей с %d на: ", temp->info.CarCount);
	gets_s(a);
	temp->info.CarCount = atoi(a);

	printf("Элемент был отредактирован");

};

void CorrElInterface()
{
	int n;
	system("cls");
	cout << "Ведите номер автобазы, информацию о которой хотите отредактировать: ";
	cin >> n;
	node* temp = beg;
	while (temp != back)
	{
		if (temp->info.ABnomber == n)
			break;
		temp = temp->next;

	}
	if (temp->info.ABnomber != n)
	{
		cout << "Нет такой автобазы...";
		cin.get();
		return;
	}
	CorrectElement(temp);
	system("pause");
	return;
};

void Average1(node* beg)
{
	system("cls");
	printf("Средний расход топлива ");
	float a;
	while (beg != 0)
	{
		a = (beg->info.FuelPOTRACHENO) / (beg->info.CarCount);
		printf("по базе %d равен %f\n", beg->info.ABnomber, a);
		cout << endl;
		printf("                       ");
		beg = beg->next;
	}
	system("pause");
}

void AverageInCityHeroSevastopol(node* beg)
{
	system("cls");
	float a = 0,
		b = 0,
		c = 0;
	while (beg != 0)
	{
		a = a + beg->info.FuelPOTRACHENO;
		b = b + beg->info.CarCount;
		beg = beg->next;
	}
	c = a / b;
	printf("Средний расход топлива городу равен %f\n", c);
	system("pause");
}

int ChoiseAverageMenu()
{
	int MenuItem = 1;
	PrintMenu(MenuItem, choiseOFaverageITEMS, MAXMENUCHOISEITEM);
	char c;
	while (c = _getch())
	{
		if (c == EscapeKeyCode) return 3;
		if (c == EnterKeyCode) return MenuItem;
		else if (c == DownKeyCode && MenuItem < MAXMENUCHOISEITEM - 1) MenuItem++;
		else if (c == UpKeyCode && MenuItem > 1) MenuItem--;
		PrintMenu(MenuItem, choiseOFaverageITEMS, MAXMENUCHOISEITEM);
	};
	return 0;
}

void AverageChoise()
{
	system("cls");

	while (1)
	{
		switch (ChoiseAverageMenu())
		{
		case 1: Average1(beg);
			break;
		case 2: AverageInCityHeroSevastopol(beg);
			break;
		case 3:
			return;
		default:
			break;
		}
	}
	return;
}

void Print(const node& t)
{
	printf_s("Номер автобазы: %d\n", t.info.ABnomber);
	cout << "Имя директора: " << t.info.Director << endl;
	printf_s("Израсходовано топлива: %f\n", t.info.FuelPOTRACHENO);
	printf_s("Количество автомобилей : %d\n", t.info.CarCount);
}

void DrawSerchItems(int Item)
{
	if (Item == 1)
	{
		printf("\n$-> Номер автобазы <-$\n");
		printf("    Фамилия директора\n");
		printf("    Топлива потрачено\n");
		printf("    Количество автомобилей\n");
	}
	else if (Item == 2)
	{
		printf("\n    Номер автобазы\n");
		printf("$-> Фамилия директора <-$\n");
		printf("    Топлива потрачено\n");
		printf("    Количество автомобилей\n");
	}
	else if (Item == 3)
	{
		printf("\n    Номер автобазы\n");
		printf("    Фамилия директора\n");
		printf("$-> Топлива потрачено <-$\n");
		printf("    Количество автомобилей\n");
	}
	else if (Item == 4)
	{
		printf("\n    Номер автобазы\n");
		printf("    Фамилия директора\n");
		printf("    Топлива потрачено\n");
		printf("$-> Количество автомобилей <-$\n");
	}
}

void sEl(node* beg)
{
	int n, cc;
	char d[DirectorNameSize];
	float f;
	system("cls");
	cout << "По какому полю осуществить поиск?\n";
	char c;
	int MenuItem = 1;
	DrawSerchItems(MenuItem);
	while (c = _getch()) //Получаем номер пукнта меню
	{
		if (c == EnterKeyCode) break;
		else if (c == UpKeyCode && MenuItem > 1) MenuItem--;
		else if (c == DownKeyCode && MenuItem < 4)	MenuItem++;
		system("cls");
		cout << "По какому полю осуществить поиск?\n";
		DrawSerchItems(MenuItem);
	}
	switch (MenuItem)
	{
	case 1:
		system("cls");
		cout << "Введите нормер нужной автобазы:\n";
		cin >> n;
		while (beg->next != 0)
		{
			if (n == beg->info.ABnomber)
			{
				Print(*beg);
				break;
			}
			beg = beg->next;
		}
		if (beg->info.ABnomber != n)
		{
			cout << "Нет автобазы с номером " << n << endl;
		}
		system("pause");
		break;
		return;

	case 2:
		system("cls");
		cout << "Введите фамилию директора нужной автобазы:\n";
		gets_s(d);
		while (beg->next != 0)
		{
			if (!strcmp(d, beg->info.Director))
			{
				Print(*beg);
				break;
			}
			beg = beg->next;
		}
		if (beg->info.Director != d)
		{
			cout << "Нет автобазы с директором " << d << endl;
		}
		system("pause");
		break;
		return;

	case 3:
		system("cls");
		cout << "Введите количество потраченного топлива на нужной автобазе:\n";
		cin >> f;
		while (beg->next != 0)
		{
			if (f == beg->info.FuelPOTRACHENO)
			{
				Print(*beg);
				break;
			}
			beg = beg->next;
		}
		if (beg->info.FuelPOTRACHENO != f)
		{
			cout << "Нет автобазы с такими затратами топлива\n";
		}
		system("pause");
		break;
		return;

	case 4:
		system("cls");
		cout << "Введите количество автомобилей на нужной автобазе:\n";
		cin >> cc;
		while (beg->next != 0)
		{
			if (cc == beg->info.CarCount)
			{
				Print(*beg);
				break;
			}
			beg = beg->next;
		}
		if (beg->info.CarCount != cc)
		{
			cout << "Нет автобазы с таким количеством машин\n";
		}
		system("pause");
		break;
		return;
	}

}

void PrintMenu(int item, const char** itemshow, const int maxel)
{
	system("cls");
	COORD asd = { 22, 8 };
	setPos(asd);
	//gotoxy((width / 2) - 6, (height / 2) - 3);
	int i = 1;
	cout << itemshow[0] << endl;
	for (; i < item; i++) {
		asd.Y++;
		setPos(asd);
		cout << "      " << itemshow[i] << endl;
	}
	asd.Y++;
	setPos(asd);
	cout << "=====>" << itemshow[i++] << "<=====\n";
	for (; i < maxel; i++) {
		asd.Y++;
		setPos(asd);
		cout << "      " << itemshow[i] << endl;
	}
};

int Menu() {
	int MenuItem = 1;
	PrintMenu(MenuItem, items, MAXMENUITEM);
	char c;
	while (c = _getch())
	{
		if (c == 83) { HolyPlus(); return -1; }
		if (c == 115) { SovyetFlag(); return -1; }
		if (c == 117) { SevSUCool(); return -1; }
		if (c == EnterKeyCode) return MenuItem;
		else if (c == DownKeyCode && MenuItem < MAXMENUITEM - 1) MenuItem++;
		else if (c == UpKeyCode && MenuItem > 1) MenuItem--;
		PrintMenu(MenuItem, items, MAXMENUITEM);
	};
	return 0;
}

void Delete()
{
	struct node* temp = beg;
	while (temp != back)
	{
		struct node* temp1 = temp->next;
		free(temp);
		temp = temp1;
	}
	beg = NULL; back = NULL;
}

void DeleteElement(struct node* _node)
{
	if (beg == back)
	{
		free(_node);
		beg = NULL;
		back = NULL;
	}
	else if (_node == beg)
	{
		beg = beg->next;
		beg->prev = NULL;
		free(_node);
	}
	else if (_node == back)
	{
		back = back->prev;
		back->next = NULL;
		free(_node);
	}
	else
	{
		_node->prev->next = _node->next;
		_node->next->prev = _node->prev;
		free(_node);
	}
	NodesCount--;
}

int GetStructLine(FILE* in, char* temp, int size)
{
	char c;
	int i = 0;
	int flag = 0;
	while (c = fgetc(in))
	{
		if (c == EOF) return 0;
		if (c != '.' && !flag) continue;
		else if (c == '.')
		{
			if (flag == 0)
			{
				flag = 1;
				c = fgetc(in);
			}
			else break;
		}
		if (i < size)
			temp[i++] = c;
		else break;
	}
	c = getc(in);
	temp[i] = '\0';
	return 1;
}

void TextLoad() {


	if (beg != NULL || back != NULL)
	{
		system("cls");
		printf("Чтобы загрузить таблицу, нужно удалить существующую...\n");
		printf("Удалить существующую таблицу?");
		char c;
		int MenuItem = 1;
		DrawAffirmationItems(MenuItem);
		while (c = _getch()) //Получаем номер пукнта меню
		{
			if (c == EnterKeyCode) break;
			else if (c == UpKeyCode && MenuItem > 1) MenuItem--;
			else if (c == DownKeyCode && MenuItem < 2)	MenuItem++;
			system("cls");
			printf("Чтобы загрузить таблицу, нужно удалить существующую...\n");
			printf("Удалить существующую таблицу?");
			DrawAffirmationItems(MenuItem);
		}
		switch (MenuItem)
		{
		case 1:
			return;
			break;
		case 2:
			system("cls");
			Delete();
			printf("Таблица удалена!\n");
			system("pause");
			break;
		}
	}

	system("cls");

	printf("Введите путь к импортируемому файлу с таблицей:\n");
	cin >> ImportFileName;
	cin.ignore(INT_MAX, '\n');

	errno_t err;
	FILE* in;


	err = fopen_s(&in, ImportFileName, "r");
	if (err != 0)
	{
		printf("Не удалось открыть файл для импорта...\n");
		system("pause");
		return;
	}

	int c;
	int RecordCount = 1;

	fscanf_s(in, "%d", &NodesCount);

	beg = new node;
	beg->prev = NULL;

	fscanf_s(in, "%d", &beg->info.ABnomber);
	GetStructLine(in, beg->info.Director, DirectorNameSize);
	fscanf_s(in, "%f", &beg->info.FuelPOTRACHENO);
	fscanf_s(in, "%d", &beg->info.CarCount);

	struct node* temp_node = beg;

	for (; RecordCount < NodesCount; RecordCount++)
	{
		temp_node->next = new node;
		temp_node->next->prev = temp_node;
		temp_node = temp_node->next;

		fscanf_s(in, "%d", &temp_node->info.ABnomber);
		GetStructLine(in, temp_node->info.Director, DirectorNameSize);
		fscanf_s(in, "%f", &temp_node->info.FuelPOTRACHENO);
		fscanf_s(in, "%d", &temp_node->info.CarCount);
	}
	back = temp_node;
	back->next = NULL;

	printf("Загружено записей:"); printf("%d\n", RecordCount);
	fclose(in);
	system("pause");
}

void BinLoad()
{
	if (beg != NULL || back != NULL)
	{
		system("cls");
		printf("Чтобы загрузить таблицу, нужно удалить существующую...\n");
		printf("Удалить существующую таблицу?");
		char c;
		int MenuItem = 1;
		DrawAffirmationItems(MenuItem);
		while (c = _getch()) //Получаем номер пукнта меню
		{
			if (c == EnterKeyCode) break;
			else if (c == UpKeyCode && MenuItem > 1) MenuItem--;
			else if (c == DownKeyCode && MenuItem < 2)	MenuItem++;
			system("cls");
			printf("Чтобы загрузить таблицу, нужно удалить существующую...\n");
			printf("Удалить существующую таблицу?");
			DrawAffirmationItems(MenuItem);
		}
		switch (MenuItem)
		{
		case 1:
			return;
			break;
		case 2:
			system("cls");
			Delete();
			printf("Таблица удалена!\n");
			system("pause");
			break;
		}
	}
	system("cls");

	errno_t err;
	FILE* in = NULL;
	err = fopen_s(&in, BinFileName, "rb");
	if (err != 0)
	{
		printf("Не удалось открыть файл для загрузки...\n");
		system("pause");
		return;
	}

	fread(&NodesCount, sizeof(NodesCount), 1, in);
	beg = new node;
	fread(beg, sizeof(struct node), 1, in);
	struct node* temp = beg;
	for (int i = 1; i < NodesCount; i++)
	{
		temp->next = new node;
		fread(temp->next, sizeof(struct node), 1, in);
		temp->next->prev = temp;
		temp = temp->next;
	}
	back = temp;
	fclose(in);
	printf("Бинарный файл найден\n");
	printf("Загружено записей: "); printf("%d\n", NodesCount);
	system("pause");
}

int ChoiseLoadMenu()
{
	int MenuItem = 1;
	PrintMenu(MenuItem, choiseOFLoadITEMS, MAXMENUCHOISEITEM);
	char c;
	while (c = _getch())
	{
		if (c == EscapeKeyCode) return 3;
		if (c == EnterKeyCode) return MenuItem;
		else if (c == DownKeyCode && MenuItem < MAXMENUCHOISEITEM - 1) MenuItem++;
		else if (c == UpKeyCode && MenuItem > 1) MenuItem--;
		PrintMenu(MenuItem, choiseOFLoadITEMS, MAXMENUCHOISEITEM);
	};
	return 0;
}

void LoadInterface()
{
	system("cls");

	while (1)
	{
		switch (ChoiseLoadMenu())
		{
		case 1: TextLoad();
			return;
		case 2: BinLoad();
			return;
		case 3:
			return;
		default:
			break;
		}
	}
	return;
}

void PrintTable(node* beg)
{
	char c;
	int elonmask,
		pages,
		lastpage,
		k = 0;
	if (!beg)
	{
		system("cls");  cout << "Данные отсутствуют..." << endl;
		cout << "Сначала необходимо импортировать таблицу из файла или создать новую" << endl;
		system("pause");
		return;
	}
	//firstPage:
	elonmask = 1;
	pages = 1;
	node* temp = beg; //указатель temp устанавливаем в начало
	system("cls");
	printf("Escape - Выход, ");
	printf("Стрелки вверх, вниз - Навигация по листам\n");
	printf("                                     АВТОБАЗЫ СЕВАСТОПОЛЯ\n");
	printf("№ АВТОБАЗЫ     ИМЯ ДИРЕКТОРА           ТОПЛИВА ПОТРАЧЕНО           КОЛИЧЕСТВО АВТОМОБИЛЕЙ\n");

	while (temp && elonmask != 6)
	{
		printf("+-----------+---------------------+---------------------------+-------------------------+\n");
		printf("|           |                     |                           |                         |\n");
		printf_s(FormatString, temp->info.ABnomber, temp->info.Director, temp->info.FuelPOTRACHENO, temp->info.CarCount);
		printf("|           |                     |                           |                         |\n");
		printf("+-----------+---------------------+---------------------------+-------------------------+\n");

		temp = temp->next;
		elonmask++;
	}
	cout << "Страница 1";

	while (c = _getch())
	{
		if (c == UpKeyCode)
		{
			if (pages != 1)
			{
				k = 0;
				pages--;
				if (temp && temp->next != NULL)
				{
					temp = temp->prev;
					elonmask--;
				}
				while (k != elonmask + 4)
				{
					temp = temp->prev;
					k++;
				}

				system("cls");
				printf("Escape - Выход, ");
				printf("Стрелки вверх, вниз - Навигация по листам\n");
				printf("                                     АВТОБАЗЫ СЕВАСТОПОЛЯ\n");
				printf("№ АВТОБАЗЫ     ИМЯ ДИРЕКТОРА           ТОПЛИВА ПОТРАЧЕНО           КОЛИЧЕСТВО АВТОМОБИЛЕЙ\n");
				elonmask = 1;
				while (temp && elonmask != 6)
				{
					printf("+-----------+---------------------+---------------------------+-------------------------+\n");
					printf("|           |                     |                           |                         |\n");
					printf_s(FormatString, temp->info.ABnomber, temp->info.Director, temp->info.FuelPOTRACHENO, temp->info.CarCount);
					printf("|           |                     |                           |                         |\n");
					printf("+-----------+---------------------+---------------------------+-------------------------+\n");
					if (temp->next == nullptr)break;
					temp = temp->next;
					elonmask++;

				}
				cout << "Страница " << pages;
			}
		}


		if (c == DownKeyCode)
		{
			if (temp && temp->next != NULL)
			{
				elonmask = 1;
				system("cls");
				printf("Escape - Выход, ");
				printf("Стрелки вверх, вниз - Навигация по листам\n");
				printf("                                     АВТОБАЗЫ СЕВАСТОПОЛЯ\n");
				printf("№ АВТОБАЗЫ     ИМЯ ДИРЕКТОРА           ТОПЛИВА ПОТРАЧЕНО           КОЛИЧЕСТВО АВТОМОБИЛЕЙ\n");
				while (temp && elonmask != 6)
				{
					printf("+-----------+---------------------+---------------------------+-------------------------+\n");
					printf("|           |                     |                           |                         |\n");
					printf_s(FormatString, temp->info.ABnomber, temp->info.Director, temp->info.FuelPOTRACHENO, temp->info.CarCount);
					printf("|           |                     |                           |                         |\n");
					printf("+-----------+---------------------+---------------------------+-------------------------+\n");
					if (temp->next == nullptr)break;
					temp = temp->next;
					elonmask++;
				}
				pages++;
				lastpage = pages + 1;
				cout << "Страница " << pages;
			}
			else if (elonmask == 6)
			{
				system("cls");
				printf("Escape - Выход, ");
				printf("Стрелки вверх, вниз - Навигация по листам\n");
				printf("                                     АВТОБАЗЫ СЕВАСТОПОЛЯ\n");
				printf("№ АВТОБАЗЫ     ИМЯ ДИРЕКТОРА           ТОПЛИВА ПОТРАЧЕНО           КОЛИЧЕСТВО АВТОМОБИЛЕЙ\n");
				printf("+-----------+---------------------+---------------------------+-------------------------+\n");
				printf("|           |                     |                           |                         |\n");
				printf_s(FormatString, temp->info.ABnomber, temp->info.Director, temp->info.FuelPOTRACHENO, temp->info.CarCount);
				printf("|           |                     |                           |                         |\n");
				printf("+-----------+---------------------+---------------------------+-------------------------+\n");
				cout << "Страница " << lastpage;
				pages = lastpage;
				elonmask = 1;
			}
		}


		if (c == EscapeKeyCode) return;
	};

}

void SortListUP()
{
	for (int i = 0; i < NodesCount - 1; i++)
	{
		struct node* temp = back;
		for (int j = (NodesCount - 1); j > i; j--) // для всех элементов после i-ого
		{
			if (temp->prev->info.ABnomber > temp->info.ABnomber)
			{
				if (temp == back)
				{
					struct node* tempprev = temp->prev;
					tempprev->prev->next = temp;
					temp->prev = tempprev->prev;
					temp->next = tempprev;
					tempprev->prev = temp;
					tempprev->next = NULL;
					back = tempprev;
					temp = tempprev;
				}
				else if (temp == beg->next)
				{
					struct node* tempnext = temp->next;
					beg->next = tempnext;
					beg->prev = temp;
					tempnext->prev = beg;
					temp->prev = NULL;
					temp->next = beg;
					beg = temp;
					temp = beg->next;
				}
				else
				{
					struct node* tempprev = temp->prev;
					temp->next->prev = tempprev;
					tempprev->prev->next = temp;
					temp->prev = tempprev->prev;
					tempprev->next = temp->next;
					temp->next = tempprev;
					tempprev->prev = temp;
					temp = tempprev;
				}
			}
			temp = temp->prev;
		}
	}
	system("cls");
	cout << "Таблица успешно отсортирована по возрастанию\n";
	system("pause");
	return;
}

void SortListDOWN()
{
	for (int i = 0; i < NodesCount - 1; i++)
	{
		struct node* temp = back;
		for (int j = (NodesCount - 1); j > i; j--) // для всех элементов после i-ого
		{
			if (temp->prev->info.ABnomber < temp->info.ABnomber)
			{
				if (temp == back)
				{
					struct node* tempprev = temp->prev;
					tempprev->prev->next = temp;
					temp->prev = tempprev->prev;
					temp->next = tempprev;
					tempprev->prev = temp;
					tempprev->next = NULL;
					back = tempprev;
					temp = tempprev;
				}
				else if (temp == beg->next)
				{
					struct node* tempnext = temp->next;
					beg->next = tempnext;
					beg->prev = temp;
					tempnext->prev = beg;
					temp->prev = NULL;
					temp->next = beg;
					beg = temp;
					temp = beg->next;
				}
				else
				{
					struct node* tempprev = temp->prev;
					temp->next->prev = tempprev;
					tempprev->prev->next = temp;
					temp->prev = tempprev->prev;
					tempprev->next = temp->next;
					temp->next = tempprev;
					tempprev->prev = temp;
					temp = tempprev;
				}
			}
			temp = temp->prev;
		}
	}
	system("cls");
	cout << "Таблица успешно отсортирована по убыванию\n";
	system("pause");
	return;
}

int SortMenu()
{
	int MenuItem = 1;
	PrintMenu(MenuItem, choiseOFSortITEMS, MAXMENUCHOISEITEM);
	char c;
	while (c = _getch())
	{
		if (c == EscapeKeyCode) return 3;
		if (c == EnterKeyCode) return MenuItem;
		else if (c == DownKeyCode && MenuItem < MAXMENUCHOISEITEM - 1) MenuItem++;
		else if (c == UpKeyCode && MenuItem > 1) MenuItem--;
		PrintMenu(MenuItem, choiseOFSortITEMS, MAXMENUCHOISEITEM);
	};
	return 0;
}

void SortInterface()
{
	system("cls");
	cout << "Ключевым полем является Номер автобазы\n";
	system("pause");
	while (1)
	{
		switch (SortMenu())
		{
		case 1: SortListUP();
			return;
		case 2: SortListDOWN();
			return;
		case 3:
			return;
		default:
			break;
		}
	}
	return;
}


void NOwords(char* a)
{
	char nol[999] = { "0" };
	char nolend[999] = { "0\n" };
	//memset(nol, 0, 999);
	do
	{
		if (!strcmp(a, nol)) return;
		printf("\nВводить нужно только числа!\n");
		fgets(a, 999, stdin);
		if (!strcmp(a, nolend)) return;

	} while (!atof(a));
}

void CreateElement()
{
	system("cls");
	printf("____Добавление нового элемента____\n");
	if (beg != NULL || back != NULL)
	{
		struct node* ab = new node;
		ab = beg;
		struct node* temp = new node;



		char a[999];
		memset(a, 0, 999);
		printf("\nВведите номер автобазы:");
		//while (!feof(stdin) && getchar() != '\n' );
		fflush(stdin);
		//cin.ignore(INT_MAX, '\n');
		gets_s(a);
		int m = atoi(a);
		while (ab->next != 0)
		{
			if (m == ab->info.ABnomber)
			{
				cout << "Автобаза под таким номером уже сужествует!\n";
				system("pause");
				CreateElement();
				/*int n = -842150451;
				node* delete_temp = beg;
				while (delete_temp != back)
				{
					if (delete_temp->info.ABnomber == n || delete_temp->info.CarCount == n)
					{
						DeleteElement(delete_temp);
						break;
					}
					delete_temp = delete_temp->next;
				}*/
				return;
			}
			ab = ab->next;
		}
		if (ab->info.ABnomber != m)
			temp->info.ABnomber = atoi(a);
		if (temp->info.ABnomber == 0)
		{
			printf("\nНомер автобазы может быть только целочисленным и натуральным!\n");
			system("pause");
			CreateElement();
			/*int n = -842150451;
			node* delete_temp = beg;
			while (delete_temp != back)
			{
				if (delete_temp->info.ABnomber == n || delete_temp->info.CarCount == n)
				{
					DeleteElement(delete_temp);
					break;
				}
				delete_temp = delete_temp->next;
			}*/
			return;
		}

		printf("\nВведите имя директора:");
		gets_s(a);
		fflush(stdin);
		//fgets(a, 15, stdin);
		//cin.ignore(INT_MAX, '\n');
		//cout << strlen(a);
		if (strlen(a) < 1)
			strcpy_s(temp->info.Director, "vacancy");
		else
			strcpy_s(temp->info.Director, a);



		printf("\nТоплива потрачено:");
		gets_s(a);
		if (!atoi(a))
			NOwords(a);

		temp->info.FuelPOTRACHENO = atof(a);

		printf("\nВведите количетсво автомобилей:");
		gets_s(a);
		if (!atoi(a))
			NOwords(a);

		temp->info.CarCount = atoi(a);

		/*int n = -842150451;
		node* delete_temp = beg;
		while (delete_temp != back)
		{
			if (delete_temp->info.ABnomber == n || delete_temp->info.CarCount == n)
			{
				DeleteElement(delete_temp);
				break;
			}
			delete_temp = delete_temp->next;
		}*/
		back->next = temp;
		temp->prev = back;
		temp->next = NULL;
		back = temp;
		NodesCount++;
		system("pause");
		return;
	};
	// Если элементов нет
	beg = new node;
	beg->prev = NULL;
	back = beg;

	struct node* ab = new node;
	ab = beg;
	struct node* temp = new node;



	char a[999];
	memset(a, 0, 999);

	printf("\nВведите номер автобазы:");
	//while (!feof(stdin) && getchar() != '\n');
	fflush(stdin);

	gets_s(a);
	int m = atoi(a);
	if (ab->info.ABnomber != m)
		temp->info.ABnomber = atoi(a);
	if (temp->info.ABnomber == 0)
	{
		printf("\nНомер автобазы может быть только целочисленным и натуральным!\n");
		system("pause");
		CreateElement();
		/*int n = -842150451;
		node* delete_temp = beg;
		while (delete_temp != back)
		{
			if (delete_temp->info.ABnomber == n || delete_temp->info.CarCount == n)
			{
				DeleteElement(delete_temp);
				break;
			}
			delete_temp = delete_temp->next;
		}*/
		return;
	}

	printf("\nВведите имя директора:");


	gets_s(a);
	fflush(stdin);
	//cin.ignore(INT_MAX, '\n');
	//fgets(a, 15, stdin);
	//cout << strlen(a);
	if (strlen(a) < 1)
		strcpy_s(temp->info.Director, "vacancy");
	else
		strcpy_s(temp->info.Director, a);


	printf("\nТоплива потрачено:");
	gets_s(a);
	if (!atoi(a))
		NOwords(a);

	temp->info.FuelPOTRACHENO = atof(a);

	printf("\nВведите количетсво автомобилей:");
	gets_s(a);
	if (!atoi(a))
		NOwords(a);

	temp->info.CarCount = atoi(a);



	/*int n = -842150451;
	node* delete_temp = beg;
	while (delete_temp != back)
	{
		if (delete_temp->info.ABnomber == n || delete_temp->info.CarCount == n)
		{
			DeleteElement(delete_temp);
			break;
		}
		delete_temp = delete_temp->next;

	}*/
	back->next = temp;
	temp->prev = back;
	temp->next = NULL;
	back = temp;
	beg = back;
	NodesCount++;
	system("pause");
}

void TextSave()
{
	system("cls");
	if (beg == NULL || back == NULL)
	{
		printf("Нет данных, что вы собрались собственно экспортировать?");
		system("pause");
		return;
	}
	printf("Для экспорта данных в файл введите путь для экспорта:\n");
	cin >> ExportFileName;
	errno_t err;
	FILE* out;
	err = fopen_s(&out, ExportFileName, "w");
	if (err != 0)
	{
		printf("Не удалось открыть файл для экспорта или неверно задан путь!");
		system("pause");
		return;
	}
	else
	{
		struct node* temp = beg;
		int RecordCount = 0;
		fprintf_s(out, "%d\n", NodesCount);
		while (RecordCount < NodesCount)
		{
			fprintf_s(out, "%d\n.%s.\n%f\n%d\n", temp->info.ABnomber, temp->info.Director, temp->info.FuelPOTRACHENO, temp->info.CarCount);
			temp = temp->next;
			RecordCount++;
		}
	}
	fclose(out);
	system("pause");
	return;
}

void BinSave()
{
	system("cls");
	if (beg == NULL || back == NULL)
	{
		printf("Пустая таблица!\n");
		printf("Что именно вы пытаетесь сохранить?\n");
		system("pause");
		return;
	}
	errno_t err;
	FILE* out;
	err = fopen_s(&out, BinFileName, "wb");
	if (err != 0)
	{
		printf("Не удалось открыть файл для сохранения...\n\
				Сохранение не произойдёт!");
		return;
	}
	else
	{
		fwrite(&NodesCount, sizeof(NodesCount), 1, out);
		struct node* temp = beg;
		for (int i = 0; i < NodesCount; i++)
		{
			fwrite(temp, sizeof(struct node), 1, out);
			temp = temp->next;
		}
		fclose(out);
		printf("Файл успешно сохранён в папку с программой\n");
		system("pause");
	}
}


int ChoiseSaveMenu()
{
	int MenuItem = 1;
	PrintMenu(MenuItem, choiseOFSaveITEMS, MAXMENUCHOISEITEM);
	char c;
	while (c = _getch())
	{
		if (c == EscapeKeyCode) return 3;
		if (c == EnterKeyCode) return MenuItem;
		else if (c == DownKeyCode && MenuItem < MAXMENUCHOISEITEM - 1) MenuItem++;
		else if (c == UpKeyCode && MenuItem > 1) MenuItem--;
		PrintMenu(MenuItem, choiseOFSaveITEMS, MAXMENUCHOISEITEM);
	};
	return 0;
}

void SaveInterface()
{
	system("cls");

	while (1)
	{
		switch (ChoiseSaveMenu())
		{
		case 1: TextSave();
			break;
		case 2: BinSave();
			break;
		case 3:
			return;
		default:
			break;
		}
	}
	return;
}

void HolyPlus()
{
	char c;
	setColor(Black, White);
	system("cls");
	const char* HOSPADI[] = {
							"```````````````````````````11`````````````````````````` ",
							"````````````````````````11111111``````````````````````` ",
							"```````````````````````101100100``````````````````````` ",
							"`````````````111```1````10100101```11```011```````````` ",
							"`````0````111111`11011``1000000```1001``11`111```11```` ",
							"````111``111``01`10000```01``11```0000``11`111```11```` ",
							"```11111`111```1110001101``````1111000111``111``111`1`` ",
							"``111111````````11111100```````101111101```````111110`` ",
							"```111111````````11111111`````11111110111`````1111111`` ",
							"```11111111``11`1111011100```1011100110111```011111111` ",
							"``11111110111````````111110``01110`````````11011111111` ",
							"```111111111111```````11110111110````````101111011111`` ",
							"11111111111111111`````11111001110``````1110111111111111 ",
							"``1111111111111111````111110011111````011110111011111`` ",
							"`11111111111111111`1`0000000000000011`1111101111111111` ",
							"```1111111111111````1000000000000001```11110111111111`` ",
							"``1111111111111111111000111100`11001111111011111111111` ",
							"```111111111111111110000000`01`0000111111111111111111`` ",
							"``000011111111111111101000111``0000111111111111111111`` ",
							"```1011111111111111110`1``101``11101111111111111111```` ",
							"`````1`11111111111``1010`11000100001``11111111111`````` ",
							"`````11``11111111```1000000000000001``111111111```````` ",
							"``````11```````````11000000000000001``````````````````` ",
							"```````1`````````1111000000000000001001```````````````` ",
							"````````1```````111111111110011101101111````11````````` ",
							"````````11``````111111``11111101``111101``110011``````` ",
							"`````````01````1111111```111101```111111`10110001`````` ",
							"``````````011```110001`110010111``00001```01`100``````` ",
							"`````````100011111`````1111111111```11110000001```````` ",
							"``````````1101````1``11111111111111`11````````````````` ",
							"````````````11````111111111111`1111111````````````````` ",
							"`````````````1```11111101`1111`10111111```````````````` ",
							"````````````````````1110110110110111`1````````````````` ",
							"``````````````````````````111`````````````````````````` " };
	for (int i = 0; i < 34; i++)
		printf("%s\n", HOSPADI[i]);
	PlaySound(TEXT("D:\\RIAnthem.wav"), NULL, SND_FILENAME | SND_ASYNC);
	c = getchar();
}

void SovyetFlag()
{
	char c;
	setColor(Yellow, Red);
	system("cls");
	const char* HOSPADI[] = { "000000000000000000000__00000000000000000000000000000000000",
								"0000000000000000000000___000000000000000000000000000000000",
								"0000000000000000000000000_____0000000000000000000000000000",
								"00000000000000000000000000000_____000000000000000000000000",
								"00000000000000000000000000000000_____000000000000000000000",
								"0000000000000000000000000000000000_____0000000000000000000",
								"00000000000000__________000000000000_____00000000000000000",
								"000000000000___________000000000000000_____000000000000000",
								"0000000000___________0000000000000000000____00000000000000",
								"000000000__________0000000000000000000000_____000000000000",
								"0000000___________000000000000000000000000_____00000000000",
								"00000___________00__00000000000000000000000_____0000000000",
								"0000___________0_____00000000000000000000000_____000000000",
								"000000_______0000______0000000000000000000000____000000000",
								"0000000____00000000______00000000000000000000_____00000000",
								"000000000000000000000______000000000000000000_____00000000",
								"0000000000000000000000______00000000000000000_____00000000",
								"000000000000000000000000______000000000000000_____00000000",
								"00000000000000000000000000______0000000000000_____00000000",
								"000000000000000000000000000_______00000000000_____00000000",
								"00000000000000000000000000000______000000000_____000000000",
								"0000000000000000000000000000000______000000______000000000",
								"000000000000000000__0000000000000______000______0000000000",
								"0000000000000000______000000000000_____________00000000000",
								"000000000000000_________000000000000__00______000000000000",
								"0000000000000_____0_________00000000________00000000000000",
								"00000000000____000000_____________________00_0000000000000",
								"00000000______0000000000________________0_______0000000000",
								"0000000______00000000000000000____00000000______0000000000",
								"0000000_____00000000000000000000000000000000___00000000000" };
	for (int i = 0; i < 30; i++)
		printf("%s\n", HOSPADI[i]);
	PlaySound(TEXT("D:\\SovietAnthem.wav"), NULL, SND_FILENAME | SND_ASYNC);
	c = getchar();
}
void SevSUCool()
{
	char c;
	setColor(White, Blue);
	system("cls");
	const char* HOSPADI[] = { "##########################",
								"########==========########",
								"#####================#####",
								"###====================###",
								"##=======#==============##",
								"#==========####==========#",
								"#===========#######======#",
								"#============######+=====#",
								"#============####========#",
								"#===========#============#",
								"##======================##",
								"####==================####",
								"######==============######",
								"##########################" };
	for (int i = 0; i < 14; i++)
		printf("%s\n", HOSPADI[i]);
	PlaySound(TEXT("D:\\SevSU.wav"), NULL, SND_FILENAME | SND_ASYNC);
	c = getchar();
}

//Вариант 3
//Даны сведения о расходовании на автобазах города ГЕРОЯ Севастополя топлива по следующему макету: номер автобазы, Ф.И.О. директора (15 символов), 
//израсходовано топлива (в условных единицах), количество автомашин на базе. Подсчитать средний расход топлива на одну машину по каждой базе и в целом по городу.