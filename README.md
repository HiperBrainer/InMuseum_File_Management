
# InMuseum

It's an application to locate artistic works inside a museum.
## 🛠 Technologies
C++, ImGui ,Visual Studio...


## Detailed Description

The project consists of an index on a balanced search tree to facilitate the search for artworks in a museum archive. The objective of the application is that, when entering the name of the artwork, it returns in which area of the museum it is, which author and also provides a preview image of the work. In relation to the ORI discipline, the concept of a strongly linked secondary index was used, since we were able to discover the RRN of the record from a secondary key, and, from the RRN, search for the other information of the work in the museum's archive. Besides the standard libraries of the C++ language, we used the Dear ImGui library to be able to structure the graphic interface of the application. Basically the integration of the data structure of the index (AVL tree) with the code of the graphic interface (Dear ImGui) was performed. The directx 11 version of the library was chosen for our implementation. In the GUI code the screens and events were instantiated according to the button clicks. A simple login system was also made so that only those who have the password can add new works to the museum. It is worth mentioning that for the text input fields we have to relate them to character vectors and, every time a button is pressed, as a good practice, the value of this variable has to be erased so that the text in the input does not remain. Finally, something very important for the implementation of states in the application, for new screens to appear from actions, such as the buttons, it is necessary to involve the execution of these screens in a condition from a boolean variable, and this variable to have its value changed to true when the event occurs, and to false when the screen is closed.




## Demonstração
[![Click here to demonstration](https://img.youtube.com/vi/P8RyPgfwXqA/0.jpg)](https://www.youtube.com/watch?v=P8RyPgfwXqA)
## Authors

- [Brainer Sueverti de Campos](https://github.com/hiperbrainer)
- [Vitor Caligaris Figueira](https://github.com/vitorcf10)
- [Pedro Malandrin Klesse](https://github.com/Klesse)
- [Thiago Martins](https://github.com/thiago0003)

