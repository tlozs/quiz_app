# Quiz App

Das Programm liest Frage-Antwort-Paare ein. Es stellt die Frage und erwartet die korrekte Antwort. Falls eine falsche Antwort gegeben wird, korrigiert es sofort. Am Ende bekommt der Benutzer eine Auswertung seiner Ergebnisse.

## Eingabe
Das Programm arbeitet ausschließlich mit `.txt` oder `.tsv` Quelldateien.

Die innere Strukturierung der Datei soll folgendermaßen aufgebaut sein:
- Leere Zeilen sind erlaubt
- In einer Zeile muss sich genau ein Frage-Antwort-Paar befinden
- Weder die Frage noch die Antwort darf leer sein
- Die Frage und die Antwort sind mit genau einem Tabulatorzeichen (`\t`) zu trennen
- Links vom Tabulatorzeichen befindet sich die Frage, und rechts vom Tabulatorzeichen befindet sich die Antwort

Beispiel für eine gut aufgebaute Quelldatei:
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

## Starten
Das Programm kann über die Kommandozeile wie folgt gestartet werden:
```
quiz_app.exe <param1> <param2> ... <paramN>
```

Als Parameter sind beliebig viele Textdateien oder Ordner, sogar beide, anzugeben.

Die angegebenen Eingabedateien sucht das Programm im `input`-Ordner, der sich im gleichen Verzeichnis wie das Programm befindet.

Wenn ein Ordner als Eingabeparameter angegeben wird, werden alle geeigneten Dateien in diesem Ordner eingelesen.

Falls keine Eingabe spezifiziert wird, wird der gesamte `input`-Ordner eingelesen.

## Lauflogik des Programms
Das Programm lädt die eingelesenen Informationen in `struct` Datentypen. Diese Dateien werden im dynamisch allokierten Speicherplatz gespeichert.

Zufällig wird eine Frage von den ersten `N` (praktisch alle) Fragen ausgewählt und gestellt. Die benutzte Frage wird danach mit der letzten Frage getauscht, und die nächste Frage wird nur aus den ersten `N-1` Fragen ausgewählt. Dies wird wiederholt, bis es keine Fragen mehr gibt.

## Nutzererlebnis
Der Benutzer kann von vier verschiedenen Spielmodi wählen:
1. **Einzeldurchlauf**: Nachdem alle Fragen gestellt wurden, werden die Ergebnisse gezeigt und das Spiel endet. Eine umgekehrte Variante dieses Spielmodus ist auch verfügbar, bei der die Antworten als Fragen gestellt werden und die Frage als Antwort erwartet wird.
2. **Endloserlebnis**: Falls es keine Fragen mehr gibt, fängt es von vorne an. Es besteht die Möglichkeit, das Spiel durch ein spezifisches Stichwort zu beenden. Dann werden die Ergebnisse gezeigt und das Spiel endet. Dieser Spielmodus hat auch eine umgekehrte Variante.

Mit dem Befehl `!exit` kann das Quiz jederzeit abgebrochen werden.

## Auswertung
Als Ergebnis werden die Spielzeit, die Anzahl der gestellten Fragen und auch die Anzahl der richtig beantworteten Fragen angezeigt.

---

# Detailierter Aufbau des Programms

In jedem Fall enthalten die Header-Dateien die Gebrauchsanweisungen für die einzelnen Funktionen, d.h. eine detaillierte Dokumentation über die Funktionsweise, Parameter und Rückgabewerte.

## Dateistruktur

- `main.c`
    - Enthält die `main` Funktion, die das Programm startet und alle Hauptprozesse aufruft:
        ```c
        int main(int argc, char *argv[]) {
            srand(time(NULL));  // random seed
            clear_screen();     // Konsole leeren
            init_quiz();        // Speicherplatz allokieren
            
            read_all_input(argc, argv); // Einlesen
            shrink_quiz_size();         // Speicherplatzminimierung
            play_quiz();                // Spiel
            evaluate_quiz();            // Auswertung

            free_quiz();        // Speicherplatz freigeben
            return 0;
        }
        ```

- `comm.c`
    - Dient der Vereinfachung der Kommunikation mit dem Benutzer. Anstelle von `printf` wird eine eigene Nachrichtenanzeigefunktion mit verschiedenen Nachrichtentypen implementiert. Enthält die Menüanzeige, den Spielmoduswähler und den Tastenanschlagsbeobachter.

- `fs_read.c`
    - Verantwortlich für das Einlesen der Eingabedateien. Navigiert im Dateisystem, benachrichtigt den Benutzer bei Fehlern und fordert gegebenenfalls eine Intervention an.
        - Einige globale Konstanten:
        ```c
        const char * const input_root = "input";
        const char * const allowed_extensions[] = {".txt", ".tsv"};
        const int allowed_extensions_size = sizeof(allowed_extensions) / sizeof(allowed_extensions[0]);
        ```
    - Das Haupteinlesen:
        ```c
        void read_all_input(int argc, char *argv[]) {
            int i;

            // wenn kein Eingabeverzeichnis vorhanden ist, erstellen wir eines
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

            // wenn keine Parameter angegeben sind, lesen wir den gesamten Input-Ordner ein
            if (argc == 1) {
                print_message(WARNING, "No input parameters specified in the command line.");
                print_message(QUESTION, "Would you like to read all files in the input directory? (y/n) ");
                if (getchar_equals('y'))
                    try_read_input(path_join(input_root, NULL));
            }
            else // ohne Fehlerbehandlung wäre es nur so :)              
                for (i = 1; i < argc; i++)
                    // da wir relativ zum Eingabeverzeichnis suchen, fügen wir den gesuchten Pfad mit dem Eingabeverzeichnis zusammen
                    try_read_input(path_join(input_root, argv[i]));

            return;
        }
        ```
    - So versuchen wir, vom angegebenen Pfad zu lesen:
        ```c
        void try_read_input(char *input_path) {
            if (!inside_input_root(input_path)) {   // es wurde gesagt, dass wir aus dem input-Verzeichnis lesen              print_message(ERROR, "Input '%s' is not within the input directory.", input_path);
                print_message(ERROR, "Input '%s' is not within the input directory.", input_path);
            }
            // zuerst versuchen wir, die Eingabe als Datei zu lesen, und falls das nicht klappt, als Ordner
            else if (!try_read_file(input_path) && !try_read_folder(input_path)) {
                print_message(ERROR, "Could not read input path: '%s'", input_path);
            }

            free(input_path);   // Speicherplatz freigeben
            return;
        }
        ```

