// Сегодня у нас важная тема - указатели и работа с динамической памятью.
// Мы специально не рассматривали её в предыдущем модуле,
// ограничиваясь высокоуровневыми вещами - контейнерами стандартной библиотеки, итераторами, алгоритмами...
// Настала пора погрузиться глубже.

#include <iostream>

int main() {
    int x = 17;
    int& ref = x;  // Все помнят обычные ссылки: это псевдонимы для переменных
    // Ссылки удобны при передаче параметров в функции:
    // обычные ссылки позволяют изменять аргументы,
    // а (константные) ссылки спасают нас от дорогого копирования сложных объектов.

    // Указатели чем-то похожи на ссылки:
    // переменная-указатель хранит адрес в памяти другой переменной заданного типа.
    int * ptr;  // ptr - это указатель на int, тип этой переменной - int *
    // Но в отличие от ссылок указатель не обязан быть сразу же проинициализирован.
    // Только что объявленный локальный указатель ptr никак не проинициализирован и содержит "мусорный" адрес.

    ptr = &x;  // присваиваем указателю адрес переменной x

    std::cout << *ptr << "\n";  // унарная * - это "разыменование" указателя,
    // то есть, обращение к значению, которое хранится по данному адресу.
    *ptr = 42;  // разыменованному указателю можно что-либо присваивать (если позволяет константность)
    // Теперь значение переменной x изменилось и равно 42.

    // В отличие от ссылок, указатели можно переприсваивать:
    int y = 123;
    ptr = &y;  // теперь ptr хранит адрес переменной y

    std::cout << *ptr << "\n";  // 123

    // Наконец, в отличие от ссылок существует выделенный "нулевой" указатель,
    // который ни на что не ссылается:
    ptr = nullptr;
    // Такой указатель нельзя разыменовывать.

    // По-хорошему, перед каждым разыменованием мы должны быть уверены,
    // что наш указатель не нулевой:
    if (ptr != nullptr)
        std::cout << *ptr << "\n";
    // Иначе поведение программы будет неопределенным.

    // Пусть в вашей программе есть два тяжёлых объекта (например, векторы),
    // и в алгоритме надо поочередно работать то с одним, то с другим (причём заранее неизвестно, с каким): 
    std::vector<int> v1, v2;

    // В этом случае вам как раз подойдёт указатель, который можно по ходу дела перенастраивать на один или другой объектЖ
    std::vector<int> * ptr = &v1;
    // ...
    ptr = &v2;
}

// ==================

// Вспомним, как мы писали функцию swap:
void swap(int& a, int& b) {  // передача параметра по ссылке, чтобы параметры можно было менять
    int c = a;
    a = b;
    b = c;
}

// Вот та же функция с аргументами-указателями:
void swap(int * a, int * b) {
    int c = *a;  // верим, что a != nullptr
    *a = *b;  // заметьте, везде приходится ставить звёздочки перед указателями
    *b = c;
}

int main() {
    int x = 1, y = 2;
    swap(x, y);   // 1-я версия
    swap(&x, &y); // 2-я версия
    swap(nullptr, &y);  // так работать не будет!

    // Версия со ссылками предпочтительнее.
    // Однако некоторые любят использовать версию с указателями, поскольку в этом случае в месте вызова
    // приходится ставить &, и явно видно, что эти аргументы будут меняться.
}

// ==================

// Ссылки легким движением руки можно сделать невалидными:
int& f() {
    int x = 17;
    return x;  // опасно! Локальная переменная возвращается из функции по ссылке
}
// После выхода из функции ссылка станет "висячей", так как локальная переменная x уже умрёт

// Сделать невалидный указатель ещё проще:
int main() {
    int * ptr;
    {
        int x = 17;
        ptr = &x;
    }
    std::cout << *ptr << "\n";  // Неопределенное поведение: разыменовывать такой указатель больше нельзя!
}

