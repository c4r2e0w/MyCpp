//УРОК 3. Структура

Задание
Начнём избавляться от пар в коде поисковой системы. Вам понадобятся функции из урока «Ранжирование по релевантности» из предыдущей темы. Можете воспользоваться поиском по материалам, чтобы быстро попасть в нужный урок.
Объявите структуру DocumentContent так же, как в уроке. Теперь функция MatchDocument должна принимать эту структуру, а функции AddDocument, FindAllDocuments и FindTopDocuments — вектор DocumentContent вместо вектора пар.
Формат входных и выходных данных
Первая строка содержит стоп-слова, разделённые пробелом. Вторая строка содержит целое число N, задающее количество документов. Следующие N строк содержат текст документов, в которых слова разделяются одним пробелом. Затем идёт строка S поискового запроса.
Программа должна вывести информацию о топ-5 самых релевантных документов, соответствующих поисковому запросу S, отсортированных по убыванию релевантности, как показано в примере ниже.
Ввод
a an on the in is has been are with for from have be was
4
a small curly guinea pig with grey hair has been found
a young 50 year old crocodile wants to make friends
a strange brown creature was seen in the box of oranges
a strange animal with big ears is building a house for its friends
cheburashka with big ears likes oranges 
Вывод
{ document_id = 3, relevance = 2 }
{ document_id = 2, relevance = 1 } 
Как будет тестироваться ваш код
Тренажёр проверит, что функции, указанные в задании, теперь принимают vector<DocumentContent> вместо vector<pair<int, vector<string>>> и работают корректно.

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

set<string> ParseStopWords(const string& text) {
    set<string> stop_words;
    for (const string& word : SplitIntoWords(text)) {
        stop_words.insert(word);
    }
    return stop_words;
}

vector<string> SplitIntoWordsNoStop(const string& text, const set<string>& stop_words) {
    vector<string> words;
    for (const string& word : SplitIntoWords(text)) {
        if (stop_words.count(word) == 0) {
            words.push_back(word);
        }
    }
    return words;
}

struct DocumentContent {
    int id;
    vector<string> words;
};


void AddDocument(vector<DocumentContent>& documents, const set<string>& stop_words,
                 int document_id, const string& document) {
    const vector<string> words = SplitIntoWordsNoStop(document, stop_words);
    documents.push_back({document_id, words});
}


set<string> ParseQuery(const string& text, const set<string>& stop_words) {
    set<string> query_words;
    for (const string& word : SplitIntoWordsNoStop(text, stop_words)) {
        query_words.insert(word);
    }
    return query_words;
}

int MatchDocument(const DocumentContent& content, const set<string>& query_words) {
    if (query_words.empty()) {
        return 0;
    }
    set<string> matched_words;
    for (const string& word : content.words) {
        if (matched_words.count(word) != 0) {
            continue;
        }
        if (query_words.count(word) != 0) {
            matched_words.insert(word);
        }
    }
    return static_cast<int>(matched_words.size());
}

vector<pair<int, int>> FindAllDocuments(const vector<DocumentContent>& documents,
                                        const set<string>& query_words) {
    vector<pair<int, int>> matched_documents;
    for (const auto& document : documents) {
        const int relevance = MatchDocument(document, query_words);
        if (relevance > 0) {
            matched_documents.push_back({relevance, document.id});
        }
    }
    return matched_documents;
}

vector<pair<int, int>> FindTopDocuments(const vector<DocumentContent>& documents,
                                        const set<string>& stop_words, const string& raw_query) {
    const set<string> query_words = ParseQuery(raw_query, stop_words);
    auto matched_documents = FindAllDocuments(documents, query_words);

    sort(matched_documents.begin(), matched_documents.end(),
         [](const pair<int, int>& a, const pair<int, int>& b) {
             return a.first > b.first; 
         });

    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }

    return matched_documents; 
}

int main() {
    const string stop_words_joined = ReadLine();
    const set<string> stop_words = ParseStopWords(stop_words_joined);

    vector<DocumentContent> documents;
    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        AddDocument(documents, stop_words, document_id, ReadLine());
    }

    const string query = ReadLine();
    for (const auto& [relevance, document_id] : FindTopDocuments(documents, stop_words, query)) {
        cout << "{ document_id = " << document_id << ", relevance = " << relevance << " }" << endl;
    }

    return 0;
}

/*Замените вектор пар {id, вектор слов} на вектор DocumentContent — vector<DocumentContent>. Поля .first и .second замените на .id и .words. Важен порядок полей при добавлении документов в вектор: сначала id, потом вектор слов! Именно в таком порядке объявлены поля в структуре DocumentContent.*/