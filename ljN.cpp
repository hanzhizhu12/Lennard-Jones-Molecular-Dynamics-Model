#include "vec3.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

const double epsilon = 1.0;
const double sigma = 1.0;
const double dt = 0.005;
const double tmax = 100;
const std::size_t step_count = static_cast<std::size_t>(std::round(tmax / dt));
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

void write_energy(std::ofstream& out, const System& sys, double U, double t) {
    double KE = kinetic_energy(sys);
    double T = temperature(sys, KE);
    out << t << " " << KE << " " << U << " " << U + KE << " " << T << "\n";
}

double step(System& sys, double dt) {
    for (std::size_t i = 0; i < sys.pos.size(); ++i) {
        sys.vel[i] += 0.5 * dt * sys.force[i] / sys.mass[i];
        sys.pos[i] += dt * sys.vel[i];
    }
    
    double U = compute_forces(sys);

    for (std::size_t i = 0; i < sys.pos.size(); ++i) {
        sys.vel[i] += 0.5 * dt * sys.force[i] / sys.mass[i];
    }
    return U;
}

double fd_check(System& sys, std::size_t i, const Vec3& d, double h) {
    compute_forces(sys);

    double F0 = dot(sys.force[i], d);
    Vec3 pos0 = sys.pos[i];

    sys.pos[i] += h * d;
    double Uplush = compute_forces(sys);
    sys.pos[i] = pos0;

    sys.pos[i] -= h * d;
    double Uminush = compute_forces(sys);

    double derivU = - ((Uplush - Uminush) / (2 * h));

    sys.pos[i] = pos0;
    compute_forces(sys);

    return derivU - F0;
}

void make_triangle(System& sys, double a) {
    sys.pos[0] = {0.0, 0.0, 0.0};
    sys.pos[1] = {a, 0.0, 0.0};
    sys.pos[2] = {a/2, (std::sqrt(3.0)/2) * a, 0.0};
}

void make_dimer(System& sys, double a) {
    sys.pos[0] = {0.0, 0.0, 0.0};
    sys.pos[1] = {a, 0.0, 0.0};
}

void make_tetrahedron(System& sys, double a) {
    sys.pos[0] = {0.0, 0.0, 0.0};
    sys.pos[1] = {a, 0.0, 0.0};
    sys.pos[2] = {a/2, (std::sqrt(3.0)/2) * a, 0.0};
    sys.pos[3] = {a/2, (std::sqrt(3.0))/6 * a, (std::sqrt(2.0/3.0)) * a};
}


int main() {
    System sys;

    std::size_t N = 0;
    std::cout << "Enter Number of Particles:" << "\n";
    std::cin >> N;

    if (!std::cin || N < 2 || N > 4) {
        std::cerr << "Error: Double Check N" << "\n";
        return 1;
    }

    sys.pos.resize(N);
    double a = 0;

    std::cout << "Enter a value for side length" << "\n";
    std::cin >> a;

    if (!std::cin) {
        std::cerr << "Error: no value inputted";
        return 1;
    }

    if (N == 2) {
        make_dimer(sys, a);
    }

    else if (N == 3) {
        make_triangle(sys, a);
    }

    else if (N == 4) {
        make_tetrahedron(sys, a);
    }

    /*if (argc < 2) {
        std::cerr << "No arguments inputted, ./ljN <a>" << "\n";
        return 1;
    } */
    
    /* double a = 0.5;
    try {
        a = std::stod(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "./ljN <side length a> error" << "\n";
        return 1;
    } */

    sys.vel.resize(N);
    sys.force.resize(N);
    sys.mass.resize(N, 1.0);
    
    double U0 = compute_forces(sys);
    std::cout << "U0: " << U0 << "\n";

    //sanity check
    const double h = 1e-6;
    const std::vector<Vec3> dirs = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},{0.0, 0.0, 1.0}};
    for (std::size_t i = 0; i < sys.pos.size(); ++i) {
        for (std::size_t j  = 0; j < dirs.size(); ++j){
            double difference = fd_check(sys, i, dirs[j], h);
            std::cout << "i:" <<  i << ", " << "j:" << j << ", " << "diff:" << difference << "\n";
        }
    }

    std::ofstream energy_out("ljN.dat");
    energy_out << std::setprecision(10);
    energy_out << "# t KE U E T\n";
    write_energy(energy_out, sys, U0, 0.0);

    std::ofstream xyz_out("ljN.xyz");
    write_xyz(xyz_out, sys, 0.0);


    for (std::size_t n = 0; n < step_count; ++n) {
        double U = step(sys, dt);

        if ((n + 1) % quantizer == 0) {
            double t = (n + 1) * dt;
            write_energy(energy_out, sys, U, t);
            write_xyz(xyz_out, sys, t);
        }
    }
}