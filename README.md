## Requirements

Before building the project, make sure you have the following installed:

- Visual Studio (2022 or later)
- CMake (Latest release)
- Project Chrono (7.0.3)

## Installation links

CMake: https://cmake.org/download/
Project Chrono: https://github.com/projectchrono/chrono/releases

### 1. Create the build folder

Inside the `ChronoProject_GeneticAlgorithm` directory, create a folder named:

```text
build
```

### 2. Configure the Project with CMake

Open CMake and set:

#### Where is the source code

Select:

```text
.../ChronoProject_GeneticAlgorithm
```

#### Where to build the binaries

Select:

```text
.../ChronoProject_GeneticAlgorithm/build
```

### 3. Configure Chrono_DIR

If `Chrono_DIR` is not automatically detected, set it manually to:

```text
.../chrono-build/cmake
```

### 4. Generate the Visual Studio Solution

Press **Configure** and then **Generate**.

CMake will generate:

```text
ChronoProject_GeneticAlgorithm.sln
```

### 5. Open & Run the Solution

Open the generated 'sln' file in Visual Studio.

In Solution Explorer:
- Set the project to run on the Release version
- Right-click on 'ChronoProject_FinalGA'
- Select ***Select as startup project***

### 6. Run the project
Compile and run the solution (Press 'F5')

### DLL Troubleshooting

If some DLLs do not get copied correctly, do the following:

- Right-click the solution file and select 'Properties'
- Select 'Project Dependencies'
- On the 'Projects' tab, press ***ALL_BUILD*** and select ***ChronoProject_FinalGA***
- Tick the 'COPY_DLLS' checkbox
- Press 'Apply' then 'OK'
