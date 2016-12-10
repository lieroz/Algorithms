## Задача 2. Порядок обхода (3 балла)

Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N. Требуется построить бинарное дерево, заданное наивным порядком вставки. Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root. Рекурсия запрещена.

#### 2_3. Выведите элементы в порядке post-order (снизу вверх).

| in | out |
|----|-----|
| 3 | 1 3 2 |
| 2 1 3 | |
| 3 | 3 2 1 |
| 1 2 3 | |
| 3 | 2 1 3 |
| 3 1 2 | |