#include <iostream>
#include <vector>
#include <string>
#include <limits> // для очистки буферов
#include <ctime>
#include <random>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <thread> // Для засыпания
#include <chrono> // Для указания времени
#include <sstream>

using namespace std;

struct person
{
    int health;
    int damage;
    int cost;
    int priority;
    int gold;
    int sila;
    int number;
};

void random_variable(int &randomNumb, int min, int max)
{

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    randomNumb = dis(gen);
}

void hideInterface()
{
#ifdef _WIN32
    system("cls"); // Для Windows
#else
    system("clear"); // Для macOS и Linux
#endif
}

void wait(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void showInterface(int gold, const person &knight, const person &archer, const person &paladin, const person &miner, int sila, int gold_per_day, int day)
{

    cout << "\n==================| BLACK ISLANDS |================" << endl;
    cout << "1 - Нанять юнита | 2 - Продать юнита | 3 - Напасть" << endl;
    cout << "4 - Пропустить   | 5 - Ход боя       | 0 - Выход" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Юниты: " << knight.number << " рыц. | " << archer.number << " лук. | "
         << paladin.number << " пал. | " << miner.number << " шахт." << endl;
    cout << gold << " золота | " << gold_per_day << " з/день | "
         << sila << " сила | " << day << " день" << endl;
    cout << "Ваш ход: ";
}

void menu(int &choise)

{
    while (true)
    {
        if (cin >> choise)
        {
            // Проверка: если ввели число, выходим из цикла защиты
            break;
        }
        else
        {
            // Если ввели букву:
            cout << "Ошибка! Введите цифру: ";
            cin.clear();             // Сбрасываем флаг ошибки
            cin.ignore(10000, '\n'); // Очищаем буфер от мусора
            wait(1500);
        }
    }
}

void enemy_units_occupancy(int &randomNum, int min, int max, int map_size, int enemyUnits[][3])
{
    for (int i = 0; i < map_size; i++)
    {
        for (int y = 0; y < 3; y++)
        {
            random_variable(randomNum, 0, 4);
            // Используем имя units вместо имени функции
            enemyUnits[i][y] = randomNum;
        }
    }
}

void enemySila(int map_size, int enemyUnits[][3], int enemySilaARR[][1], person &enemy_knight, person &enemy_archer, person &enemy_paladin)
{

    for (int i = 0; i < map_size; i++)
    {
        int temp = 0;
        for (int y = 0; y < 3; y++)
        {
            switch (y)
            {
            case 0:
                temp += enemyUnits[i][y] * enemy_archer.sila;
                break;
            case 1:
                temp += enemyUnits[i][y] * enemy_knight.sila;
                break;
            case 2:
                temp += enemyUnits[i][y] * enemy_paladin.sila;
                break;

            default:
                break;
            }
        }
        enemySilaARR[i][0] = temp;
    }
}

void print_mapa(int map_size, const person &knight, const person &archer, const person &paladin, int enemySilaARR[][1])
{
    using namespace std;
    int side = static_cast<int>(sqrt(map_size));
    int centerIndex = (side / 2) * side + (side / 2);
    int p_sila = (knight.sila * knight.number) + (archer.sila * archer.number) + (paladin.sila * paladin.number);

    cout << "\n"
         << string(side * 6, '=') << " КАРТА ОСТРОВОВ " << string(side * 6, '=') << endl;

    for (int y = 0; y < side; ++y)
    {
        string l1 = "", l2 = "", l3 = "", l4 = "";

        for (int x = 0; x < side; ++x)
        {
            int idx = y * side + x;
            bool is_home = (idx == centerIndex);
            int s = is_home ? p_sila : enemySilaARR[idx][0];

            // Подготовка строк (всегда 2 символа)
            string s_idx = (idx < 10) ? "0" + to_string(idx) : to_string(idx);
            string s_val = (s <= 0) ? "XX" : (s > 99 ? "++" : (s < 10 ? "0" + to_string(s) : to_string(s)));

            // ЯЧЕЙКА (Строго 10 символов в ширину)
            if (is_home)
            {
                l1 += "┌────────┐";
                l2 += "│ [ВАШ]  │";
                l3 += "│ SL:" + s_val + "  │";
            }
            else if (s <= 0)
            {
                l1 += "..........";
                l2 += "..[ ОК ]..";
                l3 += "..........";
            }
            else
            {
                l1 += "┌────────┐";
                l2 += "│ ID:" + s_idx + "  │";
                l3 += "│ SL:" + s_val + "  │";
            }

            // ГОРИЗОНТАЛЬНЫЙ МОСТ (3 символа)
            if (x < side - 1)
            {
                l2 += "---";
                l1 += "   ";
                l3 += "   ";
            }

            // ВЕРТИКАЛЬНЫЙ МОСТ (ровно под центром ячейки)
            if (y < side - 1)
            {
                l4 += "    |     ";
                if (x < side - 1)
                    l4 += "   "; // отступ под горизонтальным мостом
            }
        }
        cout << l1 << endl
             << l2 << endl
             << l3 << endl;
        if (y < side - 1)
            cout << l4 << endl;
    }
    cout << string(side * 13 - 3, '=') << endl;
}

void add_units(int &gold, person &knight, person &archer, person &paladin, person &miner, int &sila, int &gold_per_day, int day)
{
    int choise = -1;
    while (true) // Используем true, так как выход через return 0
    {
        hideInterface(); // Очищаем экран только один раз в начале цикла

        cout << "\n============================================| BLACK ISLANDS |========================================" << endl;
        cout << "1 - Нанять рыцаря (цена 40)      | 2 - Нанять лучника (цена 50)       | 3 - Нанять паладина (цена 60)" << endl;
        cout << "4 - Нанять шахтера (цена 60)     | 5 - Показать всех воинов   | 0 - Выход" << endl;
        cout << "-----------------------------------" << endl;
        cout << "Юниты: " << knight.number << " рыц. | " << archer.number << " лук. | "
             << paladin.number << " пал. | " << miner.number << " шахт." << endl;
        cout << gold << " золота | " << gold_per_day << " з/день | "
             << sila << " сила | " << day << " день" << endl;
        cout << "Ваш ход: ";

        menu(choise); // Ввод числа

        if (choise == 0)
            return; // Мгновенный выход без лишних проверок

        switch (choise)
        {
        case 1:
            if (gold >= 40)
            {
                knight.number++;
                sila += knight.sila;
                gold -= knight.cost;
            }
            else
            {
                cout << "! Недостаточно золота !" << endl;
                wait(1500); // Программа замрет на 1.5 секунды
            }
            break;
        case 2:
            if (gold >= 50)
            {
                archer.number++;
                sila += archer.sila;
                gold -= archer.cost;
            }
            else
            {
                cout << "! Недостаточно золота ! " << endl;
                wait(1500);
            }
            break;
        case 3:
            if (gold >= 60)
            {
                paladin.number++;
                sila += paladin.sila;
                gold -= paladin.cost;
            }
            else
            {
                cout << "! Недостаточно золота ! " << endl;
                wait(1500);
            }
            break;
        case 4:
            if (gold >= 60)
            {
                miner.number++;
                gold_per_day = miner.number * miner.gold;
                gold -= miner.cost;
            }
            else
            {
                cout << "! Недостаточно золота ! " << endl;
                wait(1500);
            }
            break;
        case 5:
            hideInterface(); // Очистим экран перед списком
            cout << "=== ВАШИ ВОЙСКА ===" << endl;

            // Рыцари
            if (knight.number > 0)
            {
                for (int i = 0; i < knight.number; i++)
                {
                    cout << "Рыцарь " << i + 1 << ": " << knight.health << " HP | "
                         << knight.damage << " ATK | Сила: " << knight.sila << endl;
                }
            }
            else
            {
                cout << "   Нету Рыцарей" << endl;
            }
            cout << "----------------------------------------" << endl;
            // Лучники
            if (archer.number > 0)
            {
                for (int i = 0; i < archer.number; i++)
                {
                    cout << "Лучник " << i + 1 << ": " << archer.health << " HP | "
                         << archer.damage << " ATK | Сила: " << archer.sila << endl;
                }
            }
            else
            {
                cout << "Нету Лучников" << endl;
            }
            cout << "----------------------------------------" << endl;
            // Паладины
            if (paladin.number > 0)
            {

                for (int i = 0; i < paladin.number; i++)
                {
                    cout << "Паладин " << i + 1 << ": " << paladin.health << " HP | "
                         << paladin.damage << " ATK | Сила: " << paladin.sila << endl;
                }
            }
            else
            {
                cout << "Нету Паладинов" << endl;
            }

            cout << "========================================" << endl;
            cout << "Возврат в меню через 6 секунды..." << endl;
            wait(6000); // Игрок успеет посмотреть список
            break;
        case 0:
            return;

        default:
            cout << "Ошибка! Введите корректную опцию!" << endl;
            wait(1500);
            break;
        }
    }
}

void remove_units(int &gold, person &knight, person &archer, person &paladin, person &miner, int &sila, int &gold_per_day, int day)
{
    int choise = -1;
    while (true)
    {
        hideInterface(); // Очищаем экран только один раз в начале цикла

        cout << "\n============================================| BLACK ISLANDS |========================================" << endl;
        cout << "1 - Продать рыцаря (цена 40)      | 2 - Продать лучника (цена 50)       | 3 - Продать паладина (цена 60)" << endl;
        cout << "4 - Продать шахтера (цена 60)     | 5 - Показать всех воинов   | 0 - Выход" << endl;
        cout << "-----------------------------------" << endl;
        cout << "Юниты: " << knight.number << " рыц. | " << archer.number << " лук. | "
             << paladin.number << " пал. | " << miner.number << " шахт." << endl;
        cout << gold << " золота | " << gold_per_day << " з/день | "
             << sila << " сила | " << day << " день" << endl;
        cout << "Ваш ход: ";
        menu(choise);

        if (choise == 0)
            return; // Мгновенный выход без лишних проверок

        switch (choise)
        {
        case 1:
            if (knight.number > 0)
            {
                knight.number--;
                sila -= knight.sila;
                gold += knight.cost;
                cout << "Рыцарь продан за 40 золота" << endl;
            }
            else
            {
                cout << "! Нету Рыцарей !" << endl;
            }
            wait(1500);
            break;
        case 2:
            if (archer.number > 0)
            {
                archer.number--;
                sila -= archer.sila;
                gold += archer.cost;
                cout << "Лучник продан за 50 золота" << endl;
                wait(1500);
            }
            else
            {
                cout << "! Нету Лучников ! " << endl;
                wait(1500);
            }
            break;
        case 3:
            if (paladin.number > 0)
            {
                paladin.number--;
                sila -= paladin.sila;
                gold += paladin.cost;

                cout << "Паладин продан за 60 золота" << endl;
                wait(1500);
            }
            else
            {
                cout << "! Нету Паладинов ! " << endl;
                wait(1500);
            }
            break;
        case 4:
            if (miner.number > 0)
            {
                miner.number--;
                gold += miner.cost;
                gold_per_day = miner.number * miner.gold;
                cout << "Шахтер продан за 60 золота" << endl;
                wait(1500);
            }
            else
            {
                cout << "! Нету Шахтеров ! " << endl;
                wait(1500);
            }
            break;
        case 5:
            hideInterface(); // Очистим экран перед списком
            cout << "=== ВАШИ ВОЙСКА ===" << endl;

            // Рыцари
            for (int i = 0; i < knight.number; i++)
            {
                cout << "Рыцарь " << i + 1 << ": " << knight.health << " HP | "
                     << knight.damage << " ATK | Сила: " << knight.sila << endl;
            }
            cout << "----------------------------------------" << endl;

            // Лучники
            for (int i = 0; i < archer.number; i++)
            {
                cout << "Лучник " << i + 1 << ": " << archer.health << " HP | "
                     << archer.damage << " ATK | Сила: " << archer.sila << endl;
            }
            cout << "----------------------------------------" << endl;

            // Паладины
            for (int i = 0; i < paladin.number; i++)
            {
                cout << "Паладин " << i + 1 << ": " << paladin.health << " HP | "
                     << paladin.damage << " ATK | Сила: " << paladin.sila << endl;
            }

            cout << "========================================" << endl;
            cout << "Возврат в меню через 6 секунды..." << endl;
            wait(6000); // Игрок успеет посмотреть список
            break;
        case 0:
            return;
            break;
        default:
            cout << "Ошибка! Введите корректную опцию!" << endl;
            wait(1500);
            break;
        }
    }
}

void attack(int &gold, person &knight, person &archer, person &paladin, int &sila, int map_size,
            int enemySilaARR[][1], int enemyUnits[49][3], person &e_knight, person &e_archer, person &e_paladin)
{
    int target;
    int side = static_cast<int>(sqrt(map_size));
    int centerIndex = (side / 2) * side + (side / 2);

    cout << "\n=== ВЫБОР ОСТРОВА ДЛЯ АТАКИ ===" << endl;
    cout << "Введите номер (0 - " << map_size - 1 << "): ";

    if (!(cin >> target))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка: Введите число!" << endl;
        wait(1500);
        return;
    }

    if (target < 0 || target >= map_size || target == centerIndex || enemySilaARR[target][0] <= 0)
    {
        cout << "Ошибка: Неверный выбор или остров уже ваш!" << endl;
        wait(1500);
        return;
    }

    // Копии вражеских юнитов на выбранном острове
    int e_arch_num = enemyUnits[target][0];
    int e_kni_num = enemyUnits[target][1];
    int e_pal_num = enemyUnits[target][2];

    bool in_battle = true;
    while (in_battle)
    {
        hideInterface();
        cout << "======= БИТВА ЗА ОСТРОВ #" << target << " =======" << endl;
        cout << "ВАШИ СИЛЫ: " << knight.number << " рыц. | " << archer.number << " лук. | " << paladin.number << " пал." << endl;
        cout << "ВРАГ: " << e_kni_num << " рыц. | " << e_arch_num << " лук. | " << e_pal_num << " пал." << endl;
        cout << "------------------------------------------" << endl;
        cout << "1 - Начать раунд | 0 - Отступить (-20 золота)" << endl;

        int b_choice;
        menu(b_choice);

        if (b_choice == 1)
        {
            // Фиксируем количество войск ДО начала размена уроном, чтобы бой был одновременным
            int start_k = knight.number;
            int start_a = archer.number;
            int start_p = paladin.number;
            int start_ek = e_kni_num;
            int start_ea = e_arch_num;
            int start_ep = e_pal_num;

            // 1. ФАЗА ПАЛАДИНОВ (Приоритет 1) - Бьют рыцарей
            if (start_p > 0)
                e_kni_num -= ceil((double)(start_p * paladin.damage) / e_knight.health);
            if (start_ep > 0)
                knight.number -= ceil((double)(start_ep * e_paladin.damage) / knight.health);

            // 2. ФАЗА РЫЦАРЕЙ (Приоритет 2) - Бьют лучников
            if (start_k > 0)
                e_arch_num -= ceil((double)(start_k * knight.damage) / e_archer.health);
            if (start_ek > 0)
                archer.number -= ceil((double)(start_ek * e_knight.damage) / archer.health);

            // 3. ФАЗА ЛУЧНИКОВ (Приоритет 3) - Бьют паладинов
            if (start_a > 0)
                e_pal_num -= ceil((double)(start_a * archer.damage) / e_paladin.health);
            if (start_ea > 0)
                paladin.number -= ceil((double)(start_ea * e_archer.damage) / paladin.health);

            // Обнуляем отрицательные значения
            knight.number = max(0, knight.number);
            archer.number = max(0, archer.number);
            paladin.number = max(0, paladin.number);
            e_kni_num = max(0, e_kni_num);
            e_arch_num = max(0, e_arch_num);
            e_pal_num = max(0, e_pal_num);

            // Проверка итогов
            if (e_kni_num + e_arch_num + e_pal_num <= 0)
            {
                cout << "\nПОБЕДА! Остров очищен. Получено 200 золота!" << endl;
                enemySilaARR[target][0] = 0;
                // Обнуляем юнитов в массиве островов, чтобы они не воскресли
                enemyUnits[target][0] = 0;
                enemyUnits[target][1] = 0;
                enemyUnits[target][2] = 0;
                gold += 200;
                in_battle = false;
                wait(2500);
            }
            else if (knight.number + archer.number + paladin.number <= 0)
            {
                cout << "\nВАША АРМИЯ ПОГИБЛА..." << endl;
                in_battle = false;
                wait(2500);
            }
        }
        else if (b_choice == 0)
        {
            gold = max(0, gold - 20);
            cout << "Вы отступили. Потеряно 20 золота." << endl;
            in_battle = false;
            wait(1500);
        }
    }
    // Обновляем общую силу игрока
    sila = (knight.number * knight.sila) + (archer.number * archer.sila) + (paladin.number * paladin.sila);
}

