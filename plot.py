import numpy as np
import matplotlib
import sys
if "--show" not in sys.argv:
    matplotlib.use("Agg")        
import matplotlib.pyplot as plt
 
t, x_e, v_e, E_e = np.loadtxt("euler.dat", unpack=True)
_, x_v, v_v, E_v = np.loadtxt("verlet.dat", unpack=True)
exact = np.cos(t)                  # x(0)=1, v(0)=0, k=m=1
 
fig, axs = plt.subplots(1, 3, figsize=(15, 6))
 
#position vs exact position
ax = axs[0]
ax.plot(t, x_e, label="Euler", alpha=0.8)
ax.plot(t, x_v, label="Verlet", alpha=0.8)
ax.plot(t, exact, "k--", label="exact cos(t)", lw=1)
ax.set_xlim(0, 100); ax.set_xlabel("time"); ax.set_ylabel("x")
ax.set_title("Position"); ax.legend()
 
#energy conservatoion
ax = axs[1]
ax.plot(t, E_e, label="Euler")
ax.plot(t, E_v, label="Verlet")
ax.axhline(0.5, color="k", ls="--", lw=1, label="exact (0.5)")
ax.set_xlabel("time"); ax.set_ylabel("total energy")
ax.set_title("Energy conservation"); ax.legend()
 
# position error log scale
ax = axs[2]
ax.semilogy(t, np.abs(x_e - exact) + 1e-16, label="Euler")
ax.semilogy(t, np.abs(x_v - exact) + 1e-16, label="Verlet")
ax.set_xlabel("time"); ax.set_ylabel("|x - cos(t)|")
ax.set_title("Position error (log scale)"); ax.legend()
 
plt.tight_layout()
plt.savefig("oscillator.png", dpi=120)
print(f"Euler  final energy: {E_e[-1]:.4f}  (start 0.5000)")
print(f"Verlet energy range: {E_v.min():.6f} to {E_v.max():.6f}")
print("Saved oscillator.png")
if "--show" in sys.argv:
    plt.show()


