#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include "vec3.h"

struct Oscillator {
    Vec3 x, v, a;
    double k, m;

    Vec3 acceleration() const {
        return -k * x / m;
    }
    double energy() const {
        return 0.5 * m * dot(v, v) + 0.5 * k * dot(x, x);
    }
    void init_a() {
        a = acceleration();
    }
};

void step_verlet(Oscillator& osc, double dt) {
    osc.x += osc.v * dt + 0.5 * osc.a * dt * dt;
    Vec3 a_new = osc.acceleration();
    osc.v += 0.5 * (osc.a + a_new) * dt;
    osc.a = a_new;
}

void step_euler(Oscillator& osc, double dt) {
    osc.x += osc.v * dt;
    osc.v += osc.a * dt;
    osc.a = osc.acceleration();
}

void run(Oscillator osc, void(*stepper)(Oscillator&, double), double dt, int n_steps, const std::string& filename) {
    osc.init_a();
    std::ofstream out(filename);
    out << std::setprecision(10);
    if (!out) {
        std::cerr << "Error" << " " << "Could not open" << " " << filename << ".\n";
        return;
        }
    out << 0 << " " << osc.x.x << " " << osc.v.x << " " << osc.energy() << "\n";
    for (int i = 0; i < n_steps; ++i) {
        stepper(osc, dt);
        out << (i + 1) * dt << " " << osc.x.x << " " << osc.v.x << " " << osc.energy() << "\n";
    }
}

int main() {
    Oscillator osc{
        {1.0, 1.0, 1.0}, //x
        {0.0, 0.0, 0.0}, //v
        {0.0, 0.0, 0.0}, //a
        1.0, //k
        1.0 //m
    };
    run(osc, step_euler, 0.01, 10000, "euler.dat");
    run(osc, step_verlet, 0.01, 10000, "verlet.dat");
}