bool checkWin(int map_size, int enemySilaARR[][1], int gold, int day, person knight, person archer, person paladin)
{
    int side = static_cast<int>(sqrt(map_size));
    int centerIndex = (side / 2) * side + (side / 2);

    // Проверяем, остались ли живые враги на островах
    for (int i = 0; i < map_size; i++)
    {
        // Пропускаем центральный остров игрока и проверяем силу врага
        if (i != centerIndex && enemySilaARR[i][0] > 0)
        {
            return false; // Если нашли хоть одного живого врага — игра продолжается
        }
    }

    // Если цикл завершился — все враги побеждены
    hideInterface();
    cout << "\n==========================================" << endl;
    cout << "  ПОЗДРАВЛЯЕМ! ВЫ ПОКОРИЛИ ЧЕРНЫЕ ОСТРОВА!  " << endl;
    cout << "==========================================" << endl;
    cout << "ВАША СТАТИСТИКА:" << endl;
    cout << "- Затрачено времени: " << day << " дней" << endl;
    cout << "- Золота в казне: " << gold << endl;
    // ТУТ ИСПОЛЬЗУЕМ ПОЛНЫЕ ИМЕНА: knight, archer, paladin
    cout << "- Финальная армия: " << knight.number << " рыц, " << archer.number << " лук, " << paladin.number << " пал." << endl;
    cout << "==========================================" << endl;
    cout << "Нажмите Enter, чтобы выйти..." << endl;

    cin.ignore(10000, '\n');
    cin.get();
    return true;
}

