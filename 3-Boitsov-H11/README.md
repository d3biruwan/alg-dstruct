Реализуйте представление линейных списков с помощью AVL­деревьев. Ваша структура данных должна
поддерживать следующие операции:
Поиск элемента по заданному ключу
Поиск k­го элемента по заданному k
Вставка элемента в определенное место
Удаление определенного элемента
Операцию сцепления двух списков.


Какова алгоритмическая сложность данных операций? В чем преимущества и в чем недостатки такого
подхода (сравните с представлением списков в виде массива, и в виде связного списка).
 

Сложность операций:  
Поиск по ключу: O(log(n))  
Поиск по данным: O(n)  
Добавление и удаление элемента: O(log(n))  


Операции добавления, удаления и поиска элементов в связном списке происходят за О(n). Единственная операция, которая имеет преимущество по времени
в случае массивов - это поиск по ключу, который выполняется за О(1).  Единственным недостатком представления списка в виде AVL дерева является наличие рекурсий, что
может увеличить время работы поиска элемента по данным или привести к переполнению стека.