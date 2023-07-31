#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

const int BABY_PRICE = 0,
ADULT_DAY_PRICE = 56000, ADULT_NIGHT_PRICE = 46000,
TEEN_DAY_PRICE = 47000, TEEN_NIGHT_PRICE = 40000,
CHILD_DAY_PRICE = 44000, CHILD_NIGHT_PRICE = 37000,
OLD_DAY_PRICE = 44000, OLD_NIGHT_PRICE = 37000;

const long long int FULL_DIGIT = 10000000000000,
FULL_DIGIT_MIN = 10000000000,
SEVEN_DIGIT = 1000000;

const int TWO_DIGIT = 100, ONE_DIGIT = 10,
OLD_GENERATION = 1900, NEW_GENERATION = 2000,
MALE_OLD = 1, FEMALE_OLD = 2, MALE_NEW = 3, FEMALE_NEW = 4,
BEFORE_BIRTH = 2, AFTER_BIRTH = 1;

const int MIN_BABY = 1, MIN_CHILD = 3, MIN_TEEN = 13, MIN_ADULT = 19,
MAX_CHILD = 12, MAX_TEEN = 18, MAX_ADULT = 64;

const int BABY = 1, CHILD = 2, TEEN = 3, ADULT = 4, OLD = 5;

const float DISABLE_DISCOUNT_RATE = 0.6, MERIT_DISCOUNT_RATE = 0.5,
MULTICHILD_DISCOUNT_RATE = 0.8,
PREGNANT_DISCOUNT_RATE = 0.85;

const int MAX_COUNT = 10, MIN_COUNT = 1;

void errorMessagePrint();

int inputTicketSelect();
long long int inputCustomerIDNumber();
int inputOrderCount();
int inputDiscountSelect();
void inputData(int* ticketSelect, long long int* customerIDNumber,
    int* orderCount, int* discountSelect);

int calcAge(long long int customerIDNumber);
int calcAgeGroup(int age);
int calcPriceProcess(int age, int ticketSelect);
int calcDiscount(int calcPrice, int discountSelect);
int calcPriceResult(int calcPrice, int orderCount);
int processIntergration(long long int customerIDNumber, int ticketSelect,
    int discountSelect, int orderCount,
    int priceResult, int* age);
void saveOrderList(int ticketSelect, int age, int orderCount,
    int priceResult, int discountSelect,
    int* position, int(*orderList)[5]);

void pricePrint(int priceResult);
void orderPrint(int totalPrice, int* position, int(*orderList)[5]);
void orderFilePrint(int totalPrice, int* position, int(*orderList)[5], int* filePosition);

int orderContinue();

/************************************************************
   ����
*************************************************************/
int main()
{
    int totalPrice = 0;
    int position = 0;
    int filePosition = 0;
    int reportPosition = 0;
    int orderList[100][5] = { 0 };
    int isExit = 0;
    do
    {
        while (true)
        {
            long long int customerIDNumber = 0;
            int ticketSelect = 0, orderCount = 0, discountSelect = 0;
            int priceResult = 0, age = 0;
            int continueSelect = 0;

            inputData(&ticketSelect, &customerIDNumber,
                &orderCount, &discountSelect);

            priceResult = processIntergration(customerIDNumber,
                ticketSelect, discountSelect, orderCount,
                priceResult, &age);

            saveOrderList(ticketSelect, age, orderCount,
                priceResult, discountSelect,
                &position, orderList);

            totalPrice += priceResult;

            pricePrint(priceResult);

            continueSelect = orderContinue();

            if (continueSelect == 2)
            {
                break;
            }
        }

        orderPrint(totalPrice, &position, orderList);
        orderFilePrint(totalPrice, &position, orderList, &filePosition);

        cout << "��� ����(1: ���ο� �ֹ�, 2: ���α׷� ����) : ";
        cin >> isExit;
        position = 0;
        totalPrice = 0;
    } while (isExit == 1);
    return 0;
}

/************************************************************
   �Էº�
*************************************************************/
int inputTicketSelect()
{
    int ticketSelect = 0;

    cout << "������ �����ϼ���." << endl;
    cout << "1. �ְ���" << endl;
    cout << "2. �߰���" << endl;
    while (true)
    {
        cin >> ticketSelect;
        if (ticketSelect == 1 || ticketSelect == 2)
        {
            break;
        }
        else
        {
            errorMessagePrint();
        }
        cin.ignore();
    }
    return ticketSelect;
}

