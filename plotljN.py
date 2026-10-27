import numpy as np
import matplotlib.pyplot as plt
import sys

data = np.loadtxt("ljN.dat")

t  = data[:, 0]
KE  = data[:, 1]
U = data[:, 2]
E  = data[:, 3]
T  = data[:, 4]

fig1, (ax1, ax2, ax3) = plt.subplots(3, 1, sharex=True, figsize=(8, 8))
fig2, ax4 = plt.subplots(figsize=(8,4))

ax1.set_xlim(0, 3)


ax1.plot(t, T)
ax1.set_ylabel("T")

ax2.plot(t, KE, label="KE")
ax2.plot(t, U, label="U")
ax2.set_ylabel("energy")
ax2.legend()

ax3.plot(t, E)
ax3.set_ylabel("E total")
ax3.set_xlabel("t")

ax4.plot(t, E - E[0])
ax4.set_ylabel("E - E initial")
ax4.set_xlabel("t")

fig1.tight_layout()
fig1.savefig("lj2_1.png")
fig2.tight_layout()
fig2.savefig("lj2_2.png")
if "--show" in sys.argv:
    plt.show()