# Kvíz app

A program kérdés-válasz párosításokat olvas be, a kérdést feltéve várja a megfelelő választ és rossz megoldás esetén kijavít, majd a végén kiértékelést ad a nyújtott teljesítményről.

## Input
A program `.txt`, `.csv` vagy `.tsv` kiterjesztésű fájlokat fogad el bemenetnek.

A fájl belső felépítése a következő formátumnak kell megfeleljen:
- Üres sorok megengedettek
- Egy sorban pontosan egy kérdés-válasz páros szerepelhet
- Soron belül a kérdést és a választ pontosan egy darab tabulátor (`\t`) választja el egymástól
- A tabulátortól balra szerepel a kérdés, jobbra pedig a várt válasz.

Példa egy helyes input fájlra:
`input.txt`

```
What is the capital of France?  Paris
Who wrote "Romeo and Juliet"?   William Shakespeare
What is the largest planet in our solar system? Jupiter
What is the chemical symbol for gold?   Au
Who painted the Mona Lisa?  Leonardo da Vinci
What is the tallest mountain in the world?  Mount Everest
What is the square root of 64?  8
What year did the Titanic sink? 1912
Who was the first president of the United States?   George Washington
What is the smallest country in the world?  Vatican City

```

## Indítás
A programot parancssorból lehet indítani a következőképpen:
```
quiz.exe <param1> <param2> ... <paramN>
```

Paraméternek megadható tetszőleges mennyiségű szövegfájl,  mappa, vagy akár mindkettő.


A megadott fájlokat és mappákat a program a fájlrendszerben vele egy szinten lévő `input` könyvtárban keresi.

Mappák megadása esetén a mappában lévő összes megfelelő kiterjesztéssel rendelkező fájl beolvasásra kerül.

Ha nincs paraméter megadva, akkor a teljes input könyvtár beolvasásra kerül.

## A program működése
A program egy beolvasott kérdés-válasz párt `struct` adatszerkezetben tárol,  az adatokat pedig egy dinamikusan allokált memóriaterületre tölti be.

Véletlenszerűen választ egyet az `N` kérdés közül, majd a választott elemet kicseréli az utolsóval. Ellenőrzi a megadott választ. Ezt követően megint választ egyet, de már csak az első `N-1` kérdés közül. És így tovább ezt addig ismétli, ameddig marad fel nem tett kérdés.

## Felhasználói élmény
A felhasználó két üzemmód közül választhat:
1. **Egyszeri végigkérdezés**: Miután minden kérdés fel lett egyszer téve, a program kiértékelést ad, majd kilép.
2. **Végtelenített kérdezés**: Ha kifogy a kérdésekből, akkor újrakezdi az egészet. Ilyenkor egy megadott kulcsszóra lehet leállítani a kérdéseket. Ezt követően kiértékelést ad, majd kilép.

## Kiértékelés
Megadja, hány kérdésre volt jó a válasz, illetve a találati arányt.