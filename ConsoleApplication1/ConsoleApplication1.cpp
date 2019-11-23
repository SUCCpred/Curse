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

using namespace std;

/************************** Макросы *******************************************/

#define MAXMENUITEM 11 //Максимальное количество элементов
#define EnterKeyCode 13 //Код клавиши ввода (по умолчанию 13 - Enter)
#define EscapeKeyCode 27 //Код клавиши отмены (по умолчанию 27 - Escape)
#define UpKeyCode 72 //Код клавиши перехода наверх (по умолчанию 72 - стрелка вверх)
#define DownKeyCode 80 //Код клавиши перехода вниз (по умолчанию 80 - стрелка вниз)
#define DirectorNameSize 15 //На сколько большое может быть имя у директора




/*********************** Глобальные переменные ****************************/

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
struct node* beg, * back; //Указатели на начало и конец списка
int NodesCount = 0; //Количество нод в списке
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
const char* items[] = {
	"$$$$$$$$$$$  СВЕЕДЕНИЯ О РАСХОДЕ ТОПЛИВА  $$$$$$$$$$$$",
	"             Начальное создание таблицы               ",//1
	"                  Просмотр таблицы                    ",//2
	"          Добавление новой записи в таблицу           ",//3
	"                  Удаление записи                     ",//4
	"            Корректировка записи в таблице            ",//5
	"                Сотрировка таблицы                    ",//6
	"              Поиск записи в таблице                  ",//7
	"        Экспорт данных в текстовый файл               ",//Приказ
	"  Обработка таблицы и просмотр результатов обработки  ",//9
	"                       Выход                          " //10
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
void PrintMenu(int item); //Отрисовка пунктов меню

void FirstEl(); //Импорт таблицы из текстового файла
void ExportTable(); //Экспорт таблицы в текстовый файл

void sEl(node* beg); //Поиск записи по ключу
void SortList(); //Сортировка

void Prosmotr(node* beg); //Печать таблицы на экран
void Delete(); //Удаление таблицы (освобождение памяти)
void DeleteElement(struct node* _node);
void DeleteInterface();

void CorrectElement(struct node* temp);
void CorrElInterface();
void CreateElement();//добавление элемента

void setColor(ConsoleColor text, ConsoleColor background) 
{
	SetConsoleTextAttribute(hConsole, (background << 4) | text);
}

void setPos(COORD& c) 
{
	SetConsoleCursorPosition(hConsole, c);
}

int main() {
	setlocale(LC_ALL, "Russian");
	setColor(Yellow, Red);
	while (1)
	{
		switch (Menu())
		{
		case 1: FirstEl();
			break;
		case 2: Prosmotr(beg);
			break;
		case 3: CreateElement();
			break;
		case 4: DeleteInterface();
			break;
		case 5: CorrElInterface();
			break;
		case 6: SortList();
			break;
		case 7: sEl(beg);
			break;
		case 8: ExportTable();
			break;
		case 9:
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
	DeleteElement(temp);
	cout << "Автобаза удалена из базы";
	cin.get();
	return;
}



void DrawAffirmationItems(int Item)
{
	if (Item == 1)
	{
		printf("Нет");
		printf("Да\n");
	}
	else if (Item == 2)
	{
		printf("Нет\n");
		printf("Да");
	}
}

void CorrectElement(struct node* temp)
{
	system("cls");
	printf("Редактирование элемента:\n");

	struct AutoBase* temp1 = new AutoBase;

	printf("\nИзменить номер автобазы %d на: ", temp->info.ABnomber);
	char a[999];
	memset(a, 0, 999);
	while (getchar() != '\n' && !feof(stdin));
	gets_s(a);
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

/*void Average1(node* beg) {
	float a;
	while (beg->next != 0) {
		a = (beg->FuelPOTRACHENO) / (beg->CarCount);
		cout << "Средний расход топлива на машину по базе " << beg->ABnomber << a;
	}
}*/

void Print(const node& t) 
{
	printf_s("Номер автобазы: %d\n", t.info.ABnomber);
	cout << "Имя директора: " <<  t.info.Director << endl;
	printf_s("Израсходовано топлива: %f\n", t.info.FuelPOTRACHENO);
	printf_s("Количество автомобилей : %d\n", t.info.CarCount);
}

void sEl(node* beg) 
{
	int n;
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
}

void PrintMenu(int item)
{
	system("cls");
	int i = 1;
	cout << items[0] << endl;
	for (; i < item; i++)
		cout << "  " << items[i] << endl;
	cout << "-$>" << items[i++] << "<$-\n";
	for (; i < MAXMENUITEM; i++)
		cout << "  " << items[i] << endl;
};

int Menu() {
	int MenuItem = 1;
	PrintMenu(MenuItem);
	char c;
	while (c = _getch()) 
	{
		if (c == EnterKeyCode) return MenuItem;
		else if (c == DownKeyCode && MenuItem < MAXMENUITEM-1) MenuItem++;
		else if (c == UpKeyCode && MenuItem > 1) MenuItem--;
		PrintMenu(MenuItem);
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
		//else if (c < 0) continue;
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

void FirstEl() {


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
			printf("Таблица удалена!");
			system("pause");
			break;
		}
	}

	system("cls");

	printf("Введите путь к импортируемому файлу с таблицей:\n");
	cin >> ImportFileName;

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

	beg = (struct node*)malloc(sizeof(struct node));
	beg->prev = NULL;
	//beg->info = *(struct AutoBase*)malloc(sizeof(struct AutoBase));

	fscanf_s(in, "%d", &beg->info.ABnomber);
	GetStructLine(in, beg->info.Director, DirectorNameSize);
	fscanf_s(in, "%f", &beg->info.FuelPOTRACHENO);
	fscanf_s(in, "%d", &beg->info.CarCount);

	struct node* temp_node = beg;

	for (; RecordCount < NodesCount; RecordCount++)
	{
		temp_node->next = (struct node*)malloc(sizeof(struct node));
		//temp_node->next->info = *(struct AutoBase*)malloc(sizeof(struct AutoBase));

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

	system("pause");
}

//----------------------------просмотр очереди---------------------------

void Prosmotr(node* beg) {
	if (!beg) { system("cls");  cout << "Данные отсутствуют..." << endl; system("pause");  return; }
	node* temp = beg; //указатель temp устанавливаем в начало
	system("cls");
	cout << "==============================" << endl;
	while (temp) 
	{
				Print(*temp); //печатаем значения элемента по указателю
				cout << "==============================" << endl;
				system("pause");
				temp = temp->next;
			
	}
	return;
}

void SortList()
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
}


void CreateElement()
{
	system("cls");
	printf("Добавление нового элемента:\n");

	struct node* temp = (struct node*)malloc(sizeof(struct node));
	//temp->info = *(struct AutoBase*)malloc(sizeof(struct AutoBase));

	back->next = temp;
	temp->prev = back;
	temp->next = NULL;
	back = temp;

	printf("\nВведите номер автобазы:");
	char a[999];
	memset(a, 0, 999);
	while (getchar() != '\n' && !feof(stdin));
	gets_s(a);
	temp->info.ABnomber = atoi(a);

	printf("\nВведите имя директора:");
	gets_s(a);
	strcpy_s(temp->info.Director, a);

	printf("\nТоплива потрачено:");
	gets_s(a);
	temp->info.FuelPOTRACHENO = atof(a);

	printf("\nВведите количетсво автомобилей:");
	gets_s(a);
	temp->info.CarCount = atoi(a);

	NodesCount++;
	system("pause");
}

void ExportTable()
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

//Вариант 3
//Даны сведения о расходовании на автобазах города топлива по следующему макету: номер автобазы, Ф.И.О. директора (15 символов), 
//израсходовано топлива (в условных единицах), количество автомашин на базе. Подсчитать средний расход топлива на одну машину по каждой базе и в целом по городу.
//niniram