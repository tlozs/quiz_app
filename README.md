# Kvíz app

A program kérdés-válasz párosításokat olvas be, a kérdést feltéve várja a megfelelő választ és rossz megoldás esetén kijavít, majd a végén kiértékelést ad a nyújtott teljesítményről.

## Input
A program `.txt` vagy `.tsv` kiterjesztésű fájlokat fogad el bemenetnek.

A fájl belső felépítése a következő formátumnak kell megfeleljen:
- Üres sorok megengedettek
- Egy sorban pontosan egy kérdés-válasz páros szerepelhet
- Se a kérdés, se a válasz nem lehet üres
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
quiz_app.exe <param1> <param2> ... <paramN>
```

Paraméternek megadható tetszőleges mennyiségű szövegfájl,  mappa, vagy akár mindkettő.

A megadott fájlokat és mappákat a program a fájlrendszerben vele egy szinten lévő `input` könyvtárban keresi.

Mappák megadása esetén a mappában lévő összes megfelelő kiterjesztéssel rendelkező fájl beolvasásra kerül.

Ha nincs paraméter megadva, akkor a teljes input könyvtár beolvasásra kerül.

## A program működése
A program egy beolvasott kérdés-válasz párt `struct` adatszerkezetben tárol, az adatokat pedig egy dinamikusan allokált memóriaterületre tölti be.

Véletlenszerűen választ egyet az `N` kérdés közül, majd a választott elemet kicseréli az utolsóval. Ellenőrzi a megadott választ. Ezt követően megint választ egyet, de már csak az első `N-1` kérdés közül. És így tovább ezt addig ismétli, ameddig marad fel nem tett kérdés.

## Felhasználói élmény
A felhasználó négy üzemmód közül választhat:
1. **Egyszeri végigkérdezés**: Miután minden kérdés fel lett téve egyszer, a program kiértékelést ad, majd kilép. Elérhető ennek egy kifordított változata is, ahol a válaszokat teszi fel kérdésként, és a kérdést kell megadni válasznak.
2. **Végtelenített kérdezés**: Ha kifogy a kérdésekből, akkor újrakezdi az egészet. Ilyenkor egy megadott kulcsszóra lehet leállítani a kérdéseket. Ezt követően kiértékelést ad, majd kilép. Ennek a módnak is van kifordított változata.

Mindig lehetőség van a `!exit` parancs beírása esetén megszakítani a kvízt.

## Kiértékelés
Megadja, hogy milyen gyorsan sikerült megválaszolni a kérdéseket, hogy hány kérdésre volt jó a válasz, illetve a találati arányt.

---

# A program részletes felépítése

Minden esetben a header fileok tartalmazzák az egyes függvények használati instrukcióit, azaz részletes dokumentációt a működésről, paraméterekről és visszatérési értékről.

## Fájlstruktúra

- `main.c`
    - Tartalmazza a `main` függvényt, amely elindítja a programot és meghív minden fő folyamatot:
        ```c
        int main(int argc, char *argv[]) {
            srand(time(NULL));  // random seed
            clear_screen();     // konzol tisztítása
            init_quiz();        // memóriafoglalás
            
            read_all_input(argc, argv); // beolvasás
            shrink_quiz_size();         // memóriaterület minimalizálása
            play_quiz();                // játék
            evaluate_quiz();            // eredményhirdetés

            free_quiz();        // feltakarítás
            return 0;
        }
        ```

- `comm.c`
    - A felhasználóval való kommunikáció egyszerűsítésére szolgál. Printf helyett saját üzenetkiíró függvényt valósít meg különböző üzenettípusokkal, tartalmazza a menükiírást, játékmódválasztót, billentyűlenyomás figyelőt.

- `fs_read.c`
    - Az input fájlok beolvasásáért felel. Navigál a fájlrendszerben, hibák esetén értesíti a felhasználót, illetve beavatkozást kér.
    - Néhány globális konstans:
        ```c
        const char * const input_root = "input";
        const char * const allowed_extensions[] = {".txt", ".tsv"};
        const int allowed_extensions_size = sizeof(allowed_extensions) / sizeof(allowed_extensions[0]);
        ```
    - A fő beolvasás:
        ```c
        void read_all_input(int argc, char *argv[]) {
            int i;

            // ha nincs input mappa, csinálunk
            if (no_input_root()) {
                print_message(ERROR, "Input directory not found.");
                print_message(QUESTION, "Would you like to create the input directory? (y/n) ");

                if (getchar_equals('y') && create_input_root()) {
                    print_message(INFO, "Input directory created.");
                    print_message(INFO, "Place your input files in the input directory, then press any key to continue.");
                    print_message(QUESTION, "If there is no ANY key on your keyboard, consult your local keyboard vendor...");
                    getchar_equals(0);
                } 
                else {
                    print_message(FATAL, "Could not create input directory.");
                }
                print_message(INFO, "");
            }

            // ha nincs paraméter, beolvassuk a teljes input mappát
            if (argc == 1) {
                print_message(WARNING, "No input parameters specified in the command line.");
                print_message(QUESTION, "Would you like to read all files in the input directory? (y/n) ");
                if (getchar_equals('y'))
                    try_read_input(path_join(input_root, NULL));
            }
            else // hibakezelés nélkül csak ennyi lenne :)
                for (i = 1; i < argc; i++)
                    // mivel az inputhoz relatív keresünk, a keresett útvonalat összeollózzuk az input útvonalával
                    try_read_input(path_join(input_root, argv[i]));

            return;
        }
        ```
    - Így próbálunk beolvasni az adott útvonalról:
        ```c
        void try_read_input(char *input_path) {
            if (!inside_input_root(input_path)) {   // arról volt szó, az input mappából olvasunk be
                print_message(ERROR, "Input '%s' is not within the input directory.", input_path);
            }
            // előszőr fájlként, vagy ha nem megy, akkor mappaként próbáljuk meg beolvasni
            else if (!try_read_file(input_path) && !try_read_folder(input_path)) {
                print_message(ERROR, "Could not read input path: '%s'", input_path);
            }

            free(input_path);   // a path_join mallocol
            return;
        }
        ```

- `fs_utils.c`
    - Rövidebb segédfüggvényeket tartalmaz: stringmanipulációk, követelményellenőrzések.

- `quiz.c`
    - A kvíz adatbázisának kezelését végzi, valamint a kvíz logikáját valósítja meg. Itt történik meg a memória lefoglalása és felszabadítása, új kérdések rögzítése és a fő játékciklus vezérlése.
    - A kvíz adatbázisa:
        ```c
        typedef struct Quiz {
            QAPair **qas;   // a kérdés-válasz párok listája
            int size;       // a számontartott adatok száma
            int capacity;   // a lista számára lefoglalt memóra mérete
        } Quiz;
        ```
    - Az egyes adatok felépítése felettébb egyszerű:
        ```c
        typedef struct QAPair {
            char *question; // a kérdés helye a memóriában
            char *answer;   // a válasz
        } QAPair;
        ```
    - Új adat felvétele az adatbázisba a következőképp történik:
        ```c
        int extend_quiz(QAPair *qa) {
            int success = 0;    // sikerül-e a művelet
            
            // a semmit azért nehéz megkédezni
            if (qa != NULL) {
                // ha kifogytunk a helyből, nagyobb területet kérünk
                if (quiz->size == quiz->capacity) {
                    quiz->capacity *= 2;
                    
                    quiz->qas = realloc(quiz->qas, quiz->capacity * sizeof(QAPair*));
                    if(quiz->qas == NULL)
                        print_message(FATAL, "Memory allocation failed.");
                }

                // így már biztos van elég hely
                quiz->qas[quiz->size++] = qa;
                success = 1;
            }

            return success;
        }
        ```
    - A fő játékciklus:
        ```c
        void play_quiz() {
            int current;    // a random választott kérdés indexe
            int exit = 0;   // érkezett-e kilépési kérés
            int range = quiz->size;
            if (range == 0)
                print_message(FATAL, "Could not read any data from the specified inputs.");

            gamemode_select();  // a játékmód kiválasztása
            start_timer();      // óra indul

            // amíg ki nem fogyunk a kérdésből, vagy kilépési kérés nem érkezik
            while (range && !exit) {
                QAPair *qa = random_question(range, &current);  // választunk egyet
                exit = ask_and_correct_question(qa);            // megkérdezzük
                swap_qas(current, --range);                     // félrerakjuk a végére és csökkentjük a teret

                // Végtelenített mód esetén, ha kifogynánk a kérdésből, visszatesszük az összeset a kalapba
                if ((gamemode == INFINITE || gamemode == INFINITE_REVERSED) && range == 0)
                    range = quiz->size;            
            }

            stop_timer();       // óra leáll
            
            return;
        }
        ```
    
    
    

- `evaluation.c`
    - A felhasználó teljesítményének nyomonkövetéséért és értékeléséért felel. 
    - Itt teszi fel a kérdést és javítja ki a választ:
        ```c
        int ask_and_correct_question(QAPair *qa) {
            char user_answer[ANSWER_SIZE];  // ide olvassuk be a választ
            char* question;         // ezt fogjuk kérdezni
            char* correct_answer;   // a jó válasz
            int exit = 0;           // ki akar-e lépni

            // fordított játékmód esetén felcseréljük a kérdést és a választ
            if (gamemode == ONEROUNDER_REVERSED || gamemode == INFINITE_REVERSED) {
                question = qa->answer;
                correct_answer = qa->question;
            }
            else {
                question = qa->question;
                correct_answer = qa->answer;
            }

            // eredményhirdetéshez kell (globális)
            asked_questions++;

            // megkérdezzük, majd beolvassuk a választ
            print_message(INFO, "%s ", question);
            fgets(user_answer, sizeof(user_answer), stdin);

            // kiszedjük az entert a végéről
            user_answer[strcspn(user_answer, "\n")] = '\0';

            // rögzítjük a kilépési szándékot
            if (strcmp(user_answer, "!exit") == 0)
                exit = 1;
            // eltalálta-e
            else if (strcmp(user_answer, correct_answer) == 0)
                correct_answers++;
            // ha nem, ezt kellett volna írni
            else
                print_message(INCORRECT, "%s", correct_answer);

            // esztétikai sortörés
            print_message(INFO, "");
            
            return exit;
        }
        ```