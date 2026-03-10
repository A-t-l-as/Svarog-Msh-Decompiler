# Svarog-Msh-Decompiler
A program for extracting .msh models from the KnightShift game to .obj format.

# Krótka instrukcja

Najpierw wprowadzamy nazwę pliku msh.<br>
Wybieramy opcje obrotu modelu czyli wprowadzamy "y".<br>
Wybieramy oś x czyli wprowadzamy "x"<br>
Wartość obrotu:<br>
Jedna jednostka obrotu czyli 1 oznacza po prostu 1 * (pi/2) czyli to będzie 90 stopni.<br>
Można wprowadzić nawet ułamek np 0.5 czyli to będzie oznaczało 0.5 * (pi/2) czyli to będzie pi/4 czyli 45 stopni.<br>

ZALECAM UŻYĆ WARTOŚCI -1 w przypadku osi obrotu x ponieważ to spowoduje, że model będzie stał pionowo tak jak powinien.<br>

No i zalecam na koniec użycie int czyli wprowadzamy "n".<br>
Gotowe!<br>

---
Program nie obsługuje particlesów ale potrafi je wykryć i je ominąć.<br>
Do exportu wszystkiego zaleca się użycie po prostu pliku .bat "Export_all.bat".<br>
---