// Сегодня мы познакомимся с векторами ("умными" динамическими массивами)

#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> v = {1, 2, 3, 4, 5};  // вектор из пяти элементов

    for (size_t i = 0; i != v.size(); ++i)  // используем специальный тип size_t для индексации
        cout << v[i] << " ";  // переберём и напечатаем эти элементы
    cout << "\n";
}

// Тип size_t - псевдоним для беззнакового целого типа (занимающего, как правило, 4 или 8 байт в зависимости от битности платформы),
// который следует использовать для индексов или размеров контейнеров

// Вывести элементы можно было бы ещё и так:

for (auto elem : v)  // автоматически выводим тип элемента (int)
    cout << elem << " ";


// Рассмотрим задачу: дано N чисел, надо их считать и вывести в обратном порядке

// Сначала - "наивное" решение

#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    cin >> n;  // количество чисел
    
    vector<int> v;

    for (size_t i = 0; i != n; ++i) {
         int x;
         cin >> x;
         v.push_back(x); // добавляем элемент в конец вектора, он при этом динамически растет
    }

    for (int i = n - 1; i >= 0; --i) {
        cout << v[i] << " ";
    }
    cout << "\n";
}

// Обратите внимание, что если бы мы во втором цикле написали бы size_t, то произошла бы неожиданная ошибка:
for (size_t i = n - 1; i >= 0; --i) { // Опасно!
    // В последний момент, когда от нулевого i будет вычитаться единица, i станет очень большим числом (так как size_t беззнаковый)
    // Программа начнёт вести себя неопределенно
    cout << v[i] << " ";
}


// Лучше было бы написать последний цикл как-то так:

for (size_t i = 0; i != v.size(); ++i) {
    cout << v[v.size() - i - 1] << " ";
}


// Заметим, что нам известно заранее количество элементов.
// Поэтому можно было бы сразу зарезервировать необходимое количество ячеек памяти в векторе.
// Иначе у нас при добавлении новых элементов могут происходить реаллокации:
// когда зарезервированная память исчерпывается, то вектор запрашивает больший кусок памяти и копирует туда старое содержимое


#include <iostream>
#include <vector>

int main() {
    int n;
    cin >> n; // количество чисел
    
    vector<int> v;
    v.reserve(n); // зарезервировали память под n целых чисел (но размер вектора пока все равно нулевой)
    // зарезервированная память пока никак не проинициализирована, обращаться к ней некорректно

    int x;
    for (size_t i = 0; i != n; ++i) {
         cin >> x;
         v.push_back(x); // добавляем элементы как обычно
    }
}


// А можно было бы сделать еще и так:

#include <iostream>
#include <vector>

int main() {
    int n;
    cin >> n; // количество чисел
    
    vector<int> v(n); // сразу инициализируем память под n целых чисел
    // размер вектора будет равен n, память будет заполнена нулями

    for (size_t i = 0; i != n; ++i) {
         cin >> v[i];
    }
}

// Если бы вместо int тут оказался бы более сложный тип, то делать так было расточительно:
// инициализация элементов значениями по умолчанию могла бы оказаться достаточно дорогой.



// Объём зарезервированной и реально занятой памяти в векторе можно узнать с помощью функций capacity() и size().
// Изменить их, соотвественно, можно с помощью reserve и resize.

#include <iostream>
#include <vector>

using namespace std;

int main() {
   vector<int> v = {1, 2, 3, 4, 5};
   cout << v.size() << "\n";  // напечатает 5
   cout << v.capacity() << "\n";  // зависит от реализации, но скорее всего, напечатает 8

   // попробуем подобавлять еще элементы и проследить, как меняется занятый и зарезервированный размер
   for (int i = 6; i != 10; ++i) {
       v.push_back(i);
       cout << "size: " << v.size() << "\n";
       cout << "capacity: " << v.capacity() << "\n";
   }
}

// Одна из стратегий - резервировать размер вдвое большего размера, чем нужно, когда зарезервированная память исчерпывается
// В среднем это приводит к тому, что добавление новых элементов в конец вектора требует константного времени
// Подробности вам расскажут на курсе алгоритмов в 3-м модуле


// ---------------

// Сортировка вектора

#include <algorithm> // в этом заголовочном файле находится функция sort
#include <iostream>
#include <vector>

int main() {
    size_t n;
    cin >> n;
    vector<int> v;

    int x;
    for (size_t i = 0; i != n; ++i) {
        cin >> x;
        v.push_back(x);
    }

    sort(v.begin(), v.end()); // передаем в функцию sort пару итераторов, ограничивающих полуинтервал обрабатываемого участка

    for (int elem : v) {
        cout << elem << " ";
    }
    cout << "\n";
}


