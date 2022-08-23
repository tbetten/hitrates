#include <iostream>
#include <array>
#include <cmath>
#include <random>
#include <map>
#include <iomanip>

struct Histogram
{
    double sigma;
    std::map<int, int> histogram;
};

struct Probs
{
    double sigma;
    int crit_miss;
    int miss;
    int hit;
    int crit_hit;
};

int main()
{
    constexpr auto sigmas = []()
    {
        std::array<double, 11>a{};
        for (int i{ 0 }; i <= 10; ++i)
        {
            a[i] = 11.0 / 6.0 - i / 6.0;
        }
        return a;
    }();

    std::vector<Histogram> histograms;
    std::vector<Probs> probs;
    std::random_device rd{};
    std::mt19937 engine(rd());
    std::normal_distribution<double> dist{};
    for (int j{ 0 }; j <= 10; ++j)
    {
        auto sigma = sigmas[j];
        Histogram h{};
        Probs p{};
        h.sigma = sigma;
        p.sigma = sigma;
        
        for (int i{ 0 }; i < 100000; ++i)
        {
            auto r = dist(engine) * sigma;
            ++h.histogram[std::round(r)];
            auto ar = std::abs(r);
            if (ar <= 0.05)
            {
                ++p.crit_hit;
            }
            if (ar > 0.05 && ar <= 0.75)
            {
                ++p.hit;
            }
            if (ar > 0.75 && ar <= 1.8)
            {
                ++p.miss;
            }
            if (ar > 1.8)
            {
                ++p.crit_miss;
            }
        }
        histograms.push_back(h);
        probs.push_back(p);
    }
    for (int i{ 0 }; i < histograms.size(); ++ i)
    {
        auto hp = histograms[i];
        std::cout << "skill level: " << i * 10 << "\n";
        std::cout << "sigma: " << hp.sigma << "\n";
        for (auto p : hp.histogram)
        {
            std::cout << std::setw(2)
                << p.first << ' ' << std::string(p.second / 2000, '*') << '\n';
        }
        std::cout << "\n\n";
    }

    for (int i{ 0 }; i < probs.size(); ++i)
    {
        auto p = probs[i];
        std::cout << "probabilities for skill level " << i * 10 <<"\n";
        std::cout << "sigma is " << p.sigma << "\n\n";
        std::cout << "crit hit: " << p.crit_hit / 1000.0 << "%\n";
        std::cout << "hit: " << p.hit / 1000.0 << "%\n";
        std::cout << "miss: " << p.miss / 1000.0 << "%\n";
        std::cout << "crit miss: " << p.crit_miss / 1000.0 << "%\n";
        std::cout << "\n\n";
    }
}