- `fs_utils.c`
    - Enthält kürzere Hilfsfunktionen: Zeichenkettenmanipulationen, Anforderungsüberprüfungen.

- `quiz.c`
    - Verwalten der Quiz-Datenbank und Implementierung der Quiz-Logik. Hier erfolgt die Speicherzuweisung und freigabe, das Hinzufügen neuer Fragen und die Steuerung der Hauptspielschleife.
    - Die Quiz-Datenbank:
        ```c
        typedef struct Quiz {
            QAPair **qas;   // die Liste der Frage-Antwort-Paare
            int size;       // die Anzahl der gespeicherten Daten
            int capacity;   // die Größe des für die Liste reservierten Speichers
        } Quiz;
        ```
    - Der Aufbau der einzelnen Daten ist äußerst einfach:
        ```c
        typedef struct QAPair {
            char *question; // die Frage im Speicher
            char *answer;   // die Antwort
        } QAPair;
        ```
    - Das Hinzufügen neuer Daten zur Datenbank erfolgt wie folgt:
        ```c
        int extend_quiz(QAPair *qa) {
            int success = 0;    // ob es erfolgreich war
            
            // es ist schwierig, nach nichts zu fragen
            if (qa != NULL) {
                // wenn wir mehr Platz brauchen
                if (quiz->size == quiz->capacity) {
                    quiz->capacity *= 2;
                    
                    quiz->qas = realloc(quiz->qas, quiz->capacity * sizeof(QAPair*));
                    if(quiz->qas == NULL)
                        print_message(FATAL, "Memory allocation failed.");
                }

                // wir haben schon genug Platz
                quiz->qas[quiz->size++] = qa;
                success = 1;
            }

            return success;
        }
        ```
    - Die Hauptspielschleife:
        ```c
        void play_quiz() {
            int aktuelle;    // der Index der zufällig ausgewählten Frage
            int exit = 0;   // ob eine Beendigungsanforderung eingegangen ist
            int range = quiz->size;
            if (range == 0)
                print_message(FATAL, "Could not read any data from the specified inputs.");

            gamemode_select();  // Auswahl des Spielmodus
            start_timer();      // Uhr startet


            // solange wir noch Fragen haben oder keine Beendigungsanforderung eingegangen ist
            while (range && !exit) {
                QAPair *qa = random_question(range, &current);  // wir wählen eine Frage aus
                exit = ask_and_correct_question(qa);            // Frage stellen und Antwort korrigieren
                swap_qas(current, --range);                     // wir tauschen mit der letzte Element

                // Im Endloserlebnis, wenn keine Fragen mehr übrig sind, werden alle Fragen wieder in den Pool zurückgelegt
                if ((gamemode == INFINITE || gamemode == INFINITE_REVERSED) && range == 0)
                    range = quiz->size;            
            }

            stop_timer();       // Uhr anhalten
            
            return;
        }
        ```

- `evaluation.c`
    - Verantwortlich für die Nachverfolgung und Bewertung der Benutzerleistung.
        - Hier wird die Frage gestellt und die Antwort korrigiert:
        ```c
        int ask_and_correct_question(QAPair *qa) {
            char user_answer[ANSWER_SIZE];  // hier wird die Antwort eingelesen
            char* question;         // das werden wir fragen
            char* correct_answer;   // die richtige Antwort
            int exit = 0;           // ob eine Beendigungsanforderung eingegangen ist

            // Im umgekehrten Spielmodus vertauschen wir die Frage und die Antwort
            if (gamemode == ONEROUNDER_REVERSED || gamemode == INFINITE_REVERSED) {
                question = qa->answer;
                correct_answer = qa->question;
            }
            else {
                question = qa->question;
                correct_answer = qa->answer;
            }

            // für die Ergebnisanzeige erforderlich (global)
            asked_questions++;

            // wir stellen die Frage und lesen die Antwort ein
            print_message(INFO, "%s ", question);
            fgets(user_answer, sizeof(user_answer), stdin);

            // wir entfernen das Enter-Zeichen am Ende
            user_answer[strcspn(user_answer, "\n")] = '\0';

            // wir erfassen die Beendigungsanforderung
            if (strcmp(user_answer, "!exit") == 0)
                exit = 1;
            // ob die Antwort richtig war
            else if (strcmp(user_answer, correct_answer) == 0)
                correct_answers++;
            // wenn nicht, hätte dies geschrieben werden sollen
            else
                print_message(INCORRECT, "%s", correct_answer);

            // ästhetischer Zeilenumbruch
            print_message(INFO, "");
            
            return exit;
        }
        ```