long long int inputCustomerIDNumber()
{
    long long int customerIDNumber = 0;

    cout << "�ֹι�ȣ�� �Է��ϼ���" << endl;
    while (true)
    {
        cin >> customerIDNumber;
        if ((customerIDNumber >= FULL_DIGIT_MIN) &&
            (customerIDNumber < FULL_DIGIT))
        {
            break;
        }
        else
        {
            errorMessagePrint();
        }
        cin.ignore();
    }
    return customerIDNumber;
}

int inputOrderCount()
{
    int orderCount = 0;

    cout << "��� �ֹ��Ͻðڽ��ϱ�? (�ִ� 10��)" << endl;
    while (true)
    {
        cin >> orderCount;
        if ((orderCount <= MAX_COUNT) && (orderCount >= MIN_COUNT))
        {
            break;
        }
        else
        {
            errorMessagePrint();
        }
        cin.ignore();
    }
    return orderCount;
}

int inputDiscountSelect()
{
    int discountSelect = 0;

    cout << "�������� �����ϼ���." << endl;
    cout << "1. ���� (���� ���� �ڵ�ó��)" << endl;
    cout << "2. �����" << endl;
    cout << "3. ����������" << endl;
    cout << "4. ���ڳ�" << endl;
    cout << "5. �ӻ��" << endl;

    while (true)
    {
        cin >> discountSelect;
        if (discountSelect >= 1 &&
            discountSelect <= 5)
        {
            break;
        }
        else
        {
            errorMessagePrint();
        }
        cin.ignore();
    }
    return discountSelect;
}

void inputData(int* ticketSelect, long long int* customerIDNumber,
    int* orderCount, int* discountSelect)
{
    *ticketSelect = inputTicketSelect();
    *customerIDNumber = inputCustomerIDNumber();
    *orderCount = inputOrderCount();
    *discountSelect = inputDiscountSelect();
}

/************************************************************
   ó����
*************************************************************/
int calcAge(long long int customerIDNumber)
{
    long int calcIDNumber = 0;
    int customerYear = 0, customerMonth = 0, customerDay = 0;
    int todayYear = 0, todayMonth = 0, todayDay = 0;
    int type = 0;
    int koreanAge = 0, age = 0;

    customerIDNumber += FULL_DIGIT;
    calcIDNumber = customerIDNumber / SEVEN_DIGIT;
    type = calcIDNumber % 10;
    calcIDNumber /= ONE_DIGIT;
    customerDay = calcIDNumber % TWO_DIGIT;
    calcIDNumber /= TWO_DIGIT;
    customerMonth = calcIDNumber % TWO_DIGIT;
    calcIDNumber /= TWO_DIGIT;
    customerYear = calcIDNumber % TWO_DIGIT;

    if ((type == MALE_NEW) || (type == FEMALE_NEW))
    {
        customerYear += NEW_GENERATION;
    }
    else
    {
        customerYear += OLD_GENERATION;
    }

    time_t current;
    time(&current);
    struct tm* structTime;
    structTime = localtime(&current);

    todayYear = structTime->tm_year + OLD_GENERATION;
    todayMonth = structTime->tm_mon + 1;
    todayDay = structTime->tm_mday;
    koreanAge = todayYear - customerYear + 1;

    if ((customerMonth < todayMonth)
        || ((customerMonth == todayMonth) && (customerDay <= todayDay)))
    {
        age = koreanAge - AFTER_BIRTH;
    }
    else
    {
        age = koreanAge - BEFORE_BIRTH;
    }
    return age;
}

int calcAgeGroup(int age)
{
    if ((age < MIN_CHILD))
    {
        return 1;
    }
    else if ((age >= MIN_CHILD) && (age <= MAX_CHILD))
    {
        return 2;
    }
    else if ((age >= MIN_TEEN) && (age <= MAX_TEEN))
    {
        return 3;
    }
    else if ((age >= MIN_ADULT) && (age <= MAX_ADULT))
    {
        return 4;
    }
    else if ((age > MAX_ADULT))
    {
        return 5;
    }
}

