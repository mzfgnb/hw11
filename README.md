# Запуск

Если при запуске программы возникает ошибка с библиотеками, то необходимо в терминале перед запуском программы прописать команду

`export LD_LIBRARY_PATH=/opt/sfml2/lib:$LD_LIBRARY_PATH`

где вместо `/opt/sfml2/lib` должен стоять ваш путь с sfml2 библиотеками

# Компиляция:
  **Для sfml-2.x**
  
  ```g++ main.cpp -o ellipse -lsfml-graphics -lsfml-window -lsfml-system -std=c++17```

  **Для sfml-3.x**
  
  ```g++ main2.cpp -o ellipse -lsfml-graphics -lsfml-window -lsfml-system -std=c++17```
  
