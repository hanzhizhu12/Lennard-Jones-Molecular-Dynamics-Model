#include "vec3.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>

const double epsilon = 1.0;
const double sigma = 1.0;
const double dt = 0.01;

struct System {
    std::vector<Vec3> pos;
    std::vector<Vec3> vel;
    std::vector<Vec3> force;
    std::vector<double> mass;
};

double compute_forces(System& sys) {
    std::fill(sys.force.begin(), sys.force.end(), Vec3{0.0, 0.0, 0.0});

    Vec3 rij = sys.pos[0] - sys.pos[1];
    double r2 = norm2(rij);
    double s = sigma*sigma / r2;
    double s3 = s * s * s;
    double s6 = s3 * s3;
    double coeff = (24*epsilon / r2) * (2*s6 - s3);
    Vec3 fij = coeff * rij;

    sys.force[0] += fij;
    sys.force[1] -= fij;

    return 4 * epsilon * (s6 - s3);
}

double kinetic_energy(const System& sys) {
    double KE = 0.0;
    for (std::size_t i = 0; i< sys.pos.size(); ++i) {
        KE += 0.5 * sys.mass[i] * dot(sys.vel[i], sys.vel[i]);
    }
    return KE;
}

int main() {
    System sys;
    sys.pos.push_back(Vec3{0, 0, 0});
    sys.pos.push_back(Vec3{1.2, 0.0, 0.0});
    sys.vel.push_back(Vec3{0.0, 0.0, 0.0});
    sys.vel.push_back(Vec3{0.0, 0.0, 0.0});
    sys.force.push_back(Vec3{0.0, 0.0, 0.0});
    sys.force.push_back(Vec3{0.0, 0.0, 0.0});
    sys.mass.push_back(1.0);
    sys.mass.push_back(1.0);
    
    double U0 = compute_forces(sys);
    double KE0 = kinetic_energy(sys);
    double r0 = std::sqrt(norm2(sys.pos[0] - sys.pos[1]));

    std::ofstream out("lj2.dat");
    out << "# t r KE U E\n";
    out << 0.0 << " " << r0 << " " << KE0 << " " << U0 << " " << U0 + KE0 << "\n";
    for (int step = 0; step < 10000; ++step) {

        for (std::size_t i = 0; i < sys.pos.size(); ++i) {
            sys.vel[i] += 0.5 * dt * sys.force[i] / sys.mass[i];
            sys.pos[i] += dt * sys.vel[i];
        }

        double U = compute_forces(sys); 

        for (std::size_t i = 0; i < sys.pos.size(); ++i) {
            sys.vel[i] += 0.5 * dt * sys.force[i] / sys.mass[i];
        }

        double KE = kinetic_energy(sys);

        if ((step + 1) % 10 == 0) {
            double r = std::sqrt(norm2(sys.pos[0] - sys.pos[1]));
            out << (step + 1) * dt << " " << r << " " << KE << " " << U << " " << U + KE << "\n";
        }
    }
}