int calcPriceProcess(int age, int ticketSelect)
{
    int calcPrice = 0;

    if (calcAgeGroup(age) == BABY)
    {
        calcPrice = BABY_PRICE;
    }
    else if (calcAgeGroup(age) == CHILD)
    {
        if (ticketSelect == 1)
        {
            calcPrice = CHILD_DAY_PRICE;
        }
        else if (ticketSelect == 2)
        {
            calcPrice = CHILD_NIGHT_PRICE;
        }
    }
    else if (calcAgeGroup(age) == TEEN)
    {
        if (ticketSelect == 1)
        {
            calcPrice = TEEN_DAY_PRICE;
        }
        else if (ticketSelect == 2)
        {
            calcPrice = TEEN_NIGHT_PRICE;
        }
    }
    else if (calcAgeGroup(age) == ADULT)
    {
        if (ticketSelect == 1)
        {
            calcPrice = ADULT_DAY_PRICE;
        }
        else if (ticketSelect == 2)
        {
            calcPrice = ADULT_NIGHT_PRICE;
        }
    }
    else
    {
        calcPrice = OLD_DAY_PRICE;
    }
    return calcPrice;
}

int calcDiscount(int calcPrice, int discountSelect)
{
    switch (discountSelect)
    {
    case 2: // �����
        calcPrice *= DISABLE_DISCOUNT_RATE;
        break;
    case 3: // ����������
        calcPrice *= MERIT_DISCOUNT_RATE;
        break;
    case 4: // ���ڳ�
        calcPrice *= MULTICHILD_DISCOUNT_RATE;
        break;
    case 5: // �ӻ��
        calcPrice *= PREGNANT_DISCOUNT_RATE;
        break;
    default:
        break;
    }
    return calcPrice;
}

int calcPriceResult(int calcPrice, int orderCount)
{
    return calcPrice * orderCount;
}

int processIntergration(long long int customerIDNumber, int ticketSelect,
    int discountSelect, int orderCount,
    int priceResult, int* age)
{
    int calcPrice = 0;

    *age = calcAge(customerIDNumber);

    calcPrice = calcPriceProcess(*age, ticketSelect);

    calcPrice = calcDiscount(calcPrice, discountSelect);

    priceResult = calcPriceResult(calcPrice, orderCount);

    return priceResult;
}

void saveOrderList(int ticketSelect, int age, int orderCount,
    int priceResult, int discountSelect,
    int* position, int(*orderList)[5])
{
    orderList[*position][0] = ticketSelect;
    orderList[*position][1] = age;
    orderList[*position][2] = orderCount;
    orderList[*position][3] = priceResult;
    orderList[*position][4] = discountSelect;
    (*position)++;
}

/************************************************************
    ��º�
*************************************************************/
void pricePrint(int priceResult)
{
    cout << "������ " << priceResult << " �� �Դϴ�." << endl;
    cout << "�����մϴ�." << endl << endl;
}

void orderPrint(int totalPrice, int* position, int(*orderList)[5])
{
    cout << "Ƽ�� �߱��� �����մϴ�. �����մϴ�." << endl;
    cout << endl;
    cout << "==================== �������� ====================" << endl;

    for (int index = 0; index < *position; index++)
    {
        switch (orderList[index][0])
        {
        case 1: // �ְ���
            cout << "�ְ��� ";
            break;
        case 2: // �߰���
            cout << "�߰��� ";
            break;
        default:
            break;
        }

        if (calcAgeGroup(orderList[index][1]) == BABY)
        {
            cout << "����   ";
        }
        else if (calcAgeGroup(orderList[index][1]) == CHILD)
        {
            cout << "��� ";
        }
        else if (calcAgeGroup(orderList[index][1]) == TEEN)
        {
            cout << "û�ҳ� ";
        }
        else if (calcAgeGroup(orderList[index][1]) == ADULT)
        {
            cout << "�   ";
        }
        else
        {
            cout << "����   ";
        }

        cout << "X " << orderList[index][2] << " ";
        cout << orderList[index][3] << "��\t";

        switch (orderList[index][4])
        {
        case 1: // ����
            cout << "*������� ����" << endl;
            break;
        case 2: // �����
            cout << "*����� �������" << endl;
            break;
        case 3: // ����������
            cout << "*���������� �������" << endl;
            break;
        case 4: // ���ڳ�
            cout << "*���ڳ� �������" << endl;
            break;
        case 5: // �ӻ��
            cout << "*�ӻ�� �������" << endl;
            break;
        default:
            break;
        }
    }
    cout << endl;
    cout << "����� �Ѿ��� " << totalPrice << "�� �Դϴ�." << endl;
    cout << "==================================================" << endl;
    cout << endl;
}