// Будьте осторожны с такими конструкциями!

// ==================

// До сих пор мы привязывали указатели к адресам уже существующих локальных переменных.
// Но часто указатели используются именно с динамической памятью. Поговорим теперь про неё.

// Обычная переменная создаётся на "стеке": при выходе из блока для такой переменной автоматически вызывается деструктор:
// Но мы можем создать переменную и в динамической памяти (куче). Тогда за её жизнью мы обязаны следить сами.
// Так, мы должны уничтожить эту переменную, когда она нам станет не нужна.
    
int main() {
    int * ptr = new int(42);  // создаём новую переменную типа int со значением 42 в динамической памяти
    // и запоминаем её адрес

    *ptr = 17;
    std::cout << *ptr << "\n";

    delete ptr;  // когда переменная станет не нужна, надо её обязательно удалить и освободить память
    // Заметьте, это не обязательно делать в конце блока (как в этом примере).
    // Динамическая переменная может "пережить" выход за границы блока.
}

// Опасной ошибкой называется утечка памяти.
// Так называется ситуация, когда не нужная больше память не освобождается (из-за ошибок программиста).
// Если новая динамическая память регулярно выделяется (например, в цикле) в процессе работы программы,
// то рано или поздно она может просто исчерпаться.

// Вот простейший пример утечки. Не пытайтесь это повторить:
int main() {
    while (true) {
        new int;  // создаём новую (нулевую) переменную и тут же забываем её адрес (и тем более не удаляем)
    }
}

// Если же вы просто написали вот так
int main() {
    int * ptr = new int;
    // ...
}
// и не удалили ptr в конце работы, то может показаться, что утечки нет.
// Действительно, программа будет запускаться и нормально и ожидаемо работать.
// Операционная система после завершения программы сама подчистит занимаемые ею ресурсы.
// Однако мы будет и такую ситуацию считать утечкой памяти.
// ВАЖНО: всю динамическую память вы должны удалить сами, иначе мы будем считать это ошибкой!

// ==================

// Заметим, что операции new и delete транзакционные.
// Операция new выполняет на самом деле два действия:
// 1. Запрашивает у операционной системы нужный кусок памяти;
// 2. Конструирует в этой памяти новый объект.

// Каждый из этих этапов может закончиться ошибкой.
// "Транзакционность" означает, что если, скажем, ошибка произошла на втором этапе
// (из-за исключения в конструкторе), то new вернет выделенную память обратно операционной системе,
// чтобы она не утекла.

// Точно так же delete выполняет на самом деле два действия:
// 1. Вызывает деструктор для объекта;
// 2. Возвращает выделенную память обратно операционной системе.
// При этом в хорошо написанной программе деструкторы не должны генерировать исключений,
// и в этом случае операции delete никаких сбоев не должно происходить.

// Проверим, что конструкторы и деструкторы действительно вызываются в new/delete:
class C {
public:
    C() {
        std::cout << "C()\n";
    }
    ~C() {
        std::cout << "~C()\n";
    }
};

int main() {
    auto ptr = new C;  // C()
    delete ptr;        // ~C()
}

// ==================

// Можно выделять динамическую память не только под отдельные переменные, но и под подряд идущие блоки - массивы:
int main() {
    int * x = new int[10];  // обратите внимание на квадратные скобки!
    // Тут создаётся блок из 10 подряд идущих int'ов, все ячейки заполняются нулями и возвращается адрес первого блока.

    // В пределах блока можно применять к указателями арифметические операции:
    *x = 17;  // изменили первый элемент блока
    *(x + 3) = 42;  // изменили четвёртый элемент блока
    int * y = x + 10;  // разрешается взять адрес следующей за последней ячейки (но её нельзя разыменовывать);
    *(y - 1) = -1;  // изменили последний (десятый) элемент
    std::cout << (y - x) << "\n";  // 10

    // Обратите внимание, что арифметические операции оперируют блоками указанного типа (у нас int, это 4 или 8 байт),
    // а не отдельными байтами.
    
    // Всё это очень похоже на знакомые вам итераторы!
    // (На самом деле всё наоборот: это итераторы создавались так, чтобы быть похожими на указатели!)

    delete [] x;  // выделенную память надо обязательно удалить (предварительно вызвав деструкторы для элементов),
    // причем для массива необходимо вызывать delete [], а не delete.
    // В delete [] обязательно надо передавать указатель именно на начало блока.
    
    // Путаница delete и delete[] - опасная ошибка, приводящая к неопределенному поведению.
}

