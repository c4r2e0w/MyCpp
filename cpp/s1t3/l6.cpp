Захват переменных по ссылке
Чтобы обратиться к локальной переменной внутри лямбда-функции, переменную нужно явно захватить, указав её имя внутри квадратных скобок:
int CountMinusWords(const vector<string>& words, char minus_prefix) {
    return count_if(
        words.begin(), words.end(),
        [minus_prefix](const string& w) {
            return !w.empty() && w[0] == minus_prefix;
        }
    );
} 
С этим требованием вы познакомились в прошлом уроке. Пора узнать, зачем оно нужно. Дело в том, что все локальные переменные, захваченные лямбда-функцией, копируются внутрь неё. При этом происходит глубокое копирование. Как вы уже знаете, оно занимает много времени.
Допустим, у вас есть поисковый запрос и множество стоп-слов. Вы хотите посчитать, сколько слов запроса не будут стоп-словами. Подойдёт алгоритм count_if:
int CountQueryWords(const vector<string>& query, const set<string>& minus_words) {
    return count_if(
        query.begin(), query.end(),
        [minus_words](const string& w) { // Множество minus_words копируется внутрь лямбда-функции
            return minus_words.count(w) == 0;
        }
    );
} 
При вызове алгоритма count_if множество minus_words копируется внутрь передаваемой в него лямбды. Множество set внутри устроено сложно. Если в нём большое количество элементов, а функция CountQueryWords вызывается часто, копирование замедлит программу.
Более того, в копировании нет смысла: вы принимаете множество minus_words по константной ссылке и только читаете его. Копия minus_words для этого не нужна — прочитать можно из оригинала. Такая ситуация нарушает основной принцип С++ — «не платите за то, что не используете».
Написать вызов алгоритма count_if нужно так, чтобы обратиться ко множеству minus_words изнутри лямбда-функции, но избежать глубокого копирования.
Переменную minus_words можно захватить по ссылке. Для этого перед именем переменной в квадратных скобках добавляют символ &:
int CountQueryWords(const vector<string>& query, const set<string>& minus_words) {
    return count_if(
        query.begin(), query.end(),
        [&minus_words](const string& w) { // Обратите внимание на символ & перед minus_words
            return minus_words.count(w) == 0;
        }
    );
} 
Символ & перед именем переменной говорит компилятору, чтобы вместо глубокого копирования переменной minus_words внутрь лямбды он сохранил только ссылку на неё.
Если лямбда-функция захватывает несколько переменных, способ захвата нужно прописать для каждой переменной отдельно:
int CountShortQueryWords(const vector<string>& query,
    const set<string>& minus_words, int max_length)
{
    return count_if(query.begin(), query.end(),
        [max_length, &minus_words](const string& w) {
            return w.size() <= max_length && minus_words.count(w) == 0;
        });
} 
Захват переменной по ссылке сокращает глубокое копирование и время работы программы.
Явное указание переменных и значений, которые вы хотите захватить, позволяет избежать ошибок и убедиться, что лямбда-функция не использует ничего лишнего. Можно однако попросить компилятор автоматически определить, какие переменные нуждаются в захвате. 
Для этого в квадратных скобках ставится знак =:
int CountShortQueryWords(const vector<string>& query,
    const set<string>& minus_words, int max_length)
{
    return count_if(query.begin(), query.end(),
        // Компилятор проверит, что в лямбда-функции используются переменные 
        // max_length и minus_words. Получится захват, эквивалентный
        // [max_length, minus_words]:
        [=](const string& w) {
            return w.size() <= max_length && minus_words.count(w) == 0;
        });
} 
Такую конструкцию лучше применять в простых случаях, когда легко догадаться, что именно использует функция.
Если вы хотите захватить все переменные по ссылке, то вместо знака = указывайте &:
int CountShortQueryWords(const vector<string>& query,
    const set<string>& minus_words, int max_length)
{
    return count_if(query.begin(), query.end(),
        // В этом случае получится захват, эквивалентный [&max_length, &minus_words]:
        [&](const string& w) {
            return w.size() <= max_length && minus_words.count(w) == 0;
        });
} 
Захваты = и & используются в качестве захватов по умолчанию. Их можно комбинировать с явным указанием конкретных переменных, которые должны быть захвачены по другому правилу. При этом захват по умолчанию должен стоять на первом месте. Захваты по умолчанию нельзя комбинировать друг с другом.
Как можно получить захват, эквивалентный [max_length, &minus_words] для лямбда-функции из примера выше? Выберите один или несколько вариантов:


