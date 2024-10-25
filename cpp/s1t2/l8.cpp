//Спринт 3/22: Спринт 1 → Тема 2/5: Структуры и классы → Урок 8/11

Создаём класс поисковой системы
Прежде чем начать проектировать класс поисковой системы, проведём обзор её архитектуры. Сейчас в поисковой системе четыре основные функции:
void AddDocument(vector<DocumentContent>& documents, const set<string>& stop_words, 
                 int document_id, const string& document) { ... }

int MatchDocument(const DocumentContent& content, const set<string>& query_words) { ... }

vector<Document> FindAllDocuments(const vector<DocumentContent>& documents,
                                  const set<string>& query_words) { ... }

vector<Document> FindTopDocuments(const vector<DocumentContent>& documents,
                                  const set<string>& stop_words, 
                                  const string& raw_query) { ... } 
Представьте, что продолжаете разрабатывать поисковую систему так же: добавляете одну за другой новые функции. С какими трудностями вы можете столкнуться?


Правильный ответ
Глядя на параметры функций, трудно понять, какие из параметров относятся к поисковой системе, а какие — к поисковым запросам.

Правильный ответ
Количество параметров функций будет увеличиться по мере роста функционала.

Правильный ответ
Код станет сложнее для разработки и отлаживания.

Правильный ответ
Скорость разработки снизится.

Правильный ответ
Составные части поисковой системы бессистемно разбросаны по коду.
Если бы наша поисковая система была электрическим устройством, она выглядела бы как набор радиодеталей, наспех скреплённых проводами и синей изолентой. В полном соответствии с одним из Законов Мерфи, сложность этой программы будет расти, пока не превысит способности программиста.
Чтобы упростить развитие поисковой системы, создадим класс SearchServer (поисковый сервер). В нём инкапсулируем данные, которые нужны для работы системы. Это позволит держать сложность программы под контролем.
Взглянем на текущую архитектуру поисковой системы. Функции поисковой системы принимают ряд параметров. Некоторые из них относятся к самой поисковой системе, а другие — к параметрам доступа к ней. Данные, которые составляют основу поисковой системы, нужно сделать полями класса SearchServer, а остальные — параметрами методов этого класса.
Какие из перечисленных ниже данных должны стать полями класса SearchServer?


Неправильный ответ
DocumentContent content_

Неправильный ответ
int document_id_

Правильный ответ
set<string> stop_words_

set<string> query_words_

Неправильный ответ
string document_

string raw_query_

Правильный ответ
vector<DocumentContent> documents_
Основу поисковой системы составляют хранящиеся в ней документы, а также множество стоп-слов. Они станут полями вашего класса.
struct DocumentContent {
    int id = 0;
    vector<string> words;
};

class SearchServer {
public:
    /* публичные методы будут здесь */
private:
    vector<DocumentContent> documents_;
    set<string> stop_words_;
}; 
Если сделаем набор документов и множество стоп-слов полями класса, нам больше не нужно будет передавать всё это в методы.
Какие из прежних функций поисковой системы должны стать публичными методами класса SearchServer?


Тоже правильный ответ
AddDocument

Тоже правильный ответ
MatchDocument

Неправильный ответ
FindAllDocuments

Правильный ответ
FindTopDocuments
Публичные методы класса формируют его интерфейс, то есть те операции, которые пользователи могут с этим классом выполнять. Для поисковой системы это:
добавление документа,
поиск наиболее релевантных документов,
проверка, соответствует ли документ поисковому запросу.
А вот поиск всех документов — это деталь реализации, вспомогательный шаг, на основе которого будет реализован метод FindTopDocuments. В классе могут быть приватными не только данные, но и методы. На основе приватных методов будут реализованы публичные.
В классе SearchServer будут следующие методы:
class SearchServer {
public:
    void SetStopWords(const string& text) {...}
    void AddDocument(int document_id, const string& document) {...}
    vector<Document> FindTopDocuments(const string& raw_query) {...}
private:
    vector<Document> FindAllDocuments(const set<string>& query_words) {...}

    vector<DocumentContent> documents_;
    set<string> stop_words_;
}; 
В таком виде код поисковой системы станет опрятнее, а её использование — проще.
int main() {
    SearchServer server;
    server.SetStopWords("a the and or with"s);
    server.AddDocument(1, "white cat with green eyes"s);
    for (auto [id, relevance] : server.FindTopDocuments("white cat")) {
        cout << "id:"s << id << ", relevance: "s << relevance << endl;
    }
} 
Код можно сделать ещё лучше. Структура DocumentContent используется только внутри методов SearchServer, поэтому её можно объявить в приватной области класса. Это спрячет детали реализации поисковой системы и упростит её использование.
class SearchServer {
public:
    ...
private:
    struct DocumentContent {
        int id = 0;
        vector<string> words;
    };
    ...

    vector<DocumentContent> documents_;
    set<string> stop_words_;
}; 
От теории перейдём к практике и шаг за шагом перенесём функционал поисковой системы в класс SearchServer.