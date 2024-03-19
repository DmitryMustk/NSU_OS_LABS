#!/bin/bash

# Создаем директорию
mkdir example_dir

# Переходим в созданную директорию
cd example_dir

# Цикл для создания файлов
for ((i=1; i<=100; i++))
do
    # Генерируем случайное имя файла
    filename="file_$i.txt"
    
    # Генерируем случайное содержимое файла
    content=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | head -c 1000)
    
    # Создаем файл и записываем в него содержимое
    echo "$content" > "$filename"
    
    # Выводим информацию о созданном файле (опционально)
    echo "Created file: $filename"
done

echo "Done."