// ==================

// Наконец рассмотрим особый случай динамических массивов символов.
// В языке C так можно было задавать строки.
// В стандартной библиотеке C++ есть удобные обёртки над динамическими массивами (vector для любых массивов, string для строк).
// Всегда лучше пользоваться этими классами-обёртками, если не требуется что-то особое.

int main() {
    char * str = new char[100];  // вместо 100 тут, конечно, может стоять переменная величина
    std::cin >> str;  // cin умеет работать с массивами char'ов
    // Обратите внимание: будет проблема, если длина вводимой строки больше 99 символов!
    // Почему 99, а не 100?
    // Потому что после последнего символа строки будет ещё принудительно записан символ с кодом 0,
    // означающий её конец.
    // Поэтому такие строки и называются Null-terminated strings (строки с завершающим нулём).

    std::cout << str << "\n";
    delete [] str;
}


// ==================

// Что не так в этой программе? (Это реальный случай из жизни).
int main() {
    char * str;
    std::cin >> str;
    std::cout << "Hello, " << str << "\n";
}

// Правильно: тут студент вообще забыл выделить динамическую память!


// ==================

// А тут студент решил проверить, совпадают ли две строки:

int main() {
    char * s1 = new char[100];
    char * s2 = new char[100];
    std::cin >> s1;
    std::cin >> s2;
    std::cout << (s1 == s2 ? "Equal\n" : "Not equal\n");  // всё ли он правильно сделал?
    delete [] s1;
    delete [] s2;
}

// Конечно же, тут будут просто сравниваться два указателя, а не содержимое блоков!
// Чтобы сравнить две null-terminated строки, в С была специальная библиотечная функция strcmp.
// Она возвращала целое число, которое можно сравнить с нулём и узнать результат сравнения:

#include <cstring>

int main() {
    char * s1 = new char[100];
    char * s2 = new char[100];
    std::cin >> s1;
    std::cin >> s2;
    int res = strcmp(s1, s2);
    if (res == 0)
        std::cout << "Equal\n";
    else if (res < 0)
        std::cout << "s1 < s2\n";
    else
        std::cout << "s1 > s2\n";
    delete [] s1;
    delete [] s2;
}

// Вы попробуете сами написать реализацию этой функции в домашней работе.
// А пока рассмотрим функцию strcpy, которая копирует одну null-terminated строку в другую.
// Она похожа на алгоритм std::copy, но её не требуется передавать указатель на конец:

#include <algorithm>
#include <cstring>

int main() {
    char * s1 = new char[100];
    char * s2 = new char[100];
    std::cin >> s1;
    // Алгоритм copy пришлось бы вызывать как-то так:
    std::copy(s1, s1 + strlen(s1) + 1, s2);  // +1, потому что нужно захватить символ с кодом 0
    // Старую добрую функцию strcpy можно вызывать так:
    strcpy(s2, s1);  // она сама разберётся с нулями и определит длину s1

    // В каждом из способов важно только, чтобы в s2 было выделено достаточно памяти, чтобы туда поместился результат.
}

// Реализация strcpy выглядит особенно элегантно:

void strcpy(char * dst, const char * src) {  // на самом деле возвращается dst, но это не важно
    while (*dst++ = *src++);  // разберитесь-ка с этим циклом!
}