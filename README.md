# Chessy

### Постановка задачи
Расставить n шахматных фигур (возможно, различных и разного цвета) на доску, размером m * m так, чтобы они

а) Не били друг друга. Если фигуры разного цвета, то белые бьют только чёрных и наоборот. Назовём это задачей о независимости.

б) Покрывали всё поле, то есть чтобы каждая клетка поля была атакована хотя бы одной фигурой. Считается, что клетка, на которой стоит какая-нибудь фигура, покрыта.
Также нет разделения на цвета. Назовём это задачей о доминировании.

Нужно вывести все возможные фундаментальные решения, то есть решения, не получающиеся из других поворотом, отражением и сменой цвета.

### Подход к решению
Разобьём задачу на три части.
* Задача о независимости с фигурами одного цвета
* Задача о независимости с фигурами разных цветов
* Задача о доминировании

Так как нужно перечислить все решения, то воспользуемся методом ветвей и границ или его вариацией. В частности, методом поиска с возвратом (backtracking).
На Википедии можно найти [Задачу о восьми ферзях](https://ru.wikipedia.org/wiki/Задача_о_восьми_ферзях). Это задача о независимости с фигурами одного цвета.
Но эту идею можно использовать во всех трёх случаях. Таким образом, нам нужен алгоритм поиска с возвратом, который мы будем "настраивать"
под конкретную задачу.

Так как одинаковые фигуры неотличимы, то их перестановка будет давать одно и то же решение. Поэтому мысленно пронумеруем поле слева направо,
сверху вниз. То есть если это обычная шахматная доска, то в первой строке будут числа от 0 до 7, во второй от 8 до 15 и т.д.
Тогда следующую одинаковую фигуру будем ставить в клетку с большим номером. Тогда мы не будем получать одинаковые решения.

Основная суть алгоритма:
* Берём новую фигуру
* Если эта та же самая фигура, что и на прошлом шаге, то будем ставить только в номера, большие предыдущего
* Пробуем её поставить на поле с учётом определённых условий
* Если получилось, то рекурсивно вызываем эту же функцию со следующей фигурой. Затем убираем поставленную фигуру
* В начале функции проверяем, если эта последняя фигура, то сохраняем это решение

Это реализовано в solver.recursive_solve(). "Настройка" алгоритма производится наследниками класса board. В нём определены
несколько основных функций: check_chessman() - проверка, можно ли поставить фигуру в данное место поля. set_chessman() - поставить
фигуру в данное место поля, unset_chessman() - снять фигуру, get_solution() - получить решение. Также в классе хранится
двумерный массив фигур - игровое поле.

#### Задача о независимости с фигурами одного цвета
За эту задачу отвечает класс colorless_independent_board.

Для ускорения операций заведём несколько массивов. Два массива длиной m, в которых будем хранить флаги, находится ли под атакой
i-я строка и столбец. А также два массива длиной 2 * m - 1, в которых будут флаги относительно диагоналей 45 градусов (далее - восходящяя) и
135 градусов (далее - нисходящая).

Проверка проходит так:
* Проверяем, занята ли текущая ячейка (это делается первым шагом во всех остальных наследниках board, поэтому дальше об этом не говорится).
Также проверяем не бьётся ли текущий  и строка, а также диагонали. Если что-нибудь выполнятся, то поставить сюда нельзя

Далее для задач о независимости если мы проверяем ячейки, по которым бьёт фигура, то сначала проверим, есть ли у нас такие фигуры. То есть
перед тем, как проверять диагонали от текущей ячейки, сначала проверим, а есть ли собственно ферзи или слоны.
* Если фигура ферзь или ладья, то последовательно проверяем все ячейки на текущей строке и столбце. Причём мы можем себе позволить проверять
с начальной ячейки строки/столбца до конечной (в следующей задаче это будет не так, аналогично с диагоналями). Если есть хоть одна фигура, то
поставить нельзя
* Аналогично если ферзь или слон, только проверяются диагонали
* Проверяем для короля, смотрим все клетки вокруг текущей. Если там есть король,
либо текущая фигура король и встречается любая другая, то поставить нельзя.
* Аналогично с конём, только провяются клетки, в которые может пойти конь с текущей позиции.
* Считается, что все пешки - белые. То есть бьют они "вниз". Поэтому проверяем,
если текущая фигура пешка, то не бьёт ли она фигуры, которые могут быть внизу. А также проверяем, не могут ли текущую клетку бить
пешки сверху. Если что-то выполняется, то поставить фигуру нельзя
* Если ничего из этого не выполнилось, то фигуру можно поставить

Постановка (снятие) фигуры куда проще. В игровое поле для текущих координат ставим фигуру (зануляем ячейку). Если фигура ферзь или ладья,
то устанавливаем (снимаем) соответствующие "прямые" флаги, если ферзь или слон, то устанавливаем (снимаем) соответствующие диагональные флаги.

Насчёт получение решения. Само решение - это класс solution. Это обёртка над хэш-функцией, которая по паре координат отображает в фигуру (пока примем, что это
просто беззнаковое число, размером один байт). Проходимся по игровому полю, добавляем непустые ячейки в solution.

#### Задача о независимости с фигурами разного цвета
За эту задачу отвечает класс independent_board.

Здесь уже не получится завести такие массивы, так как, например, перед белой ладьёй может стоять белая пешка, тогда все
последующие клетки открыты для чёрных фигур. По похожим причинам будем расставлять сначала все белые фигуры, а только потом чёрные.
Иначе, например, если мы будем ставить сначала белую ладью, а потом чёрного слона, то потеряем случай, когда их будет разделять
белая пешка, потому что слона нельзя будет поставить.

Проверка:
* Если фигура белая, то разрешаем поставить и выходим
* Проверяем ферзя и ладью. От текущей ячейки проходимся сначала вправо, затем влево, затем вверх, затем вниз (похожим образом работает с диагоналями,
если фигура ферзь или слон). Останавливаемся на каждом
этапе тогда, когда находим непустую фигуру на пути или вышли за пределы. Далее смотрим, как соотносится эта фигура и та, которую
пытаемся поставить. Если изначальная фигура - ферзь или ладья (обязательно чёрная фигура, потому что белые мы без проблем расставили),
а встреченная фигура белая. То поставить не можем. Или же если встретили белого ферзя или белую ладью, тогда так же нельзя поставить
* Похожим образом для других фигур. Если проверяем для фигуры f, то смотрим, либо встреченная фигура - это белая f, либо встреченная фигура
белая, а ту, которую надо поставить, чёрная f. Отличие только для пешек. Чёрные бьют наверх, а в целом всё то же самое.

Поставить и снять фигуру ещё проще, чем в предыдущей задаче. То же самое, только не надо устанавливать и снимать флаги.

Получение решение такое же

#### Задача о доминировании
За эту задачу отвечает класс dominant_board.

Здесь все немного по-другому, в отличие от предыдущих двух задач. Заведём переменную, в которой будем хранить количество непокрытых ячеек.
Изначально там будет m * m. Заведём также переменную, с потенциальным количеством ячеек, которые могут покрыть ещё не взятые фигуры.
Потенциальное в том смысле, что если, к примеру, ладья бьёт s ячеек, то две ладьи будут бить 2 * s. Для каждой фигуры это потенциальное
количество отдельно высчитывается в функции dominant_board.max_weight(). У пешки, например, значение 3 потому, что она занимает одну
клетку и бьёт две другие.

Проверка:
* Если потенциальное количество не меньше непокрытого, то считаем, что можно поставить (не забываем про то, что если текущая ячейка
занята, то поставить нельзя).

В этой задаче проверка до ужаса проста. Но зато постановка и снятие фигуры уравнивают сложность.

Итак, постановка (снятие) фигуры:
* Устанавливаем (обнуляем) ячейку поля
* Уменьшаем (увеличиваем) потенциал на величину, равному максимальному весу фигуры

В классе dominant_board есть ещё одно вспомогательное поле. Но оно уже хранит не фигуры, а счётчик покрытия, который изначально обнулён.
А так же четыре массива из первой задачи.
* Для всех ячеек, которые бьёт фигура (определяется аналогично первой задачи), вызываем некую функцию. В случае установки фигуры,
она проверяет значение в ячейке и если там ноль, декрементирует количество непокрытых ячеек, паралельно увеличивая значение ячейки.
В случае снятия фигуры, если там единица, то инкрементируем непокрытые ячейки, параллельно уменьшая значение ячейки. Обращение с
массивами как в первой задаче. Зачем они нужны? Дело в том, что если фигура дальнобойная, то при установке (снятии) фигуры на линию (с линии),
которую она бьёт, нарушится покрытие. Поэтому мы должны пройтись по проблемной линии и восстановить порядок.

Получаение решения:
Так как мы пользовались потенциальным покрытием, то когда расставим все фигуры, не факт, что задача будет решена. Поэтому
проходимся по всем ячейкам для поля покрытия. Если встретили нулевую, то решение неверное и игнорируем его. Иначе получаем решение
как в первой задаче.

### Подробности реализации
figure - это класс, который хранит в себе unsigned char. В нём первые семь бит - enum, связанный с типом фигуры, последний бит - цвет.

Программа начинается с функции io_interface.parse(), которая принимает входной файл и возвращает массив figures_count и enum, показывающий,
какую задачу надо решать. Массив - это количество каждого типа фигур. Всего 6 типов фигур, они расположены в таком порядке:
ферзь, ладья, слон, конь, король и пешка. Такой порядок позволяет отбрасывать больше ветвей при переборе. Если мы решаем задачу
с цветами, то в массиве сначала будет информация про белые фигуры, затем про чёрные, то есть 12 элементов. Иначе 6. Для
биекции между фигурами и индексами в этом массиве в классе figure есть методы figure_from_index() и figure_index().

Далее после отработки parse() вызывается solver.solve(), в который подставляется нужный наследний board в зависимости от возвращенного
перечисления из parse().

Класс board хранит в себе ещё один массив, размером как figures_count. Его i-й элемент равен сумме figures_count[j], j = 0,...i
Такой подход позволяет в классе solver абстрагироваться от этих массивов и работать только с номером фигуры, по которому затем
можно восстановить и саму фигуру.

Описанные решения задач дают не фундаментальные решения. Поэтому класс solution определяет функцию remove_duplicates().
В ней решения попарно сравниваются и проверяются на "равенство". Решения равны, если не получаются из других поворотом, отражением и сменой цвета.
Равенство определяется в функции equal(). В неё передаётся флаг, нужно ли сравнивать на смену цветов. Пока предположим, что не надо.
Существует 7 базовых модификаций: повороты на 90, 180 и 270 градусов; отражение по вертикали, горизонтали, по возврастающей и убывающей
диагонали. Поэтому заведём массив из семи элементов, в котором будут флаги, равны ли решения в плане i-й модификации. Далее
для каждой установленной фигуры из первого решения смотрим, есть ли во втором решении такая же фигура с учётом модицикации. Если нет, то
соответствующий флаг устанавливается в false и далее эта модификация игнорируется. Решения будут равны, если хотя бы один флаг останется true
(изначально все элементы true). Если же нужно проверять на цвета, то модификаций будет 15: старые + 7 старых со сменой цвета + 1 модификация,
в которой координаты не меняются, но меняется цвет. В остальном всё то же самое.

### Структуры данных
Класс solution содержит хэш-таблицу, потому что надо часто проверять наличие элемента.

Поле - двумерный массив, потому что нужен произвольный доступ к элементам. То же относится и к другим массивам
в программе.

Решения содержатся в классе vector, так как добавление n элементов происходит за O(n), что нас устраивает, а
также мы не будем хранить лишние указатели, как делали бы, используя list.

### Сложности
#### Сложность по памяти
Мы храним массив figures_count и другой такого же размера в классе board. Они либо 6, либо 12 элементов. Это O(1).
В классе board ещё хранится поле, на него тратим O(m * m). В зависимости от реализации наследника, можем дополнительно хранить
такое же поле, что потребует O(m * m), а так же четыре массива, которые в сумме будут давать O(m). Тем не менее основное количество
памяти уходит на хранение всех решений, с целью их дальшей обработки. Это займёт O(N), где N - количество решений.
Формально мы затратим O(m * m + N) памяти. (+ стек вызовов для рекурсии)
#### Сложность по времени
На обработку входных данные максимум можем затратить O(n) времени (хотя можно и за O(1), зависит от формата).

Сама задача NP-полная. Потому что задачу о "расстановке n ферзей на доску, размером n * n, чтобы они не били друг друга"
можно свести к задаче о точном покрытии, которая NP-полная. Из [Википедии](https://en.wikipedia.org/wiki/Eight_queens_puzzle):

`
Consider a matrix with one primary column for each of the n ranks of the board, one primary column for each of the n files,
and one secondary column for each of the 4n − 6 nontrivial diagonals of the board. The matrix has n2 rows: one for each possible
queen placement, and each row has a 1 in the columns corresponding to that square's rank, file, and diagonals and a 0 in all the other columns.
Then the n queens problem is equivalent to choosing a subset of the rows of this matrix such that every primary column has a 1 in
precisely one of the chosen rows and every secondary column has a 1 in at most one of the chosen rows; this is an example of a
generalized exact cover problem, of which sudoku is another example.
`
Задача о расстановке n ферзей является частным случаем, решаемым этой программой.

В теории в самом плохом случае может быть O((m * m)!) рекурсивных вызовов. Но на самом деле меньше, потому что этот результат, когда
n = m * m и все фигуры разные. Но такое может быть только при m < 4. На каждый вызов мы можем потратить O(m) времени, например, пройтись по клеткам,
которые бьёт ферзь. А для получения решений тратим O(m * m) на каждое. Затем попарно сравниваем решения, сравнений будет O(N * N), на каждое
сравнение потратим O(n) времени, так как надо сравнить все n фигур. Таким образом, потратим O(m * (m * m)! + N * m * m + n * N * N) времени.

### Использование
#### Инструкция по компилированию
```
mkdir build && cd build
cmake .. && make
```
Для этого должен быть установлен CMake версии не менее 2.8.2

Далее появится два исполняемых файла. tests запускает тесты, никаких аргументов не принимает. chessy - сама программа.
Должно использоваться так: chessy <входной_файл> <выходной_файл> [размер доски], где последний аргумент необязательный.

#### Формат входных и выходных файлов
На каждой строке входного файла должна быть буква, обозначающая фигуру. q - ферзь, r - ладья, b - слон, n - конь, k - король,
p - пешка (можно заглавными буквами). Перед фигурой может быть необязательная часть - 'C*', где вместо C стоит какое-то число, показывающее количество данной фигуры.
После фигуры через пробел задаётся цвет буквой w (белый) или b (чёрный), если это необходимо. Регулярное выражение для фигуры
следующее: '(\d+\*)?[qrbnkpQRBNKP]( [wb])?'. Если это задача о доминировании, то первая строка должна состоять из буквы 'd'

Выходной файл состоит из решений, разделённых пустой строкой. Решение представляет наглядное изображение доски, где на месте
пустой клетки стоит '-', а на месте фигуры заглавная буква, соответствующая ей. Если заданы цвета, то пустая клетка становится '- ',
а фигура представляется так же буквой, а затем буквой, обозначающей цвет.

Пример:
```
-Q--
---Q
Q---
--Q-
```