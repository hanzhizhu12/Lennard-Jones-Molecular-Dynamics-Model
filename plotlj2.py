import numpy as np
import matplotlib.pyplot as plt
import sys

data = np.loadtxt("lj2.dat")

t  = data[:, 0]
r  = data[:, 1]
KE = data[:, 2]
U  = data[:, 3]
E  = data[:, 4]

fig, (ax1, ax2, ax3) = plt.subplots(3, 1, sharex=True, figsize=(8, 8))

ax1.plot(t, r)
ax1.axhline(2**(1/6), linestyle="--", color="gray", label="r_min = 2^(1/6)")
ax1.set_ylabel("r")
ax1.legend()

ax2.plot(t, KE, label="KE")
ax2.plot(t, U, label="U")
ax2.set_ylabel("energy")
ax2.legend()

ax3.plot(t, E)
ax3.set_ylabel("E total")
ax3.set_xlabel("t")

plt.tight_layout()
plt.savefig("lj2.png")
plt.show()
if "--show" in sys.argv:
    plt.show()