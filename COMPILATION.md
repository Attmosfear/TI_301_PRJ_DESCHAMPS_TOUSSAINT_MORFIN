# Instructions de Compilation

## ✅ Fichiers du projet

Vérifiez que vous avez tous les fichiers suivants :

**Fichiers source (.c)** :
- main.c
- utils.c
- hasse.c
- matrix.c

**Fichiers d'en-tête (.h)** :
- utils.h
- hasse.h
- matrix.h

**Fichiers de build** :
- CMakeLists.txt
- Makefile

---

## 🔨 Compilation sur Windows

### Méthode 1 : Visual Studio (MSVC)

1. **Ouvrir "Developer Command Prompt for VS"**

2. **Compiler** :
```cmd
cd "C:\Users\desch\Desktop\Programmation\PROJET_C_P2\TI_301_PRJ_DESCHAMPS_TOUSSAINT_MORFIN"

cl /W4 /std:c11 /Fe:markov.exe main.c utils.c hasse.c matrix.c
```

3. **Exécuter** :
```cmd
markov.exe
```

### Méthode 2 : MinGW (GCC pour Windows)

1. **Installer MinGW** depuis https://www.mingw-w64.org/

2. **Compiler** :
```cmd
cd "C:\Users\desch\Desktop\Programmation\PROJET_C_P2\TI_301_PRJ_DESCHAMPS_TOUSSAINT_MORFIN"

gcc -Wall -Wextra -std=c11 main.c utils.c hasse.c matrix.c -o markov.exe -lm
```

3. **Exécuter** :
```cmd
markov.exe
```

### Méthode 3 : CMake

1. **Installer CMake** depuis https://cmake.org/download/

2. **Créer un dossier de build** :
```cmd
mkdir build
cd build
```

3. **Configurer et compiler** :
```cmd
cmake ..
cmake --build .
```

4. **Exécuter** :
```cmd
Debug\markov.exe
```

---

## 🐧 Compilation sur Linux/Mac

### Avec GCC

```bash
cd ~/TI_301_PRJ_DESCHAMPS_TOUSSAINT_MORFIN

gcc -Wall -Wextra -std=c11 main.c utils.c hasse.c matrix.c -o markov -lm

./markov
```

### Avec Make

```bash
make
./markov
```

### Avec CMake

```bash
mkdir build && cd build
cmake ..
cmake --build .
./markov
```

---

## ⚠️ Résolution des problèmes

### Erreur : "gcc not found"
- **Windows** : Installez MinGW ou utilisez Visual Studio
- **Linux** : `sudo apt install build-essential` (Ubuntu/Debian)
- **Mac** : Installez Xcode Command Line Tools

### Erreur : "Cannot find -lm"
- Sur Windows avec MSVC, la bibliothèque mathématique est incluse automatiquement
- Supprimez `-lm` de la commande

### Erreur : "Permission denied"
- **Linux/Mac** : Rendez l'exécutable exécutable avec `chmod +x markov`

---

## ✅ Vérification

Après compilation réussie, vous devriez voir :
- Un fichier `markov.exe` (Windows) ou `markov` (Linux/Mac)
- Aucun warning lors de la compilation

Lors de l'exécution :
```
========================================
  PROJET GRAPHES DE MARKOV - TI301
========================================

Nom du fichier à lire dans le dossier data :
```

---

## 📂 Structure attendue

```
TI_301_PRJ_DESCHAMPS_TOUSSAINT_MORFIN/
├── data/
│   ├── exemple1.txt
│   ├── exemple2.txt
│   └── ...
├── main.c
├── utils.c
├── hasse.c
├── matrix.c
├── utils.h
├── hasse.h
├── matrix.h
├── CMakeLists.txt
├── Makefile
└── README.md
```

Le programme cherchera les fichiers de données dans le dossier `data/`.

---

Bonne compilation ! 🎉
