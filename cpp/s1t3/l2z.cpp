/* УРОК 2. Компоратор сортировки
Задание
Внедрите лямбда-функцию в код метода FindTopDocuments вашей поисковой системы вместо функции HasDocumentGreaterRelevance. Других изменений в работе поисковой системы быть не должно.
Формат входных и выходных данных
Первая строка содержит стоп-слова, разделённые пробелом. Вторая строка содержит целое число N, задающее количество документов. Следующие N строк содержат текст документов, в которых слова разделяются одним пробелом. Затем идёт строка S поискового запроса.
Программа должна вывести информацию о топ-5 самых релевантных документов, соответствующих поисковому запросу S, отсортированных по убыванию релевантности, как показано в примерах ниже.
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
Ввод
a the on cat
6
a fat cat sat on a mat and ate a fat rat
fat rat
a fat cat rat
a fat cat sat
a fat cat
a fat dog
funny fat cat on a mat with rat 
Вывод
{ document_id = 0, relevance = 3 }
{ document_id = 1, relevance = 2 }
{ document_id = 2, relevance = 2 }
{ document_id = 3, relevance = 1 }
{ document_id = 4, relevance = 1 } 
Возможен и другой порядок вывода документов с одинаковой релевантностью, например:
{ document_id = 0, relevance = 3 }
{ document_id = 2, relevance = 2 }
{ document_id = 1, relevance = 2 }
{ document_id = 5, relevance = 1 }
{ document_id = 4, relevance = 1 } 
Как будет тестироваться ваш код
Кроме того, что тренажёр проверит работу поисковой системы, он также убедится, что вместо функции HasDocumentGreaterRelevance используется лямбда-функция.