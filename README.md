# Lennard-Jones-MD-Model

This starter project models molecular dynamics using C++ with a ground-up approach to quantitative chemistry and physics.

It simulates 2 to 4 particles interacting through the Lennard-Jones potential, integrated with velocity Verlet, in reduced units (σ = ε = m = 1).

## Requirements

- A C++17 compiler (`g++` or `clang++`)
- Python 3 with `numpy` and `matplotlib`

## Setup

From the project folder, create the Python environment once:

    python3 -m venv .venv
    source .venv/bin/activate
    pip install numpy matplotlib

`runljN.sh` activates the environment itself, so you don't need to activate it again.

## Running

    bash runljN.sh          # compile, run, save the plot
    bash runljN.sh --show   # same, and open the plot window

The program asks for two values:

| Prompt | Allowed values | Starting shape |
|---|---|---|
| Number of particles | 2, 3 or 4 | 2: dimer, 3: equilateral triangle, 4: regular tetrahedron |
| Side length `a` | any number > 0 | distance between every pair of particles |

At `a = 2^(1/6) ≈ 1.122462`, every pair sits at the bottom of the potential well, so the particles stay still. Any other value makes the shape oscillate.

The timestep (`dt = 0.005`) and run length (`tmax = 100`) are set at the top of `ljN.cpp`.

## Output

| File | Contents |
|---|---|
| `ljN.dat` | one row per step: `t KE U E T` |
| `ljN.xyz` | trajectory in XYZ format (opens in OVITO or VMD) |
| `ljN.png` | T, KE and U, total E, and E − E0 against t |

At startup the program also prints U₀ and a finite-difference check of every force component. Each difference should be about 1e-9 or smaller.

## Validation

| N | U₀ at a = 2^(1/6) | U₀ at a = 1.2 |
|---|---|---|
| 2 | −1 | −0.89096 |
| 3 | −3 | −2.6729 |
| 4 | −6 | −5.3458 |

In every run, E − E0 should stay in a narrow band with no drift. The width of the band scales as dt².

## Files

- `ljN.cpp`: the simulation (forces, integrator, setup, output)
- `vec3.h`: the 3D vector type and its operators
- `plotljN.py`: plots `ljN.dat`
- `runljN.sh`: builds and runs everything