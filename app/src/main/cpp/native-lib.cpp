#include <jni.h>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <thread>
#include <chrono>

extern "C" JNIEXPORT jstring JNICALL
Java_com_architect_r8forkmpplugin_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 20;
const int NUM_PARTICLES = 50;
const double TIME_STEP = 0.1;

class Particle {
public:
    double x, y;        // Position
    double vx, vy;      // Velocity

    Particle(double startX, double startY, double startVX, double startVY)
            : x(startX), y(startY), vx(startVX), vy(startVY) {}

    void update() {
        x += vx * TIME_STEP;
        y += vy * TIME_STEP;

        // Bounce off walls
        if (x <= 0 || x >= SCREEN_WIDTH - 1) {
            vx = -vx;
        }
        if (y <= 0 || y >= SCREEN_HEIGHT - 1) {
            vy = -vy;
        }

        // Keep within bounds
        x = std::max(0.0, std::min(x, static_cast<double>(SCREEN_WIDTH - 1)));
        y = std::max(0.0, std::min(y, static_cast<double>(SCREEN_HEIGHT - 1)));
    }
};

class ParticleSystem {
private:
    std::vector<Particle> particles;

public:
    ParticleSystem(int numParticles) {
        for (int i = 0; i < numParticles; ++i) {
            double x = rand() % SCREEN_WIDTH;
            double y = rand() % SCREEN_HEIGHT;
            double vx = (rand() % 10 - 5) / 10.0; // Random velocity in range [-0.5, 0.5]
            double vy = (rand() % 10 - 5) / 10.0;
            particles.emplace_back(x, y, vx, vy);
        }
    }

    void update() {
        for (auto& particle : particles) {
            particle.update();
        }
    }

    void render() const {
        char screen[SCREEN_HEIGHT][SCREEN_WIDTH] = {};

        // Clear screen
        for (int i = 0; i < SCREEN_HEIGHT; ++i) {
            for (int j = 0; j < SCREEN_WIDTH; ++j) {
                screen[i][j] = ' ';
            }
        }

        // Place particles
        for (const auto& particle : particles) {
            screen[static_cast<int>(particle.y)][static_cast<int>(particle.x)] = '*';
        }

        // Render to console
        for (int i = 0; i < SCREEN_HEIGHT; ++i) {
            for (int j = 0; j < SCREEN_WIDTH; ++j) {
                std::cout << screen[i][j];
            }
            std::cout << '\n';
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    ParticleSystem particleSystem(NUM_PARTICLES);

    while (true) {
        particleSystem.update();

        // Clear console
        std::system("clear"); // For Linux/macOS
        // std::system("cls"); // Uncomment this line for Windows

        particleSystem.render();

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Add delay
    }

    return 0;
}