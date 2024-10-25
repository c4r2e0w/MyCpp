Спринт 3/22: Спринт 1 → Тема 3/5: Лямбда-функции → Урок 2/12
Компаратор для сортировки
В вашей поисковой системе есть метод FindTopDocuments, который сортирует документы по релевантности:
vector<Document> FindTopDocuments(const string& raw_query) const {
    const set<string> query_words = ParseQuery(raw_query);
    auto matched_documents = FindAllDocuments(query_words);

    // Сортируем документы по убыванию релевантности
    sort(matched_documents.begin(), matched_documents.end(), HasDocumentGreaterRelevance);

    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) { 
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }
    return matched_documents;
} 
Посмотрите на вызов сортировки. В нём вы использовали компаратор HasDocumentGreaterRelevance. У этого компаратора довольно простое тело:
bool HasDocumentGreaterRelevance(const Document& lhs, const Document& rhs) {
    return lhs.relevance > rhs.relevance;
} 
Но у метода FindTopDocuments есть проблема. Читая его, нужно переходить в другую часть кода, чтобы посмотреть критерий сортировки. У вашего компаратора понятное имя, однако лучше видеть именно код, который будет выполняться.
Сам компаратор недлинный. Удобнее, если его тело находится рядом с вызовом сортировки. Так его можно быстро прочитать и понять, по какому критерию сортируются документы.
Когда удобнее и понятнее иметь в месте вызова тело функции, а не её название, применяют лямбда-функции, или безымянные функции. Вот как можно переписать метод FindTopDocuments с лямбда-функцией в качестве компаратора:
vector<Document> FindTopDocuments(const string& raw_query) const {
    const set<string> query_words = ParseQuery(raw_query);
    auto matched_documents = FindAllDocuments(query_words);

    sort(matched_documents.begin(), matched_documents.end(),
        [](const Document& lhs, const Document& rhs) {  // Это лямбда-функция
            return lhs.relevance > rhs.relevance;
        });

    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }
    return matched_documents;
} 
Третьим параметром функции sort вместо HasDocumentGreaterRelevance передано нечто похожее на определение функции. Сначала идёт пара квадратных скобок — []. Они сообщают компилятору, что это лямбда-функция. Дальше в круглых скобках — список параметров, как у обычной функции. После в фигурных скобках идёт тело — тоже как в обычных функциях.
Если добавить лямбда-функцию в метод FindTopDocuments, сразу будет видно, по какому критерию выполняется сортировка. К тому же больше не надо запоминать, что значит имя HasDocumentGreaterRelevance. Оно всё равно использовано всего один раз. Читатель теперь не перегружен лишними именами.
У лямбда-функции обычно нигде не указывают тип возвращаемого значения. Компилятор сам выводит его по типу выражения в команде return. Однако лямбда-функции, как и обычные функции могут ничего не возвращать, а, например, выводить какую-либо информацию в поток.
В данном случае лямбда-функция возвращает результат сравнения двух целых чисел, поэтому тип возвращаемого значения — bool. Впрочем, если нужно, тип возвращаемого значения можно указать явно:
sort(matched_documents.begin(), matched_documents.end(),
    [](const Document& lhs, const Document& rhs) -> bool { // указали тип результата явно
        return lhs.relevance > rhs.relevance;
    }); 
Выберите лямбда-функции, которые принимают два целых числа и возвращают результат типа bool.


Тоже правильный ответ
[](int x, int y) { return x > y; }

[](int x, int y) { return x + y; }

Неправильный ответ
[](int x, double y) { return (x > y) ? x : y; }

Правильный ответ
[](int x, int y) { return x > 0 && y > 0; }
Несмотря на то, что лямбда-функции удобны для создания «одноразовых» функций, их можно использовать и многократно. Для этого им надо задать имя и вызывать как обычные функции:
#include <iostream>
#include <string>
using namespace std;

int Test() {
    // Объявили лямбда-функцию quote
    auto quote = [](const string& s) {
        return '"' + s + '"';
    }; // Точка с запятой обязательна, как при объявлении переменной

    string movie1 = "Forrest Gump"s;
    string movie2 = "Green Mile"s;

    // Функцию quote можно вызвать несколько раз
    cout << "My favorite movies are "s << quote(movie1) << " and "s << quote(movie2) << endl;
    // Выведет: My favorite movies are "Forrest Gump" and "Green Mile"
} 
Эту особенность можно использовать даже когда лямбда-функция вызывается один раз. Это может улучшить читаемость кода:
// Так как лямбда-функция объявлена близко к месту её использования,
// ей можно дать короткое, но понятное имя
auto by_relevance = [](const Document& lhs, const Document& rhs) {
    return lhs.relevance > rhs.relevance;
};

// Сортируем документы от начала и до конца по релевантности
sort(matched_documents.begin(), matched_documents.end(), by_relevance); 
От теории перейдём к практике и улучшим код поисковой системы.
