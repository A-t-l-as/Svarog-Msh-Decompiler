# EN:

## Svarog-Msh-Decompiler
A program for extracting `.msh` models from the **KnightShift** game to `.obj` format.
The latest version of the program requires **.NET 4.8** because it has been rewritten from **C++** to **C#**.

## How to use:
1. Double-click to launch the program.
2. Enter the path to the msh file.
3. Select the model rotation option by entering `y`.
4. Select the x-axis by entering `x`.
5. Rotation value:
	- One unit of rotation i.e. `1` simply means `1 * (pi/2)` i.e. it will be `90 degrees`.
	- You can even enter a fraction e.g. `0.5` i.e. it will mean `0.5 * (pi/2)`.
	- `0.5 * (pi/2) = pi/4 = 45 degrees`.
	- I recommend using a value of `-1` for the x axis of rotation, because this will make the model stand vertically as it should.
6. Done.

The program also works in ARGC&ARGV mode:<br>
`Svarog.exe <msh file path> r x -1`<br>

-> The program does not support particles but can detect them and bypass them.<br>
-> To export everything it is recommended to simply use the .bat file `_EXPORT_ALL.bat` or .ps1 file `_EXPORT_ALL.ps1`.<br>

---

# PL:

## Svarog-Msh-Decompiler
Program do exportowania modeli `.msh` z gry **KnightShift** do formatu `.obj`.
Najnowsza wersja programu wymaga **.NET 4.8** ponieważ została przepisana z **C++** do **C#**.

## Jak używać:
1. Włączamy program przez dwuklik.
2. Wprowadzamy ścieżkę do pliku msh.
3. Wybieramy opcję obrotu modelu czyli wprowadzamy `y`.
4. Wybieramy oś x czyli wprowadzamy `x`.
5. Wartość obrotu:
	- Jedna jednostka obrotu czyli `1` oznacza po prostu `1 * (pi/2)` czyli to będzie `90 stopni`.
	- Można wprowadzić nawet ułamek np `0.5` czyli to będzie oznaczało `0.5 * (pi/2)`.
	- `0.5 * (pi/2) = pi/4 = 45 stopni`.
	- Zalecam użyć wartości `-1` w przypadku osi obrotu x, ponieważ to spowoduje że model będzie stać pionowo tak jak powinien.
6. Gotowe.


Program działa również w trybie ARGC&ARGV:<br>
`Svarog.exe <scieżka do pliku msh> r x -1`<br>

-> Program nie obsługuje particlesów ale potrafi je wykryć i je ominąć.<br>
-> Do exportu wszystkiego zaleca się użycie po prostu pliku .bat `_EXPORT_ALL.bat` lub pliku .ps1 `_EXPORT_ALL.ps1`.<br>
