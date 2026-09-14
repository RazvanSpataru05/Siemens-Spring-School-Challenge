# Structural Optimization with a Genetic Algorithm

A genetic algorithm that removes material from a 3D block structure while keeping
it standing. Each individual is a bit vector over the cells of an
`OX x OY x OZ` grid; every candidate is rebuilt as a rigid-body structure,
simulated with [Project Chrono](https://projectchrono.org/), and scored on how
much material it sheds without exceeding an allowed stress.

Rendering is [Irrlicht](https://irrlicht.sourceforge.io/) (via `chrono_irrlicht`),
and the in-scene UI is [Dear ImGui](https://github.com/ocornut/imgui) 1.90.9 on the
DirectX 9 backend. You configure and launch a run from panels inside the 3D view,
then step through the per-epoch results.

> Built during the Siemens Spring School.

---

## Requirements

| | |
|---|---|
| OS | Windows (the ImGui DirectX 9 backend is not portable) |
| Compiler | Visual Studio 2022, with the *Desktop development with C++* workload |
| CMake | 3.21 or newer (ships with VS 2022) |
| Project Chrono | **7.0.3**, built with the `Irrlicht` module |

C++20 is required — the app runs the GA on a `std::jthread`.

Chrono itself needs Eigen 3.4 and Irrlicht 1.8.5; follow the
[Chrono install guide](https://api.projectchrono.org/tutorial_install_chrono.html)
and build it before building this project.

---

## Building

### 1. Locate your Chrono build

You need the folder containing `ChronoConfig.cmake`. For a Chrono **build tree**
that is:

```text
<chrono-build>/cmake
```

For an **installed** Chrono it is `<chrono-install>/lib/cmake`.

### 2. Configure and build

Point `CHRONO_DIR` at that folder and use the bundled preset:

```bash
cmake --preset windows-x64
```

```bash
cmake --build --preset release
```

If you would rather not set an environment variable, pass the path directly:

```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DChrono_DIR="C:/path/to/chrono-build/cmake"
```

Or copy `CMakeUserPresets.json.example` to `CMakeUserPresets.json` and hard-code
your path there — that file is gitignored, so it stays local to your machine.

Both `Debug` and `Release` work. The build copies `Irrlicht.dll` and the
`ChronoEngine*.dll` files for the configuration being built next to the
executable, so there is nothing to copy by hand.

### 3. Run

Open `build/ChronoProject_FinalGA.sln` and press **F5** — CMake already sets the
startup project.

Or launch the executable directly, from anywhere:

```bash
build/Release/ChronoProject_FinalGA.exe
```

The working directory does not matter. CMake compiles the project directory into
the binary as `PROJECT_DATA_DIR`, and `algorithm_settings.txt`, `initial_individual.txt`
and the generated `final_individual.txt` / `individual_values.csv` are all resolved
against it, so the app reads and writes the same files however it is started.

---

## Configuration

### `algorithm_settings.txt`

Read once at startup. Defines the problem; the GA parameters here are defaults
that the in-app panel can override.

| Key | Meaning |
|---|---|
| `OX_SIZE`, `OY_SIZE`, `OZ_SIZE` | Grid dimensions, in cells |
| `ELEMENT_SIZE` | Edge length of one cell, in metres |
| `NUMBER_OF_EPOCHS` | Generations to run |
| `NUMBER_OF_INDIVIDUALS` | Population size |
| `CROSSOVER_PROBABILITY`, `MUTATION_PROBABILITY` | Genetic operator rates |
| `MAXIM_STRESS` | Stress ceiling a structure may not exceed, in Pa |
| `YOUNG_MODULUS`, `POISSON_RATIO`, `DENSITY` | Material properties |

### `initial_individual.txt`

The starting structure: `OX_SIZE * OY_SIZE * OZ_SIZE` values of `0` or `1`, one
per cell, whitespace-separated. `1` means the cell is filled.

---

## Using the app

The ImGui panel lets you pick the population size, epoch count, operator
probabilities, and one of each strategy before starting a run:

- **Fitness** — Original, Efficiency Ratio, Exponential Penalty, Multi Objective
- **Selection** — Roulette Wheel, Tournament, Ranked, Stochastic Universal Sampling
- **Crossover** — Single Point, 3D Block, 2 Point Planar, Symmetry Forced

When the run finishes, the scene switches to a viewer that steps through the best
individual of each epoch alongside its removed-element count, peak stress, and
fitness.

### Controls

| Key | Action |
|---|---|
| `H` | Show/hide the UI panels |
| `R` | Reset the camera |
| `Space` | Toggle camera auto-rotation (results viewer) |
| `A` / `D` | Rotate the camera left/right |
| `Esc` | Close the window |

---

## Layout

```text
inc/, src/
├── GeneticAlgorithm/   Individual encoding, GA loop, helpers
├── Selection/          Selection strategies
├── Crossover/          Crossover strategies
├── FitnessFunctions/   Fitness functions
├── GraphicalObjects/   Building and body construction
├── Services/           Application wiring, settings, file I/O
├── System/             Irrlicht/ImGui setup and input handling
└── Imgui/              Vendored Dear ImGui 1.90.9 + DX9 backend
```

---

## Troubleshooting

**`Could not find Project Chrono (7.0.3) or its Irrlicht module`**
`Chrono_DIR` is unset or wrong. It must name the folder that *contains*
`ChronoConfig.cmake`, not the Chrono root.

**`the include path it records does not exist`**
`ChronoConfig.cmake` stores absolute paths captured when Chrono was configured.
If you moved the Chrono source or build tree afterwards, those paths dangle —
and `find_package` still reports success, so without this check the build fails
much later with dozens of *cannot open include file* errors. Either re-run CMake
on the Chrono tree where it now lives, or rewrite the stale prefix inside
`ChronoConfig.cmake`.

**The window opens but the structure is empty**
`initial_individual.txt` could not be read, or holds fewer values than the grid
needs. It must contain exactly `OX_SIZE * OY_SIZE * OZ_SIZE` values — short files
fail silently and the missing cells are read as empty.

**Missing `ChronoEngine.dll` / `Irrlicht.dll` at startup**
The post-build copy step did not run, or you moved the `.exe` away from the DLLs
beside it. Rebuild, or copy `Irrlicht.dll` and the `ChronoEngine*.dll` files from
`<chrono-build>/bin/<Config>` next to the executable. Never mix Debug and Release
DLLs.

---

## License

MIT — see [LICENSE](LICENSE).