// Обратите внимание, что оператор [] у вектора никак не проверяет корректность индексов.
// Если вы обратитесь к вектору по некорректному индексу, то программа попадёт в неопределённое поведение.
// Вы должны сами следить за тем, чтобы индекс был корректным.

// Если вам не хочется делать много лишних проверок, то можно использовать функцию at:
vector<int> v = {1, 2, 3};
cout << v.at(100500);  // произойдёт исключение - его можно будет перехватить и обработать


// ---------------

// Строки (std::string) можно рассматривать как просто как векторы символов.
// В частности, к строке можно дописывать символы с помощью push_back, можно менять размер с помощью resize и т. д.

// У строк есть несколько особых полезных методов.
// Например, функция find находит первое вхождение символа:

#include <string>

int main() {
    std::string str = "Hello, world!";
    // ...
    size_t pos = str.find(' ');  // если символ не найден, вернется специальное значение std::string::npos (равное самому большому числу из size_t).
    if (pos != std::string::npos) {
        std::string left = str.substr(0, pos);  // левая подстрока (до пробела)
        std::string right = str.substr(pos + 1);  // правая подстрока (после пробела)
    }
}

// Функция str.substr(start, len) вырезает из str подстроку длины len начиная с позиции start.
// Если пропустить последний аргумент, то будет вырезана подстрока от start до конца строки str.

// Обратите внимание, что у функции find есть дополнительный необязательный аргумент: позиция, начиная с которой начинать поиск.
// Он может быть полезен в задаче, где надо строку разбить на части по разделителю, а частей много.

// Для поиска самого правого вхождения символа в строку есть аналогичная функция rfind.


// ---------------

// Теперь поговорим о структурах.
// Структура в первом приближении - это созданный пользователем тип данных, 
// объединяющий несколько переменных (полей) "под одной крышей"
// В C++ структуры и классы почти ничем не отличаются
// (все члены структур по умолчанию открыты, а в классе - закрыты).
// Но мы будем использовать слово "структура", когда не будем хотеть от этого типа ничего другого,
// кроме объединения полей. 
// Мы не будем пока определять функции-члены структуры, писать конструкторы и деструкторы, перегружать операторы и т. д.
// Всё это мы, конечно, будем проходить, но чуть позже. И будем говорить тогда не "структура", а "класс".
// А сейчас "структура" для нас - это POD (plain old data, "старые добрые данные") - есть такой термин в C++-жаргоне.

#include <string>
#include <vector>
#include <iostream>

// Дата задаётся днём, месяцем и годом.
// По отдельности эти величины таскать неудобно. Объединим их в структуру.
struct Date {
    int Day;
    int Month;
    int Year;
}; // обратите внимание на эту точку с запятой, она нужна.

// Теперь Date - полноценное новое имя типа данных.

// Студент, например, описывается именем и фамилией, датой рождения, годом поступления и рейтингом.
struct Student {
    std::string Name;
    std::string Surname;
    Date Birthday; // тип этого поля - структура, определенная выше
    int Year;
    double Rating;
};

int main() {
    Date d1 {23, 10, 2015}; // инициализируем поля структуры в порядке их объявления

    Date d2;
    d2.Day = 23; // Но можно было бы присвоить им значения и вот так
    d2.Month = 10;
    d2.Year = 2015;

    Date d3; // обратите внимание: тут по умолчанию int-поля структуры никак не проинициализированы (и там лежит мусор)

    Student s { // описываем студента
        "Ivan",
        "Ivanov",
        d1, // это его дата рождения; можно было бы также написать {23, 10, 2015} 
        2015,
        10.0
    };

    // Допустим, надо считать даты с клавиатуры. Сделаем примерно так:
    size_t n;
    std::cin >> n; // узнаём, сколько будет дат
    std::vector<Date> dates(n); // создаём вектор дат размера N
    for (size_t i = 0; i != n; ++i)
        std::cin >> dates[i].Day >> dates[i].Month >> dates[i].Year;
}

// Как отсортировать даты? Для этого надо научиться их попарно сравнивать.
// Можно определить поведение оператора < для дат:
bool operator < (const Date& d1, const Date& d2) {  // почему const Date&? Про это будет рассказано на следующей лекции
    return d1.Year < d2.Year
        || d1.Year == d2.Year && d1.Month < d2.Month
        || d1.Year == d2.Year && d1.Month == d2.Month && d1.Day < d2.Day;
    // конечно, тут можно было бы написать и обычный if
}

// отсортировать теперь можно так:
#include <algorithm> // надо подключить этот заголовок вверху
// ...
std::sort(dates.begin(), dates.end());

