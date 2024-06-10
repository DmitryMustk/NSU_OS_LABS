with open("memory.txt", "wb") as file:
    file.write(b'\x00' * (1024 * 1024))

