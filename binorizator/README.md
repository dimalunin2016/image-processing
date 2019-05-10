# Биноризация изображения
-

В качестве алгоритма был взят алгоритм Бредли (http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.420.7883&rep=rep1&type=pdf).

Тестовые изображения лежат в папке Dataset, результаты - в Result. 

Код выполнен на двух языках: на Python (файл `Binorization.ipynb`) и на С++ (binorizator.cpp).

Все результаты по анализу работы алгоритма представлены в ноутбуке `Binorization.ipynb`.

Для запуска кода на Python достаточно открыть ноутбук `Binorization.ipynb`.

Для запуска кода на C++:
 
на тестовых изображениях - вызвать `run.sh`

В общем случае: скачать OpenCV, а дальше 

```
g++ binorizator.cpp -o img-display `pkg-config --cflags --libs opencv` 
./img-display <Имя исходного файлв> <Куда сохранить результат>
```


