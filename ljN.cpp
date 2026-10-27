#include "vec3.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>

const double epsilon = 1.0;
const double sigma = 1.0;
const double dt = 0.005;
const double tmax = 100;
const std::size_t step_count = static_cast<std::size_t>(std::round(tmax / dt));

const std::size_t N = 3;
const std::size_t quantizer = 1;

struct System {
    std::vector<Vec3> pos;
    std::vector<Vec3> vel;
    std::vector<Vec3> force;
    std::vector<double> mass;
};

double compute_forces(System& sys) {
    std::fill(sys.force.begin(), sys.force.end(), Vec3{0.0, 0.0, 0.0});

    double U = 0.0;

    for (std::size_t i = 0; i < sys.pos.size(); ++ i) {
        for (std::size_t j = i + 1; j < sys.pos.size(); ++j) {
            Vec3 rij = sys.pos[i] - sys.pos[j];
            double r2 = norm2(rij);
            double s = sigma*sigma / r2;
            double s3 = s * s * s;
            double s6 = s3 * s3;
            double coeff = (24*epsilon / r2) * (2*s6 - s3);
            Vec3 fij = coeff * rij;

            sys.force[i] += fij;
            sys.force[j] -= fij;

            U += 4 * epsilon * (s6 - s3);

        }
    }

    return U;
}

double kinetic_energy(const System& sys) {
    double KE = 0.0;
    for (std::size_t i = 0; i< sys.pos.size(); ++i) {
        KE += 0.5 * sys.mass[i] * dot(sys.vel[i], sys.vel[i]);
    }
    return KE;
}

double temperature(const System& sys, double KE) {
    double T = (2 * KE) / (3 * sys.pos.size());
    return T;
}

void write_xyz(std::ofstream& out, const System& sys, double t) {
    out << sys.pos.size() << "\n";
    out << "t = " << t << "\n";
    for (std::size_t i = 0; i < sys.pos.size(); ++i) {
        out << "Ar" << " " << sys.pos[i].x << " " << sys.pos[i].y << " " << sys.pos[i].z << "\n";
    }
}

int main() {
    System sys;
    sys.pos.resize(N);
    sys.pos[0] = {0.0, 0.0, 0.0};
    sys.pos[1] = {1.2, 0.0, 0.0};
    sys.pos[2] = {0.61, 1.08, 0.0};

    sys.vel.resize(N);
    sys.force.resize(N);
    sys.mass.resize(N, 1.0);
    
    double U0 = compute_forces(sys);
    double KE0 = kinetic_energy(sys);
    double T0 = temperature(sys, KE0);

    //sanity check
    double F0 = sys.force[0].x;
    double pos0 = sys.pos[0].x;
    const double h = 1e-6;
    sys.pos[0].x += h;
    double Uplush = compute_forces(sys);
    sys.pos[0].x = pos0;
    sys.pos[0].x -= h;
    double Uminush = compute_forces(sys);
    double derivU = - ((Uplush - Uminush) / (2 * h));
    std::cout << "Derivative - Analytic Force:" << derivU - F0 << " "<< "Derivative:" << derivU << " " << "Analytic:" << F0 << "\n";
    
    sys.pos[0].x = pos0;
    compute_forces(sys);

    std::ofstream energy_out("ljN.dat");
    energy_out << "# t KE U E T\n";
    energy_out << 0.0 << " " << KE0 << " " << U0 << " " << U0 + KE0 << " " << T0 << "\n";

    std::ofstream xyz_out("ljN.xyz");
    write_xyz(xyz_out, sys, 0.0);
    for (std::size_t step = 0; step < step_count; ++step) {

        for (std::size_t i = 0; i < sys.pos.size(); ++i) {
            sys.vel[i] += 0.5 * dt * sys.force[i] / sys.mass[i];
            sys.pos[i] += dt * sys.vel[i];
        }
        double U = compute_forces(sys);

        for (std::size_t i = 0; i < sys.pos.size(); ++i) {
            sys.vel[i] += 0.5 * dt * sys.force[i] / sys.mass[i];
        }

        double KE = kinetic_energy(sys);
        double T = temperature(sys, KE);

        if ((step + 1) % quantizer == 0) {
            energy_out << (step + 1) * dt << " " << KE << " " << U << " " << U + KE << " " << T << "\n";
       
            write_xyz(xyz_out, sys, (step + 1) * dt);
        }
    }
}