# project
programming project (lab1)

Обрані варіанти :
(****) Графи, дерева. Додавання та видалення елементів/вершин/ребер. Побудова кістякового дерева для заданого графу. 
+* за побудову мінімального кістякового дерева; 
Транспортні засоби. 
Транспортні засоби різних типів – наземні, водні, повітряні. Середовище, яким вони пересуваються (зокрема, з пунктами відправлення/призначення, дорогами, перешкодами тощо). 

16 класів, 19 полів : (Environment(4 поля), Transport(4 поля)->(LandTransport->(Car, Bus, Train), WaterTransport->(Ship), AirTransport->(Airplane)),Node(1 поле), Edge(3 поля),UnionFind(2 поля), Graph(2 поля),TreeNode(2 поля), Tree(1 поле)) 
* ->означає успадкування

Реалізовані структури графів підтримують роботу на різних типах даних, а саме : int, double, string, vector<int>, vector<string>. А також на елементах реалізованих класів з переліку 2. Реалізовано додавання та видалення елементів/вершин/ребер, побудова кістякового дерева для заданого графу та побудова мінімального кістякового дерева  за допомогою алгоритму Крускала.
demoTransportGraph демонструє роботу графа для транспорту, створюючи різні види транспорту (автомобіль, автобус, поїзд, літак, корабель) з їхніми характеристиками та середовищем.




  