[=, &minus_words]

[&, &minus_words]

[=, max_length]

[&, max_length]

[minus_words, &max_length]

[&minus_words, max_length]

[&]

[=, &]

Перейти к заданию

Краткий пересказ урока


#include <map>
#include <string>
#include <vector>
#include <iostream>
int CountAndAddNewDogs(const vector<string>& new_dogs, const map<string, int>& max_amount,
                       map<string, int>& shelter) {
10
    //...
11
}/*
Задание
В приют привозят собак разных пород. Информация о них хранится в двух словарях:
словарь shelter хранит текущее количество собак каждой породы,
словарь max_amount — максимальное возможное количество собак каждой породы, которое может вместить приют. Может иметь значение 0.
Оба словаря используют название породы в качестве ключа и количество в качестве значения.
Напишите функцию CountAndAddNewDogs, которая будет на основании вектора с породами поступивших собак обновлять словарь shelter и возвращать количество собак, которых удалось пристроить в приют.
Что нужно сделать
Напишите функцию CountAndAddNewDogs, которая будет на основании вектора с породами поступивших собак обновлять словарь shelter и возвращать количество собак, которых удалось пристроить в приют. Для решения используйте лямбда-функцию.
Формат входных данных
Гарантируется, что никакие другие собаки, кроме известных из словаря max_amount, в приют не приедут. Однако в словаре shelter могут изначально присутствовать не все породы.
В векторе new_dogs перечисляются породы поступивших собак в произвольном порядке. Значения в этом векторе могут повторяться. 
Примеры
Пример 1
Входные данные:
*/
map<string, int> shelter = {{"landseer"s, 1}, {"otterhound"s, 2}, {"pekingese"s, 2}, {"pointer"s, 3}};
const map<string, int> max_amount = {{"landseer"s, 2}, {"otterhound"s, 3}, {"pekingese"s, 4}, {"pointer"s, 7}};
const vector<string> new_dogs = {"landseer"s, "otterhound"s, "otterhound"s, "otterhound"s, "pointer"s};
cout << CountAndAddNewDogs(new_dogs, max_amount, shelter) << endl; 
Результат:
3 
При этом словарь shelter должен изменить значение:
{{"landseer"s, 2}, {"otterhound"s, 3}, {"pekingese"s, 2}, {"pointer"s, 4}}; 
Пример 2
Входные данные:
map<string, int> shelter = {{"beagle"s, 4}, {"dachshund"s, 1}};
const map<string, int> max_amount = {{"beagle"s, 4}, {"dachshund"s, 2}, {"pug"s, 0}};
const vector<string> new_dogs = {"beagle"s, "pug"s, "pug"s, "beagle"s};
cout << CountAndAddNewDogs(new_dogs, max_amount, shelter) << endl; 
Результат:
0 
При этом словарь shelter изменился:
{{"beagle"s, 4}, {"dachshund"s, 1}, {"pug"s, 0}}; 
Требования
Не меняйте сигнатуру функции CountAndAddNewDogs
Для нахождения ответа используйте алгоритм count_if.
Вектор max_amount принимается по константной ссылке и поэтому не должен меняться. В частности, с ним запрещается использовать операцию [].
Тело функции должно состоять из одного return.
Не используйте циклы.
Как будет тестироваться ваш код
Тренажёр проверит работу функции CountAndAddNewDogs, передавая ей различные комбинации входных данных и сравнивая возвращаемое значение и содержимое контейнера shelter c ожидаемым.

Подсказка




Запустить

Проверить
Терминал

Спринт 3/22: Спринт 1 → Тема 3/5: Лямбда-функции → Урок 6/12

