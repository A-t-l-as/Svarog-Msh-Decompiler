# EN:

## Svarog-Msh-Decompiler
A program for extracting .msh models from the KnightShift game to .obj format.<br>

1. First we enter the name of the msh file.
2. We choose the model rotation options ie enter "y".
3. we select the x-axis i.e. enter "x".
4. The rotation value:

One unit of rotation i.e. 1 simply means 1 * (pi/2) i.e. it will be 90 degrees.<br>
You can even enter a fraction e.g. 0.5 i.e. it will mean 0.5 * (pi/2). <br>

`0.5 * (pi/2) = pi/4 = 45 degrees.`

I recommend using a value of -1 for the x axis of rotation, because this will make the model stand vertically as it should.<br>

5. And finally you can use int which means you enter "n".
Done!<br>

The program also works in ARGC&ARGV mode:<br>
`MSHdecompiler.exe <nazwapliku.msh> r x -1 int`<br>

-> The program does not support particles but can detect them and bypass them.<br>
-> To export everything it is recommended to simply use the .bat file `_EXPORT_ALL.bat` or .ps1 file `_EXPORT_ALL.ps1`.<br>

---

# PL:

## Svarog-Msh-Decompiler
Program do exportowania modeli .msh z gry KnightShift do formatu .obj.

1. Najpierw wprowadzamy nazwę pliku msh.
2. Wybieramy opcje obrotu modelu czyli wprowadzamy "y".
3. Wybieramy oś x czyli wprowadzamy "x"
4. Wartość obrotu:

Jedna jednostka obrotu czyli 1 oznacza po prostu 1 * (pi/2) czyli to będzie 90 stopni.<br>
Można wprowadzić nawet ułamek np 0.5 czyli to będzie oznaczało 0.5 * (pi/2). <br>

`0.5 * (pi/2) = pi/4 = 45 stopni.`<br>

Zalecam użyć wartości -1 w przypadku osi obrotu x, ponieważ to spowoduje że model będzie stał pionowo tak jak powinien.<br>

5. No i koniec można użyć int czyli wprowadzamy "n".
Gotowe!<br>

Program działa również w trybie ARGC&ARGV:<br>
`MSHdecompiler.exe <nazwapliku.msh> r x -1 int`<br>

-> Program nie obsługuje particlesów ale potrafi je wykryć i je ominąć.<br>
-> Do exportu wszystkiego zaleca się użycie po prostu pliku .bat `_EXPORT_ALL.bat` lub pliku .ps1 `_EXPORT_ALL.ps1`.<br>
