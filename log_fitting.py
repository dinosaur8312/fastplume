import numpy as np
import pandas as pd
from scipy.stats import linregress

# Load the CSV file into the 'data' variable
csv_file_path = 'hpac_dispersion_coefs.csv'  # Replace with the actual path to your CSV file
data = pd.read_csv(csv_file_path)

# Define parameters for maximum dimensions again for completeness
max_istab = 7
max_wind = 6
max_elements = 14  # Maximum number of elements expected

# Reset header file content
header_file_content = "#pragma once\n#include <array>\n\n"

# Initialize 3D arrays for ln_sig_x, ln_sig_y, ln_sig_z
ln_sig_x = np.full((max_istab, max_wind, max_elements), np.nan)  # Initialize with NaN for non-existing values
ln_sig_y = np.full((max_istab, max_wind, max_elements), np.nan)
ln_sig_z = np.full((max_istab, max_wind, max_elements), np.nan)

# Create containers for slopes and intercepts
slope_sig_x = np.full((max_istab, max_wind), np.nan)
intercept_sig_x = np.full((max_istab, max_wind), np.nan)
slope_sig_y = np.full((max_istab, max_wind), np.nan)
intercept_sig_y = np.full((max_istab, max_wind), np.nan)
slope_sig_z = np.full((max_istab, max_wind), np.nan)
intercept_sig_z = np.full((max_istab, max_wind), np.nan)

# x values should be the same across all istab/wind combinations
x_values = sorted(data['x'].unique())
ln_x_values = np.log(x_values[:max_elements])  # Trim x_values to max_elements

# Add ln(x) to the header file
header_file_content += f"// ln(x) values (shared across all istab/wind combinations)\n"
header_file_content += f"constexpr std::array<float, {len(ln_x_values)}> ln_x = {{ {', '.join(map(str, ln_x_values))} }};\n\n"

# Process each istab and wind combination
unique_istab = sorted(data['istab'].unique())
for istab in unique_istab:
    istab_data = data[data['istab'] == istab]
    unique_wind = sorted(istab_data['wind'].unique())

    for wind_index, wind in enumerate(unique_wind):
        wind_data = istab_data[istab_data['wind'] == wind]

        # Extract sig_x, sig_y, sig_z values
        sig_x_values = wind_data['sig_x'].tolist()[:max_elements]  # Trim to max_elements
        sig_y_values = wind_data['sig_y'].tolist()[:max_elements]  # Trim to max_elements
        sig_z_values = wind_data['sig_z'].tolist()[:max_elements]  # Trim to max_elements

        # Compute ln(sig_x), ln(sig_y), ln(sig_z)
        ln_sig_x_values = np.log(sig_x_values)
        ln_sig_y_values = np.log(sig_y_values)
        ln_sig_z_values = np.log(sig_z_values)

        # Store the values in the respective 3D arrays
        ln_sig_x[istab, wind_index, :len(ln_sig_x_values)] = ln_sig_x_values
        ln_sig_y[istab, wind_index, :len(ln_sig_y_values)] = ln_sig_y_values
        ln_sig_z[istab, wind_index, :len(ln_sig_z_values)] = ln_sig_z_values

        # Perform least square fitting between ln_x and ln_sig_x, ln_sig_y, ln_sig_z
        slope_x, intercept_x, _, _, _ = linregress(ln_x_values[:len(ln_sig_x_values)], ln_sig_x_values)
        slope_y, intercept_y, _, _, _ = linregress(ln_x_values[:len(ln_sig_y_values)], ln_sig_y_values)
        slope_z, intercept_z, _, _, _ = linregress(ln_x_values[:len(ln_sig_z_values)], ln_sig_z_values)

        # Store the slope and intercept
        slope_sig_x[istab, wind_index] = slope_x
        intercept_sig_x[istab, wind_index] = intercept_x
        slope_sig_y[istab, wind_index] = slope_y
        intercept_sig_y[istab, wind_index] = intercept_y
        slope_sig_z[istab, wind_index] = slope_z
        intercept_sig_z[istab, wind_index] = intercept_z

# Generate 3D arrays for ln_sig_x, ln_sig_y, ln_sig_z
header_file_content += f"// 3D arrays for ln(sig_x), ln(sig_y), ln(sig_z)\n"
header_file_content += f"constexpr std::array<std::array<std::array<float, {max_elements}>, {max_wind}>, {max_istab}> ln_sig_x = {{\n"
for istab in range(max_istab):
    header_file_content += "    {\n"
    for wind in range(max_wind):
        header_file_content += f"        {{{', '.join(map(str, ln_sig_x[istab, wind]))}}},\n"
    header_file_content += "    },\n"
header_file_content += "};\n\n"

header_file_content += f"constexpr std::array<std::array<std::array<float, {max_elements}>, {max_wind}>, {max_istab}> ln_sig_y = {{\n"
for istab in range(max_istab):
    header_file_content += "    {\n"
    for wind in range(max_wind):
        header_file_content += f"        {{{', '.join(map(str, ln_sig_y[istab, wind]))}}},\n"
    header_file_content += "    },\n"
header_file_content += "};\n\n"

header_file_content += f"constexpr std::array<std::array<std::array<float, {max_elements}>, {max_wind}>, {max_istab}> ln_sig_z = {{\n"
for istab in range(max_istab):
    header_file_content += "    {\n"
    for wind in range(max_wind):
        header_file_content += f"        {{{', '.join(map(str, ln_sig_z[istab, wind]))}}},\n"
    header_file_content += "    },\n"
header_file_content += "};\n\n"

# Generate slope and intercept arrays
header_file_content += f"// Slope and intercept arrays for least square fitting results\n"
header_file_content += f"constexpr std::array<std::array<float, {max_wind}>, {max_istab}> slope_sig_x = {{\n"
for istab in range(max_istab):
    header_file_content += f"    {{{', '.join(map(str, slope_sig_x[istab]))}}},\n"
header_file_content += "};\n\n"

header_file_content += f"constexpr std::array<std::array<float, {max_wind}>, {max_istab}> intercept_sig_x = {{\n"
for istab in range(max_istab):
    header_file_content += f"    {{{', '.join(map(str, intercept_sig_x[istab]))}}},\n"
header_file_content += "};\n\n"

header_file_content += f"constexpr std::array<std::array<float, {max_wind}>, {max_istab}> slope_sig_y = {{\n"
for istab in range(max_istab):
    header_file_content += f"    {{{', '.join(map(str, slope_sig_y[istab]))}}},\n"
header_file_content += "};\n\n"

header_file_content += f"constexpr std::array<std::array<float, {max_wind}>, {max_istab}> intercept_sig_y = {{\n"
for istab in range(max_istab):
    header_file_content += f"    {{{', '.join(map(str, intercept_sig_y[istab]))}}},\n"
header_file_content += "};\n\n"

header_file_content += f"constexpr std::array<std::array<float, {max_wind}>, {max_istab}> slope_sig_z = {{\n"
for istab in range(max_istab):
    header_file_content += f"    {{{', '.join(map(str, slope_sig_z[istab]))}}},\n"
header_file_content += "};\n\n"

header_file_content += f"constexpr std::array<std::array<float, {max_wind}>, {max_istab}> intercept_sig_z = {{\n"
for istab in range(max_istab):
    header_file_content += f"    {{{', '.join(map(str, intercept_sig_z[istab]))}}},\n"
header_file_content += "};\n"

# Save the header file
header_file_path_corrected = "dispersion_coefficients_corrected.h"
with open(header_file_path_corrected, 'w') as header_file:
    header_file.write(header_file_content)

header_file_path_corrected  # Return the path to the newly generated header file
