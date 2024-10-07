#include <iostream>
#include <string>
#include <vector>
#include <functional>

// Template struct for IstabWindData
template <typename T>
struct IstabWindData {
    T wind;
    std::vector<T> x_values;
    std::vector<T> sig_x_values;
    std::vector<T> sig_y_values;
    std::vector<T> sig_z_values;

    IstabWindData(T wind_value) : wind(wind_value) {}

    void print_data() const {
        std::cout << "Wind: " << wind << "\n";
        std::cout << "x_values: ";
        for (T x : x_values) std::cout << x << " ";
        std::cout << "\nsig_x_values: ";
        for (T sx : sig_x_values) std::cout << sx << " ";
        std::cout << "\nsig_y_values: ";
        for (T sy : sig_y_values) std::cout << sy << " ";
        std::cout << "\nsig_z_values: ";
        for (T sz : sig_z_values) std::cout << sz << " ";
        std::cout << "\n\n";
    }
};

// Template struct for IstabData
template <typename T>
struct IstabData {
    int istab;
    std::vector<IstabWindData<T>> istab_wind_data;

    IstabData(int istab_value) : istab(istab_value) {}

    void add_wind_data(const IstabWindData<T>& wind_data) {
        istab_wind_data.push_back(wind_data);
    }

    void print_data() const {
        std::cout << "Istab: " << istab << "\n";
        for (const auto& wind_data : istab_wind_data) {
            wind_data.print_data();
        }
    }
};

// Template class for DispersionData
template <typename T>
class DispersionData {
private:
    std::vector<IstabData<T>> istab_data;

public:
    DispersionData() {
        for (int i = 0; i < 7; ++i) {
            istab_data.emplace_back(i);
        }
    }

    void add_istab_wind_data(int istab, const IstabWindData<T>& wind_data) {
        if (istab >= 0 && istab < istab_data.size()) {
            istab_data[istab].add_wind_data(wind_data);
        }
    }

    void print_data() const {
        for (const auto& data : istab_data) {
            data.print_data();
        }
    }
};

// Template function to process the data
template <typename T>
void process_data() {
    DispersionData<T> dispersion_data;

    IstabWindData<T> wind_data_0_23(static_cast<T>(2.3));
    wind_data_0_23.x_values = {static_cast<T>(10.0), static_cast<T>(100.0), static_cast<T>(1000.0)};
    wind_data_0_23.sig_x_values = {static_cast<T>(1.1), static_cast<T>(1.2), static_cast<T>(1.3)};
    wind_data_0_23.sig_y_values = {static_cast<T>(2.1), static_cast<T>(2.2), static_cast<T>(2.3)};
    wind_data_0_23.sig_z_values = {static_cast<T>(3.1), static_cast<T>(3.2), static_cast<T>(3.3)};
    dispersion_data.add_istab_wind_data(0, wind_data_0_23);

    dispersion_data.print_data();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [float|double]\n";
        return 1;
    }

    std::string precision = argv[1];

    // Use a lambda to select the appropriate function based on input
    if (precision == "float") {
        std::cout << "Processing data using float\n";
        process_data<float>();  // Single template function to process float
    } else if (precision == "double") {
        std::cout << "Processing data using double\n";
        process_data<double>();  // Single template function to process double
    } else {
        std::cerr << "Unknown precision type. Use 'float' or 'double'.\n";
        return 1;
    }

    return 0;
}
