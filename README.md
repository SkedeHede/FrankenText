# FrankenText

Et C-program der analyserer og gendanner tekst baseret på et indlæst bog-dokument.  
Programmet opdeler teksten i ord (tokens), registrerer hvilke ord der følger efter hinanden,  
og kan derefter generere tilfældige sætninger ud fra mønstrene i originalteksten.

## Brug
```bash
gcc -std=c11 -Wall -Wextra -O0 -g FrankenText.c -o FrankenText
./FrankenText
```

Programmet:

- Indlæser bogen fra en tekstfil (defineret i load_book()).

- Erstatter ikke-printbare tegn med mellemrum.

- Deler teksten op i ord vha. tokenize_and_fill_succs().

- Finder sandsynlige efterfølger-relationer mellem ordene.

- Genererer derefter to tilfældige sætninger:

- - Én der slutter med ?

- - Én der slutter med !

Filer:

- FrankenText.c – Hele programmet (indlæsning, tokenisering, generering)

- pg84.txt – Bogen som teksten hentes fra