void orderFilePrint(int totalPrice, int* position, int(*orderList)[5], int* filePosition)
{
    ofstream outFile;
    outFile.open("report.csv", ios::app);

    time_t timer;
    struct tm* t;
    timer = time(NULL);
    t = localtime(&timer);

    for (int index = 0; index < *position; index++)
    {
        if (*filePosition == 0)
        {
            outFile << "��¥ ,���� ,���ɱ��� ,���� ,���� ,������" << endl;
        }
        (*filePosition)++;
        outFile << t->tm_year + 1900 << setfill('0') << setw(2) << t->tm_mon + 1 << setfill('0') << setw(2) << t->tm_mday << ",";
        switch (orderList[index][0])
        {
        case 1: // �ְ���
            outFile << "�ְ��� ,";
            break;
        case 2: // �߰���
            outFile << "�߰��� ,";
            break;
        default:
            break;
        }

        if (calcAgeGroup(orderList[index][1]) == BABY)
        {
            outFile << "���� ,";
        }
        else if (calcAgeGroup(orderList[index][1]) == CHILD)
        {
            outFile << "��� ,";
        }
        else if (calcAgeGroup(orderList[index][1]) == TEEN)
        {
            outFile << "û�ҳ� ,";
        }
        else if (calcAgeGroup(orderList[index][1]) == ADULT)
        {
            outFile << "� ,";
        }
        else
        {
            outFile << "���� ,";
        }

        outFile << orderList[index][2] << ",";
        outFile << orderList[index][3] << ",";

        switch (orderList[index][4])
        {
        case 1: // ����
            outFile << "����" << endl;
            break;
        case 2: // �����
            outFile << "�����" << endl;
            break;
        case 3: // ����������
            outFile << "����������" << endl;
            break;
        case 4: // ���ڳ�
            outFile << "���ڳ�" << endl;
            break;
        case 5: // �ӻ��
            outFile << "�ӻ��" << endl;
            break;
        default:
            break;
        }
    }
    outFile.close();
}

int orderContinue()
{
    int continueSelect = 0;

    cout << "��� �߱� �Ͻðڽ��ϱ�?" << endl;
    cout << "1. Ƽ�� �߱�" << endl;
    cout << "2. ����" << endl;
    while (true)
    {
        cin >> continueSelect;
        if (continueSelect == 1 || continueSelect == 2)
        {
            break;
        }
        else
        {
            errorMessagePrint();
        }
        cin.ignore(); // �Է� ���۸� ���
    }
    return continueSelect;
}

void errorMessagePrint()
{
    cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
}

int main()
{
    int totalPrice = 0;
    int position = 0;
    int filePosition = 0;
    int reportPosition = 0;
    int orderList[100][5] = { 0 };
    int isExit = 0;
    do
    {
        while (true)
        {
            long long int customerIDNumber = 0;
            int ticketSelect = 0, orderCount = 0, discountSelect = 0;
            int priceResult = 0, age = 0;
            int continueSelect = 0;

            inputData(&ticketSelect, &customerIDNumber,
                &orderCount, &discountSelect);

            priceResult = processIntergration(customerIDNumber,
                ticketSelect, discountSelect, orderCount,
                priceResult, &age);

            saveOrderList(ticketSelect, age, orderCount,
                priceResult, discountSelect,
                &position, orderList);

            totalPrice += priceResult;

            pricePrint(priceResult);

            continueSelect = orderContinue();

            if (continueSelect == 2)
            {
                break;
            }
        }
        orderPrint(totalPrice, &position, orderList);
        orderFilePrint(totalPrice, &position, orderList, &filePosition);

        cout << "��� ����(1: ���ο� �ֹ�, 2: ���α׷� ����) : ";
        cin >> isExit;
        position = 0;
        totalPrice = 0;
    } while (isExit == 1);
    return 0;
}
