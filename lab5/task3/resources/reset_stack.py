# Открываем файл "stack.txt" для записи бинарных данных
with open("stack.txt", "wb") as file:
    # Записываем 1024*1024 нулей в файл
    file.write(b'\x00' * (1024 * 1024))

