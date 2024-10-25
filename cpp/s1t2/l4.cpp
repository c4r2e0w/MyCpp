//Спринт 3/22: Спринт 1 → Тема 2/5: Структуры и классы → Урок 4/11

Сортировка вектора структур
Вы справились с внедрением структуры DocumentContent в код поисковой системы. Это упростило хранение документов — вместо вектора пар используется вектор структур. Пора проделать то же самое с возвращаемыми результатами функций FindAllDocuments и FindTopDocuments.
Сначала объявите новую структуру Document.
struct Document {
    int id;
    int relevance;
}; 
Используйте структуру вместо pair<int, int> в типе возвращаемого значения функции FindAllDocuments.
Прежний код:
// Для каждого документа возвращает его релевантность и id
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
Новый код:
vector<Document> FindAllDocuments(const vector<DocumentContent>& documents,
                                  const set<string>& query_words) {
    vector<Document> matched_documents;
    for (const auto& document : documents) {
        const int relevance = MatchDocument(document, query_words);
        if (relevance > 0) {
            // В структуре Document первое поле - id, а второе - релевантность
            matched_documents.push_back({document.id, relevance});
        }
    }
    return matched_documents;
} 
Обратите внимание, что в прежней версии функции FindAllDocuments релевантность документа была первым элементом пары. Это нужно для сортировки пар по релевантности: сначала документы сортировались по релевантности, а документы с одинаковой релевантностью — по id документа. В структуре Document порядок полей противоположный.
Напишем основную функцию — FindTopDocuments.
vector<Document> FindTopDocuments(const vector<DocumentContent>& documents,
                                  const set<string>& stop_words, const string& raw_query) {
    const set<string> query_words = ParseQuery(raw_query, stop_words);
    auto matched_documents = FindAllDocuments(documents, query_words);
    // Сортировка по убыванию релевантности
    // Раньше здесь было sort и reverse
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }
    // Исправление порядка полей в парах
    return matched_documents;
} 
Теперь везде вместо пар структура, поэтому исправлять порядок полей в конце функции больше не придётся. Зато надо сортировать структуры в определённом порядке. Попробуйте просто взять и вызвать sort для вектора документов:
#include <algorithm>
#include <vector>
using namespace std;
struct Document {
    int id;
    int relevance;
};
int main() {
    vector<Document> documents;
    sort(documents.begin(), documents.end());
} 
Сообщение об ошибке будет громоздким. Конкретный его вид будет зависеть от используемого компилятора:
In file included from <...>/include/c++/bits/stl_algobase.h:71,
                 from <...>/include/c++/algorithm:61,
                 from test.cpp:1:
<...>/include/c++/bits/predefined_ops.h: In instantiation of 'constexpr bool __gnu_cxx::__ops::_Iter_less_iter::operator()(_Iterator1, _Iterator2) const [with _Iterator1 = __gnu_cxx::__normal_iterator<Document*, std::vector<Document> >; _Iterator2 = __gnu_cxx::__normal_iterator<Document*, std::vector<Document> >]':
<...>/include/c++/bits/stl_algo.h:81:17:   required from 'void std::__move_median_to_first(_Iterator, _Iterator, _Iterator, _Iterator, _Compare) [with _Iterator = __gnu_cxx::__normal_iterator<Document*, std::vector<Document> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
<...>/include/c++/bits/stl_algo.h:1921:34:   required from '_RandomAccessIterator std::__unguarded_partition_pivot(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Document*, std::vector<Document> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
<...>/include/c++/bits/stl_algo.h:1953:38:   required from 'void std::__introsort_loop(_RandomAccessIterator, _RandomAccessIterator, _Size, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Document*, std::vector<Document> >; _Size = long long int; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
<...>/include/c++/bits/stl_algo.h:1968:25:   required from 'void std::__sort(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Document*, std::vector<Document> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
<...>/include/c++/bits/stl_algo.h:4834:18:   required from 'void std::sort(_RAIter, _RAIter) [with _RAIter = __gnu_cxx::__normal_iterator<Document*, std::vector<Document> >]'
test.cpp:13:44:   required from here
<...>/include/c++/bits/predefined_ops.h:43:23: error: no match for 'operator<' (operand types are 'Document' and 'Document')
       { return *__it1 < *__it2; }
                ~~~~~~~^~~~~~~~
In file included from <...>/include/c++/bits/stl_algobase.h:67,
<...> 
Ошибка в 13-й строке программы — вызов sort:
<...>
<...>/include/c++/bits/stl_algo.h:4834:18:   required from 'void std::sort(_RAIter, _RAIter) [with _RAIter = __gnu_cxx::__normal_iterator<Document*, std::vector<Document> >]'
test.cpp:13:44:   required from here 
В сообщении говорится: вызвана функция std::sort, из неё — std::__sort, затем std::__introsort_loop. Писать sort вместо std::sort можно благодаря using namespace std;. То, как реализация сортировки разбита на подфункции, сейчас неважно. Основная проблема — в сообщении ниже:
<...>/include/c++/bits/predefined_ops.h:43:23: error: no match for 'operator<' (operand types are 'Document' and 'Document')
       { return *__it1 < *__it2; }
                ~~~~~~~^~~~~~~~ 
Где-то в коде сортировки оператор < сравнивает два документа со странными названиями *__it1 и *__it2. На звёздочки пока не обращайте внимания. Важна фраза no match for 'operator<' (operand types are 'Document' and 'Document'): не найден оператор < для объектов Document и Document.
Для сортировки документов достаточно уточнить, в каком случае считается, что один из них меньше другого. После сортировки меньшие документы будут в начале, а бóльшие — в конце.
В коде ниже — функция-компаратор, которая принимает два документа по константной ссылке. Первый документ называется lhs, а второй rhs. Функция возвращает true, если lhs меньше rhs: 
bool HasDocumentLessId(const Document& lhs, const Document& rhs) {
        return lhs.id < rhs.id;
} 
Название у функции говорящее: «правда ли id документа меньше». Теперь, чтобы  отсортировать документы по возрастанию id, нужно вызвать функцию sort, и передать в неё новый аргумент — функцию-компаратор:
// Теперь sort будет использовать функцию HasDocumentLessId,
// чтобы узнать, как нужно упорядочивать документы
sort(documents.begin(), documents.end(), HasDocumentLessId); 
Строго говоря, сортировка будет не по возрастанию, а по неубыванию. Некоторые документы могут иметь одинаковый id. После сортировки они будут расположены в произвольном порядке. Подумайте, важно ли это, когда сортируете только по одному из полей. Сортировка по нескольким полям будет рассмотрена позже.
Допустим, вы поменяли < на > в функции-компараторе: return lhs.id > rhs.id;. Тогда произойдёт сортировка по убыванию — точнее, по невозрастанию. Меньшими будут считаться документы с бо́льшим id, поэтому они окажутся в начале вектора. Но смысл функции поменяется — значит, ей нужно другое название, например HasDocumentGreaterId.