//=======================================================================================
//=======================================================================================
//=======================================================================================
int main()
{
    int sila, choise, gold, gold_per_day, day, number, map_size, current_map_size, randomNum;
    int min = 0, max = 4;

    string map;

    int enemyUnits[49][3]; // 0 - archer, 1 - knight, 2 - paladin
    int enemySilaARR[49][1];

    person knight = {25, 6, 40, 2, 0, 3, 0};
    person archer = {15, 8, 50, 3, 0, 2, 0};
    person paladin = {40, 5, 60, 1, 0, 5, 0};
    person miner = {5, 0, 60, 4, 15, 0, 4};

    person enemy_knight = {25, 6, 0, 2, 0, 3, 0};
    person enemy_archer = {15, 8, 0, 3, 0, 2, 0};
    person enemy_paladin = {40, 5, 0, 1, 0, 5, 0};

    sila = 0;
    choise = -1;
    day = 0;
    gold = 40;
    gold_per_day = 0; // ОБЯЗАТЕЛЬНО: обнуляем доход в начале игры

    hideInterface();
    bool running = true;

    while (running)
    {

        cout << "" << endl;
        cout << "=====================| BLACK ISLANDS |=========================" << endl;

        cout << "Суть игры -- захватить все острова. Вы появляетесь на центральном острове и вокруг вас вражеские острова где написан их айди (для атаки) и сила врага на острове. Игра происходит по дням и в один день можно как купить юнитов, так и продать их либо же пропустить день для получения золота которое добыли шахтеры. Так же хочу сказать про возможность атаки, вы можете напасть на любой не занятый вами остров в любой момент в течении текущего дня , но при этом выйти из боя и закупить воинов вы не можете, однако есть возможность сбежать заплатив небольшой откуп. А сейчас выберете размер карты и удачной игры)" << endl;
        cout << "Выберите размер карты: 1- 3х3 | 2- 5х5 | 3- 7х7 | 0 - выход" << endl;
        menu(choise);

        switch (choise)
        {
        case 1:
            map_size = current_map_size = 9;
            running = false;
            break;
        case 2:
            map_size = current_map_size = 25;
            running = false;
            break;
        case 3:
            map_size = current_map_size = 49;
            running = false;
            break;
        case 0:
            return 0;
            break;
        default:
            cout << "Ошибка! Введите корректную опцию!" << endl;
            cout << "" << endl;
            wait(1500);

            break;
        }
    }

    map_size = current_map_size;
    enemy_units_occupancy(randomNum, 0, 4, map_size, enemyUnits);
    enemySila(map_size, enemyUnits, enemySilaARR, enemy_knight, enemy_archer, enemy_paladin);
    while (choise != 0)
    {

        gold_per_day = miner.number * miner.gold;

        hideInterface();
        print_mapa(map_size, knight, archer, paladin, enemySilaARR);
        showInterface(gold, knight, archer, paladin, miner, sila, gold_per_day, day);
        if (checkWin(map_size, enemySilaARR, gold, day, knight, archer, paladin))
        {
            break; // Выход из игрового цикла при победе
        }
        menu(choise);

        switch (choise)
        {
        case 1:
            add_units(gold, knight, archer, paladin, miner, sila, gold_per_day, day);
            break;
        case 2:
            remove_units(gold, knight, archer, paladin, miner, sila, gold_per_day, day);
            break;
        case 3:
            attack(gold, knight, archer, paladin, sila, map_size, enemySilaARR, enemyUnits, enemy_knight, enemy_archer, enemy_paladin);
            break;

        case 4:
            gold += gold_per_day;
            day++;
            cout << "" << endl;
            cout << "Вы пропустили целый день..." << endl;
            cout << "" << endl;
            wait(2000);
            break;
        case 5:
            /* code */
            break;
        case 0:
            cout << "=====================| СПАСИБО ЗА ИГРУ |=========================" << endl;
            return 0;
            break;

        default:
            cout << "Ошибка! Введите корректную опцию!" << endl;
            cout << "" << endl;
            wait(1500);
            break;
        }
    }
}